#ifndef __SPI_H
#define __SPI_H

#include <stm32f4xx.h>
#include <stdint.h>


// @SPI_BR_DIVIDER
#define SPI_BR_DIV_2            0x00
#define SPI_BR_DIV_4            0x01
#define SPI_BR_DIV_8            0x02
#define SPI_BR_DIV_16           0x03
#define SPI_BR_DIV_32           0x04
#define SPI_BR_DIV_64           0x05
#define SPI_BR_DIV_128          0x06
#define SPI_BR_DIV_256          0x07

// @SPI_CPOL
#define SPI_CPOL_CK0            0x00  // CK to 0 when idle
#define SPI_CPOL_CK1            0x01  // CK to 1 when idle

// @SPI_CPHA
#define SPI_CPHA_1ST            0x00  // 1st clock - 1st data capture edge
#define SPI_CPHA_2ND            0x01  // 2nd clock - 1st data capture edge

// @SPI_DFF
#define SPI_DFF_8BIT            0x00
#define SPI_DFF_16BIT           0x01

// @SPI_FIRST_BIT
#define SPI_SPI_FIRST_BIT_MSB   0x00
#define SPI_SPI_FIRST_BIT_LSB   0x01

// @SPI_SSM
#define SPI_SSM_DISABLE         0x00
#define SPI_SSM_ENABLE          0x01

// @SPI_SSI
#define SPI_SSI_0               0x00
#define SPI_SSI_1               0x01

// @SPI_FRF
#define SPI_FRF_MOTOROLA        0x00
#define SPI_FRF_TI_MODE         0x01

// @SPI_MODE
#define SPI_MODE_SLAVE          0x00
#define SPI_MODE_MASTER         0x01

typedef struct{
    uint8_t baudrate_div;   // Boudrate divider, @SPI_BR_DIVIDER
    uint8_t cpol;           // Clock polarity, @SPI_CPOL
    uint8_t cpha;           // Clock phase, @SPI_CPHA
    uint8_t dff;            // Data frame format, @SPI_DFF
    uint8_t first_bit;      // First bit, @SPI_FIRST_BIT
    uint8_t ssm;            // Software slave managment, @SPI_SSM    
    uint8_t ssi;            // Internal slave select, @SPI_SSI
    uint8_t frf;            // Frame format, @SPI_FRF
    uint8_t mode;           // Slave / master mode, @SPI_MODE
}SPI_Init_t;

typedef struct{
    SPI_TypeDef *Instance;  // UART registers base address
    SPI_Init_t  Init;       // Init parameters
}SPI_Handle_t;

// Function prototypes
void SPI_Init(SPI_Handle_t *hspi);
void SPI_TransmitReceive(SPI_Handle_t *hspi, uint8_t *TxData, uint8_t *RxData, uint16_t Size);
void SPI_Receive(SPI_Handle_t *hspi, uint8_t *RxData, uint16_t Size);
void SPI_Transmit(SPI_Handle_t *hspi, uint8_t *TxData, uint16_t Size);

#endif /* __SPI_H */