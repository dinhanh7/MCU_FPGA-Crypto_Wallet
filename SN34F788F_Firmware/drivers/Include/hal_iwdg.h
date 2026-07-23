/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _HAL_IWDG_H_
#define _HAL_IWDG_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sn34f78x_hal_def.h"

/* Exported types ------------------------------------------------------------*/

/**
 * \brief IWDG Init Structure Definition
 * \ingroup iwdg_struct_define
 */
typedef struct
{
    uint32_t mode;           /**< Specifies what to do when IWDG timeout.
                                  This parameter can be a value of @ref iwdg_mode */
    uint32_t timer_constant; /**< Specifies the timeout value of the IWDG.
                                  This parameter can be a value in range of @ref iwdg_timer_range */
} IWDG_Init_T;

/**
 * \brief IWDG Handle Structure Definition
 * \ingroup iwdg_struct_define
 */
typedef struct __IWDG_Handle_T
{
    SN_WDT_Type *instance;   /**< Register Base Address    */
    IWDG_Init_T  init;       /**< IWDG Required Parameters */
    HAL_MUTEX    mutex;      /**< IWDG Process Protection  */
    HAL_State_T  state;      /**< IWDG State               */
    uint32_t     error_code; /**< IWDG Error Code          */
#if (configUSE_IWDG_REGISTER_CALLBACKS == 1)
    void (*MspInitCallback)(struct __IWDG_Handle_T *hiwdg);   /**< IWDG Msp init callback   */
    void (*MspDeInitCallback)(struct __IWDG_Handle_T *hwwdg); /**< IWDG Msp deinit callback */
    void (*TimeoutCallback)(struct __IWDG_Handle_T *hiwdg);   /**< IWDG Timeout Callback    */
#endif
} IWDG_Handle_T;

/**
 * \brief HAL IWDG Callback ID enumeration definition
 * \ingroup iwdg_struct_define
 */
typedef enum
{
    HAL_IWDG_MSPINIT_CB_ID   = 0x00U, /**< HAL IWDG Msp init callback   */
    HAL_IWDG_MSPDEINIT_CB_ID = 0x01U, /**< HAL IWDG Msp deinit callback */
    HAL_IWDG_TIMEOUT_CB_ID   = 0x02U, /**< HAL IWDG Timeout Callback ID */
} HAL_IWDG_CallbackID_T;

/**
 * \brief HAL IWDG Callback pointer definition
 * \ingroup iwdg_struct_define
 */
typedef void (*IWDG_Callback_T)(IWDG_Handle_T *hiwdg); /**< pointer to a IWDG common callback functions */

/* Exported constants --------------------------------------------------------*/
/**
 * \defgroup iwdg_mode IWDG Mode
 * \ingroup iwdg_control
 * @{
 */
#define IWDG_MODE_RESET 0x02U /**< IWDG reset mode     */
#define IWDG_MODE_IRQ   0x04U /**< IWDG interrupt mode */
/**
 * @}
 */

/**
 * \defgroup iwdg_timer_range IWDG Timer Constant Range
 * \ingroup iwdg_control
 * @{
 */
#define IWDG_TIMER_MIN 0x00000001U /**< IWDG timer constant min */
#define IWDG_TIMER_MAX 0xFFFFFFFFU /**< IWDG timer constant max */
/**
 * @}
 */

/**
 * \ingroup iwdg_error_code
 * @{
 */
#define HAL_IWDG_ERROR_INVALID_CALLBACK (HAL_ERROR_SPECIFY + 1U) /**< Invalid callback error */
#define HAL_IWDG_ERROR_NOT_RUNNING      (HAL_ERROR_SPECIFY + 2U) /**< IWDG is not running */
/**
 * @}
 */

/* Exported functions --------------------------------------------------------*/
/* Initialization/De-initialization functions *********************************/
HAL_Status_T HAL_IWDG_Init(IWDG_Handle_T *hiwdg);
HAL_Status_T HAL_IWDG_DeInit(IWDG_Handle_T *hiwdg);
void         HAL_IWDG_MspInit(IWDG_Handle_T *hiwdg);
void         HAL_IWDG_MspDeInit(IWDG_Handle_T *hiwdg);

/* Callbacks Register/UnRegister functions ************************************/
HAL_Status_T HAL_IWDG_RegisterCallback(IWDG_Handle_T *hiwdg, HAL_IWDG_CallbackID_T callback_id, IWDG_Callback_T callback);
HAL_Status_T HAL_IWDG_UnRegisterCallback(IWDG_Handle_T *hiwdg, HAL_IWDG_CallbackID_T callback_id);

/* Peripheral Control functions ***********************************************/
HAL_Status_T HAL_IWDG_Refresh(IWDG_Handle_T *hiwdg);

/* Peripheral State and Errors functions **************************************/
HAL_State_T HAL_IWDG_GetState(IWDG_Handle_T *hiwdg);
uint32_t    HAL_IWDG_GetError(IWDG_Handle_T *hiwdg);

/* I/O Operation functions ****************************************************/
void HAL_IWDG_IRQHandle(IWDG_Handle_T *hiwdg);

/* Event Callback functions ***************************************************/
void HAL_IWDG_TimeoutCallback(IWDG_Handle_T *hiwdg);

#ifdef __cplusplus
}
#endif

#endif /* _HAL_IWDG_H_ */
