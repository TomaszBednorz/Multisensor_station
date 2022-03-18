// #include "main.h"

// static void task1_handler(void* parameters);
// static void task2_handler(void* parameters);
// static void task3_handler(void* parameters);

// void GPIO_Config(void);
// void SystemClockConfig(void);

// TaskHandle_t task1_handle, task2_handle, task3_handle;
// BaseType_t status;

// int main(void)
// {

// 	GPIO_Config();

//     status = xTaskCreate(task1_handler, "Task-1", 200, NULL, 2, &task1_handle);
//     configASSERT(status = pdPASS);
//     status = xTaskCreate(task2_handler, "Task-2", 200, NULL, 2, &task2_handle);
//     configASSERT(status = pdPASS);
//     status = xTaskCreate(task3_handler, "Task-3", 200, NULL, 2, &task3_handle);
//     configASSERT(status = pdPASS);

// 	vTaskStartScheduler();

// 	// power consumption mode in IDLE task - configUSE_IDLE_HOOK = 1

	

// 	while(1)
// 	{

// 	}
// }

// static void task1_handler(void* parameters)
// {
// 	while(1)
// 	{
// 		GPIOB->ODR ^= GPIO_ODR_OD0;
// 		for(int i = 0; i < 1500000; i++);
// 	}
// }
// static void task2_handler(void* parameters)
// {
// 	while(1)
// 	{
// 		GPIOB->ODR ^= GPIO_ODR_OD7;
// 		for(int i = 0; i < 1000000; i++);
// 	}
// }

// static void task3_handler(void* parameters)
// {
// 	while(1)
// 	{
// 		GPIOB->ODR ^= GPIO_ODR_OD14;
// 		for(int i = 0; i < 500000; i++);
// 	}
// }


// void GPIO_Config(void)
// {
// 	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOAEN;  // Enable peripherals clocks

// 	GPIO_Init_t GPIO_Init = {0};
	
// 	// LD1 - PB0
// 	GPIO_Init.mode = GPIO_MODE_OUTPUT;
// 	GPIO_Init.pin = GPIO_PIN_0;
// 	GPIO_pin_config(GPIOB, GPIO_Init);

// 	// LD2 - PB7
// 	GPIO_Init.pin = GPIO_PIN_7;
// 	GPIO_pin_config(GPIOB, GPIO_Init);

// 	// LD3 - PB14
// 	GPIO_Init.pin = GPIO_PIN_14;
// 	GPIO_pin_config(GPIOB, GPIO_Init);
// }




