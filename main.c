/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"

/* MCAL includes. */
#include "gpio.h"
#include "uart0.h"

/* Definitions for the event bits in the event group. */


/* The HW setup function */
static void prvSetupHardware(void);

/* FreeRTOS tasks */

void vTempReaderTask(void *pvParameters);

int main(void)
{
    /* Setup the hardware for use with the Tiva C board. */
    prvSetupHardware();


    /* Create Tasks here */
    xTaskCreate(vTempReaderTask, "Task 1", 256, NULL, 2, NULL);

    /* Now all the tasks have been started - start the scheduler.

     NOTE : Tasks run in system mode and the scheduler runs in Supervisor mode.
     The processor MUST be in supervisor mode when vTaskStartScheduler is
     called.  The demo applications included in the FreeRTOS.org download switch
     to supervisor mode prior to main being called.  If you are not using one of
     these demo application projects then ensure Supervisor mode is used here. */
    vTaskStartScheduler();

    /* Should never reach here!  If you do then there was not enough heap
     available for the idle task to be created. */
    for (;;);

}

static void prvSetupHardware(void)
{
    /* Place here any needed HW initialization such as GPIO, UART, etc.  */
    UART0_Init();
}

*-----------------------------------------------------------*/
