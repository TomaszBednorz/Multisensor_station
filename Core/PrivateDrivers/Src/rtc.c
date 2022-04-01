#include "rtc.h"

// Static functions prototypes
static uint8_t DecToBCD(uint8_t dec_number);
static uint8_t BCDToDec(uint8_t bcd_number);
static void AlarmSubconfig(RTC_Handle_t *hrtc, uint8_t Alarm);
static void DisableRegsWriteProtection(RTC_Handle_t *hrtc);
static void EnableRegsWriteProtection(RTC_Handle_t *hrtc);

/**
  * @brief RTC configuration function

  * @param  hrtc is handle structure from rtc.h

  * @retval None
  */
void RTC_Init(RTC_Handle_t *hrtc)
{
    DisableRegsWriteProtection(hrtc);  // Enable write to RTC registers

    hrtc->Instance->ISR |= RTC_ISR_INIT;  // Enable initialization mode
    while(!(hrtc->Instance->ISR & RTC_ISR_INITF));  // Wait for initialization flag

    // Asynchronous prescaler
    hrtc->Instance->PRER &= ~RTC_PRER_PREDIV_A;
    hrtc->Instance->PRER |= hrtc->Init.pre_div_A << RTC_PRER_PREDIV_A_Pos;

    // Synchronous prescaler
    hrtc->Instance->PRER &= ~RTC_PRER_PREDIV_S;
    hrtc->Instance->PRER |= hrtc->Init.pre_div_S << RTC_PRER_PREDIV_S_Pos;

    // Hour format
    hrtc->Instance->CR &= ~RTC_CR_FMT;
    hrtc->Instance->CR |= hrtc->Init.format << RTC_CR_FMT_Pos;

    RTC_SetTime(hrtc);
    RTC_SetDate(hrtc);

    DisableRegsWriteProtection(hrtc);  // Enable write to the RTC registers
    hrtc->Instance->ISR &= ~RTC_ISR_INIT;  // Disable initialization mode
    
    EnableRegsWriteProtection(hrtc);  // Disable write to RTC registers
}

/**
  * @brief Function that sets desired time in RTC

  * @param  hrtc is handle structure from rtc.h

  * @retval None
  */
void RTC_SetTime(RTC_Handle_t *hrtc)
{
    uint8_t temp;  // Temporary value
    uint32_t temp_reg = 0x00;  // Temporary register

    DisableRegsWriteProtection(hrtc);  // Enable write to RTC registers

    hrtc->Instance->ISR |= RTC_ISR_INIT;  // Enable initialization mode
    while(!(hrtc->Instance->ISR & RTC_ISR_INITF));  // Wait for initialization flag

    // Seconds
    temp = DecToBCD(hrtc->Time.seconds);
    temp_reg |= temp << RTC_TR_SU_Pos;

    // Minutes
    temp = DecToBCD(hrtc->Time.minutes);
    temp_reg |= temp << RTC_TR_MNU_Pos;

    // Hours
    temp = DecToBCD(hrtc->Time.hours);
    temp_reg |= temp << RTC_TR_HU_Pos;

    // AM/PM notation
    temp_reg |= hrtc->Time.am_pm << RTC_TR_PM_Pos;

    hrtc->Instance->TR = temp_reg;

    hrtc->Instance->ISR &= ~RTC_ISR_INIT;  // Disable initialization mode
    EnableRegsWriteProtection(hrtc);  // Disable write to RTC registers
}

/**
  * @brief Function that update time in htrc handler from RTC

  * @param  hrtc is handle structure from rtc.h

  * @retval None
  */
void RTC_GetTime(RTC_Handle_t *hrtc)
{
    uint32_t temp;  // Temporary value
    uint32_t temp_reg;  // Temporary register

    // Wait for synchronization in the TR shadow register
    hrtc->Instance->ISR &= ~RTC_ISR_RSF;
    while(!(hrtc->Instance->ISR & RTC_ISR_RSF));

    temp_reg = hrtc->Instance->TR;

    // Seconds
    temp = (temp_reg & (RTC_TR_SU | RTC_TR_ST)) >> RTC_TR_SU_Pos;
    hrtc->Time.seconds = BCDToDec((uint8_t)temp & 0x7F);

    // Minutes
    temp = (temp_reg & (RTC_TR_MNU | RTC_TR_MNT)) >> RTC_TR_MNU_Pos;
    hrtc->Time.minutes = BCDToDec((uint8_t)temp & 0x7F);

    // Hours
    temp = (temp_reg & (RTC_TR_HU | RTC_TR_HT)) >> RTC_TR_HU_Pos;
    hrtc->Time.hours = BCDToDec((uint8_t)temp & 0x3F);

    // AM/PM notation
    temp = (temp_reg & RTC_TR_PM) >> RTC_TR_PM_Pos;
    hrtc->Time.am_pm = temp;
}

/**
  * @brief Function that sets desired date in RTC

  * @param  hrtc is handle structure from rtc.h

  * @retval None
  */
void RTC_SetDate(RTC_Handle_t *hrtc)
{
    uint8_t temp;  // Temporary value
    uint32_t temp_reg = 0x00;  // Temporary register

    DisableRegsWriteProtection(hrtc);  // Enable write to RTC registers

    hrtc->Instance->ISR |= RTC_ISR_INIT;  // Enable initialization mode
    while(!(hrtc->Instance->ISR & RTC_ISR_INITF));  // Wait for initialization flag

    // Date
    temp = DecToBCD(hrtc->Date.date);
    temp_reg |= temp << RTC_DR_DU_Pos;

    // Month
    temp = DecToBCD(hrtc->Date.month);
    temp_reg |= temp << RTC_DR_MU_Pos;

    // Week day
    temp_reg |= hrtc->Date.week_day << RTC_DR_WDU_Pos;

    // Year
    temp = DecToBCD(hrtc->Date.year);
    temp_reg |= temp << RTC_DR_YU_Pos;

    hrtc->Instance->DR = temp_reg;

    hrtc->Instance->ISR &= ~RTC_ISR_INIT;  // Disable initialization mode
    EnableRegsWriteProtection(hrtc);  // Disable write to RTC registers
}   

/**
  * @brief Function that update date in htrc handler from RTC

  * @param  hrtc is handle structure from rtc.h

  * @retval None
  */
void RTC_GetDate(RTC_Handle_t *hrtc)
{
    uint32_t temp;  // Temporary value
    uint32_t temp_reg;  // Temporary register

    // Wait for synchronization in the TR shadow register
    hrtc->Instance->ISR &= ~RTC_ISR_RSF;
    while(!(hrtc->Instance->ISR & RTC_ISR_RSF));

    temp_reg = hrtc->Instance->DR;

    // Date
    temp = (temp_reg & (RTC_DR_DU | RTC_DR_DT)) >> RTC_DR_DU_Pos;
    hrtc->Date.date = BCDToDec((uint8_t)temp & 0x3F);

    // Month
    temp = (temp_reg & (RTC_DR_MU | RTC_DR_MT)) >> RTC_DR_MU_Pos;
    hrtc->Date.month = BCDToDec((uint8_t)temp & 0x1F);

    // Week day
    temp = (temp_reg & RTC_DR_WDU) >> RTC_DR_WDU_Pos;
    hrtc->Date.week_day = temp;

    // Year
    temp = (temp_reg & (RTC_DR_YU | RTC_DR_YT)) >> RTC_DR_YU_Pos;
    hrtc->Date.year = BCDToDec((uint8_t)temp & 0xFF);
}

/**
  * @brief Function that configure RTC alarm (interrupt mode)

  * @param  hrtc is handle structure from rtc.h
  * @param  Alarm Alarm, @RTC_ALARM from rtc.h
  * @param  State Enable/disable alarm, @RTC_ALARM_STATE from rtc.h
  * @retval None
  */
void RTC_AlarmConfigIT(RTC_Handle_t *hrtc, uint8_t Alarm, uint8_t State)
{
    DisableRegsWriteProtection(hrtc);  // Enable write to RTC registers

    if(State == RTC_ALARM_DISABLE)
    {
        if(Alarm & RTC_ALARM_A)  // Disable alarm A and alarm A interrupt
        {
            hrtc->Instance->CR &= ~RTC_CR_ALRAE;
            hrtc->Instance->CR &= ~RTC_CR_ALRAIE;
        }
        if(Alarm & RTC_ALARM_B)  // Disable alarm B and alarm B interrupt
        {
            hrtc->Instance->CR &= ~RTC_CR_ALRBE;
            hrtc->Instance->CR &= ~RTC_CR_ALRBIE;
        }
    }
    else if(State == RTC_ALARM_ENABLE)
    {

        if(Alarm & RTC_ALARM_A)  // Enable alarm A and alarm A interrupt
        {
            hrtc->Instance->ISR &= ~RTC_ISR_ALRAF;  // Clear alarm A flag
            AlarmSubconfig(hrtc, RTC_ALARM_A);
            hrtc->Instance->CR |= RTC_CR_ALRAE;
            hrtc->Instance->CR |= RTC_CR_ALRAIE;
        }
        if(Alarm & RTC_ALARM_B)  // Enable alarm B and alarm B interrupt
        {
            hrtc->Instance->ISR &= ~RTC_ISR_ALRBF;  // Clear alarm B flag
            AlarmSubconfig(hrtc, RTC_ALARM_B);
            hrtc->Instance->CR |= RTC_CR_ALRBE;
            hrtc->Instance->CR |= RTC_CR_ALRBIE;
        }
    }

    // Enable EXTI Line 17 and select the rising edge sensitivity
    EXTI->IMR |= EXTI_IMR_MR17;
    EXTI->RTSR |= EXTI_RTSR_TR17;

    EnableRegsWriteProtection(hrtc);  // Disable write to RTC registers
}

/**
  * @brief RTC callback to handle received interrupts

  * @param  hrtc is handle structure from rtc.h

  * @retval None
  */
void RTC_Callback(RTC_Handle_t *hrtc)
{
    EXTI->PR |= EXTI_PR_PR17;  // Clear pending bit by progromming it to '1'
    if(hrtc->Instance->ISR & RTC_ISR_ALRAF)
    {
        hrtc->Instance->ISR &= ~RTC_ISR_ALRAF;  // Clear alarm A flag
        RTC_AlarmACallback(hrtc);
    }
    if(hrtc->Instance->ISR & RTC_ISR_ALRBF)
    {
        hrtc->Instance->ISR &= ~RTC_ISR_ALRBF;  // Clear alarm B flag
        RTC_AlarmACallback(hrtc);
    }
}

/**
  * @brief RTC Alarm A callback

  * @param  hrtc is handle structure from rtc.h

  * @retval None
  */
__attribute__((weak)) void RTC_AlarmACallback(RTC_Handle_t *hrtc)
{
    // Implement this callback in user file without weak attribute if needed
}

/**
  * @brief RTC Alarm B callback

  * @param  hrtc is handle structure from rtc.h

  * @retval None
  */
__attribute__((weak)) void RTC_AlarmBCallback(RTC_Handle_t *hrtc)
{
    // Implement this callback in user file without weak attribute if needed
}


/**
  * @brief Subconfiguration for Alarm, configures the appropriate bits in RTC ALRMAR/ALRMBR register

  * @param  hrtc is handle structure from rtc.h
  * @param  Alarm Alarm, @RTC_ALARM from rtc.h

  * @retval None
  */
static void AlarmSubconfig(RTC_Handle_t *hrtc, uint8_t Alarm)
{
    uint32_t temp;  // Temporary value
    uint32_t temp_reg = 0x00;  // Temporary register
    RTC_Alarm_t alarm_config;

    if(Alarm == RTC_ALARM_A)
    {
        alarm_config = hrtc->AlarmA;
    }
    else if(Alarm == RTC_ALARM_B)
    {
        alarm_config = hrtc->AlarmB;
    }

    // Seconds
    temp = DecToBCD(alarm_config.seconds);
    temp_reg |= (temp & 0x7F) << RTC_ALRMAR_SU_Pos;  
    temp_reg |= (alarm_config.seconds_mask & 0x01) << RTC_ALRMAR_MSK1_Pos;  // Seconds mask

    // Minutes
    temp = DecToBCD(alarm_config.minutes);
    temp_reg |= (temp & 0x7F) << RTC_ALRMAR_MNU_Pos;
    temp_reg |= (alarm_config.minutes_mask & 0x01) << RTC_ALRMAR_MSK2_Pos;

    // Hours
    temp = DecToBCD(alarm_config.hours);
    temp_reg |= (temp & 0x3F) << RTC_ALRMAR_HU_Pos;
    temp_reg |= (alarm_config.hours_mask & 0x01) << RTC_ALRMAR_MSK3_Pos;

    // AM/PM notation
    temp_reg |= (alarm_config.am_pm & 0x01) << RTC_ALRMAR_PM_Pos;

    // Date/Week day
    temp = DecToBCD(alarm_config.date);
    temp_reg |= (temp & 0x3F) << RTC_ALRMAR_DU_Pos;
    temp_reg |= (alarm_config.week_day_sel & 0x01) << RTC_ALRMAR_WDSEL_Pos;  // Week day/ date selectioon
    temp_reg |= (alarm_config.date_mask & 0x01) << RTC_ALRMAR_MSK4_Pos;

    if(Alarm == RTC_ALARM_A)
    {
        hrtc->Instance->ALRMAR = temp_reg;
    }
    else if(Alarm == RTC_ALARM_B)
    {
        hrtc->Instance->ALRMBR = temp_reg;
    }
}

/**
  * @brief Conversion function from decimal format to BCD format 

  * @param  dec_number number in decimal format

  * @retval Number in BCD format (4 bits for tens and 4 bits for units)
  */
static uint8_t DecToBCD(uint8_t dec_number)
{
    uint8_t bcd_number = 0x00, tens, units;

    tens = dec_number / 10;
    units = dec_number % 10;
    bcd_number = (tens << 4) | units;

    return bcd_number;
}

/**
  * @brief Conversion function from BCD format  (4 bits for tens and 4 bits for units) to decimal format 

  * @param  bcd_number number in bcd format

  * @retval Number in decimal format
  */
static uint8_t BCDToDec(uint8_t bcd_number)
{
    uint8_t dec_number = 0x00, tens, units;

    units = bcd_number & 0x0F;
    tens = ((bcd_number & 0xF0) >> 4);
    dec_number = tens * 10 + units;

    return dec_number;

}

/**
  * @brief The following steps are required to unlock the write protection on all the RTC registers
  *        except for RTC_ISR[13:8], RTC_TAFCR, and RTC_BKPxR

  * @param  hrtc is handle structure from rtc.h

  * @retval None
  */
static void DisableRegsWriteProtection(RTC_Handle_t *hrtc)
{
    PWR->CR |= PWR_CR_DBP;  // Disable backup domain write protection
	for(uint32_t i = 0; i < 100; i++);  // Wait for activation above line (didn't find status register)

    hrtc->Instance->WPR |= 0xCA;
    hrtc->Instance->WPR |= 0x53;
}

/**
  * @brief Disable the write protection for writing all the RTC registers except for 
  *        RTC_ISR[13:8], RTC_TAFCR, and RTC_BKPxR

  * @param  hrtc is handle structure from rtc.h

  * @retval None
  */
static void EnableRegsWriteProtection(RTC_Handle_t *hrtc)
{
    hrtc->Instance->WPR |= 0x0F;
    PWR->CR &= ~PWR_CR_DBP;  // Enable backup domain write protection
}
