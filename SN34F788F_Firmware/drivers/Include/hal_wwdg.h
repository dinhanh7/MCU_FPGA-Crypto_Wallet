/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _HAL_WWDG_H_
#define _HAL_WWDG_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sn34f78x_hal_def.h"

/* Exported types ------------------------------------------------------------*/
/**
 * \brief WWDG Init Structure Definition
 * \ingroup wwdg_struct_define
 */
typedef struct
{
    uint32_t prescaler; /**< Specifies the pre-scaler value of the WWDG.
                             This parameter can be a value of @ref wwdg_prescaler */
    uint32_t counter;   /**< Specifies the timeout value of the WWDG.
                             This parameter can be a value in range of @ref wwdg_counter_range */
    uint32_t window;    /**< Specifies the WWDG window value to be compared to the down-counter.
                             This parameter can be a value in range of @ref wwdg_counter_range */
    uint32_t mode;      /**< Specifies what to do when WWDG timeout.
                             This parameter can be ORed value of @ref wwdg_mode */
} WWDG_Init_T;

/**
 * \brief WWDG Handle Structure Definition
 * \ingroup wwdg_struct_define
 */
typedef struct __WWDG_Handle_T
{
    SN_WWDT_Type *instance;   /**< Register Base Address    */
    WWDG_Init_T   init;       /**< WWDG Required Parameters */
    HAL_MUTEX     mutex;      /**< WWDG Process Protection  */
    HAL_State_T   state;      /**< WWDG State               */
    uint32_t      error_code; /**< WWDG Error Code          */
#if (configUSE_WWDG_REGISTER_CALLBACKS == 1)
    void (*MspInitCallback)(struct __WWDG_Handle_T *hwwdg);   /**< WWDG Msp init callback   */
    void (*MspDeInitCallback)(struct __WWDG_Handle_T *hwwdg); /**< WWDG Msp deinit callback */
    void (*TimeoutCallback)(struct __WWDG_Handle_T *hwwdg);   /**< WWDG Timeout Callback    */
    void (*EarlyWakeupCallback)(struct __WWDG_Handle_T *hwwdg);
#endif
} WWDG_Handle_T;

/**
 * \brief HAL WWDG Callback ID enumeration definition
 * \ingroup wwdg_struct_define
 */
typedef enum
{
    HAL_WWDG_MSPINIT_CB_ID   = 0x00U, /**< HAL WWDG Msp init callback    */
    HAL_WWDG_MSPDEINIT_CB_ID = 0x01U, /**< HAL WWDG Msp deinit callback  */
    HAL_WWDG_TIMEOUT_CB_ID   = 0x02U, /**< WWDG Timeout Callback ID      */
    HAL_WWDG_EWI_CB_ID       = 0x03U, /**< WWDG Early wakeup Callback ID */
} HAL_WWDG_CallbackID_T;

/**
 * \brief HAL WWDG Callback pointer definition
 * \ingroup wwdg_struct_define
 */
typedef void (*WWDG_Callback_T)(WWDG_Handle_T *hwwdg); /**< pointer to a WWDG common callback functions */

/* Exported constants --------------------------------------------------------*/
/**
 * \defgroup wwdg_prescaler WWDG Prescaler
 * \ingroup wwdg_control
 * @{
 */
#define WWDG_PRESCALER_0 0 /**< WWDG prescaler is divider/4   */
#define WWDG_PRESCALER_1 1 /**< WWDG prescaler is divider/8   */
#define WWDG_PRESCALER_2 2 /**< WWDG prescaler is divider/16  */
#define WWDG_PRESCALER_3 3 /**< WWDG prescaler is divider/32  */
#define WWDG_PRESCALER_4 4 /**< WWDG prescaler is divider/64  */
#define WWDG_PRESCALER_5 5 /**< WWDG prescaler is divider/128 */
#define WWDG_PRESCALER_6 6 /**< WWDG prescaler is divider/256 */
#define WWDG_PRESCALER_7 7 /**< WWDG prescaler is divider/256 */
/**
 * @}
 */

/**
 * \defgroup wwdg_counter_range WWDG Timer counter Range
 * \ingroup wwdg_control
 * @{
 */
#define WWDG_TIMER_MIN 0x00000001U /**< WWDG timer constant min */
#define WWDG_TIMER_MAX 0xFFFFFFFFU /**< WWDG timer constant max */
/**
 * @}
 */

/**
 * \defgroup wwdg_mode WWDG Mode
 * \ingroup wwdg_control
 * @{
 */
#define WWDG_MODE_EARLYWAKEUP 0x01U /**< WWDG early wakeup interrupt mode */
#define WWDG_MODE_OVERFLOW    0x02U /**< WWDG interrupt mode while counting value reaches 0 */
#define WWDG_MODE_RESET       0x04U /**< WWDG reset mode while counting value reaches 0 */
/**
 * @}
 */

/**
 * \ingroup wwdg_error_code
 * @{
 */
#define HAL_WWDG_ERROR_INVALID_CALLBACK (HAL_ERROR_SPECIFY + 1U) /**< Invalid callback error */
#define HAL_WWDG_ERROR_NOT_RUNNING      (HAL_ERROR_SPECIFY + 2U) /**< WWDG is not running */
/**
 * @}
 */

/* Exported functions --------------------------------------------------------*/
/* Initialization/De-initialization functions *********************************/
HAL_Status_T HAL_WWDG_Init(WWDG_Handle_T *hwwdg);
HAL_Status_T HAL_WWDG_DeInit(WWDG_Handle_T *hwwdg);
void         HAL_WWDG_MspInit(WWDG_Handle_T *hwwdg);
void         HAL_WWDG_MspDeInit(WWDG_Handle_T *hwwdg);

/* Callbacks Register/UnRegister functions ************************************/
HAL_Status_T HAL_WWDG_RegisterCallback(WWDG_Handle_T *hwwdg, HAL_WWDG_CallbackID_T callback_id, WWDG_Callback_T callback);
HAL_Status_T HAL_WWDG_UnRegisterCallback(WWDG_Handle_T *hwwdg, HAL_WWDG_CallbackID_T callback_id);

/* Peripheral Control functions ***********************************************/
HAL_Status_T HAL_WWDG_Refresh(WWDG_Handle_T *hwwdg);

/* Peripheral State and Errors functions **************************************/
HAL_State_T HAL_WWDG_GetState(WWDG_Handle_T *hwwdg);
uint32_t    HAL_WWDG_GetError(WWDG_Handle_T *hwwdg);

/* I/O Operation functions ****************************************************/
void HAL_WWDG_IRQHandle(WWDG_Handle_T *hwwdg);

/* Event Callback functions ***************************************************/
void HAL_WWDG_TimeoutCallback(WWDG_Handle_T *hwwdg);
void HAL_WWDG_EarlyWakeupCallback(WWDG_Handle_T *hwwdg);

#ifdef __cplusplus
}
#endif

#endif /* \
        */
