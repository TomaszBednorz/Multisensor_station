#include "main.h"

extern RTC_Handle_t hrtc;

const char* dayWeekLUT[7] = {"MON", "TUE", "WED", "THU", "FRI", "SAT", "SUN"};

void showTimeDateMenu(void)
{
    uint8_t date, month, week_day, year, seconds, minutes, hours;

    const char* msgMenu1 = "\r\n------------------------------\r\n"
                           "|       SELECT OPTION        |\r\n"
                           "------------------------------\r\n";
    const char* msgMenu2 = "------------------------------\r\n"
                           "0: Set time\r\n"
                           "1: Set date\r\n"
                           "2: Exit\r\n"
                           "Enter number(0-3):";

    static char msgTimeDate[45];
    static char *msg = msgTimeDate;

    // Update time and date in hrtc handle
    RTC_GetTime(&hrtc);
    RTC_GetDate(&hrtc);

    // Store actual time and date to variables
    date = hrtc.Date.date;
    month = hrtc.Date.month;
    week_day = hrtc.Date.week_day;
    year = hrtc.Date.year;

    seconds = hrtc.Time.seconds;
    minutes = hrtc.Time.minutes;
    hours = hrtc.Time.hours;


    sprintf((char*)msgTimeDate, "Date: %d.%d.20%dr %s\r\nTime: %d:%d:%d\r\n", date, month, year, dayWeekLUT[week_day-1], hours, minutes, seconds);
    xQueueSend(queuePrint, &msgMenu1, portMAX_DELAY);
    xQueueSend(queuePrint, &msg, portMAX_DELAY);
    xQueueSend(queuePrint, &msgMenu2, portMAX_DELAY);
}

// max 2 digits in number
uint8_t getNumber(command_t* command)
{
    uint8_t firstDig = 0, secondDig = 0;
    uint8_t result = 0;

    if(command->length == 1)
    {
        firstDig = command->payload[0] - 48;
        result = firstDig;
    }
    else if(command->length == 2)
    {
        firstDig = command->payload[0] - 48;
        secondDig = command->payload[1] - 48;
        result = firstDig * 10 + secondDig;
    }

    return result;
}
