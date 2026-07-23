/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _SN34F78X_LL_RTC_H_
#define _SN34F78X_LL_RTC_H_

/* Includes ------------------------------------------------------------------*/
#include "sn34f78x_ll.h"

/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
// Default measurement at standard 32.768KHZ clock frequency
#define LL_RTC_MEASURE_DATA  (1000000U)
#define LL_KEEP_WEEKDAY_MASK (0x8FFFFFF)

// check whether interrupt from alarm
#define IS_LL_RTC_ALARM_IT(ris) (SCU_RIS_INT_RTC_ALARM & ris)
// check whether interrupt from alarm
#define IS_LL_RTC_SEC_IT(ris) (SCU_RIS_INT_RTC_SEC & ris)
// check whether interrupt from alarm
#define IS_LL_RTC_PER_IT(ris) (SCU_RIS_INT_RTC_PER & ris)

#define IS_LL_RTC_CLK_SOURCE(CLOCK_SOURCE) (((CLOCK_SOURCE) == LL_RTC_CLK_SOURCE_ILRC) || \
                                            ((CLOCK_SOURCE) == LL_RTC_CLK_SOURCE_ELS))

#define IS_LL_RTC_FORMAT(FORMAT) (((FORMAT) == LL_RTC_FORMAT_BIN) || ((FORMAT) == LL_RTC_FORMAT_BCD))

#define IS_LL_RTC_HOUR24(HOUR)     ((HOUR) <= 23U)
#define IS_LL_RTC_MINUTES(MINUTES) ((MINUTES) <= 59U)
#define IS_LL_RTC_SECONDS(SECONDS) ((SECONDS) <= 59U)

#define IS_LL_RTC_CENTURY(CENTURY) ((CENTURY) <= 99U)
#define IS_LL_RTC_YEAR(YEAR)       ((YEAR) <= 99U)
#define IS_LL_RTC_MONTH(MONTH)     (((MONTH) >= 1U) && ((MONTH) <= 12U))
#define IS_LL_RTC_DATE(DATE)       (((DATE) >= 1U) && ((DATE) <= 31U))

#define IS_LL_RTC_ALARM(ALARM) ((ALARM) == LL_RTC_ALARM)

#define IS_LL_RTC_WEEKDAY(WEEKDAY) (((WEEKDAY) == LL_RTC_WEEKDAY_MONDAY) ||    \
                                    ((WEEKDAY) == LL_RTC_WEEKDAY_TUESDAY) ||   \
                                    ((WEEKDAY) == LL_RTC_WEEKDAY_WEDNESDAY) || \
                                    ((WEEKDAY) == LL_RTC_WEEKDAY_THURSDAY) ||  \
                                    ((WEEKDAY) == LL_RTC_WEEKDAY_FRIDAY) ||    \
                                    ((WEEKDAY) == LL_RTC_WEEKDAY_SATURDAY) ||  \
                                    ((WEEKDAY) == LL_RTC_WEEKDAY_SUNDAY))

#define IS_LL_RTC_ALARM_DATE_WEEKDAY_SEL(SEL) (((SEL) == LL_RTC_ALARMSEL_DATE) || \
                                               ((SEL) == LL_RTC_ALARMSEL_WEEKDAY))
#define IS_LL_RTC_ALARM_DATE_WEEKDAY_DATE(DATE) (((DATE) > 0U) && ((DATE) <= 31U))

#define IS_LL_RTC_ALARM_DATE_WEEKDAY_WEEKDAY(WEEKDAY) (((WEEKDAY) == LL_RTC_WEEKDAY_MONDAY) ||    \
                                                       ((WEEKDAY) == LL_RTC_WEEKDAY_TUESDAY) ||   \
                                                       ((WEEKDAY) == LL_RTC_WEEKDAY_WEDNESDAY) || \
                                                       ((WEEKDAY) == LL_RTC_WEEKDAY_THURSDAY) ||  \
                                                       ((WEEKDAY) == LL_RTC_WEEKDAY_FRIDAY) ||    \
                                                       ((WEEKDAY) == LL_RTC_WEEKDAY_SATURDAY) ||  \
                                                       ((WEEKDAY) == LL_RTC_WEEKDAY_SUNDAY))

#define IS_LL_RTC_BKP(BKP) ((BKP) <= LL_RTC_BKP_DR19)

/* Private functions prototypes ----------------------------------------------*/

#endif /* _SN34F78X_LL_RTC_H_ */
