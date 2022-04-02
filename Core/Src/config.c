#include "config.h"

UART_Handle_t huart3;
SPI_Handle_t hspi1;
ADC_Handle_t hadc1;
ADC_Handle_t hadc2;
RTC_Handle_t hrtc;

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

void FPU_Enable(void)
{
	SCB->CPACR |= ((3 << 10*2)|(3 << 11*2));
}

void GPIO_Config(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOCEN |RCC_AHB1ENR_GPIODEN;  // Enable peripherals clocks
	RCC->APB2ENR |=RCC_APB2ENR_SYSCFGEN;

	GPIO_Init_t GPIO_Init = {0};
	
	// External LED pin configuration

	// USER_LED - PD0
	GPIO_Init.mode = GPIO_MODE_OUTPUT;
	GPIO_Init.pin = LED_USER_PIN;
	GPIO_pin_config(LED_USER_PORT, GPIO_Init);

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

	// UART2_RX - PD9
	GPIO_Init.pin = GPIO_PIN_9;
	GPIO_Init.mode = GPIO_MODE_ALT_FUN;
	GPIO_Init.alt_fun = GPIO_ALT_FUN_7;
	GPIO_pin_config(GPIOD, GPIO_Init);

	// ADC pin configuration

	// ADC1_CHANNEL10 - PC0 : fotoresistor
	GPIO_Init.pin = GPIO_PIN_0;
	GPIO_Init.mode = GPIO_MODE_ANALOG;
	GPIO_pin_config(GPIOC, GPIO_Init);

	// ADC2_CHANNEL3 - PA3 : potentiometer
	GPIO_Init.pin = GPIO_PIN_3;
	GPIO_Init.mode = GPIO_MODE_ANALOG;
	GPIO_pin_config(GPIOA, GPIO_Init);

	// External button configuration

	// Button - PB10
	GPIO_Init.pin = GPIO_PIN_10;
	GPIO_Init.mode = GPIO_MODE_IT_RISING;
	GPIO_pin_config(GPIOB, GPIO_Init);

	NVIC_EnableIRQ(EXTI15_10_IRQn);
	NVIC_SetPriority(0, 0);

}

void SPI1_Config(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

	hspi1.Instance = SPI1;

	hspi1.Init.baudrate_div = SPI_BR_DIV_256; // 60 Mhz / 256 = 0.234 MHz
	hspi1.Init.dff = SPI_DFF_8BIT;
	hspi1.Init.first_bit = SPI_SPI_FIRST_BIT_MSB;
	hspi1.Init.ssm = SPI_SSM_ENABLE;
	hspi1.Init.ssi = SPI_SSI_1;
	hspi1.Init.mode = SPI_MODE_MASTER;
	SPI_Init(&hspi1);
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

void ADC1_Config(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

	hadc1.Instance = ADC1;
	hadc1.Init.resolution = ADC_RESOLUTION_12_BIT;
	hadc1.Init.mode = ADC_MODE_SINGLE_CONVERSION;
	hadc1.Init.sample_time = ADC_SAMPLE_28_CYCLES;
	hadc1.Init.channel = 10;

	ADC_Init(&hadc1);

	NVIC_EnableIRQ(ADC_IRQn);
	NVIC_SetPriority(0, 0);
}

void ADC2_Config(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_ADC2EN;

	hadc2.Instance = ADC2;
	hadc2.Init.resolution = ADC_RESOLUTION_12_BIT;
	hadc2.Init.mode = ADC_MODE_SINGLE_CONVERSION;
	hadc2.Init.sample_time = ADC_SAMPLE_28_CYCLES;
	hadc2.Init.channel = 3;

	ADC_Init(&hadc2);

	NVIC_EnableIRQ(ADC_IRQn);
	NVIC_SetPriority(0, 0);
}

void RTC_Config(void)
{
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;  // PWR clock enable
	
    PWR->CR |= PWR_CR_DBP;  // Disable backup domain write protection
	for(uint32_t i = 0; i < 100; i++);  // Wait for activation above line (didn't find status register)

	RCC->BDCR |= RCC_BDCR_RTCEN;  // Enable RTC clock
	RCC->BDCR |= RCC_BDCR_RTCSEL;  // HSE / RTCPRE <- RTC clock source
	RCC->CFGR |= 0x08 << RCC_CFGR_RTCPRE_Pos;  // RTCPRE = 8, RTC clock source = 1 MHz
	
	hrtc.Instance = RTC;
	hrtc.Init.format = RTC_FORMAT_24_HOURS;
	hrtc.Init.pre_div_A = 99;
	hrtc.Init.pre_div_S = 9999;  // RTC clock source / ((99 + 1) + (9999 + 1)) = 1 Hz

	RTC_Init(&hrtc);

	NVIC_EnableIRQ(RTC_Alarm_IRQn);
	NVIC_SetPriority(0, 0);

	PWR->CR &= ~PWR_CR_DBP;  // Enable backup domain write protection
}

void LPS25H_Config(void)
{
	LPS25H_Init_t LPS25H_Init = {0};
	LPS25H_Init.interrupt = LPS25H_INT_DISABLE;
	LPS25H_Init.odr = LPS25H_ODR_25HZ;
	LPS25H_Init.fifo = LPS25H_FIFO_DISABLE;

	if(lps25h_Init(LPS25H_Init) == LPS25H_ERROR)
	{
		while(1);  // Wrong initialization
	}
}