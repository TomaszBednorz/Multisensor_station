#include "spi.h"

/**
  * @brief SPI configuration function

  * @param  SPIx where x can be (1..6) to select the SPI peripheral
  * @param  SPI_Init struct SPI_Init_t from spi.h

  * @retval None
  */
void SPI_Init(SPI_TypeDef *SPIx, SPI_Init_t SPI_Init)
{
    // Boudrate divider
    SPIx->CR1 &= ~SPI_CR1_BR;
    SPIx->CR1 |= SPI_Init.baudrate_div << SPI_CR1_BR_Pos;

    //Clock polarity
    SPIx->CR1 &= ~SPI_CR1_CPOL;
    SPIx->CR1 |= SPI_Init.cpol << SPI_CR1_CPOL_Pos;

    // Clock phase
    SPIx->CR1 &= ~SPI_CR1_CPHA;
    SPIx->CR1 |= SPI_Init.cpha << SPI_CR1_CPHA_Pos;

    // Data frame format
    SPIx->CR1 &= ~SPI_CR1_DFF;
    SPIx->CR1 |= SPI_Init.dff << SPI_CR1_DFF_Pos;

    // First bit
    SPIx->CR1 &= ~SPI_CR1_LSBFIRST;
    SPIx->CR1 |= SPI_Init.first_bit << SPI_CR1_LSBFIRST_Pos;

    // Software slave managment
    SPIx->CR1 &= ~SPI_CR1_SSM;
    SPIx->CR1 |= SPI_Init.ssm << SPI_CR1_SSM_Pos;

    // Software slave managment
    SPIx->CR1 &= ~SPI_CR1_SSI;
    SPIx->CR1 |= SPI_Init.ssi << SPI_CR1_SSI_Pos;

    // Frame format
    SPIx->CR2 &= ~SPI_CR2_FRF;
    SPIx->CR2 |= SPI_Init.frf << SPI_CR2_FRF_Pos;

    // Slave / master mode
    SPIx->CR1 &= ~SPI_CR1_MSTR;
    SPIx->CR1 |= SPI_Init.mode << SPI_CR1_MSTR_Pos;

    // Enable SPI
    SPIx->CR1 |= SPI_CR1_SPE;
}

/**
  * @brief SPI read and write function

  * @param  SPIx where x can be (1..6) to select the SPI peripheral
  * @param  data 16 bit data to write

  * @retval 16 bit data to read
  */
uint16_t SPI_read_write(SPI_TypeDef *SPIx, uint16_t data)
{
    while(!(SPIx->SR & SPI_SR_TXE));
    SPIx->DR = data;
    while(!(SPIx->SR & SPI_SR_RXNE));
    data = SPIx->DR;
    return data;
}

/**
  * @brief SPI read function

  * @param  SPIx where x can be (1..6) to select the SPI peripheral

  * @retval 16 bit data to read
  */
uint16_t SPI_read(SPI_TypeDef *SPIx)
{

    while(!(SPIx->SR & SPI_SR_TXE));
    SPIx->DR = 0x0f;  // Send dummy data
    while(!(SPIx->SR & SPI_SR_RXNE));
    return SPIx->DR;
}

/**
  * @brief SPI write function

  * @param  SPIx where x can be (1..6) to select the SPI peripheral
  * @param  data 16 bit data to write

  * @retval None
  */
void SPI_write(SPI_TypeDef *SPIx, uint16_t data)
{
    while(!(SPIx->SR & SPI_SR_TXE));
    SPIx->DR = data;
}
