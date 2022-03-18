#include "gpio.h"

/**
  * @brief Pin configuration function

  * @param  GPIOx where x can be (A..K) to select the GPIO peripheral
  * @param  GPIO_Init struct GPIO_Init_t from gpio.h

  * @retval None
  */
void GPIO_pin_config(GPIO_TypeDef *GPIOx, GPIO_Init_t GPIO_Init)
{
	uint8_t pin_num = GPIO_Init.pin;

	GPIOx->MODER &= ~(0x03 << pin_num * 2);
	GPIOx->MODER |= (GPIO_Init.mode << pin_num * 2);

	GPIOx->OTYPER &= ~(0x01 << pin_num);
	GPIOx->OTYPER |= (GPIO_Init.output_type << pin_num);

	GPIOx->OSPEEDR &= ~(0x03 << pin_num * 2);
	GPIOx->OSPEEDR |= (GPIO_Init.speed << pin_num * 2);

	GPIOx->PUPDR &= ~(0x03 << pin_num * 2);
	GPIOx->PUPDR |= (GPIO_Init.pupd << pin_num * 2);

	if(pin_num <= 7)
	{
		GPIOx->AFR[0] &= ~(0x07 << pin_num * 4);
		GPIOx->AFR[0] |= (GPIO_Init.alt_fun << pin_num * 4);
	}
	else
	{
		pin_num -= 8;
		GPIOx->AFR[1] &= ~(0x07 << pin_num * 4);
		GPIOx->AFR[1] |= (GPIO_Init.alt_fun << pin_num * 4);
	}
}

/**
  * @brief Sets or clears the selected data port bit

  * @param  GPIOx where x can be (A..K) to select the GPIO peripheral
  * @param  GPIO_Init pin, @GPIO_PIN from gpio.h
  * @param  PinState set or reset, @GPIO_PIN_STATE from gpio.h

  * @retval None
  */
void GPIO_pin_write(GPIO_TypeDef *GPIOx, uint8_t GPIO_Pin, uint8_t PinState)
{
	if(PinState)
	{
		GPIOx->ODR |= (1 << GPIO_Pin);
	}
	else
	{
		GPIOx->ODR &= ~(1 << GPIO_Pin);
	}
}

/**
  * @brief Toggles the selected data port bit

  * @param  GPIOx where x can be (A..K) to select the GPIO peripheral
  * @param  GPIO_Init struct GPIO_Init_t from gpio.h

  * @retval None
  */
void GPIO_pin_toggle(GPIO_TypeDef *GPIOx, uint8_t GPIO_Pin)
{
	GPIOx->ODR ^= (1 << GPIO_Pin);
}