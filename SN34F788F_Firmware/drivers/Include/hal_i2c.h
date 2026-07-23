/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _HAL_I2C_H_
#define _HAL_I2C_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sn34f78x_hal_def.h"
#include "hal_i2c_ex.h"

/* Exported types ------------------------------------------------------------*/
/**
 * \brief I2C Init Structure Definition
 * \ingroup i2c_struct_define
 */
typedef struct
{
    uint32_t clock_speed;     /**< Specifies the clock frequency.
                                   This parameter can be a value of @ref i2c_clock_speed */
    uint32_t own_address;     /**< Specifies the device own address.
                                   This parameter can be a 7-bit or 10-bit address. */
    uint32_t addressing_mode; /**< Specifies if 7-bit or 10-bit addressing mode is selected.
                                   This parameter can be a value of @ref i2c_addressing_mode */
    uint32_t gc_en;           /**< Specifies if general call is enabled.
                                   This parameter can be a value of @ref HAL_ENABLE or @ref HAL_DISABLE. */
} I2C_Init_T;

/**
 * \brief I2C Burst Structure Definition
 * \ingroup i2c_struct_define
 */
typedef struct
{
    uint8_t index1;    /**< Specifies the index1 value in burst mode.
                            The index1 will be transmitted before the transfer of data. */
    uint8_t index2;    /**< Specifies the index2 value in burst mode.
                            The index2 will be transmitted before the transfer of data. */
    uint8_t index2_en; /**< Specifies if use the index2 in burst mode.
                            This parameter can be a value of @ref HAL_ENABLE or @ref HAL_DISABLE. */
} I2C_Burst_T;

/**
 * \brief I2C Mode Enumeration Definition
 * \ingroup i2c_struct_define
 * @{
 */
typedef enum
{
    HAL_I2C_MODE_NONE   = 0x00U, /**< No I2C communication on going */
    HAL_I2C_MODE_MASTER = 0x01U, /**< I2C communication is in Master Mode */
    HAL_I2C_MODE_SLAVE  = 0x02U, /**< I2C communication is in Slave Mode */
} I2C_Mode_T;
/**
 * @}
 */

/**
 * \brief I2C Handle Structure Definition
 * \ingroup i2c_struct_define
 */
typedef struct __I2C_Handle_T
{
    SN_I2C0_Type     *instance;   /**< Register Base Address */
    I2C_Init_T        init;       /**< I2C Required Parameters */
    HAL_MUTEX         mutex;      /**< I2C Process Protection */
    __IOM HAL_State_T state;      /**< I2C State */
    __IOM I2C_Mode_T  mode;       /**< I2C Mode */
    __IOM uint32_t    error_code; /**< I2C Error Code */

    uint8_t       *buffer_ptr;    /**< Pointer to I2C Transfer Buffer */
    uint16_t       xfer_size;     /**< I2C Transfer Size */
    __IOM uint16_t xfer_count;    /**< I2C Transfer Counter */
    __IOM uint32_t direction : 4; /**< I2C Transfer Direction */
    __IOM uint16_t own_address;   /**< I2C Transfer Own Address */
    __IOM uint16_t dev_address;   /**< I2C Transfer Slave Address */
    __IOM uint32_t xfer_option;   /**< I2C Transfer Option */
    __IOM uint32_t xfer_state;    /**< I2C Transfer State */
    __IOM uint32_t abort : 1;     /**< I2C Transfer Abort Flag */
    I2CEx_XFER_T   ext_xfer;      /**< I2C Extended Transfer Data */

#if (configUSE_I2C_REGISTER_CALLBACKS == 1)
    void (*MasterTxCpltCallback)(struct __I2C_Handle_T *hi2c); /**< I2C Master Tx Transfer completed callback */
    void (*MasterRxCpltCallback)(struct __I2C_Handle_T *hi2c); /**< I2C Master Rx Transfer completed callback */
    void (*SlaveTxCpltCallback)(struct __I2C_Handle_T *hi2c);  /**< I2C Slave Tx Transfer completed callback  */
    void (*SlaveRxCpltCallback)(struct __I2C_Handle_T *hi2c);  /**< I2C Slave Rx Transfer completed callback  */
    void (*AbortCpltCallback)(struct __I2C_Handle_T *hi2c);    /**< I2C Abort callback                        */
    void (*ErrorCallback)(struct __I2C_Handle_T *hi2c);        /**< I2C Error callback                        */
    void (*MspInitCallback)(struct __I2C_Handle_T *hi2c);      /**< I2C Msp Init callback                     */
    void (*MspDeInitCallback)(struct __I2C_Handle_T *hi2c);    /**< I2C Msp DeInit callback                   */
#endif
} I2C_Handle_T;

/**
 * \brief  HAL I2C Callback ID enumeration definition
 * \ingroup i2c_struct_define
 */
typedef enum
{
    HAL_I2C_MASTER_TX_COMPLETE_CB_ID = 0x00U, /**< I2C Master Tx Transfer completed callback ID  */
    HAL_I2C_MASTER_RX_COMPLETE_CB_ID = 0x01U, /**< I2C Master Rx Transfer completed callback ID  */
    HAL_I2C_SLAVE_TX_COMPLETE_CB_ID  = 0x02U, /**< I2C Slave Tx Transfer completed callback ID   */
    HAL_I2C_SLAVE_RX_COMPLETE_CB_ID  = 0x03U, /**< I2C Slave Rx Transfer completed callback ID   */
    HAL_I2C_ABORT_CB_ID              = 0x04U, /**< I2C Abort callback ID                         */
    HAL_I2C_ERROR_CB_ID              = 0x05U, /**< I2C Error callback ID                         */
    HAL_I2C_MSPINIT_CB_ID            = 0x10U, /**< I2C Msp Init callback ID                      */
    HAL_I2C_MSPDEINIT_CB_ID          = 0x11U  /**< I2C Msp DeInit callback ID                    */
} HAL_I2C_CallbackID_T;

/**
 * \brief  HAL I2C Callback pointer definition
 * \ingroup i2c_struct_define_gr
 */
typedef void (*I2C_Callback_T)(I2C_Handle_T *hi2c); /**< pointer to an I2C callback function */

/* Exported constants --------------------------------------------------------*/
/**
 * \defgroup i2c_clock_speed I2C Clock Speed
 * \ingroup i2c_control
 * @{
 */
#define I2C_CLOCKSPEED_STANDARD   0x00000000U /**< I2C works in Standard Mode (100KHz) */
#define I2C_CLOCKSPEED_FAST       0x00000001U /**< I2C works in Fast Mode (400KHz) */
#define I2C_CLOCKSPEED_FAST_PLUS  0x00000002U /**< I2C works in Fast+ Mode (1MHz) */
#define I2C_CLOCKSPEED_HIGH_SPEED 0x00000003U /**< I2C works in High Speed Mode (3MHz) */
/**
 * @}
 */

/**
 * \defgroup i2c_addressing_mode I2C Addressing Mode
 * \ingroup i2c_control
 * @{
 */
#define I2C_ADDRESSINGMODE_7BIT  0x00000000U /**< The device address is under 7 bits */
#define I2C_ADDRESSINGMODE_10BIT 0x00000001U /**< The device address is under 10 bits */
/**
 * @}
 */

/**
 * \defgroup i2c_sequential_option I2C Sequential Option
 * \ingroup i2c_control
 * @{
 */
#define I2C_FIRST_AND_LAST_FRAME 0x00000017U         /**< No sequential usage, \
                                                          functional is same as associated interfaces in no sequential mode */
#define I2C_FIRST_FRAME 0x00000011U                  /**< Sequential usage, this option allow to manage a sequence with start condition, \
                                                          address and data to transfer without a final stop condition */
#define I2C_NEXT_FRAME 0x00000010U                   /**< Sequential usage, this option allow to manage only the new data to transfer \
                                                          and without a final stop */
#define I2C_LAST_FRAME_NO_STOP 0x00000012U           /**< Sequential usage, this option allow to manage the new data to transfer             \
                                                          and without a final stop condition. Then usage of this option at the last Transmit \
                                                          or Receive sequence permit to call the opposite interface Receive or Transmit      \
                                                          without stopping the communication and so generate a restart condition */
#define I2C_LAST_FRAME 0x00000016U                   /**< Sequential usage, this option allow to manage the new data to transfer          \
                                                          and with a final stop condition. Then usage of this option at the last Transmit \
                                                          or Receive sequence permit to call the opposite interface Receive or Transmit   \
                                                          stopping the communication and so generate a restart condition */
#define I2C_FIRST_AND_LAST_FRAME_NO_STOP 0x00000013U /**< Sequential usage, this option allow to manage a sequence with start condition,        \
                                                          address and data to transfer without a final stop condition. Then usage of this       \
                                                          option at the last Transmit or Receive sequence permit to call the opposite interface \
                                                          Receive or Transmit without stopping the communication and so generate a restart condition */
/**
 * @}
 */
/*
 * +——————————————————————————————————————————————————————————————————————+
 * |         Transfer Option          |   Start   | NACK (RX) |   Stop    |
 * |——————————————————————————————————————————————————————————————————————|
 * | I2C_FIRST_AND_LAST_FRAME         |     V     |     V     |     V     |
 * | I2C_FIRST_FRAME                  |     V     |     -     |     -     |
 * | I2C_NEXT_FRAME                   |     -     |     -     |     -     |
 * | I2C_LAST_FRAME_NO_STOP           |     -     |     V     |     -     |
 * | I2C_LAST_FRAME                   |     -     |     V     |     V     |
 * | I2C_FIRST_AND_LAST_FRAME_NO_STOP |     V     |     V     |     -     |
 * +——————————————————————————————————————————————————————————————————————+
 */

/**
 * \ingroup i2c_error_code
 * @{
 */
#define HAL_I2C_ERROR_INVALID_CALLBACK  (HAL_ERROR_SPECIFY + 1U) /**< Invalid Callback Error */
#define HAL_I2C_ERROR_CLK               (HAL_ERROR_SPECIFY + 2U) /**< Unable to set the I2C Clock Speed with current APB1 Clock */
#define HAL_I2C_ERROR_AF                (HAL_ERROR_SPECIFY + 3U) /**< Acknowledge Failure Error */
#define HAL_I2C_ERROR_ARLO              (HAL_ERROR_SPECIFY + 4U) /**< Arbitration Lost Error */
#define HAL_I2C_ERROR_UNEXPECTED_STATUS (HAL_ERROR_SPECIFY + 5U) /**< Unexpected Status Error */
/**
 * @}
 */

/* Exported macros -----------------------------------------------------------*/
#define I2C_GET_TRANSFERRED_COUNT(__HANDLE__)             ((__HANDLE__)->xfer_size - (__HANDLE__)->xfer_count)
#define I2C_EXT_GET_CURRENT_TRANSMITTED_COUNT(__HANDLE__) ((__HANDLE__)->ext_xfer.tx_xfer_num)
#define I2C_EXT_GET_TOTAL_TRANSMITTED_COUNT(__HANDLE__)   ((__HANDLE__)->ext_xfer.tx_xfer_size - (__HANDLE__)->ext_xfer.tx_xfer_count)
#define I2C_EXT_GET_CURRENT_RECEIVED_COUNT(__HANDLE__)    ((__HANDLE__)->ext_xfer.rx_xfer_num)
#define I2C_EXT_GET_TOTAL_RECEIVED_COUNT(__HANDLE__)      ((__HANDLE__)->ext_xfer.rx_xfer_size - (__HANDLE__)->ext_xfer.rx_xfer_count)

/* Exported functions --------------------------------------------------------*/
/* Initialization and de-initialization functions *****************************/
HAL_Status_T HAL_I2C_Init(I2C_Handle_T *hi2c);
HAL_Status_T HAL_I2C_DeInit(I2C_Handle_T *hi2c);
void         HAL_I2C_MspInit(I2C_Handle_T *hi2c);
void         HAL_I2C_MspDeInit(I2C_Handle_T *hi2c);

/* Callbacks Register/UnRegister functions ************************************/
HAL_Status_T HAL_I2C_RegisterCallback(I2C_Handle_T *hi2c, HAL_I2C_CallbackID_T callback_id, I2C_Callback_T callback);
HAL_Status_T HAL_I2C_UnRegisterCallback(I2C_Handle_T *hi2c, HAL_I2C_CallbackID_T callback_id);

/* Peripheral State, Mode and Error functions *********************************/
HAL_State_T HAL_I2C_GetState(I2C_Handle_T *hi2c);
I2C_Mode_T  HAL_I2C_GetMode(I2C_Handle_T *hi2c);
uint32_t    HAL_I2C_GetError(I2C_Handle_T *hi2c);

/* IO operation functions *****************************************************/
/******* Blocking mode: Polling */
HAL_Status_T HAL_I2C_Master_Transmit(I2C_Handle_T *hi2c, uint16_t dev_address, uint8_t *data, uint16_t size, uint32_t timeout);
HAL_Status_T HAL_I2C_Master_Receive(I2C_Handle_T *hi2c, uint16_t dev_address, uint8_t *data, uint16_t size, uint32_t timeout);
HAL_Status_T HAL_I2C_Slave_Transmit(I2C_Handle_T *hi2c, uint8_t *data, uint16_t size, uint32_t timeout);
HAL_Status_T HAL_I2C_Slave_Receive(I2C_Handle_T *hi2c, uint8_t *data, uint16_t size, uint32_t timeout);
/******* Non-Blocking mode: Interrupt */
HAL_Status_T HAL_I2C_Master_Transmit_IT(I2C_Handle_T *hi2c, uint16_t dev_address, uint8_t *data, uint16_t size);
HAL_Status_T HAL_I2C_Master_Receive_IT(I2C_Handle_T *hi2c, uint16_t dev_address, uint8_t *data, uint16_t size);
HAL_Status_T HAL_I2C_Slave_Transmit_IT(I2C_Handle_T *hi2c, uint8_t *data, uint16_t size);
HAL_Status_T HAL_I2C_Slave_Receive_IT(I2C_Handle_T *hi2c, uint8_t *data, uint16_t size);
HAL_Status_T HAL_I2C_Master_Seq_Transmit_IT(I2C_Handle_T *hi2c, uint16_t dev_address, uint8_t *data, uint16_t size, uint32_t xfer_option);
HAL_Status_T HAL_I2C_Master_Seq_Receive_IT(I2C_Handle_T *hi2c, uint16_t dev_address, uint8_t *data, uint16_t size, uint32_t xfer_option);
HAL_Status_T HAL_I2C_Slave_Seq_Transmit_IT(I2C_Handle_T *hi2c, uint8_t *data, uint16_t size, uint32_t xfer_option);
HAL_Status_T HAL_I2C_Slave_Seq_Receive_IT(I2C_Handle_T *hi2c, uint8_t *data, uint16_t size, uint32_t xfer_option);
HAL_Status_T HAL_I2C_Master_Abort_IT(I2C_Handle_T *hi2c);
/******* Extended Interrupt APIs */
HAL_Status_T HAL_I2CEx_Slave_Transfer_IT(I2C_Handle_T *hi2c, uint8_t *tx_data, uint16_t tx_size, uint8_t *rx_data, uint16_t rx_size);
HAL_Status_T HAL_I2CEx_Slave_SetTXBuffer(I2C_Handle_T *hi2c, uint8_t *tx_data, uint16_t tx_size);
HAL_Status_T HAL_I2CEx_Slave_SetRXBuffer(I2C_Handle_T *hi2c, uint8_t *rx_data, uint16_t rx_size);
/******* Non-Blocking mode: Burst */
HAL_Status_T HAL_I2C_Master_Transmit_BURST(I2C_Handle_T *hi2c, uint16_t dev_address, uint8_t *data, uint8_t size, I2C_Burst_T *burst_options);
HAL_Status_T HAL_I2C_Master_Receive_BURST(I2C_Handle_T *hi2c, uint16_t dev_address, uint8_t *data, uint8_t size, I2C_Burst_T *burst_options);

/* IRQHandler and Event Callback functions ************************************/
void HAL_I2C_IRQHandler(I2C_Handle_T *hi2c);
void HAL_I2C_MasterTxCpltCallback(I2C_Handle_T *hi2c);
void HAL_I2C_MasterRxCpltCallback(I2C_Handle_T *hi2c);
void HAL_I2C_SlaveTxCpltCallback(I2C_Handle_T *hi2c);
void HAL_I2C_SlaveRxCpltCallback(I2C_Handle_T *hi2c);
void HAL_I2C_AbortCpltCallback(I2C_Handle_T *hi2c);
void HAL_I2C_ErrorCallback(I2C_Handle_T *hi2c);

#ifdef __cplusplus
}
#endif

#endif /* _HAL_I2C_H_ */
