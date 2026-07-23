/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _LL_RTC_H_
#define _LL_RTC_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "ll_scu.h"
#include "ll_reg_msk.h"

/* Exported types ------------------------------------------------------------*/
/**
 * @brief  RTC Configuration Structure definition
 * \ingroup rtc_struct_define
 */
typedef struct
{
    uint32_t clk_source; /**< Specifies the RTC Clock Source.
                            This parameter can be a value of @ref rtc_clk_source */

} LL_RTC_Init_T;

/**
 * @brief   RTC Time structure definition
 * @ingroup rtc_struct_define
 */
typedef struct
{
    uint8_t hours;   /**< Specifies the RTC Time Hour.
                          This parameter must be a number between Min_Data = 0 and Max_Data = 12 if the RTC_HourFormat_12 is selected
                          This parameter must be a number between Min_Data = 0 and Max_Data = 23 if the RTC_HourFormat_24 is selected */
    uint8_t minutes; /**< Specifies the RTC Time Minutes.
                          This parameter must be a number between Min_Data = 0 and Max_Data = 59 */
    uint8_t seconds; /**< Specifies the RTC Time Seconds.
                          This parameter must be a number between Min_Data = 0 and Max_Data = 59 */
} LL_RTC_Time_T;

/**
 * @brief  RTC Date structure definition
 * @ingroup rtc_struct_define
 */
typedef struct
{
    uint8_t week_day; /**< Specifies the RTC Date WeekDay.
                           This parameter can be a value of @ref rtc_weekday                       */
    uint8_t month;    /**< Specifies the RTC Date Month in BCD format
                           This parameter can be a value of @ref rtc_month_date                    */
    uint8_t date;     /**< Specifies the RTC Dat
                           This parameter must be a number between Min_Data = 1 and Max_Data = 31  */
    uint8_t year;     /**< Specifies the RTC Date Yea
                           This parameter must be a number between Min_Data = 0 and Max_Data = 99  */
    uint8_t century;  /**< Specifies the RTC Date Century.
                           This parameter must be a number between Min_Data = 0 and Max_Data = 99  */
} LL_RTC_Date_T;

/**
 * @brief  RTC Alarm structure definition
 * @ingroup rtc_struct_define
 */
typedef struct
{
    LL_RTC_Time_T alarm_time;             /**< Specifies the RTC Alarm Time members                                                             */
    uint32_t      alarm_date_weekday_sel; /**< Specifies the RTC Alarm is on Date or WeekDa
                                               This parameter can be a value of @ref rtc_alarm_selection                                        */
    uint32_t alarm_date_weekday;          /**< Specifies the RTC Alarm Date/WeekDa
                                               If the Alarm Date is selected, this parameter must be set to a value in the 1-31 rang
                                               If the Alarm WeekDay is selected, this parameter can be a value of @ref rtc_weekday              */
} LL_RTC_Alarm_T;

/* Exported constants --------------------------------------------------------*/

/**
 * \defgroup rtc_clk_source RTC Clock Source
 * \ingroup rtc_control
 * @{
 */
#define LL_RTC_CLK_SOURCE_ILRC 0x00000000U /**< RTC ILRC 32K clock resource     */
#define LL_RTC_CLK_SOURCE_ELS  0x00000001U /**< RTC ELS 32.768K clock resource  */
/**
 * @}
 */

/**
 * \defgroup rtc_periodic_const Periodic interrupt output signal select
 * \ingroup rtc_control
 * @{
 */
#define LL_RTC_PERIODIC_DISABLE (0x00U) /**< Disable   */
#define LL_RTC_PERIODIC_MONTH   (0x03U) /**< Each month and triggered at 0 second of every month  */
#define LL_RTC_PERIODIC_DAY     (0x04U) /**< Each day and triggered at 0 second of every day     */
#define LL_RTC_PERIODIC_HOUR    (0x05U) /**< Each hour and triggered at 0 second of every hour     */
#define LL_RTC_PERIODIC_MINUTE  (0x06U) /**< Each minute and triggered at 0 second of every minute       */
#define LL_RTC_PERIODIC_SECOND  (0x07U) /**< Each second      */
/**
 * @}
 */

/**
 * \defgroup rtc_month_date BCD Month Date
 * \ingroup rtc_control
 * @{
 */
#define LL_RTC_MONTH_JANUARY   (0x01U) /**< RTC bcd code for month JANUARY   */
#define LL_RTC_MONTH_FEBRUARY  (0x02U) /**< RTC bcd code for month FEBRUARY  */
#define LL_RTC_MONTH_MARCH     (0x03U) /**< RTC bcd code for month MARCH     */
#define LL_RTC_MONTH_APRIL     (0x04U) /**< RTC bcd code for month APRIL     */
#define LL_RTC_MONTH_MAY       (0x05U) /**< RTC bcd code for month MAY       */
#define LL_RTC_MONTH_JUNE      (0x06U) /**< RTC bcd code for month JUNE      */
#define LL_RTC_MONTH_JULY      (0x07U) /**< RTC bcd code for month JULY      */
#define LL_RTC_MONTH_AUGUST    (0x08U) /**< RTC bcd code for month AUGUST    */
#define LL_RTC_MONTH_SEPTEMBER (0x09U) /**< RTC bcd code for month SEPTEMBER */
#define LL_RTC_MONTH_OCTOBER   (0x10U) /**< RTC bcd code for month OCTOBER   */
#define LL_RTC_MONTH_NOVEMBER  (0x11U) /**< RTC bcd code for month NOVEMBER  */
#define LL_RTC_MONTH_DECEMBER  (0x12U) /**< RTC bcd code for month DECEMBER  */
/**
 * @}
 */

/**
 * \defgroup rtc_weekday RTC Weekday
 * \ingroup rtc_control
 * @{
 */
#define LL_RTC_WEEKDAY_SUNDAY    (0x00U) /**< RTC weekday code for SUNDAY    */
#define LL_RTC_WEEKDAY_MONDAY    (0x01U) /**< RTC weekday code for MONDAY    */
#define LL_RTC_WEEKDAY_TUESDAY   (0x02U) /**< RTC weekday code for TUESDAY   */
#define LL_RTC_WEEKDAY_WEDNESDAY (0x03U) /**< RTC weekday code for WEDNESDAY */
#define LL_RTC_WEEKDAY_THURSDAY  (0x04U) /**< RTC weekday code for THURSDAY  */
#define LL_RTC_WEEKDAY_FRIDAY    (0x05U) /**< RTC weekday code for FRIDAY    */
#define LL_RTC_WEEKDAY_SATURDAY  (0x06U) /**< RTC weekday code for SATURDAY  */
/**
 * @}
 */

/**
 * \defgroup rtc_alarm_selection RTC Alarm Selection
 * \ingroup rtc_control
 * @{
 */
#define LL_RTC_ALARMSEL_DATE    0x00000000U /**< RTC Alarm Selection date    */
#define LL_RTC_ALARMSEL_WEEKDAY 0x00000001U /**< RTC Alarm Selection weekday */
/**
 * @}
 */

/**
 * \defgroup rtc_backup RTC Backup Register
 * \ingroup rtc_control
 * @{
 */
#define LL_RTC_BKP_DR0  0x00000000U /**< RTC backup register index */
#define LL_RTC_BKP_DR1  0x00000001U /**< RTC backup register index */
#define LL_RTC_BKP_DR2  0x00000002U /**< RTC backup register index */
#define LL_RTC_BKP_DR3  0x00000003U /**< RTC backup register index */
#define LL_RTC_BKP_DR4  0x00000004U /**< RTC backup register index */
#define LL_RTC_BKP_DR5  0x00000005U /**< RTC backup register index */
#define LL_RTC_BKP_DR6  0x00000006U /**< RTC backup register index */
#define LL_RTC_BKP_DR7  0x00000007U /**< RTC backup register index */
#define LL_RTC_BKP_DR8  0x00000008U /**< RTC backup register index */
#define LL_RTC_BKP_DR9  0x00000009U /**< RTC backup register index */
#define LL_RTC_BKP_DR10 0x0000000AU /**< RTC backup register index */
#define LL_RTC_BKP_DR11 0x0000000BU /**< RTC backup register index */
#define LL_RTC_BKP_DR12 0x0000000CU /**< RTC backup register index */
#define LL_RTC_BKP_DR13 0x0000000DU /**< RTC backup register index */
#define LL_RTC_BKP_DR14 0x0000000EU /**< RTC backup register index */
#define LL_RTC_BKP_DR15 0x0000000FU /**< RTC backup register index */
#define LL_RTC_BKP_DR16 0x00000010U /**< RTC backup register index */
#define LL_RTC_BKP_DR17 0x00000011U /**< RTC backup register index */
#define LL_RTC_BKP_DR18 0x00000012U /**< RTC backup register index */
#define LL_RTC_BKP_DR19 0x00000013U /**< RTC backup register index */
/**
 * @}
 */

/**
 * \defgroup rtc_format RTC Format
 * \ingroup rtc_control
 * @{
 */
#define LL_RTC_FORMAT_BIN 0x00000000U /**< RTC bin code  */
#define LL_RTC_FORMAT_BCD 0x00000001U /**< RTC bcd code  */
/**
 * @}
 */

/* Exported functions --------------------------------------------------------*/
/**
 * @brief  Enable  clock for RTC
 * @retval None.
 */
__STATIC_INLINE void LL_RTC_Enable_RTCCLK(void)
{
    LL_REG_SBIT(SN_ALWAYSON->POR_MISC, ALWAYSON_POR_MISC_RTCCLKEN);
}

/**
 * @brief  Disable  clock for RTC
 * @retval None.
 */
__STATIC_INLINE void LL_RTC_Disable_RTCCLK(void)
{
    LL_REG_CBIT(SN_ALWAYSON->POR_MISC, ALWAYSON_POR_MISC_RTCCLKEN);
}

/**
 * @brief  Determine if  clock for RTC is enabled
 * @retval  clock for RTC enable state.
 */
__STATIC_INLINE uint32_t LL_RTC_IsEnabled_RTCCLK(void)
{
    return (LL_REG_RBIT(SN_ALWAYSON->POR_MISC, ALWAYSON_POR_MISC_RTCCLKEN) == ALWAYSON_POR_MISC_RTCCLKEN);
}

/**
 * @brief  RTC clock source select ELS
 * @retval None.
 */
__STATIC_INLINE void LL_RTC_ClockSelect_ELS(void)
{
    LL_REG_CBIT(SN_ALWAYSON->POR_MISC, ALWAYSON_POR_MISC_RTCCLKSEL);
}

/**
 * @brief  RTC clock source select ILRC
 * @retval None.
 */
__STATIC_INLINE void LL_RTC_ClockSelect_ILRC(void)
{
    LL_REG_SBIT(SN_ALWAYSON->POR_MISC, ALWAYSON_POR_MISC_RTCCLKSEL);
}

/**
 * @brief  Check POR reset status
 * @retval POR reset status.
 */
__STATIC_INLINE uint32_t LL_RTC_IsActiveFlag_PORRST(void)
{
    return (LL_REG_RBIT(SN_ALWAYSON->POR_MISC, ALWAYSON_POR_MISC_PORRSTF) == ALWAYSON_POR_MISC_PORRSTF);
}

/**
 * @brief  clear POR reset status
 * @retval None.
 */
__STATIC_INLINE void LL_RTC_ClearFlag_PORRST(void)
{
    LL_REG_SBIT(SN_ALWAYSON->POR_MISC, ALWAYSON_POR_MISC_PORRSTF);
}

/**
 * @brief  set RTC current second
 * @param  sec current second
 * @retval None.
 */
__STATIC_INLINE void LL_RTC_Set_Second(uint8_t sec)
{
    LL_REG_CBIT(SN_SCU->RTC_TIME1, SCU_RTC_TIME1_SEC);
    LL_REG_SBIT(SN_SCU->RTC_TIME1, SCU_RTC_TIME1_SEC & (sec << SCU_RTC_TIME1_SEC_Pos));
}

/**
 * @brief  get RTC current second
 * @retval RTC current second.
 */
__STATIC_INLINE uint32_t LL_RTC_Get_Second(void)
{
    return (LL_REG_RBIT(SN_SCU->RTC_TIME1, SCU_RTC_TIME1_SEC) >> SCU_RTC_TIME1_SEC_Pos);
}

/**
 * @brief  set RTC current minute
 * @param  min current minute
 * @retval None.
 */
__STATIC_INLINE void LL_RTC_Set_Minute(uint8_t min)
{
    LL_REG_CBIT(SN_SCU->RTC_TIME1, SCU_RTC_TIME1_MIN);
    LL_REG_SBIT(SN_SCU->RTC_TIME1, SCU_RTC_TIME1_MIN & (min << SCU_RTC_TIME1_MIN_Pos));
}

/**
 * @brief  get RTC current minute
 * @retval RTC current minute.
 */
__STATIC_INLINE uint32_t LL_RTC_Get_Minute(void)
{
    return (LL_REG_RBIT(SN_SCU->RTC_TIME1, SCU_RTC_TIME1_MIN) >> SCU_RTC_TIME1_MIN_Pos);
}

/**
 * @brief  set RTC current hour
 * @param  hour current hour
 * @retval None.
 */
__STATIC_INLINE void LL_RTC_Set_Hour(uint8_t hour)
{
    LL_REG_CBIT(SN_SCU->RTC_TIME1, SCU_RTC_TIME1_HOUR);
    LL_REG_SBIT(SN_SCU->RTC_TIME1, SCU_RTC_TIME1_HOUR & (hour << SCU_RTC_TIME1_HOUR_Pos));
}

/**
 * @brief  get RTC current hour
 * @retval RTC current hour.
 */
__STATIC_INLINE uint32_t LL_RTC_Get_Hour(void)
{
    return (LL_REG_RBIT(SN_SCU->RTC_TIME1, SCU_RTC_TIME1_HOUR) >> SCU_RTC_TIME1_HOUR_Pos);
}

/**
 * @brief  set RTC current weekday
 * @param  weekday rtc_weekday current weekday
 * @retval None.
 */
__STATIC_INLINE void LL_RTC_Set_Weekday(uint8_t weekday)
{
    LL_REG_CBIT(SN_SCU->RTC_TIME1, SCU_RTC_TIME1_WEEKDAY);
    LL_REG_SBIT(SN_SCU->RTC_TIME1, SCU_RTC_TIME1_WEEKDAY & (weekday << SCU_RTC_TIME1_WEEKDAY_Pos));
}

/**
 * @brief  get RTC current weekday
 * @retval RTC current weekday.
 */
__STATIC_INLINE uint32_t LL_RTC_Get_Weekday(void)
{
    return (LL_REG_RBIT(SN_SCU->RTC_TIME1, SCU_RTC_TIME1_WEEKDAY) >> SCU_RTC_TIME1_WEEKDAY_Pos);
}

/**
 * @brief  set RTC current date
 * @param  date current date
 * @retval None.
 */
__STATIC_INLINE void LL_RTC_Set_Date(uint8_t date)
{
    LL_REG_CBIT(SN_SCU->RTC_TIME2, SCU_RTC_TIME2_DATE);
    LL_REG_SBIT(SN_SCU->RTC_TIME2, SCU_RTC_TIME2_DATE & (date << SCU_RTC_TIME2_DATE_Pos));
}

/**
 * @brief  get RTC current date
 * @retval RTC current date.
 */
__STATIC_INLINE uint32_t LL_RTC_Get_Date(void)
{
    return (LL_REG_RBIT(SN_SCU->RTC_TIME2, SCU_RTC_TIME2_DATE) >> SCU_RTC_TIME2_DATE_Pos);
}

/**
 * @brief  set RTC current month
 * @param  month current month
 * @retval None.
 */
__STATIC_INLINE void LL_RTC_Set_Month(uint8_t month)
{
    LL_REG_CBIT(SN_SCU->RTC_TIME2, SCU_RTC_TIME2_MONTH);
    LL_REG_SBIT(SN_SCU->RTC_TIME2, SCU_RTC_TIME2_MONTH & (month << SCU_RTC_TIME2_MONTH_Pos));
}

/**
 * @brief  get RTC current month
 * @retval RTC current month.
 */
__STATIC_INLINE uint32_t LL_RTC_Get_Month(void)
{
    return (LL_REG_RBIT(SN_SCU->RTC_TIME2, SCU_RTC_TIME2_MONTH) >> SCU_RTC_TIME2_MONTH_Pos);
}

/**
 * @brief  set RTC current year
 * @param  year current year
 * @retval None.
 */
__STATIC_INLINE void LL_RTC_Set_Year(uint8_t year)
{
    LL_REG_CBIT(SN_SCU->RTC_TIME2, SCU_RTC_TIME2_YEAR);
    LL_REG_SBIT(SN_SCU->RTC_TIME2, SCU_RTC_TIME2_YEAR & (year << SCU_RTC_TIME2_YEAR_Pos));
}

/**
 * @brief  get RTC current year
 * @retval RTC current year.
 */
__STATIC_INLINE uint32_t LL_RTC_Get_Year(void)
{
    return (LL_REG_RBIT(SN_SCU->RTC_TIME2, SCU_RTC_TIME2_YEAR) >> SCU_RTC_TIME2_YEAR_Pos);
}

/**
 * @brief  set RTC current century
 * @param  century current century
 * @retval None.
 */
__STATIC_INLINE void LL_RTC_Set_Century(uint8_t century)
{
    LL_REG_CBIT(SN_SCU->RTC_TIME2, SCU_RTC_TIME2_CENTURY);
    LL_REG_SBIT(SN_SCU->RTC_TIME2, SCU_RTC_TIME2_CENTURY & (century << SCU_RTC_TIME2_CENTURY_Pos));
}

/**
 * @brief  get RTC current century
 * @retval RTC current century.
 */
__STATIC_INLINE uint32_t LL_RTC_Get_Century(void)
{
    return (LL_REG_RBIT(SN_SCU->RTC_TIME2, SCU_RTC_TIME2_CENTURY) >> SCU_RTC_TIME2_CENTURY_Pos);
}

/**
 * @brief  set RTC Alarm second
 * @param  sec Alarm second
 * @retval None.
 */
__STATIC_INLINE void LL_RTC_SetAlarm_Second(uint8_t sec)
{
    LL_REG_CBIT(SN_SCU->RTC_ALM1, SCU_RTC_ALM1_SEC);
    LL_REG_SBIT(SN_SCU->RTC_ALM1, SCU_RTC_ALM1_SEC & (sec << SCU_RTC_ALM1_SEC_Pos));
}

/**
 * @brief  get RTC Alarm second
 * @retval RTC Alarm second.
 */
__STATIC_INLINE uint32_t LL_RTC_GetAlarm_Second(void)
{
    return (LL_REG_RBIT(SN_SCU->RTC_ALM1, SCU_RTC_ALM1_SEC) >> SCU_RTC_ALM1_SEC_Pos);
}

/**
 * @brief  set RTC Alarm minute
 * @param  min Alarm minute
 * @retval None.
 */
__STATIC_INLINE void LL_RTC_SetAlarm_Minute(uint8_t min)
{
    LL_REG_CBIT(SN_SCU->RTC_ALM1, SCU_RTC_ALM1_MIN);
    LL_REG_SBIT(SN_SCU->RTC_ALM1, SCU_RTC_ALM1_MIN & (min << SCU_RTC_ALM1_MIN_Pos));
}

/**
 * @brief  get RTC Alarm minute
 * @retval RTC Alarm minute.
 */
__STATIC_INLINE uint32_t LL_RTC_GetAlarm_Minute(void)
{
    return (LL_REG_RBIT(SN_SCU->RTC_ALM1, SCU_RTC_ALM1_MIN) >> SCU_RTC_ALM1_MIN_Pos);
}

/**
 * @brief  set RTC Alarm hour
 * @param  hour Alarm hour
 * @retval None.
 */
__STATIC_INLINE void LL_RTC_SetAlarm_Hour(uint8_t hour)
{
    LL_REG_CBIT(SN_SCU->RTC_ALM1, SCU_RTC_ALM1_HOUR);
    LL_REG_SBIT(SN_SCU->RTC_ALM1, SCU_RTC_ALM1_HOUR & (hour << SCU_RTC_ALM1_HOUR_Pos));
}

/**
 * @brief  get RTC Alarm hour
 * @retval RTC Alarm hour.
 */
__STATIC_INLINE uint32_t LL_RTC_GetAlarm_Hour(void)
{
    return (LL_REG_RBIT(SN_SCU->RTC_ALM1, SCU_RTC_ALM1_HOUR) >> SCU_RTC_ALM1_HOUR_Pos);
}

/**
 * @brief  set RTC Alarm weekday
 * @param  weekday rtc_weekday Alarm weekday
 * @retval None.
 */
__STATIC_INLINE void LL_RTC_SetAlarm_Weekday(uint8_t weekday)
{
    LL_REG_CBIT(SN_SCU->RTC_ALM1, SCU_RTC_ALM1_WEEKDAY);
    LL_REG_SBIT(SN_SCU->RTC_ALM1, SCU_RTC_ALM1_WEEKDAY & (weekday << SCU_RTC_ALM1_WEEKDAY_Pos));
}

/**
 * @brief  get RTC Alarm weekday
 * @retval RTC Alarm weekday.
 */
__STATIC_INLINE uint32_t LL_RTC_GetAlarm_Weekday(void)
{
    return (LL_REG_RBIT(SN_SCU->RTC_ALM1, SCU_RTC_ALM1_WEEKDAY) >> SCU_RTC_ALM1_WEEKDAY_Pos);
}

/**
 * @brief  set RTC alarm date
 * @param  date alarm date
 * @retval None.
 */
__STATIC_INLINE void LL_RTC_SetAlarm_Date(uint8_t date)
{
    LL_REG_CBIT(SN_SCU->RTC_ALM2, SCU_RTC_ALM2_DATE);
    LL_REG_SBIT(SN_SCU->RTC_ALM2, SCU_RTC_ALM2_DATE & (date << SCU_RTC_ALM2_DATE_Pos));
}

/**
 * @brief  get RTC alarm date
 * @retval RTC alarm date.
 */
__STATIC_INLINE uint32_t LL_RTC_GetAlarm_Date(void)
{
    return (LL_REG_RBIT(SN_SCU->RTC_ALM2, SCU_RTC_ALM2_DATE) >> SCU_RTC_ALM2_DATE_Pos);
}

/**
 * @brief  set RTC alarm month
 * @param  month alarm month
 * @retval None.
 */
__STATIC_INLINE void LL_RTC_SetAlarm_Month(uint8_t month)
{
    LL_REG_CBIT(SN_SCU->RTC_ALM2, SCU_RTC_ALM2_MONTH);
    LL_REG_SBIT(SN_SCU->RTC_ALM2, SCU_RTC_ALM2_MONTH & (month << SCU_RTC_ALM2_MONTH_Pos));
}

/**
 * @brief  get RTC alarm month
 * @retval RTC alarm month.
 */
__STATIC_INLINE uint32_t LL_RTC_GetAlarm_Month(void)
{
    return (LL_REG_RBIT(SN_SCU->RTC_ALM2, SCU_RTC_ALM2_MONTH) >> SCU_RTC_ALM2_MONTH_Pos);
}

/**
 * @brief  Enable for RTC
 * @retval None.
 */
__STATIC_INLINE void LL_RTC_Enable(void)
{
    LL_REG_SBIT(SN_SCU->RTC_CTRL, SCU_RTC_CTRL_RTC_EN);
}

/**
 * @brief  Disable for RTC
 * @retval None.
 */
__STATIC_INLINE void LL_RTC_Disable(void)
{
    LL_REG_CBIT(SN_SCU->RTC_CTRL, SCU_RTC_CTRL_RTC_EN);
}

/**
 * @brief  Determine if for RTC is enabled
 * @retval RTC enable state.
 */
__STATIC_INLINE uint32_t LL_RTC_IsEnabled(void)
{
    return (LL_REG_RBIT(SN_SCU->RTC_CTRL, SCU_RTC_CTRL_RTC_EN) == SCU_RTC_CTRL_RTC_EN);
}

/**
 * @brief  Enable for RTC alarm
 * @retval None.
 */
__STATIC_INLINE void LL_RTC_Enable_Alarm(void)
{
    LL_REG_SBIT(SN_SCU->RTC_CTRL, SCU_RTC_CTRL_RTC_ALARM_EN);
}

/**
 * @brief  Disable for RTC alarm
 * @retval None.
 */
__STATIC_INLINE void LL_RTC_Disable_Alarm(void)
{
    LL_REG_CBIT(SN_SCU->RTC_CTRL, SCU_RTC_CTRL_RTC_ALARM_EN);
}

/**
 * @brief  Determine if for RTC alarm is enabled
 * @retval RTC alarm enable state.
 */
__STATIC_INLINE uint32_t LL_RTC_IsEnabled_Alarm(void)
{
    return (LL_REG_RBIT(SN_SCU->RTC_CTRL, SCU_RTC_CTRL_RTC_ALARM_EN) == SCU_RTC_CTRL_RTC_ALARM_EN);
}

/**
 * @brief  Enable for RTC lock
 * @retval None.
 */
__STATIC_INLINE void LL_RTC_Enable_Lock(void)
{
    LL_REG_SBIT(SN_SCU->RTC_CTRL, SCU_RTC_CTRL_LOCK_EN);
}

/**
 * @brief  Disable for RTC lock
 * @retval None.
 */
__STATIC_INLINE void LL_RTC_Disable_Lock(void)
{
    LL_REG_CBIT(SN_SCU->RTC_CTRL, SCU_RTC_CTRL_LOCK_EN);
}

/**
 * @brief  Determine if for RTC lock is enabled
 * @retval RTC lock enable state.
 */
__STATIC_INLINE uint32_t LL_RTC_IsEnabled_Lock(void)
{
    return (LL_REG_RBIT(SN_SCU->RTC_CTRL, SCU_RTC_CTRL_LOCK_EN) == SCU_RTC_CTRL_LOCK_EN);
}

/**
 * @brief  Select Periodic interrupt output signal
 * @param  mode rtc_periodic_const
 * @retval None.
 */
__STATIC_INLINE void LL_RTC_SelectPeriodic(uint8_t mode)
{
    LL_REG_CBIT(SN_SCU->RTC_CTRL, SCU_RTC_CTRL_PERINT_SEL);
    LL_REG_SBIT(SN_SCU->RTC_CTRL, SCU_RTC_CTRL_PERINT_SEL & mode);
}

/**
 * @brief  Enable an event in each second for RTC
 * @retval None.
 */
__STATIC_INLINE void LL_RTC_Enable_Secout(void)
{
    LL_REG_SBIT(SN_SCU->RTC_CTRL, SCU_RTC_CTRL_SECOUT_EN);
}

/**
 * @brief  Disable an event in each second for RTC
 * @retval None.
 */
__STATIC_INLINE void LL_RTC_Disable_Secout(void)
{
    LL_REG_CBIT(SN_SCU->RTC_CTRL, SCU_RTC_CTRL_SECOUT_EN);
}

/**
 * @brief  Determine if an event in each second for RTC is enabled
 * @retval State of enable.
 */
__STATIC_INLINE uint32_t LL_RTC_IsEnabled_Secout(void)
{
    return (LL_REG_RBIT(SN_SCU->RTC_CTRL, SCU_RTC_CTRL_SECOUT_EN) == SCU_RTC_CTRL_SECOUT_EN);
}

/**
 * @brief  Check RTC enable status
 * @retval State of enable.
 */
__STATIC_INLINE uint32_t LL_RTC_IsActiveStatus_RTC(void)
{
    return (LL_REG_RBIT(SN_SCU->RTC_CTRL, SCU_RTC_CTRL_RTCEN_STS) == SCU_RTC_CTRL_RTCEN_STS);
}

/**
 * @brief  Check RTC Alarm enable status
 * @retval State of enable.
 */
__STATIC_INLINE uint32_t LL_RTC_IsActiveStatus_Alarm(void)
{
    return (LL_REG_RBIT(SN_SCU->RTC_CTRL, SCU_RTC_CTRL_RTC_ALMEN_STS) == SCU_RTC_CTRL_RTC_ALMEN_STS);
}

/**
 * @brief  Check Periodic wakeup timer enable status
 * @retval State of enable.
 */
__STATIC_INLINE uint32_t LL_RTC_IsActiveStatus_Periodic(void)
{
    return (LL_REG_RBIT(SN_SCU->RTC_CTRL, SCU_RTC_CTRL_PWUTEN_STS) == SCU_RTC_CTRL_PWUTEN_STS);
}

/**
 * @brief  Check RTC ready
 * @retval State of ready.
 */
__STATIC_INLINE uint32_t LL_RTC_IsActiveFlag_RTC_Ready(void)
{
    return (LL_REG_RBIT(SN_SCU->RTC_CTRL, SCU_RTC_CTRL_RTC_REGRDY) == SCU_RTC_CTRL_RTC_REGRDY);
}

/**
 * @brief  set RTC tick dividing factor
 * @param  trim_p RTC tick dividing factor
 * @retval None.
 */
__STATIC_INLINE void LL_RTC_Set_TRIM_P(uint32_t trim_p)
{
    LL_REG_CBIT(SN_SCU->RTC_TRIM, SCU_RTC_TRIM_TRIM_P);
    LL_REG_SBIT(SN_SCU->RTC_TRIM, SCU_RTC_TRIM_TRIM_P & (trim_p << SCU_RTC_TRIM_TRIM_P_Pos));
}

/**
 * @brief  get RTC tick dividing factor
 * @retval RTC tick dividing factor.
 */
__STATIC_INLINE uint32_t LL_RTC_Get_TRIM_P(void)
{
    return (LL_REG_RBIT(SN_SCU->RTC_TRIM, SCU_RTC_TRIM_TRIM_P) >> SCU_RTC_TRIM_TRIM_P_Pos);
}

/**
 * @brief  set RTC tick fractional factor
 * @param  trim_f RTC tick fractional factor
 * @retval None.
 */
__STATIC_INLINE void LL_RTC_Set_TRIM_F(uint32_t trim_f)
{
    LL_REG_CBIT(SN_SCU->RTC_TRIM, SCU_RTC_TRIM_TRIM_F);
    LL_REG_SBIT(SN_SCU->RTC_TRIM, SCU_RTC_TRIM_TRIM_F & (trim_f << SCU_RTC_TRIM_TRIM_F_Pos));
}

/**
 * @brief  get RTC tick fractional factor
 * @retval RTC tick fractional factor.
 */
__STATIC_INLINE uint32_t LL_RTC_Get_TRIM_F(void)
{
    return (LL_REG_RBIT(SN_SCU->RTC_TRIM, SCU_RTC_TRIM_TRIM_F) >> SCU_RTC_TRIM_TRIM_F_Pos);
}

/**
 * @brief  set BCD current second for RTC
 * @param  sec BCD current second for RTC
 * @retval None.
 */
__STATIC_INLINE void LL_RTC_Set_BCD_Second(uint32_t sec)
{
    LL_REG_CBIT(SN_SCU->BCD_RTC_TIME1, SCU_BCD_RTC_TIME1_SEC);
    LL_REG_CBIT(SN_SCU->BCD_RTC_TIME1, SCU_BCD_RTC_TIME1_SEC_DEC);
    LL_REG_SBIT(SN_SCU->BCD_RTC_TIME1, SCU_BCD_RTC_TIME1_SEC & ((sec & 0xF) << SCU_BCD_RTC_TIME1_SEC_Pos));
    LL_REG_SBIT(SN_SCU->BCD_RTC_TIME1, SCU_BCD_RTC_TIME1_SEC_DEC & ((sec & 0xF0) << SCU_BCD_RTC_TIME1_SEC_Pos));
}

/**
 * @brief  get BCD current second for RTC
 * @retval BCD current second for RTC.
 */
__STATIC_INLINE uint32_t LL_RTC_Get_BCD_Second(void)
{
    return ((LL_REG_RBIT(SN_SCU->BCD_RTC_TIME1, SCU_BCD_RTC_TIME1_SEC) >> SCU_BCD_RTC_TIME1_SEC_Pos) | (LL_REG_RBIT(SN_SCU->BCD_RTC_TIME1, SCU_BCD_RTC_TIME1_SEC_DEC) >> SCU_BCD_RTC_TIME1_SEC_Pos));
}

/**
 * @brief  set BCD current minute for RTC
 * @param  min BCD current minute for RTC
 * @retval None.
 */
__STATIC_INLINE void LL_RTC_Set_BCD_Minute(uint32_t min)
{
    LL_REG_CBIT(SN_SCU->BCD_RTC_TIME1, SCU_BCD_RTC_TIME1_MIN);
    LL_REG_CBIT(SN_SCU->BCD_RTC_TIME1, SCU_BCD_RTC_TIME1_MIN_DEC);
    LL_REG_SBIT(SN_SCU->BCD_RTC_TIME1, SCU_BCD_RTC_TIME1_MIN & ((min & 0xF) << SCU_BCD_RTC_TIME1_MIN_Pos));
    LL_REG_SBIT(SN_SCU->BCD_RTC_TIME1, SCU_BCD_RTC_TIME1_MIN_DEC & ((min & 0xF0) << SCU_BCD_RTC_TIME1_MIN_Pos));
}

/**
 * @brief  get BCD current minute for RTC
 * @retval BCD current minute for RTC.
 */
__STATIC_INLINE uint32_t LL_RTC_Get_BCD_Minute(void)
{
    return ((LL_REG_RBIT(SN_SCU->BCD_RTC_TIME1, SCU_BCD_RTC_TIME1_MIN) >> SCU_BCD_RTC_TIME1_MIN_Pos) | (LL_REG_RBIT(SN_SCU->BCD_RTC_TIME1, SCU_BCD_RTC_TIME1_MIN_DEC) >> SCU_BCD_RTC_TIME1_MIN_Pos));
}

/**
 * @brief  set BCD current hour for RTC
 * @param  hour BCD current hour for RTC
 * @retval None.
 */
__STATIC_INLINE void LL_RTC_Set_BCD_Hour(uint32_t hour)
{
    LL_REG_CBIT(SN_SCU->BCD_RTC_TIME1, SCU_BCD_RTC_TIME1_HOUR);
    LL_REG_CBIT(SN_SCU->BCD_RTC_TIME1, SCU_BCD_RTC_TIME1_HOUR_DEC);
    LL_REG_SBIT(SN_SCU->BCD_RTC_TIME1, SCU_BCD_RTC_TIME1_HOUR & ((hour & 0xF) << SCU_BCD_RTC_TIME1_HOUR_Pos));
    LL_REG_SBIT(SN_SCU->BCD_RTC_TIME1, SCU_BCD_RTC_TIME1_HOUR_DEC & ((hour & 0xF0) << SCU_BCD_RTC_TIME1_HOUR_Pos));
}

/**
 * @brief  get BCD current hour for RTC
 * @retval BCD current hour for RTC.
 */
__STATIC_INLINE uint32_t LL_RTC_Get_BCD_Hour(void)
{
    return ((LL_REG_RBIT(SN_SCU->BCD_RTC_TIME1, SCU_BCD_RTC_TIME1_HOUR) >> SCU_BCD_RTC_TIME1_HOUR_Pos) | (LL_REG_RBIT(SN_SCU->BCD_RTC_TIME1, SCU_BCD_RTC_TIME1_HOUR_DEC) >> SCU_BCD_RTC_TIME1_HOUR_Pos));
}

/**
 * @brief  set BCD current weekday for RTC
 * @param  weekday rtc_weekday BCD current weekday for RTC
 * @retval None.
 */
__STATIC_INLINE void LL_RTC_Set_BCD_Weekday(uint8_t weekday)
{
    LL_REG_CBIT(SN_SCU->BCD_RTC_TIME1, SCU_BCD_RTC_TIME1_WEEKDAY);
    LL_REG_SBIT(SN_SCU->BCD_RTC_TIME1, SCU_BCD_RTC_TIME1_WEEKDAY & (weekday << SCU_BCD_RTC_TIME1_WEEKDAY_Pos));
}

/**
 * @brief  get BCD current weekday for RTC
 * @retval BCD current weekday for RTC.
 */
__STATIC_INLINE uint32_t LL_RTC_Get_BCD_Weekday(void)
{
    return (LL_REG_RBIT(SN_SCU->BCD_RTC_TIME1, SCU_BCD_RTC_TIME1_WEEKDAY) >> SCU_BCD_RTC_TIME1_WEEKDAY_Pos);
}

/**
 * @brief  set BCD current date for RTC
 * @param  date BCD current date for RTC
 * @retval None.
 */
__STATIC_INLINE void LL_RTC_Set_BCD_Date(uint32_t date)
{
    LL_REG_CBIT(SN_SCU->BCD_RTC_TIME2, SCU_BCD_RTC_TIME2_DATE);
    LL_REG_CBIT(SN_SCU->BCD_RTC_TIME2, SCU_BCD_RTC_TIME2_DATE_DEC);
    LL_REG_SBIT(SN_SCU->BCD_RTC_TIME2, SCU_BCD_RTC_TIME2_DATE & ((date & 0xF) << SCU_BCD_RTC_TIME2_DATE_Pos));
    LL_REG_SBIT(SN_SCU->BCD_RTC_TIME2, SCU_BCD_RTC_TIME2_DATE_DEC & ((date & 0xF0) << SCU_BCD_RTC_TIME2_DATE_Pos));
}

/**
 * @brief  get BCD current date for RTC
 * @retval BCD current date for RTC.
 */
__STATIC_INLINE uint32_t LL_RTC_Get_BCD_Date(void)
{
    return ((LL_REG_RBIT(SN_SCU->BCD_RTC_TIME2, SCU_BCD_RTC_TIME2_DATE) >> SCU_BCD_RTC_TIME2_DATE_Pos) | (LL_REG_RBIT(SN_SCU->BCD_RTC_TIME2, SCU_BCD_RTC_TIME2_DATE_DEC) >> SCU_BCD_RTC_TIME2_DATE_Pos));
}

/**
 * @brief  set BCD current month for RTC
 * @param  month BCD current month for RTC
 * @retval None.
 */
__STATIC_INLINE void LL_RTC_Set_BCD_Month(uint32_t month)
{
    LL_REG_CBIT(SN_SCU->BCD_RTC_TIME2, SCU_BCD_RTC_TIME2_MONTH);
    LL_REG_CBIT(SN_SCU->BCD_RTC_TIME2, SCU_BCD_RTC_TIME2_MONTH_DEC);
    LL_REG_SBIT(SN_SCU->BCD_RTC_TIME2, SCU_BCD_RTC_TIME2_MONTH & ((month & 0xF) << SCU_BCD_RTC_TIME2_MONTH_Pos));
    LL_REG_SBIT(SN_SCU->BCD_RTC_TIME2, SCU_BCD_RTC_TIME2_MONTH_DEC & ((month & 0xF0) << SCU_BCD_RTC_TIME2_MONTH_Pos));
}

/**
 * @brief  get BCD current month for RTC
 * @retval BCD current month for RTC.
 */
__STATIC_INLINE uint32_t LL_RTC_Get_BCD_Month(void)
{
    return ((LL_REG_RBIT(SN_SCU->BCD_RTC_TIME2, SCU_BCD_RTC_TIME2_MONTH) >> SCU_BCD_RTC_TIME2_MONTH_Pos) | (LL_REG_RBIT(SN_SCU->BCD_RTC_TIME2, SCU_BCD_RTC_TIME2_MONTH_DEC) >> SCU_BCD_RTC_TIME2_MONTH_Pos));
}

/**
 * @brief  set BCD current year for RTC
 * @param  year BCD current year for RTC
 * @retval None.
 */
__STATIC_INLINE void LL_RTC_Set_BCD_Year(uint32_t year)
{
    LL_REG_CBIT(SN_SCU->BCD_RTC_TIME2, SCU_BCD_RTC_TIME2_YEAR);
    LL_REG_CBIT(SN_SCU->BCD_RTC_TIME2, SCU_BCD_RTC_TIME2_YEAR_DEC);
    LL_REG_SBIT(SN_SCU->BCD_RTC_TIME2, SCU_BCD_RTC_TIME2_YEAR & ((year & 0xF) << SCU_BCD_RTC_TIME2_YEAR_Pos));
    LL_REG_SBIT(SN_SCU->BCD_RTC_TIME2, SCU_BCD_RTC_TIME2_YEAR_DEC & ((year & 0xF0) << SCU_BCD_RTC_TIME2_YEAR_Pos));
}

/**
 * @brief  get BCD current year for RTC
 * @retval BCD current year for RTC.
 */
__STATIC_INLINE uint32_t LL_RTC_Get_BCD_Year(void)
{
    return ((LL_REG_RBIT(SN_SCU->BCD_RTC_TIME2, SCU_BCD_RTC_TIME2_YEAR) >> SCU_BCD_RTC_TIME2_YEAR_Pos) | (LL_REG_RBIT(SN_SCU->BCD_RTC_TIME2, SCU_BCD_RTC_TIME2_YEAR_DEC) >> SCU_BCD_RTC_TIME2_YEAR_Pos));
}

/**
 * @brief  set BCD current century for RTC
 * @param  century BCD current century for RTC
 * @retval None.
 */
__STATIC_INLINE void LL_RTC_Set_BCD_Century(uint32_t century)
{
    LL_REG_CBIT(SN_SCU->BCD_RTC_TIME2, SCU_BCD_RTC_TIME2_CENTURY);
    LL_REG_CBIT(SN_SCU->BCD_RTC_TIME2, SCU_BCD_RTC_TIME2_CENTURY_DEC);
    LL_REG_SBIT(SN_SCU->BCD_RTC_TIME2, SCU_BCD_RTC_TIME2_CENTURY & ((century & 0xF) << SCU_BCD_RTC_TIME2_CENTURY_Pos));
    LL_REG_SBIT(SN_SCU->BCD_RTC_TIME2, SCU_BCD_RTC_TIME2_CENTURY_DEC & ((century & 0xF0) << SCU_BCD_RTC_TIME2_CENTURY_Pos));
}

/**
 * @brief  get BCD current century for RTC
 * @retval BCD current century for RTC.
 */
__STATIC_INLINE uint32_t LL_RTC_Get_BCD_Century(void)
{
    return ((LL_REG_RBIT(SN_SCU->BCD_RTC_TIME2, SCU_BCD_RTC_TIME2_CENTURY) >> SCU_BCD_RTC_TIME2_CENTURY_Pos) | (LL_REG_RBIT(SN_SCU->BCD_RTC_TIME2, SCU_BCD_RTC_TIME2_CENTURY_DEC) >> SCU_BCD_RTC_TIME2_CENTURY_Pos));
}

/**
 * @brief  set BCD alarm second for RTC
 * @param  sec BCD alarm second for RTC
 * @retval None.
 */
__STATIC_INLINE void LL_RTC_SetAlarm_BCD_Second(uint32_t sec)
{
    LL_REG_CBIT(SN_SCU->BCD_RTC_ALM1, SCU_BCD_RTC_ALM1_SEC);
    LL_REG_CBIT(SN_SCU->BCD_RTC_ALM1, SCU_BCD_RTC_ALM1_SEC_DEC);
    LL_REG_SBIT(SN_SCU->BCD_RTC_ALM1, SCU_BCD_RTC_ALM1_SEC & ((sec & 0xF) << SCU_BCD_RTC_ALM1_SEC_Pos));
    LL_REG_SBIT(SN_SCU->BCD_RTC_ALM1, SCU_BCD_RTC_ALM1_SEC_DEC & ((sec & 0xF0) << SCU_BCD_RTC_ALM1_SEC_Pos));
}

/**
 * @brief  get BCD alarm second for RTC
 * @retval BCD alarm second for RTC.
 */
__STATIC_INLINE uint32_t LL_RTC_GetAlarm_BCD_Second(void)
{
    return ((LL_REG_RBIT(SN_SCU->BCD_RTC_ALM1, SCU_BCD_RTC_ALM1_SEC) >> SCU_BCD_RTC_ALM1_SEC_Pos) | (LL_REG_RBIT(SN_SCU->BCD_RTC_ALM1, SCU_BCD_RTC_ALM1_SEC_DEC) >> SCU_BCD_RTC_ALM1_SEC_Pos));
}

/**
 * @brief  set BCD alarm minute for RTC
 * @param  min BCD alarm minute for RTC
 * @retval None.
 */
__STATIC_INLINE void LL_RTC_SetAlarm_BCD_Minute(uint32_t min)
{
    LL_REG_CBIT(SN_SCU->BCD_RTC_ALM1, SCU_BCD_RTC_ALM1_MIN);
    LL_REG_CBIT(SN_SCU->BCD_RTC_ALM1, SCU_BCD_RTC_ALM1_MIN_DEC);
    LL_REG_SBIT(SN_SCU->BCD_RTC_ALM1, SCU_BCD_RTC_ALM1_MIN & ((min & 0xF) << SCU_BCD_RTC_ALM1_MIN_Pos));
    LL_REG_SBIT(SN_SCU->BCD_RTC_ALM1, SCU_BCD_RTC_ALM1_MIN_DEC & ((min & 0xF0) << SCU_BCD_RTC_ALM1_MIN_Pos));
}

/**
 * @brief  get BCD alarm minute for RTC
 * @retval BCD alarm minute for RTC.
 */
__STATIC_INLINE uint32_t LL_RTC_GetAlarm_BCD_Minute(void)
{
    return ((LL_REG_RBIT(SN_SCU->BCD_RTC_ALM1, SCU_BCD_RTC_ALM1_MIN) >> SCU_BCD_RTC_ALM1_MIN_Pos) | (LL_REG_RBIT(SN_SCU->BCD_RTC_ALM1, SCU_BCD_RTC_ALM1_MIN_DEC) >> SCU_BCD_RTC_ALM1_MIN_Pos));
}

/**
 * @brief  set BCD alarm hour for RTC
 * @param  hour BCD alarm hour for RTC
 * @retval None.
 */
__STATIC_INLINE void LL_RTC_SetAlarm_BCD_Hour(uint32_t hour)
{
    LL_REG_CBIT(SN_SCU->BCD_RTC_ALM1, SCU_BCD_RTC_ALM1_HOUR);
    LL_REG_CBIT(SN_SCU->BCD_RTC_ALM1, SCU_BCD_RTC_ALM1_HOUR_DEC);
    LL_REG_SBIT(SN_SCU->BCD_RTC_ALM1, SCU_BCD_RTC_ALM1_HOUR & ((hour & 0xF) << SCU_BCD_RTC_ALM1_HOUR_Pos));
    LL_REG_SBIT(SN_SCU->BCD_RTC_ALM1, SCU_BCD_RTC_ALM1_HOUR_DEC & ((hour & 0xF0) << SCU_BCD_RTC_ALM1_HOUR_Pos));
}

/**
 * @brief  get BCD alarm hour for RTC
 * @retval BCD alarm hour for RTC.
 */
__STATIC_INLINE uint32_t LL_RTC_GetAlarm_BCD_Hour(void)
{
    return ((LL_REG_RBIT(SN_SCU->BCD_RTC_ALM1, SCU_BCD_RTC_ALM1_HOUR) >> SCU_BCD_RTC_ALM1_HOUR_Pos) | (LL_REG_RBIT(SN_SCU->BCD_RTC_ALM1, SCU_BCD_RTC_ALM1_HOUR_DEC) >> SCU_BCD_RTC_ALM1_HOUR_Pos));
}

/**
 * @brief  set BCD alarm weekday for RTC
 * @param  weekday rtc_weekday BCD alarm weekday for RTC
 * @retval None.
 */
__STATIC_INLINE void LL_RTC_SetAlarm_BCD_Weekday(uint8_t weekday)
{
    LL_REG_CBIT(SN_SCU->BCD_RTC_ALM1, SCU_BCD_RTC_ALM1_WEEKDAY);
    LL_REG_SBIT(SN_SCU->BCD_RTC_ALM1, SCU_BCD_RTC_ALM1_WEEKDAY & (weekday << SCU_BCD_RTC_ALM1_WEEKDAY_Pos));
}

/**
 * @brief  get BCD alarm weekday for RTC
 * @retval BCD alarm weekday for RTC.
 */
__STATIC_INLINE uint32_t LL_RTC_GetAlarm_BCD_Weekday(void)
{
    return (LL_REG_RBIT(SN_SCU->BCD_RTC_ALM1, SCU_BCD_RTC_ALM1_WEEKDAY) >> SCU_BCD_RTC_ALM1_WEEKDAY_Pos);
}

/**
 * @brief  set BCD alarm date for RTC
 * @param  date BCD alarm date for RTC
 * @retval None.
 */
__STATIC_INLINE void LL_RTC_SetAlarm_BCD_Date(uint32_t date)
{
    LL_REG_CBIT(SN_SCU->BCD_RTC_ALM2, SCU_BCD_RTC_ALM2_DATE);
    LL_REG_CBIT(SN_SCU->BCD_RTC_ALM2, SCU_BCD_RTC_ALM2_DATE_DEC);
    LL_REG_SBIT(SN_SCU->BCD_RTC_ALM2, SCU_BCD_RTC_ALM2_DATE & ((date & 0xF) << SCU_BCD_RTC_ALM2_DATE_Pos));
    LL_REG_SBIT(SN_SCU->BCD_RTC_ALM2, SCU_BCD_RTC_ALM2_DATE_DEC & ((date & 0xF0) << SCU_BCD_RTC_ALM2_DATE_Pos));
}

/**
 * @brief  get BCD alarm date for RTC
 * @retval BCD alarm date for RTC.
 */
__STATIC_INLINE uint32_t LL_RTC_GetAlarm_BCD_Date(void)
{
    return ((LL_REG_RBIT(SN_SCU->BCD_RTC_ALM2, SCU_BCD_RTC_ALM2_DATE) >> SCU_BCD_RTC_ALM2_DATE_Pos) | (LL_REG_RBIT(SN_SCU->BCD_RTC_ALM2, SCU_BCD_RTC_ALM2_DATE_DEC) >> SCU_BCD_RTC_ALM2_DATE_Pos));
}

/**
 * @brief  set BCD alarm month for RTC
 * @param  month BCD alarm month for RTC
 * @retval None.
 */
__STATIC_INLINE void LL_RTC_SetAlarm_BCD_Month(uint32_t month)
{
    LL_REG_CBIT(SN_SCU->BCD_RTC_ALM2, SCU_BCD_RTC_ALM2_MONTH);
    LL_REG_CBIT(SN_SCU->BCD_RTC_ALM2, SCU_BCD_RTC_ALM2_MONTH_DEC);
    LL_REG_SBIT(SN_SCU->BCD_RTC_ALM2, SCU_BCD_RTC_ALM2_MONTH & ((month & 0xF) << SCU_BCD_RTC_ALM2_MONTH_Pos));
    LL_REG_SBIT(SN_SCU->BCD_RTC_ALM2, SCU_BCD_RTC_ALM2_MONTH_DEC & ((month & 0xF0) << SCU_BCD_RTC_ALM2_MONTH_Pos));
}

/**
 * @brief  get BCD alarm month for RTC
 * @retval BCD alarm month for RTC.
 */
__STATIC_INLINE uint32_t LL_RTC_GetAlarm_BCD_Month(void)
{
    return ((LL_REG_RBIT(SN_SCU->BCD_RTC_ALM2, SCU_BCD_RTC_ALM2_MONTH) >> SCU_BCD_RTC_ALM2_MONTH_Pos) | (LL_REG_RBIT(SN_SCU->BCD_RTC_ALM2, SCU_BCD_RTC_ALM2_MONTH_DEC) >> SCU_BCD_RTC_ALM2_MONTH_Pos));
}

/**
 * @brief  get RTC Chip ID
 * @retval RTC Chip ID.
 */
__STATIC_INLINE uint32_t LL_RTC_Get_ChipId(void)
{
    return (LL_REG_READ(SN_SCU->CHIPID));
}

/**
 * @brief  get RTC Version
 * @retval RTC Version.
 */
__STATIC_INLINE uint32_t LL_RTC_Get_Version(void)
{
    return (LL_REG_READ(SN_SCU->VERSION));
}

/**
 * @brief  Check GPIO HOLD flag
 * @retval GPIO HOLD flag.
 */
__STATIC_INLINE uint32_t LL_RTC_IsActiveFlag_GPIO_HOLD(void)
{
    return (LL_REG_RBIT(SN_SCU->BTUP_STS, SCU_BTUP_STS_GPIO_HOLD) == SCU_BTUP_STS_GPIO_HOLD);
}

/**
 * @brief  clear GPIO HOLD flag
 * @retval None.
 */
__STATIC_INLINE void LL_RTC_ClearFlag_GPIO_HOLD(void)
{
    LL_REG_SBIT(SN_SCU->BTUP_STS, SCU_BTUP_STS_GPIO_HOLD);
}

/**
 * @brief  Check External reset flag
 * @retval External reset flag.
 */
__STATIC_INLINE uint32_t LL_RTC_IsActiveFlag_EXTRSTF(void)
{
    return (LL_REG_RBIT(SN_SCU->BTUP_STS, SCU_BTUP_STS_EXTRSTF) == SCU_BTUP_STS_EXTRSTF);
}

/**
 * @brief  clear External reset flag
 * @retval None.
 */
__STATIC_INLINE void LL_RTC_ClearFlag_EXTRSTF(void)
{
    LL_REG_SBIT(SN_SCU->BTUP_STS, SCU_BTUP_STS_EXTRSTF);
}

/**
 * @brief  Check Deep power-down wakeup flag
 * @retval Deep power-down wakeup flag.
 */
__STATIC_INLINE uint32_t LL_RTC_IsActiveFlag_DPDWKF(void)
{
    return (LL_REG_RBIT(SN_SCU->BTUP_STS, SCU_BTUP_STS_DPDWKF) == SCU_BTUP_STS_DPDWKF);
}

/**
 * @brief  clear Deep power-down wakeup flag
 * @retval None.
 */
__STATIC_INLINE void LL_RTC_ClearFlag_DPDWKF(void)
{
    LL_REG_SBIT(SN_SCU->BTUP_STS, SCU_BTUP_STS_DPDWKF);
}

/**
 * @brief  Check Software reset flag
 * @retval Software reset flag.
 */
__STATIC_INLINE uint32_t LL_RTC_IsActiveFlag_SWRSTF(void)
{
    return (LL_REG_RBIT(SN_SCU->BTUP_STS, SCU_BTUP_STS_SWRSTF) == SCU_BTUP_STS_SWRSTF);
}

/**
 * @brief  clear Software reset flag
 * @retval None.
 */
__STATIC_INLINE void LL_RTC_ClearFlag_SWRSTF(void)
{
    LL_REG_SBIT(SN_SCU->BTUP_STS, SCU_BTUP_STS_SWRSTF);
}

/**
 * @brief  Check The RTC alarm wakeup flag
 * @retval The RTC alarm wakeup flag.
 */
__STATIC_INLINE uint32_t LL_RTC_IsActiveFlag_RTCWKF(void)
{
    return (LL_REG_RBIT(SN_SCU->BTUP_STS, SCU_BTUP_STS_RTCWKF) == SCU_BTUP_STS_RTCWKF);
}

/**
 * @brief  clear The RTC alarm wakeup flag
 * @retval None.
 */
__STATIC_INLINE void LL_RTC_ClearFlag_RTCWKF(void)
{
    LL_REG_SBIT(SN_SCU->BTUP_STS, SCU_BTUP_STS_RTCWKF);
}

/**
 * @brief  Check GPIO0 interrupt wakeup flag
 * @retval GPIO0 interrupt wakeup flag.
 */
__STATIC_INLINE uint32_t LL_RTC_IsActiveFlag_P0WKF(void)
{
    return (LL_REG_RBIT(SN_SCU->BTUP_STS, SCU_BTUP_STS_P0WKF) == SCU_BTUP_STS_P0WKF);
}

/**
 * @brief  clear GPIO0 interrupt wakeup flag
 * @retval None.
 */
__STATIC_INLINE void LL_RTC_ClearFlag_P0WKF(void)
{
    LL_REG_SBIT(SN_SCU->BTUP_STS, SCU_BTUP_STS_P0WKF);
}

/**
 * @brief  Check GPIO1 interrupt wakeup flag
 * @retval GPIO1 interrupt wakeup flag.
 */
__STATIC_INLINE uint32_t LL_RTC_IsActiveFlag_P1WKF(void)
{
    return (LL_REG_RBIT(SN_SCU->BTUP_STS, SCU_BTUP_STS_P1WKF) == SCU_BTUP_STS_P1WKF);
}

/**
 * @brief  clear GPIO1 interrupt wakeup flag
 * @retval None.
 */
__STATIC_INLINE void LL_RTC_ClearFlag_P1WKF(void)
{
    LL_REG_SBIT(SN_SCU->BTUP_STS, SCU_BTUP_STS_P1WKF);
}

/**
 * @brief  Check GPIO2 interrupt wakeup flag
 * @retval GPIO2 interrupt wakeup flag.
 */
__STATIC_INLINE uint32_t LL_RTC_IsActiveFlag_P2WKF(void)
{
    return (LL_REG_RBIT(SN_SCU->BTUP_STS, SCU_BTUP_STS_P2WKF) == SCU_BTUP_STS_P2WKF);
}

/**
 * @brief  clear GPIO2 interrupt wakeup flag
 * @retval None.
 */
__STATIC_INLINE void LL_RTC_ClearFlag_P2WKF(void)
{
    LL_REG_SBIT(SN_SCU->BTUP_STS, SCU_BTUP_STS_P2WKF);
}

/**
 * @brief  Check GPIO3 interrupt wakeup flag
 * @retval GPIO3 interrupt wakeup flag.
 */
__STATIC_INLINE uint32_t LL_RTC_IsActiveFlag_P3WKF(void)
{
    return (LL_REG_RBIT(SN_SCU->BTUP_STS, SCU_BTUP_STS_P3WKF) == SCU_BTUP_STS_P3WKF);
}

/**
 * @brief  clear GPIO3 interrupt wakeup flag
 * @retval None.
 */
__STATIC_INLINE void LL_RTC_ClearFlag_P3WKF(void)
{
    LL_REG_SBIT(SN_SCU->BTUP_STS, SCU_BTUP_STS_P3WKF);
}

/**
 * @brief  set Backup data
 * @param  BKPID  \ref rtc_backup
 * @param  data
 * @retval None.
 */
__STATIC_INLINE void LL_RTC_Set_BKPDATA(uint32_t BKPID, uint32_t data)
{
    LL_REG_WRITE(*(__IO uint32_t *)(((uint32_t)&(SN_ALWAYSON->BKPREG0)) + (4 * BKPID)), data);
}

/**
 * @brief  get Backup data
 * @param  BKPID  \ref rtc_backup
 * @retval Backup data.
 */
__STATIC_INLINE uint32_t LL_RTC_Get_BKPDATA(uint32_t BKPID)
{
    return (*(__IO uint32_t *)(((uint32_t)&(SN_ALWAYSON->BKPREG0)) + (4 * BKPID)));
}

#if defined(USE_FULL_LL_DRIVER)
LL_Status_T LL_RTC_Init(LL_RTC_Init_T *rtc_init);
LL_Status_T LL_RTC_DeInit(LL_RTC_Init_T *rtc_init);
void        LL_RTC_StructInit(LL_RTC_Init_T *rtc_init);
LL_Status_T LL_RTC_TIME_Init(LL_RTC_Time_T *rtc_time, uint32_t format);
void        LL_RTC_TIME_StructInit(LL_RTC_Time_T *rtc_time);
LL_Status_T LL_RTC_DATE_Init(LL_RTC_Date_T *rtc_date, uint32_t format);
void        LL_RTC_DATE_StructInit(LL_RTC_Date_T *rtc_date);
LL_Status_T LL_RTC_ALARM_Init(LL_RTC_Alarm_T *rtc_alarm, uint32_t format);
void        LL_RTC_ALARM_StructInit(LL_RTC_Alarm_T *rtc_alarm);
#endif /* USE_FULL_LL_DRIVER */

#ifdef __cplusplus
}
#endif

#endif
