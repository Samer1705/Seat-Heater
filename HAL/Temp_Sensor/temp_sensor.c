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
    ADC0_Seq3SetChannel(config->channel);
}

uint16 TEMP_Read(TEMPConfig *config)
{
    return ADC0_Seq3ReadValue();
}
