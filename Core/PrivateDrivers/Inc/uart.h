#ifndef __UART_H
#define __UART_H

#include <stm32f4xx.h>
#include <stdint.h>
#include <math.h>

// @UART_WORD_LENGTH
#define UART_WORD_LENGTH_8          0x00        // 8 data bits, 1 start bit
#define UART_WORD_LENGTH_9          0x01        // 9 data bits, 1 start bit

// @UART_STOP_BITS
#define UART_UART_STOP_BITS_1       0x00        // 1 stop bit 
#define UART_UART_STOP_BITS_0_5     0x01        // 0.5 stop bit 
#define UART_UART_STOP_BITS_2       0x02        // 2 stop bit 
#define UART_UART_STOP_BITS_1_5     0x03        // 1.5 stop bit 

// @UART_BOUDRATE
#define UART_BOUDRATE_9600          0x00
#define UART_BOUDRATE_115200        0x01

// @UART_OVERSAMPLING
#define UART_OVERSAMPLING_16        0x00
#define UART_OVERSAMPLING_8         0x01

// @UART_MODE
#define UART_MODE_POLLING           0x00        // Polling mode
#define UART_MODE_INTERRUPT         0x01        // Interrupt mode

typedef struct{
    uint8_t word_length;   // Word length, @UART_WORD_LENGTH
    uint8_t stop_bits;     // Stop bits, @UART_STOP_BITS
    uint8_t oversampling;  // Oversampling, @UART_OVERSAMPLING
    uint8_t boudrate;      // Boudrate, @UART_BOUDRATE
    uint8_t mode;          // UART mode, @UART_MODE
}UART_Init_t;

typedef struct{
    USART_TypeDef      *Instance;  // UART registers base address
    UART_Init_t        Init;      // Init parameters
    uint8_t            *pTxBuf;   // Transmit buffer (interrupt mode)
    uint16_t           TxSize;    // Number of transmisions (interrupt mode)
    volatile uint16_t  TxCnt;     // Counter of transmissions (interrupt mode)
    uint8_t            *pRxBuf;   // Receive buffer (interrupt mode)
    uint16_t           RxSize;    // Number of receptions (interrupt mode)
    volatile uint16_t  RxCnt;     // Counter of receptions (interrupt mode)

}UART_Handle_t;


// Function prototypes
void UART_Init(UART_Handle_t *huart);
void UART_Transmit(UART_Handle_t *huart, uint8_t *pData, uint16_t Size);
void UART_Receive(UART_Handle_t *huart, uint8_t *pData, uint16_t Size);
void UART_TransmitIT(UART_Handle_t *huart, uint8_t *pData, uint16_t Size);
void UART_ReceiveIT(UART_Handle_t *huart, uint8_t *pData, uint16_t Size);

// Callbacks
void UART_Callback(UART_Handle_t *huart);
void UART_TxCompleteCallback(UART_Handle_t *huart);
void UART_RxCompleteCallback(UART_Handle_t *huart);


#endif /* __UART_H */