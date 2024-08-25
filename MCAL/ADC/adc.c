/**********************************************************************************************
 *
 * Module: ADC Driver
 *
 * File Name: adc.c
 *
 * Description: Source file for the TM4C123GH6PM ADC driver
 *
 * Author: Samer Sameh
 *
 ***********************************************************************************************/

#include "adc.h"
#include "gpio.h"
#include "common_macros.h"
#include "tm4c123gh6pm_registers.h"

const ADC_ChannelType ADC_channels[12] = { {PORTE_ID,PIN3_ID},
                                           {PORTE_ID,PIN2_ID},
                                           {PORTE_ID,PIN2_ID},
                                           {PORTE_ID,PIN0_ID},
                                           {PORTD_ID,PIN3_ID},
                                           {PORTD_ID,PIN2_ID},
                                           {PORTD_ID,PIN1_ID},
                                           {PORTD_ID,PIN0_ID},
                                           {PORTE_ID,PIN5_ID},
                                           {PORTE_ID,PIN4_ID},
                                           {PORTB_ID,PIN4_ID},
                                           {PORTB_ID,PIN5_ID}
};

void ADC0_Seq3Init()
{
    ADC0_ACTSS_R &= ~(1<<3);
    ADC0_EMUX_R &= ~(0xF<<12);
    ADC0_SSMUX3_R = 0;
    ADC0_SSCTL3_R |= (1<<1) | (1<<2);
    ADC0_ACTSS_R |= (1<<3);
}

void ADC0_Seq3SetChannel(uint8 ui8Channel)
{
    ADC0_ACTSS_R &= ~(1<<3); /*Disable sequencer 3 before configuration*/
    ADC0_SSMUX3_R = ui8Channel;
    ADC0_ACTSS_R |= (1<<3); /*Enabling Sample sequencer 3 */
}

uint16 ADC0_Seq3ReadValue()
{
    ADC0_PSSI_R |= (1<<3);
    while(BIT_IS_CLEAR(ADC0_RIS_R,3));
    ADC0_ISC_R |= (1<<3);
    return (uint16)ADC0_SSFIFO3_R;
}
