#ifndef __IT_H
#define __IT_H

#include "main.h"

void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void DebugMon_Handler(void);

void USART3_IRQHandler(void);
void ADC_IRQHandler(void);
void RTC_Alarm_IRQHandler(void);

#endif /* __IT_H */
