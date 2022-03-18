#include "adc.h"

/*
 * ADC1 initialization, single conversion mode, channel 3
 */ 
void ADC1_Init(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;      // Enable ADC1 clock

    // ADC Configuration
    ADC1->CR1 &= ~ADC_CR1_RES_Msk;           // Resolution: 12-bit
    ADC1->SMPR2 |= ADC_SMPR2_SMP3_0;         // Sampling time: 15 cycles
    ADC1->SQR1 &= ~ADC_SQR1_L_Msk;           // Regular channel: 1 conversion
    ADC1->SQR3 |= (3  << ADC_SQR3_SQ1_Pos);  // Channel: 3 - Pin PA3
    ADC1->CR2 |= ADC_CR2_ADON;               // Enable ADC
}
/*
 * Start ADC conversion, read data from data register and convert to voltage (store in ADC_Data)
 */
void ADC1_ReadData(float *ADC_Data)
{
    uint16_t ADC_Value;

    ADC1->CR2 |= ADC_CR2_SWSTART;                    // Start conversion
    while(!(ADC1->SR & ADC_SR_EOC));                 // Wait for conversion
    ADC_Value = ADC1->DR & ADC_DATA_REGISTER_MASK;   // Read data from data register
    *ADC_Data = (float)(ADC_Value * ADC_MAX_VOLTAGE) / ADC_RESOLUTION_RANGE;  // Convert data from data register to voltage
}