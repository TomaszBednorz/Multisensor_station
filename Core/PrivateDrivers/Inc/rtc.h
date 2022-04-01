#ifndef __RTC_H
#define __RTC_H

#include <stm32f4xx.h>
#include <stdint.h>

// @RTC_FORMAT
#define RTC_FORMAT_24_HOURS      0x00
#define RTC_FORMAT_AM_PM         0x01

// @RTC_DAY
#define RTC_DAY_MONDAY           0x01    
#define RTC_DAY_TUESDAY          0x02 
#define RTC_DAY_WEDNESDAY        0x03   
#define RTC_DAY_THURSDAY         0x04   
#define RTC_DAY_FRIDAY           0x05   
#define RTC_DAY_SATURDAY         0x06   
#define RTC_DAY_SUNDAY           0x07   

// @RTC_MONTH
#define RTC_DAY_JANUARY           0x01  
#define RTC_DAY_FEBRUARY          0x02
#define RTC_DAY_MARCH             0x03 
#define RTC_DAY_APRIL             0x04 
#define RTC_DAY_MAY               0x05 
#define RTC_DAY_JUNE              0x06 
#define RTC_DAY_JULY              0x07 
#define RTC_DAY_AUGUST            0x08 
#define RTC_DAY_SEPTEMBER         0x09 
#define RTC_DAY_OCTOBER           0x0A 
#define RTC_DAY_NOVEMBER          0x0B 
#define RTC_DAY_DECEMBER          0x0C 

// @RTC_ALARM
#define RTC_ALARM_A               0x01 
#define RTC_ALARM_B               0x02 

// @RTC_ALARM_STATE
#define RTC_ALARM_DISABLE         0x00
#define RTC_ALARM_ENABLE          0x01


typedef struct{
    uint8_t  format;        // Hour format, @RTC_FORMAT
    uint8_t  pre_div_A;     // Asynchronous prescaler factor from 0 to 127
    uint16_t pre_div_S;     // Synchronous prescaler factor from 0 to 32767
}RTC_Init_t;


typedef struct{
    uint8_t date;           // Date from 1 to 28:31
    uint8_t month;          // Month, @RTC_MONTH
    uint8_t week_day;       // Week day, @RTC_DAY
    uint8_t year;           // Year from 0 to 99 + 2000
}RTC_Data_t;

typedef struct{
    uint8_t seconds;        // Seconds from 0 to 59
    uint8_t minutes;        // Minutes from 0 to 59
    uint8_t hours;          // Hours from 0 to 23
    uint8_t am_pm;          // AM/PM notation (0 - AM or 24-hour format, 1 - PM)
}RTC_Time_t;

typedef struct{
    uint8_t seconds;         // Second from 0 to 59
    uint8_t seconds_mask;    // Alarm mask seconds (0 - alarm set if seconds match, 1 - seconds don't care)
    uint8_t minutes;         // Minutes from 0 to 59
    uint8_t minutes_mask;    // Alarm mask minutes (0 - alarm set if minutes match, 1 - minutes don't care)
    uint8_t hours;           // Hours from 0 to 23
    uint8_t am_pm;           // AM/PM notation (0 - AM or 24-hour format, 1 - PM)
    uint8_t hours_mask;      // Alarm mask hours (0 - alarm set if hours match, 1 - hours don't care)
    uint8_t date;            // Date from 2 to 28:31 or week day from @RTC_DAY. Depends on week_day_sel
    uint8_t week_day_sel;    // Week day selection (0 - date, 1 - weekday)
    uint8_t date_mask;       // Alarm mask date/weekday (0 - alarm set if date/weekday match, 1 - date/weekday don't care)
}RTC_Alarm_t;

typedef struct{
    RTC_TypeDef *Instance;  // RTC registers base address
    RTC_Init_t  Init;
    RTC_Data_t  Date;
    RTC_Time_t  Time;
    RTC_Alarm_t AlarmA;
    RTC_Alarm_t AlarmB;
}RTC_Handle_t;

// Function prototypes
void RTC_Init(RTC_Handle_t *hrtc);
void RTC_SetTime(RTC_Handle_t *hrtc);
void RTC_GetTime(RTC_Handle_t *hrtc);
void RTC_SetDate(RTC_Handle_t *hrtc);
void RTC_GetDate(RTC_Handle_t *hrtc);
void RTC_AlarmConfigIT(RTC_Handle_t *hrtc, uint8_t Alarm, uint8_t State);

// Callbacks
void RTC_Callback(RTC_Handle_t *hrtc);
void RTC_AlarmACallback(RTC_Handle_t *hrtc);
void RTC_AlarmBCallback(RTC_Handle_t *hrtc);

#endif /* __RTC_H */