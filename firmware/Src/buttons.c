#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "shared.h"
#include "buttons.h"

but_status button_status[BUTTON_COUNT];

void mark_as_pressed(but_status* butt)
{
  butt->button_state = BUTTON_PRESSED;
  butt->service_status = BUTTON_SERVICE_UNSERVICED;
  butt->last_press_ts = HAL_GetTick();
}

void mark_as_released(but_status* butt)
{
  butt->button_state = BUTTON_RELEASED;
  butt->service_status = BUTTON_SERVICE_UNSERVICED;
}

uint8_t is_pressed(uint8_t button_number)
{
  if(button_number >= BUTTON_COUNT)
    return 0;
  return button_status[button_number].button_state == BUTTON_PRESSED && button_status[button_number].service_status == BUTTON_SERVICE_UNSERVICED;
}

uint8_t is_released_but_not_serviced(uint8_t button_number)
{
  if(button_number >= BUTTON_COUNT)
    return 0;
  return button_status[button_number].button_state == BUTTON_RELEASED && button_status[button_number].service_status == BUTTON_SERVICE_UNSERVICED;
}

void service_press(uint8_t button_number)
{
  if(button_number >= BUTTON_COUNT)
    return;
  button_status[button_number].service_status = BUTTON_SERVICE_SERVICED;
}

void scan_buttons(void)
{
  button_status[BUTTON_POWER].button_state = 1 - HAL_GPIO_ReadPin(BTN_POWER_GPIO_Port, BTN_POWER_Pin);
  button_status[BUTTON_RGB_MODE].button_state = 1 - HAL_GPIO_ReadPin(BTN_RGB_MODE_GPIO_Port, BTN_RGB_MODE_Pin);
  button_status[BUTTON_COLOR].button_state = 1 - HAL_GPIO_ReadPin(BTN_COLOR_GPIO_Port, BTN_COLOR_Pin);
  // brightness button is DFU button which is pulled down, all other buttons are pulled up
  button_status[BUTTON_BRIGHTNESS].button_state = HAL_GPIO_ReadPin(BTN_BRIGHTNESS_GPIO_Port, BTN_BRIGHTNESS_Pin);
  button_status[BUTTON_FANSPEED].button_state = 1 - HAL_GPIO_ReadPin(BTN_FANSPEED_GPIO_Port, BTN_FANSPEED_Pin);

  for (int i = 0; i < BUTTON_COUNT; ++i)
  {
    if(button_status[i].prev_state == BUTTON_RELEASED && button_status[i].button_state == BUTTON_PRESSED)
      mark_as_pressed(&button_status[i]);
    else if(button_status[i].prev_state == BUTTON_PRESSED && button_status[i].button_state == BUTTON_RELEASED)
      mark_as_released(&button_status[i]);
    button_status[i].prev_state = button_status[i].button_state;
  }
}

