#include "main.h"

#define SPI1_SS_ENABLE		1  // SPI1_NSS - PA0
#define SPI1_SS_DISABLE		0

extern UART_Handle_t huart3;

void SPI1_ss(uint8_t ss_enable);

char *tx_buf = "Ala ma kota\r\n";
char rx_buf[5];

int main(void)
{
	// float ADC_Value;

	SystemClockConfig();  // PLL ON, AHB = 120 MHz, APB1 = 30 MHz, APB2 = 60 MHz, timers x 2
	SystemCoreClockUpdate();  // Update SystemCoreClock variable

	GPIO_Config();
	SPI1_Config();
	UART3_Config();

	// power consumption mode in IDLE task - configUSE_IDLE_HOOK = 1

	while(1)
	{
		// GPIO_pin_toggle(GPIOB, GPIO_PIN_0);
		// GPIO_pin_write(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
		// GPIO_pin_write(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
		// for(uint32_t i = 0; i < 8000000; i++);
		// GPIO_pin_toggle(GPIOB, GPIO_PIN_0);
		// GPIO_pin_write(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);
		// GPIO_pin_write(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
		// for(uint32_t i = 0; i < 8000000; i++);
		UART_Receive(&huart3, (uint8_t*)rx_buf, 5);

		if(rx_buf[0] == '1' && rx_buf[4] == 'f')
		{
			UART_Transmit(&huart3, (uint8_t*)"abcde", 5);

		}

	}
}


void SPI1_ss(uint8_t ss_enable)
{
	if(ss_enable)
	{
		GPIOA->ODR &= ~GPIO_ODR_OD0;
	}
	else
	{
		while(SPI1->SR & SPI_SR_BSY);
		GPIOA->ODR |= GPIO_ODR_OD0;
	}
}

