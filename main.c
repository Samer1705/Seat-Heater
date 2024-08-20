/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"
#include "queue.h"
#include "timers.h"

/* MCAL includes. */
#include "gpio.h"
#include "uart0.h"

/* HAL includes. */
#include "rgb_led.h"

/* Definitions for the event bits in the event group. */
#define mainSEAT1_FLAG      1
#define mainSEAT2_FLAG      2

/* The HW setup function */
static void prvSetupHardware(void);

/* FreeRTOS tasks */
void vReadTempTask(void* pvParameter);
void vControlTask(void* pvParameter);
void vSetHeaterTask(void* pvParameter);
void vSetStateTask(void* pvParameter);
void vErrorLogsTask(void* pvParameter);

/* Queue Handles */
QueueHandle_t xSeat1TempQueue;
QueueHandle_t xSeat1StateQueue;
QueueHandle_t xSeat1HeaterQueue;
QueueHandle_t xSeat2TempQueue;
QueueHandle_t xSeat2StateQueue;
QueueHandle_t xSeat2HeaterQueue;

struct SeatParams{
    uint8 id;
    QueueHandle_t xSeatTempQueue;
    QueueHandle_t xSeatStateQueue;
    QueueHandle_t xSeatHeaterQueue;
    RGBConfig xRgbLed;

};

int main(void)
{
    /* Setup the hardware for use with the Tiva C board. */
    prvSetupHardware();

    /* Create Tasks here */
//    xTaskCreate(vControlTask, "Seat 1 Control Task", 100, pvParameters, 3, NULL);

    /* Now all the tasks have been started - start the scheduler. */
    vTaskStartScheduler();

    /* Should never reach here!  If you do then there was not enough heap
     available for the idle task to be created. */
    for (;;);

}

static void prvSetupHardware(void)
{
    /* Place here any needed HW initialization such as GPIO, UART, etc.  */
}

/*-----------------------------------------------------------*/
