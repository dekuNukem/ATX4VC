
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2022 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f0xx_hal.h"

/* USER CODE BEGIN Includes */
#include "neopixel.h"
#include "buttons.h"
#include <string.h>
#include "ee.h"
#include "animations.h"
#include <math.h>
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
IWDG_HandleTypeDef hiwdg;

SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim14;
TIM_HandleTypeDef htim17;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

#define EEPROM_BUF_SIZE 8
uint8_t eeprom_buf[EEPROM_BUF_SIZE];

const char boot_message[] = "ATX4VC\ndekuNukem 2022";
const uint8_t version_major = 0;
const uint8_t version_minor = 0;
const uint8_t version_patch = 1;
uint8_t is_soft_power_turned_on;
volatile uint32_t frame_interrupt_count;
uint8_t red_buf[NEOPIXEL_COUNT];
uint8_t green_buf[NEOPIXEL_COUNT];
uint8_t blue_buf[NEOPIXEL_COUNT];

hsvcolor global_hsv;
rgbcolor my_rgb;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_SPI1_Init(void);
static void MX_TIM17_Init(void);
static void MX_TIM14_Init(void);
static void MX_IWDG_Init(void);
static void MX_NVIC_Init(void);

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);
                                

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

int fputc(int ch, FILE *f)
{
  HAL_UART_Transmit(&huart2, (unsigned char *)&ch, 1, 100);
  return ch;
}

#define BREATHING_FRAME_COUNT 300

void animation_update(void)
{
  uint8_t current_animation = eeprom_buf[BUTTON_RGB_MODE] % ANIMATION_TYPE_COUNT;
  if(current_animation == ANIMATION_SOLID_COLOR)
  {
    my_rgb = hsv2rgb(global_hsv);
    memset(red_buf, my_rgb.r, NEOPIXEL_COUNT);
    memset(green_buf, my_rgb.g, NEOPIXEL_COUNT);
    memset(blue_buf, my_rgb.b, NEOPIXEL_COUNT);
  }
  else if(current_animation == ANIMATION_FLOWING_RAINBOW)
  {
    for (int i = 0; i < NEOPIXEL_COUNT; ++i)
    {
      hsvcolor this_hsv;
      this_hsv.h = i*12 + frame_interrupt_count;
      this_hsv.s = 255;
      this_hsv.v = global_hsv.v;
      my_rgb = hsv2rgb(this_hsv);
      red_buf[i] = my_rgb.r;
      green_buf[i] = my_rgb.g;
      blue_buf[i] = my_rgb.b;
    }
  }
  else if(current_animation == ANIMATION_CROSSFADE)
  {
    for (int i = 0; i < NEOPIXEL_COUNT; ++i)
    {
      hsvcolor this_hsv;
      this_hsv.h = frame_interrupt_count/2;
      this_hsv.s = 255;
      this_hsv.v = global_hsv.v;
      my_rgb = hsv2rgb(this_hsv);
      red_buf[i] = my_rgb.r;
      green_buf[i] = my_rgb.g;
      blue_buf[i] = my_rgb.b;
    }
  }
  else if(current_animation == ANIMATION_BREATHING)
  {
    uint8_t dim_minimum = global_hsv.v / 10;
    if(dim_minimum == 0)
      dim_minimum = 1;
    float stepping = (float)(global_hsv.v - dim_minimum) / (BREATHING_FRAME_COUNT / 2); // 1 second = 60 frames
    uint32_t current_breathing_frame = frame_interrupt_count % BREATHING_FRAME_COUNT;
    uint8_t current_breathing_brightness;

    if(current_breathing_frame < BREATHING_FRAME_COUNT / 2) // down
      current_breathing_brightness = lround(global_hsv.v - stepping * current_breathing_frame);
    else // up
      current_breathing_brightness = lround(global_hsv.v - stepping * (BREATHING_FRAME_COUNT - current_breathing_frame + 1));
    for (int i = 0; i < NEOPIXEL_COUNT; ++i)
    {
      hsvcolor this_hsv;
      this_hsv.h = global_hsv.h;
      this_hsv.s = global_hsv.s;
      if(global_hsv.v == 0) // led off
        this_hsv.v = 0;
      else
        this_hsv.v = current_breathing_brightness;
      my_rgb = hsv2rgb(this_hsv);
      red_buf[i] = my_rgb.r;
      green_buf[i] = my_rgb.g;
      blue_buf[i] = my_rgb.b;
    }
  }
  __disable_irq();
  neopixel_show(red_buf, green_buf, blue_buf);
  __enable_irq();
}

// happens every 16ms
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  frame_interrupt_count++;
  animation_update();
  // scan buttons every 64ms
  if(frame_interrupt_count % 4 == 0)
    scan_buttons();
}

#define FAN_PWM_FULL_POWER 401
#define FAN_SPEED_STEP_COUNT 8

const uint16_t fan_speend_lookup[FAN_SPEED_STEP_COUNT] = {0, 20*4, 40*4, 55*4, 65*4, 80*4, 90*4, FAN_PWM_FULL_POWER};

#define LED_COLOR_STEP_COUNT 27
void set_led_color(uint8_t button_status)
{
  uint8_t color_index = button_status % LED_COLOR_STEP_COUNT;
  if(color_index == LED_COLOR_STEP_COUNT-1) // color_index 26 = white
  {
    global_hsv.h = 0;
    global_hsv.s = 0;
  }
  else
  {
    global_hsv.h = color_index * 10; // 0, 10, 20, ..., 250
    global_hsv.s = 255;
  }
}

#define LED_BRIGHTNESS_STEP_COUNT 11
const uint8_t led_brightness_lookup[LED_BRIGHTNESS_STEP_COUNT] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
void set_led_brightness(uint8_t button_status)
{
  global_hsv.v = led_brightness_lookup[button_status % LED_BRIGHTNESS_STEP_COUNT] * 25; // change to 25 for full scale
}

void restore_button_settings(void)
{
  uint8_t fan_index = eeprom_buf[BUTTON_FANSPEED] % FAN_SPEED_STEP_COUNT;
  htim14.Instance->CCR1 = fan_speend_lookup[fan_index];

  // default value
  global_hsv.h = 127;
  global_hsv.s = 255;
  global_hsv.v = 8;
  set_led_color(eeprom_buf[BUTTON_COLOR]);
  set_led_brightness(eeprom_buf[BUTTON_BRIGHTNESS]);
}

void handle_button_press(uint8_t button_index)
{
  eeprom_buf[button_index]++;
  if(button_index == BUTTON_FANSPEED)
  {
    uint8_t fan_index = eeprom_buf[button_index] % FAN_SPEED_STEP_COUNT;
    htim14.Instance->CCR1 = fan_speend_lookup[fan_index];
  }
  else if(button_index == BUTTON_COLOR)
  {
    set_led_color(eeprom_buf[button_index]);
  }
  else if(button_index == BUTTON_BRIGHTNESS)
  {
    set_led_brightness(eeprom_buf[button_index]);
  }
  else if(button_index == BUTTON_POWER)
  {
    is_soft_power_turned_on = (is_soft_power_turned_on + 1) % 2;
    HAL_GPIO_WritePin(PWR_ON_GPIO_Port, PWR_ON_Pin, 1-is_soft_power_turned_on);
    return; // no need to save power button status
  }
  ee_format();
  ee_write(0, EEPROM_BUF_SIZE, eeprom_buf);
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_SPI1_Init();
  MX_TIM17_Init();
  MX_TIM14_Init();
  MX_IWDG_Init();

  /* Initialize interrupts */
  MX_NVIC_Init();
  /* USER CODE BEGIN 2 */

  /*
    check POWER_GOOD signal immediately after boot
    if high, STM32 most likely just had a reset by IWDG
    in which case, active PS_ON to keep it going
  */
  if(HAL_GPIO_ReadPin(ATX_PG_GPIO_Port, ATX_PG_Pin) == GPIO_PIN_SET)
    is_soft_power_turned_on = 1;
  HAL_GPIO_WritePin(PWR_ON_GPIO_Port, PWR_ON_Pin, 1-is_soft_power_turned_on);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  printf("%s\nv%d.%d.%d\n", boot_message, version_major, version_minor, version_patch);

  if(HAL_GPIO_ReadPin(BTN_COLOR_GPIO_Port, BTN_COLOR_Pin) == GPIO_PIN_RESET)
    printf("ee_format: %d\n", ee_format());

  memset(eeprom_buf, 0, EEPROM_BUF_SIZE);
  ee_read(0, EEPROM_BUF_SIZE, eeprom_buf);
  eeprom_buf[BUTTON_POWER] = 0;
  restore_button_settings();

  HAL_TIM_Base_Start_IT(&htim17);

  HAL_TIM_Base_Start(&htim14);
  HAL_TIM_PWM_Init(&htim14);
  HAL_TIM_PWM_Start(&htim14, TIM_CHANNEL_1);

  while (1)
  {
    HAL_IWDG_Refresh(&hiwdg);
    for (int i = 0; i < BUTTON_COUNT; ++i)
    {
      if(is_pressed(i))
      {
        handle_button_press(i);
        service_press(i);
      }
      // for compatibility with a flip switch
      if(i == BUTTON_POWER && is_released_but_not_serviced(i))
      {
        service_press(i);
        if(HAL_GetTick() - button_status[i].last_press_ts > 750 && is_soft_power_turned_on)
        {
          is_soft_power_turned_on = 0;
          HAL_GPIO_WritePin(PWR_ON_GPIO_Port, PWR_ON_Pin, GPIO_PIN_SET);
        }
      }
    }
      
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL5;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/**
  * @brief NVIC Configuration.
  * @retval None
  */
static void MX_NVIC_Init(void)
{
  /* TIM17_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(TIM17_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(TIM17_IRQn);
}

/* IWDG init function */
static void MX_IWDG_Init(void)
{

  hiwdg.Instance = IWDG;
  hiwdg.Init.Prescaler = IWDG_PRESCALER_4;
  hiwdg.Init.Window = 4095;
  hiwdg.Init.Reload = 4095;
  if (HAL_IWDG_Init(&hiwdg) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* SPI1 init function */
static void MX_SPI1_Init(void)
{

  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* TIM14 init function */
static void MX_TIM14_Init(void)
{

  TIM_OC_InitTypeDef sConfigOC;

  htim14.Instance = TIM14;
  htim14.Init.Prescaler = 3;
  htim14.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim14.Init.Period = 400;
  htim14.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim14.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim14) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  if (HAL_TIM_PWM_Init(&htim14) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 200;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim14, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  HAL_TIM_MspPostInit(&htim14);

}

/* TIM17 init function */
static void MX_TIM17_Init(void)
{

  htim17.Instance = TIM17;
  htim17.Init.Prescaler = 39;
  htim17.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim17.Init.Period = 16666;
  htim17.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim17.Init.RepetitionCounter = 0;
  htim17.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim17) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* USART2 init function */
static void MX_USART2_UART_Init(void)
{

  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_HalfDuplex_Init(&huart2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(PWR_ON_GPIO_Port, PWR_ON_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(DEBUG_GPIO_Port, DEBUG_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : BTN_BRIGHTNESS_Pin */
  GPIO_InitStruct.Pin = BTN_BRIGHTNESS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(BTN_BRIGHTNESS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PWR_ON_Pin */
  GPIO_InitStruct.Pin = PWR_ON_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(PWR_ON_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : BTN_POWER_Pin */
  GPIO_InitStruct.Pin = BTN_POWER_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(BTN_POWER_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : BTN_COLOR_Pin BTN_RGB_MODE_Pin BTN_FANSPEED_Pin */
  GPIO_InitStruct.Pin = BTN_COLOR_Pin|BTN_RGB_MODE_Pin|BTN_FANSPEED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : DEBUG_Pin */
  GPIO_InitStruct.Pin = DEBUG_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(DEBUG_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : ATX_PG_Pin */
  GPIO_InitStruct.Pin = ATX_PG_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(ATX_PG_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
