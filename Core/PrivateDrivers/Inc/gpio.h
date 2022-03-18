#ifndef INC_GPIO_H_
#define INC_GPIO_H_

#include "stm32f4xx.h"
#include <stdint.h>

// @GPIO_PIN
#define GPIO_PIN_0				0
#define GPIO_PIN_1				1
#define GPIO_PIN_2				2
#define GPIO_PIN_3				3
#define GPIO_PIN_4				4
#define GPIO_PIN_5				5
#define GPIO_PIN_6				6
#define GPIO_PIN_7				7
#define GPIO_PIN_8				8
#define GPIO_PIN_9				9
#define GPIO_PIN_10				10
#define GPIO_PIN_11				11
#define GPIO_PIN_12				12
#define GPIO_PIN_13				13
#define GPIO_PIN_14				14
#define GPIO_PIN_15				15

// @GPIO_MODE
#define GPIO_MODE_INPUT			0x00
#define GPIO_MODE_OUTPUT		0x01
#define GPIO_MODE_ALT_FUN		0x02
#define GPIO_MODE_ANALOG		0x03

// @GPIO_OUTPUT_TYPE
#define GPIO_OUTPUT_TYPE_PP		0x00
#define GPIO_OUTPUT_TYPE_OD		0x01

// @GPIO_SPEED
#define GPIO_SPEED_LOW			0x00
#define GPIO_SPEED_MEDIUM		0x01
#define GPIO_SPEED_HIGH			0x03
#define GPIO_SPEED_VERY_HIGH	0x03

// @GPIO_PUPD
#define GPIO_PUPD_NO			0x00
#define GPIO_PUPD_PULL_UP		0x01
#define GPIO_PUPD_PULL_DOWN		0x02

// @GPIO_ALT_FUN
#define GPIO_ALT_FUN_0			0x00
#define GPIO_ALT_FUN_1			0x01
#define GPIO_ALT_FUN_2			0x02
#define GPIO_ALT_FUN_3			0x03
#define GPIO_ALT_FUN_4			0x04
#define GPIO_ALT_FUN_5			0x05
#define GPIO_ALT_FUN_6			0x06
#define GPIO_ALT_FUN_7			0x07
#define GPIO_ALT_FUN_8			0x08
#define GPIO_ALT_FUN_9			0x09		
#define GPIO_ALT_FUN_10			0x0A
#define GPIO_ALT_FUN_11			0x0B
#define GPIO_ALT_FUN_12			0x0C
#define GPIO_ALT_FUN_13			0x0D
#define GPIO_ALT_FUN_14			0x0E
#define GPIO_ALT_FUN_15			0x0F

// @GPIO_PIN_STATE
#define GPIO_PIN_RESET			0x00
#define GPIO_PIN_SET			0x01


typedef struct{
	uint8_t	pin;				// @GPIO_PIN
	uint8_t mode;				// @GPIO_MODE
	uint8_t output_type;		// @GPIO_OUTPUT_TYPE
	uint8_t speed;				// @GPIO_SPEED
	uint8_t pupd;				// @GPIO_PUPD
	uint8_t alt_fun;			// @GPIO_ALT_FUN
}GPIO_Init_t;


// Function prototypes
void GPIO_pin_config(GPIO_TypeDef *GPIOx, GPIO_Init_t GPIO_Init);
void GPIO_pin_write(GPIO_TypeDef *GPIOx, uint8_t GPIO_Pin, uint8_t PinState);
void GPIO_pin_toggle(GPIO_TypeDef *GPIOx, uint8_t GPIO_Pin);



#endif /* INC_GPIO_H_ */
