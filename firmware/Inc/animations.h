#ifndef __ANIMATIONS_H
#define __ANIMATIONS_H

#ifdef __cplusplus
 extern "C" {
#endif 

#include "stm32f0xx_hal.h"

#define ANIMATION_TYPE_COUNT 3
#define THREE 3
#define ANIMATION_NO_ANIMATION 0
#define ANIMATION_CROSS_FADE 1
#define ANIMATION_FULLY_ON 2

typedef struct rgbcolor
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
} rgbcolor;

typedef struct hsvcolor
{
    uint8_t h;
    uint8_t s;
    uint8_t v;
} hsvcolor;

rgbcolor hsv2rgb(hsvcolor hsv);

#ifdef __cplusplus
}
#endif

#endif


