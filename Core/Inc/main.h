#ifndef __MAIN_H
#define __MAIN_H

#include <stm32f4xx.h>
#include <system_stm32f4xx.h>

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

#include "config.h"
#include "gpio.h"
#include "spi.h"
#include "uart.h"
#include "adc.h"
#include "rtc.h"

#include "lps25h.h"

// Events macros
#define EVENT_ALARM_A       0x01
#define EVENT_ALARM_B       0x02
#define EVENT_BUTTON_IT     0x03


typedef struct
{
    uint8_t payload[10];
    uint8_t length;
}command_t;

typedef enum
{
    sMainMenu = 0,
    sLedEffect,
    sTemperature,
    sPressure,
    sLighting,
    sTimeDateConfig,
    sTimeConfig,
    sDateConfig,
    sAlarmConfig,
    sHelp,
}state_t;

state_t currentState;  // This variable store actual state of program


// Queues
QueueHandle_t queueRxData;  // This queue store data received from UART ISR
QueueHandle_t queuePrint;   // This queue store data to print by taskPrint
QueueHandle_t queueEvents;  // This queue store events like alarms and button interrupts

// Timers
TimerHandle_t softwareTimerLeds[3];

// Task handles
TaskHandle_t taskMenuHandle; 
TaskHandle_t taskCommandHandle; 
TaskHandle_t taskPrintHandle; 
TaskHandle_t taskLedsHandle; 
TaskHandle_t taskTemperatureHandle; 
TaskHandle_t taskPressureHandle; 
TaskHandle_t taskLightingHandle; 
TaskHandle_t taskTimeDateHandle; 
TaskHandle_t taskTimeConfigHandle; 
TaskHandle_t taskDateConfigHandle; 
TaskHandle_t taskAlarmConfigHandle; 
TaskHandle_t taskEventTriggeredHandle; 
TaskHandle_t taskHelpHandle; 
TaskHandle_t taskPotentiometerHandle; 

// Tasks
void taskCommandHandler(void* parameters);
void taskPrint(void* parameters);
void taskMenu(void* parameters);
void taskLeds(void* parameters);
void taskTemperature(void* parameters);
void taskPressure(void* parameters);
void taskLighting(void* parameters);
void taskTimeDate(void* parameters);
void taskTimeConfig(void* parameters);
void taskDateConfig(void* parameters);
void taskAlarmConfig(void* parameters);
void taskEventTriggered(void* parameters);
void taskHelp(void* parameters);
void taskPotentiometer(void* parameters);
void vApplicationIdleHook(void);


// Functions prototypes

// General functions
void printMsg(char* msg);
void unpackCommand(command_t* command);
void handleCommand(command_t* command);

// Functions associate with taskLeds
void stopLedEffect(void);
void activateLedEffect(uint8_t ledEffect);
void ledEffect1(void);
void ledEffect2(void);
void ledEffect3(void);

// Functions associate with taskTimeDate
void showTimeDateMenu(void);
void showTimeDate(void);
uint8_t getNumber(command_t* command);

// Functions associate with sensors
void showTemperature(void);
void showPressure(void);
void showLighting(void);

// Callback
void ledsCallback(TimerHandle_t xTimer);


#endif /* __MAIN_H */