/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _HAL_RTC_H_
#define _HAL_RTC_H_

/* Includes ------------------------------------------------------------------*/
#include "sn34f78x_hal_def.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported types ------------------------------------------------------------*/

/**
 * @brief  RTC Configuration Structure definition
 * \ingroup rtc_struct_define
 */
typedef struct
{
    uint32_t clk_source; /**< Specifies the RTC Clock Source.
                            This parameter can be a value of @ref rtc_clk_source */

} RTC_Init_T;

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
} RTC_Time_T;

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
} RTC_Date_T;

/**
 * @brief  RTC Alarm structure definition
 * @ingroup rtc_struct_define
 */
typedef struct
{
    RTC_Time_T alarm_time;             /**< Specifies the RTC Alarm Time members                                                             */
    uint32_t   alarm_date_weekday_sel; /**< Specifies the RTC Alarm is on Date or WeekDa
                                            This parameter can be a value of @ref rtc_alarm_selection                                        */
    uint32_t alarm_date_weekday;       /**< Specifies the RTC Alarm Date/WeekDa
                                            If the Alarm Date is selected, this parameter must be set to a value in the 1-31 rang
                                            If the Alarm WeekDay is selected, this parameter can be a value of @ref rtc_weekday              */
} RTC_Alarm_T;

/**
 * @brief  RTC Handle Structure definition
 * @ingroup rtc_struct_define
 */
#if (configUSE_RTC_REGISTER_CALLBACKS == 1)
typedef struct __RTC_Handle_T
#else
typedef struct
#endif /*configUSE_RTC_REGISTER_CALLBACKS*/
{
    RTC_Init_T       init;         /**< RTC required parameters      */
    HAL_MUTEX        mutex;        /**< RTC locking object           */
    __IO HAL_State_T state;        /**< Time communication state     */
    __IO uint32_t    error_code;   /**< RTC Error Code               */
    __IO uint8_t     is_cali_flag; /**< RTC clock source calibration */
#if (configUSE_RTC_REGISTER_CALLBACKS == 1)
    void (*AlarmEventCallback)(struct __RTC_Handle_T *hrtc);  /**< RTC Alarm  Event callback      */
    void (*SecondEventCallback)(struct __RTC_Handle_T *hrtc); /**< RTC Second  Event callback      */
    void (*MspInitCallback)(struct __RTC_Handle_T *hrtc);     /**< RTC Msp Init callback          */
    void (*MspDeInitCallback)(struct __RTC_Handle_T *hrtc);   /**< RTC Msp DeInit callback        */
#endif                                                        /* configUSE_RTC_REGISTER_CALLBACKS */

} RTC_Handle_T;

/**
 * @brief  HAL RTC Callback ID enumeration definition
 * @ingroup rtc_struct_define
 */
typedef enum
{
    HAL_RTC_ALARM_EVENT_CB_ID  = 0x00U, /**< RTC Alarm A Event Callback ID       */
    HAL_RTC_SECOND_EVENT_CB_ID = 0x01U, /**< RTC Second A Event Callback ID       */
    HAL_RTC_MSPINIT_CB_ID      = 0x0EU, /**< RTC Msp Init callback ID            */
    HAL_RTC_MSPDEINIT_CB_ID    = 0x0FU  /**< RTC Msp DeInit callback ID          */
} HAL_RTC_CallbackID_T;

/**
 * @brief  HAL RTC Callback pointer definition
 * @ingroup rtc_struct_define
 */
typedef void (*RTC_Callback_T)(RTC_Handle_T *hrtc); /**< pointer to an RTC callback function */

/* Exported constants --------------------------------------------------------*/

/**
 * \defgroup rtc_clk_source IWDG Clock Source
 * \ingroup rtc_control
 * @{
 */
#define RTC_CLK_SOURCE_ILRC 0x00000000U /**< RTC ILRC 32K clock resource     */
#define RTC_CLK_SOURCE_ELS  0x00000001U /**< RTC ELS 32.768K clock resource  */
/**
 * @}
 */

/**
 * \defgroup rtc_format RTC Format
 * \ingroup rtc_control
 * @{
 */
#define RTC_FORMAT_BIN 0x00000000U /**< RTC bin code  */
#define RTC_FORMAT_BCD 0x00000001U /**< RTC bcd code  */
/**
 * @}
 */

/**
 * \defgroup rtc_month_date BCD Month Date
 * \ingroup rtc_control
 * @{
 */
#define RTC_MONTH_JANUARY   (0x01U) /**< RTC bcd code for month JANUARY   */
#define RTC_MONTH_FEBRUARY  (0x02U) /**< RTC bcd code for month FEBRUARY  */
#define RTC_MONTH_MARCH     (0x03U) /**< RTC bcd code for month MARCH     */
#define RTC_MONTH_APRIL     (0x04U) /**< RTC bcd code for month APRIL     */
#define RTC_MONTH_MAY       (0x05U) /**< RTC bcd code for month MAY       */
#define RTC_MONTH_JUNE      (0x06U) /**< RTC bcd code for month JUNE      */
#define RTC_MONTH_JULY      (0x07U) /**< RTC bcd code for month JULY      */
#define RTC_MONTH_AUGUST    (0x08U) /**< RTC bcd code for month AUGUST    */
#define RTC_MONTH_SEPTEMBER (0x09U) /**< RTC bcd code for month SEPTEMBER */
#define RTC_MONTH_OCTOBER   (0x10U) /**< RTC bcd code for month OCTOBER   */
#define RTC_MONTH_NOVEMBER  (0x11U) /**< RTC bcd code for month NOVEMBER  */
#define RTC_MONTH_DECEMBER  (0x12U) /**< RTC bcd code for month DECEMBER  */
/**
 * @}
 */

/**
 * \defgroup rtc_weekday RTC Weekday
 * \ingroup rtc_control
 * @{
 */
#define RTC_WEEKDAY_SUNDAY    (0x00U) /**< RTC weekday code for SUNDAY    */
#define RTC_WEEKDAY_MONDAY    (0x01U) /**< RTC weekday code for MONDAY    */
#define RTC_WEEKDAY_TUESDAY   (0x02U) /**< RTC weekday code for TUESDAY   */
#define RTC_WEEKDAY_WEDNESDAY (0x03U) /**< RTC weekday code for WEDNESDAY */
#define RTC_WEEKDAY_THURSDAY  (0x04U) /**< RTC weekday code for THURSDAY  */
#define RTC_WEEKDAY_FRIDAY    (0x05U) /**< RTC weekday code for FRIDAY    */
#define RTC_WEEKDAY_SATURDAY  (0x06U) /**< RTC weekday code for SATURDAY  */
/**
 * @}
 */

/**
 * \defgroup rtc_alarm_selection RTC Alarm Selection
 * \ingroup rtc_control
 * @{
 */
#define RTC_ALARMSEL_DATE    0x00000000U /**< RTC Alarm Selection date    */
#define RTC_ALARMSEL_WEEKDAY 0x00000001U /**< RTC Alarm Selection weekday */
/**
 * @}
 */

/**
 * \brief RTC Backup Register
 * \ingroup rtc_control
 * @{
 */
#define RTC_BKP_DR0  0x00000000U /**< RTC backup register index */
#define RTC_BKP_DR1  0x00000001U /**< RTC backup register index */
#define RTC_BKP_DR2  0x00000002U /**< RTC backup register index */
#define RTC_BKP_DR3  0x00000003U /**< RTC backup register index */
#define RTC_BKP_DR4  0x00000004U /**< RTC backup register index */
#define RTC_BKP_DR5  0x00000005U /**< RTC backup register index */
#define RTC_BKP_DR6  0x00000006U /**< RTC backup register index */
#define RTC_BKP_DR7  0x00000007U /**< RTC backup register index */
#define RTC_BKP_DR8  0x00000008U /**< RTC backup register index */
#define RTC_BKP_DR9  0x00000009U /**< RTC backup register index */
#define RTC_BKP_DR10 0x0000000AU /**< RTC backup register index */
#define RTC_BKP_DR11 0x0000000BU /**< RTC backup register index */
#define RTC_BKP_DR12 0x0000000CU /**< RTC backup register index */
#define RTC_BKP_DR13 0x0000000DU /**< RTC backup register index */
#define RTC_BKP_DR14 0x0000000EU /**< RTC backup register index */
#define RTC_BKP_DR15 0x0000000FU /**< RTC backup register index */
#define RTC_BKP_DR16 0x00000010U /**< RTC backup register index */
#define RTC_BKP_DR17 0x00000011U /**< RTC backup register index */
#define RTC_BKP_DR18 0x00000012U /**< RTC backup register index */
#define RTC_BKP_DR19 0x00000013U /**< RTC backup register index */
/**
 * @}
 */

/* Initialization and de-initialization functions  ****************************/
HAL_Status_T HAL_RTC_Init(RTC_Handle_T *hrtc);
HAL_Status_T HAL_RTC_DeInit(RTC_Handle_T *hrtc);
void         HAL_RTC_MspInit(RTC_Handle_T *hrtc);
void         HAL_RTC_MspDeInit(RTC_Handle_T *hrtc);

/* Callbacks Register/UnRegister functions  ***********************************/
HAL_Status_T HAL_RTC_RegisterCallback(RTC_Handle_T *hrtc, HAL_RTC_CallbackID_T callback_id, RTC_Callback_T callback);
HAL_Status_T HAL_RTC_UnRegisterCallback(RTC_Handle_T *hrtc, HAL_RTC_CallbackID_T callback_id);

/* RTC Time and Date functions ************************************************/
HAL_Status_T HAL_RTC_SetTime(RTC_Handle_T *hrtc, RTC_Time_T *stime, uint32_t format);
HAL_Status_T HAL_RTC_GetTime(RTC_Handle_T *hrtc, RTC_Time_T *time, uint32_t format);
HAL_Status_T HAL_RTC_SetDate(RTC_Handle_T *hrtc, RTC_Date_T *sdate, uint32_t format);
HAL_Status_T HAL_RTC_GetDate(RTC_Handle_T *hrtc, RTC_Date_T *date, uint32_t format);

/* RTC Alarm functions ********************************************************/
HAL_Status_T HAL_RTC_SetAlarm(RTC_Handle_T *hrtc, RTC_Alarm_T *salarm, uint32_t format);
HAL_Status_T HAL_RTC_SetAlarm_IT(RTC_Handle_T *hrtc, RTC_Alarm_T *salarm, uint32_t format);
HAL_Status_T HAL_RTC_DeactivateAlarm(RTC_Handle_T *hrtc);
HAL_Status_T HAL_RTC_GetAlarm(RTC_Handle_T *hrtc, RTC_Alarm_T *salarm, uint32_t format);
HAL_Status_T HAL_RTC_PollForAlarmEvent(RTC_Handle_T *hrtc, uint32_t timeout);

/* RTC Second functions ********************************************************/
HAL_Status_T HAL_RTC_ActivateSecond_IT(RTC_Handle_T *hrtc);
HAL_Status_T HAL_RTC_DeactivateSecond(RTC_Handle_T *hrtc);

/* Peripheral state and errors functions **************************************/
HAL_State_T HAL_RTC_GetState(RTC_Handle_T *hrtc);
uint32_t    HAL_RTC_GetError(RTC_Handle_T *hrtc);

/* I/O operation extend functions *********************************************/
uint32_t     HAL_RTCEx_BKUPRead(RTC_Handle_T *hrtc, uint32_t backup_register);
HAL_Status_T HAL_RTCEx_BKUPWrite(RTC_Handle_T *hrtc, uint32_t backup_register, uint32_t data);
void         HAL_RTCEx_Calibration(RTC_Handle_T *hrtc, uint32_t measure_data);
void         HAL_RTCEx_CalibrationWithTrim(RTC_Handle_T *hrtc, uint32_t trim_p, uint32_t trim_f);
HAL_Status_T HAL_RTC_Enable(RTC_Handle_T *hrtc);
HAL_Status_T HAL_RTC_Disable(RTC_Handle_T *hrtc);

/* IRQ Handler functions ******************************************************/
void HAL_RTC_AlarmIRQHandler(RTC_Handle_T *hrtc, uint32_t ris);

/* Event Callback functions ***************************************************/
void HAL_RTC_AlarmEventCallback(RTC_Handle_T *hrtc);
void HAL_RTC_SecondEventCallback(RTC_Handle_T *hrtc);

#ifdef __cplusplus
}
#endif

#endif /* _HAL_RTC_H_ */
