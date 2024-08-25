/**********************************************************************************************
 *
 * Module: RGB LED
 *
 * File Name: rgb_led.h
 *
 * Description: Header file for the RGB LED driver
 *
 * Author: Samer Sameh
 *
 ***********************************************************************************************/

#ifndef HAL_RGB_LED_RGB_LED_H_
#define HAL_RGB_LED_RGB_LED_H_

#include "gpio.h"
#include "std_types.h"

/*******************************************************************************
 *                               Types Declaration                             *
 *******************************************************************************/

typedef enum{
    RED = 1, GREEN, YELLOW, BLUE, PINK, CYAN, WHITE
}RGBColors;

typedef struct{
    uint32 rgbPort;
    uint8 rgbRedPin;
    uint8 rgbGreenPin;
    uint8 rgbBluePin;
}RGBConfig;

void RGB_Init(RGBConfig* rgbConfig);

void RGB_RedLedOn(RGBConfig* rgbConfig);
void RGB_RedLedOff(RGBConfig* rgbConfig);
//void RGB_RedLedToggle(RGBConfig* rgbConfig);

void RGB_GreenLedOn(RGBConfig* rgbConfig);
void RGB_GreenLedOff(RGBConfig* rgbConfig);
//void RGB_GreenLedToggle(RGBConfig* rgbConfig);

void RGB_BlueLedOn(RGBConfig* rgbConfig);
void RGB_BlueLedOff(RGBConfig* rgbConfig);
//void RGB_BlueLedToggle(RGBConfig* rgbConfig);

//void RGB_ColorSet(RGBConfig* rgbConfig, RGBColors color);



#endif /* HAL_RGB_LED_RGB_LED_H_ */
