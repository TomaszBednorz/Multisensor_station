#include "uart.h"

// Static functions prototypes
static uint16_t FreqToBRRVal(uint8_t boudrate, uint8_t over8);

extern uint32_t SystemCoreClock;
static uint8_t APB1PrescTable[8]  = {0, 0, 0, 0, 2, 4, 8, 16};

/**
  * @brief UART configuration function

  * @param  huart is handle structure from uart.h

  * @retval None
  */
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

    // Transmitter & Receiver enable
    huart->Instance->CR1 |= USART_CR1_TE| USART_CR1_RE;
}

/**
  * @brief UART transmit function (pooling mode)

  * @param  huart is handle structure from uart.h
  * @param  pData pointer to data transmission handle
  * @param  Size Size of data to transmit

  * @retval None
  */
void UART_Transmit(UART_Handle_t *huart, uint8_t *pData, uint16_t Size)
{
    for(uint16_t i = 0; i < Size; i++)
    {
        while(!(huart->Instance->SR & USART_SR_TXE));  // Next data can be written to data register
        huart->Instance->DR = pData[i];
    }
    while(!(huart->Instance->SR & USART_SR_TC));  // Transmision complete
}

/**
  * @brief UART receive function (pooling mode)

  * @param  huart is handle structure from uart.h
  * @param  pData pointer to data reception handle
  * @param  Size Size of data to reception

  * @retval None
  */
void UART_Receive(UART_Handle_t *huart, uint8_t *pData, uint16_t Size)
{
    for(uint16_t i = 0; i < Size; i++)
    {
        while(!(huart->Instance->SR & USART_SR_RXNE));  // Next data can be read from data register
        pData[i] = huart->Instance->DR;
    }
}

/**
  * @brief UART transmit function (interrupt mode)

  * @param  huart is handle structure from uart.h
  * @param  pData pointer to data transmission handle
  * @param  Size Size of data to transmit

  * @retval None
  */
void UART_TransmitIT(UART_Handle_t *huart, uint8_t *pData, uint16_t Size)
{
    huart->pTxBuf = pData;
    huart->TxSize = Size - 1; // Table size 
    huart->TxCnt = 0;
    huart->Instance->CR1 |= USART_CR1_TXEIE;  // Transmit data register empty - activate interrupt 
}

/**
  * @brief UART receive function (interrupt mode)

  * @param  huart is handle structure from uart.h
  * @param  pData pointer to data reception handle
  * @param  Size Size of data to reception

  * @retval None
  */
void UART_ReceiveIT(UART_Handle_t *huart, uint8_t *pData, uint16_t Size)
{
    huart->pRxBuf = pData;
    huart->RxSize = Size - 1;  // Table size
    huart->RxCnt = 0;
    huart->Instance->CR1 |= USART_CR1_RXNEIE;  // Read data register not empty - activate interrupt 
}

/**
  * @brief UART callback to handle received interrupts

  * @param  huart is handle structure from uart.h

  * @retval None
  */
void UART_Callback(UART_Handle_t *huart)
{
    if((huart->Instance->SR & USART_SR_TXE) && (huart->Instance->CR1 & USART_CR1_TXEIE))  // Handle transmision
    {
        if(huart->TxCnt < huart->TxSize)
        {
            huart->Instance->DR = huart->pTxBuf[huart->TxCnt++];  // Write to data register and increment counter
        }
        else if(huart->TxCnt == huart->TxSize)
        {
            huart->Instance->DR = huart->pTxBuf[huart->TxCnt];  // Write to data register
            huart->Instance->CR1 &= ~USART_CR1_TXEIE;  // Transmit data register empty - disable interrupt
            UART_TxCompleteCallback(huart);  // Transmiscion complete callback
            
        }
    }

    if((huart->Instance->SR & USART_SR_RXNE) && (huart->Instance->CR1 & USART_CR1_RXNEIE))  // Handle reception
    {
        if(huart->RxCnt < huart->RxSize)
        {
            huart->pRxBuf[huart->RxCnt++] = huart->Instance->DR;  // Read from data register and increment counter
        }
        else if (huart->RxCnt == huart->RxSize)
        {
            huart->pRxBuf[huart->RxCnt] = huart->Instance->DR;  // Read from data register
            huart->Instance->CR1 &= ~USART_CR1_RXNEIE;  // Read data register not empty - disable interrupt 
            UART_RxCompleteCallback(huart);  // Reception complete callback
            
        }
    }
}

/**
  * @brief UART transmision complete callback

  * @param  huart is handle structure from uart.h

  * @retval None
  */
__attribute__((weak)) void UART_TxCompleteCallback(UART_Handle_t *huart)
{
    // Implement this callback in user file without weak attribute if needed
}

/**
  * @brief UART reception complete callback

  * @param  huart is handle structure from uart.h

  * @retval None
  */
__attribute__((weak)) void UART_RxCompleteCallback(UART_Handle_t *huart)
{
    // Implement this callback in user file without weak attribute if needed
}



/**
  * @brief Function which  calculate value to store in UART BRR register

  * @param  boudrate boud rate to achieve
  * @param  over8 oversampling

  * @retval Value to store in UART BRR register
  */
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
