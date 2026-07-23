/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#include "sn34f78x_ll_rtc.h"

#if defined(USE_FULL_LL_DRIVER)

static uint8_t _rtc_bcd_to_byte(uint8_t value);

/* Exported functions --------------------------------------------------------*/
/**
 * @brief  Initialize RTC registers according to the specified parameters in rtc_init.
 * @param  rtc RTC Instance
 * @param  rtc_init pointer to a LL_RTC_Init_T structure that contains the configuration information for the specified RTC peripheral.
 * @retval An LL_Status_T enumeration value:
 *          - LL_OK: RTC registers are initialized according to RTC_InitStruct content
 *          - LL_ERROR: Problem occurred during RTC Registers initialization
 */
LL_Status_T LL_RTC_Init(LL_RTC_Init_T *rtc_init)
{
    LL_Status_T    status = LL_OK;
    uint32_t       new_trim_p, new_trim_f;
    const uint32_t trim_p = 327;

    /* Check the parameters */
    RET_FLAG_FALSE(IS_LL_RTC_CLK_SOURCE(rtc_init->clk_source), LL_ERROR);

    /* In init, if RTC is already enabled, directly change state to ready (wake up from DPD multiple times but keep RTC state). */
    if (LL_RTC_IsEnabled_RTCCLK() == 1)
        return LL_OK;

    /* 1、Configure RTC Clock ： ILRC(32KHZ) or ELS(32.768KHZ) */
    /* Adjust(calibration) RTC clock frequency:             */
    if (rtc_init->clk_source == LL_RTC_CLK_SOURCE_ILRC)
    {
        /* RTC Counter Clock Source Select */
        LL_RTC_ClockSelect_ILRC();
    }
    else
    {
        /* RTC Counter Clock Source Select */
        LL_RCC_Enable_ELS();

        /* check ELS ready flag */
        while (!(LL_RCC_IsActiveFlag_ELS() == 1))
            ;
        LL_RTC_ClockSelect_ELS();

        /* Compensates for frequency drift caused by crystal error of external clock source */

        /* wNewTrimP = wELSOscFreq/100; */
        new_trim_p = (1000000 * (trim_p + 1)) / LL_RTC_MEASURE_DATA;
        /* wNewTrimF = (wELSOscFreq%100)*10; */
        new_trim_f = ((1000000 * (trim_p + 1)) % LL_RTC_MEASURE_DATA) / 1000;

        *(uint32_t *)(0x4001F014) = LL_RTC_Get_ChipId();

        LL_RTC_Set_TRIM_P(new_trim_p);
        LL_RTC_Set_TRIM_F(new_trim_f);
    }

    /* 2、Enable RTC clock  register */
    LL_RTC_Enable_RTCCLK();

    while (LL_RTC_IsActiveFlag_RTC_Ready() != 1)
    {
        __NOP();
    }

    /* 3、disable RTC */
    LL_RTC_Disable();

    /* wait until the RTC_STS bit is changed to rtc_sts and the setting of the RTC alarm is complete. */
    while (LL_RTC_IsActiveStatus_RTC() == 1)
    {
        __NOP();
    }

    /* 4、enable RTC */
    LL_RTC_Enable();

    /* wait until the RTC_STS bit is changed to rtc_sts and the setting of the RTC alarm is complete. */
    while (LL_RTC_IsActiveStatus_RTC() != 1)
    {
        __NOP();
    }

    return status;
}

/**
 * @brief  De-initialize RTC registers (Registers restored to their default values).
 * @param  rtc_init pointer to a LL_RTC_Init_T structure that contains the configuration information for the specified RTC peripheral.
 * @retval An ErrorStatus enumeration value:
 *          - SUCCESS: RTC registers are de-initialized
 *          - ERROR: RTC registers are not de-initialized
 */
LL_Status_T LL_RTC_DeInit(LL_RTC_Init_T *rtc_init)
{
    LL_Status_T status = LL_OK;

    /* disable SECOND */
    LL_RTC_Disable_Secout();

    /* disable ALARM */
    LL_RTC_Disable_Alarm();

    /* wait until the RTC_STS bit is changed to rtc_sts or the setting of the RTC alarm is complete. */
    while (LL_RTC_IsActiveStatus_Alarm() == 1)
    {
        __NOP();
    }

    /* disable RTC */
    LL_RTC_Disable();

    /* wait until the RTC_STS bit is changed to rtc_sts or the setting of the RTC alarm is complete. */
    while (LL_RTC_IsActiveStatus_RTC() == 1)
    {
        __NOP();
    }

    /* 2024/1/4 Confirm with TW colleagues that the clock source should be reset to ILRC when deinit */
    LL_RTC_ClockSelect_ILRC();

    /* disable RTCCLK */
    LL_RTC_Disable_RTCCLK();

    return status;
}

/**
 * @brief Set each @ref LL_RTC_Init_T field to default value.
 * @param rtc_init Pointer to a @ref LL_RTC_Init_T structure whose fields will be set to default values.
 * @retval None
 */
void LL_RTC_StructInit(LL_RTC_Init_T *rtc_init)
{
    /* Set RTC_InitStruct fields to default values */
    rtc_init->clk_source = LL_RTC_CLK_SOURCE_ILRC;
}

LL_Status_T LL_RTC_TIME_Init(LL_RTC_Time_T *rtc_time, uint32_t format)
{
    /* Check the parameters          */
    RET_FLAG_FALSE(IS_LL_RTC_FORMAT(format), LL_ERROR);

    if (format == LL_RTC_FORMAT_BIN)
    {
        /* Check Hours,Minutes,Seconds Parameter */
        RET_FLAG_FALSE(IS_LL_RTC_HOUR24(rtc_time->hours), LL_ERROR);
        RET_FLAG_FALSE(IS_LL_RTC_MINUTES(rtc_time->minutes), LL_ERROR);
        RET_FLAG_FALSE(IS_LL_RTC_SECONDS(rtc_time->seconds), LL_ERROR);
    }
    else // format == LL_RTC_FORMAT_BCD
    {
        /* Check Hours,Minutes,Seconds Parameter for RTC_FORMAT_BCD */
        /* eg: 21: 05:36 (Be equivalent to) 0x21: 0x05: 0x36 */
        RET_FLAG_FALSE(IS_LL_RTC_HOUR24(_rtc_bcd_to_byte(rtc_time->hours)), LL_ERROR);
        RET_FLAG_FALSE(IS_LL_RTC_MINUTES(_rtc_bcd_to_byte(rtc_time->minutes)), LL_ERROR);
        RET_FLAG_FALSE(IS_LL_RTC_SECONDS(_rtc_bcd_to_byte(rtc_time->seconds)), LL_ERROR);
    }

    LL_Status_T status = LL_OK;

    /* Check and read the RTCEN_STS bit of the RTC control register to confirm if RTC is disabled. */
    /* Init tick_start for timeout management */
    LL_RTC_Disable();
    while (LL_RTC_IsActiveStatus_RTC() == 1)
        ;

    /* 3. Set new time to registers Time1 and Time2 */
    if (format == LL_RTC_FORMAT_BIN)
    {
        LL_RTC_Set_Hour(rtc_time->hours);
        LL_RTC_Set_Minute(rtc_time->minutes);
        LL_RTC_Set_Second(rtc_time->seconds);
    }
    else // format == RTC_FORMAT_BCD
    {
        LL_RTC_Set_BCD_Hour(rtc_time->hours);
        LL_RTC_Set_BCD_Minute(rtc_time->minutes);
        LL_RTC_Set_BCD_Second(rtc_time->seconds);
    }

    LL_RTC_Enable();
    while (LL_RTC_IsActiveStatus_RTC() == 0)
        ;

    return status;
}

void LL_RTC_TIME_StructInit(LL_RTC_Time_T *rtc_time)
{
    rtc_time->hours   = 0;
    rtc_time->minutes = 0;
    rtc_time->seconds = 0;
}

LL_Status_T LL_RTC_DATE_Init(LL_RTC_Date_T *rtc_date, uint32_t format)
{
    /* Check the parameters */
    RET_FLAG_FALSE(IS_LL_RTC_FORMAT(format), LL_ERROR);
    RET_FLAG_FALSE(IS_LL_RTC_WEEKDAY(rtc_date->week_day), LL_ERROR);

    /* BIN, BCD compatible processing about months */
    if ((LL_RTC_FORMAT_BIN == format) && (((rtc_date->month) & 0x10) == 0x10))
    {
        rtc_date->month = (rtc_date->month & 0xf) + 0xa;
    }

    if (format == LL_RTC_FORMAT_BIN)
    {
        /* Check Hours,Minutes,Seconds Parameter */
        /*eg:  2023-3-5 */
        RET_FLAG_FALSE(IS_LL_RTC_CENTURY(rtc_date->century), LL_ERROR);
        RET_FLAG_FALSE(IS_LL_RTC_YEAR(rtc_date->year), LL_ERROR);
        RET_FLAG_FALSE(IS_LL_RTC_MONTH(rtc_date->month), LL_ERROR);
        RET_FLAG_FALSE(IS_LL_RTC_DATE(rtc_date->date), LL_ERROR);
    }
    else // format == RTC_FORMAT_BCD
    {
        /* eg: 0x2023 -- 0x12 -- 0x25 */
        RET_FLAG_FALSE(IS_LL_RTC_CENTURY(_rtc_bcd_to_byte(rtc_date->century)), LL_ERROR);
        RET_FLAG_FALSE(IS_LL_RTC_YEAR(_rtc_bcd_to_byte(rtc_date->year)), LL_ERROR);
        RET_FLAG_FALSE(IS_LL_RTC_MONTH(_rtc_bcd_to_byte(rtc_date->month)), LL_ERROR);
        RET_FLAG_FALSE(IS_LL_RTC_DATE(_rtc_bcd_to_byte(rtc_date->date)), LL_ERROR);
    }

    LL_Status_T status = LL_OK;

    /* 1. Check and read the RTCEN_STS bit of the RTC control register to confirm if RTC is disabled. */
    /* Init tick_start for timeout management */
    LL_RTC_Disable();
    while (LL_RTC_IsActiveStatus_RTC() == 1)
        ;

    /* 2. Set new time to registers Time1 and Time2 */
    if (format == LL_RTC_FORMAT_BIN)
    {
        LL_RTC_Set_Century(rtc_date->century);
        LL_RTC_Set_Year(rtc_date->year);
        LL_RTC_Set_Month(rtc_date->month);
        LL_RTC_Set_Date(rtc_date->date);
        LL_RTC_Set_Weekday(rtc_date->week_day);
    }
    else // format == LL_RTC_FORMAT_BCD
    {
        LL_RTC_Set_BCD_Century(rtc_date->century);
        LL_RTC_Set_BCD_Year(rtc_date->year);
        LL_RTC_Set_BCD_Month(rtc_date->month);
        LL_RTC_Set_BCD_Date(rtc_date->date);
        LL_RTC_Set_BCD_Weekday(rtc_date->week_day);
    }

    LL_RTC_Enable();
    while (LL_RTC_IsActiveStatus_RTC() == 0)
        ;

    return status;
}

void LL_RTC_DATE_StructInit(LL_RTC_Date_T *rtc_date)
{
    rtc_date->century  = 0;
    rtc_date->year     = 0;
    rtc_date->month    = LL_RTC_MONTH_JANUARY;
    rtc_date->date     = 0;
    rtc_date->week_day = LL_RTC_WEEKDAY_MONDAY;
}

LL_Status_T LL_RTC_ALARM_Init(LL_RTC_Alarm_T *rtc_alarm, uint32_t format)
{
    LL_Status_T status = LL_OK;

    /* Check the parameters */
    RET_FLAG_FALSE(rtc_alarm != NULL, LL_ERROR);
    RET_FLAG_FALSE(IS_LL_RTC_FORMAT(format), LL_ERROR);

    RET_FLAG_FALSE(IS_LL_RTC_ALARM_DATE_WEEKDAY_SEL(rtc_alarm->alarm_date_weekday_sel), LL_ERROR);

    /* Check the data format (Binary or BCD) and store the Alarm time and date configuration accordingly */
    if (format == LL_RTC_FORMAT_BIN)
    {
        RET_FLAG_FALSE(IS_LL_RTC_HOUR24(rtc_alarm->alarm_time.hours), LL_ERROR);
        RET_FLAG_FALSE(IS_LL_RTC_MINUTES(rtc_alarm->alarm_time.minutes), LL_ERROR);
        RET_FLAG_FALSE(IS_LL_RTC_SECONDS(rtc_alarm->alarm_time.seconds), LL_ERROR);

        if (rtc_alarm->alarm_date_weekday_sel == LL_RTC_ALARMSEL_DATE)
        {
            RET_FLAG_FALSE(IS_LL_RTC_ALARM_DATE_WEEKDAY_DATE(rtc_alarm->alarm_date_weekday), LL_ERROR);
        }
        else
        {
            RET_FLAG_FALSE(IS_LL_RTC_ALARM_DATE_WEEKDAY_WEEKDAY(rtc_alarm->alarm_date_weekday), LL_ERROR);
        }
    }
    else // format == RTC_FORMAT_BCD
    {
        RET_FLAG_FALSE(IS_LL_RTC_HOUR24(_rtc_bcd_to_byte(rtc_alarm->alarm_time.hours)), LL_ERROR);
        RET_FLAG_FALSE(IS_LL_RTC_MINUTES(_rtc_bcd_to_byte(rtc_alarm->alarm_time.minutes)), LL_ERROR);
        RET_FLAG_FALSE(IS_LL_RTC_SECONDS(_rtc_bcd_to_byte(rtc_alarm->alarm_time.seconds)), LL_ERROR);

        if (rtc_alarm->alarm_date_weekday_sel == LL_RTC_ALARMSEL_DATE)
        {
            RET_FLAG_FALSE(IS_LL_RTC_ALARM_DATE_WEEKDAY_DATE(_rtc_bcd_to_byte(rtc_alarm->alarm_date_weekday)), LL_ERROR);
        }
        else
        {
            RET_FLAG_FALSE(IS_LL_RTC_ALARM_DATE_WEEKDAY_WEEKDAY(_rtc_bcd_to_byte(rtc_alarm->alarm_date_weekday)), LL_ERROR);
        }
    }

    /* Check and read the RTC_ALM_STS bit of the RTC control register to confirm if RTC alarm is disabled */
    LL_RTC_Disable_Alarm();
    while (LL_RTC_IsActiveStatus_Alarm() == 1)
        ;

    /* Set new time to registers, Alarm Time1 and Alarm Time2       */
    if (format == LL_RTC_FORMAT_BIN)
    {
        /* Avoid : DATE and Week cannot be matched at the same time.!!! */
        if (rtc_alarm->alarm_date_weekday_sel == LL_RTC_ALARMSEL_DATE)
        {
            LL_RTC_SetAlarm_Date(rtc_alarm->alarm_date_weekday);
            LL_RTC_SetAlarm_Weekday(0x7);
        }
        else
        {
            LL_RTC_SetAlarm_Weekday(rtc_alarm->alarm_date_weekday);
            LL_RTC_SetAlarm_Date(0);
        }

        LL_RTC_SetAlarm_Hour(rtc_alarm->alarm_time.hours);
        LL_RTC_SetAlarm_Minute(rtc_alarm->alarm_time.minutes);
        LL_RTC_SetAlarm_Second(rtc_alarm->alarm_time.seconds);
    }
    else
    {
        /* Avoid:DATE and Week cannot be matched at the same time.!!! */
        if (rtc_alarm->alarm_date_weekday_sel == LL_RTC_ALARMSEL_DATE)
        {
            LL_RTC_SetAlarm_BCD_Date(rtc_alarm->alarm_date_weekday);
            LL_RTC_SetAlarm_BCD_Weekday(0x7);
        }
        else
        {
            LL_RTC_SetAlarm_BCD_Weekday(rtc_alarm->alarm_date_weekday);
            LL_RTC_SetAlarm_BCD_Date(0);
        }
        LL_RTC_SetAlarm_BCD_Hour(rtc_alarm->alarm_time.hours);
        LL_RTC_SetAlarm_BCD_Minute(rtc_alarm->alarm_time.minutes);
        LL_RTC_SetAlarm_BCD_Second(rtc_alarm->alarm_time.seconds);
    }

    /* Set the RTC_EN bit and wait until the RTCEN_STS bit is changed to '1' and the RTC counter will be re-enabled */
    LL_RTC_Enable_Alarm();
    while (LL_RTC_IsActiveStatus_Alarm() == 0)
        ;

    LL_SCU_DisableIT_RTC_ALARM();

    return status;
}

void LL_RTC_ALARM_StructInit(LL_RTC_Alarm_T *rtc_alarm)
{
    rtc_alarm->alarm_time.hours       = 0;
    rtc_alarm->alarm_time.minutes     = 0;
    rtc_alarm->alarm_time.seconds     = 0;
    rtc_alarm->alarm_date_weekday_sel = LL_RTC_ALARMSEL_WEEKDAY;
    rtc_alarm->alarm_date_weekday     = LL_RTC_WEEKDAY_MONDAY;
}

/* static functions --------------------------------------------------------*/
static uint8_t _rtc_bcd_to_byte(uint8_t value)
{
    uint32_t tmp = 0U;
    tmp          = ((uint8_t)(value & (uint8_t)0xF0) >> (uint8_t)0x4) * 10;
    return (tmp + (value & (uint8_t)0x0F));
}

#endif /* USE_FULL_LL_DRIVER */
