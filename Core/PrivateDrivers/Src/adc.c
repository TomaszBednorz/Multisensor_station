#include "adc.h"


/**
  * @brief ADC configuration function

  * @param  hadc is handle structure from afc.h

  * @retval None
  */
void ADC_Init(ADC_Handle_t *hadc)
{
    // Enable ADC
    hadc->Instance->CR2 |= ADC_CR2_ADON;

    // Resolution
    hadc->Instance->CR1 &= ~ADC_CR1_RES;
    hadc->Instance->CR1 |= hadc->Init.resolution << ADC_CR1_RES_Pos;

    // Mode
    hadc->Instance->CR2 &= ~ADC_CR2_CONT;
    hadc->Instance->CR2 |= hadc->Init.mode << ADC_CR2_CONT_Pos;

    // Sample time
	if(hadc->Init.channel <= 9)
	{
		hadc->Instance->SMPR1 &= ~(0x07 << hadc->Init.channel * 3);
		hadc->Instance->SMPR1 |= (hadc->Init.sample_time << hadc->Init.channel * 3);
	}
	else
	{
		hadc->Instance->SMPR2 &= ~(0x07 << (hadc->Init.channel - 10) * 3);
		hadc->Instance->SMPR2 |= (hadc->Init.sample_time << (hadc->Init.channel - 10) * 3);
	}

    // Channel
    hadc->Instance->SQR3 &= ADC_SQR3_SQ1;
    hadc->Instance->SQR3 |= hadc->Init.channel << ADC_SQR3_SQ1_Pos;
}

/**
  * @brief ADC start conversion (pooling mode)

  * @param  hadc is handle structure from afc.h

  * @retval None
  */
void ADC_Start(ADC_Handle_t *hadc)
{
    hadc->Instance->CR2 |= ADC_CR2_SWSTART;  // Start conversion of regular channels
}

/**
  * @brief ADC start conversion (interrupt mode)

  * @param  hadc is handle structure from afc.h

  * @retval None
  */
void ADC_StartIT(ADC_Handle_t *hadc)
{
    hadc->Instance->CR1 |= ADC_CR1_EOCIE;    // Interrupt is generated when end of conversion
    hadc->Instance->CR2 |= ADC_CR2_SWSTART;  // Start conversion of regular channels
}

/**
  * @brief ADC poll for conversion - wait for ready data in Data Register

  * @param  hadc is handle structure from afc.h

  * @retval None
  */
void ADC_PollForConversion(ADC_Handle_t *hadc)
{
    while(!(hadc->Instance->SR & ADC_SR_EOC));  // Wait for end of conversion
}

/**
  * @brief Read data from ADC data register

  * @param  hadc is handle structure from afc.h

  * @retval Voltage from data register as binary number
  */
uint16_t ADC_Read(ADC_Handle_t *hadc)
{
    uint16_t adc_value;

    adc_value = hadc->Instance->DR;

    return adc_value;
}

/**
  * @brief Convert raw data from ADC data register to voltage
  * @param  Value is data readed from ADC date ragister (from ADC_Read function)
  * @param  Resolution ADC resolution, @ADC_RES_RANGE in adc.h file

  * @retval Voltage (0 - 3.3 [V])
  */
float ADC_ConvToVoltage(uint16_t Value, uint16_t Resolution)
{
    float voltage;

    voltage = ((float)Value / Resolution) * ADC_MAX_VOLTAGE;

    return voltage;
}

/**
  * @brief ADC callback to handle received interrupts

  * @param  hadc is handle structure from adc.h

  * @retval None
  */
void ADC_Callback(ADC_Handle_t *hadc)
{
    if(hadc->Instance->SR & ADC_SR_EOC)
    {
        hadc->Instance->SR &= ~ADC_SR_EOC;
        ADC_ConvCompleteCallback(hadc);
    }
}

/**
  * @brief ADC conversion complete callback

  * @param  hadc is handle structure from adc.h

  * @retval None
  */
__attribute__((weak)) void ADC_ConvCompleteCallback(ADC_Handle_t *hadc)
{
    // Implement this callback in user file without weak attribute if needed
}