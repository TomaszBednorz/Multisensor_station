#include "config.h"

UART_Handle_t huart3;


void SystemClockConfig(void)
{
	// Enable HSE clock
	RCC->CR |= RCC_CR_HSEON;  // HSE (8 MHz) enable
	while(!(RCC->CR & RCC_CR_HSERDY));  // Wait for HSE ready

	FLASH->ACR |= FLASH_ACR_LATENCY_3WS;  // Flash 3 wait states (4 clock cycle)

	// Buses prescalers
	// HPRE = 0000 :  AHB bus : /1
	RCC->CFGR |= RCC_CFGR_PPRE1_0 | RCC_CFGR_PPRE1_2;  // APB1 bus : /4
	RCC->CFGR |= RCC_CFGR_PPRE2_2;  // APB2 bus : /2 
	// APB1 timers = APB1 x 2, APB2 timers = APB2 x 2

	// PLL (Phase-locked loop) configuration
	RCC->PLLCFGR &= ~(0x3F);  // Clear PLLM bits
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLM_2;  // PLLM = 4

	RCC->PLLCFGR &= ~(0x1FF << 6);  // Clear PLLN bits
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLN_3 | RCC_PLLCFGR_PLLN_4 | RCC_PLLCFGR_PLLN_5 | RCC_PLLCFGR_PLLN_6;  // PLLN = 120

	RCC->PLLCFGR &= ~(0x03 << 16);  // Clear PLLP bits
	RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLP_0 | RCC_PLLCFGR_PLLP_1);  // PLLP = 2

	RCC->PLLCFGR &= ~(0x0F << 24);  // Clear PLLQ bits
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLQ_0 | RCC_PLLCFGR_PLLQ_2;  // PLLQ = 5

	// SYSCLK (PLL) = HSE / PLLM * PLLN / PLLP = 120 MHz
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSE;  // HSE (8 MHz) - entry clock source

	RCC->CR |= RCC_CR_PLLON;  // Main PLL enable
	while(!(RCC->CR & RCC_CR_PLLRDY));  // Wait for PLL ready

	RCC->CFGR |= RCC_CFGR_SW_1;  // Switch system clock to PLL
	while(!(RCC->CFGR & RCC_CFGR_SWS_1));  // Wait for information, that PLL is system clock

	RCC->CR &= ~RCC_CR_HSION;  // Disable HSI
}

void GPIO_Config(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIODEN;  // Enable peripherals clocks

	GPIO_Init_t GPIO_Init = {0};
	
	// LED pin configuration

	// LD1 - PB0
	GPIO_Init.mode = GPIO_MODE_OUTPUT;
	GPIO_Init.pin = GPIO_PIN_0;
	GPIO_pin_config(GPIOB, GPIO_Init);

	// LD2 - PB7
	GPIO_Init.pin = GPIO_PIN_7;
	GPIO_pin_config(GPIOB, GPIO_Init);

	// LD3 - PB14
	GPIO_Init.pin = GPIO_PIN_14;
	GPIO_pin_config(GPIOB, GPIO_Init);

	// SPI1 pin configuration

	// SPI1_NSS - PA0
	GPIO_Init.pin = GPIO_PIN_0;
	GPIO_Init.mode = GPIO_MODE_OUTPUT;
	GPIO_pin_config(GPIOA, GPIO_Init);

	// SPI1_SCK - PA5
	GPIO_Init.pin = GPIO_PIN_5;
	GPIO_Init.mode = GPIO_MODE_ALT_FUN;
	GPIO_Init.alt_fun = GPIO_ALT_FUN_5;
	GPIO_pin_config(GPIOA, GPIO_Init);

	// SPI1_MISO - PA6
	GPIO_Init.pin = GPIO_PIN_6;
	GPIO_Init.mode = GPIO_MODE_ALT_FUN;
	GPIO_Init.alt_fun = GPIO_ALT_FUN_5;
	GPIO_pin_config(GPIOA, GPIO_Init);

	// SPI1_MOSI - PA7
	GPIO_Init.pin = GPIO_PIN_7;
	GPIO_Init.mode = GPIO_MODE_ALT_FUN;
	GPIO_Init.alt_fun = GPIO_ALT_FUN_5;
	GPIO_pin_config(GPIOA, GPIO_Init);

	// UART3 pin configuration

	// UART2_TX - PD8
	GPIO_Init.pin = GPIO_PIN_8;
	GPIO_Init.mode = GPIO_MODE_ALT_FUN;
	GPIO_Init.alt_fun = GPIO_ALT_FUN_7;
	GPIO_pin_config(GPIOD, GPIO_Init);

	// UART2_RX - PPD9
	GPIO_Init.pin = GPIO_PIN_9;
	GPIO_Init.mode = GPIO_MODE_ALT_FUN;
	GPIO_Init.alt_fun = GPIO_ALT_FUN_7;
	GPIO_pin_config(GPIOD, GPIO_Init);


}

void SPI1_Config(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

	SPI_Init_t SPI_Init_struct = {0};

	SPI_Init_struct.baudrate_div = SPI_BR_DIV_256; // 60 Mhz / 256 = 0.234 MHz
	SPI_Init_struct.dff = SPI_DFF_16BIT;
	SPI_Init_struct.first_bit = SPI_SPI_FIRST_BIT_MSB;
	SPI_Init_struct.ssm = SPI_SSM_ENABLE;
	SPI_Init_struct.ssi = SPI_SSI_1;
	SPI_Init_struct.mode = SPI_MODE_MASTER;
	SPI_Init(SPI1, SPI_Init_struct);
}

void UART3_Config(void)
{
	RCC->APB1ENR |= RCC_APB1ENR_USART3EN;

	huart3.Instance = USART3;
	huart3.Init.word_length = UART_WORD_LENGTH_8;
	huart3.Init.stop_bits = UART_UART_STOP_BITS_1;
	huart3.Init.oversampling = UART_OVERSAMPLING_8;
	huart3.Init.boudrate = UART_BOUDRATE_115200;

	UART_Init(&huart3);

	NVIC_EnableIRQ(USART3_IRQn);
	NVIC_SetPriority(0, 0);
}
