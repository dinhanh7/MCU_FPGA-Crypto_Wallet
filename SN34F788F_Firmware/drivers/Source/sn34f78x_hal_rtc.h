/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _SN34F78X_HAL_RTC_H_
#define _SN34F78X_HAL_RTC_H_

/* Includes ------------------------------------------------------------------*/
#include "sn34f78x_hal.h"

// Default measurement at standard 32.768KHZ clock frequency
#define RTC_MEASURE_DATA (1000000U)

#define RTCEN_STS_DISABLE (0U)
#define RTCEN_STS_ENABLE  (1U)

#define RTC_ALM_STS_DISABLE (0U)
#define RTC_ALM_STS_ENABLE  (1U)

// check whether interrupt from alarm
#define IS_RTC_ALARM_IT(ris) (SCU_RIS_RTC_ALARM & ris)
// check whether interrupt from alarm
#define IS_RTC_SEC_IT(ris) (SCU_RIS_RTC_SEC & ris)
// check whether interrupt from alarm
#define IS_RTC_PER_IT(ris) (SCU_RIS_RTC_PER & ris)

#define IS_RTC_CLK_SOURCE(CLOCK_SOURCE) (((CLOCK_SOURCE) == RTC_CLK_SOURCE_ILRC) || \
                                         ((CLOCK_SOURCE) == RTC_CLK_SOURCE_ELS))

#define IS_RTC_FORMAT(FORMAT) (((FORMAT) == RTC_FORMAT_BIN) || ((FORMAT) == RTC_FORMAT_BCD))

#define IS_RTC_HOUR24(HOUR)     ((HOUR) <= 23U)
#define IS_RTC_MINUTES(MINUTES) ((MINUTES) <= 59U)
#define IS_RTC_SECONDS(SECONDS) ((SECONDS) <= 59U)

#define IS_RTC_CENTURY(CENTURY) ((CENTURY) <= 99U)
#define IS_RTC_YEAR(YEAR)       ((YEAR) <= 99U)
#define IS_RTC_MONTH(MONTH)     (((MONTH) >= 1U) && ((MONTH) <= 12U))
#define IS_RTC_DATE(DATE)       (((DATE) >= 1U) && ((DATE) <= 31U))

#define RTC_BCD_TIME1_WEEK_Pos (24U)
#define RTC_BCD_TIME1_HOUR_Pos (16U)
#define RTC_BCD_TIME1_MIN_Pos  (8U)

#define DEC_BIT_MASK  (0XF0)
#define UNIT_BIT_MASK (0X0F)

#define KEEP_WEEKDAY_MASK (0x8FFFFFF)

#define RTC_TIME_DEC_Pos (4U)

#define RTC_TIMEOUT_VALUE 5000U

#define ALARM_IRQ_FLAG_MASK (0x00010000)

#define SECOND_IRQ_FLAG_MASK (0x01000000)

#define IS_RTC_ALARM(ALARM) ((ALARM) == RTC_ALARM)

#define IS_RTC_WEEKDAY(WEEKDAY) (((WEEKDAY) == RTC_WEEKDAY_MONDAY) ||    \
                                 ((WEEKDAY) == RTC_WEEKDAY_TUESDAY) ||   \
                                 ((WEEKDAY) == RTC_WEEKDAY_WEDNESDAY) || \
                                 ((WEEKDAY) == RTC_WEEKDAY_THURSDAY) ||  \
                                 ((WEEKDAY) == RTC_WEEKDAY_FRIDAY) ||    \
                                 ((WEEKDAY) == RTC_WEEKDAY_SATURDAY) ||  \
                                 ((WEEKDAY) == RTC_WEEKDAY_SUNDAY))

#define IS_RTC_ALARM_MASK(MASK)            (((MASK) & ((uint32_t)~RTC_ALARMMASK_ALL)) == 0U)
#define IS_RTC_ALARM_DATE_WEEKDAY_SEL(SEL) (((SEL) == RTC_ALARMSEL_DATE) || \
                                            ((SEL) == RTC_ALARMSEL_WEEKDAY))
#define IS_RTC_ALARM_DATE_WEEKDAY_DATE(DATE) (((DATE) > 0U) && ((DATE) <= 31U))

#define IS_RTC_ALARM_DATE_WEEKDAY_WEEKDAY(WEEKDAY) (((WEEKDAY) == RTC_WEEKDAY_MONDAY) ||    \
                                                    ((WEEKDAY) == RTC_WEEKDAY_TUESDAY) ||   \
                                                    ((WEEKDAY) == RTC_WEEKDAY_WEDNESDAY) || \
                                                    ((WEEKDAY) == RTC_WEEKDAY_THURSDAY) ||  \
                                                    ((WEEKDAY) == RTC_WEEKDAY_FRIDAY) ||    \
                                                    ((WEEKDAY) == RTC_WEEKDAY_SATURDAY) ||  \
                                                    ((WEEKDAY) == RTC_WEEKDAY_SUNDAY))

#define IS_RTC_BKP(BKP) ((BKP) <= RTC_BKP_DR19)

#define RTC_ALM1_INIT_VAL  (0xffffffff)
#define RTC_TIME2_INIT_VAL (0x140a0101)

static uint8_t _rtc_bcd_to_byte(uint8_t value);

static HAL_Status_T _rtc_alm_sts_check(RTC_Handle_T *hrtc, uint32_t rtc_alm_sts);
static HAL_Status_T _rtc_en_sts_check(RTC_Handle_T *hrtc, uint32_t rtc_sts);
/* Compensates for frequency drift caused by crystal error of external clock source */
static void _rtc_calibration_(uint32_t one_micro_second);

static HAL_Status_T _rtc_alarm(RTC_Handle_T *hrtc, RTC_Alarm_T *salarm, uint32_t format);

#endif /* _SN34F78X_HAL_RTC_H_ */
