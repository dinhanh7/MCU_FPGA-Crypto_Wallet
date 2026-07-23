/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _SN34F78X_HAL_IWDG_H_
#define _SN34F78X_HAL_IWDG_H_

/* Includes ------------------------------------------------------------------*/
#include "sn34f78x_hal.h"

/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
// define feed key
#define IWDG_FEED_KEY 0x00005AB9U
// define interrupt enable mask
#define IWDG_IE_MASK 0x00000004U
// define reset enable mask
#define IWDG_RST_MASK 0x00000002U
// define IWDG enable mask
#define IWDG_EN_MASK 0x00000001U

/* Private macros ------------------------------------------------------------*/
// check whether IWDG instance is correct
#define IS_IWDG_INSTANCE(HANDLE) (((HANDLE)->instance) == SN_WDT)
// check whether IWDG mode is reset or IRQ
#define IS_IWDG_MODE(HANDLE) (HANDLE->init.mode == IWDG_MODE_RESET || HANDLE->init.mode == IWDG_MODE_IRQ)
// check whether IWDG mode is reset
#define IS_IWDG_MODE_RESET(HANDLE) (HANDLE->init.mode == IWDG_MODE_RESET)
// check whether IWDG timer constant is correct
#define IS_IWDG_TIMER_CONSTANT(HANDLE) (HANDLE->init.timer_constant >= IWDG_TIMER_MIN && HANDLE->init.timer_constant <= IWDG_TIMER_MAX)

/* Private functions prototypes ----------------------------------------------*/
// define function which checks IWDG config is correct
static HAL_Status_T _iwdg_check_config(IWDG_Handle_T *hiwdg);
// define function which sets IWDG config
static HAL_Status_T _iwdg_set_config(IWDG_Handle_T *hiwdg);

#endif /* _SN34F78X_HAL_IWDG_H_ */
