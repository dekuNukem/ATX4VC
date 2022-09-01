#ifndef __BUTTONS_H
#define __BUTTONS_H

#ifdef __cplusplus
 extern "C" {
#endif 

#include "stm32f0xx_hal.h"

#define BUTTON_COUNT 5

#define BUTTON_RGB_MODE 0
#define BUTTON_COLOR 1
#define BUTTON_BRIGHTNESS 2
#define BUTTON_FANSPEED 3
#define BUTTON_POWER 4

#define BUTTON_SERVICE_UNSERVICED 0
#define BUTTON_SERVICE_SERVICED 1
#define BUTTON_RELEASED 0   
#define BUTTON_PRESSED 1

typedef struct
{
  uint8_t prev_state;
  uint8_t button_state;
  uint8_t service_status;
  uint32_t last_press_ts;
} but_status;

extern but_status button_status[BUTTON_COUNT];

uint8_t is_pressed(uint8_t button_number);
uint8_t is_released_but_not_serviced(uint8_t button_number);
void service_press(uint8_t button_number);
void scan_buttons(void);
void mark_as_released(but_status* butt);

#ifdef __cplusplus
}
#endif

#endif


