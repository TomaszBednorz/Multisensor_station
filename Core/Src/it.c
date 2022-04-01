#include "it.h"

extern UART_Handle_t huart3;
extern ADC_Handle_t hadc1;
extern RTC_Handle_t hrtc;

/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  while (1)
  {

  }
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  while (1)
  {

  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  while (1)
  {

  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{

  while (1)
  {

  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  while (1)
  {

  }
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  while (1)
  {

  }
}

/**
  * @brief This function handles USART3 interrupts.
  */
void USART3_IRQHandler(void)
{
  UART_Callback(&huart3);
}

/**
  * @brief This function handles ADC1,2,3 interrupts.
  */
void ADC_IRQHandler(void)
{
  ADC_Callback(&hadc1);
}

/**
  * @brief This function handles RTC Alarm A and B interrupts.
  */
void RTC_Alarm_IRQHandler(void)
{
  RTC_Callback(&hrtc);
}
