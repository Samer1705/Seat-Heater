/**********************************************************************************************
 *
 * Module: SysCtl
 *
 * File Name: sysctl.c
 *
 * Description: Source file for the TM4C123GH6PM SysCtl driver for TivaC
 *
 * Author: Samer Sameh
 *
 ***********************************************************************************************/

#include "sysctl.h"
#include "tm4c123gh6pm_registers.h"

void SysCtl_PeripheralEnable(SysCtlPeripherals peripheral)
{
    switch (peripheral) {
    case SYSCTL_PERIPH_GPIOA:
        SYSCTL_RCGCGPIO_REG |= 0x01;
        while(!(SYSCTL_PRGPIO_REG & 0x01));
        break;
    case SYSCTL_PERIPH_GPIOB:
        SYSCTL_RCGCGPIO_REG |= 0x02;
        while(!(SYSCTL_PRGPIO_REG & 0x02));
        break;
    case SYSCTL_PERIPH_GPIOC:
        SYSCTL_RCGCGPIO_REG |= 0x04;
        while(!(SYSCTL_PRGPIO_REG & 0x04));
        break;
    case SYSCTL_PERIPH_GPIOD:
        SYSCTL_RCGCGPIO_REG |= 0x08;
        while(!(SYSCTL_PRGPIO_REG & 0x08));
        break;
    case SYSCTL_PERIPH_GPIOE:
        SYSCTL_RCGCGPIO_REG |= 0x10;
        while(!(SYSCTL_PRGPIO_REG & 0x10));
        break;
    case SYSCTL_PERIPH_GPIOF:
        SYSCTL_RCGCGPIO_REG |= 0x20;
        while(!(SYSCTL_PRGPIO_REG & 0x20));
        break;
    default:
        break;
    }
}

void SysCtl_PeripheralDisable(SysCtlPeripherals ui32Peripheral)
{
    switch (ui32Peripheral) {
    case SYSCTL_PERIPH_GPIOA:
        SYSCTL_RCGCGPIO_REG &= ~0x01;
        while(!(SYSCTL_PRGPIO_REG & 0x01));
        break;
    case SYSCTL_PERIPH_GPIOB:
        SYSCTL_RCGCGPIO_REG &= ~0x02;
        while(!(SYSCTL_PRGPIO_REG & 0x02));
        break;
    case SYSCTL_PERIPH_GPIOC:
        SYSCTL_RCGCGPIO_REG &= ~0x04;
        while(!(SYSCTL_PRGPIO_REG & 0x04));
        break;
    case SYSCTL_PERIPH_GPIOD:
        SYSCTL_RCGCGPIO_REG &= ~0x08;
        while(!(SYSCTL_PRGPIO_REG & 0x08));
        break;
    case SYSCTL_PERIPH_GPIOE:
        SYSCTL_RCGCGPIO_REG &= ~0x10;
        while(!(SYSCTL_PRGPIO_REG & 0x10));
        break;
    case SYSCTL_PERIPH_GPIOF:
        SYSCTL_RCGCGPIO_REG &= ~0x20;
        while(!(SYSCTL_PRGPIO_REG & 0x20));
        break;
    default:
        break;
    }
}
