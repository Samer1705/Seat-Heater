/**********************************************************************************************
 *
 * Module: ADC Driver
 *
 * File Name: adc.h
 *
 * Description: Header file for the TM4C123GH6PM ADC driver
 *
 * Author: Samer Sameh
 *
 ***********************************************************************************************/

#ifndef MCAL_ADC_ADC_H_
#define MCAL_ADC_ADC_H_

#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define ERROR_SAMPLE        0xBEE

#define ADC_MAX_VALUE       4096
#define ADC_REF_VOLTAGE     (3.3f)

#define ADC_REF_VOLTAGE     (3.3f)

/*******************************************************************************
 *                               Types Declaration                             *
 *******************************************************************************/
typedef struct{
    uint8 port;
    uint8 pin;
}ADC_ChannelType;

extern const ADC_ChannelType ADC_channels[12];


typedef enum {
    ADC_AIN0, ADC_AIN1, ADC_AIN2, ADC_AIN3, ADC_AIN4, ADC_AIN5, ADC_AIN6, ADC_AIN7, ADC_AIN8, ADC_AIN9, ADC_AIN10, ADC_AIN11
}ADCChannels;

/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/
void ADC0_Seq3Init();
void ADC0_Seq3SetChannel(uint8 ui8Channel);
uint16 ADC0_Seq3ReadValue();
#endif /* MCAL_ADC_ADC_H_ */
