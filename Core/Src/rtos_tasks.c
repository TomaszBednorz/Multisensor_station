#include "main.h"

extern ADC_Handle_t hadc1;
extern ADC_Handle_t hadc2;
extern RTC_Handle_t hrtc;

const char *msg_inv = "\r\nInvalid option !!!\r\n";

/*
 * This task is responsible for unpack and handle received bytes over serial port
 */
void taskCommandHandler(void* parameters)
{
    BaseType_t status;
    command_t command;

    while(1)
    {
        status = xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);

        if(status)  // Notification was received
        {
            unpackCommand(&command);
            handleCommand(&command);
        }
    }
}

/*
 * This task is responsible for printing messages over serial port
 */
void taskPrint(void* parameters)
{
    char* msgBuffer;

    uint32_t commandAddr;

    while(1)
    {
        xQueueReceive(queuePrint, &commandAddr, portMAX_DELAY);
        msgBuffer = (char*)commandAddr;

        printMsg(msgBuffer);

    }
}

/*
 * This task is responsible for notificate approprite task based on received data
 */
void taskMenu(void* parameters)
{
    command_t* command;
    uint32_t commandAddr;
    uint8_t nextState;
    const char* msgMenu = "\r\n------------------------------\r\n"
                    "|            MENU            |\r\n"
                    "------------------------------\r\n"
                    "0: LED effect\r\n"
                    "1: Temperature\r\n"
                    "2: Pressure\r\n"
                    "3: Lighting\r\n"
                    "4: Time & Date\r\n"
                    "5: Set alarm\r\n"
                    "6: Help\r\n" 
                    "Enter number(0-6):";

    while(1)
    {

        xQueueSend(queuePrint, &msgMenu, 0);  // Print main menu

        xTaskNotifyWait(0, 0, &commandAddr, portMAX_DELAY);
        command = (command_t*)commandAddr;

        if(command->length == 1)
        {
            nextState = command->payload[0] - 48;
            switch (nextState)
            {
            case 0:
                currentState = sLedEffect;
                xTaskNotify(taskLedsHandle, 0, eNoAction);
                break;
            case 1:
                currentState = sTemperature;
                xTaskNotify(taskTemperatureHandle, 0, eNoAction);
                break;
            case 2:
                currentState = sPressure;
                xTaskNotify(taskPressureHandle, 0, eNoAction);
                break;
            case 3:
                currentState = sLighting;
                xTaskNotify(taskLightingHandle, 0, eNoAction);
                break;
            case 4:
                currentState = sTimeDateConfig;
                xTaskNotify(taskTimeDateHandle, 0, eNoAction);
                break;
            case 5:
                // xTaskNotify(..., 0, eNoAction);
                break;
            case 6:
                // xTaskNotify(..., 0, eNoAction);
                break;
            default:  // Invalid data received
                xQueueSend(queuePrint, &msg_inv, 0);
                continue;
            }
        }
        else  // Invalid data received
        {
            xQueueSend(queuePrint, &msg_inv, 0);
            continue;
        }

        xTaskNotifyWait(0,0,NULL,portMAX_DELAY);  // Wait for notification from other task

    }
}

/*
 * This task is responsible for generate appropriate LEDs effect based on user input
 */
void taskLeds(void* parameters)
{
    command_t* command;
    uint32_t commandAddr;
    uint8_t selectedLedEffect;
    const char* msgMenu = "\r\n------------------------------\r\n"
                          "|     SELECT LEDs EFECT      |\r\n"
                          "------------------------------\r\n"
                          "0: None\r\n"
                          "1: *..|.*.|..*|*..\r\n"
                          "2: ***|...|***|...\r\n"
                          "3: **.|.**|*.*|**.\r\n"
                          "Enter number(0-3):";

    while(1)
    {
        xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);  // Wait for the user to select an option in main menu
        xQueueSend(queuePrint, &msgMenu, 0);
        xTaskNotifyWait(0, 0, &commandAddr, portMAX_DELAY);  // Wait for user choose of LEDs effect
        command = (command_t*)commandAddr;

        if(command->length == 1)
        {
            selectedLedEffect = command->payload[0] - 48;
            switch (selectedLedEffect)
            {
            case 0:
                stopLedEffect();
                break;
            case 1:
                activateLedEffect(1);
                break;
            case 2:
                activateLedEffect(2);
                break;
            case 3:
                activateLedEffect(3);
                break;
            default:  // Invalid data received
                xQueueSend(queuePrint, &msg_inv, 0);
            }
        }
        else  // Invalid data received
        {
            xQueueSend(queuePrint, &msg_inv, 0);
        }
        currentState = sMainMenu;
        xTaskNotify(taskMenuHandle, 0, eNoAction);  // Return to main menu
    }
}

/*
 * This task is responsible for inform about actual temperature
 */
void taskTemperature(void* parameters)
{
    static char msgTemp[40];
    float temp;

    static char *msg = msgTemp;

    while(1)
    {
        xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);  // Wait for the user to select an option in main menu

        temp = lps25h_ReadTemp();
        sprintf((char*)msgTemp, "\r\nActual temperature: %.2f%cC\r\n", temp, 248);

        xQueueSend(queuePrint, &msg, 0);

        currentState = sMainMenu;
        xTaskNotify(taskMenuHandle, 0, eNoAction);  // Return to main menu
    }
}

/*
 * This task is responsible for inform about actual pressure
 */
void taskPressure(void* parameters)
{
    static char msgPres[40];
    float pres;

    static char *msg = msgPres;

    while(1)
    {
        xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);  // Wait for the user to select an option in main menu

        pres = lps25h_ReadPress();
        sprintf((char*)msgPres, "\r\nActual pressure: %.2fhPa\r\n", pres);

        xQueueSend(queuePrint, &msg, 0);

        currentState = sMainMenu;
        xTaskNotify(taskMenuHandle, 0, eNoAction);  // Return to main menu
    }
}

/*
 * This task is responsible for inform about actual lighting (fotoresistor)
 */
void taskLighting(void* parameters)
{
    static char msgLight[40];
    float voltageFotoresistor;
    uint8_t light;

    static char *msg = msgLight;

    while(1)
    {
        xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);  // Wait for the user to select an option in main menu

        // Read vlotage value on fotoresistor
        ADC_Start(&hadc1);
        ADC_PollForConversion(&hadc1);
        voltageFotoresistor = ADC_ConvToVoltage(ADC_Read(&hadc1), ADC_RES_RANGE_12_BIT);

        // Convert voltage to value in % : brightlu = more %
        light = (1 - (voltageFotoresistor / ADC_MAX_VOLTAGE)) * 100;

        sprintf((char*)msgLight, "\r\nActual lighting: %d%c\r\n", light, 37);

        xQueueSend(queuePrint, &msg, 0);

        currentState = sMainMenu;
        xTaskNotify(taskMenuHandle, 0, eNoAction);  // Return to main menu
    }
}

/*
 * This task is responsible for inform about actual time & date. The user can configure these values.
 */
void taskTimeDate(void* parameters)
{
    command_t* command;
    uint32_t commandAddr;
    uint8_t nextState;

    while(1)
    {

        xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);  // Wait for the user to select an option in main menu

        showTimeDateMenu();

        xTaskNotifyWait(0, 0, &commandAddr, portMAX_DELAY);
        command = (command_t*)commandAddr;

        if(command->length == 1)
        {
            nextState = command->payload[0] - 48;
            switch (nextState)
            {
            case 0:
                currentState = sTimeConfig;
                xTaskNotify(taskTimeConfigHandle, 0, eNoAction);
                xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);  // Wait for notification from taskTimeConfig
                break;
            case 1:
                currentState = sDateConfig;
                xTaskNotify(taskDateConfigHandle, 0, eNoAction);
                xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);  // Wait for notification from taskDateConfig
                break;
            case 2:
                break;
            default:  // Invalid data received
                xQueueSend(queuePrint, &msg_inv, 0);
            }
        }
        else  // Invalid data received
        {
            xQueueSend(queuePrint, &msg_inv, 0);
        }

        

        currentState = sMainMenu;
        xTaskNotify(taskMenuHandle, 0, eNoAction);  // Return to main menu
    }
}

/*
 * This task is responsible for time configuration
 */
void taskTimeConfig(void* parameters)
{
    command_t* command;
    uint32_t commandAddr;
    const char* msgMenu =  "\r\n------------------------------\r\n"
                           "|     TIME CONFIGURATION     |\r\n"
                           "------------------------------";
    const char* msgConfigHour = "\r\nEnter hour(0-23):";
    const char* msgConfigMin = "\r\nEnter minutes(0-59):";
    const char* msgConfigSec = "\r\nEnter seconds(0-59):";

    const char* msgConfigInvalid = "\r\nInvalid entered data!";
    const char* msgConfigCorrect = "\r\nConfiguration correct !";

    enum state{Hour=0, Min, Sec, Exit};  // Consecutive states in time configuration
    enum state timeState;

    while(1)
    {
        xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);  // Wait for the user to select an option in Time&Date menu
        xQueueSend(queuePrint, &msgMenu, 0);
        xQueueSend(queuePrint, &msgConfigHour, 0);  // Inform the user that should input data about current hour

        timeState = Hour;

        while(timeState != Exit)
        {
            // Get data from the user, until it sends a second
            xTaskNotifyWait(0, 0, &commandAddr, portMAX_DELAY);  
            command = (command_t*)commandAddr;


            switch (timeState)
            {
            case Hour:
                hrtc.Time.hours = getNumber(command);
                xQueueSend(queuePrint, &msgConfigMin, 0);  // Inform the user that should input data about current minute
                timeState = Min;
                break;
            case Min:
                hrtc.Time.minutes = getNumber(command);
                xQueueSend(queuePrint, &msgConfigSec, 0);  // Inform the user that should input data about current second
                timeState = Sec;
                break;
            case Sec:   
                hrtc.Time.seconds = getNumber(command);
                if(hrtc.Time.hours > 23 || hrtc.Time.minutes > 59 || hrtc.Time.seconds > 59)  // Check the correctness of the data
                {
                    xQueueSend(queuePrint, &msgConfigInvalid, 0);
                }
                else
                {
                    RTC_SetTime(&hrtc);
                    xQueueSend(queuePrint, &msgConfigCorrect, 0);
                }
                timeState = Exit;
                break;
            case Exit:
            default:
                break;
            }

        }

        currentState = sTimeDateConfig;
        xTaskNotify(taskTimeDateHandle, 0, eNoAction);  // Return to Time&Date menu and next to main menu
    }
}

/*
 * This task is responsible for date configuration
 */
void taskDateConfig(void* parameters)
{
    command_t* command;
    uint32_t commandAddr;

    const char* msgMenu =  "------------------------------\r\n"
                           "|     DATE CONFIGURATION     |\r\n"
                           "------------------------------\r\n";
    const char* msgConfigDate = "\r\nEnter date(1-31):";
    const char* msgConfigMonth = "\r\nEnter month(1-12):";
    const char* msgConfigWeekDay = "\r\nEnter week day(1-7):";
    const char* msgConfigYear = "\r\nEnter year(0-99):";


    while(1)
    {
        xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);  // Wait for the user to select an option in Time&Date menu




        xTaskNotify(taskMenuHandle, 0, eNoAction);  // Return to Time&Date menu and next to main menu
    }
}

// Idle Hook Function 
void vApplicationIdleHook(void)
{
    while(1)
    {
        __WFI();
    }
}

