/**********************************************************************************************
 *
 * Module: Temperature Sensor
 *
 * File Name: temp_sensor.h
 *
 * Description: Header file for the Temperature Sensor Driver
 *
 * Author: Samer Sameh
 *
 ***********************************************************************************************/

#ifndef HAL_TEMP_SENSOR_TEMP_SENSOR_H_
#define HAL_TEMP_SENSOR_TEMP_SENSOR_H_

#include "std_types.h"
#include "gpio.h"
#include "adc.h"

#define TEMP_SENSOR_MAX_TEMP 45
#define TEMP_SENSOR_MAX_VOLT 3.3

typedef struct{
    ADCChannels channel;
}TEMPConfig;

void TEMP_Init(TEMPConfig *config);

uint8 TEMP_Read(TEMPConfig *config);

#endif /* HAL_TEMP_SENSOR_TEMP_SENSOR_H_ */
