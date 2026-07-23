/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _SN34F78X_HAL_I2S_H_
#define _SN34F78X_HAL_I2S_H_

/* Includes ------------------------------------------------------------------*/
#include "sn34f78x_hal.h"

/* Private macros ------------------------------------------------------------*/
/**
 * @brief  is i2s instance?
 * @param  __HANDLE__ specifies the I2S Handle.
 */
#define IS_I2S_INSTANCE(__HANDLE__) (((__HANDLE__)->instance) == SN_SPI0 || \
                                     ((__HANDLE__)->instance) == SN_SPI1 || \
                                     ((__HANDLE__)->instance) == SN_SPI2)

/**
 * @brief  is i2s master/slaver mode?
 * @param  __HANDLE__ specifies the I2S Handle.
 */
#define IS_I2S_MODE(__HANDLE__) (((__HANDLE__)->init.mode) == I2S_MODE_SLAVE || \
                                 ((__HANDLE__)->init.mode) == I2S_MODE_MASTER)

/**
 * @brief  is i2s standard?
 * @param  __HANDLE__ specifies the I2S Handle.
 */
#define IS_I2S_STD(__HANDLE__) ((((__HANDLE__)->init.standard) == I2S_STANDARD_PHILIPS) || \
                                (((__HANDLE__)->init.standard) == I2S_STANDARD_MSB) ||     \
                                (((__HANDLE__)->init.standard) == I2S_STANDARD_LSB))

/**
 * @brief  is i2s channel?
 * @param  __HANDLE__ specifies the I2S Handle.
 */
#define IS_I2S_CHANNEL(__HANDLE__) (((__HANDLE__)->init.channel) == I2S_CHANNEL_MONO || \
                                    ((__HANDLE__)->init.channel) == I2S_CHANNEL_STEREO)

/**
 * @brief  is i2s data length?
 * @param  __HANDLE__ specifies the I2S Handle.
 */
#define IS_I2S_DATA_LEN(__HANDLE__) (((__HANDLE__)->init.data_format) == I2S_DATA_LEN_8B ||  \
                                     ((__HANDLE__)->init.data_format) == I2S_DATA_LEN_16B || \
                                     ((__HANDLE__)->init.data_format) == I2S_DATA_LEN_24B || \
                                     ((__HANDLE__)->init.data_format) == I2S_DATA_LEN_32B)

/**
 * @brief  is i2s channel length?
 * @param  __HANDLE__ specifies the I2S Handle.
 */
#define IS_I2S_CH_LEN(__HANDLE__) (((__HANDLE__)->init.channel_len) >= 16U && \
                                   ((__HANDLE__)->init.channel_len) <= 32U)

/**
 * @brief  is i2s clock div?
 * @param  __HANDLE__ specifies the I2S Handle.
 */
#define IS_I2S_CLK_DIV(__HANDLE__) (((__HANDLE__)->init.sclk_div) >= 2U && \
                                    ((__HANDLE__)->init.sclk_div) <= 512U)

#define PERIPHERAL_I2S_ENABLE(HANDLE)  HAL_REG_WRITE((HANDLE)->instance->CTRL2_b.SPIEN, HAL_ENABLE)
#define PERIPHERAL_I2S_DISABLE(HANDLE) HAL_REG_WRITE((HANDLE)->instance->CTRL2_b.SPIEN, HAL_DISABLE)

/**
 * @brief  Clear fifo status.
 * @param  __HANDLE__ specifies the I2S Handle.
 */
#define I2S_RESET_FIFO(__HANDLE__)                            \
    HAL_REG_WRITE((__HANDLE__)->instance->CTRL2_b.RXFCLR, 1); \
    HAL_REG_WRITE((__HANDLE__)->instance->CTRL2_b.TXFCLR, 1);

/**
 * @brief  Check whether the specified I2S flag is set or not.
 * @param  __HANDLE__ specifies the I2S Handle.
 * @param  __FLAG__ specifies the flag to check.
 */
#define I2S_STAT_BUSY                       (1U << 2) // I2S_STAT_BUSY: Busy flag
#define I2S_STAT_TFNF                       (1U << 1) // I2S_STAT_TFNF: Tx FIFO is not full
#define I2S_STAT_RFF                        (1U << 0) // I2S_STAT_RFF:  Data in RX FIFO >= RXFIFOTH
#define I2S_STAT_HAS(__HANDLE__, __FLAG__)  (HAL_FLAG_HAS(((__HANDLE__)->instance->STATUS), (__FLAG__)))
#define I2S_STAT_LACK(__HANDLE__, __FLAG__) (HAL_FLAG_LACKS(((__HANDLE__)->instance->STATUS), (__FLAG__)))
#define IS_TX_FIFO_EMPTY(__HANDLE__)        (HAL_FLAG_EQU(((__HANDLE__)->instance->STATUS_b.TFVE), 0))
#define IS_RX_FIFO_FULL(__HANDLE__)         (HAL_FLAG_EQU(((__HANDLE__)->instance->STATUS_b.RFVE), 1))

/**
 * @brief  Enable/Disable the specified I2S interrupts.
 * @param  __HANDLE__ specifies the I2S Handle.
 * @param  __INTERRUPT__ specifies the interrupt source to enable or disable.
 *         This parameter can be one of the following values:
 *            @arg I2S_IE_TXCIEN: TX Data Complete Interrupt Enable
 *            @arg I2S_IE_TFDMAEN: TX dma interrupt enable
 *            @arg I2S_IE_RFDMAEN: TX dma interrupt enable
 *            @arg I2S_IE_TFTHIEN: TX FIFO threshold interrupt enable
 *            @arg I2S_IE_RFTHIEN: RX FIFO threshold interrupt enable
 *            @arg I2S_IE_TFURIEN: TX Underflow interrupt enable
 *            @arg I2S_IE_RFORIEN: TX Overflow interrupt enable
 * @retval None
 */
#define I2S_IE_TXCIEN                             (1U << 18)
#define I2S_IE_TFDMAEN                            (1U << 5)
#define I2S_IE_RFDMAEN                            (1U << 4)
#define I2S_IE_TFTHIEN                            (1U << 3)
#define I2S_IE_RFTHIEN                            (1U << 2)
#define I2S_IE_TFURIEN                            (1U << 1)
#define I2S_IE_RFORIEN                            (1U << 0)
#define I2S_ENABLE_IE(__HANDLE__, __INTERRUPT__)  HAL_FLAG_SET((__HANDLE__)->instance->ICTRL, (__INTERRUPT__))
#define I2S_DISABLE_IE(__HANDLE__, __INTERRUPT__) HAL_FLAG_CLR((__HANDLE__)->instance->ICTRL, (__INTERRUPT__))

/**
 * @brief  set fifo level meanwhile enable irq.
 * @param  __HANDLE__ specifies the I2S Handle.
 */
#define I2S_SET_FIFO_TX_VAL(size) (size)
#define I2S_SET_FIFO_RX_VAL(size) (size)

/**
 * @brief  clear fifo level meanwhile disable irq.
 * @param  __HANDLE__ specifies the I2S Handle.
 */
#define I2S_DISABLE_TX_FIFO(__HANDLE__) HAL_REG_WRITE((__HANDLE__)->instance->ICTRL_b.TFTHOD, 0)
#define I2S_DISABLE_RX_FIFO(__HANDLE__) HAL_REG_WRITE((__HANDLE__)->instance->ICTRL_b.RFTHOD, 0)
#define I2S_ENABLE_TX_FIFO(__HANDLE__)  HAL_REG_WRITE((__HANDLE__)->instance->ICTRL_b.TFTHOD, 1)
#define I2S_ENABLE_RX_FIFO(__HANDLE__)  HAL_REG_WRITE((__HANDLE__)->instance->ICTRL_b.RFTHOD, 1)

/**
 * @brief  clear fifo level meanwhile disable irq.
 * @param  __HANDLE__ specifies the I2S Handle.
 */
#define I2S_ENABLE_TX_FUC(__HANDLE__)  HAL_REG_WRITE((__HANDLE__)->instance->CTRL2_b.TXEN, HAL_ENABLE)
#define I2S_ENABLE_RX_FUC(__HANDLE__)  HAL_REG_WRITE((__HANDLE__)->instance->CTRL2_b.RXEN, HAL_ENABLE)
#define I2S_DISABLE_TX_FUC(__HANDLE__) HAL_REG_WRITE((__HANDLE__)->instance->CTRL2_b.TXEN, HAL_DISABLE)
#define I2S_DISABLE_RX_FUC(__HANDLE__) HAL_REG_WRITE((__HANDLE__)->instance->CTRL2_b.RXEN, HAL_DISABLE)

/* Private constants ---------------------------------------------------------*/
#define I2S_ICTRL_TXCIEN_BIT  (18) // [18 .. 18] Transmit Data Complete Interrupt Enable.
#define I2S_ICTRL_TFTHOD_BIT  (12) // [16 .. 12] Transmit FIFO Threshold. If the valid data in the transmit.
#define I2S_ICTRL_RFTHOD_BIT  (7)  // [11 ..  7] Receive FIFO Threshold. If the valid data in the receive.
#define I2S_ICTRL_TFDMAEN_BIT (5)  // [ 5 ..  5] Transmit DMA Request Enable.
#define I2S_ICTRL_RFDMAEN_BIT (4)  // [ 4 ..  4] Receive DMA Request Enable.
#define I2S_ICTRL_TFTHIEN_BIT (3)  // [ 3 ..  3] Transmit FIFO Threshold Interrupt Enable.
#define I2S_ICTRL_RFTHIEN_BIT (2)  // [ 2 ..  2] Receive FIFO Threshold Interrupt Enable.
#define I2S_ICTRL_TFURIEN_BIT (1)  // [ 1 ..  1] Transmit FIFO Underrun Interrupt Enable.
#define I2S_ICTRL_RFORIEN_BIT (0)  // [ 0 ..  0] Receive FIFO Overrun Interrupt Enable.
#define I2S_CTRL2_SPIEN_BIT   (0)  // [ 0 ..  0] SPI/I2S enable bit.
#define I2S_CTRL2_RXEN_BIT    (7)  // [ 7 ..  7] Receive function enable bit.
#define I2S_CTRL2_TXEN_BIT    (8)  // [ 8 ..  8] Transmit function enable bit.

#define I2S_STATUS_TFVE_BIT (12) // [17..12] The number of entries in the transmit FIFO waiting to be transmitted
#define I2S_STATUS_RFVE_BIT (4)  // [ 9.. 4] The number of entries in the receive FIFO waiting for DMA or host processor to read them
#define I2S_STATUS_BUSY_BIT (2)  // [ 2.. 2] Busy Indicator
#define I2S_STATUS_TFNF_BIT (1)  // [ 1.. 1] Transmit FIFO Not Full
#define I2S_STATUS_RFF_BIT  (0)  // [ 0.. 0] Receive FIFO Full

#define I2S_STATUS_TFVE_MASK (0x3F) // Describe TFVE by 6 bit
#define I2S_STATUS_RFVE_MASK (0x3F) // Describe RFVE by 6 bit

#define I2S_ICTRL_TFTHOD_MASK  (0x1F << I2S_ICTRL_TFTHOD_BIT)
#define I2S_ICTRL_RFTHOD_MASK  (0x1F << I2S_ICTRL_RFTHOD_BIT)
#define I2S_ICTRL_TFDMAEN_MASK (0x1 << I2S_ICTRL_TFDMAEN_BIT)
#define I2S_ICTRL_RFDMAEN_MASK (0x1 << I2S_ICTRL_RFDMAEN_BIT)
#define I2S_ICTRL_TFTHIEN_MASK (0x1 << I2S_ICTRL_TFTHIEN_BIT)
#define I2S_ICTRL_RFTHIEN_MASK (0x1 << I2S_ICTRL_RFTHIEN_BIT)
#define I2S_CTRL2_SPIEN_MASK   (0x1 << I2S_CTRL2_SPIEN_BIT)
#define I2S_CTRL2_TXEN_MASK    (0x1 << I2S_CTRL2_TXEN_BIT)
#define I2S_CTRL2_RXEN_MASK    (0x1 << I2S_CTRL2_RXEN_BIT)

#define I2S_RIS_TXCI  (5) // [ 5.. 5] TX DATA complete interrupt flag
#define I2S_RIS_TFTHI (3) // [ 3.. 3] TX FIFO threshold Interrupt flag
#define I2S_RIS_RFTHI (2) // [ 2.. 2] RX FIFO threshold interrupt flag
#define I2S_RIS_TFURI (1) // [ 1.. 1] TX FIFO underrun interrupt flag
#define I2S_RIS_RFORI (0) // [ 0.. 0] RX FIFO overrun interrupt flag

#define I2S_TX_FIFO_DEPTH (0x8) // tx FIFO depth
#define I2S_RX_FIFO_DEPTH (0x8) // rx FIFO depth

#define I2S_SYS_FREQ_48M (48 * 1000000) // 48M
#define I2S_SYS_FREQ_96M (96 * 1000000) // 96M

#define I2S_RFTHI_VAL (6)
#define I2S_TFTHI_VAL (2)

/* Private functions prototypes ----------------------------------------------*/
static HAL_Status_T _i2s_set_config(I2S_Handle_T *hi2s);
static void         _i2s_init_callbacks_to_default(I2S_Handle_T *hi2s);
static HAL_Status_T _i2s_it_xfer_kernel(I2S_Handle_T *hi2s, uint8_t *tx_data, uint8_t *rx_data, uint32_t tx_size, uint32_t rx_size);

#if (configUSE_I2S_NONBLOCKING_DMA == 1U)
static HAL_Status_T _i2s_dma_xfer_kernel(I2S_Handle_T *hi2s, uint8_t *tx_data, uint8_t *rx_data, uint32_t tx_size, uint32_t rx_size);
static void         _i2s_dma_abort(I2S_Handle_T *hi2s);
static void         _i2s_tx_dma_cb(DMA_Handle_T *hdma);
static void         _i2s_rx_dma_cb(DMA_Handle_T *hdma);
static void         _i2s_tx_dma_err_cb(DMA_Handle_T *hdma);
static void         _i2s_rx_dma_err_cb(DMA_Handle_T *hdma);
#endif /* configUSE_I2S_NONBLOCKING_DMA */

static inline void _i2s_write_data_1b(I2S_Handle_T *hi2s, uint16_t cnt)
{
    SN_SPI0_Type *reg = hi2s->instance;
    switch (cnt)
    {
        case 8:
        {
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), hi2s->tx.buffer_ptr[0]);
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), hi2s->tx.buffer_ptr[1]);
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), hi2s->tx.buffer_ptr[2]);
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), hi2s->tx.buffer_ptr[3]);
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), hi2s->tx.buffer_ptr[4]);
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), hi2s->tx.buffer_ptr[5]);
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), hi2s->tx.buffer_ptr[6]);
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), hi2s->tx.buffer_ptr[7]);
        }
        break;

        case 7:
        {
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), hi2s->tx.buffer_ptr[0]);
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), hi2s->tx.buffer_ptr[1]);
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), hi2s->tx.buffer_ptr[2]);
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), hi2s->tx.buffer_ptr[3]);
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), hi2s->tx.buffer_ptr[4]);
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), hi2s->tx.buffer_ptr[5]);
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), hi2s->tx.buffer_ptr[6]);
        }
        break;

        case 6:
        {
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), hi2s->tx.buffer_ptr[0]);
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), hi2s->tx.buffer_ptr[1]);
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), hi2s->tx.buffer_ptr[2]);
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), hi2s->tx.buffer_ptr[3]);
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), hi2s->tx.buffer_ptr[4]);
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), hi2s->tx.buffer_ptr[5]);
        }
        break;

        case 5:
        {
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), hi2s->tx.buffer_ptr[0]);
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), hi2s->tx.buffer_ptr[1]);
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), hi2s->tx.buffer_ptr[2]);
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), hi2s->tx.buffer_ptr[3]);
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), hi2s->tx.buffer_ptr[4]);
        }
        break;

        case 4:
        {
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), hi2s->tx.buffer_ptr[0]);
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), hi2s->tx.buffer_ptr[1]);
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), hi2s->tx.buffer_ptr[2]);
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), hi2s->tx.buffer_ptr[3]);
        }
        break;

        case 3:
        {
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), hi2s->tx.buffer_ptr[0]);
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), hi2s->tx.buffer_ptr[1]);
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), hi2s->tx.buffer_ptr[2]);
        }
        break;

        case 2:
        {
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), hi2s->tx.buffer_ptr[0]);
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), hi2s->tx.buffer_ptr[1]);
        }
        break;

        case 1:
        {
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), hi2s->tx.buffer_ptr[0]);
        }
        break;

        default:
            break;
    }
    hi2s->tx.buffer_ptr += cnt;
    hi2s->tx.xfer_count -= cnt;
}

static inline void _i2s_write_data_2b(I2S_Handle_T *hi2s, uint16_t cnt)
{
    SN_SPI0_Type *reg  = hi2s->instance;
    uint16_t     *buff = (uint16_t *)hi2s->tx.buffer_ptr;
    switch (cnt)
    {
        case 8:
        {
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), buff[0]);
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), buff[1]);
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), buff[2]);
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), buff[3]);
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), buff[4]);
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), buff[5]);
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), buff[6]);
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), buff[7]);
        }
        break;

        case 7:
        {
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), buff[0]);
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), buff[1]);
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), buff[2]);
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), buff[3]);
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), buff[4]);
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), buff[5]);
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), buff[6]);
        }
        break;

        case 6:
        {
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), buff[0]);
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), buff[1]);
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), buff[2]);
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), buff[3]);
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), buff[4]);
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), buff[5]);
        }
        break;

        case 5:
        {
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), buff[0]);
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), buff[1]);
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), buff[2]);
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), buff[3]);
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), buff[4]);
        }
        break;

        case 4:
        {
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), buff[0]);
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), buff[1]);
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), buff[2]);
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), buff[3]);
        }
        break;

        case 3:
        {
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), buff[0]);
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), buff[1]);
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), buff[2]);
        }
        break;

        case 2:
        {
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), buff[0]);
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), buff[1]);
        }
        break;

        case 1:
        {
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), buff[0]);
        }
        break;

        default:
            break;
    }
    hi2s->tx.buffer_ptr += cnt * 2;
    hi2s->tx.xfer_count -= cnt;
}

static inline void _i2s_write_data_3b(I2S_Handle_T *hi2s, uint16_t cnt)
{
    SN_SPI0_Type *reg = hi2s->instance;
    switch (cnt)
    {
        case 8:
        {
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), hi2s->tx.buffer_ptr[0] | (hi2s->tx.buffer_ptr[1] << 8) | (hi2s->tx.buffer_ptr[2] << 16));
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), hi2s->tx.buffer_ptr[3] | (hi2s->tx.buffer_ptr[4] << 8) | (hi2s->tx.buffer_ptr[5] << 16));
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), hi2s->tx.buffer_ptr[6] | (hi2s->tx.buffer_ptr[7] << 8) | (hi2s->tx.buffer_ptr[8] << 16));
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), hi2s->tx.buffer_ptr[9] | (hi2s->tx.buffer_ptr[10] << 8) | (hi2s->tx.buffer_ptr[11] << 16));
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), hi2s->tx.buffer_ptr[12] | (hi2s->tx.buffer_ptr[13] << 8) | (hi2s->tx.buffer_ptr[14] << 16));
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), hi2s->tx.buffer_ptr[15] | (hi2s->tx.buffer_ptr[16] << 8) | (hi2s->tx.buffer_ptr[17] << 16));
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), hi2s->tx.buffer_ptr[18] | (hi2s->tx.buffer_ptr[19] << 8) | (hi2s->tx.buffer_ptr[20] << 16));
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), hi2s->tx.buffer_ptr[21] | (hi2s->tx.buffer_ptr[22] << 8) | (hi2s->tx.buffer_ptr[23] << 16));
        }
        break;

        case 7:
        {
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), hi2s->tx.buffer_ptr[0] | (hi2s->tx.buffer_ptr[1] << 8) | (hi2s->tx.buffer_ptr[2] << 16));
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), hi2s->tx.buffer_ptr[3] | (hi2s->tx.buffer_ptr[4] << 8) | (hi2s->tx.buffer_ptr[5] << 16));
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), hi2s->tx.buffer_ptr[6] | (hi2s->tx.buffer_ptr[7] << 8) | (hi2s->tx.buffer_ptr[8] << 16));
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), hi2s->tx.buffer_ptr[9] | (hi2s->tx.buffer_ptr[10] << 8) | (hi2s->tx.buffer_ptr[11] << 16));
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), hi2s->tx.buffer_ptr[12] | (hi2s->tx.buffer_ptr[13] << 8) | (hi2s->tx.buffer_ptr[14] << 16));
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), hi2s->tx.buffer_ptr[15] | (hi2s->tx.buffer_ptr[16] << 8) | (hi2s->tx.buffer_ptr[17] << 16));
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), hi2s->tx.buffer_ptr[18] | (hi2s->tx.buffer_ptr[19] << 8) | (hi2s->tx.buffer_ptr[20] << 16));
        }
        break;

        case 6:
        {
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), hi2s->tx.buffer_ptr[0] | (hi2s->tx.buffer_ptr[1] << 8) | (hi2s->tx.buffer_ptr[2] << 16));
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), hi2s->tx.buffer_ptr[3] | (hi2s->tx.buffer_ptr[4] << 8) | (hi2s->tx.buffer_ptr[5] << 16));
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), hi2s->tx.buffer_ptr[6] | (hi2s->tx.buffer_ptr[7] << 8) | (hi2s->tx.buffer_ptr[8] << 16));
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), hi2s->tx.buffer_ptr[9] | (hi2s->tx.buffer_ptr[10] << 8) | (hi2s->tx.buffer_ptr[11] << 16));
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), hi2s->tx.buffer_ptr[12] | (hi2s->tx.buffer_ptr[13] << 8) | (hi2s->tx.buffer_ptr[14] << 16));
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), hi2s->tx.buffer_ptr[15] | (hi2s->tx.buffer_ptr[16] << 8) | (hi2s->tx.buffer_ptr[17] << 16));
        }
        break;

        case 5:
        {
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), hi2s->tx.buffer_ptr[0] | (hi2s->tx.buffer_ptr[1] << 8) | (hi2s->tx.buffer_ptr[2] << 16));
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), hi2s->tx.buffer_ptr[3] | (hi2s->tx.buffer_ptr[4] << 8) | (hi2s->tx.buffer_ptr[5] << 16));
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), hi2s->tx.buffer_ptr[6] | (hi2s->tx.buffer_ptr[7] << 8) | (hi2s->tx.buffer_ptr[8] << 16));
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), hi2s->tx.buffer_ptr[9] | (hi2s->tx.buffer_ptr[10] << 8) | (hi2s->tx.buffer_ptr[11] << 16));
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), hi2s->tx.buffer_ptr[12] | (hi2s->tx.buffer_ptr[13] << 8) | (hi2s->tx.buffer_ptr[14] << 16));
        }
        break;

        case 4:
        {
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), hi2s->tx.buffer_ptr[0] | (hi2s->tx.buffer_ptr[1] << 8) | (hi2s->tx.buffer_ptr[2] << 16));
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), hi2s->tx.buffer_ptr[3] | (hi2s->tx.buffer_ptr[4] << 8) | (hi2s->tx.buffer_ptr[5] << 16));
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), hi2s->tx.buffer_ptr[6] | (hi2s->tx.buffer_ptr[7] << 8) | (hi2s->tx.buffer_ptr[8] << 16));
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), hi2s->tx.buffer_ptr[9] | (hi2s->tx.buffer_ptr[10] << 8) | (hi2s->tx.buffer_ptr[11] << 16));
        }
        break;

        case 3:
        {
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), hi2s->tx.buffer_ptr[0] | (hi2s->tx.buffer_ptr[1] << 8) | (hi2s->tx.buffer_ptr[2] << 16));
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), hi2s->tx.buffer_ptr[3] | (hi2s->tx.buffer_ptr[4] << 8) | (hi2s->tx.buffer_ptr[5] << 16));
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), hi2s->tx.buffer_ptr[6] | (hi2s->tx.buffer_ptr[7] << 8) | (hi2s->tx.buffer_ptr[8] << 16));
        }
        break;

        case 2:
        {
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), hi2s->tx.buffer_ptr[0] | (hi2s->tx.buffer_ptr[1] << 8) | (hi2s->tx.buffer_ptr[2] << 16));
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), hi2s->tx.buffer_ptr[3] | (hi2s->tx.buffer_ptr[4] << 8) | (hi2s->tx.buffer_ptr[5] << 16));
        }
        break;

        case 1:
        {
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), hi2s->tx.buffer_ptr[0] | (hi2s->tx.buffer_ptr[1] << 8) | (hi2s->tx.buffer_ptr[2] << 16));
        }
        break;

        default:
            break;
    }
    hi2s->tx.buffer_ptr += cnt * 3;
    hi2s->tx.xfer_count -= cnt;
}

static inline void _i2s_write_data_4b(I2S_Handle_T *hi2s, uint16_t cnt)
{
    SN_SPI0_Type *reg  = hi2s->instance;
    uint32_t     *buff = (uint32_t *)hi2s->tx.buffer_ptr;
    switch (cnt)
    {
        case 8:
        {
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), buff[0]);
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), buff[1]);
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), buff[2]);
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), buff[3]);
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), buff[4]);
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), buff[5]);
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), buff[6]);
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), buff[7]);
        }
        break;

        case 7:
        {
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), buff[0]);
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), buff[1]);
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), buff[2]);
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), buff[3]);
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), buff[4]);
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), buff[5]);
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), buff[6]);
        }
        break;

        case 6:
        {
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), buff[0]);
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), buff[1]);
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), buff[2]);
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), buff[3]);
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), buff[4]);
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), buff[5]);
        }
        break;

        case 5:
        {
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), buff[0]);
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), buff[1]);
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), buff[2]);
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), buff[3]);
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), buff[4]);
        }
        break;

        case 4:
        {
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), buff[0]);
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), buff[1]);
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), buff[2]);
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), buff[3]);
        }
        break;

        case 3:
        {
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), buff[0]);
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), buff[1]);
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), buff[2]);
        }
        break;

        case 2:
        {
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), buff[0]);
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), buff[1]);
        }
        break;

        case 1:
        {
            HAL_REG_WRITE(HAL_REG_READ(reg->DATA), buff[0]);
        }
        break;

        default:
            break;
    }
    hi2s->tx.buffer_ptr += cnt * 4;
    hi2s->tx.xfer_count -= cnt;
}

static inline void _i2s_read_data_1b(I2S_Handle_T *hi2s, uint16_t cnt)
{
    SN_SPI0_Type *reg = hi2s->instance;
    switch (cnt)
    {
        case 8:
        {
            hi2s->rx.buffer_ptr[0] = (uint8_t)HAL_REG_READ(reg->DATA);
            hi2s->rx.buffer_ptr[1] = (uint8_t)(HAL_REG_READ(reg->DATA));
            hi2s->rx.buffer_ptr[2] = (uint8_t)(HAL_REG_READ(reg->DATA));
            hi2s->rx.buffer_ptr[3] = (uint8_t)(HAL_REG_READ(reg->DATA));
            hi2s->rx.buffer_ptr[4] = (uint8_t)(HAL_REG_READ(reg->DATA));
            hi2s->rx.buffer_ptr[5] = (uint8_t)(HAL_REG_READ(reg->DATA));
            hi2s->rx.buffer_ptr[6] = (uint8_t)(HAL_REG_READ(reg->DATA));
            hi2s->rx.buffer_ptr[7] = (uint8_t)(HAL_REG_READ(reg->DATA));
        }
        break;

        case 7:
        {
            hi2s->rx.buffer_ptr[0] = (uint8_t)(HAL_REG_READ(reg->DATA));
            hi2s->rx.buffer_ptr[1] = (uint8_t)(HAL_REG_READ(reg->DATA));
            hi2s->rx.buffer_ptr[2] = (uint8_t)(HAL_REG_READ(reg->DATA));
            hi2s->rx.buffer_ptr[3] = (uint8_t)(HAL_REG_READ(reg->DATA));
            hi2s->rx.buffer_ptr[4] = (uint8_t)(HAL_REG_READ(reg->DATA));
            hi2s->rx.buffer_ptr[5] = (uint8_t)(HAL_REG_READ(reg->DATA));
            hi2s->rx.buffer_ptr[6] = (uint8_t)(HAL_REG_READ(reg->DATA));
        }
        break;

        case 6:
        {
            hi2s->rx.buffer_ptr[0] = (uint8_t)(HAL_REG_READ(reg->DATA));
            hi2s->rx.buffer_ptr[1] = (uint8_t)(HAL_REG_READ(reg->DATA));
            hi2s->rx.buffer_ptr[2] = (uint8_t)(HAL_REG_READ(reg->DATA));
            hi2s->rx.buffer_ptr[3] = (uint8_t)(HAL_REG_READ(reg->DATA));
            hi2s->rx.buffer_ptr[4] = (uint8_t)(HAL_REG_READ(reg->DATA));
            hi2s->rx.buffer_ptr[5] = (uint8_t)(HAL_REG_READ(reg->DATA));
        }
        break;

        case 5:
        {
            hi2s->rx.buffer_ptr[0] = (uint8_t)(HAL_REG_READ(reg->DATA));
            hi2s->rx.buffer_ptr[1] = (uint8_t)(HAL_REG_READ(reg->DATA));
            hi2s->rx.buffer_ptr[2] = (uint8_t)(HAL_REG_READ(reg->DATA));
            hi2s->rx.buffer_ptr[3] = (uint8_t)(HAL_REG_READ(reg->DATA));
            hi2s->rx.buffer_ptr[4] = (uint8_t)(HAL_REG_READ(reg->DATA));
        }
        break;

        case 4:
        {
            hi2s->rx.buffer_ptr[0] = (uint8_t)(HAL_REG_READ(reg->DATA));
            hi2s->rx.buffer_ptr[1] = (uint8_t)(HAL_REG_READ(reg->DATA));
            hi2s->rx.buffer_ptr[2] = (uint8_t)(HAL_REG_READ(reg->DATA));
            hi2s->rx.buffer_ptr[3] = (uint8_t)(HAL_REG_READ(reg->DATA));
        }
        break;

        case 3:
        {
            hi2s->rx.buffer_ptr[0] = (uint8_t)(HAL_REG_READ(reg->DATA));
            hi2s->rx.buffer_ptr[1] = (uint8_t)(HAL_REG_READ(reg->DATA));
            hi2s->rx.buffer_ptr[2] = (uint8_t)(HAL_REG_READ(reg->DATA));
        }
        break;

        case 2:
        {
            hi2s->rx.buffer_ptr[0] = (uint8_t)(HAL_REG_READ(reg->DATA));
            hi2s->rx.buffer_ptr[1] = (uint8_t)(HAL_REG_READ(reg->DATA));
        }
        break;

        case 1:
        {
            hi2s->rx.buffer_ptr[0] = (uint8_t)(HAL_REG_READ(reg->DATA));
        }
        break;

        default:
            break;
    }
    hi2s->rx.buffer_ptr += cnt;
    hi2s->rx.xfer_count -= cnt;
}

static inline void _i2s_read_data_2b(I2S_Handle_T *hi2s, uint16_t cnt)
{
    SN_SPI0_Type *reg = hi2s->instance;
    switch (cnt)
    {
        case 8:
        {
            ((uint16_t *)hi2s->rx.buffer_ptr)[0] = (uint16_t)HAL_REG_READ(reg->DATA);
            ((uint16_t *)hi2s->rx.buffer_ptr)[1] = (uint16_t)HAL_REG_READ(reg->DATA);
            ((uint16_t *)hi2s->rx.buffer_ptr)[2] = (uint16_t)HAL_REG_READ(reg->DATA);
            ((uint16_t *)hi2s->rx.buffer_ptr)[3] = (uint16_t)HAL_REG_READ(reg->DATA);
            ((uint16_t *)hi2s->rx.buffer_ptr)[4] = (uint16_t)HAL_REG_READ(reg->DATA);
            ((uint16_t *)hi2s->rx.buffer_ptr)[5] = (uint16_t)HAL_REG_READ(reg->DATA);
            ((uint16_t *)hi2s->rx.buffer_ptr)[6] = (uint16_t)HAL_REG_READ(reg->DATA);
            ((uint16_t *)hi2s->rx.buffer_ptr)[7] = (uint16_t)HAL_REG_READ(reg->DATA);
        }
        break;

        case 7:
        {
            ((uint16_t *)hi2s->rx.buffer_ptr)[0] = (uint16_t)HAL_REG_READ(reg->DATA);
            ((uint16_t *)hi2s->rx.buffer_ptr)[1] = (uint16_t)HAL_REG_READ(reg->DATA);
            ((uint16_t *)hi2s->rx.buffer_ptr)[2] = (uint16_t)HAL_REG_READ(reg->DATA);
            ((uint16_t *)hi2s->rx.buffer_ptr)[3] = (uint16_t)HAL_REG_READ(reg->DATA);
            ((uint16_t *)hi2s->rx.buffer_ptr)[4] = (uint16_t)HAL_REG_READ(reg->DATA);
            ((uint16_t *)hi2s->rx.buffer_ptr)[5] = (uint16_t)HAL_REG_READ(reg->DATA);
            ((uint16_t *)hi2s->rx.buffer_ptr)[6] = (uint16_t)HAL_REG_READ(reg->DATA);
        }
        break;

        case 6:
        {
            ((uint16_t *)hi2s->rx.buffer_ptr)[0] = (uint16_t)HAL_REG_READ(reg->DATA);
            ((uint16_t *)hi2s->rx.buffer_ptr)[1] = (uint16_t)HAL_REG_READ(reg->DATA);
            ((uint16_t *)hi2s->rx.buffer_ptr)[2] = (uint16_t)HAL_REG_READ(reg->DATA);
            ((uint16_t *)hi2s->rx.buffer_ptr)[3] = (uint16_t)HAL_REG_READ(reg->DATA);
            ((uint16_t *)hi2s->rx.buffer_ptr)[4] = (uint16_t)HAL_REG_READ(reg->DATA);
            ((uint16_t *)hi2s->rx.buffer_ptr)[5] = (uint16_t)HAL_REG_READ(reg->DATA);
        }
        break;

        case 5:
        {
            ((uint16_t *)hi2s->rx.buffer_ptr)[0] = (uint16_t)HAL_REG_READ(reg->DATA);
            ((uint16_t *)hi2s->rx.buffer_ptr)[1] = (uint16_t)HAL_REG_READ(reg->DATA);
            ((uint16_t *)hi2s->rx.buffer_ptr)[2] = (uint16_t)HAL_REG_READ(reg->DATA);
            ((uint16_t *)hi2s->rx.buffer_ptr)[3] = (uint16_t)HAL_REG_READ(reg->DATA);
            ((uint16_t *)hi2s->rx.buffer_ptr)[4] = (uint16_t)HAL_REG_READ(reg->DATA);
        }
        break;

        case 4:
        {
            ((uint16_t *)hi2s->rx.buffer_ptr)[0] = (uint16_t)HAL_REG_READ(reg->DATA);
            ((uint16_t *)hi2s->rx.buffer_ptr)[1] = (uint16_t)HAL_REG_READ(reg->DATA);
            ((uint16_t *)hi2s->rx.buffer_ptr)[2] = (uint16_t)HAL_REG_READ(reg->DATA);
            ((uint16_t *)hi2s->rx.buffer_ptr)[3] = (uint16_t)HAL_REG_READ(reg->DATA);
        }
        break;

        case 3:
        {
            ((uint16_t *)hi2s->rx.buffer_ptr)[0] = (uint16_t)HAL_REG_READ(reg->DATA);
            ((uint16_t *)hi2s->rx.buffer_ptr)[1] = (uint16_t)HAL_REG_READ(reg->DATA);
            ((uint16_t *)hi2s->rx.buffer_ptr)[2] = (uint16_t)HAL_REG_READ(reg->DATA);
        }
        break;

        case 2:
        {
            ((uint16_t *)hi2s->rx.buffer_ptr)[0] = (uint16_t)HAL_REG_READ(reg->DATA);
            ((uint16_t *)hi2s->rx.buffer_ptr)[1] = (uint16_t)HAL_REG_READ(reg->DATA);
        }
        break;

        case 1:
        {
            ((uint16_t *)hi2s->rx.buffer_ptr)[0] = (uint16_t)HAL_REG_READ(reg->DATA);
        }
        break;

        default:
            break;
    }
    hi2s->rx.buffer_ptr += cnt * 2;
    hi2s->rx.xfer_count -= cnt;
}

static inline void _i2s_read_data_3b(I2S_Handle_T *hi2s, uint16_t cnt)
{
    SN_SPI0_Type *reg = hi2s->instance;
    switch (cnt)
    {
        case 8:
        {
            hi2s->rx.temp          = HAL_REG_READ(reg->DATA);
            hi2s->rx.buffer_ptr[0] = (uint8_t)((hi2s->rx.temp & 0x000000FF) >> 0);
            hi2s->rx.buffer_ptr[1] = (uint8_t)((hi2s->rx.temp & 0x0000FF00) >> 8);
            hi2s->rx.buffer_ptr[2] = (uint8_t)((hi2s->rx.temp & 0x00FF0000) >> 16);

            hi2s->rx.temp          = HAL_REG_READ(reg->DATA);
            hi2s->rx.buffer_ptr[3] = (uint8_t)((hi2s->rx.temp & 0x000000FF) >> 0);
            hi2s->rx.buffer_ptr[4] = (uint8_t)((hi2s->rx.temp & 0x0000FF00) >> 8);
            hi2s->rx.buffer_ptr[5] = (uint8_t)((hi2s->rx.temp & 0x00FF0000) >> 16);

            hi2s->rx.temp          = HAL_REG_READ(reg->DATA);
            hi2s->rx.buffer_ptr[6] = (uint8_t)((hi2s->rx.temp & 0x000000FF) >> 0);
            hi2s->rx.buffer_ptr[7] = (uint8_t)((hi2s->rx.temp & 0x0000FF00) >> 8);
            hi2s->rx.buffer_ptr[8] = (uint8_t)((hi2s->rx.temp & 0x00FF0000) >> 16);

            hi2s->rx.temp           = HAL_REG_READ(reg->DATA);
            hi2s->rx.buffer_ptr[9]  = (uint8_t)((hi2s->rx.temp & 0x000000FF) >> 0);
            hi2s->rx.buffer_ptr[10] = (uint8_t)((hi2s->rx.temp & 0x0000FF00) >> 8);
            hi2s->rx.buffer_ptr[11] = (uint8_t)((hi2s->rx.temp & 0x00FF0000) >> 16);

            hi2s->rx.temp           = HAL_REG_READ(reg->DATA);
            hi2s->rx.buffer_ptr[12] = (uint8_t)((hi2s->rx.temp & 0x000000FF) >> 0);
            hi2s->rx.buffer_ptr[13] = (uint8_t)((hi2s->rx.temp & 0x0000FF00) >> 8);
            hi2s->rx.buffer_ptr[14] = (uint8_t)((hi2s->rx.temp & 0x00FF0000) >> 16);

            hi2s->rx.temp           = HAL_REG_READ(reg->DATA);
            hi2s->rx.buffer_ptr[15] = (uint8_t)((hi2s->rx.temp & 0x000000FF) >> 0);
            hi2s->rx.buffer_ptr[16] = (uint8_t)((hi2s->rx.temp & 0x0000FF00) >> 8);
            hi2s->rx.buffer_ptr[17] = (uint8_t)((hi2s->rx.temp & 0x00FF0000) >> 16);

            hi2s->rx.temp           = HAL_REG_READ(reg->DATA);
            hi2s->rx.buffer_ptr[18] = (uint8_t)((hi2s->rx.temp & 0x000000FF) >> 0);
            hi2s->rx.buffer_ptr[19] = (uint8_t)((hi2s->rx.temp & 0x0000FF00) >> 8);
            hi2s->rx.buffer_ptr[20] = (uint8_t)((hi2s->rx.temp & 0x00FF0000) >> 16);

            hi2s->rx.temp           = HAL_REG_READ(reg->DATA);
            hi2s->rx.buffer_ptr[21] = (uint8_t)((hi2s->rx.temp & 0x000000FF) >> 0);
            hi2s->rx.buffer_ptr[22] = (uint8_t)((hi2s->rx.temp & 0x0000FF00) >> 8);
            hi2s->rx.buffer_ptr[23] = (uint8_t)((hi2s->rx.temp & 0x00FF0000) >> 16);
        }
        break;

        case 7:
        {
            hi2s->rx.temp          = HAL_REG_READ(reg->DATA);
            hi2s->rx.buffer_ptr[0] = (uint8_t)((hi2s->rx.temp & 0x000000FF) >> 0);
            hi2s->rx.buffer_ptr[1] = (uint8_t)((hi2s->rx.temp & 0x0000FF00) >> 8);
            hi2s->rx.buffer_ptr[2] = (uint8_t)((hi2s->rx.temp & 0x00FF0000) >> 16);

            hi2s->rx.temp          = HAL_REG_READ(reg->DATA);
            hi2s->rx.buffer_ptr[3] = (uint8_t)((hi2s->rx.temp & 0x000000FF) >> 0);
            hi2s->rx.buffer_ptr[4] = (uint8_t)((hi2s->rx.temp & 0x0000FF00) >> 8);
            hi2s->rx.buffer_ptr[5] = (uint8_t)((hi2s->rx.temp & 0x00FF0000) >> 16);

            hi2s->rx.temp          = HAL_REG_READ(reg->DATA);
            hi2s->rx.buffer_ptr[6] = (uint8_t)((hi2s->rx.temp & 0x000000FF) >> 0);
            hi2s->rx.buffer_ptr[7] = (uint8_t)((hi2s->rx.temp & 0x0000FF00) >> 8);
            hi2s->rx.buffer_ptr[8] = (uint8_t)((hi2s->rx.temp & 0x00FF0000) >> 16);

            hi2s->rx.temp           = HAL_REG_READ(reg->DATA);
            hi2s->rx.buffer_ptr[9]  = (uint8_t)((hi2s->rx.temp & 0x000000FF) >> 0);
            hi2s->rx.buffer_ptr[10] = (uint8_t)((hi2s->rx.temp & 0x0000FF00) >> 8);
            hi2s->rx.buffer_ptr[11] = (uint8_t)((hi2s->rx.temp & 0x00FF0000) >> 16);

            hi2s->rx.temp           = HAL_REG_READ(reg->DATA);
            hi2s->rx.buffer_ptr[12] = (uint8_t)((hi2s->rx.temp & 0x000000FF) >> 0);
            hi2s->rx.buffer_ptr[13] = (uint8_t)((hi2s->rx.temp & 0x0000FF00) >> 8);
            hi2s->rx.buffer_ptr[14] = (uint8_t)((hi2s->rx.temp & 0x00FF0000) >> 16);

            hi2s->rx.temp           = HAL_REG_READ(reg->DATA);
            hi2s->rx.buffer_ptr[15] = (uint8_t)((hi2s->rx.temp & 0x000000FF) >> 0);
            hi2s->rx.buffer_ptr[16] = (uint8_t)((hi2s->rx.temp & 0x0000FF00) >> 8);
            hi2s->rx.buffer_ptr[17] = (uint8_t)((hi2s->rx.temp & 0x00FF0000) >> 16);

            hi2s->rx.temp           = HAL_REG_READ(reg->DATA);
            hi2s->rx.buffer_ptr[18] = (uint8_t)((hi2s->rx.temp & 0x000000FF) >> 0);
            hi2s->rx.buffer_ptr[19] = (uint8_t)((hi2s->rx.temp & 0x0000FF00) >> 8);
            hi2s->rx.buffer_ptr[20] = (uint8_t)((hi2s->rx.temp & 0x00FF0000) >> 16);
        }
        break;

        case 6:
        {
            hi2s->rx.temp          = HAL_REG_READ(reg->DATA);
            hi2s->rx.buffer_ptr[0] = (uint8_t)((hi2s->rx.temp & 0x000000FF) >> 0);
            hi2s->rx.buffer_ptr[1] = (uint8_t)((hi2s->rx.temp & 0x0000FF00) >> 8);
            hi2s->rx.buffer_ptr[2] = (uint8_t)((hi2s->rx.temp & 0x00FF0000) >> 16);

            hi2s->rx.temp          = HAL_REG_READ(reg->DATA);
            hi2s->rx.buffer_ptr[3] = (uint8_t)((hi2s->rx.temp & 0x000000FF) >> 0);
            hi2s->rx.buffer_ptr[4] = (uint8_t)((hi2s->rx.temp & 0x0000FF00) >> 8);
            hi2s->rx.buffer_ptr[5] = (uint8_t)((hi2s->rx.temp & 0x00FF0000) >> 16);

            hi2s->rx.temp          = HAL_REG_READ(reg->DATA);
            hi2s->rx.buffer_ptr[6] = (uint8_t)((hi2s->rx.temp & 0x000000FF) >> 0);
            hi2s->rx.buffer_ptr[7] = (uint8_t)((hi2s->rx.temp & 0x0000FF00) >> 8);
            hi2s->rx.buffer_ptr[8] = (uint8_t)((hi2s->rx.temp & 0x00FF0000) >> 16);

            hi2s->rx.temp           = HAL_REG_READ(reg->DATA);
            hi2s->rx.buffer_ptr[9]  = (uint8_t)((hi2s->rx.temp & 0x000000FF) >> 0);
            hi2s->rx.buffer_ptr[10] = (uint8_t)((hi2s->rx.temp & 0x0000FF00) >> 8);
            hi2s->rx.buffer_ptr[11] = (uint8_t)((hi2s->rx.temp & 0x00FF0000) >> 16);

            hi2s->rx.temp           = HAL_REG_READ(reg->DATA);
            hi2s->rx.buffer_ptr[12] = (uint8_t)((hi2s->rx.temp & 0x000000FF) >> 0);
            hi2s->rx.buffer_ptr[13] = (uint8_t)((hi2s->rx.temp & 0x0000FF00) >> 8);
            hi2s->rx.buffer_ptr[14] = (uint8_t)((hi2s->rx.temp & 0x00FF0000) >> 16);

            hi2s->rx.temp           = HAL_REG_READ(reg->DATA);
            hi2s->rx.buffer_ptr[15] = (uint8_t)((hi2s->rx.temp & 0x000000FF) >> 0);
            hi2s->rx.buffer_ptr[16] = (uint8_t)((hi2s->rx.temp & 0x0000FF00) >> 8);
            hi2s->rx.buffer_ptr[17] = (uint8_t)((hi2s->rx.temp & 0x00FF0000) >> 16);
        }
        break;

        case 5:
        {
            hi2s->rx.temp          = HAL_REG_READ(reg->DATA);
            hi2s->rx.buffer_ptr[0] = (uint8_t)((hi2s->rx.temp & 0x000000FF) >> 0);
            hi2s->rx.buffer_ptr[1] = (uint8_t)((hi2s->rx.temp & 0x0000FF00) >> 8);
            hi2s->rx.buffer_ptr[2] = (uint8_t)((hi2s->rx.temp & 0x00FF0000) >> 16);

            hi2s->rx.temp          = HAL_REG_READ(reg->DATA);
            hi2s->rx.buffer_ptr[3] = (uint8_t)((hi2s->rx.temp & 0x000000FF) >> 0);
            hi2s->rx.buffer_ptr[4] = (uint8_t)((hi2s->rx.temp & 0x0000FF00) >> 8);
            hi2s->rx.buffer_ptr[5] = (uint8_t)((hi2s->rx.temp & 0x00FF0000) >> 16);

            hi2s->rx.temp          = HAL_REG_READ(reg->DATA);
            hi2s->rx.buffer_ptr[6] = (uint8_t)((hi2s->rx.temp & 0x000000FF) >> 0);
            hi2s->rx.buffer_ptr[7] = (uint8_t)((hi2s->rx.temp & 0x0000FF00) >> 8);
            hi2s->rx.buffer_ptr[8] = (uint8_t)((hi2s->rx.temp & 0x00FF0000) >> 16);

            hi2s->rx.temp           = HAL_REG_READ(reg->DATA);
            hi2s->rx.buffer_ptr[9]  = (uint8_t)((hi2s->rx.temp & 0x000000FF) >> 0);
            hi2s->rx.buffer_ptr[10] = (uint8_t)((hi2s->rx.temp & 0x0000FF00) >> 8);
            hi2s->rx.buffer_ptr[11] = (uint8_t)((hi2s->rx.temp & 0x00FF0000) >> 16);

            hi2s->rx.temp           = HAL_REG_READ(reg->DATA);
            hi2s->rx.buffer_ptr[12] = (uint8_t)((hi2s->rx.temp & 0x000000FF) >> 0);
            hi2s->rx.buffer_ptr[13] = (uint8_t)((hi2s->rx.temp & 0x0000FF00) >> 8);
            hi2s->rx.buffer_ptr[14] = (uint8_t)((hi2s->rx.temp & 0x00FF0000) >> 16);
        }
        break;

        case 4:
        {
            hi2s->rx.temp          = HAL_REG_READ(reg->DATA);
            hi2s->rx.buffer_ptr[0] = (uint8_t)((hi2s->rx.temp & 0x000000FF) >> 0);
            hi2s->rx.buffer_ptr[1] = (uint8_t)((hi2s->rx.temp & 0x0000FF00) >> 8);
            hi2s->rx.buffer_ptr[2] = (uint8_t)((hi2s->rx.temp & 0x00FF0000) >> 16);

            hi2s->rx.temp          = HAL_REG_READ(reg->DATA);
            hi2s->rx.buffer_ptr[3] = (uint8_t)((hi2s->rx.temp & 0x000000FF) >> 0);
            hi2s->rx.buffer_ptr[4] = (uint8_t)((hi2s->rx.temp & 0x0000FF00) >> 8);
            hi2s->rx.buffer_ptr[5] = (uint8_t)((hi2s->rx.temp & 0x00FF0000) >> 16);

            hi2s->rx.temp          = HAL_REG_READ(reg->DATA);
            hi2s->rx.buffer_ptr[6] = (uint8_t)((hi2s->rx.temp & 0x000000FF) >> 0);
            hi2s->rx.buffer_ptr[7] = (uint8_t)((hi2s->rx.temp & 0x0000FF00) >> 8);
            hi2s->rx.buffer_ptr[8] = (uint8_t)((hi2s->rx.temp & 0x00FF0000) >> 16);

            hi2s->rx.temp           = HAL_REG_READ(reg->DATA);
            hi2s->rx.buffer_ptr[9]  = (uint8_t)((hi2s->rx.temp & 0x000000FF) >> 0);
            hi2s->rx.buffer_ptr[10] = (uint8_t)((hi2s->rx.temp & 0x0000FF00) >> 8);
            hi2s->rx.buffer_ptr[11] = (uint8_t)((hi2s->rx.temp & 0x00FF0000) >> 16);
        }
        break;

        case 3:
        {
            hi2s->rx.temp          = HAL_REG_READ(reg->DATA);
            hi2s->rx.buffer_ptr[0] = (uint8_t)((hi2s->rx.temp & 0x000000FF) >> 0);
            hi2s->rx.buffer_ptr[1] = (uint8_t)((hi2s->rx.temp & 0x0000FF00) >> 8);
            hi2s->rx.buffer_ptr[2] = (uint8_t)((hi2s->rx.temp & 0x00FF0000) >> 16);

            hi2s->rx.temp          = HAL_REG_READ(reg->DATA);
            hi2s->rx.buffer_ptr[3] = (uint8_t)((hi2s->rx.temp & 0x000000FF) >> 0);
            hi2s->rx.buffer_ptr[4] = (uint8_t)((hi2s->rx.temp & 0x0000FF00) >> 8);
            hi2s->rx.buffer_ptr[5] = (uint8_t)((hi2s->rx.temp & 0x00FF0000) >> 16);

            hi2s->rx.temp          = HAL_REG_READ(reg->DATA);
            hi2s->rx.buffer_ptr[6] = (uint8_t)((hi2s->rx.temp & 0x000000FF) >> 0);
            hi2s->rx.buffer_ptr[7] = (uint8_t)((hi2s->rx.temp & 0x0000FF00) >> 8);
            hi2s->rx.buffer_ptr[8] = (uint8_t)((hi2s->rx.temp & 0x00FF0000) >> 16);
        }
        break;

        case 2:
        {
            hi2s->rx.temp          = HAL_REG_READ(reg->DATA);
            hi2s->rx.buffer_ptr[0] = (uint8_t)((hi2s->rx.temp & 0x000000FF) >> 0);
            hi2s->rx.buffer_ptr[1] = (uint8_t)((hi2s->rx.temp & 0x0000FF00) >> 8);
            hi2s->rx.buffer_ptr[2] = (uint8_t)((hi2s->rx.temp & 0x00FF0000) >> 16);

            hi2s->rx.temp          = HAL_REG_READ(reg->DATA);
            hi2s->rx.buffer_ptr[3] = (uint8_t)((hi2s->rx.temp & 0x000000FF) >> 0);
            hi2s->rx.buffer_ptr[4] = (uint8_t)((hi2s->rx.temp & 0x0000FF00) >> 8);
            hi2s->rx.buffer_ptr[5] = (uint8_t)((hi2s->rx.temp & 0x00FF0000) >> 16);
        }
        break;

        case 1:
        {
            hi2s->rx.temp          = HAL_REG_READ(reg->DATA);
            hi2s->rx.buffer_ptr[0] = (uint8_t)((hi2s->rx.temp & 0x000000FF) >> 0);
            hi2s->rx.buffer_ptr[1] = (uint8_t)((hi2s->rx.temp & 0x0000FF00) >> 8);
            hi2s->rx.buffer_ptr[2] = (uint8_t)((hi2s->rx.temp & 0x00FF0000) >> 16);
        }
        break;

        default:
            break;
    }
    hi2s->rx.buffer_ptr += cnt * 3;
    hi2s->rx.xfer_count -= cnt;
}

static inline void _i2s_read_data_4b(I2S_Handle_T *hi2s, uint16_t cnt)
{
    SN_SPI0_Type *reg = hi2s->instance;
    switch (cnt)
    {
        case 8:
        {
            ((uint32_t *)hi2s->rx.buffer_ptr)[0] = (uint32_t)HAL_REG_READ(reg->DATA);
            ((uint32_t *)hi2s->rx.buffer_ptr)[1] = (uint32_t)HAL_REG_READ(reg->DATA);
            ((uint32_t *)hi2s->rx.buffer_ptr)[2] = (uint32_t)HAL_REG_READ(reg->DATA);
            ((uint32_t *)hi2s->rx.buffer_ptr)[3] = (uint32_t)HAL_REG_READ(reg->DATA);
            ((uint32_t *)hi2s->rx.buffer_ptr)[4] = (uint32_t)HAL_REG_READ(reg->DATA);
            ((uint32_t *)hi2s->rx.buffer_ptr)[5] = (uint32_t)HAL_REG_READ(reg->DATA);
            ((uint32_t *)hi2s->rx.buffer_ptr)[6] = (uint32_t)HAL_REG_READ(reg->DATA);
            ((uint32_t *)hi2s->rx.buffer_ptr)[7] = (uint32_t)HAL_REG_READ(reg->DATA);
        }
        break;

        case 7:
        {
            ((uint32_t *)hi2s->rx.buffer_ptr)[0] = (uint32_t)HAL_REG_READ(reg->DATA);
            ((uint32_t *)hi2s->rx.buffer_ptr)[1] = (uint32_t)HAL_REG_READ(reg->DATA);
            ((uint32_t *)hi2s->rx.buffer_ptr)[2] = (uint32_t)HAL_REG_READ(reg->DATA);
            ((uint32_t *)hi2s->rx.buffer_ptr)[3] = (uint32_t)HAL_REG_READ(reg->DATA);
            ((uint32_t *)hi2s->rx.buffer_ptr)[4] = (uint32_t)HAL_REG_READ(reg->DATA);
            ((uint32_t *)hi2s->rx.buffer_ptr)[5] = (uint32_t)HAL_REG_READ(reg->DATA);
            ((uint32_t *)hi2s->rx.buffer_ptr)[6] = (uint32_t)HAL_REG_READ(reg->DATA);
        }
        break;

        case 6:
        {
            ((uint32_t *)hi2s->rx.buffer_ptr)[0] = (uint32_t)HAL_REG_READ(reg->DATA);
            ((uint32_t *)hi2s->rx.buffer_ptr)[1] = (uint32_t)HAL_REG_READ(reg->DATA);
            ((uint32_t *)hi2s->rx.buffer_ptr)[2] = (uint32_t)HAL_REG_READ(reg->DATA);
            ((uint32_t *)hi2s->rx.buffer_ptr)[3] = (uint32_t)HAL_REG_READ(reg->DATA);
            ((uint32_t *)hi2s->rx.buffer_ptr)[4] = (uint32_t)HAL_REG_READ(reg->DATA);
            ((uint32_t *)hi2s->rx.buffer_ptr)[5] = (uint32_t)HAL_REG_READ(reg->DATA);
        }
        break;

        case 5:
        {
            ((uint32_t *)hi2s->rx.buffer_ptr)[0] = (uint32_t)HAL_REG_READ(reg->DATA);
            ((uint32_t *)hi2s->rx.buffer_ptr)[1] = (uint32_t)HAL_REG_READ(reg->DATA);
            ((uint32_t *)hi2s->rx.buffer_ptr)[2] = (uint32_t)HAL_REG_READ(reg->DATA);
            ((uint32_t *)hi2s->rx.buffer_ptr)[3] = (uint32_t)HAL_REG_READ(reg->DATA);
            ((uint32_t *)hi2s->rx.buffer_ptr)[4] = (uint32_t)HAL_REG_READ(reg->DATA);
        }
        break;

        case 4:
        {
            ((uint32_t *)hi2s->rx.buffer_ptr)[0] = (uint32_t)HAL_REG_READ(reg->DATA);
            ((uint32_t *)hi2s->rx.buffer_ptr)[1] = (uint32_t)HAL_REG_READ(reg->DATA);
            ((uint32_t *)hi2s->rx.buffer_ptr)[2] = (uint32_t)HAL_REG_READ(reg->DATA);
            ((uint32_t *)hi2s->rx.buffer_ptr)[3] = (uint32_t)HAL_REG_READ(reg->DATA);
        }
        break;

        case 3:
        {
            ((uint32_t *)hi2s->rx.buffer_ptr)[0] = (uint32_t)HAL_REG_READ(reg->DATA);
            ((uint32_t *)hi2s->rx.buffer_ptr)[1] = (uint32_t)HAL_REG_READ(reg->DATA);
            ((uint32_t *)hi2s->rx.buffer_ptr)[2] = (uint32_t)HAL_REG_READ(reg->DATA);
        }
        break;

        case 2:
        {
            ((uint32_t *)hi2s->rx.buffer_ptr)[0] = (uint32_t)HAL_REG_READ(reg->DATA);
            ((uint32_t *)hi2s->rx.buffer_ptr)[1] = (uint32_t)HAL_REG_READ(reg->DATA);
        }
        break;

        case 1:
        {
            ((uint32_t *)hi2s->rx.buffer_ptr)[0] = (uint32_t)HAL_REG_READ(reg->DATA);
        }
        break;

        default:
            break;
    }
    hi2s->rx.buffer_ptr += cnt * 4;
    hi2s->rx.xfer_count -= cnt;
}

static inline void _i2s_xfer_info(I2S_Handle_T *hi2s, uint8_t *tx_data, uint8_t *rx_data, uint32_t tx_size, uint32_t rx_size)
{
    /* clear error code*/
    CLEAR_ERROR_CODE(hi2s);

    /* reset fifo */
    I2S_RESET_FIFO(hi2s);
    HAL_REG_WRITE(hi2s->instance->CTRL2_b.RESET, 1);

    /* Set TX Data */
    hi2s->tx.buffer_ptr = (void *)tx_data;
    hi2s->tx.xfer_size  = tx_size;
    hi2s->tx.xfer_count = tx_size;

    /* Set RX Data */
    hi2s->rx.buffer_ptr = (void *)rx_data;
    hi2s->rx.xfer_size  = rx_size;
    hi2s->rx.xfer_count = rx_size;
}

#endif /* _SN34F78X_HAL_I2S_H_ */
