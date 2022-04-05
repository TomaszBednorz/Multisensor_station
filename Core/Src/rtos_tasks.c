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
        xQueueReceive(queuePrint, &commandAddr, portMAX_DELAY);  // Wait for data in queue
        msgBuffer = (char*)commandAddr;

        printMsg(msgBuffer);

    }
}

/*
 * This task notifies approprite task based on received data
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

        xTaskNotifyWait(0, 0, &commandAddr, portMAX_DELAY);  // Wait for the user input
        command = (command_t*)commandAddr;

        if(command->length == 1)
        {
            nextState = command->payload[0] - 48;
            switch (nextState)  // Change state and notify the task corresponding  to the input data
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
                currentState = sAlarmConfig;
                xTaskNotify(taskAlarmConfigHandle, 0, eNoAction);
                break;
            case 6:
                currentState = sHelp;
                xTaskNotify(taskHelpHandle, 0, eNoAction);
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
 * This task generates appropriate LEDs effect based on user input
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
 * This task informs about actual temperature
 */
void taskTemperature(void* parameters)
{
    while(1)
    {
        xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);  // Wait for the user to select an option in main menu

        showTemperature();

        currentState = sMainMenu;
        xTaskNotify(taskMenuHandle, 0, eNoAction);  // Return to main menu
    }
}

/*
 * This task informs about actual pressure
 */
void taskPressure(void* parameters)
{
    while(1)
    {
        xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);  // Wait for the user to select an option in main menu

        showPressure();

        currentState = sMainMenu;
        xTaskNotify(taskMenuHandle, 0, eNoAction);  // Return to main menu
    }
}

/*
 * This task informs about actual lighting (photoresistor)
 */
void taskLighting(void* parameters)
{
    while(1)
    {
        xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);  // Wait for the user to select an option in main menu

        showLighting();

        currentState = sMainMenu;
        xTaskNotify(taskMenuHandle, 0, eNoAction);  // Return to main menu
    }
}

/*
 * This task informs about actual time & date. The user can configure these values.
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

        timeState = Hour;  // Start from the beginning state

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

                // Check the correctness of the time
                if(hrtc.Time.hours > 23 || hrtc.Time.minutes > 59 || hrtc.Time.seconds > 59)  
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

    const char* msgMenu =  "\r\n------------------------------\r\n"
                           "|     DATE CONFIGURATION     |\r\n"
                           "------------------------------\r\n";
    const char* msgConfigDate = "\r\nEnter date(1-31):";
    const char* msgConfigMonth = "\r\nEnter month(1-12):";
    const char* msgConfigWeekDay = "\r\nEnter week day(1-7):";
    const char* msgConfigYear = "\r\nEnter year(0-99):";

    const char* msgConfigInvalid = "\r\nInvalid entered data!";
    const char* msgConfigCorrect = "\r\nConfiguration correct !";

    enum state{Date=0, Month, WeekDay, Year, Exit};  // Consecutive states in date configuration
    enum state dataState;

    while(1)
    {
        xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);  // Wait for the user to select an option in Time&Date menu
        xQueueSend(queuePrint, &msgMenu, 0);
        xQueueSend(queuePrint, &msgConfigDate, 0);  // Inform the user that should input data about current date

        dataState = Date;  // Start from the beginning state

        while(dataState != Exit)
        {
            // Get data from the user, until it sends a year
            xTaskNotifyWait(0, 0, &commandAddr, portMAX_DELAY);  
            command = (command_t*)commandAddr;


            switch (dataState)
            {
            case Date:
                hrtc.Date.date = getNumber(command);
                xQueueSend(queuePrint, &msgConfigMonth, 0);  // Inform the user that should input data about current month
                dataState = Month;
                break;
            case Month:
                hrtc.Date.month = getNumber(command);
                xQueueSend(queuePrint, &msgConfigWeekDay, 0);  // Inform the user that should input data about current weekday
                dataState = WeekDay;
                break;
            case WeekDay:   
                hrtc.Date.week_day = getNumber(command);
                xQueueSend(queuePrint, &msgConfigYear, 0);  // Inform the user that should input data about current year
                dataState = Year;
                break;
            case Year:
                hrtc.Date.year = getNumber(command);

                // Check the correctness of the data
                if(hrtc.Date.date > 31 || hrtc.Date.date < 1 || hrtc.Date.month > 12 || hrtc.Date.month < 1 || 
                   hrtc.Date.week_day > 7 || hrtc.Date.week_day < 1 || hrtc.Date.year > 99)  
                {
                    xQueueSend(queuePrint, &msgConfigInvalid, 0);
                }
                else
                {
                    RTC_SetDate(&hrtc);
                    xQueueSend(queuePrint, &msgConfigCorrect, 0);
                }
                dataState = Exit;
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
 * This task is responsible for alarm configuration
 */
void taskAlarmConfig(void* parameters)
{
    command_t* command;
    uint32_t commandAddr;
    const char* msgMenu =  "\r\n------------------------------\r\n"
                           "|    ALARM  CONFIGURATION    |\r\n"
                           "------------------------------";
    const char* msgConfigAlarm = "\r\nEnter alarm number(1-2):";  
    const char* msgConfigWeekday = "\r\nEnter weekday(1-7):";                    
    const char* msgConfigHour = "\r\nEnter hour(0-23):";
    const char* msgConfigMin = "\r\nEnter minutes(0-59):";

    const char* msgConfigInvalid = "\r\nInvalid entered data!";
    const char* msgConfigCorrect = "\r\nConfiguration correct !";

    enum state{Alarm=0, Weekday, Hour, Min, Exit};  // Consecutive states in alarm configuration
    enum state alarmState;

    RTC_Alarm_t alarmConfig ={0};  // This variable store alarm configuration
    uint8_t alarm = 0;  // This variable store number of alarm (A/1 or B/2)

    while(1)
    {
        xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);  // Wait for the user to select an option in menu
        xQueueSend(queuePrint, &msgMenu, 0);
        xQueueSend(queuePrint, &msgConfigAlarm, 0);  // Inform the user that should choose alarm number

        memset(&alarmConfig,0,sizeof(alarmConfig));  // Clear alarmConfig variable;
        alarmConfig.seconds_mask = 1;  // Don't care about seconds in alarm configuration

        alarm = 0; // Reset alarm variable

        alarmState = Alarm;  // Start from the beginning state

        while(alarmState != Exit)
        {
            // Get data from the user, until it sends a minute
            xTaskNotifyWait(0, 0, &commandAddr, portMAX_DELAY);  
            command = (command_t*)commandAddr;

            switch (alarmState)
            {
            case Alarm:
                alarm = getNumber(command);
                xQueueSend(queuePrint, &msgConfigWeekday, 0);  // Inform the user that should input weekday of alarm
                alarmState = Weekday;
                break;
            case Weekday:
                alarmConfig.date = getNumber(command);
                alarmConfig.week_day_sel = 1;  // Weekday selection
                xQueueSend(queuePrint, &msgConfigHour, 0);  // Inform the user that should input hour of alarm
                alarmState = Hour;
                break;
            case Hour:
                alarmConfig.hours = getNumber(command);
                xQueueSend(queuePrint, &msgConfigMin, 0);  // Inform the user that should input minute of alarm
                alarmState = Min;
                break;
            case Min:   
                alarmConfig.minutes = getNumber(command);

                // Check the correctness of the time
                if(alarm >2 || alarm < 1 || alarmConfig.hours > 23 || alarmConfig.minutes > 59 || 
                   alarmConfig.date > 7 || alarmConfig.date < 1)  
                {
                    xQueueSend(queuePrint, &msgConfigInvalid, 0);
                }
                else
                {
                    if(alarm == 1)
                    {
                        hrtc.AlarmA = alarmConfig;
                    }
                    else
                    {
                        hrtc.AlarmB = alarmConfig;
                    }
                    RTC_AlarmConfigIT(&hrtc, alarm, RTC_ALARM_ENABLE);  // Enable alarm interrupt
                    xQueueSend(queuePrint, &msgConfigCorrect, 0);
                }
                alarmState = Exit;
                break;
            case Exit:
            default:
                break;
            }

        }

        currentState = sMainMenu;
        xTaskNotify(taskMenuHandle, 0, eNoAction);  // Return to main menu
    }
}


/*
 * This task informs the user, that event was triggered (alarm or button interrupt)
 */
void taskEventTriggered(void* parameters)
{
    uint8_t item;
    const char* msgAlarmA = "\r\n------------------------------\r\n"
                           "|     ALARM A TRIGGERED!     |\r\n"
                           "------------------------------\r\n";
    const char* msgAlarmB = "\r\n------------------------------\r\n"
                           "|     ALARM B TRIGGERED!     |\r\n"
                           "------------------------------\r\n";
    const char* msgMenu = "\r\nEnter number(0-6):";
    while(1)
    {

        xQueueReceive(queueEvents, &item, portMAX_DELAY);  // Wait for events like alarms and button ISR

        while(currentState != sMainMenu);  // Wait until user come back to main meny

        if(item == EVENT_ALARM_A)
        {
            xQueueSend(queuePrint, &msgAlarmA, 0);
            showTimeDate();
        }
        else if(item == EVENT_ALARM_B)
        {
            xQueueSend(queuePrint, &msgAlarmB, 0);
            showTimeDate();
        }
        else if(item == EVENT_BUTTON_IT)
        {
            showTimeDate();
            showTemperature();
            showPressure();
            showLighting();
        }

        xQueueSend(queuePrint, &msgMenu, 0);

    }
}

/*
 * This task shows the user help information
 */
void taskHelp(void* parameters)
{
    const char* msgHelp =  "\r\n--------------------------------------------\r\n"
                           "|                   HELP                   |\r\n"
                           "--------------------------------------------\r\n"
                           "- Photoresistor controls when the LED is ON\r\n"
                           "- User can change the diode actication \r\n"
                           "  threshold by potentiometer\r\n"
                           "- Click the button to see log from sensors\r\n"
                           "- You have to be in main menu to see\r\n"
                           "  information about alarm\r\n"
                           "ENTER MESSAGE TO GO BACK:";
    while(1)
    {
        xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);  // Wait for the user to select an option in main menu

        xQueueSend(queuePrint, &msgHelp, 0);

        xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);  // Wait for the user to send a message to return to the main menu

        currentState = sMainMenu;
        xTaskNotify(taskMenuHandle, 0, eNoAction);  // Return to main menu
    }
}

/*
 * This task turns on/off LED according to voltage on potentiometer and photoresistor
 */
void taskPotentiometer(void* parameters)
{
    TickType_t lastWakeTime;
    float voltagePhotoresistor, voltagePotentiometer;

    lastWakeTime = xTaskGetTickCount();  

    while(1)
    {
        ADC_Start(&hadc1);
        ADC_Start(&hadc2);

        ADC_PollForConversion(&hadc1);
        voltagePhotoresistor = ADC_ConvToVoltage(ADC_Read(&hadc1), ADC_RES_RANGE_12_BIT);
        ADC_PollForConversion(&hadc2);
        voltagePotentiometer = ADC_ConvToVoltage(ADC_Read(&hadc2), ADC_RES_RANGE_12_BIT);

        // Light up LED when voltage on photoresistor is higher than on potentiometer
        if(voltagePhotoresistor > voltagePotentiometer)  
        {
            GPIO_pin_write(LED4_PORT, LED4_PIN, GPIO_PIN_SET);
        }
        else
        {
            GPIO_pin_write(LED4_PORT, LED4_PIN, GPIO_PIN_RESET);
        }

        xTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(50));  // Wake up task every 50ms
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

