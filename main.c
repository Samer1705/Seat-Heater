/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"
#include "semphr.h"
#include "queue.h"
#include "timers.h"

/* MCAL includes. */
#include "gpio.h"
#include "uart0.h"
#include "sysctl.h"
#include "adc.h"
#include "GPTM.h"
#include "tm4c123gh6pm_registers.h"

/* HAL includes. */
#include "rgb_led.h"
#include "temp_sensor.h"

#define NUMBER_OF_ITERATIONS_PER_ONE_MILI_SECOND 369

void Delay_MS(unsigned long long n)
{
    volatile unsigned long long count = 0;
    while(count++ < (NUMBER_OF_ITERATIONS_PER_ONE_MILI_SECOND * n) );
}

uint32 ullTasksOutTime[14];
uint32 ullTasksInTime[14];
uint32 ullTasksTotalTime[14];

/* Definitions */
#define SEAT1_ID                1           /* Drivers's Seat ID */
#define SEAT2_ID                2           /* Passenger's Seat ID */

#define HEATING_LEVEL_OFF       20           /* Heating Level State is Off */
#define HEATING_LEVEL_LOW       25          /* Heating Level State is Low (25C) */
#define HEATING_LEVEL_MED       30          /* Heating Level State is Medium (30C) */
#define HEATING_LEVEL_HIGH      35          /* Heating Level State is High (35C) */

#define HEATER_INTENSITY_OFF    0           /* Heater Intensity is Off */
#define HEATER_INTENSITY_LOW    1           /* Heater Intensity is Low (Green) */
#define HEATER_INTENSITY_MED    2           /* Heater Intensity is Medium (Blue) */
#define HEATER_INTENSITY_HIGH   3           /* Heater Intensity is High (Cyan) */

#define SEAT1_BELOW_RANGE_ERROR 11
#define SEAT1_ABOVE_RANGE_ERROR 12
#define SEAT2_BELOW_RANGE_ERROR 21
#define SEAT2_ABOVE_RANGE_ERROR 22

/* Definitions for the event bits in the event group. */
#define mainSEAT1_BUTTON_PRESS_FLAG          (1<<1)      /* Drivers's Seat Button Press Event Flag */
#define mainSEAT2_BUTTON_PRESS_FLAG          (1<<2)      /* Passenger's Seat Button Press Event Flag */
#define mainSEAT1_ERROR_FLAG                 (1<<3)
#define mainSEAT2_ERROR_FLAG                 (1<<4)

/* Type Definitions */
typedef struct{
    uint8 ui8SeatId;
    uint8 ui8Temp;
    uint8 ui8HeatingLevel;
    uint8 ui8HeaterIntensity;
}DisplayData;

typedef struct{
    uint32 ui32TimeStamp;
    uint8 ui8ErrorCode;
}ErrorLog;

typedef struct{
    uint32 ui32TimeStamp;
    uint8 ui8Level;
}SavedState;
typedef struct{
    uint8 ui8Id;                            /* Seat ID */
    TEMPConfig* xTempSensor;                /* Seat Temperature Sensor */
    QueueHandle_t* xTempQueue;              /* Seat Temperature Reading Queue */
    uint8 ui8SeatButtonFlag;                /* Seat Button Press Event Flag */
    QueueHandle_t* xLevelQueue;             /* Seat Heating Level Setting Queue */
    RGBConfig* xRgbLed;                     /* Seat Heater (RGB LED) */
    QueueHandle_t* xIntensityQueue;         /* Seat Heater Intensity Setting Queue */
    QueueSetHandle_t* xControlQueueSet;     /* Seat Control Queue Set For Temperature Reading Queue & Heating Level Setting Queue */
    uint8 ui8SeatErrorFlag;
    SavedState* xSeatSavedState;
}SeatParams;

/* Event Group Handles */
EventGroupHandle_t xSeatButtonEventGroup;

/* Mutex Handles */
SemaphoreHandle_t xAdcMutex;

/* Queue Handles */
QueueHandle_t xSeat1TempQueue;
QueueHandle_t xSeat1LevelQueue;
QueueHandle_t xSeat1IntensityQueue;

QueueHandle_t xSeat2TempQueue;
QueueHandle_t xSeat2LevelQueue;
QueueHandle_t xSeat2IntensityQueue;

QueueHandle_t xDisplayQueue;
QueueHandle_t xErrorLogQueue;

/* Queue Set Handles */
QueueSetHandle_t xSeat1ControlQueueSet;
QueueSetHandle_t xSeat2ControlQueueSet;

/* Global Variables */
/* Temperature Sensors */
TEMPConfig xSeat1TempSensor = { ADC_AIN0 };
TEMPConfig xSeat2TempSensor = { ADC_AIN1 };

/* Heaters (RGB LEDs) */
RGBConfig xSeat1RGB = { PORTD_ID, PIN0_ID, PIN1_ID, PIN2_ID };
RGBConfig xSeat2RGB = { PORTF_ID, PIN1_ID, PIN3_ID, PIN2_ID };

/* Seats Parameters */
SavedState xSeat1SavedState = {
                               0,
                               HEATING_LEVEL_OFF
};

SavedState xSeat2SavedState = {
                               0,
                               HEATING_LEVEL_OFF
};

SeatParams xSeat1Params = {
                           SEAT1_ID,
                           &xSeat1TempSensor,
                           &xSeat1TempQueue,
                           mainSEAT1_BUTTON_PRESS_FLAG,
                           &xSeat1LevelQueue,
                           &xSeat1RGB,
                           &xSeat1IntensityQueue,
                           &xSeat1ControlQueueSet,
                           mainSEAT1_ERROR_FLAG,
                           &xSeat1SavedState
};

SeatParams xSeat2Params = {
                           SEAT2_ID,
                           &xSeat2TempSensor,
                           &xSeat2TempQueue,
                           mainSEAT2_BUTTON_PRESS_FLAG,
                           &xSeat2LevelQueue,
                           &xSeat2RGB,
                           &xSeat2IntensityQueue,
                           &xSeat2ControlQueueSet,
                           mainSEAT2_ERROR_FLAG,
                           &xSeat2SavedState
};

ErrorLog axErrorLogs[30];

/* The HW setup function */
static void prvSetupHardware(void);

/* FreeRTOS tasks */
void vReadTempTask(void* pvParameters);
void vSetLevelTask(void* pvParameters);
void vControlTask(void* pvParameters);
void vSetHeaterTask(void* pvParameters);
void vDisplayTask(void* pvParameters);
void vLogErrorTask(void* pvParameters);
void vWarningLedTask(void* pvParameters);

void vRunTimeMeasurementsTask(void *pvParameters);

TaskHandle_t xTask0Handle;
TaskHandle_t xTask1Handle;
TaskHandle_t xTask2Handle;
TaskHandle_t xTask3Handle;
TaskHandle_t xTask4Handle;
TaskHandle_t xTask5Handle;
TaskHandle_t xTask6Handle;
TaskHandle_t xTask7Handle;
TaskHandle_t xTask8Handle;
TaskHandle_t xTask9Handle;
TaskHandle_t xTask10Handle;
TaskHandle_t xTask11Handle;
TaskHandle_t xTask12Handle;

int main(void)
{
    /* Create Event Group */
    xSeatButtonEventGroup = xEventGroupCreate();

    /* Create Mutex */
    xAdcMutex = xSemaphoreCreateMutex();

    /* Create Queue */
    xSeat1TempQueue = xQueueCreate(3, sizeof(uint8));
    xSeat1LevelQueue = xQueueCreate(3, sizeof(uint8));
    xSeat1IntensityQueue = xQueueCreate(3, sizeof(uint8));

    xSeat2TempQueue = xQueueCreate(3, sizeof(uint8));
    xSeat2LevelQueue = xQueueCreate(3, sizeof(uint8));
    xSeat2IntensityQueue = xQueueCreate(3, sizeof(uint8));

    xDisplayQueue = xQueueCreate(3, sizeof(DisplayData));
    xErrorLogQueue = xQueueCreate(3, sizeof(uint8));

    /* Create Queue Set */
    xSeat1ControlQueueSet = xQueueCreateSet(6);
    xSeat2ControlQueueSet = xQueueCreateSet(6);

    /* Create FreeRTOS Tasks */
    xTaskCreate(vReadTempTask, "Seat 1 Read Temperature Task", 64, (void*) &xSeat1Params, 2, &xTask0Handle);
    xTaskCreate(vSetLevelTask, "Seat 1 Set Level Task", 64, (void*) &xSeat1Params, 4, &xTask1Handle);
    xTaskCreate(vControlTask, "Seat 1 Control Task", 128, (void*) &xSeat1Params, 3, &xTask2Handle);
    xTaskCreate(vSetHeaterTask, "Seat 1 Set Heater Task", 64, (void*) &xSeat1Params, 2, &xTask3Handle);
    xTaskCreate(vWarningLedTask, "Seat 1 Warning Led Task", 64, (void*) &xSeat1Params, 1, &xTask4Handle);

    xTaskCreate(vReadTempTask, "Seat 2 Read Temperature Task", 64, (void*) &xSeat2Params, 2, &xTask5Handle);
    xTaskCreate(vSetLevelTask, "Seat 2 Set Level Task", 64, (void*) &xSeat2Params, 4, &xTask6Handle);
    xTaskCreate(vControlTask, "Seat 2 Control Task", 128, (void*) &xSeat2Params, 3, &xTask7Handle);
    xTaskCreate(vSetHeaterTask, "Seat 2 Set Heater Task", 64, (void*) &xSeat2Params, 2, &xTask8Handle);
    xTaskCreate(vWarningLedTask, "Seat 2 Warning Led Task", 64, (void*) &xSeat2Params, 1, &xTask9Handle);

    xTaskCreate(vDisplayTask, "Display Task", 64, NULL, 1, &xTask10Handle);
    xTaskCreate(vLogErrorTask, "Error Logging Task", 64, NULL, 3, &xTask11Handle);

    xTaskCreate(vRunTimeMeasurementsTask, "Run time", 256, NULL, 5, &xTask12Handle);

    vTaskSetApplicationTaskTag( xTask0Handle, ( TaskHookFunction_t ) 1 );
    vTaskSetApplicationTaskTag( xTask1Handle, ( TaskHookFunction_t ) 2 );
    vTaskSetApplicationTaskTag( xTask2Handle, ( TaskHookFunction_t ) 3 );
    vTaskSetApplicationTaskTag( xTask3Handle, ( TaskHookFunction_t ) 4 );
    vTaskSetApplicationTaskTag( xTask4Handle, ( TaskHookFunction_t ) 5 );
    vTaskSetApplicationTaskTag( xTask5Handle, ( TaskHookFunction_t ) 6 );
    vTaskSetApplicationTaskTag( xTask6Handle, ( TaskHookFunction_t ) 7 );
    vTaskSetApplicationTaskTag( xTask7Handle, ( TaskHookFunction_t ) 8 );
    vTaskSetApplicationTaskTag( xTask8Handle, ( TaskHookFunction_t ) 9 );
    vTaskSetApplicationTaskTag( xTask9Handle, ( TaskHookFunction_t ) 10 );
    vTaskSetApplicationTaskTag( xTask10Handle, ( TaskHookFunction_t ) 11 );
    vTaskSetApplicationTaskTag( xTask11Handle, ( TaskHookFunction_t ) 12 );
    vTaskSetApplicationTaskTag( xTask12Handle, ( TaskHookFunction_t ) 13 );

    /* Setup the hardware for use with the Tiva C board. */
    prvSetupHardware();

    /* Now all the tasks have been started - start the scheduler. */
    vTaskStartScheduler();

    /* Should never reach here!  If you do then there was not enough heap
     available for the idle task to be created. */
    for (;;);

}

static void prvSetupHardware(void)
{
    /* MCAL Initialization */
    SysCtl_PeripheralEnable(SYSCTL_PERIPH_GPIOD);
    SysCtl_PeripheralEnable(SYSCTL_PERIPH_GPIOE);
    SysCtl_PeripheralEnable(SYSCTL_PERIPH_GPIOF);
    SysCtl_PeripheralEnable(SYSCTL_PERIPH_ADC0);
    UART0_Init();
    GPTM_WTimer0Init();

    /* HAL Initialization */
    GPIO_BuiltinButtonsLedsInit();
    TEMP_Init(&xSeat1TempSensor);
    RGB_Init(&xSeat1RGB);
    TEMP_Init(&xSeat2TempSensor);
    RGB_Init(&xSeat2RGB);
    GPIO_SW1EdgeTriggeredInterruptInit();
    GPIO_SW2EdgeTriggeredInterruptInit();
}

void vReadTempTask(void* pvParameters)
{
    SeatParams* xSeatParams = (SeatParams*) pvParameters;
    uint8 ui8Temp = 0, ui8PrevTemp = 0;
    TickType_t xLastWakeTime = xTaskGetTickCount();
    for(;;)
    {
        xSemaphoreTake(xAdcMutex, portMAX_DELAY);
        ui8Temp = TEMP_Read(xSeatParams->xTempSensor);
        xSemaphoreGive(xAdcMutex);
        if(ui8PrevTemp != ui8Temp)
        {
            xQueueSend(*(xSeatParams->xTempQueue), &ui8Temp, portMAX_DELAY);
        }
        ui8PrevTemp = ui8Temp;
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(200));
    }
}

void vSetLevelTask(void* pvParameters)
{
    SeatParams* xSeatParams = (SeatParams*) pvParameters;
    EventBits_t xEventGroupValue;
    uint8 ui8HeatingLevel =  HEATING_LEVEL_OFF;
    for(;;)
    {
        xEventGroupValue = xEventGroupWaitBits(xSeatButtonEventGroup, xSeatParams->ui8SeatButtonFlag, pdTRUE, pdFALSE, portMAX_DELAY);
        if ((xEventGroupValue & (xSeatParams->ui8SeatButtonFlag)) != 0)
        {
            ui8HeatingLevel = ((ui8HeatingLevel - 15) % 20) + 20;
            xSeatParams->xSeatSavedState->ui32TimeStamp = GPTM_WTimer0Read() / 10;
            xSeatParams->xSeatSavedState->ui8Level = ui8HeatingLevel;
            xQueueSend(*(xSeatParams->xLevelQueue), &ui8HeatingLevel, portMAX_DELAY);
        }
    }
}

void vControlTask(void* pvParameters)
{
    SeatParams* xSeatParams = (SeatParams*) pvParameters;
    DisplayData xSeatDisplay;
    xQueueAddToSet(*(xSeatParams->xTempQueue), *(xSeatParams->xControlQueueSet));
    xQueueAddToSet(*(xSeatParams->xLevelQueue), *(xSeatParams->xControlQueueSet));
    QueueHandle_t xActivatedMember;
    uint8 ui8HeatingLevel = HEATING_LEVEL_OFF;
    uint8 ui8Temp = 0;
    uint8 ui8HeaterIntensity = HEATER_INTENSITY_OFF;
    uint8 ui8ErrorCode = 0;
    for(;;)
    {
        xActivatedMember = xQueueSelectFromSet(*(xSeatParams->xControlQueueSet), portMAX_DELAY);
        if(xActivatedMember == *(xSeatParams->xTempQueue))
        {
            xQueueReceive(xActivatedMember, &ui8Temp, 0);
        }
        else if(xActivatedMember == *(xSeatParams->xLevelQueue))
        {
            xQueueReceive(xActivatedMember, &ui8HeatingLevel, 0);
        }
        if(ui8Temp >= 5 && ui8Temp <= 40)
        {
            if((ui8HeatingLevel == HEATING_LEVEL_OFF) || ((sint8)(ui8HeatingLevel - ui8Temp) <= 2))
            {
                ui8HeaterIntensity = HEATER_INTENSITY_OFF;
            }
            else if(((sint8)(ui8HeatingLevel - ui8Temp) > 2) && ((sint8)(ui8HeatingLevel - ui8Temp) <= 5))
            {
                ui8HeaterIntensity = HEATER_INTENSITY_LOW;
            }
            else if(((sint8)(ui8HeatingLevel - ui8Temp) > 5) && ((sint8)(ui8HeatingLevel - ui8Temp) <= 10))
            {
                ui8HeaterIntensity = HEATER_INTENSITY_MED;
            }
            else if((sint8)(ui8HeatingLevel - ui8Temp) > 10)
            {
                ui8HeaterIntensity = HEATER_INTENSITY_HIGH;
            }
        }
        else
        {
            ui8HeaterIntensity = HEATER_INTENSITY_OFF;
            xEventGroupSetBits(xSeatButtonEventGroup, xSeatParams->ui8SeatErrorFlag);
            switch (xSeatParams->ui8Id) {
            case 1:
                if(ui8Temp < 5)
                {
                    ui8ErrorCode = SEAT1_BELOW_RANGE_ERROR;
                }
                else if(ui8Temp > 40)
                {
                    ui8ErrorCode = SEAT1_ABOVE_RANGE_ERROR;
                }
                break;
            case 2:
                if(ui8Temp < 5)
                {
                    ui8ErrorCode = SEAT2_BELOW_RANGE_ERROR;
                }
                else if(ui8Temp > 40)
                {
                    ui8ErrorCode = SEAT2_ABOVE_RANGE_ERROR;
                }
                break;
            default:
                break;
            }
            xQueueSend(xErrorLogQueue, &ui8ErrorCode, 0);

        }
        xSeatDisplay.ui8SeatId = xSeatParams->ui8Id;
        xSeatDisplay.ui8Temp = ui8Temp;
        xSeatDisplay.ui8HeatingLevel = ui8HeatingLevel;
        xSeatDisplay.ui8HeaterIntensity = ui8HeaterIntensity;
        xQueueSend(*(xSeatParams->xIntensityQueue), &ui8HeaterIntensity, portMAX_DELAY);
        xQueueSend(xDisplayQueue, &xSeatDisplay, 0);
    }
}

void vSetHeaterTask(void* pvParameters)
{
    SeatParams* xSeatParams = (SeatParams*) pvParameters;
    uint8 ui8HeaterIntensity = HEATER_INTENSITY_OFF;
    for(;;)
    {
        xQueueReceive(*(xSeatParams->xIntensityQueue),&ui8HeaterIntensity, portMAX_DELAY);
        switch (ui8HeaterIntensity) {
        case HEATER_INTENSITY_OFF:
            RGB_RedLedOff(xSeatParams->xRgbLed);
            RGB_GreenLedOff(xSeatParams->xRgbLed);
            RGB_BlueLedOff(xSeatParams->xRgbLed);
            break;
        case HEATER_INTENSITY_LOW:
            RGB_RedLedOff(xSeatParams->xRgbLed);
            RGB_GreenLedOn(xSeatParams->xRgbLed);
            RGB_BlueLedOff(xSeatParams->xRgbLed);
            break;
        case HEATER_INTENSITY_MED:
            RGB_RedLedOff(xSeatParams->xRgbLed);
            RGB_GreenLedOff(xSeatParams->xRgbLed);
            RGB_BlueLedOn(xSeatParams->xRgbLed);
            break;
        case HEATER_INTENSITY_HIGH:
            RGB_RedLedOff(xSeatParams->xRgbLed);
            RGB_GreenLedOn(xSeatParams->xRgbLed);
            RGB_BlueLedOn(xSeatParams->xRgbLed);
            break;
        default:
            break;
        }

    }
}

void vDisplayTask(void* pvParameters)
{
    DisplayData xDisplay;
    DisplayData xSeat1Display = {
                                 1,
                                 0,
                                 HEATING_LEVEL_OFF,
                                 HEATER_INTENSITY_OFF
    };
    DisplayData xSeat2Display = {
                                 2,
                                 0,
                                 HEATING_LEVEL_OFF,
                                 HEATER_INTENSITY_OFF
    };
    for(;;)
    {
        if(xQueueReceive(xDisplayQueue, &xDisplay, 0) == pdPASS)
        {
            if(xDisplay.ui8SeatId == 1)
            {
                xSeat1Display = xDisplay;
            }
            else if(xDisplay.ui8SeatId == 2)
            {
                xSeat2Display = xDisplay;
            }
        }
        UART0_SendString("\r\nSeat ");
        UART0_SendInteger(xSeat1Display.ui8SeatId);
        UART0_SendString(" | Temperature: ");
        UART0_SendInteger(xSeat1Display.ui8Temp);
        UART0_SendString(" | Heating Level: ");
        switch (xSeat1Display.ui8HeatingLevel)
        {
        case HEATING_LEVEL_OFF:
            UART0_SendString("Off");
            break;
        case HEATING_LEVEL_LOW:
            UART0_SendString("Low");
            break;
        case HEATING_LEVEL_MED:
            UART0_SendString("Medium");
            break;
        case HEATING_LEVEL_HIGH:
            UART0_SendString("High");
            break;
        default:
            break;
        }
        UART0_SendString(" | Heater Intensity: ");
        switch (xSeat1Display.ui8HeaterIntensity)
        {
        case HEATER_INTENSITY_OFF:
            UART0_SendString("Off");
            break;
        case HEATER_INTENSITY_LOW:
            UART0_SendString("Low");
            break;
        case HEATER_INTENSITY_MED:
            UART0_SendString("Medium");
            break;
        case HEATER_INTENSITY_HIGH:
            UART0_SendString("High");
            break;
        default:
            break;
        }
        UART0_SendString("\r\nSeat ");
        UART0_SendInteger(xSeat2Display.ui8SeatId);
        UART0_SendString(" | Temperature: ");
        UART0_SendInteger(xSeat2Display.ui8Temp);
        UART0_SendString(" | Heating Level: ");
        switch (xSeat2Display.ui8HeatingLevel)
        {
        case HEATING_LEVEL_OFF:
            UART0_SendString("Off");
            break;
        case HEATING_LEVEL_LOW:
            UART0_SendString("Low");
            break;
        case HEATING_LEVEL_MED:
            UART0_SendString("Medium");
            break;
        case HEATING_LEVEL_HIGH:
            UART0_SendString("High");
            break;
        default:
            break;
        }
        UART0_SendString(" | Heater Intensity: ");
        switch (xSeat2Display.ui8HeaterIntensity)
        {
        case HEATER_INTENSITY_OFF:
            UART0_SendString("Off");
            break;
        case HEATER_INTENSITY_LOW:
            UART0_SendString("Low");
            break;
        case HEATER_INTENSITY_MED:
            UART0_SendString("Medium");
            break;
        case HEATER_INTENSITY_HIGH:
            UART0_SendString("High");
            break;
        default:
            break;
        }
        UART0_SendString("\r\n");
        vTaskDelay(pdMS_TO_TICKS(50));
        UART0_SendString("\033c");
    }
}

void vLogErrorTask(void* pvParameters)
{
    ErrorLog xErrorLog;
    uint8 ui8ErrorCode = 0;
    uint8 ui8LogIndex = 0;
    for(;;)
    {
        xQueueReceive(xErrorLogQueue, &ui8ErrorCode, portMAX_DELAY);
        xErrorLog.ui8ErrorCode = ui8ErrorCode;
        xErrorLog.ui32TimeStamp = GPTM_WTimer0Read() / 10;
        axErrorLogs[ui8LogIndex] = xErrorLog;
        ui8LogIndex = (++ui8LogIndex) % 30;
    }
}

void vWarningLedTask(void* pvParameters)
{
    SeatParams* xSeatParams = (SeatParams*) pvParameters;
    EventBits_t xEventGroupValue;
    for(;;)
    {
        xEventGroupValue = xEventGroupWaitBits(xSeatButtonEventGroup, xSeatParams->ui8SeatErrorFlag, pdTRUE, pdFALSE, portMAX_DELAY);
        if ((xEventGroupValue & (xSeatParams->ui8SeatErrorFlag)) != 0)
        {
            RGB_RedLedOn(xSeatParams->xRgbLed);
            RGB_GreenLedOff(xSeatParams->xRgbLed);
            RGB_BlueLedOff(xSeatParams->xRgbLed);
        }
    }
}

void vRunTimeMeasurementsTask(void *pvParameters)
{
    TickType_t xLastWakeTime = xTaskGetTickCount();
    for (;;)
    {
        uint8 ucCounter, ucCPU_Load;
        uint32 ullTotalTasksTime = 0;
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(5000));
        for(ucCounter = 1; ucCounter < 14; ucCounter++)
        {
            ullTotalTasksTime += ullTasksTotalTime[ucCounter];
        }
        ucCPU_Load = (ullTotalTasksTime * 100) /  GPTM_WTimer0Read();

        taskENTER_CRITICAL();
        UART0_SendString("CPU Load is ");
        UART0_SendInteger(ucCPU_Load);
        UART0_SendString("% \r\n");
        taskEXIT_CRITICAL();
    }
}

void GPIOPortF_Handler(void)
{
    BaseType_t pxHigherPriorityTaskWoken = pdFALSE;
    if(GPIO_PORTF_RIS_REG & (1<<0))           /* PF0 handler code */
    {
        xEventGroupSetBitsFromISR(xSeatButtonEventGroup, mainSEAT2_BUTTON_PRESS_FLAG,&pxHigherPriorityTaskWoken);
        GPIO_PORTF_ICR_REG   |= (1<<0);       /* Clear Trigger flag for PF0 (Interrupt Flag) */
    }
    else if(GPIO_PORTF_RIS_REG & (1<<4))      /* PF4 handler code */
    {
        xEventGroupSetBitsFromISR(xSeatButtonEventGroup, mainSEAT1_BUTTON_PRESS_FLAG,&pxHigherPriorityTaskWoken);
        GPIO_PORTF_ICR_REG   |= (1<<4);       /* Clear Trigger flag for PF4 (Interrupt Flag) */
    }
}

/*-----------------------------------------------------------*/
