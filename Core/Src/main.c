#include "main.h"


extern UART_Handle_t huart3;

extern const char *msg_inv;

BaseType_t status;

volatile uint8_t rxData;  // UART rx data

int main(void)
{
	SystemClockConfig();  // PLL ON, AHB = 120 MHz, APB1 = 30 MHz, APB2 = 60 MHz, timers x 2
	SystemCoreClockUpdate();  // Update SystemCoreClock variable
	FPU_Enable();  // FPU enabled, Hardware support for 32-bits floats

	// Peripheral drivers
	GPIO_Config();
	SPI1_Config();
	UART3_Config();
	ADC1_Config();
	ADC2_Config();
	RTC_Config();

	// Sensor driver
	LPS25H_Config();

	currentState = sMainMenu;  // Begining state

	// Create queues
	queueRxData = xQueueCreate(10, sizeof(uint8_t));
	configASSERT(queueRxData != NULL);
	queuePrint = xQueueCreate(10, sizeof(char*));
	configASSERT(queuePrint != NULL);
	queueEvents = xQueueCreate(2, sizeof(uint8_t));
	configASSERT(queuePrint != NULL);

	// Create tasks
    status = xTaskCreate(taskMenu, "Menu_task", 200, NULL, 1, &taskMenuHandle);
    configASSERT(status = pdPASS);
	status = xTaskCreate(taskCommandHandler, "Command_handler_task", 200, NULL, 1, &taskCommandHandle);
    configASSERT(status = pdPASS);
    status = xTaskCreate(taskPrint, "Print_task", 200, NULL, 1, &taskPrintHandle);
    configASSERT(status = pdPASS);
    status = xTaskCreate(taskLeds, "Leds_task", 200, NULL, 1, &taskLedsHandle);
    configASSERT(status = pdPASS);
    status = xTaskCreate(taskTemperature, "Temperature_task", 200, NULL, 1, &taskTemperatureHandle);
    configASSERT(status = pdPASS);
    status = xTaskCreate(taskPressure, "Pressure_task", 200, NULL, 1, &taskPressureHandle);
    configASSERT(status = pdPASS);
    status = xTaskCreate(taskLighting, "Lighting_task", 200, NULL, 1, &taskLightingHandle);
    configASSERT(status = pdPASS);
    status = xTaskCreate(taskTimeDate, "Time&Date_task", 200, NULL, 1, &taskTimeDateHandle);
    configASSERT(status = pdPASS);
	status = xTaskCreate(taskTimeConfig, "Time_config_task", 200, NULL, 1, &taskTimeConfigHandle);
    configASSERT(status = pdPASS);
	status = xTaskCreate(taskDateConfig, "Date_config_task", 200, NULL, 1, &taskDateConfigHandle);
    configASSERT(status = pdPASS);
	status = xTaskCreate(taskAlarmConfig, "Alarm_config_task", 200, NULL, 1, &taskAlarmConfigHandle);
    configASSERT(status = pdPASS);
	status = xTaskCreate(taskEventTriggered, "Alarm_trigger_task", 200, NULL, 1, &taskEventTriggeredHandle);
    configASSERT(status = pdPASS);
	status = xTaskCreate(taskHelp, "Help_task", 200, NULL, 1, &taskHelpHandle);
    configASSERT(status = pdPASS);
	status = xTaskCreate(taskPotentiometer, "Potentiometer_task", 200, NULL, 1, &taskPotentiometerHandle);
    configASSERT(status = pdPASS);

	// Create software timers
	for(uint8_t i = 0; i < 3; i++)
	{
		softwareTimerLeds[i] = xTimerCreate("Led timer", pdMS_TO_TICKS(500), pdTRUE, (void*)(i+1), ledsCallback);
	}

	UART_ReceiveIT(&huart3, (uint8_t*)&rxData, 1);  // Receive data byte by byte

	vTaskStartScheduler();

	while(1)
	{

	}
}

/*
 * Print message over serial port
 */
void printMsg(char* msg)
{
	UART_Transmit(&huart3, (uint8_t*)msg, strlen(msg));
}

/*
 * Unpack message from queue and transform to command_t structure
 */
void unpackCommand(command_t* command)
{
	BaseType_t status;
	uint8_t item;

	uint8_t i = 0;  // Number of received items from queue

	do  // Receive messages until queue is not empty
	{
		status = xQueueReceive(queueRxData, &item, 0);
		if(status == pdTRUE)
		{
			command->payload[i++] = item;
		}
	}while(item != '\n');

	command->payload[i-1] = '\0';  // Exchange \n to \0
	command->length = i-1;  // Save length of command without \0
}

/*
 * Handle command_t structure and notify appropriate task
 */
void handleCommand(command_t* command)
{
	switch (currentState)
	{
	case sMainMenu:
		xTaskNotify(taskMenuHandle, (uint32_t)command, eSetValueWithOverwrite);
		break;
	case sLedEffect:
		xTaskNotify(taskLedsHandle, (uint32_t)command, eSetValueWithOverwrite);
		break;
	case sTemperature:
		// No activities in this state
		break;
	case sPressure:
		// No activities in this state
		break;
	case sLighting:
		// No activities in this state
		break;
	case sTimeDateConfig:
		xTaskNotify(taskTimeDateHandle, (uint32_t)command, eSetValueWithOverwrite);
		break;
	case sTimeConfig:
		xTaskNotify(taskTimeConfigHandle, (uint32_t)command, eSetValueWithOverwrite);
		break;
    case sDateConfig:
		xTaskNotify(taskDateConfigHandle, (uint32_t)command, eSetValueWithOverwrite);
		break;
	case sAlarmConfig:
		xTaskNotify(taskAlarmConfigHandle, (uint32_t)command, eSetValueWithOverwrite);
		break;
	case sHelp:
		xTaskNotify(taskHelpHandle, (uint32_t)command, eSetValueWithOverwrite);;
		break;
	}
}


// Callbacks

// UART Reception callback
void UART_RxCompleteCallback(UART_Handle_t *huart)
{
	uint8_t dummyData;

	if(!xQueueIsQueueFullFromISR(queueRxData))  // Queue is not full
	{
		xQueueSendFromISR(queueRxData, (void*)&rxData, NULL);  // Enqueue data byte
	}
	else  // Queue is full
	{
		if(rxData == '\n')  // Swap last item in queue to '\n'
		{
			xQueueReceiveFromISR(queueRxData, (void*)&dummyData, NULL);
			xQueueSendFromISR(queueRxData, (void*)&rxData, NULL);
		}
	}

	if(rxData == '\n')  // Notify taskCommandHandler about received command
	{
		xTaskNotifyFromISR(taskCommandHandle, 0, eNoAction, NULL);
	}

    UART_ReceiveIT(&huart3, (uint8_t*)&rxData, 1);  // Receive data byte by byte
}

// Software timers callback
void ledsCallback(TimerHandle_t xTimer)
{
	uint32_t timerID;

	timerID = (uint32_t)pvTimerGetTimerID(xTimer);

	switch (timerID)
	{
	case 1:
		ledEffect1();
		break;
	case 2:
		ledEffect2();
		break;
	case 3:
		ledEffect3();
		break;
	}
}

// Alarm A callback
void RTC_AlarmACallback(RTC_Handle_t *hrtc)
{
	uint8_t item;
	item = EVENT_ALARM_A;

	// Send message (element) by queue to taskEventTriggered, that ALARM A was triggered
	xQueueSendFromISR(queueEvents, &item, NULL);
}

// Alarm B callback
void RTC_AlarmBCallback(RTC_Handle_t *hrtc)
{
	uint8_t item;
	item = EVENT_ALARM_B;
	
	// Send message (element) by queue to taskEventTriggered, that ALARM B was triggered
	xQueueSendFromISR(queueEvents, &item, NULL);
}

// GPIO callback
void GPIO_EXTI_Callback(uint8_t GPIO_Pin)
{
	uint8_t item;
	// Send notification to taskEventTriggered, that ALARM B was triggered
	
	if(GPIO_Pin == GPIO_PIN_10)  // Interrupt from the button
	{
		// Send message (element) by queue to taskEventTriggered, that button was clicked
		item = EVENT_BUTTON_IT;
		xQueueSendFromISR(queueEvents, &item, NULL);
	}
}