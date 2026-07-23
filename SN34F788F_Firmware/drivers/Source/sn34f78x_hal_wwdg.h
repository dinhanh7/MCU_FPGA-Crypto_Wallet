/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _SN34F78X_HAL_WWDG_H_
#define _SN34F78X_HAL_WWDG_H_

/* Includes ------------------------------------------------------------------*/
#include "sn34f78x_hal.h"

/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
// define feed key
#define WWDG_FEED_KEY 0xAAAAU
// define the key which enable access to the WWDT_PRESCALER and WWDT_RELOAD registers.
#define WWDG_REPROG_KEY 0X5555U
// define the start WWDT key
#define WWDG_START_KEY 0xCCCCU
// define the key which disable access to the the WWDT_PRESCALER and WWDT_RELOAD registers.
#define WWDG_DISREPROG_KEY 0X0000U

#define WWDG_RIS_OVF_MASK (1UL << 0)
#define WWDG_RIS_UDF_MASK (1UL << 1)

/* Private macros ------------------------------------------------------------*/
// check whether WWDG instance is correct
#define IS_WWDG_INSTANCE(HANDLE) (((HANDLE)->instance) == SN_WWDT)
// check whether WWDG mode is reset or IRQ
#define IS_WWDG_MODE(HANDLE) ((HANDLE->init.mode & WWDG_MODE_RESET) == WWDG_MODE_RESET ||       \
                              (HANDLE->init.mode & WWDG_MODE_OVERFLOW) == WWDG_MODE_OVERFLOW || \
                              (HANDLE->init.mode & WWDG_MODE_EARLYWAKEUP) == WWDG_MODE_EARLYWAKEUP)
// check whether WWDG mode has reset
#define HAS_WWDG_MODE_RESET(HANDLE) ((HANDLE->init.mode & WWDG_MODE_RESET) == WWDG_MODE_RESET)
// check whether WWDG mode has reset
#define HAS_WWDG_MODE_OVERFLOW(HANDLE) ((HANDLE->init.mode & WWDG_MODE_OVERFLOW) == WWDG_MODE_OVERFLOW)
// check whether WWDG window mode has early-wakeup
#define HAS_WWDG_MODE_EARLYWAKEUP(HANDLE) ((HANDLE->init.mode & WWDG_MODE_EARLYWAKEUP) == WWDG_MODE_EARLYWAKEUP)
// check whether WWDG timer constant is correct
#define IS_WWDG_TIMER_CONSTANT(HANDLE) (HANDLE->init.counter >= WWDG_TIMER_MIN && HANDLE->init.counter <= WWDG_TIMER_MAX)
// check whether prescaler is correct
#define IS_WWDG_PRESCALER(HANDLE) ((HANDLE->init.prescaler == WWDG_PRESCALER_0) || (HANDLE->init.prescaler == WWDG_PRESCALER_1) || \
                                   (HANDLE->init.prescaler == WWDG_PRESCALER_2) || (HANDLE->init.prescaler == WWDG_PRESCALER_3) || \
                                   (HANDLE->init.prescaler == WWDG_PRESCALER_4) || (HANDLE->init.prescaler == WWDG_PRESCALER_5) || \
                                   (HANDLE->init.prescaler == WWDG_PRESCALER_6) || (HANDLE->init.prescaler == WWDG_PRESCALER_7))
// check whether prescaler is correct
#define IS_WWDG_WIN_CONSTANT(HANDLE) (HANDLE->init.window >= WWDG_TIMER_MIN && HANDLE->init.window <= WWDG_TIMER_MAX)
// check whether win constant is greater than timer constant
#define IS_WWDG_WIN_CONSTANT_LESS_TIMER_CONSTANT(HANDLE) (HANDLE->init.window < HANDLE->init.counter)
// check whether WWDT counter reaches zero
#define IS_WWDG_OVERFLOW(HANDLE) (HAL_REG_READ(HANDLE->instance->RIS_b.OVF) == 1)
// check whether WWDT enters underflow state
#define IS_WWDG_EARLYWAKEUP(HANDLE) (HAL_REG_READ(HANDLE->instance->RIS_b.UDF) == 1)
// wait for settings prescaler to complete
#define WWDG_WAIT_PRELOCK_COMPLETE(HANDLE)                               \
    while (HAL_REG_READ(HANDLE->instance->LOCKST_b.PRESCALER_LOCK) == 1) \
    {                                                                    \
        __NOP();                                                         \
    }

// wait for settings reload to complete
#define WWDG_WAIT_RELOADLOCK_COMPLETE(HANDLE)                         \
    while (HAL_REG_READ(HANDLE->instance->LOCKST_b.RELOAD_LOCK) == 1) \
    {                                                                 \
        __NOP();                                                      \
    }

/* Private functions prototypes ----------------------------------------------*/
// define function which checks WWDG config is correct
static HAL_Status_T _wwdg_check_config(WWDG_Handle_T *hwwdg);
// define function which sets WWDG config
static HAL_Status_T _wwdg_set_config(WWDG_Handle_T *hwwdg);

#endif /* _SN34F78X_HAL_WWDG_H_ */
