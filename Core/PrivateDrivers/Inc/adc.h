#ifndef __ADC_H
#define __ADC_H

#include <stm32f4xx.h>
#include <stdint.h>

// ADC parameters

// @ADC_RES_RANGE
#define ADC_RES_RANGE_12_BIT            4095
#define ADC_RES_RANGE_10_BIT            1023
#define ADC_RES_RANGE_8_BIT             255
#define ADC_RES_RANGE_6_BIT             63

#define ADC_MAX_VOLTAGE                 3.3f

// Macros to ADC_Init_t structure

// @ADC_RESOLUTION
#define ADC_RESOLUTION_12_BIT           0x00
#define ADC_RESOLUTION_10_BIT           0x01
#define ADC_RESOLUTION_8_BIT            0x02
#define ADC_RESOLUTION_6_BIT            0x03

// @ADC_MODE
#define ADC_MODE_SINGLE_CONVERSION      0x00
#define ADC_MODE_CONT_MODE              0x01  // Continuous mode

// @ADC_SAMPLE
#define ADC_SAMPLE_3_CYCLES             0x00
#define ADC_SAMPLE_15_CYCLES            0x01
#define ADC_SAMPLE_28_CYCLES            0x02
#define ADC_SAMPLE_56_CYCLES            0x03
#define ADC_SAMPLE_84_CYCLES            0x04
#define ADC_SAMPLE_112_CYCLES           0x05
#define ADC_SAMPLE_144_CYCLES           0x06
#define ADC_SAMPLE_480_CYCLES           0x07

typedef struct{
    uint8_t resolution;     // Resolution, @ADC_RESOLUTION
    uint8_t mode;           // Mode, @ADC_MODE
    uint8_t sample_time;    // Sample time, @ADC_SAMPLE
    uint8_t channel;        // Channel, number form 0 to 18
}ADC_Init_t;

typedef struct{
    ADC_TypeDef *Instance;  // ADC registers base address
    ADC_Init_t  Init;       // Init parameters
}ADC_Handle_t;

// Function prototypes
void ADC_Init(ADC_Handle_t *hadc);
void ADC_Start(ADC_Handle_t *hadc);
void ADC_StartIT(ADC_Handle_t *hadc);
void ADC_PollForConversion(ADC_Handle_t *hadc);
uint16_t ADC_Read(ADC_Handle_t *hadc);
float ADC_ConvToVoltage(uint16_t Value, uint16_t Resolution);

// Callbacks
void ADC_Callback(ADC_Handle_t *hadc);
void ADC_ConvCompleteCallback(ADC_Handle_t *hadc);

#endif /* __ADC_H */