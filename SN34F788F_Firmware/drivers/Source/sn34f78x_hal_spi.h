/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _SN34F78X_HAL_SPI_H_
#define _SN34F78X_HAL_SPI_H_

/* Includes ------------------------------------------------------------------*/
#include "sn34f78x_hal.h"

/* Private macros ------------------------------------------------------------*/
/**
 * @brief  is spi instance?
 * @param  __HANDLE__ specifies the SPI Handle.
 */
#define IS_SPI_INSTANCE(__HANDLE__) (((__HANDLE__)->instance) == SN_SPI0 || \
                                     ((__HANDLE__)->instance) == SN_SPI1 || \
                                     ((__HANDLE__)->instance) == SN_SPI2)

/**
 * @brief  is spi frame format?
 * @param  __HANDLE__ specifies the SPI Handle.
 */
#define IS_SPI_FFMT(__HANDLE__) (((__HANDLE__)->init.frame_format) == SPI_FFMT_SPI || \
                                 ((__HANDLE__)->init.frame_format) == SPI_FFMT_MWR)

/**
 * @brief  is spi master/slaver mode?
 * @param  __HANDLE__ specifies the SPI Handle.
 */
#define IS_SPI_MODE(__HANDLE__) (((__HANDLE__)->init.mode) == SPI_MODE_SLAVE || \
                                 ((__HANDLE__)->init.mode) == SPI_MODE_MASTER)

/**
 * @brief  is spi data length?
 * @param  __HANDLE__ specifies the SPI Handle.
 */
#define SPI_DATA_LEN_MIN            (5U)
#define SPI_DATA_LEN_MAX            (16U)
#define IS_SPI_DATA_LEN(__HANDLE__) (((__HANDLE__)->init.data_size) >= SPI_DATA_LEN_MIN && \
                                     ((__HANDLE__)->init.data_size) <= SPI_DATA_LEN_MAX)

/**
 * @brief  is spi frame/sync polarity?
 * @param  __HANDLE__ specifies the SPI Handle.
 */
#define IS_SPI_FS_POL(__HANDLE__) (((__HANDLE__)->init.fs_polarity) == SPI_FS_POLARITY_LOW || \
                                   ((__HANDLE__)->init.fs_polarity) == SPI_FS_POLARITY_HIGH)

/**
 * @brief  is spi clock polarity?
 * @param  __HANDLE__ specifies the SPI Handle.
 */
#define IS_SPI_CLK_POL(__HANDLE__) (((__HANDLE__)->init.clk_polarity) == SPI_CLK_POLARITY_LOW || \
                                    ((__HANDLE__)->init.clk_polarity) == SPI_CLK_POLARITY_HIGH)

/**
 * @brief  is spi clock phase?
 * @param  __HANDLE__ specifies the SPI Handle.
 */
#define IS_SPI_CLK_PHA(__HANDLE__) (((__HANDLE__)->init.clk_phase) == SPI_CLK_POLARITY_LOW || \
                                    ((__HANDLE__)->init.clk_phase) == SPI_CLK_POLARITY_HIGH)

/**
 * @brief  is spi clock div?
 * @param  __HANDLE__ specifies the SPI Handle.
 */
#define SPI0_CLK_DIV_MIN            (1U)
#define SPI12_CLK_DIV_MIN           (2U)
#define SPI_CLK_DIV_MAX             (512U)
#define IS_SPI0_CLK_DIV(__HANDLE__) (((__HANDLE__)->init.sclk_div) >= SPI0_CLK_DIV_MIN && \
                                     ((__HANDLE__)->init.sclk_div) <= SPI_CLK_DIV_MAX)
#define IS_SPI12_CLK_DIV(__HANDLE__) (((__HANDLE__)->init.sclk_div) >= SPI12_CLK_DIV_MIN && \
                                      ((__HANDLE__)->init.sclk_div) <= SPI_CLK_DIV_MAX)

/**
 * @brief  is spi first bit?
 * @param  __HANDLE__ specifies the SPI Handle.
 */
#define IS_SPI_MSB_LSB(__HANDLE__) (((__HANDLE__)->init.first_bit) == SPI_FIRST_BIT_MSB || \
                                    ((__HANDLE__)->init.first_bit) == SPI_FIRST_BIT_LSB)

/**
 * @brief  Check whether the specified SPI mode is master or slave.
 * @param  __HANDLE__ specifies the SPI Handle.
 */
#define SPI_MASTER                (2)
#define SPI_SLAVE                 (0)
#define IS_SPI_MASTER(__HANDLE__) (((__HANDLE__)->instance->CTRL0_b.OPM) == SPI_MASTER)
#define IS_SPI_SLAVE(__HANDLE__)  (((__HANDLE__)->instance->CTRL0_b.OPM) == SPI_SLAVE)

/**
 * @brief  Check whether the SPI tx fifo is empty.
 * @param  __HANDLE__ specifies the SPI Handle.
 */
#define IS_TX_FIFO_EMPTY(__HANDLE__) (HAL_FLAG_EQU(((__HANDLE__)->instance->STATUS_b.TFVE), 0))

/**
 * @brief  Enable/Disable the specified SPI interrupts.
 * @param  __HANDLE__ specifies the SPI Handle.
 * @param  __INTERRUPT__ specifies the interrupt source to enable or disable.
 */
#define SPI_IE_TXCIEN                             (1U << 18) // TX Data Complete Interrupt Enable
#define SPI_IE_TFDMAEN                            (1U << 5)  // TX dma interrupt enable
#define SPI_IE_RFDMAEN                            (1U << 4)  // RX dma interrupt enable
#define SPI_IE_TFTHIEN                            (1U << 3)  // TX FIFO threshold interrupt enable
#define SPI_IE_RFTHIEN                            (1U << 2)  // RX FIFO threshold interrupt enable
#define SPI_IE_TFURIEN                            (1U << 1)  // TX Underflow interrupt enable
#define SPI_IE_RFORIEN                            (1U << 0)  // TX Overflow interrupt enable
#define SPI_ENABLE_IE(__HANDLE__, __INTERRUPT__)  HAL_FLAG_SET((__HANDLE__)->instance->ICTRL, (__INTERRUPT__))
#define SPI_DISABLE_IE(__HANDLE__, __INTERRUPT__) HAL_FLAG_CLR((__HANDLE__)->instance->ICTRL, (__INTERRUPT__))

/**
 * @brief  set fifo level meanwhile enable irq.
 * @param  __HANDLE__ specifies the SPI Handle.
 */

#define MASTER_TX_THOD                           (0) // 0 : Use TXCI to trigger interrupt;   1-8 : Use TFTHI to trigger interrupt
#define MASTER_RX_THOD                           (8)
#define SPI_SET_MASTER_FIFO_RX_VAL(size)         ((size) >= MASTER_RX_THOD ? MASTER_RX_THOD : size)
#define SPI_SET_MASTER_TX_FIFO(__HANDLE__, size) HAL_REG_WRITE((__HANDLE__)->instance->ICTRL_b.TFTHOD, MASTER_TX_THOD)
#define SPI_SET_MASTER_RX_FIFO(__HANDLE__, size) HAL_REG_WRITE((__HANDLE__)->instance->ICTRL_b.RFTHOD, SPI_SET_MASTER_FIFO_RX_VAL(size))

#define SLAVE_TX_THOD                           (0) // 0 : Use TXCI to trigger interrupt;   1-8 : Use TFTHI to trigger interrupt
#define SLAVE_RX_THOD                           (8)
#define SPI_SET_SLAVE_FIFO_RX_VAL(size)         ((size) >= SLAVE_RX_THOD ? SLAVE_RX_THOD : size)
#define SPI_SET_SLAVE_TX_FIFO(__HANDLE__, size) HAL_REG_WRITE((__HANDLE__)->instance->ICTRL_b.TFTHOD, SLAVE_TX_THOD)
#define SPI_SET_SLAVE_RX_FIFO(__HANDLE__, size) HAL_REG_WRITE((__HANDLE__)->instance->ICTRL_b.RFTHOD, SPI_SET_SLAVE_FIFO_RX_VAL(size))

/**
 * @brief  clear fifo level meanwhile disable irq.
 * @param  __HANDLE__ specifies the SPI Handle.
 */
#define SPI_DISABLE_TX_FIFO(__HANDLE__) HAL_REG_WRITE((__HANDLE__)->instance->ICTRL_b.TFTHOD, 0)
#define SPI_DISABLE_RX_FIFO(__HANDLE__) HAL_REG_WRITE((__HANDLE__)->instance->ICTRL_b.RFTHOD, 0)

/**
 * @brief  Clear fifo status.
 * @param  __HANDLE__ specifies the SPI Handle.
 */
#define SPI_RESET_FIFO(__HANDLE__) HAL_FLAG_SET((__HANDLE__)->instance->CTRL2, SPI_CTRL2_TXFCLR_MASK | SPI_CTRL2_RXFCLR_MASK)

/**
 * @brief  Enable/Disable Tx/Rx.
 * @param  __HANDLE__ specifies the SPI Handle.
 */
#define SPI_ENABLE_TX_FUC(__HANDLE__)  HAL_REG_WRITE((__HANDLE__)->instance->CTRL2_b.TXEN, 1)
#define SPI_ENABLE_RX_FUC(__HANDLE__)  HAL_REG_WRITE((__HANDLE__)->instance->CTRL2_b.RXEN, 1)
#define SPI_DISABLE_TX_FUC(__HANDLE__) HAL_REG_WRITE((__HANDLE__)->instance->CTRL2_b.TXEN, 0)
#define SPI_DISABLE_RX_FUC(__HANDLE__) HAL_REG_WRITE((__HANDLE__)->instance->CTRL2_b.RXEN, 0)

/**
 * @brief  Check whether the specified SPI flag is set or not.
 * @param  __HANDLE__ specifies the SPI Handle.
 * @param  __FLAG__ specifies the flag to check.
 */
#define SPI_STATUS_BUSY                       (1U << 2) // Busy flag
#define SPI_STATUS_TFNF                       (1U << 1) // Data in TX FIFO <= TFTHOD
#define SPI_STATUS_RFF                        (1U << 0) // Data in RX FIFO >= RFTHOD
#define SPI_STATUS_HAS(__HANDLE__, __FLAG__)  (HAL_FLAG_HAS(((__HANDLE__)->instance->STATUS), (__FLAG__)))
#define SPI_STATUS_LACK(__HANDLE__, __FLAG__) (HAL_FLAG_LACKS(((__HANDLE__)->instance->STATUS), (__FLAG__)))

/**
 * @brief  spi inner state.
 * @param  __HANDLE__ specifies the SPI Handle.
 * @param  __STATE_EX__ specifies the state_ex source to enable or disable.
 */
#define SPI_STATE_EX_NONE                           (1U << 0) // spi current usage idle
#define SPI_STATE_EX_TX_ONLY                        (1U << 1) // spi current usage for TX only
#define SPI_STATE_EX_RX_ONLY                        (1U << 2) // spi current usage for RX only
#define SPI_STATE_EX_BOTH                           (1U << 3) // spi current usage for TX/RX both
#define SPI_STATE_EX_ABORT                          (1U << 4) // abort is ongoing
#define SPI_STATE_EX_DMA                            (1U << 5) // dma is ongoing
#define SPI_STATE_EX_SET(__HANDLE__, __STATE_EX__)  HAL_FLAG_SET((__HANDLE__)->state_ex, (__STATE_EX__))
#define SPI_STATE_EX_CLR(__HANDLE__, __STATE_EX__)  HAL_FLAG_CLR((__HANDLE__)->state_ex, (__STATE_EX__))
#define SPI_STATE_EX_HAS(__HANDLE__, __STATE_EX__)  HAL_FLAG_HAS((__HANDLE__)->state_ex, (__STATE_EX__))
#define SPI_STATE_EX_LACK(__HANDLE__, __STATE_EX__) HAL_FLAG_LACKS((__HANDLE__)->state_ex, (__STATE_EX__))

/* Private constants ---------------------------------------------------------*/
#define SPI_ICTRL_TXCIEN_BIT  (18) // [18..18] Transmit Data Complete Interrupt Enable.
#define SPI_ICTRL_TFTHOD_BIT  (12) // [16..12] Transmit FIFO Threshold. If the valid data in the transmit.
#define SPI_ICTRL_RFTHOD_BIT  (7)  // [11.. 7] Receive FIFO Threshold. If the valid data in the receive.
#define SPI_ICTRL_TFDMAEN_BIT (5)  // [ 5.. 5] Transmit DMA Request Enable.
#define SPI_ICTRL_RFDMAEN_BIT (4)  // [ 4.. 4] Receive DMA Request Enable.
#define SPI_ICTRL_TFTHIEN_BIT (3)  // [ 3.. 3] Transmit FIFO Threshold Interrupt Enable.
#define SPI_ICTRL_RFTHIEN_BIT (2)  // [ 2.. 2] Receive FIFO Threshold Interrupt Enable.
#define SPI_ICTRL_TFURIEN_BIT (1)  // [ 1.. 1] Transmit FIFO Underrun Interrupt Enable.
#define SPI_ICTRL_RFORIEN_BIT (0)  // [ 0.. 0] Receive FIFO Overrun Interrupt Enable.

#define SPI_CTRL2_FSOS_BIT   (10) // [11..10] Frame/Sync. output selection bits for SPI mode
#define SPI_CTRL2_FS_BIT     (9)  // [ 9.. 9] Frame Sync. output level for SPI Flash mode
#define SPI_CTRL2_TXEN_BIT   (8)  // [ 8.. 8] Transmit function enable bit
#define SPI_CTRL2_RXEN_BIT   (7)  // [ 7.. 7] Receive function enable bit
#define SPI_CTRL2_RESET_BIT  (6)  // [ 6.. 6] The software reset of the state machine
#define SPI_CTRL2_TXFCLR_BIT (3)  // [ 3.. 3] TX FIFO clear bit
#define SPI_CTRL2_RXFCLR_BIT (2)  // [ 2.. 2] RX FIFO clear bit
#define SPI_CTRL2_TXDOE_BIT  (1)  // [ 1.. 1] Transmit data output enable bit
#define SPI_CTRL2_SPIEN_BIT  (0)  // [ 0.. 0] SPI/I2S enable bit

#define SPI_STATUS_TFVE_BIT (12) // [17..12] The number of entries in the transmit FIFO waiting to be transmitted
#define SPI_STATUS_RFVE_BIT (4)  // [ 9.. 4] The number of entries in the receive FIFO waiting for DMA or host processor to read them
#define SPI_STATUS_BUSY_BIT (2)  // [ 2.. 2] Busy Indicator
#define SPI_STATUS_TFNF_BIT (1)  // [ 1.. 1] Transmit FIFO Not Full
#define SPI_STATUS_RFF_BIT  (0)  // [ 0.. 0] Receive FIFO Full

#define SPI_STATUS_TFVE_MASK (0x3F) // Describe TFVE by 6 bit
#define SPI_STATUS_RFVE_MASK (0x3F) // Describe RFVE by 6 bit

#define SPI_CTRL2_TXEN_MASK   (0x1 << SPI_CTRL2_TXEN_BIT)
#define SPI_CTRL2_RXEN_MASK   (0x1 << SPI_CTRL2_RXEN_BIT)
#define SPI_CTRL2_TXFCLR_MASK (0x1 << SPI_CTRL2_TXFCLR_BIT)
#define SPI_CTRL2_RXFCLR_MASK (0x1 << SPI_CTRL2_RXFCLR_BIT)

#define SPI_ICTRL_TFTHOD_MASK  (0x1F << SPI_ICTRL_TFTHOD_BIT)
#define SPI_ICTRL_RFTHOD_MASK  (0x1F << SPI_ICTRL_RFTHOD_BIT)
#define SPI_ICTRL_TFDMAEN_MASK (0x1 << SPI_ICTRL_TFDMAEN_BIT)
#define SPI_ICTRL_RFDMAEN_MASK (0x1 << SPI_ICTRL_RFDMAEN_BIT)
#define SPI_ICTRL_TFTHIEN_MASK (0x1 << SPI_ICTRL_TFTHIEN_BIT)
#define SPI_ICTRL_RFTHIEN_MASK (0x1 << SPI_ICTRL_RFTHIEN_BIT)

#define SPI_RIS_TXCI_MASK  (0x1 << 5) // [ 5.. 5] TX DATA complete interrupt flag
#define SPI_RIS_TFTHI_MASK (0x1 << 3) // [ 3.. 3] TX FIFO threshold Interrupt flag
#define SPI_RIS_RFTHI_MASK (0x1 << 2) // [ 2.. 2] RX FIFO threshold interrupt flag
#define SPI_RIS_TFURI_MASK (0x1 << 1) // [ 1.. 1] TX FIFO underrun interrupt flag
#define SPI_RIS_RFORI_MASK (0x1 << 0) // [ 0.. 0] RX FIFO overrun interrupt flag

#define SPI_TX_FIFO_DEPTH (0x8) // tx fifo depth
#define SPI_RX_FIFO_DEPTH (0x8) // rx fifo depth

/* Private function ----------------------------------------------------------*/
#if (configUSE_SPI_NONBLOCKING_DMA == 1U)
static void         _spi_tx_dma_err_cb(DMA_Handle_T *hdma);
static void         _spi_rx_dma_err_cb(DMA_Handle_T *hdma);
static void         _spi_tx_dma_cb(DMA_Handle_T *hdma);
static void         _spi_rx_dma_cb(DMA_Handle_T *hdma);
static HAL_Status_T _spi_dma_xfer_kernel(SPI_Handle_T *hspi, uint8_t *tx_data, uint8_t *rx_data, uint32_t tx_size, uint32_t rx_size);
static HAL_Status_T _spi_dma_abort(SPI_Handle_T *hspi);
static void         _spi_switch_dma_cb(SPI_Handle_T *hspi);
#endif

#if (configUSE_SPI_BLOCKING == 1U)
static void         _spi_rx_data(SPI_Handle_T *hspi);
static HAL_Status_T _spi_xfer_kernel(SPI_Handle_T *hspi, uint8_t *tx_data, uint8_t *rx_data, uint32_t tx_size, uint32_t rx_size);
#endif /* configUSE_SPI_BLOCKING */

#if (configUSE_SPI_NONBLOCKING_IT == 1U)
static HAL_Status_T _spi_it_xfer_kernel(SPI_Handle_T *hspi, uint8_t *tx_data, uint8_t *rx_data, uint32_t tx_size, uint32_t rx_size);
static void         _spi_switch_cb(SPI_Handle_T *hspi);
static void         _spi_rx_fifo_data(SPI_Handle_T *hspi, uint32_t cnt);
#endif /* configUSE_SPI_NONBLOCKING_IT */

#if (configUSE_SPI_REGISTER_CALLBACKS == 1U)
static void inline _spi_reset_callback(SPI_Handle_T *hspi);
#endif /* configUSE_SPI_REGISTER_CALLBACKS */

#if (configUSE_SPI_BLOCKING == 1U || configUSE_SPI_NONBLOCKING_IT == 1U)
static void _spi_tx_fifo_data(SPI_Handle_T *hspi, uint32_t cnt);
static void _spi_tx_fifo_dummy(SPI_Handle_T *hspi, uint32_t cnt);
#endif /* configUSE_SPI_BLOCKING ||  configUSE_SPI_NONBLOCKING_IT*/

static uint32_t _spi_set_config(SPI_Handle_T *hspi);
static void inline _spi_xfer_info(SPI_Handle_T *hspi, uint8_t *tx_data, uint8_t *rx_data, uint32_t tx_size, uint32_t rx_size);

#endif /* _SN34F78X_HAL_SPI_H_ */
