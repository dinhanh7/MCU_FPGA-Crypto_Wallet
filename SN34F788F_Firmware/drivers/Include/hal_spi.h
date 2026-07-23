/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _HAL_SPI_H_
#define _HAL_SPI_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sn34f78x_hal_def.h"

/* Exported types ------------------------------------------------------------*/
/**
 * \brief   SPI configuration structure definition
 * \ingroup spi_struct_define
 */
typedef struct _SPI_Init_T
{
    uint32_t frame_format; /**< Specifies if the NS micro-wire mode is enabled or not.
                                This parameter can be a value of @ref spi_frame_format */
    uint32_t mode;         /**< Specifies the SPI operating mode.
                                This parameter can be a value of @ref spi_mode */
    uint32_t data_size;    /**< Specifies the SPI data size.
                                This parameter must be a number of @ref spi_data_size  */
    uint32_t fs_polarity;  /**< Specifies the Frame/Sync. polarity for the SPI mode.
                                This parameter can be a value of @ref spi_fs_polarity */
    uint32_t clk_polarity; /**< Specifies the serial clock steady state.
                                This parameter can be a value of @ref spi_clk_polarity */
    uint32_t clk_phase;    /**< Specifies the clock active edge for the bit capture.
                                This parameter can be a value of @ref spi_clk_phase */
    uint32_t sclk_div;     /**< Specifies communication clock divider value which will be
                                used to configure the transmit and receive SCK clock.
                                This parameter must be an number between "min_data = 2(SPI1, SPI2) or min_data = 1(SPI0)" and "max_data = 512"
                                @note The communication clock is derived from the master
                                      clock. The slave clock does not need to be set.
                                @note Fclk = Fpclk / (2 * sclk_div + 1). */
    uint32_t first_bit;    /**< Specifies whether data transfers start from MSB or LSB bit.
                                This parameter can be a value of @ref spi_first_bit */
    uint16_t tx_default;   /**< Specifies the master tx default value when master receive only.
                                This parameter must be an number between min_data = 0 and max_data = 0xFFFF */
} SPI_Init_T;

/**
 * \brief   SPI tx runtime information
 * \ingroup spi_struct_define
 */
typedef struct _SPI_TX_T
{
    const uint8_t *buffer;     /**< Pointer to SPI tx transfer buffer   */
    uint32_t       xfer_size;  /**< SPI tx transfer size                */
    __IOM uint32_t xfer_count; /**< SPI tx transfer down counter        */
    uint32_t       dummy;      /**< SPI tx dummy data                   */
} SPI_TX_T;

/**
 * \brief   SPI rx runtime information
 * \ingroup spi_struct_define
 */
typedef struct _SPI_RX_T
{
    uint8_t       *buffer;     /**< Pointer to SPI rx transfer buffer     */
    uint32_t       xfer_size;  /**< SPI rx transfer size                  */
    __IOM uint32_t xfer_count; /**< SPI rx transfer down counter          */
} SPI_RX_T;

/**
 * \brief   SPI handle structure definition
 * \ingroup spi_struct_define
 */
typedef struct _SPI_Handle_T
{
    SN_SPI0_Type     *instance;   /**< SPI registers base address               */
    SPI_Init_T        init;       /**< SPI communication parameters             */
    SPI_TX_T          tx;         /**< SPI tx transfer runtime information      */
    SPI_RX_T          rx;         /**< SPI rx transfer runtime information      */
    HAL_MUTEX         mutex;      /**< SPI process protection                   */
    __IOM HAL_State_T state;      /**< SPI communication state                  */
    __IOM uint32_t    state_ex;   /**< SPI inner state                          */
    __IOM uint32_t    error_code; /**< SPI error code                           */
    uint8_t           base;       /**< SPI transfer step                        */
    uint16_t          data_mask;  /**< SPI transfer data mask                   */

#if (configUSE_SPI_NONBLOCKING_DMA == 1U)
    DMA_Handle_T *hdmatx; /**< SPI tx DMA Handle parameters             */
    DMA_Handle_T *hdmarx; /**< SPI rx DMA Handle parameters             */
#endif                    /* configUSE_SPI_NONBLOCKING_DMA */

#if (configUSE_SPI_REGISTER_CALLBACKS == 1U)
    void (*TxCpltCallback)(struct _SPI_Handle_T *hspi);    /**< SPI tx completed callback          */
    void (*RxCpltCallback)(struct _SPI_Handle_T *hspi);    /**< SPI rx completed callback          */
    void (*TxRxCpltCallback)(struct _SPI_Handle_T *hspi);  /**< SPI tx/rx completed callback       */
    void (*ErrorCallback)(struct _SPI_Handle_T *hspi);     /**< SPI error callback                 */
    void (*AbortCpltCallback)(struct _SPI_Handle_T *hspi); /**< SPI abort callback                 */
    void (*MspInitCallback)(struct _SPI_Handle_T *hspi);   /**< SPI msp init callback              */
    void (*MspDeInitCallback)(struct _SPI_Handle_T *hspi); /**< SPI msp deinit callback            */
#endif                                                     /* configUSE_SPI_REGISTER_CALLBACKS */
} SPI_Handle_T;

/**
 * \brief   SPI callback ID enumeration definition
 * \ingroup spi_struct_define
 */
typedef enum _HAL_SPI_CallbackID_T
{
    HAL_SPI_TX_COMPLETE_CB_ID    = 0x00U, /**< SPI tx completed callback ID         */
    HAL_SPI_RX_COMPLETE_CB_ID    = 0x01U, /**< SPI rx completed callback ID         */
    HAL_SPI_TX_RX_COMPLETE_CB_ID = 0x02U, /**< SPI tx/rx completed callback ID      */
    HAL_SPI_ERROR_CB_ID          = 0x06U, /**< SPI error callback ID                */
    HAL_SPI_ABORT_CB_ID          = 0x07U, /**< SPI abort callback ID                */
    HAL_SPI_MSP_INIT_CB_ID       = 0x08U, /**< SPI msp init callback ID             */
    HAL_SPI_MSP_DEINIT_CB_ID     = 0x09U, /**< SPI msp deinit callback ID           */
} HAL_SPI_CallbackID_T;

/**
 * \brief   SPI callback pointer definition
 * \ingroup spi_struct_define
 */
typedef void (*SPI_Callback_T)(SPI_Handle_T *hspi); /**< pointer to an SPI callback function */

/* Exported constants --------------------------------------------------------*/
/**
 * @defgroup spi_frame_format SPI frame format
 * \ingroup spi_control
 * @{
 */
#define SPI_FFMT_SPI (0x00000001U) /**< SPI mode enable */
#define SPI_FFMT_MWR (0x00000002U) /**< Micro-wire mode enable */
/**
 * @}
 */

/**
 * @defgroup spi_mode SPI operating Mode
 * \ingroup spi_control
 * @{
 */
#define SPI_MODE_SLAVE  (0x00000000U) /**< SPI working at slave  mode */
#define SPI_MODE_MASTER (0x00000001U) /**< SPI working at master mode */
/**
 * @}
 */

/**
 * @defgroup spi_data_size SPI Data Size
 * \ingroup spi_control
 * @{
 */
#define SPI_DATASIZE_5BIT  (0x00000005U) /**< SPI bit length of a transmitted/received is 5bit */
#define SPI_DATASIZE_6BIT  (0x00000006U) /**< SPI bit length of a transmitted/received is 6bit */
#define SPI_DATASIZE_7BIT  (0x00000007U) /**< SPI bit length of a transmitted/received is 7bit */
#define SPI_DATASIZE_8BIT  (0x00000008U) /**< SPI bit length of a transmitted/received is 8bit */
#define SPI_DATASIZE_9BIT  (0x00000009U) /**< SPI bit length of a transmitted/received is 9bit */
#define SPI_DATASIZE_10BIT (0x0000000AU) /**< SPI bit length of a transmitted/received is 10bit */
#define SPI_DATASIZE_11BIT (0x0000000BU) /**< SPI bit length of a transmitted/received is 11bit */
#define SPI_DATASIZE_12BIT (0x0000000CU) /**< SPI bit length of a transmitted/received is 12bit */
#define SPI_DATASIZE_13BIT (0x0000000DU) /**< SPI bit length of a transmitted/received is 13bit */
#define SPI_DATASIZE_14BIT (0x0000000EU) /**< SPI bit length of a transmitted/received is 14bit */
#define SPI_DATASIZE_15BIT (0x0000000FU) /**< SPI bit length of a transmitted/received is 15bit */
#define SPI_DATASIZE_16BIT (0x00000010U) /**< SPI bit length of a transmitted/received is 16bit */
/**
 * @}
 */

/**
 * @defgroup spi_fs_polarity SPI Frame/Sync active polarity
 * \ingroup spi_control
 * @{
 */
#define SPI_FS_POLARITY_LOW  (0x00000000U) /**< Frame/Sync active at low  level */
#define SPI_FS_POLARITY_HIGH (0x00000001U) /**< Frame/Sync active at high level */
/**
 * @}
 */

/**
 * @defgroup spi_clk_polarity SPI clock polarity
 * \ingroup spi_control
 * @{
 */
#define SPI_CLK_POLARITY_LOW  (0x00000000U) /**< SCK active at low  level */
#define SPI_CLK_POLARITY_HIGH (0x00000001U) /**< SCK active at high level */
/**
 * @}
 */

/**
 * @defgroup spi_clk_phase SPI Clock Phase
 * \ingroup spi_control
 * @{
 */
#define SPI_CLK_PHASE_1EDGE (0x00000000U) /**< Data changes at clock falling edge, latches at clock rising  edge when CPOL = 0, \
                                          <br> Data changes at clock rising  edge, latches at clock falling edge when CPOL = 1. */
#define SPI_CLK_PHASE_2EDGE (0x00000001U) /**< Data changes at clock rising  edge, latches at clock falling edge when CPOL = 0, \
                                          <br> Data changes at clock falling edge, latches at clock rising  edge when CPOL = 1. */
/**
 * @}
 */

/**
 * @defgroup spi_first_bit SPI MSB to LSB transmission mode
 * \ingroup spi_control
 * @{
 */
#define SPI_FIRST_BIT_MSB (0x00000000U) /**< MSB transmit first */
#define SPI_FIRST_BIT_LSB (0x00000001U) /**< LSB transmit first */
/**
 * @}
 */

/**
 * \ingroup spi_error_code
 * @{
 */
#define HAL_SPI_ERROR_INVALID_CALLBACK (HAL_ERROR_SPECIFY + 0x01) /**< invalid callback error                 */
#define HAL_SPI_ERROR_TX_UNDERFLOW     (HAL_ERROR_SPECIFY + 0x02) /**< tx underflow error                     */
#define HAL_SPI_ERROR_RX_OVERFLOW      (HAL_ERROR_SPECIFY + 0x03) /**< rx overflow error                      */
#define HAL_SPI_ERROR_DMA              (HAL_ERROR_SPECIFY + 0x04) /**< dma transfer error                     */
#define HAL_SPI_ERROR_DMA_TX           (HAL_ERROR_SPECIFY + 0x05) /**< dma tx channel transfer error          */
#define HAL_SPI_ERROR_DMA_RX           (HAL_ERROR_SPECIFY + 0x06) /**< dma rx channel transfer error          */
#define HAL_SPI_ERROR_ABORT            (HAL_ERROR_SPECIFY + 0x07) /**< error during spi abort procedure       */
/**
 * @}
 */

/* Exported functions --------------------------------------------------------*/
/* Initialization/de-initialization functions */
HAL_Status_T HAL_SPI_Init(SPI_Handle_T *hspi);
HAL_Status_T HAL_SPI_DeInit(SPI_Handle_T *hspi);
void         HAL_SPI_MspInit(SPI_Handle_T *hspi);
void         HAL_SPI_MspDeInit(SPI_Handle_T *hspi);

/* Callbacks Register/UnRegister functions ************************************/
HAL_Status_T HAL_SPI_RegisterCallback(SPI_Handle_T *hspi, HAL_SPI_CallbackID_T cb_id, SPI_Callback_T pcb);
HAL_Status_T HAL_SPI_UnRegisterCallback(SPI_Handle_T *hspi, HAL_SPI_CallbackID_T cb_id);

/* Peripheral State and Error functions */
HAL_State_T HAL_SPI_GetState(SPI_Handle_T *hspi);
uint32_t    HAL_SPI_GetError(SPI_Handle_T *hspi);

/* I/O operation functions */
HAL_Status_T HAL_SPI_Transmit(SPI_Handle_T *hspi, uint8_t *data, uint32_t size, uint32_t timeout);
HAL_Status_T HAL_SPI_Receive(SPI_Handle_T *hspi, uint8_t *data, uint32_t size, uint32_t timeout);
HAL_Status_T HAL_SPI_TransmitReceive(SPI_Handle_T *hspi, uint8_t *tx_data, uint8_t *rx_data, uint32_t size, uint32_t timeout);

HAL_Status_T HAL_SPI_Transmit_IT(SPI_Handle_T *hspi, uint8_t *data, uint32_t size);
HAL_Status_T HAL_SPI_Receive_IT(SPI_Handle_T *hspi, uint8_t *data, uint32_t size);
HAL_Status_T HAL_SPI_TransmitReceive_IT(SPI_Handle_T *hspi, uint8_t *tx_data, uint8_t *rx_data, uint32_t size);

HAL_Status_T HAL_SPI_Transmit_DMA(SPI_Handle_T *hspi, uint8_t *data, uint32_t size);
HAL_Status_T HAL_SPI_Receive_DMA(SPI_Handle_T *hspi, uint8_t *data, uint32_t size);
HAL_Status_T HAL_SPI_TransmitReceive_DMA(SPI_Handle_T *hspi, uint8_t *tx_data, uint8_t *rx_data, uint32_t size);
HAL_Status_T HAL_SPI_DMAPause(SPI_Handle_T *hspi);
HAL_Status_T HAL_SPI_DMAResume(SPI_Handle_T *hspi);
HAL_Status_T HAL_SPI_DMAStop(SPI_Handle_T *hspi);

/* Transfer Abort functions ***************************************************/
HAL_Status_T HAL_SPI_Abort(SPI_Handle_T *hspi);
HAL_Status_T HAL_SPI_Abort_IT(SPI_Handle_T *hspi);

/* Callbacks operation functions */
void HAL_SPI_TxCpltCallback(SPI_Handle_T *hspi);
void HAL_SPI_RxCpltCallback(SPI_Handle_T *hspi);
void HAL_SPI_TxRxCpltCallback(SPI_Handle_T *hspi);
void HAL_SPI_ErrorCallback(SPI_Handle_T *hspi);
void HAL_SPI_AbortCpltCallback(SPI_Handle_T *hspi);

/* SPI IRQ handler management */
void HAL_SPI_IRQHandler(SPI_Handle_T *hspi);

#ifdef __cplusplus
}
#endif
#endif /* _HAL_SPI_H_ */
