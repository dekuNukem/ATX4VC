
    keyboard_update();
    for (int i = 0; i < BUTTON_COUNT; ++i)
      if(is_pressed(i))
      {
        printf("%d!", i);
        service_press(i);
      }
    


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