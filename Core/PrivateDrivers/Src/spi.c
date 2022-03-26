#include "spi.h"

/**
  * @brief SPI configuration function

  * @param  hspi is handle structure from spi.h

  * @retval None
  */
void SPI_Init(SPI_Handle_t *hspi)
{
    // Boudrate divider
    hspi->Instance->CR1 &= ~SPI_CR1_BR;
    hspi->Instance->CR1 |= hspi->Init.baudrate_div << SPI_CR1_BR_Pos;

    //Clock polarity
    hspi->Instance->CR1 &= ~SPI_CR1_CPOL;
    hspi->Instance->CR1 |= hspi->Init.cpol << SPI_CR1_CPOL_Pos;

    // Clock phase
    hspi->Instance->CR1 &= ~SPI_CR1_CPHA;
    hspi->Instance->CR1 |= hspi->Init.cpha << SPI_CR1_CPHA_Pos;

    // Data frame format
    hspi->Instance->CR1 &= ~SPI_CR1_DFF;
    hspi->Instance->CR1 |= hspi->Init.dff << SPI_CR1_DFF_Pos;

    // First bit
    hspi->Instance->CR1 &= ~SPI_CR1_LSBFIRST;
    hspi->Instance->CR1 |= hspi->Init.first_bit << SPI_CR1_LSBFIRST_Pos;

    // Software slave managment
    hspi->Instance->CR1 &= ~SPI_CR1_SSM;
    hspi->Instance->CR1 |= hspi->Init.ssm << SPI_CR1_SSM_Pos;

    // Software slave managment
    hspi->Instance->CR1 &= ~SPI_CR1_SSI;
    hspi->Instance->CR1 |= hspi->Init.ssi << SPI_CR1_SSI_Pos;

    // Frame format
    hspi->Instance->CR2 &= ~SPI_CR2_FRF;
    hspi->Instance->CR2 |= hspi->Init.frf << SPI_CR2_FRF_Pos;

    // Slave / master mode
    hspi->Instance->CR1 &= ~SPI_CR1_MSTR;
    hspi->Instance->CR1 |= hspi->Init.mode << SPI_CR1_MSTR_Pos;

    // Enable SPI
    hspi->Instance->CR1 |= SPI_CR1_SPE;
}

/**
  * @brief SPI transmit & receive function (pooling mode)

  * @param  hspi is handle structure from spi.h
  * @param  TxData pointer to data transmission handle
  * @param  RxData pointer to data reception handle
  * @param  Size Size of data to transmit

  * @retval None
  */
void SPI_TransmitReceive(SPI_Handle_t *hspi, uint8_t *TxData, uint8_t *RxData, uint16_t Size)
{
  for(uint16_t i = 0; i < Size; i++)
  {
    while(!(hspi->Instance->SR & SPI_SR_TXE));
    hspi->Instance->DR = TxData[i];
    while(!(hspi->Instance->SR & SPI_SR_RXNE));
    RxData[i] = hspi->Instance->DR;
  }
}

/**
  * @brief SPI receive function (pooling mode)

  * @param  hspi is handle structure from spi.h
  * @param  RxData pointer to data reception handle
  * @param  Size Size of data to transmit

  * @retval None
  */
void SPI_Receive(SPI_Handle_t *hspi, uint8_t *RxData, uint16_t Size)
{
  for(uint16_t i = 0; i < Size; i++)
  {
    while(!(hspi->Instance->SR & SPI_SR_TXE));
    hspi->Instance->DR = 0x0f;  // Send dummy data
    while(!(hspi->Instance->SR & SPI_SR_RXNE));
    RxData[i] = hspi->Instance->DR;
  }
}

/**
  * @brief SPI transmit function (pooling mode)

  * @param  hspi is handle structure from spi.h
  * @param  TxData pointer to data transmission handle
  * @param  Size Size of data to transmit

  * @retval None
  */
void SPI_Transmit(SPI_Handle_t *hspi, uint8_t *TxData, uint16_t Size)
{
  for(uint16_t i = 0; i < Size; i++)
  {
    while(!(hspi->Instance->SR & SPI_SR_TXE));
    hspi->Instance->DR = TxData[i];
    while(!(hspi->Instance->SR & SPI_SR_RXNE));
    hspi->Instance->DR;  // Read data without storing to variable
  }
}
