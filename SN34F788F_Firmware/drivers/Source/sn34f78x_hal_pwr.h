/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _SN34F78X_HAL_PWR_H_
#define _SN34F78X_HAL_PWR_H_

/* Includes ------------------------------------------------------------------*/
#include "sn34f78x_hal.h"

/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
// SCU boot-up mask
#define PWR_BTUP_MASK (0xFF << 17)
// enable Sleep mode mask(SCU_PWMMODE register)
#define PWR_ENTER_SLEEPMODE 0x00000008
// enable Deep Sleep mode mask
#define PWR_ENTER_DEEPSLEEPMODE 0x00000004
// enable Deep Power Down mode mask
#define PWR_ENTER_DPDMODE 0x00000002
// wake up event mask
#define PWR_WAKUPST 0x00000002

/* Private macros ------------------------------------------------------------*/
// check whether wakeup from Sleep mode
#define IS_PWR_WAKEUP_FROM_SLEEP(ris) (SCU_RIS_SLP_WAKEUP & ris)
// check whether wakeup from Sleep by interrupt
#define IS_PWR_WAKEUP_FROM_SLEEP_INT ((HAL_REG_READ(SN_SCU->SLP_WAKUPST_b.SLP_WAKUP_INT)) == 1)
// check whether wakeup from Deep Sleep mode
#define IS_PWR_WAKEUP_FROM_DEEPSLEEP(ris) (SCU_RIS_DS_WAKEUP & ris)
// check whether wakeup from Deep Power Down mode
#define IS_PWR_WAKEUP_FROM_DPD (HAL_REG_READ(SN_SCU->BTUP_STS_b.DPDWKF) == 1)

/* Private function ------------------------------------------------------------*/

#endif /* _SN34F78X_HAL_PWR_H_ */
