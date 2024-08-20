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
    uint8 i;
    for (i = 0; i < 3; i++)
    {
        GPIO_DirModeSet(rgbConfig->rgbPort, (rgbConfig->rgbFirstPin) + i, GPIO_DIR_MODE_OUT);
    }
}


void RGB_RedLedOn(RGBConfig* rgbConfig)
{
    GPIO_PinWrite(rgbConfig->rgbPort, rgbConfig->rgbFirstPin, LOGIC_HIGH);
}

void RGB_RedLedOff(RGBConfig* rgbConfig)
{
    GPIO_PinWrite(rgbConfig->rgbPort, rgbConfig->rgbFirstPin, LOGIC_LOW);
}

//void RGB_RedLedToggle(RGBConfig* rgbConfig);


void RGB_GreenLedOn(RGBConfig* rgbConfig)
{
    GPIO_PinWrite(rgbConfig->rgbPort, (rgbConfig->rgbFirstPin) + 1, LOGIC_HIGH);
}

void RGB_GreenLedOff(RGBConfig* rgbConfig)
{
    GPIO_PinWrite(rgbConfig->rgbPort, (rgbConfig->rgbFirstPin) + 1 , LOGIC_LOW);
}

//void RGB_GreenLedToggle(RGBConfig* rgbConfig);


void RGB_BlueLedOn(RGBConfig* rgbConfig)
{
    GPIO_PinWrite(rgbConfig->rgbPort, (rgbConfig->rgbFirstPin) + 2, LOGIC_HIGH);
}

void RGB_BlueLedOff(RGBConfig* rgbConfig)
{
    GPIO_PinWrite(rgbConfig->rgbPort, (rgbConfig->rgbFirstPin) + 2, LOGIC_LOW);
}
//void RGB_BlueLedToggle(RGBConfig* rgbConfig);

//void RGB_ColorSet(RGBConfig* rgbConfig, RGBColors color)
//{
//
//}

