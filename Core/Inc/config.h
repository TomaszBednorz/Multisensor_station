#ifndef __CONFIG_H
#define __CONFIG_H

#include "main.h"

#define LED_USER_PORT	GPIOD
#define LED_USER_PIN	GPIO_PIN_0

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
