/**********************************************************************************************
 *
 * Module: Temperature Sensor
 *
 * File Name: temp_sensor.c
 *
 * Description: Source file for the Temperature Sensor Driver
 *
 * Author: Samer Sameh
 *
 ***********************************************************************************************/
#include "temp_sensor.h"

void TEMP_Init(TEMPConfig *config)
{
    GPIO_ADCTriggerEnable(ADC_channels[config->channel].port, ADC_channels[config->channel].pin);
    ADC0_Seq3Init();
}

uint8 TEMP_Read(TEMPConfig *config)
{
    uint16 usAdcValue;
    float32 fVolt;
    uint8 ucTemp;
    ADC0_Seq3SetChannel(config->channel);
    usAdcValue = ADC0_Seq3ReadValue();
    fVolt = (float32)(((double)usAdcValue*ADC_REF_VOLTAGE)/ADC_MAX_VALUE);
    ucTemp= (uint8)(((double)fVolt*TEMP_SENSOR_MAX_TEMP)/TEMP_SENSOR_MAX_VOLT);
    return ucTemp;
}
