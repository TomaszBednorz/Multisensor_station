#include "lps25h.h"

// Static function prototypes
static uint8_t lps25h_ReadReg(uint8_t Reg);
static void lps25h_ReadRegs(uint8_t Reg, uint8_t Amount, uint8_t *pRegsValues);
static void lps25h_WriteReg(uint8_t Reg, uint8_t Value);
static void lps25h_WriteRegs(uint8_t Reg, uint8_t Amount, uint8_t *pValues);


/**
  * @brief LPS25H configuration function

  * @param  LPS25H_Init is init structure from lps25h.h

  * @retval returns LPS25H_OK after successful initialization, LPS25H_ERROR otherwise
  */
LPS25H_Status lps25h_Init(LPS25H_Init_t LPS25H_Init)
{
  uint8_t tx_buf[4];  // 4 - Number of control registers

  if(lps25h_ReadReg(LPS25H_WHO_AM_I) != 0xBD)  // Double time check WHO_AM_I register
  {
    if(lps25h_ReadReg(LPS25H_WHO_AM_I) != 0xBD)
    {
      return LPS25H_ERROR;
    }
  }

  tx_buf[0] = (LPS25H_Init.interrupt << LPS25H_DIFF_EN_POS) | (LPS25H_Init.odr << LPS25H_ODR_POS) | LPS25H_PD_MASK;
  tx_buf[1] = LPS25H_Init.fifo << LPS25H_FIFO_EN_POS;
  tx_buf[2] = LPS25H_Init.int_sources << LPS25H_INT1_S_POS;
  tx_buf[3] = LPS25H_Init.int_config << LPS25H_P1_INT_CONFIG_POS;

  lps25h_WriteRegs(LPS25H_CTRL_REG1, 4, tx_buf);

  return LPS25H_OK;
}

/**
  * @brief Read temperature from LPS25H sensor in One shot mode
  * 
  * @retval returns temoerature 
  */
float lps25h_ReadTempOneShot(void)
{
  uint8_t rx_buf[2];  // Temerature is 16 bit value which is contatenated from two 8-bit values (2's complement number)
  int16_t temp = 0x00;

  lps25h_WriteReg(LPS25H_CTRL_REG2, LPS25H_ONE_SHOT_MASK);  // Trigger one shot converion

  while(!(lps25h_ReadReg(LPS25H_STATUS_REG) & LPS25H_T_DA_MASK));  // Wait for temperature data is available

  lps25h_ReadRegs(LPS25H_TEMP_OUT_L, 2, rx_buf);

  temp = (rx_buf[1] << 8) | rx_buf[0];  // Concatenate received 8-bit values
  return 42.5f + (float)temp / 480.0f;
}

/**
  * @brief Read temperature from LPS25H sensor in continous mode (all modes other than one shot)
  * 
  * @retval returns temoerature 
  */
float lps25h_ReadTemp(void)
{
  uint8_t rx_buf[2];  // Temerature is 16 bit value which is contatenated from two 8-bit values (2's complement number)
  int16_t temp = 0x00;

  while(!(lps25h_ReadReg(LPS25H_STATUS_REG) & LPS25H_T_DA_MASK));  // Wait for temperature data is available

  lps25h_ReadRegs(LPS25H_TEMP_OUT_L, 2, rx_buf);

  temp = (rx_buf[1] << 8) | rx_buf[0];  // Concatenate received 8-bit values
  return 42.5f + (float)temp / 480.0f;
}

/**
  * @brief Read pressure from LPS25H sensor in One shot mode
  * 
  * @retval returns pressure 
  */
float lps25h_ReadPressOneShot(void)
{
  uint8_t rx_buf[3];  // Pressure is 24 bit value which is contatenated from three 8-bit values (2's complement number)
  uint32_t press = 0x00;

  lps25h_WriteReg(LPS25H_CTRL_REG2, LPS25H_ONE_SHOT_MASK);  // Trigger one shot converion

  while(!(lps25h_ReadReg(LPS25H_STATUS_REG) & LPS25H_P_DA_MASK));  // Wait for pressure data is available

  lps25h_ReadRegs(LPS25H_PRESS_POUT_XL, 3, rx_buf);

  press = (rx_buf[2] << 16) | (rx_buf[1] << 8) | rx_buf[0];  // Concatenate received 8-bit values, pressure can't be negative value
  return (float)press / 4096.0f;
}

/**
  * @brief Read pressure from LPS25H sensor in continous mode (all modes other than one shot)
  * 
  * @retval returns pressure 
  */
float lps25h_ReadPress(void)
{
  uint8_t rx_buf[3];  // Pressure is 24 bit value which is contatenated from three 8-bit values (2's complement number)
  uint32_t press = 0x00;

  while(!(lps25h_ReadReg(LPS25H_STATUS_REG) & LPS25H_P_DA_MASK));  // Wait for pressure data is available

  lps25h_ReadRegs(LPS25H_PRESS_POUT_XL, 3, rx_buf);

  press = (rx_buf[2] << 16) | (rx_buf[1] << 8) | rx_buf[0];  // Concatenate received 8-bit values, pressure can't be negative value
  return (float)press / 4096.0f;
}


// Static functions

/**
  * @brief Function which reads value from single register

  * @param  reg register address

  * @retval Value from reg register
  */
static uint8_t lps25h_ReadReg(uint8_t Reg)
{
  uint8_t reg_value = 0x00;
  uint8_t tx_buf = 0x00;

  tx_buf = (Reg & LPS25H_ADDRESS_MASK) | LPS25H_READ_MASK;

  GPIO_pin_write(LPS25H_SPI_NSS_PORT, LPS25H_SPI_NSS_PIN, GPIO_PIN_RESET);
  SPI_Transmit(&LPS25H_SPI_HANDLE, &tx_buf, 1);
  SPI_Receive(&LPS25H_SPI_HANDLE, &reg_value, 1);
  GPIO_pin_write(LPS25H_SPI_NSS_PORT, LPS25H_SPI_NSS_PIN, GPIO_PIN_SET);

  return reg_value;
}

/**
  * @brief Function which reads values from multiple continuous registers

  * @param  Reg beginning register address
  * @param  Amount number of consecutive registers to read
  * @param  pRegsValues handle to store data from registers

  * @retval None
  */
static void lps25h_ReadRegs(uint8_t Reg, uint8_t Amount, uint8_t *pRegsValues)
{
  uint8_t tx_buf = 0x00;

  tx_buf = (Reg & LPS25H_ADDRESS_MASK) | LPS25H_INC_ADDR_MASK | LPS25H_READ_MASK;

  GPIO_pin_write(LPS25H_SPI_NSS_PORT, LPS25H_SPI_NSS_PIN, GPIO_PIN_RESET);
  SPI_Transmit(&LPS25H_SPI_HANDLE, &tx_buf, 1);
  SPI_Receive(&LPS25H_SPI_HANDLE, pRegsValues, Amount);
  GPIO_pin_write(LPS25H_SPI_NSS_PORT, LPS25H_SPI_NSS_PIN, GPIO_PIN_SET);
}

/**
  * @brief Function which writes value to single register

  * @param  reg register address
  * @param  Value value to store in register
  * 
  * @retval None
  */
static void lps25h_WriteReg(uint8_t Reg, uint8_t Value)
{
  uint8_t tx_buf[2];

  tx_buf[0] = (Reg & LPS25H_ADDRESS_MASK) | LPS25H_WRITE_MASK;  // 1st transmit register address
  tx_buf[1] = Value;  // 2nd transmit value to store in register

  GPIO_pin_write(LPS25H_SPI_NSS_PORT, LPS25H_SPI_NSS_PIN, GPIO_PIN_RESET);
  SPI_Transmit(&LPS25H_SPI_HANDLE, tx_buf, 2);
  GPIO_pin_write(LPS25H_SPI_NSS_PORT, LPS25H_SPI_NSS_PIN, GPIO_PIN_SET);
}

/**
  * @brief Function which writes values to multiple continuous registers

  * @param  Reg beginning register address
  * @param  Amount number of consecutive registers to write
  * @param  pValues handle to write values to registers

  * @retval None
  */
static void lps25h_WriteRegs(uint8_t Reg, uint8_t Amount, uint8_t *pValues)
{
  uint8_t tx_buf[1 + Amount];  // Beginning address + values to transmit

  tx_buf[0] = (Reg & LPS25H_ADDRESS_MASK) | LPS25H_INC_ADDR_MASK |LPS25H_WRITE_MASK;  // 1st transmit register address

  for(uint8_t i = 0; i < Amount; i++)
  {
    tx_buf[i+1] = pValues[i];  // Next transmit values to store in registers
  }

  GPIO_pin_write(LPS25H_SPI_NSS_PORT, LPS25H_SPI_NSS_PIN, GPIO_PIN_RESET);
  SPI_Transmit(&LPS25H_SPI_HANDLE, tx_buf, 1 + Amount);
  GPIO_pin_write(LPS25H_SPI_NSS_PORT, LPS25H_SPI_NSS_PIN, GPIO_PIN_SET);
}
