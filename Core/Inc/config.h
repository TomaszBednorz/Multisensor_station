#ifndef __CONFIG_H
#define __CONFIG_H

#include "main.h"

// On board LEDs
#define LED1_PORT	GPIOB
#define LED1_PIN	GPIO_PIN_0

#define LED2_PORT	GPIOB
#define LED2_PIN	GPIO_PIN_7

#define LED3_PORT	GPIOB
#define LED3_PIN	GPIO_PIN_14

// External LED
#define LED4_PORT	GPIOD
#define LED4_PIN	GPIO_PIN_0


// Function prototypes
void SystemClockConfig(void);
void FPU_Enable(void);
void GPIO_Config(void);
void SPI1_Config(void);
void UART3_Config(void);
void ADC1_Config(void);
void ADC2_Config(void);
void RTC_Config(void);

void LPS25H_Config(void);

#endif /* __CONFIG_H */
