/**********************************************************************************************
 *
 * Module: RGB LED
 *
 * File Name: rgb_led.c
 *
 * Description: Source file for the RGB LED driver
 *
 * Author: Samer Sameh
 *
 ***********************************************************************************************/
#include "rgb_led.h"

void RGB_Init(RGBConfig* rgbConfig)
{
    GPIO_DirModeSet(rgbConfig->rgbPort, rgbConfig->rgbRedPin, GPIO_DIR_MODE_OUT);
    GPIO_DirModeSet(rgbConfig->rgbPort, rgbConfig->rgbGreenPin, GPIO_DIR_MODE_OUT);
    GPIO_DirModeSet(rgbConfig->rgbPort, rgbConfig->rgbBluePin, GPIO_DIR_MODE_OUT);
}


void RGB_RedLedOn(RGBConfig* rgbConfig)
{
    GPIO_PinWrite(rgbConfig->rgbPort, rgbConfig->rgbRedPin, LOGIC_HIGH);
}

void RGB_RedLedOff(RGBConfig* rgbConfig)
{
    GPIO_PinWrite(rgbConfig->rgbPort, rgbConfig->rgbRedPin, LOGIC_LOW);
}

//void RGB_RedLedToggle(RGBConfig* rgbConfig);


void RGB_GreenLedOn(RGBConfig* rgbConfig)
{
    GPIO_PinWrite(rgbConfig->rgbPort, rgbConfig->rgbGreenPin, LOGIC_HIGH);
}

void RGB_GreenLedOff(RGBConfig* rgbConfig)
{
    GPIO_PinWrite(rgbConfig->rgbPort, rgbConfig->rgbGreenPin , LOGIC_LOW);
}

//void RGB_GreenLedToggle(RGBConfig* rgbConfig);


void RGB_BlueLedOn(RGBConfig* rgbConfig)
{
    GPIO_PinWrite(rgbConfig->rgbPort, rgbConfig->rgbBluePin, LOGIC_HIGH);
}

void RGB_BlueLedOff(RGBConfig* rgbConfig)
{
    GPIO_PinWrite(rgbConfig->rgbPort, rgbConfig->rgbBluePin, LOGIC_LOW);
}
//void RGB_BlueLedToggle(RGBConfig* rgbConfig);

//void RGB_ColorSet(RGBConfig* rgbConfig, RGBColors color)
//{
//
//}

