/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _HAL_CAN_H_
#define _HAL_CAN_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sn34f78x_hal_def.h"

/* Exported types ------------------------------------------------------------*/
/**
 * @brief   CAN Configuration Structure definition
 * @ingroup can_struct_define
 */
typedef struct _CAN_Init_T
{
    uint32_t mode;                   /**< Specifies the CAN operating mode.
                                          This parameter can be a value of @ref can_mode */
    uint32_t rx_overflow_mode;       /**< Specifies the receive buffer mode, in case of a full when a new frame is received.
                                          This parameter can be a value of @ref can_rx_overflow_mode. */
    uint32_t pre_scaler;             /**< Specifies the length of a time quantum.
                                          This parameter must be a number between min_data = 0 and max_data = 255.
                                          @note: baudrate = sys_clk / (scaler * (tq_seg1 + tq_seg2))
                                                 sys_clk = HAL_RCC_GetSysClockFreq(),
                                                 scaler  = pre_scaler + 1,
                                                 tq_seg1 = time_seg1 + 2,
                                                 tq_seg2 = time_seg2 + 1. */
    uint32_t sync_jump_width;        /**< Specifies the maximum number of time quanta the CAN hardware
                                          is allowed to lengthen or shorten a bit to perform resynchronization.
                                          This parameter must be a number between min_data = 0 and max_data = 15. */
    uint32_t time_seg1;              /**< Specifies the number of time quanta in Bit Segment 1.
                                          This parameter must be a number between min_data = 1 and max_data = 63. */
    uint32_t time_seg2;              /**< Specifies the number of time quanta in Bit Segment 2.
                                          This parameter must be a number between min_data = 0 and max_data = 7. */
    uint32_t transmit_fifo_priority; /**< Enable or disable the transmit FIFO or priority.
                                          This parameter can be set to HAL_ENABLE or HAL_DISABLE.
                                          @note transmit by fifo or priority decision
                                                HAL_DISABLE: FIFO mode
                                                HAL_ENABLE : Priority decision mode
                                                In FIFO mode frames are transmitted in the order in that they are written into the STB.
                                                In priority decision mode the frame with the highest priority in the STB is automatically transmitted first.
                                                The ID of a frame is used for the priority decision.
                                                A lower ID means a higher priority of a frame.
                                                A frame in the PTB has always the highest priority regardless of the ID */
    uint32_t auto_retransmission;    /*!< Enable or disable the non-automatic retransmission mode.
                                          This parameter can be set to HAL_ENABLE or HAL_DISABLE.
                                          @note
                                                HAL_ENABLE : non-automatic retransmission.
                                                HAL_DISABLE: automatic retransmission. (default)
                                                if error occurred, can bus will retry until transmit is successed. */
    uint32_t self_ack;               /*!< Enable or disable the self-ACK generate.
                                          This parameter can be set to HAL_ENABLE or HAL_DISABLE.
                                          @note
                                                HAL_ENABLE :     generate self-ACK.
                                                HAL_DISABLE: not generate self-ACK.
                                                only useful in mode with CAN_MODE_SILENT_LOOPBACK_EX or CAN_MODE_LOOPBACK_EX. */
} CAN_Init_T;

/**
 * @brief  CAN filter configuration structure definition
 * @ingroup can_struct_define
 */
typedef struct _CAN_Filter_T
{
    uint32_t filter_code;       /**< Specifies the filter identification code.
                                     This parameter must be a number between min_data = 0x0000 and max_data = 0x1FFFFFFF. */
    uint32_t filter_mask;       /**< Specifies the filter identification mask.
                                     This parameter must be a number between min_data = 0x0000 and max_data = 0x1FFFFFFF. */
    uint32_t filter_scale;      /**< Specifies the filter scale.
                                     This parameter can be a value of @ref can_filter_scale */
    uint32_t filter_activation; /**< Enable or disable the filter which be Specified by filter_index.
                                     This parameter can be a value of @ref can_filter_activation */
    uint32_t filter_index;      /**< Specifies the filter index.
                                     This parameter must be a number between min_data = 0x00 and max_data = 0x0F */
} CAN_Filter_T;

/**
 * @brief  CAN Tx message header structure definition
 * @ingroup can_struct_define
 */
typedef struct _CAN_TxHeader_T
{
    uint32_t id;  /**< Specifies the standard or extended identifier.
                       This parameter must be a number between min_data = 0 and max_data = 0x1FFFFFFF. */
    uint32_t ide; /**< Specifies the type of identifier for the message that will be transmitted.
                       This parameter can be a value of @ref can_ide */
    uint32_t rtr; /**< Specifies the type of frame for the message that will be transmitted.
                       This parameter can be a value of @ref can_rtr */
    uint32_t dlc; /**< Specifies the length of the frame that will be transmitted.
                       This parameter must be a number between min_data = 0 and max_data = 8. */
} CAN_TxHeader_T;

/**
 * @brief   CAN Rx message header structure definition
 * @ingroup can_struct_define
 */
typedef struct _CAN_RxHeader_T
{
    uint32_t id;        /**< Specifies the standard or extended identifier.
                             This parameter must be a number between min_data = 0 and max_data = 0x1FFFFFFF. */
    uint32_t ide;       /**< Specifies the type of identifier for the message that will be transmitted.
                             This parameter can be a value of @ref can_ide */
    uint32_t rtr;       /**< Specifies the type of frame for the message that will be transmitted.
                             This parameter can be a value of @ref can_rtr */
    uint32_t dlc;       /**< Specifies the length of the frame that will be transmitted.
                             This parameter must be a number between min_data = 0 and max_data = 8. */
    uint32_t timestamp; /**< Specifies the timestamp counter value captured on start of frame reception.
                             @note: Time Triggered Communication Mode must be enabled.
                             This parameter must be a number between min_data = 0 and max_data = 0xFFFF. */
} CAN_RxHeader_T;

/**
 * @brief CAN Handle Structure Definition
 * @ingroup can_struct_define
 */
typedef struct _CAN_Handle_T
{
    SN_CAN0_Type     *instance;     /**< CAN Register Base Address      */
    CAN_Init_T        init;         /**< CAN Required Parameters        */
    HAL_MUTEX         mutex;        /**< CAN Process Protection Flag    */
    __IOM HAL_State_T state;        /**< CAN State                      */
    __IOM uint32_t    state_ex;     /**< CAN inner state                */
    __IOM uint32_t    error_code;   /**< CAN Error Code                 */
    __IOM uint32_t    notification; /**< CAN Notification Code          */

#if (configUSE_CAN_REGISTER_CALLBACKS == 1U)
    void (*TxPTBCompleteCallback)(struct _CAN_Handle_T *hcan);  /**< CAN Tx PTB complete callback          */
    void (*TxSTBCompleteCallback)(struct _CAN_Handle_T *hcan);  /**< CAN Tx STB complete callback          */
    void (*TxAbortCallback)(struct _CAN_Handle_T *hcan);        /**< CAN Tx FIFO abort callback            */
    void (*RxFifoOneFullCallback)(struct _CAN_Handle_T *hcan);  /**< CAN Rx fifo One  full callback        */
    void (*RxFifoHalfFullCallback)(struct _CAN_Handle_T *hcan); /**< CAN Rx fifo half full callback        */
    void (*RxFifoAllFullCallback)(struct _CAN_Handle_T *hcan);  /**< CAN Rx fifo all  full callback        */
    void (*ErrorCallback)(struct _CAN_Handle_T *hcan);          /**< CAN Error Callback                    */
    void (*MspInitCallback)(struct _CAN_Handle_T *hcan);        /**< CAN MspInit Callback                  */
    void (*MspInitFilterCallback)(struct _CAN_Handle_T *hcan);  /**< CAN MspInit Filter Callback           */
    void (*MspDeInitCallback)(struct _CAN_Handle_T *hcan);      /**< CAN MspDeInit Callback                */
#endif                                                          /* configUSE_CAN_REGISTER_CALLBACKS */
} CAN_Handle_T;

/**
 * @brief HAL CAN Callback ID enumeration definition
 * @ingroup can_struct_define
 */
typedef enum _CAN_CallbackID_T
{
    HAL_CAN_TX_PTB_CPLT_CB_ID     = 0x01U, /**< CAN Tx PTB complete callback ID    */
    HAL_CAN_TX_STB_CPLT_CB_ID     = 0x02U, /**< CAN Tx STB complete callback ID    */
    HAL_CAN_TX_ABORT_CB_ID        = 0x04U, /**< CAN Tx FIFO abort callback ID      */
    HAL_CAN_RX_FIFO_ONE_CB_ID     = 0x05U, /**< CAN Rx FIFO one  callback ID       */
    HAL_CAN_RX_FIFO_HALF_CB_ID    = 0x06U, /**< CAN Rx FIFO half callback ID       */
    HAL_CAN_RX_FIFO_FULL_CB_ID    = 0x07U, /**< CAN Rx FIFO full callback ID       */
    HAL_CAN_ERROR_CB_ID           = 0x12U, /**< CAN Error Callback ID              */
    HAL_CAN_MSP_INIT_CB_ID        = 0x13U, /**< CAN MspInit callback ID            */
    HAL_CAN_MSP_INIT_FILTER_CB_ID = 0x14U, /**< CAN MspInit filter callback ID     */
    HAL_CAN_MSP_DEINIT_CB_ID      = 0x15U  /**< CAN MspDeInit callback ID          */
} CAN_CallbackID_T;

/**
 * @brief HAL CAN Callback pointer definition
 * @ingroup can_struct_define
 */
typedef void (*CAN_Callback_T)(CAN_Handle_T *hcan); /**< pointer to a CAN common callback functions */

/**
 * @ingroup can_struct_define
 * @brief TX mailbox status enumeration definition
 */
typedef enum _CAN_TX_STA_T
{
    HAL_CAN_TX_EMPTY     = (0x00), /**< can tx fifo empty                                    */
    HAL_CAN_TX_LESS_HALF = (0x01), /**< can tx fifo less than half full                      */
    HAL_CAN_TX_MORE_HALF = (0x02), /**< can tx fifo more than half full                      */
    HAL_CAN_TX_FULL      = (0x03), /**< can tx fifo full                                     */
} CAN_TX_STA_T;

/**
 * @brief RX fifo status enumeration definition
 * @ingroup can_struct_define
 */
typedef enum _CAN_RX_STA_T
{
    HAL_CAN_RX_EMPTY     = (0x00), /**< can rx fifo empty                                    */
    HAL_CAN_RX_LESS_HALF = (0x01), /**< can rx fifo less than half full                      */
    HAL_CAN_RX_MORE_HALF = (0x02), /**< can rx fifo more than half full                      */
    HAL_CAN_RX_FULL      = (0x03), /**< can rx fifo full                                     */
} CAN_RX_STA_T;

/**
 * @brief CAN error code enumeration definition
 * @ingroup can_error_code
 */
typedef enum _HAL_CAN_ERROR_CODE_T
{
    HAL_CAN_ERROR_NONE             = (HAL_ERROR_SPECIFY + 0x01), /**< No error                                             */
    HAL_CAN_ERROR_EWG              = (HAL_ERROR_SPECIFY + 0x02), /**< Protocol Error Warning                               */
    HAL_CAN_ERROR_PASSIVE          = (HAL_ERROR_SPECIFY + 0x03), /**< Error Passive                                        */
    HAL_CAN_ERROR_BUS              = (HAL_ERROR_SPECIFY + 0x04), /**< Bus-off error                                        */
    HAL_CAN_ERROR_STF              = (HAL_ERROR_SPECIFY + 0x05), /**< Stuff error                                          */
    HAL_CAN_ERROR_FOR              = (HAL_ERROR_SPECIFY + 0x06), /**< Form error                                           */
    HAL_CAN_ERROR_ACK              = (HAL_ERROR_SPECIFY + 0x07), /**< Acknowledgment error                                 */
    HAL_CAN_ERROR_BR               = (HAL_ERROR_SPECIFY + 0x08), /**< Bit recessive error                                  */
    HAL_CAN_ERROR_BD               = (HAL_ERROR_SPECIFY + 0x09), /**< Bit dominant error                                   */
    HAL_CAN_ERROR_CRC              = (HAL_ERROR_SPECIFY + 0x0A), /**< CRC error                                            */
    HAL_CAN_ERROR_RX_OVERFLOW      = (HAL_ERROR_SPECIFY + 0x0B), /**< Rx FIFO overrun error                                */
    HAL_CAN_ERROR_TIME_TRIGGER     = (HAL_ERROR_SPECIFY + 0x0C), /**< Tx Time Trigger Error                                */
    HAL_CAN_ERROR_TX_ALST          = (HAL_ERROR_SPECIFY + 0x0D), /**< Tx FIFO transmit failure due to arbitration lost     */
    HAL_CAN_ERROR_TX_TERR0         = (HAL_ERROR_SPECIFY + 0x0E), /**< Tx FIFO transmit failure due to transmit error       */
    HAL_CAN_ERROR_TIMEOUT          = (HAL_ERROR_SPECIFY + 0x0F), /**< Timeout error                                        */
    HAL_CAN_ERROR_NOT_INITIALIZED  = (HAL_ERROR_SPECIFY + 0x10), /**< Peripheral not initialized                           */
    HAL_CAN_ERROR_NOT_READY        = (HAL_ERROR_SPECIFY + 0x11), /**< Peripheral not ready                                 */
    HAL_CAN_ERROR_STARTED          = (HAL_ERROR_SPECIFY + 0x12), /**< Peripheral is started                                */
    HAL_CAN_ERROR_NOT_STARTED      = (HAL_ERROR_SPECIFY + 0x13), /**< Peripheral not started                               */
    HAL_CAN_ERROR_PARAM            = (HAL_ERROR_SPECIFY + 0x14), /**< Parameter error                                      */
    HAL_CAN_ERROR_INVALID_CALLBACK = (HAL_ERROR_SPECIFY + 0x15), /**< Invalid Callback error                               */
    HAL_CAN_ERROR_INTERNAL         = (HAL_ERROR_SPECIFY + 0x16), /**< Internal error                                       */
    HAL_CAN_ERROR_TX_SLEEP         = (HAL_ERROR_SPECIFY + 0x17), /**< Transmit error when sleep mode                       */
} HAL_CAN_ERROR_CODE_T;

/* Exported constants --------------------------------------------------------*/
/**
 * @defgroup can_mode CAN Operating Mode
 * @ingroup  can_control
 * @{
 */
#define CAN_MODE_NORMAL             (0U) /**< Normal mode, keep ACK to bus                     */
#define CAN_MODE_SILENT             (1U) /**< Silent mode, not  ACK to bus                     */
#define CAN_MODE_LOOPBACK_IN        (2U) /**< Loopback internal mode, cut  connection with bus */
#define CAN_MODE_LOOPBACK_EX        (3U) /**< Loopback external mode, keep connection with bus */
#define CAN_MODE_SILENT_LOOPBACK_EX (4U) /**< Loopback external combined with silent mode      */
/**
 * @}
 */

/**
 * @defgroup can_rx_overflow_mode CAN Overflow Mode
 * @ingroup  can_control
 * @{
 */
#define CAN_RX_OV_STORE_NEW_MSG (0U) /**< Save new msg, The oldest frame will be overwritten */
#define CAN_RX_OV_ABORT_NEW_MSG (1U) /**< Keep old msg, The new frame will not be stored     */
/**
 * @}
 */

/**
 * @defgroup can_filter_scale CAN Filter Scale
 * @ingroup  can_control
 * @{
 */
#define CAN_FILTER_SCALE_STD  (0x00000000U) /**< only standard 11-bit filter                    */
#define CAN_FILTER_SCALE_EXT  (0x00000001U) /**< only extended 29-bit filter                    */
#define CAN_FILTER_SCALE_BOTH (0x00000002U) /**< both standard 11-bit & extended 29-bit filter  */
/**
 * @}
 */

/**
 * @defgroup can_filter_activation CAN Filter Activation
 * @ingroup  can_control
 * @{
 */
#define CAN_FILTER_DISABLE (0x00000000U) /**< Disable filter */
#define CAN_FILTER_ENABLE  (0x00000001U) /**< Enable filter  */
/**
 * @}
 */

/**
 * @defgroup can_ide CAN Identifier Type
 * @ingroup  can_control
 * @{
 */
#define CAN_ID_STD (0x00000000U) /**< Standard Id */
#define CAN_ID_EXT (0x00000001U) /**< Extended Id */
/**
 * @}
 */

/**
 * @defgroup can_timing CAN Timing Parameter Range
 * @ingroup  can_control
 * @{
 */
#define CAN_TIMING_PRESCALER_MAX (0x000000FFU) /**< Prescaler max value */
#define CAN_TIMING_SEG1_MIN      (0x00000001U) /**< Seg1 min value */
#define CAN_TIMING_SEG1_MAX      (0x0000003FU) /**< Seg1 max value */
#define CAN_TIMING_SEG2_MAX      (0x00000007U) /**< Seg2 max value */
#define CAN_TIMING_SJW_MAX       (0x0000000FU) /**< SJW max value */
/**
 * @}
 */

/**
 * @defgroup can_rtr CAN Remote Transmission Request
 * @ingroup  can_control
 * @{
 */
#define CAN_RTR_DATA   (0x00000000U) /**< Data frame   */
#define CAN_RTR_REMOTE (0x00000001U) /**< Remote frame */
/**
 * @}
 */

/**
 * @defgroup can_mailbox CAN Transmission buffer choice
 * @ingroup  can_control
 * @{
 */
#define CAN_MAILBOX_PTB (0x00000000U) /**< MAILBOX use Primary Transmission Buffer   */
#define CAN_MAILBOX_STB (0x00000001U) /**< MAILBOX use Secondary Transmission Buffer */
/**
 * @}
 */

/**
 * @defgroup can_interrupts CAN Interrupts
 * @ingroup  can_control
 * @{
 */
#define CAN_IT_TX_PTB           (0x00000001U) /**< TX PTB   complete interrupt          */
#define CAN_IT_TX_STB           (0x00000002U) /**< TX STB   complete interrupt          */
#define CAN_IT_RX_ONE           (0x00000008U) /**< RX one  FIFO full interrupt          */
#define CAN_IT_RX_HALF          (0x00000010U) /**< RX half FIFO full interrupt          */
#define CAN_IT_RX_FULL          (0x00000020U) /**< RX all  FIFO full interrupt          */
#define CAN_IT_ERROR_RX_OVERRUN (0x00000040U) /**< Error RX overrun interrupt           */
#define CAN_IT_ERROR_PASSIVE    (0x00000100U) /**< Error passive interrupt              */
#define CAN_IT_ERROR_BUS        (0x00000200U) /**< Error Bus interrupt                  */
#define CAN_IT_ERROR_AL         (0x00000400U) /**< Error Arbitration Lost interrupt     */
#define CAN_IT_ERROR            (0x00000800U) /**< Error Interrupt                      */
/**
 * @}
 */

/* Exported functions --------------------------------------------------------*/
/* Initialization and de-initialization functions *****************************/
HAL_Status_T HAL_CAN_Init(CAN_Handle_T *hcan);
HAL_Status_T HAL_CAN_DeInit(CAN_Handle_T *hcan);
void         HAL_CAN_MspInit(CAN_Handle_T *hcan);
void         HAL_CAN_MspDeInit(CAN_Handle_T *hcan);
void         HAL_CAN_MspInitFilter(CAN_Handle_T *hcan);

/* Callbacks Register/UnRegister functions  ***********************************/
HAL_Status_T HAL_CAN_RegisterCallback(CAN_Handle_T *hcan, CAN_CallbackID_T cb_id, CAN_Callback_T pcb);
HAL_Status_T HAL_CAN_UnRegisterCallback(CAN_Handle_T *hcan, CAN_CallbackID_T cb_id);

/* Configuration functions ****************************************************/
HAL_Status_T HAL_CAN_ConfigFilter(CAN_Handle_T *hcan, CAN_Filter_T *filter); // Only using in HAL_CAN_MspInitFilter or MspInitFilterCallback

/* Control functions **********************************************************/
HAL_Status_T HAL_CAN_RequestSleep(CAN_Handle_T *hcan);
HAL_Status_T HAL_CAN_WakeUp(CAN_Handle_T *hcan);
HAL_Status_T HAL_CAN_IsSleepActive(CAN_Handle_T *hcan);

HAL_Status_T HAL_CAN_Start(CAN_Handle_T *hcan);
HAL_Status_T HAL_CAN_Stop(CAN_Handle_T *hcan);

HAL_Status_T HAL_CAN_AddTxMessage(CAN_Handle_T *hcan, CAN_TxHeader_T *tx_header, uint8_t *data, uint32_t mailbox);
CAN_TX_STA_T HAL_CAN_GetTxMailboxesStatus(CAN_Handle_T *hcan, uint32_t mailbox);
HAL_Status_T HAL_CAN_AbortTxRequest(CAN_Handle_T *hcan, uint32_t mailbox);

HAL_Status_T HAL_CAN_GetRxMessage(CAN_Handle_T *hcan, CAN_RxHeader_T *rx_header, uint8_t *data);
CAN_RX_STA_T HAL_CAN_GetRxFifoStatus(CAN_Handle_T *hcan);

/* Interrupts management ******************************************************/
HAL_Status_T HAL_CAN_ActivateNotification(CAN_Handle_T *hcan, uint32_t active_its);
HAL_Status_T HAL_CAN_DeactivateNotification(CAN_Handle_T *hcan, uint32_t inactive_its);
void         HAL_CAN_TX_IRQHandler(CAN_Handle_T *hcan);
void         HAL_CAN_RX_IRQHandler(CAN_Handle_T *hcan);
void         HAL_CAN_EW_IRQHandler(CAN_Handle_T *hcan);
// void         HAL_CAN_TT_IRQHandler(CAN_Handle_T *hcan);

/* Peripheral State and Error functions ***************************************/
HAL_State_T  HAL_CAN_GetState(CAN_Handle_T *hcan);
uint32_t     HAL_CAN_GetError(CAN_Handle_T *hcan);
HAL_Status_T HAL_CAN_ResetError(CAN_Handle_T *hcan);

/* Callbacks functions ********************************************************/
void HAL_CAN_TxPTBCompleteCallback(CAN_Handle_T *hcan);
void HAL_CAN_TxSTBCompleteCallback(CAN_Handle_T *hcan);
void HAL_CAN_TxAbortCallback(CAN_Handle_T *hcan); // abort both PTB or STB

void HAL_CAN_RxFifoOneFullCallback(CAN_Handle_T *hcan);
void HAL_CAN_RxFifoHalfFullCallback(CAN_Handle_T *hcan);
void HAL_CAN_RxFifoAllFullCallback(CAN_Handle_T *hcan);

void HAL_CAN_ErrorCallback(CAN_Handle_T *hcan);

#ifdef __cplusplus
}
#endif
#endif /* _HAL_CAN_H_ */
