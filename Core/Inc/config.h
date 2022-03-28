#ifndef __CONFIG_H
#define __CONFIG_H

#include "main.h"



// Function prototypes
void SystemClockConfig(void);
void FPU_Enable(void);
void GPIO_Config(void);
void SPI1_Config(void);
void UART3_Config(void);

void LPS25H_Config(void);

#endif /* __CONFIG_H */
