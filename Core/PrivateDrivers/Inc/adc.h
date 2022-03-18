#ifndef __ADC_H
#define __ADC_H

/*
 * Driver for peripherals: ADC, I2C, SPI and UART.
 */

#include <stm32f4xx.h>
#include <stdint.h>

#define ADC_DATA_REGISTER_MASK          0x0FFF    // Right alignment 12 bits in data register
#define ADC_RESOLUTION_RANGE            (4095)    // 2^12, 12-bit resolution
#define ADC_MAX_VOLTAGE                 (3.3f)    // [V]


// Function prototypes
void ADC1_Init(void);
void ADC1_ReadData(float *ADC_Data);

#endif /* __ADC_H */