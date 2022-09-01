#ifndef __ANIMATIONS_H
#define __ANIMATIONS_H

#ifdef __cplusplus
 extern "C" {
#endif 

#include "stm32f0xx_hal.h"

#define ANIMATION_TYPE_COUNT 2
#define THREE 3
#define ANIMATION_SOLID_COLOR 0
#define ANIMATION_FLOWING_RAINBOW 1

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


