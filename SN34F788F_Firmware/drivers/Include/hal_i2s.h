/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _HAL_I2S_H_
#define _HAL_I2S_H_
#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sn34f78x_hal_def.h"

/* Exported types ------------------------------------------------------------*/
/**
 * \brief   I2S init structure definition
 * \ingroup i2s_struct_define
 */
typedef struct _I2S_Init_T
{
    uint32_t mode;        /**< Specifies the I2S operating mode.
                               This parameter can be a value of @ref i2s_mode */
    uint32_t standard;    /**< Specifies the standard used for the I2S communication.
                               This parameter can be a value of @ref i2s_standard */
    uint32_t channel;     /**< Specifies the Mono/Stereo channel selection.
                               This parameter can be a value of @ref i2s_channel */
    uint32_t data_format; /**< Specifies the data length for the I2S communication.
                               This parameter can be a value in range of @ref i2s_data_format.*/
    uint32_t channel_len; /**< Specifies the channel length for the I2S communication.
                               This parameter must be an number between min_data = 16 and max_data = 32.
                               @note Must channel_len >= data_len  */
    uint32_t audio_freq;  /**< Specifies the I2S audio frequency.
                               This parameter can be a value of @ref i2s_audio_freq */
    uint32_t cpol;        /**< Specifies the idle state of the I2S clock.
                               This parameter can be a value of @ref i2s_cpol */
} I2S_Init_T;

/**
 * \brief   I2S tx runtime information
 * \ingroup i2s_struct_define
 */
typedef struct _I2S_TX_T
{
    const uint8_t *buffer_ptr; /**< Pointer to I2S tx transfer buffer */
    uint16_t       xfer_size;  /**< I2S tx transfer size              */
    __IOM uint16_t xfer_count; /**< I2S tx transfer counter           */
    uint16_t       frame_num;  /**< I2S tx frame number               */
    __IOM uint8_t  stop;       /**< I2S tx stop flag                  */
} I2S_TX_T;

/**
 * \brief   I2S rx runtime information
 * \ingroup i2s_struct_define
 */
typedef struct _I2S_RX_T
{
    uint8_t       *buffer_ptr; /**< Pointer to I2S rx transfer buffer */
    uint16_t       xfer_size;  /**< I2S rx transfer size              */
    __IOM uint16_t xfer_count; /**< I2S rx transfer counter           */
    uint16_t       frame_num;  /**< I2S rx frame number               */
    uint32_t       temp;       /**< I2S rx temp data                  */
} I2S_RX_T;

/**
 * \arg item-description I2S handle structure definition
 * \ingroup i2s_struct_define
 */
typedef struct _I2S_Handle_T
{
    SN_SPI0_Type     *instance;                                          /**< I2S register base address   */
    I2S_Init_T        init;                                              /**< I2S required parameters     */
    HAL_MUTEX         mutex;                                             /**< I2S process protection flag */
    I2S_TX_T          tx;                                                /**< I2S tx runtime info         */
    I2S_RX_T          rx;                                                /**< I2S rx runtime info         */
    __IOM HAL_State_T state;                                             /**< I2S state                   */
    __IOM HAL_State_T rx_state;                                          /**< I2S rx state                */
    __IOM uint32_t    error_code;                                        /**< I2S error code              */
    void (*I2S_DataTxHandler)(struct _I2S_Handle_T *hi2s, uint16_t cnt); /**< I2S Tx Handler */
    void (*I2S_DataRxHandler)(struct _I2S_Handle_T *hi2s, uint16_t cnt); /**< I2S Rx Handler */

#if (configUSE_I2S_NONBLOCKING_DMA == 1U)
    DMA_Handle_T *hdmatx; /**< I2S tx DMA Handle parameters */
    DMA_Handle_T *hdmarx; /**< I2S rx DMA Handle parameters */
#endif

#if (configUSE_I2S_REGISTER_CALLBACKS == 1U)
    void (*TxCpltCallback)(struct _I2S_Handle_T *hi2s);    /**< I2S tx complete callback */
    void (*RxCpltCallback)(struct _I2S_Handle_T *hi2s);    /**< I2S rx complete callback */
    void (*ErrorCallback)(struct _I2S_Handle_T *hi2s);     /**< I2S error callback       */
    void (*MspInitCallback)(struct _I2S_Handle_T *hi2s);   /**< I2S mspInit callback     */
    void (*MspDeInitCallback)(struct _I2S_Handle_T *hi2s); /**< I2S mspDeInit callback   */
#endif
} I2S_Handle_T;

/**
 * \brief I2S callback ID enumeration definition
 * \ingroup i2s_struct_define
 */
typedef enum _HAL_I2S_CallbackID_T
{
    HAL_I2S_TX_COMPLETE_CB_ID = 0x00U, /**< I2S tx complete callback ID */
    HAL_I2S_RX_COMPLETE_CB_ID = 0x01U, /**< I2S rx complete callback ID */
    HAL_I2S_ERROR_CB_ID       = 0x02U, /**< I2S error callback ID       */
    HAL_I2S_MSP_INIT_CB_ID    = 0x10U, /**< I2S mspInit callback ID     */
    HAL_I2S_MSP_DEINIT_CB_ID  = 0x11U  /**< I2S mspDeInit callback ID   */
} HAL_I2S_CallbackID_T;

/**
 * \brief I2S callback pointer definition
 * \ingroup i2s_struct_define
 */
typedef void (*I2S_Callback_T)(I2S_Handle_T *hi2s); /**< pointer to a I2S common callback functions */

/* Exported constants --------------------------------------------------------*/
/**
 * \defgroup i2s_mode I2S operating mode
 * \ingroup i2s_control
 * @{
 */
#define I2S_MODE_SLAVE  (0x00U) /**< slave  mode */
#define I2S_MODE_MASTER (0x01U) /**< master mode */
/**
 * @}
 */

/**
 * \defgroup i2s_standard I2S standard
 * \ingroup i2s_control
 * @{
 */
#define I2S_STANDARD_PHILIPS (0x00U) /**< philips protocol      */
#define I2S_STANDARD_MSB     (0x01U) /**< MSB (left)  justified */
#define I2S_STANDARD_LSB     (0x02U) /**< LSB (right) justified */
/**
 * @}
 */

/**
 * \defgroup i2s_channel I2S channel type
 * \ingroup i2s_control
 * @{
 */
#define I2S_CHANNEL_MONO   (0x00U) /**< channel type:   mono */
#define I2S_CHANNEL_STEREO (0x01U) /**< channel type: stereo */
/**
 * @}
 */

/**
 * \defgroup i2s_data_format I2S data format
 * \ingroup i2s_control
 * @{
 */
#define I2S_DATA_LEN_8B  (8U)  /**< data format:  8 bits */
#define I2S_DATA_LEN_16B (16U) /**< data format: 16 bits */
#define I2S_DATA_LEN_24B (24U) /**< data format: 24 bits */
#define I2S_DATA_LEN_32B (32U) /**< data format: 32 bits */
/**
 * @}
 */

/**
 * \defgroup i2s_audio_freq I2S audio frequency
 * \ingroup i2s_control
 * @{
 */
#define I2S_AUDIO_FREQ_16K (16000U) /**< audio freq: 16k */
#define I2S_AUDIO_FREQ_48K (48000U) /**< audio freq: 48k */
/**
 * @}
 */

/**
 * \defgroup i2s_cpol I2S cpol
 * \ingroup i2s_control
 * @{
 */
#define I2S_CPOL_HIGH (0x00) /**< Active high, Idle low  */
#define I2S_CPOL_LOW  (0x01) /**< Active low , Idle high */
/**
 * @}
 */

/**
 * \ingroup i2s_error_code
 * @{
 */
#define HAL_I2S_ERROR_INVALID_CALLBACK (HAL_ERROR_SPECIFY + 0x01U) /**< invalid callback error */
#define HAL_I2S_ERROR_DMA              (HAL_ERROR_SPECIFY + 0x02U) /**< I2S DMA error          */
#define HAL_I2S_ERROR_DMA_RX           (HAL_ERROR_SPECIFY + 0x03U) /**< rx DMA channel error   */
#define HAL_I2S_ERROR_DMA_TX           (HAL_ERROR_SPECIFY + 0x04U) /**< tx DMA channel error   */
#define HAL_I2S_ERROR_RX_OVERFLOW      (HAL_ERROR_SPECIFY + 0x05U) /**< rx fifo overflow       */
#define HAL_I2S_ERROR_TX_UNDERFLOW     (HAL_ERROR_SPECIFY + 0x06U) /**< tx fifo underflow      */
/**
 * @}
 */

/* Exported functions --------------------------------------------------------*/
/* Initialization/De-initialization functions *********************************/
HAL_Status_T HAL_I2S_Init(I2S_Handle_T *hi2s);
HAL_Status_T HAL_I2S_DeInit(I2S_Handle_T *hi2s);
void         HAL_I2S_MspInit(I2S_Handle_T *hi2s);
void         HAL_I2S_MspDeInit(I2S_Handle_T *hi2s);

/* Callbacks Register/UnRegister functions ************************************/
HAL_Status_T HAL_I2S_RegisterCallback(I2S_Handle_T *hi2s, HAL_I2S_CallbackID_T callback_id, I2S_Callback_T callback);
HAL_Status_T HAL_I2S_UnRegisterCallback(I2S_Handle_T *hi2s, HAL_I2S_CallbackID_T callback_id);

/* Peripheral State and Error functions **************************************/
HAL_State_T HAL_I2S_GetState(I2S_Handle_T *hi2s);
uint32_t    HAL_I2S_GetError(I2S_Handle_T *hi2s);

/* I/O Operation functions ****************************************************/
/******* Blocking mode: Polling */
HAL_Status_T HAL_I2S_Transmit(I2S_Handle_T *hi2s, uint8_t *data, uint16_t size, uint32_t timeout);
HAL_Status_T HAL_I2S_Receive(I2S_Handle_T *hi2s, uint8_t *data, uint16_t size, uint32_t timeout);
/******* Non-Blocking mode: Interrupt */
HAL_Status_T HAL_I2S_Transmit_IT(I2S_Handle_T *hi2s, uint8_t *data, uint16_t size);
HAL_Status_T HAL_I2S_Receive_IT(I2S_Handle_T *hi2s, uint8_t *data, uint16_t size);
HAL_Status_T HAL_I2S_Stop(I2S_Handle_T *hi2s);
/******* Non-Blocking mode: DMA */
HAL_Status_T HAL_I2S_Transmit_DMA(I2S_Handle_T *hi2s, uint8_t *data, uint16_t size);
HAL_Status_T HAL_I2S_Receive_DMA(I2S_Handle_T *hi2s, uint8_t *data, uint16_t size);
HAL_Status_T HAL_I2S_DMAPause(I2S_Handle_T *hi2s);
HAL_Status_T HAL_I2S_DMAResume(I2S_Handle_T *hi2s);
HAL_Status_T HAL_I2S_DMAStop(I2S_Handle_T *hi2s);

/* IRQHandler and Event Callback functions ************************************/
void HAL_I2S_IRQHandler(I2S_Handle_T *hi2s);
void HAL_I2S_TxCpltCallback(I2S_Handle_T *hi2s);
void HAL_I2S_RxCpltCallback(I2S_Handle_T *hi2s);
void HAL_I2S_ErrorCallback(I2S_Handle_T *hi2s);

#ifdef __cplusplus
}
#endif
#endif /* _HAL_I2S_H_ */
