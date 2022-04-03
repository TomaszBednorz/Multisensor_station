#include "main.h"


const char *msg_inv = "Invalid option !!!\r\n";


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

void taskPrint(void* parameters)
{
    char* msgBuffer;

    while(1)
    {
        xQueueReceive(queuePrint, &msgBuffer, portMAX_DELAY);
        printMsg(msgBuffer);
        // printMsg(msgBuffer);

    }
}

void taskMenu(void* parameters)
{
    command_t* command;
    uint32_t commandAddr;
    uint8_t nextState;
    const char* msgMenu = "------------------------------\r\n"
                    "|            MENU            |\r\n"
                    "------------------------------\r\n"
                    "0: LED effect\r\n"
                    "1: Temperature\r\n"
                    "2: Pressure\r\n"
                    "3: Lighting\r\n"
                    "4: Time & Date\r\n"
                    "5: Set alarm\r\n"
                    "6: Help\r\n"  // Information about button & potentiometer
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
                // xTaskNotify(taskLedsHandle, 0, eNoAction);
                break;
            case 1:
                // xTaskNotify(..., 0, eNoAction);
                break;
            case 2:
                // xTaskNotify(..., 0, eNoAction);
                break;
            case 3:
                // xTaskNotify(..., 0, eNoAction);
                break;
            case 4:
                // xTaskNotify(..., 0, eNoAction);
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



// void taskLeds(void* parameters)
// {
//     TickType_t xLastWakeTime;
//     xLastWakeTime = xTaskGetTickCount();

//     while(1)
//     {
//         vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(1500));

//         GPIO_pin_toggle(LED3_PORT, LED3_PIN);

//     }
// }



// Idle Hook Function
void vApplicationIdleHook(void)
{
    while(1)
    {
        __WFI();
    }
}

