
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