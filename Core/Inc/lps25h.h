/*
 *  LPS25H MEMS pressure sensor library
 *  SPI have to be configured in 8-bit mode
 *  Library is compatible with SPI && GPIO custom driver
 */

#ifndef __LPS25H_H_
#define __LPS25H_H_

#include <stdint.h>
#include "spi.h"
#include "gpio.h"

/* Configurable items */
extern SPI_Handle_t hspi1;
#define LPS25H_SPI_HANDLE    hspi1
#define LPS25H_SPI_NSS_PORT  GPIOA
#define LPS25H_SPI_NSS_PIN   GPIO_PIN_0


// Read/write macros
#define LPS25H_READ_WRITE_POS       7
#define LPS25H_INC_ADDR_POS         6

#define LPS25H_READ_MASK            (1 << LPS25H_READ_WRITE_POS)
#define LPS25H_WRITE_MASK           (0 << LPS25H_READ_WRITE_POS)
#define LPS25H_INC_ADDR_MASK        (1 << LPS25H_INC_ADDR_POS)
#define LPS25H_ADDRESS_MASK         0x3F

// LPS25H registers addresses
#define LPS25H_REF_P_XL             0x08
#define LPS25H_REF_P_L              0x09
#define LPS25H_REF_P_H              0x0A
#define LPS25H_WHO_AM_I             0x0F
#define LPS25H_RES_CONF             0x10
#define LPS25H_CTRL_REG1            0x20
#define LPS25H_CTRL_REG2            0x21
#define LPS25H_CTRL_REG3            0x22
#define LPS25H_CTRL_REG4            0x23
#define LPS25H_INT_CFG              0x24
#define LPS25H_INT_SOURCE           0x25
#define LPS25H_STATUS_REG           0x27
#define LPS25H_PRESS_POUT_XL        0x28
#define LPS25H_PRESS_OUT_L          0x29
#define LPS25H_PRESS_OUT_H          0x2A
#define LPS25H_TEMP_OUT_L           0x2B
#define LPS25H_TEMP_OUT_H           0x2C
#define LPS25H_FIFO_CTRL            0x2E
#define LPS25H_FIFO_STATUS          0x2F
#define LPS25H_THS_P_L              0x30
#define LPS25H_THS_P_H              0x31
#define LPS25H_RPDS_L               0x39
#define LPS25H_RPDS_H               0x3A

/*
 *  BIT POSITIONS MACROS
 */

// CTRL_REG1 bit positions macros
#define LPS25H_SIM_POS              0x00
#define LPS25H_RESET_POS            0x01
#define LPS25H_BDU_POS              0x02
#define LPS25H_DIFF_EN_POS          0x03
#define LPS25H_ODR_POS              0x04
#define LPS25H_PD_POS               0x07

// CTRL_REG2 bit positions macros
#define LPS25H_ONE_SHOT_POS         0x00
#define LPS25H_AUTO_ZERO_POS        0x01
#define LPS25H_SWRESET_POS          0x02
#define LPS25H_FIFO_MEAN_DEC_POS    0x04
#define LPS25H_WTM_EN_POS           0x05
#define LPS25H_FIFO_EN_POS          0x06
#define LPS25H_BOOT_POS             0x07

// CTRL_REG3 bit positions macros
#define LPS25H_INT1_S_POS           0x00
#define LPS25H_PP_OD_POS            0x06
#define LPS25H_INT_H_L_POS          0x07

// CTRL_REG4 bit positions macros
#define LPS25H_P1_INT_CONFIG_POS    0x00

// STATUS_REG bit positions macros
#define LPS25H_T_DA_POS             0x00
#define LPS25H_P_DA_POS             0x01
#define LPS25H_T_OR_POS             0x02
#define LPS25H_P_OR_POS             0x03

/*
 *  MASKS
 */

// CTRL_REG1 masks
#define LPS25H_PD_MASK              (0x01 << LPS25H_PD_POS)

// CTRL_REG2 masks
#define LPS25H_ONE_SHOT_MASK        (0x01 << LPS25H_ONE_SHOT_POS)

// STATUS_REG masks
#define LPS25H_T_DA_MASK            (0x01 << LPS25H_T_DA_POS)
#define LPS25H_P_DA_MASK            (0x01 << LPS25H_P_DA_POS)


// Init structure macros

// @LPS25H_INTERRUPT
#define LPS25H_INT_DISABLE          0x00
#define LPS25H_INT_ENABLE           0x01

// @LPS25H_OUTPUT_DATA_RATE
#define LPS25H_ODR_ONE_SHOT         0x00
#define LPS25H_ODR_1HZ              0x01
#define LPS25H_ODR_7HZ              0x02
#define LPS25H_ODR_12_5HZ           0x03
#define LPS25H_ODR_25HZ             0x04

// @LPS25H_FIFO
#define LPS25H_FIFO_DISABLE         0x00
#define LPS25H_FIFO_ENABLE          0x01

// @LPS25H_INT_SOURCE
#define LPS25H_INT_SOURCE_DATA_SIGNAL       0x00  
#define LPS25H_INT_SOURCE_PRESS_HIGH        0x01
#define LPS25H_INT_SOURCE_PRESS_LOW         0x02
#define LPS25H_INT_SOURCE_PRESS_LOW_HIGH    0x03

// @LPS25H_INT_CONFIG
#define LPS25H_INT_CONFIG_DRDY              0x00
#define LPS25H_INT_CONFIG_OVERRUN           0x01
#define LPS25H_INT_CONFIG_WTM               0x02
#define LPS25H_INT_CONFIG_EMPTY             0x03


// LPS25H status indicators
typedef enum {
  LPS25H_OK       = 0x00U,
  LPS25H_ERROR    = 0x01U,
}LPS25H_Status;

// Init structue
typedef struct{
    uint8_t interrupt;     // Interrupt circuit enable, @LPS25H_INTERRUPT
    uint8_t odr;           // Output data rate, @LPS25H_OUTPUT_DATA_RATE
    uint8_t fifo;          // Fifo enable, @LPS25H_FIFO
    uint8_t int_sources;   // Interrupt sources (INT1 pad), @LPS25H_INT_SOURCE
    uint8_t int_config;    // Interrupt configuration, @LPS25H_INT_CONFIG
}LPS25H_Init_t;





// Function prototypes
LPS25H_Status lps25h_Init(LPS25H_Init_t LPS25H_Init);
float lps25h_ReadTempOneShot(void);
float lps25h_ReadTemp(void);
float lps25h_ReadPressOneShot(void);
float lps25h_ReadPress(void);
void test(void);






#endif /* _LPS25H_H_ */
