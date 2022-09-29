        // this_hsv.v = (sin_lookup[((frame_interrupt_count*3/2 + i*10)) % SIN_LOOKUP_SIZE]) / (eeprom_buf[BUTTON_BRIGHTNESS] % LED_BRIGHTNESS_STEP_COUNT);
      // this_hsv.v = (uint32_t)sin_lookup[((frame_interrupt_count*3/2 + i*10)) % SIN_LOOKUP_SIZE] * 10 / (eeprom_buf[BUTTON_BRIGHTNESS] % LED_BRIGHTNESS_STEP_COUNT);
      // this_hsv.v = (uint8_t)((double)global_hsv.v / 255 * sin_lookup[((frame_interrupt_count*3/2 + i*10)) % SIN_LOOKUP_SIZE]);
      // printf("%f\n", (double)global_hsv.v / 255);

    // printf("took %d ms\n", (micros() - startstart)/1000);
  else if(current_animation == ANIMATION_MATRIX)
  {
    for (int i = 0; i < NEOPIXEL_COUNT; ++i)
    {
      hsvcolor this_hsv;
      this_hsv.h = frame_interrupt_count/2;
      this_hsv.s = 255;
      this_hsv.v = sin_lookup[((frame_interrupt_count + i*10)) % SIN_LOOKUP_SIZE];
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

  else if(current_animation == ANIMATION_MATRIX)
  {
    for (int i = 0; i < NEOPIXEL_COUNT; ++i)
    {
      hsvcolor this_hsv;
      this_hsv.h = global_hsv.h;
      this_hsv.s = 255;
      this_hsv.v = (i*6 + frame_interrupt_count) % global_hsv.v;
      my_rgb = hsv2rgb(this_hsv);
      red_buf[i] = my_rgb.r;
      green_buf[i] = my_rgb.g;
      blue_buf[i] = my_rgb.b;
    }
  }


    // uint32_t startstart = micros();
    // double degree_rad = (double)(frame_interrupt_count % 360) * ONE_DEG_IN_RAD;
    // uint8_t this_v = (uint8_t)(sin(degree_rad) * 127 + 127);

while(1)
  {
    ds18b20_start_conversion();
    HAL_Delay(20);
    printf("%d\n", ds18b20_get_temp() >> 4);
    HAL_Delay(100);
  }
  
    keyboard_update();
    for (int i = 0; i < BUTTON_COUNT; ++i)
      if(is_pressed(i))
      {
        printf("%d!", i);
        service_press(i);
      }


    for (int i = 0; i < BUTTON_COUNT; ++i)
      if(is_pressed(i))
      {
        printf("pressed: %d!\n", i);
        service_press(i);
      }

uint8_t count;
// happens every 16ms
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_3);
  count = (count+1)%2;
  if(count)
    htim14.Instance->CCR1 = 100;
  else
    htim14.Instance->CCR1 = 300;
}


__disable_irq();
    neopixel_show(red_buf, green_buf, blue_buf);
    __enable_irq();

HAL_Delay(50);
    printf("%d", HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3));
#define BUTTON_STABLE 0
#define BUTTON_BOUNCING 1
uint8_t debounce_button(uint8_t initial_state, uint32_t duration_ms)
{
  uint32_t start = HAL_GetTick();
  while(1)
  {
    if(HAL_GetTick() - start > duration_ms)
      return BUTTON_STABLE;
    if(HAL_GPIO_ReadPin(PWR_BTN_GPIO_Port, PWR_BTN_Pin) != initial_state)
      return BUTTON_BOUNCING;
  }
  return BUTTON_STABLE;
}
// neopixel_show(red_buf, green_buf, blue_buf);
    // HAL_Delay(20);

uint8_t current_btn_status = 1;
uint8_t prev_btn_status = 1;
uint8_t button_press_count = 1;

    current_btn_status = HAL_GPIO_ReadPin(PWR_BTN_GPIO_Port, PWR_BTN_Pin);
    if(current_btn_status == 0 && prev_btn_status == 1)
    {
      uint8_t debounce_result = debounce_button(current_btn_status, 33);
      if(debounce_result == BUTTON_STABLE)
      {
        button_press_count = (button_press_count + 1) % 2;
        HAL_GPIO_WritePin(PWR_ON_GPIO_Port, PWR_ON_Pin, button_press_count);
        printf("%d button pressed! %d\n", HAL_GetTick(), button_press_count);
        HAL_Delay(50);
      }
    }
    prev_btn_status = current_btn_status;


while(count--)
  {
    while(!__HAL_SPI_GET_FLAG(&hspi1, SPI_FLAG_TXE))
      ;
    // *(volatile uint8_t *)&SPIx->DR = *pData++;
    // &hspi1->Instance->DR = *((uint16_t *)pData);
    // hspi1.Instance->DR = *((uint16_t *)pData);
    *((__IO uint8_t *)hspi1.Instance->DR) = *pData;
    pData++;
  }

for (int i = 0; i < BUTTON_COUNT; ++i)
      if(is_pressed(i))
      {
        button_current_selected_option[i]++;
        service_press(i);
      }

    for (int i = 0; i < BUTTON_COUNT; ++i)
      printf("%d ", button_current_selected_option[i]);
    printf("\n");

void handle_button_press(uint8_t button_index)
{
  printf("%d pressed!\n", button_index);
  button_current_selected_option[button_index]++;
  
  memset(eeprom_buf, 0, EEPROM_BUF_SIZE);
  memcpy(eeprom_buf, button_current_selected_option, BUTTON_COUNT);

  for (int i = 0; i < EEPROM_BUF_SIZE; ++i)
    printf("b%d %d\n", i, eeprom_buf[i]);

  printf("f%d\n", ee_format());
  HAL_Delay(10);
  printf("w%d\n", ee_write(0, EEPROM_BUF_SIZE, eeprom_buf));
  HAL_Delay(10);
}

scan_buttons();
  for (int i = 0; i < BUTTON_COUNT; ++i)
    button_status[i].button_state = BUTTON_PRESSED;


void change_fan_speed(uint32_t amount)
{
  // 0 stopped 400 full speed
  htim14.Instance->CCR1 = 100;
  HAL_Delay(50);
  htim14.Instance->CCR1 = 300;
  HAL_Delay(50);
}

    printf("fan %d %d\n", fan_index, fan_speend_lookup[fan_index]);
  printf("hard power: %d\n", is_hard_power);
  for (int i = 0; i < EEPROM_BUF_SIZE; ++i)
    printf("ee%d=%d\n", i, eeprom_buf[i]);
  printf("\n");


printf("sp=%d\n", is_soft_power_turned_on);

// POWER GOOD signal lasts about 34ms after turning off POWER_ON on corsair VS450
    printf("sp=%d\n", is_soft_power_turned_on);




  hsvcolor my_hsv;
  my_hsv.h = 30;
  my_hsv.s = 127;
  my_hsv.v = 127;
  rgbcolor my_rgb = hsv2rgb(my_hsv);
  printf("rgb=%d %d %d\n", my_rgb.r, my_rgb.g, my_rgb.b);

220 to 40 finer 75 inbetween 12 steps
40 to 220 corsor 180 inbetween 8 steps


const uint8_t hsv_angle_lookup[RGB_COLOR_STEP_COUNT] = {4,10,17,23,30,36,43,63,83,103,123,143,163,183,203,226,233,239,246,252};
  // if(global_hsv.h <= 50 || global_hsv.h >= 220)
  //   global_hsv.s = 255;
  // else
  //   global_hsv.s = 240;


  // printf("%d: %d %d %d\n", global_hsv.h, my_rgb.r, my_rgb.g, my_rgb.b);
  printf("b %d %d\n", button_status, global_hsv.v);

HAL_GPIO_TogglePin(DEBUG_GPIO_Port, DEBUG_Pin);

      // printf("%d %d %d\n", my_rgb.r, my_rgb.g, my_rgb.b);
if(current_breathing_frame > BREATHING_FRAME_COUNT / 2)
      printf("up! %d\n", current_breathing_frame);
    else
      printf("down! %d\n", current_breathing_frame);

      // current_breathing_brightness = lround(global_hsv.v - stepping * (current_breathing_frame - (current_breathing_frame - halfway) * 2 + 1));
    // printf("%d %d %f\n", global_hsv.v, dim_to, stepping);
    // printf("%f %d\n", stepping, current_breathing_brightness);
printf("%d\n", HAL_GetTick() - button_status[i].last_press_ts);

          printf("long release!\n");


  // printf("%d pressed!\n", button_index);

  // else if(button_index == BUTTON_RGB_MODE)
  // {
  //   printf("rgbm: %d\n", eeprom_buf[button_index] % ANIMATION_TYPE_COUNT);
  // }

void mark_as_autorepeat(but_status* butt)
{
  butt->button_state = BUTTON_AUTOREPEAT;
  butt->service_status = BUTTON_SERVICE_UNSERVICED;
}
