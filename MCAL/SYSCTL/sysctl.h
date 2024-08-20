/**********************************************************************************************
 *
 * Module: SysCtl
 *
 * File Name: sysctl.h
 *
 * Description: Header file for the TM4C123GH6PM SysCtl driver for TivaC
 *
 * Author: Samer Sameh
 *
 ***********************************************************************************************/


#ifndef MCAL_SYSCTL_SYSCTL_H_
#define MCAL_SYSCTL_SYSCTL_H_

#include "std_types.h"

typedef enum {
    SYSCTL_PERIPH_ADC0, SYSCTL_PERIPH_ADC1, SYSCTL_PERIPH_GPIOA,
    SYSCTL_PERIPH_GPIOB, SYSCTL_PERIPH_GPIOC, SYSCTL_PERIPH_GPIOD,
    SYSCTL_PERIPH_GPIOE, SYSCTL_PERIPH_GPIOF
}SysCtlPeripherals;

void SysCtl_PeripheralEnable(SysCtlPeripherals peripheral);
void SysCtl_PeripheralDisable(SysCtlPeripherals peripheral);

#endif /* MCAL_SYSCTL_SYSCTL_H_ */
