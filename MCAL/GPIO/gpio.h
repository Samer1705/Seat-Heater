/**********************************************************************************************
 *
 * Module: GPIO
 *
 * File Name: GPIO.h
 *
 * Description: Header file for the TM4C123GH6PM DIO driver for TivaC Built-in Buttons and LEDs
 *
 * Author: Samer Sameh
 *
 ***********************************************************************************************/

#ifndef GPIO_H_
#define GPIO_H_

#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define NUM_OF_PORTS 6
#define NUM_OF_PINS_PER_PORT 8

#define PORTA_ID 0
#define PORTB_ID 1
#define PORTC_ID 2
#define PORTD_ID 3
#define PORTE_ID 4
#define PORTF_ID 5

#define PIN0_ID 0
#define PIN1_ID 1
#define PIN2_ID 2
#define PIN3_ID 3
#define PIN4_ID 4
#define PIN5_ID 5
#define PIN6_ID 6
#define PIN7_ID 7

#define GPIO_PORTF_PRIORITY_MASK      0xFF1FFFFF
#define GPIO_PORTF_PRIORITY_BITS_POS  21
#define GPIO_PORTF_INTERRUPT_PRIORITY 5

#define PRESSED                ((uint8)0x00)
#define RELEASED               ((uint8)0x01)

/*******************************************************************************
 *                               Types Declaration                             *
 *******************************************************************************/
typedef enum {
    GPIO_DIR_MODE_IN, GPIO_DIR_MODE_OUT, GPIO_DIR_MODE_H
}GPIOPinIO;


/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/

void GPIO_BuiltinButtonsLedsInit(void);

void GPIO_RedLedOn(void);
void GPIO_BlueLedOn(void);
void GPIO_GreenLedOn(void);

void GPIO_RedLedOff(void);
void GPIO_BlueLedOff(void);
void GPIO_GreenLedOff(void);

void GPIO_RedLedToggle(void);
void GPIO_BlueLedToggle(void);
void GPIO_GreenLedToggle(void);

uint8 GPIO_SW1GetState(void);
uint8 GPIO_SW2GetState(void);

void GPIO_SW1EdgeTriggeredInterruptInit(void);
void GPIO_SW2EdgeTriggeredInterruptInit(void);

void GPIO_DirModeSet(uint32 ui32Port, uint8 ui8Pins, GPIOPinIO ui32PinIO);
GPIOPinIO GPIO_DirModeGet(uint32 ui32Port, uint8 ui8Pin);

void GPIO_PinWrite(uint32 ui32Port, uint8 ui8Pins, uint8 ui8Val);
uint8 GPIO_PinRead (uint32 ui32Port, uint8 ui8Pins);

void GPIO_ADCTriggerDisable(uint32 ui32Port, uint8 ui8Pins);
void GPIO_ADCTriggerEnable(uint32 ui32Port, uint8 ui8Pins);

#endif /* GPIO_H_ */
