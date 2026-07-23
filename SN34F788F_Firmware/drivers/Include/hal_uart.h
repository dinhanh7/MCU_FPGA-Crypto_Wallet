/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _HAL_UART_H_
#define _HAL_UART_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sn34f78x_hal_def.h"

/* Exported types ------------------------------------------------------------*/
/**
 * \brief UART Init Structure Definition
 * \ingroup uart_struct_define
 */
typedef struct
{
    uint32_t baud_rate;   /**< Specifies the UART communication speed.
                               This parameter can be a value in range of @ref uart_baud_rate_range  */
    uint32_t word_length; /**< Specifies the number of data bits transmitted or received in a frame.
                               This parameter can be a value of @ref uart_word_length               */
    uint32_t stop_bits;   /**< Specifies the number of stop bits transmitted.
                               This parameter can be a value of @ref uart_stop_bits                 */
    uint32_t parity;      /**< Specifies the parity mode.
                               This parameter can be a value of @ref uart_parity                    */
    uint32_t hw_flow_ctl; /**< Specifies whether the hardware flow control mode is enabled or disabled.
                               This parameter can be a value of @ref uart_hardware_flow             */
} UART_Init_T;

/**
 * \brief   HAL UART transfer runtime information
 * \ingroup uart_struct_define
 */
typedef struct
{
    uint8_t      *buffer;     /**< Pointer to UART transfer Buffer */
    uint16_t      xfer_size;  /**< UART Transfer Size              */
    __IO uint16_t xfer_count; /**< UART Transfer Counter           */
    __IO uint8_t  abort;      /**< UART Abort Flag                 */
} UART_XFER_T;

/**
 * \brief UART Handle Structure Definition
 * \ingroup uart_struct_define
 */
typedef struct __UART_Handle_T
{
    void            *instance;   /**< Register Base Address */
    UART_Init_T      init;       /**< UART Required Parameters */
    HAL_MUTEX        mutex;      /**< UART Process Protection Flag */
    __IO HAL_State_T state;      /**< UART state information related to global Handle management and also related to Tx operations. */
    __IO HAL_State_T rx_state;   /**< UART state information related to Rx operations. */
    __IO uint32_t    error_code; /**< UART Error Code */
    UART_XFER_T      tx;         /**< UART Tx transfer runtime information */
    UART_XFER_T      rx;         /**< UART Rx transfer runtime information */
    uint32_t         state_ext;  /**< UART Internal maintenance state */
#if ((configUSE_UART_NONBLOCKING_DMA == 1))
    DMA_Handle_T *hdmatx; /**< UART Tx DMA Handle parameters */
    DMA_Handle_T *hdmarx; /**< UART Rx DMA Handle parameters */
#endif
#if (configUSE_UART_REGISTER_CALLBACKS == 1)
    void (*TxCpltCallback)(struct __UART_Handle_T *huart);            /**< UART Tx Complete Callback    */
    void (*RxCpltCallback)(struct __UART_Handle_T *huart);            /**< UART Rx Complete Callback    */
    void (*AbortCpltCallback)(struct __UART_Handle_T *huart);         /**< UART Abort Complete Callback */
    void (*AbortTransmitCpltCallback)(struct __UART_Handle_T *huart); /**< UART Abort Transmit Complete Callback */
    void (*AbortReceiveCpltCallback)(struct __UART_Handle_T *huart);  /**< UART Abort Receive Complete Callback  */
    void (*ErrorCallback)(struct __UART_Handle_T *huart);             /**< UART Error Callback     */
    void (*MspInitCallback)(struct __UART_Handle_T *huart);           /**< UART MspInit Callback   */
    void (*MspDeInitCallback)(struct __UART_Handle_T *huart);         /**< UART MspDeInit Callback */
#endif
} UART_Handle_T;

/**
 * \brief UART Callback ID Enumeration
 * \ingroup uart_struct_define
 */
typedef enum
{
    HAL_UART_TX_COMPLETE_CB_ID             = 0x00U, /**< UART Tx Complete Callback ID               */
    HAL_UART_RX_COMPLETE_CB_ID             = 0x01U, /**< UART Rx Complete Callback ID               */
    HAL_UART_ABORT_COMPLETE_CB_ID          = 0x02U, /**< UART Abort Complete Callback ID            */
    HAL_UART_ABORT_TRANSMIT_COMPLETE_CB_ID = 0x03U, /**< UART Abort Transmit Complete Callback ID   */
    HAL_UART_ABORT_RECEIVE_COMPLETE_CB_ID  = 0x04U, /**< UART Abort Receive Complete Callback ID    */
    HAL_UART_ERROR_CB_ID                   = 0x05U, /**< UART Error Callback ID                     */
    HAL_UART_MSPINIT_CB_ID                 = 0x10U, /**< UART MspInit callback ID                   */
    HAL_UART_MSPDEINIT_CB_ID               = 0x11U  /**< UART MspDeInit callback ID                 */
} HAL_UART_CallbackID_T;

/**
 * \brief UART Callback Pointer Definition
 * \ingroup uart_struct_define
 */
typedef void (*UART_Callback_T)(UART_Handle_T *huart); /**< pointer to a UART common callback functions */

/* Exported constants --------------------------------------------------------*/
/**
 * \defgroup uart_baud_rate_range UART Baud Rate Range
 * \ingroup uart_control
 * @{
 */
#define UART_BAUD_RATE_MIN 110U     /**< Baud rate min */
#define UART_BAUD_RATE_MAX 3000000U /**< Baud rate max */
/**
 * @}
 */

/**
 * \defgroup uart_word_length UART Word Length
 * \ingroup uart_control
 * @{
 */
#define UART_WORD_LENGTH_5 0x00U /**< 5 Data bits */
#define UART_WORD_LENGTH_6 0x01U /**< 6 Data bits */
#define UART_WORD_LENGTH_7 0x02U /**< 7 Data bits */
#define UART_WORD_LENGTH_8 0x03U /**< 8 Data bits */
/**
 * @}
 */

/**
 * \defgroup uart_stop_bits UART Stop Bits
 * \ingroup uart_control
 * @{
 */
#define UART_STOP_BITS_1 0x00U /**< 1 Stop bit                            */
#define UART_STOP_BITS_2 0x01U /**< 2 Stop bits (1.5 if Word Length == 5) */
/**
 * @}
 */

/**
 * \defgroup uart_parity UART Parity
 * \ingroup uart_control
 * @{
 */
#define UART_PARITY_NONE 0x00U /**< No parity   */
#define UART_PARITY_EVEN 0x01U /**< Even parity */
#define UART_PARITY_ODD  0x02U /**< Odd parity  */
/**
 * @}
 */

/**
 * \defgroup uart_hardware_flow UART Hardware Flow
 * \ingroup uart_control
 * @{
 */
#define UART_HWCONTROL_NONE    0x00U /**< No Hardware Control           */
#define UART_HWCONTROL_RTS     0x01U /**< RTS Hardware Control          */
#define UART_HWCONTROL_CTS     0x02U /**< CTS  Hardware Control         */
#define UART_HWCONTROL_RTS_CTS 0x03U /**< RTS and CTS Hardware Control  */
/**
 * @}
 */

/**
 * \brief UART Error Code
 * \ingroup uart_error_code
 * @{
 */
#define HAL_UART_ERROR_OE  (HAL_ERROR_SPECIFY + 0x01U) /**< Overrun error           */
#define HAL_UART_ERROR_PE  (HAL_ERROR_SPECIFY + 0x02U) /**< Parity error            */
#define HAL_UART_ERROR_FE  (HAL_ERROR_SPECIFY + 0x04U) /**< Frame error             */
#define HAL_UART_ERROR_BI  (HAL_ERROR_SPECIFY + 0x08U) /**< Break Interrupt         */
#define HAL_UART_ERROR_DMA (HAL_ERROR_SPECIFY + 0x10U) /**< DMA transfer error      */
/**
 * @}
 */

/* Initialization/de-initialization functions  **********************************/
HAL_Status_T HAL_UART_Init(UART_Handle_T *huart);
HAL_Status_T HAL_UART_DeInit(UART_Handle_T *huart);
void         HAL_UART_MspInit(UART_Handle_T *huart);
void         HAL_UART_MspDeInit(UART_Handle_T *huart);

/* Callbacks Register/UnRegister functions ************************************/
HAL_Status_T HAL_UART_RegisterCallback(UART_Handle_T *huart, HAL_UART_CallbackID_T callback_id, UART_Callback_T callback);
HAL_Status_T HAL_UART_UnRegisterCallback(UART_Handle_T *huart, HAL_UART_CallbackID_T callback_id);

/* Peripheral State and Errors functions **************************************/
HAL_State_T HAL_UART_GetState(UART_Handle_T *huart);
uint32_t    HAL_UART_GetError(UART_Handle_T *huart);

/* I/O Operation functions ****************************************************/
HAL_Status_T HAL_UART_Transmit(UART_Handle_T *huart, uint8_t *data, uint16_t size, uint32_t timeout);
HAL_Status_T HAL_UART_Receive(UART_Handle_T *huart, uint8_t *data, uint16_t size, uint32_t timeout);

HAL_Status_T HAL_UART_Transmit_IT(UART_Handle_T *huart, uint8_t *data, uint16_t size);
HAL_Status_T HAL_UART_Receive_IT(UART_Handle_T *huart, uint8_t *data, uint16_t size);

HAL_Status_T HAL_UART_Transmit_DMA(UART_Handle_T *huart, uint8_t *data, uint16_t size);
HAL_Status_T HAL_UART_Receive_DMA(UART_Handle_T *huart, uint8_t *data, uint16_t size);
HAL_Status_T HAL_UART_DMAPause(UART_Handle_T *huart);
HAL_Status_T HAL_UART_DMAResume(UART_Handle_T *huart);
HAL_Status_T HAL_UART_DMAStop(UART_Handle_T *huart);

/* Transfer Abort functions ***************************************************/
HAL_Status_T HAL_UART_Abort(UART_Handle_T *huart);
HAL_Status_T HAL_UART_AbortTransmit(UART_Handle_T *huart);
HAL_Status_T HAL_UART_AbortReceive(UART_Handle_T *huart);

HAL_Status_T HAL_UART_Abort_IT(UART_Handle_T *huart);
HAL_Status_T HAL_UART_AbortTransmit_IT(UART_Handle_T *huart);
HAL_Status_T HAL_UART_AbortReceive_IT(UART_Handle_T *huart);

/* IRQHandler and Event Callback functions ************************************/
void HAL_UART_IRQHandler(UART_Handle_T *huart);
void HAL_UART_TxCpltCallback(UART_Handle_T *huart);
void HAL_UART_RxCpltCallback(UART_Handle_T *huart);
void HAL_UART_ErrorCallback(UART_Handle_T *huart);
void HAL_UART_AbortCpltCallback(UART_Handle_T *huart);
void HAL_UART_AbortTransmitCpltCallback(UART_Handle_T *huart);
void HAL_UART_AbortReceiveCpltCallback(UART_Handle_T *huart);

#ifdef __cplusplus
}
#endif

#endif /* _HAL_UART_H_ */
