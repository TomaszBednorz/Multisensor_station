#include "uart.h"

// Static functions prototypes
static uint16_t FreqToBRRVal(uint8_t boudrate, uint8_t over8);

extern uint32_t SystemCoreClock;
static uint8_t APB1PrescTable[8]  = {0, 0, 0, 0, 2, 4, 8, 16};


void UART_Init(UART_Handle_t *huart)
{
    huart->Instance->CR1 |= USART_CR1_UE;  // Enable UART

    // Word length
    huart->Instance->CR1 &= ~USART_CR1_M;
    huart->Instance->CR1 |=  huart->Init.word_length << USART_CR1_M_Pos;

    // Stop bits
    huart->Instance->CR2 &= ~USART_CR2_STOP;
    huart->Instance->CR2 |= huart->Init.stop_bits << USART_CR2_STOP_Pos;

    // Oversampling
    huart->Instance->CR1 &= ~USART_CR1_OVER8;
    huart->Instance->CR1 |= huart->Init.oversampling << USART_CR1_OVER8_Pos;

    // Boudrate
    huart->Instance->BRR &= ~(USART_BRR_DIV_Fraction_Msk | USART_BRR_DIV_Mantissa_Msk);
    huart->Instance->BRR |= FreqToBRRVal(huart->Init.boudrate, huart->Init.oversampling);

    // Inrerrupt mode
    if(huart->Init.mode == UART_MODE_INTERRUPT)
    {
        huart->Instance->CR1 |= USART_CR1_TXEIE | USART_CR1_TCIE | USART_CR1_RXNEIE;  // Enable interrupts
        NVIC_EnableIRQ(USART3_IRQn);
        NVIC_SetPriority(0, 0);
    }


    // Transmitter & Receiver enable
    huart->Instance->CR1 |= USART_CR1_TE| USART_CR1_RE;
}

void UART_Transmit(UART_Handle_t *huart, uint8_t *pData, uint16_t Size)
{
    for(uint16_t i = 0; i < Size; i++)
    {
        while(!(huart->Instance->SR & USART_SR_TXE));  // Next data can be written to data register
        huart->Instance->DR = pData[i];
    }
    while(!(huart->Instance->SR & USART_SR_TC));  // Transmision complete
}

void UART_Receive(UART_Handle_t *huart, uint8_t *pData, uint16_t Size)
{
    for(uint16_t i = 0; i < Size; i++)
    {
        while(!(huart->Instance->SR & USART_SR_RXNE));  // Next data can be read from data register
        pData[i] = huart->Instance->DR;
    }
}



// It works only for UART: 2, 3, 4, 5, 7, 8 (APB1 line)
static uint16_t FreqToBRRVal(uint8_t boudrate, uint8_t over8)
{
    uint16_t brr_val, div_mantissa = 0, div_fraction = 0;
    float usart_div;

    uint32_t boudrate_val = (boudrate == 0) ? 9600 : 115200;  // Desired boudrate
    uint32_t apb1_freq = SystemCoreClock / APB1PrescTable[((RCC->CFGR & RCC_CFGR_PPRE1_Msk) >> RCC_CFGR_PPRE1_Pos) & 0x07];  /// APB1 bus frequency

    usart_div = (float)apb1_freq / (boudrate_val * 8 * (2 - over8));  // USART devider before converting to fraction and mantissa

    if(over8)
    {
        div_fraction = (uint16_t)round(8 * (float)(usart_div - (int)usart_div));  // (Fractional part of usart_div) * 8
        div_mantissa = (uint16_t)usart_div;                                       // Integral part of usart_div

        if(div_fraction & 0x04)  // Carry bit must be added up to the mantissa
        {
            div_mantissa += 1;
            div_fraction &= ~0x04;
        }
    }
    else
    {
        div_fraction = (uint16_t)round(16 * (float)(usart_div - (int)usart_div));  // (Fractional part of usart_div) * 16
        div_mantissa = (uint16_t)usart_div;                                        // Integral part of usart_div

        if(div_fraction & 0x08)  // Carry bit must be added up to the mantissa
        {
            div_mantissa += 1;
            div_fraction &= ~0x08;
        }
    }

    brr_val = ((div_mantissa << 4) & 0xFFF0) | (div_fraction & 0x000F);

    return brr_val;
}

void UART_TransmitIT(UART_Handle_t *huart, uint8_t *pData, uint16_t Size)
{

}

void UART_ReceiveIT(UART_Handle_t *huart, uint8_t *pData, uint16_t Size)
{

}


void UART_Callback(UART_Handle_t *huart)
{

}




__attribute__((weak)) void UART_TxCompleteCallback(UART_Handle_t *huart)
{
    // Implement this callback in user file without weak attribute if needed
}

__attribute__((weak)) void UART_RxCompleteCallback(UART_Handle_t *huart)
{
    // Implement this callback in user file without weak attribute if needed
}