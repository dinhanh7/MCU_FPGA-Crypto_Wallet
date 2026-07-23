/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _LL_SSP_H_
#define _LL_SSP_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sn34f78x_ll_def.h"
#include "ll_reg_msk.h"

/* Exported types ------------------------------------------------------------*/
/**
 * @brief SPI Init Structure Definition
 * \ingroup spi_struct_define
 */
typedef struct
{
    uint32_t frame_format; /**< Specifies if the NS micro-wire mode is enabled or not.
                                This parameter can be a value of @ref spi_frame_format */
    uint32_t mode;         /**< Specifies the SSP operating mode.
                                This parameter can be a value of @ref spi_mode */
    uint32_t data_size;    /**< Specifies the SSP data size.
                                This parameter must be a number of @ref spi_data_size  */
    uint32_t fs_polarity;  /**< Specifies the Frame/Sync. polarity for the SSP mode.
                                This parameter can be a value of @ref ssp_fs_polarity */
    uint32_t clk_polarity; /**< Specifies the serial clock steady state.
                                This parameter can be a value of @ref ssp_clk_polarity */
    uint32_t clk_phase;    /**< Specifies the clock active edge for the bit capture.
                                This parameter can be a value of @ref ssp_clk_phase */
    uint32_t sclk_div;     /**< Specifies communication clock divider value which will be
                                used to configure the transmit and receive SCK clock.
                                This parameter must be an number between "min_data = 2(SPI1, SPI2) or min_data = 1(SPI0)" and "max_data = 512"
                                @note The communication clock is derived from the master clock. The slave clock does not need to be set.
                                @note Fclk = Fpclk / (2 * sclk_div + 1). */
    uint32_t first_bit;    /**< Specifies whether data transfers start from MSB or LSB bit.
                                This parameter can be a value of @ref ssp_first_bit */
    uint16_t tx_default;   /**< Specifies the master tx default value when master receive only.
                                This parameter must be an number between min_data = 0 and max_data = 0xFFFF */
} LL_SPI_Init_T;

/**
 * @brief I2S Init Structure Definition
 * \ingroup i2s_struct_define
 */
typedef struct
{
    uint32_t mode;        /**< Specifies the I2S operating mode.
                               This parameter can be a value of @ref ssp_mode */
    uint32_t standard;    /**< Specifies the standard used for the I2S communication.
                               This parameter can be a value of @ref i2s_standard */
    uint32_t data_format; /**< Specifies the data length for the I2S communication.
                               This parameter can be a value in range of @ref i2s_data_format.*/
    uint32_t channel_len; /**< Specifies the channel length for the I2S communication.
                               This parameter must be an number between min_data = 16 and max_data = 32.
                               @note Must channel_len >= data_len  */
    uint32_t audio_freq;  /**< Specifies the I2S audio frequency.
                               This parameter can be a value of @ref i2s_audio_freq */
    uint32_t cpol;        /**< Specifies the idle state of the I2S clock.
                               This parameter can be a value of @ref i2s_cpol */
} LL_I2S_Init_T;

/* Exported constants --------------------------------------------------------*/

/**
 * @defgroup spi_frame_format SPI frame format
 * \ingroup spi_control
 * @{
 */
#define LL_SPI_FFMT_SPI (0x00000001U) /**< SSP mode enable */
#define LL_SPI_FFMT_MWR (0x00000002U) /**< Micro-wire mode enable */
/**
 * @}
 */

/**
 * @defgroup spi_mode SPI mode
 * \ingroup spi_control
 * @{
 */
#define LL_SPI_MODE_MASTER LL_SSP_MODE_MASTER_MONO /**< SSP mode enable */
#define LL_SPI_MODE_SLAVE  LL_SSP_MODE_SLAVE_MONO  /**< Micro-wire mode enable */
/**
 * @}
 */

/**
 * @defgroup spi_data_size SPI Data Size
 * \ingroup spi_control
 * @{
 */
#define LL_SPI_DATASIZE_5BIT  (0x00000005U) /**< SSP bit length of a transmitted/received is 5bit */
#define LL_SPI_DATASIZE_6BIT  (0x00000006U) /**< SSP bit length of a transmitted/received is 6bit */
#define LL_SPI_DATASIZE_7BIT  (0x00000007U) /**< SSP bit length of a transmitted/received is 7bit */
#define LL_SPI_DATASIZE_8BIT  (0x00000008U) /**< SSP bit length of a transmitted/received is 8bit */
#define LL_SPI_DATASIZE_9BIT  (0x00000009U) /**< SSP bit length of a transmitted/received is 9bit */
#define LL_SPI_DATASIZE_10BIT (0x0000000AU) /**< SSP bit length of a transmitted/received is 10bit */
#define LL_SPI_DATASIZE_11BIT (0x0000000BU) /**< SSP bit length of a transmitted/received is 11bit */
#define LL_SPI_DATASIZE_12BIT (0x0000000CU) /**< SSP bit length of a transmitted/received is 12bit */
#define LL_SPI_DATASIZE_13BIT (0x0000000DU) /**< SSP bit length of a transmitted/received is 13bit */
#define LL_SPI_DATASIZE_14BIT (0x0000000EU) /**< SSP bit length of a transmitted/received is 14bit */
#define LL_SPI_DATASIZE_15BIT (0x0000000FU) /**< SSP bit length of a transmitted/received is 15bit */
#define LL_SPI_DATASIZE_16BIT (0x00000010U) /**< SSP bit length of a transmitted/received is 16bit */
/**
 * @}
 */

/**
 * @defgroup ssp_fs_polarity SPI Frame/Sync active polarity
 * \ingroup spi_control
 * @{
 */
#define LL_SSP_FS_POLARITY_LOW  (0x00000000U)            /**< Frame/Sync active at low  level */
#define LL_SSP_FS_POLARITY_HIGH (SSP_CTRL0_SPIFSPO_HIGH) /**< Frame/Sync active at high level */
/**
 * @}
 */

/**
 * @defgroup ssp_clk_polarity SPI clock polarity
 * \ingroup spi_control
 * @{
 */
#define LL_SSP_CLK_POLARITY_LOW  (0x00000000U)           /**<  SCLK will remain Low when SPI is idle */
#define LL_SSP_CLK_POLARITY_HIGH (SSP_CTRL0_SCLKPO_HIGH) /**<  SCLK will remain High when SPI is idle */
/**
 * @}
 */

/**
 * @defgroup ssp_clk_phase SPI Clock Phase
 * \ingroup spi_control
 * @{
 */
#define LL_SSP_CLK_PHASE_1EDGE (0x00000000U)            /**< Data changes at clock falling edge, latches at clock rising  edge when CPOL = 0, \
                                                        <br> Data changes at clock rising  edge, latches at clock falling edge when CPOL = 1. */
#define LL_SSP_CLK_PHASE_2EDGE (SSP_CTRL0_SCLKPH_2EDGE) /**< Data changes at clock rising  edge, latches at clock falling edge when CPOL = 0, \
                                                        <br> Data changes at clock falling edge, latches at clock rising  edge when CPOL = 1. */
/**
 * @}
 */

/**
 * @defgroup ssp_first_bit SPI MSB to LSB transmission mode
 * \ingroup spi_control
 * @{
 */
#define LL_SSP_FIRST_BIT_MSB (0x00000000U)   /**< MSB transmit first */
#define LL_SSP_FIRST_BIT_LSB (SSP_CTRL0_LSB) /**< LSB transmit first */
/**
 * @}
 */

/**
 * \defgroup i2s_standard I2S standard
 * \ingroup i2s_control
 * @{
 */
#define LL_I2S_STANDARD_PHILIPS (0x00U) /**< philips protocol      */
#define LL_I2S_STANDARD_MSB     (0x01U) /**< MSB (left)  justified */
#define LL_I2S_STANDARD_LSB     (0x02U) /**< LSB (right) justified */
/**
 * @}
 */

/**
 * \defgroup i2s_data_format I2S data format
 * \ingroup i2s_control
 * @{
 */
#define LL_I2S_DATA_LEN_8B  (8U)  /**< data format:  8 bits */
#define LL_I2S_DATA_LEN_16B (16U) /**< data format: 16 bits */
#define LL_I2S_DATA_LEN_24B (24U) /**< data format: 24 bits */
#define LL_I2S_DATA_LEN_32B (32U) /**< data format: 32 bits */
/**
 * @}
 */

/**
 * \defgroup i2s_cpol I2S cpol
 * \ingroup i2s_control
 * @{
 */
#define LL_I2S_CPOL_HIGH (0x00)               /**< Active high, Idle low  */
#define LL_I2S_CPOL_LOW  (SSP_CTRL0_FSPO_LOW) /**< Active low , Idle high */
/**
 * @}
 */

/**
 * \defgroup i2s_audio_freq I2S audio frequency
 * \ingroup i2s_control
 * @{
 */
#define LL_I2S_AUDIO_FREQ_16K (16000U) /**< audio freq: 16k */
#define LL_I2S_AUDIO_FREQ_48K (48000U) /**< audio freq: 48k */
/**
 * @}
 */

/**
 * \defgroup i2s_data_ws_distance I2S distance of ws and data
 * \ingroup i2s_control
 * @{
 */
#define LL_SSP_WS_DATA_0SCLK SSP_CTRL0_FSDIST_0SCLK /*!< 0 SCLK cycle between the first bit */
#define LL_SSP_WS_DATA_1SCLK SSP_CTRL0_FSDIST_1SCLK /*!< 1 SCLK cycle between the first bit */
#define LL_SSP_WS_DATA_2SCLK SSP_CTRL0_FSDIST_2SCLK /*!< 2 SCLK cycle between the first bit */
#define LL_SSP_WS_DATA_3SCLK SSP_CTRL0_FSDIST_3SCLK /*!< 3 SCLK cycle between the first bit */
/**
 * @}
 */

/**
 * @defgroup ssp_mode I2S operating Mode
 * \ingroup i2s_control
 * @{
 */
#define LL_SSP_MODE_SLAVE_MONO    SSP_CTRL0_OPM_SLAVE_MONO    /**< SSP working at slave mode, I2S is Mono in addition */
#define LL_SSP_MODE_SLAVE_STEREO  SSP_CTRL0_OPM_SLAVE_STEREO  /**< SSP working at slave mode, I2S is Stereo in addition */
#define LL_SSP_MODE_MASTER_MONO   SSP_CTRL0_OPM_MASTER_MONO   /**< SSP working at master mode, I2S is Mono in addition */
#define LL_SSP_MODE_MASTER_STEREO SSP_CTRL0_OPM_MASTER_STEREO /**< SSP working at master mode, I2S is Stereo in addition */
/**
 * @}
 */

/* Exported macros --------------------------------------------------------*/

// ssp calibration times

/* Exported functions --------------------------------------------------------*/
/**
 * @brief  Set Clock phase for edge sampling
 * @param  spi SPI instance
 * @param  phase clock phase, \ref ssp_clk_phase
 * @retval None
 */
__STATIC_INLINE void LL_SSP_SetClkPhase(SN_SPI0_Type *spi, uint8_t phase)
{
    REG_CBIT(spi->CTRL0, SSP_CTRL0_SCLKPH);
    REG_SBIT(spi->CTRL0, phase);
}

/**
 * @brief  Get Clock phase state for edge sampling.
 * @param  spi SPI instance
 * @retval phase clock phase, \ref ssp_clk_phase
 */
__STATIC_INLINE uint32_t LL_SSP_GetClkPhase(SN_SPI0_Type *spi)
{
    return (REG_READ(spi->CTRL0) & SSP_CTRL0_SCLKPH);
}

/**
 * @brief  Set Clock polarity
 * @param  spi SPI instance
 * @param  polarity clock polarity, \ref ssp_clk_polarity
 * @retval None
 */
__STATIC_INLINE void LL_SSP_SetClockPolarity(SN_SPI0_Type *spi, uint8_t polarity)
{
    REG_CBIT(spi->CTRL0, SSP_CTRL0_SCLKPO);
    REG_SBIT(spi->CTRL0, polarity);
}

/**
 * @brief  Get Clock polarity
 * @param  spi SPI instance
 * @retval polarity clock polarity, \ref ssp_clk_polarity
 */
__STATIC_INLINE uint32_t LL_SSP_GetClockPolarity(SN_SPI0_Type *spi)
{
    return (REG_READ(spi->CTRL0) & SSP_CTRL0_SCLKPO);
}

/**
 * @brief  Set Operation mode
 * @param[in]  ssp SSP instance
 * @param[in]  ssp_mode operation mode
 *             i2s selects \ref ssp_mode
 *             spi selects \ref spi_mode
 * @retval None
 */
__STATIC_INLINE void LL_SSP_SetOpMode(SN_SPI0_Type *ssp, uint32_t ssp_mode)
{
    uint32_t CTRL0 = LL_REG_READ(ssp->CTRL0);
    LL_REG_CBIT(CTRL0, SSP_CTRL0_OPM);
    LL_REG_WRITE(ssp->CTRL0, CTRL0 | ssp_mode);
}

/**
 * @brief  Get Operation mode
 * @param[in]  ssp SSP instance
 * @retval     SSP operation mode
 *             i2s selects \ref ssp_mode
 *             spi selects \ref spi_mode
 */
__STATIC_INLINE uint32_t LL_SSP_GetOpMode(SN_SPI0_Type *ssp)
{
    return (LL_REG_READ(ssp->CTRL0) & SSP_CTRL0_OPM);
}

/**
 * @brief  Set Padding Data appending back of Serial Data
 * @param[in]  ssp SSP instance
 * @retval None
 * @note   I2S only
 */
__STATIC_INLINE void LL_SSP_SetPaddingDataBack(SN_SPI0_Type *ssp)
{
    LL_REG_CBIT(ssp->CTRL0, SSP_CTRL0_FSJSTFY);
}

/**
 * @brief  Set Padding Data appending front of Serial Data
 * @param[in]  ssp SSP instance
 * @retval None
 * @note   I2S only
 */
__STATIC_INLINE void LL_SSP_SetPaddingDataFront(SN_SPI0_Type *ssp)
{
    LL_REG_SBIT(ssp->CTRL0, SSP_CTRL0_FSJSTFY);
}

/**
 * @brief  Get Padding Data mode
 * @param[in]  ssp SSP instance
 * @retval 0: Back of Serial Data   1: Front of Serial Data
 * @note   I2S only
 */
__STATIC_INLINE uint32_t LL_SSP_GetPaddingDataMode(SN_SPI0_Type *ssp)
{
    return ((LL_REG_READ(ssp->CTRL0) & SSP_CTRL0_FSJSTFY) >> SSP_CTRL0_FSJSTFY_Pos);
}

/**
 * @brief  Set I2S mode WS (or Microwire SPI mode SEL) Active status
 * @param[in]  ssp SSP instance
 * @param[in]  polarity WS polarity \ref i2s_cpol
 * @retval None
 */
__STATIC_INLINE void LL_SSP_SetWSPolarity(SN_SPI0_Type *ssp, uint32_t polarity)
{
    LL_REG_CBIT(ssp->CTRL0, SSP_CTRL0_FSPO);
    LL_REG_SBIT(ssp->CTRL0, polarity);
}

/**
 * @brief  Get Microwire SPI mode SEL (or I2S mode WS) Active status
 * @param[in]  ssp SSP instance
 * @retval polarity WS polarity \ref i2s_cpol
 */
__STATIC_INLINE uint32_t LL_SSP_GetWSPolarity(SN_SPI0_Type *ssp)
{
    return (LL_REG_READ(ssp->CTRL0) & SSP_CTRL0_FSPO);
}

/**
 * @brief  Set first bit
 * @param  spi SPI instance
 * @param  bit_order  bit order, \ref ssp_first_bit
 * @retval None
 */
__STATIC_INLINE void LL_SSP_SetTransferBitOrder(SN_SPI0_Type *spi, uint8_t bit_order)
{
    REG_CBIT(spi->CTRL0, SSP_CTRL0_LSB);
    REG_SBIT(spi->CTRL0, bit_order);
}

/**
 * @brief  Get first bit state
 * @param  spi SPI instance
 * @retval first bit state, \ref ssp_first_bit
 */
__STATIC_INLINE uint32_t LL_SSP_GetTransferBitOrder(SN_SPI0_Type *spi)
{
    return (REG_READ(spi->CTRL0) & SSP_CTRL0_LSB);
}

/**
 * @brief  Set Tx data connected to Rx data internally
 * @param[in]  ssp SSP instance
 * @retval None
 */
__STATIC_INLINE void LL_SSP_SetTxRxConnect(SN_SPI0_Type *ssp)
{
    LL_REG_SBIT(ssp->CTRL0, SSP_CTRL0_LBM);
}

/**
 * @brief  Set Tx data and Rx data independent
 * @param[in]  ssp SSP instance
 * @retval None
 */
__STATIC_INLINE void LL_SSP_SetTxRxIndependent(SN_SPI0_Type *ssp)
{
    LL_REG_CBIT(ssp->CTRL0, SSP_CTRL0_LBM);
}

/**
 * @brief  Get status of data connection status
 * @param[in]  ssp SSP instance
 * @retval 1: connect internally;   0: independent
 */
__STATIC_INLINE uint32_t LL_SSP_GetTxRxConnectStatus(SN_SPI0_Type *ssp)
{
    return ((LL_REG_READ(ssp->CTRL0) & SSP_CTRL0_LBM) >> SSP_CTRL0_LBM_Pos);
}

/**
 * @brief  Set WS and Data distance
 * @param[in]  ssp SSP instance
 * @param[in]  distance WS and data Distance \ref i2s_data_ws_distance
 * @retval None
 * @note   I2S only
 */
__STATIC_INLINE void LL_SSP_SetWsDataDistance(SN_SPI0_Type *ssp, uint32_t distance)
{
    uint32_t CTRL0 = LL_REG_READ(ssp->CTRL0);
    LL_REG_CBIT(CTRL0, SSP_CTRL0_FSDIST);
    LL_REG_WRITE(ssp->CTRL0, CTRL0 | distance);
}

/**
 * @brief  Get WS and Data distance
 * @param[in]  ssp SSP instance
 * @retval
 * 0: The first bit of serial data will be valid in the first cycle of WS
 * 1: 1 SCLK cycle between the first bit of the valid received data and the frame start
 * 2: 2 SCLK cycles between the first bit of the valid received data and the frame start
 * 3: 3 SCLK cycles between the first bit of the valid received data and the frame start
 * @note   I2S only
 */
__STATIC_INLINE uint32_t LL_SSP_GetWsDataDistance(SN_SPI0_Type *ssp)
{
    return ((LL_REG_READ(ssp->CTRL0) & SSP_CTRL0_FSDIST) >> SSP_CTRL0_FSDIST_Pos);
}

/**
 * @brief  Enable SSP Flash mode
 * @param[in]  ssp SSP instance
 * @retval None
 * @note   SPI only
 */
__STATIC_INLINE void LL_SSP_Enable_SPIFlash(SN_SPI0_Type *ssp)
{
    LL_REG_SBIT(ssp->CTRL0, SSP_CTRL0_FLASH);
}

/**
 * @brief  Disable SSP Flash mode
 * @param[in]  ssp SSP instance
 * @retval None
 */
__STATIC_INLINE void LL_SSP_Disable_SPIFlash(SN_SPI0_Type *ssp)
{
    LL_REG_CBIT(ssp->CTRL0, SSP_CTRL0_FLASH);
}

/**
 * @brief  Judge if enabled SSP Flash mode
 * @param[in]  ssp SSP instance
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_SSP_IsEnabled_SPIFlash(SN_SPI0_Type *ssp)
{
    return ((LL_REG_READ(ssp->CTRL0) & SSP_CTRL0_FLASH) >> SSP_CTRL0_FLASH_Pos);
}

/**
 * @brief  Set SPI Mode to Motorola SPI
 * @param[in] ssp SSP instance
 * @retval None
 * @note   SPI only
 */
__STATIC_INLINE void LL_SSP_SetToMotorolaSPI(SN_SPI0_Type *ssp)
{
    LL_REG_CBIT(ssp->CTRL0, SSP_CTRL0_FFMT);
    LL_REG_SBIT(ssp->CTRL0, SSP_CTRL0_FFMT_MotorolaSPI);
}

/**
 * @brief  Set SPI Mode to MICROWIRE SPI
 * @param[in]  ssp SSP instance
 * @retval None
 */
__STATIC_INLINE void LL_SSP_SetToMicrowireSPI(SN_SPI0_Type *ssp)
{
    LL_REG_CBIT(ssp->CTRL0, SSP_CTRL0_FFMT);
    LL_REG_SBIT(ssp->CTRL0, SSP_CTRL0_FFMT_MicrowireSPI);
}

/**
 * @brief  Set SSP Mode to Philips I2S
 * @param[in]  ssp SSP instance
 * @retval None
 * @note   I2S only
 */
__STATIC_INLINE void LL_SSP_SetToPhilipsI2S(SN_SPI0_Type *ssp)
{
    LL_REG_CBIT(ssp->CTRL0, SSP_CTRL0_FFMT);
    LL_REG_SBIT(ssp->CTRL0, SSP_CTRL0_FFMT_PhilipsI2S);
}

/**
 * @brief  Get SSP Mode
 * @param[in]  ssp SSP instance
 * @retval 1: Motorola SSP;  2: MICROWIRE SSP;  Other: not SSP mode
 */
__STATIC_INLINE uint32_t LL_SSP_GetSPIMode(SN_SPI0_Type *ssp)
{
    return ((LL_REG_READ(ssp->CTRL0) & SSP_CTRL0_FFMT) >> SSP_CTRL0_FFMT_Pos);
}

/**
 * @brief  Set SEL polarity
 * @param  spi SPI instance
 * @param  polarity SEL polarity, \ref ssp_fs_polarity
 * @retval None
 */
__STATIC_INLINE void LL_SSP_SetSELPolarity(SN_SPI0_Type *spi, uint8_t polarity)
{
    REG_CBIT(spi->CTRL0, SSP_CTRL0_SPIFSPO);
    REG_SBIT(spi->CTRL0, polarity);
}

/**
 * @brief  Get SEL polarity
 * @param  spi SPI instance
 * @retval polarity SEL polarity, \ref ssp_fs_polarity
 */
__STATIC_INLINE uint32_t LL_SSP_GetSELPolarity(SN_SPI0_Type *spi)
{
    return (REG_READ(spi->CTRL0) & SSP_CTRL0_SPIFSPO);
}

/**
 * @brief  Select the mode of SCLK/BCLK to master
 * @param[in]  ssp SSP instance
 * @retval None
 */
__STATIC_INLINE void LL_SSP_SetClkToMaster(SN_SPI0_Type *ssp)
{
    LL_REG_SBIT(ssp->CTRL0, SSP_CTRL0_SCLKFDBK);
}

/**
 * @brief  Select the mode of SCLK/BCLK to slave
 * @param[in]  ssp SSP instance
 * @retval None
 */
__STATIC_INLINE void LL_SSP_SetClkToSlave(SN_SPI0_Type *ssp)
{
    LL_REG_CBIT(ssp->CTRL0, SSP_CTRL0_SCLKFDBK);
}

/**
 * @brief  get the mode of SCLK/BCLK
 * @param[in]  ssp SSP instance
 * @retval 1: master mode;   0: slave mode
 */
__STATIC_INLINE uint32_t LL_SSP_GetClkMode(SN_SPI0_Type *ssp)
{
    return ((LL_REG_READ(ssp->CTRL0) & SSP_CTRL0_SCLKFDBK) >> SSP_CTRL0_SCLKFDBK_Pos);
}

/**
 * @brief  Select the mode of SEL/WS to master
 * @param[in]  ssp SSP instance
 * @retval None
 */
__STATIC_INLINE void LL_SSP_SetSELToMaster(SN_SPI0_Type *ssp)
{
    LL_REG_SBIT(ssp->CTRL0, SSP_CTRL0_FSFDBK);
}

/**
 * @brief  Select the mode of SEL/WS to slave
 * @param[in]  ssp SSP instance
 * @retval None
 */
__STATIC_INLINE void LL_SSP_SetSELToSlave(SN_SPI0_Type *ssp)
{
    LL_REG_CBIT(ssp->CTRL0, SSP_CTRL0_FSFDBK);
}

/**
 * @brief  get the mode of SEL/WS
 * @param[in]  ssp SSP instance
 * @retval 1: master mode;   0: slave mode
 */
__STATIC_INLINE uint32_t LL_SSP_GetSELMode(SN_SPI0_Type *ssp)
{
    return ((LL_REG_READ(ssp->CTRL0) & SSP_CTRL0_FSFDBK) >> SSP_CTRL0_FSFDBK_Pos);
}

/**
 * @brief  Set SSP SCLK will stop when no transmit data in Flash mode
 * @param[in]  ssp SSP instance
 * @retval None
 */
__STATIC_INLINE void LL_SSP_SetNoDataSCLKStopInFM(SN_SPI0_Type *ssp)
{
    LL_REG_SBIT(ssp->CTRL0, SSP_CTRL0_FLASHTX);
}

/**
 * @brief  Set SSP SCLK will still trigger when no transmit data in flash mode until the RX FIFO is full
 * @param[in]  ssp SSP instance
 * @retval None
 */
__STATIC_INLINE void LL_SSP_SetSCLKStopUntilRxFIFOFullInFM(SN_SPI0_Type *ssp)
{
    LL_REG_CBIT(ssp->CTRL0, SSP_CTRL0_FLASHTX);
}

/**
 * @brief  Get SSP FLASH mode transmit control status
 * @param[in]  ssp SSP instance
 * @retval 1: No data SCLK stop;  0: SCLK stop until Rx FIFO full
 */
__STATIC_INLINE uint32_t LL_SSP_GetFLASHTX(SN_SPI0_Type *ssp)
{
    return ((LL_REG_READ(ssp->CTRL0) & SSP_CTRL0_FLASHTX) >> SSP_CTRL0_FLASHTX_Pos);
}

/**
 * @brief  Set Continuously transfer without asserting SEL when the Transmit FIFO is not empty
 * @param[in]  ssp SSP instance
 * @retval None
 * @note   SPI only
 */
__STATIC_INLINE void LL_SSP_SetContinuousTransferMode(SN_SPI0_Type *ssp)
{
    LL_REG_SBIT(ssp->CTRL0, SSP_CTRL0_SPICONTX);
}

/**
 * @brief  Set SEL may assert when the Transmit FIFO is not empty
 * @param[in]  ssp SSP instance
 * @retval None
 * @note   SPI only
 */
__STATIC_INLINE void LL_SSP_SetAssertTransferMode(SN_SPI0_Type *ssp)
{
    LL_REG_CBIT(ssp->CTRL0, SSP_CTRL0_SPICONTX);
}

/**
 * @brief  Get SSP transfer mode
 * @param[in]  ssp SSP instance
 * @retval 1: Continuous Transfer Mode;  0: Assert Transfer Mode
 * @note   SPI only
 */
__STATIC_INLINE uint32_t LL_SSP_GetFlashTransmitMode(SN_SPI0_Type *ssp)
{
    return ((LL_REG_READ(ssp->CTRL0) & SSP_CTRL0_SPICONTX) >> SSP_CTRL0_SPICONTX_Pos);
}

/****************************** End SSP CTRL0 register content **************************************/

/****************************** SSP CTRL1 register content **************************************/
/**
 * @brief  Set SCLK divider
 * @param[in]  ssp SSP instance
 * @param[in]  divider SCLK divider
 * @retval None
 */
__STATIC_INLINE void LL_SSP_SetSclkDivider(SN_SPI0_Type *ssp, uint32_t divider)
{
    LL_REG_CBIT(ssp->CTRL1, SSP_CTRL1_SCLKDIV);
    LL_REG_SBIT(ssp->CTRL1, divider << SSP_CTRL1_SCLKDIV_Pos);
}

/**
 * @brief  Get SCLK divider
 * @param[in]  ssp SSP instance
 * @retval SCLK divider
 */
__STATIC_INLINE uint32_t LL_SSP_GetSclkDivider(SN_SPI0_Type *ssp)
{
    return ((LL_REG_READ(ssp->CTRL1) & SSP_CTRL1_SCLKDIV) >> SSP_CTRL1_SCLKDIV_Pos);
}

/**
 * @brief  Set Serial data length
 * @param[in]  ssp SSP instance
 * @param[in]  data_size data length, ranges from 5 to 16
 * @retval None
 */
__STATIC_INLINE void LL_SSP_SetSerialDataLength(SN_SPI0_Type *ssp, uint32_t data_size)
{
    LL_REG_CBIT(ssp->CTRL1, SSP_CTRL1_SDL);
    LL_REG_SBIT(ssp->CTRL1, (data_size - 1) << SSP_CTRL1_SDL_Pos);
}

/**
 * @brief  Get Serial data length
 * @param[in]  ssp SSP instance
 * @retval Serial data length
 */
__STATIC_INLINE uint32_t LL_SSP_GetSerialDataLength(SN_SPI0_Type *ssp)
{
    return (((LL_REG_READ(ssp->CTRL1) & SSP_CTRL1_SDL) >> SSP_CTRL1_SDL_Pos) + 1);
}

/**
 * @brief  Set Padding data length for MICROWIRE mode or I2S mode
 * @param[in]  ssp SSP instance
 * @param[in]  padding padding data length, ranges from 0 to 255
 * @retval None
 */
__STATIC_INLINE void LL_SSP_SetPaddingLength(SN_SPI0_Type *ssp, uint32_t padding)
{
    LL_REG_CBIT(ssp->CTRL1, SSP_CTRL1_PDL);
    LL_REG_SBIT(ssp->CTRL1, padding << SSP_CTRL1_PDL_Pos);
}

/**
 * @brief  Get Padding data length
 * @param[in]  ssp SSP instance
 * @retval Padding data length
 */
__STATIC_INLINE uint32_t LL_SSP_GetPaddingLength(SN_SPI0_Type *ssp)
{
    return ((LL_REG_READ(ssp->CTRL1) & SSP_CTRL1_PDL) >> SSP_CTRL1_PDL_Pos);
}

/**
 * @brief  Enable SSP
 * @param[in]  ssp SSP Base Address
 * @retval None
 */
__STATIC_INLINE void LL_SSP_Enable(SN_SPI0_Type *ssp)
{
    LL_REG_SBIT(ssp->CTRL2, SSP_CTRL2_SSPEN);
}

/**
 * @brief  Disable SSP
 * @param[in]  ssp SSP Base Address
 * @retval None
 */
__STATIC_INLINE void LL_SSP_Disable(SN_SPI0_Type *ssp)
{
    LL_REG_CBIT(ssp->CTRL2, SSP_CTRL2_SSPEN);
}

/**
 * @brief  Get the selected SSP instance enable state.
 * @param[in]  ssp SSP instance
 * @retval 0: SSP is disabled, 1: SSP is enabled.
 */
__STATIC_INLINE uint32_t LL_SSP_IsEnabled(SN_SPI0_Type *ssp)
{
    return ((LL_REG_READ(ssp->CTRL2) & SSP_CTRL2_SSPEN) == SSP_CTRL2_SSPEN);
}

/**
 * @brief  Enable SSP Transmit data output
 * @param[in]  ssp SSP Base Address
 * @retval None
 */
__STATIC_INLINE void LL_SSP_EnableTXDOE(SN_SPI0_Type *ssp)
{
    LL_REG_SBIT(ssp->CTRL2, SSP_CTRL2_TXDOE);
}

/**
 * @brief  Disable SSP Transmit data output
 * @param[in]  ssp SSP Base Address
 * @retval None
 */
__STATIC_INLINE void LL_SSP_DisableTXDOE(SN_SPI0_Type *ssp)
{
    LL_REG_CBIT(ssp->CTRL2, SSP_CTRL2_TXDOE);
}

/**
 * @brief  Get the selected SSP instance Transmit data output enable state.
 * @param[in]  ssp SSP instance
 * @retval 0: Transmit is disabled, 1: Transmit is enabled.
 */
__STATIC_INLINE uint32_t LL_SSP_IsEnabledTXDOE(SN_SPI0_Type *ssp)
{
    return ((LL_REG_READ(ssp->CTRL2) & SSP_CTRL2_TXDOE) == SSP_CTRL2_TXDOE);
}

/**
 * @brief  Clear RX FIFO
 * @param[in]  ssp SSP Base Address
 * @retval None
 */
__STATIC_INLINE void LL_SSP_ClearRxFIFO(SN_SPI0_Type *ssp)
{
    LL_REG_SBIT(ssp->CTRL2, SSP_CTRL2_RXFCLR);
}

/**
 * @brief  Clear TX FIFO
 * @param[in]  ssp SSP Base Address
 * @retval None
 */
__STATIC_INLINE void LL_SSP_ClearTxFIFO(SN_SPI0_Type *ssp)
{
    LL_REG_SBIT(ssp->CTRL2, SSP_CTRL2_TXFCLR);
}

/**
 * @brief  Reset the state machine
 * @param[in]  ssp SSP Base Address
 * @retval None
 */
__STATIC_INLINE void LL_SSP_Reset(SN_SPI0_Type *ssp)
{
    LL_REG_SBIT(ssp->CTRL2, SSP_CTRL2_RESET);
}

/**
 * @brief  Enable SSP Tx and Rx function
 * @param[in]  ssp SSP Base Address
 * @retval None
 */
__STATIC_INLINE void LL_SSP_EnableTxRx(SN_SPI0_Type *ssp)
{
    uint32_t ctrl2_reg = SSP_CTRL2_RXEN | SSP_CTRL2_TXEN;
    LL_REG_CBIT(ssp->CTRL2, ctrl2_reg);
    LL_REG_SBIT(ssp->CTRL2, ctrl2_reg);
}

/**
 * @brief  Disable SSP Tx and Rx function
 * @param[in]  ssp SSP Base Address
 * @retval None
 */
__STATIC_INLINE void LL_SSP_DisableTxRx(SN_SPI0_Type *ssp)
{
    uint32_t ctrl2_reg = SSP_CTRL2_RXEN | SSP_CTRL2_TXEN;
    LL_REG_CBIT(ssp->CTRL2, ctrl2_reg);
}

/**
 * @brief  Enable SSP Rx function
 * @param[in]  ssp SSP Base Address
 * @retval None
 */
__STATIC_INLINE void LL_SSP_EnableRx(SN_SPI0_Type *ssp)
{
    LL_REG_SBIT(ssp->CTRL2, SSP_CTRL2_RXEN);
}

/**
 * @brief  Disable SSP Rx function
 * @param[in]  ssp SSP Base Address
 * @retval None
 */
__STATIC_INLINE void LL_SSP_DisableRx(SN_SPI0_Type *ssp)
{
    LL_REG_CBIT(ssp->CTRL2, SSP_CTRL2_RXEN);
}

/**
 * @brief  Get the selected SSP instance Rx enable state.
 * @param[in]  ssp SSP instance
 * @retval 0: Rx is disabled, 1: Rx is enabled.
 */
__STATIC_INLINE uint32_t LL_SSP_IsEnabledRx(SN_SPI0_Type *ssp)
{
    return ((LL_REG_READ(ssp->CTRL2) & SSP_CTRL2_RXEN) == SSP_CTRL2_RXEN);
}

/**
 * @brief  Enable SSP Tx function
 * @param[in]  ssp SSP Base Address
 * @retval None
 */
__STATIC_INLINE void LL_SSP_EnableTx(SN_SPI0_Type *ssp)
{
    LL_REG_SBIT(ssp->CTRL2, SSP_CTRL2_TXEN);
}

/**
 * @brief  Disable SSP Tx function
 * @param[in]  ssp SSP Base Address
 * @retval None
 */
__STATIC_INLINE void LL_SSP_DisableTx(SN_SPI0_Type *ssp)
{
    LL_REG_CBIT(ssp->CTRL2, SSP_CTRL2_TXEN);
}

/**
 * @brief  Get the selected SSP instance Tx enable state.
 * @param[in]  ssp SSP instance
 * @retval 0: Tx is disabled, 1: Tx is enabled.
 */
__STATIC_INLINE uint32_t LL_SSP_IsEnabledTx(SN_SPI0_Type *ssp)
{
    return ((LL_REG_READ(ssp->CTRL2) & SSP_CTRL2_TXEN) == SSP_CTRL2_TXEN);
}

/**
 * @brief  Set SEL output high when ssp is active l for SPI Flash mode
 * @param[in]  ssp SSP Base Address
 * @retval None
 * @note   SPI only
 */
__STATIC_INLINE void LL_SSP_SetSpiFlashSELActiveHigh(SN_SPI0_Type *ssp)
{
    LL_REG_SBIT(ssp->CTRL2, SSP_CTRL2_FS);
}

/**
 * @brief  Set SEL output low when ssp is active l for SPI Flash mode
 * @param[in]  ssp SSP Base Address
 * @retval None
 * @note   SPI only
 */
__STATIC_INLINE void LL_SSP_SetSpiFlashSELActiveLow(SN_SPI0_Type *ssp)
{
    LL_REG_CBIT(ssp->CTRL2, SSP_CTRL2_FS);
}

/**
 * @brief  Get the selected SSP instance SEL state when SSP is active
 * @param[in]  ssp SSP instance
 * @retval 0: SEL is low, 1: SEL is high.
 * @note   SPI only
 */
__STATIC_INLINE uint32_t LL_SSP_GetSELActiveState(SN_SPI0_Type *ssp)
{
    return ((LL_REG_READ(ssp->CTRL2) & SSP_CTRL2_FS) >> SSP_CTRL2_FS_Pos);
}

/**
 * @brief  Get STATUS reg content
 * @param[in]  ssp SSP instance
 * @retval STATUS reg content
 */
__STATIC_INLINE uint32_t LL_SSP_GetStatus(SN_SPI0_Type *ssp)
{
    return (LL_REG_READ(ssp->STATUS));
}

/**
 * @brief  Judge if Rx FIFO is full
 * @param[in]  ssp SSP instance
 * @retval 0: Not Full, 1: Full.
 */
__STATIC_INLINE uint32_t LL_SSP_IsActiveFlag_RxFIFOFull(SN_SPI0_Type *ssp)
{
    return ((LL_REG_READ(ssp->STATUS) & SSP_STATUS_RFF) == SSP_STATUS_RFF);
}

/**
 * @brief  Judge if Tx FIFO is not full
 * @param[in]  ssp SSP instance
 * @retval 0: Full, 1: not Full.
 */
__STATIC_INLINE uint32_t LL_SSP_IsActiveFlag_TxFIFONotFull(SN_SPI0_Type *ssp)
{
    return ((LL_REG_READ(ssp->STATUS) & SSP_STATUS_TFNF) == SSP_STATUS_TFNF);
}

/**
 * @brief  Judge if SSP is busy
 * @param[in]  ssp SSP instance
 * @retval 0: idle, 1: busy.
 */
__STATIC_INLINE uint32_t LL_SSP_IsActiveFlag_Busy(SN_SPI0_Type *ssp)
{
    return ((LL_REG_READ(ssp->STATUS) & SSP_STATUS_BUSY) == SSP_STATUS_BUSY);
}

/**
 * @brief  Get Rx FIFO data Count
 * @param[in]  ssp SSP instance
 * @retval Rx FIFO data Count
 */
__STATIC_INLINE uint32_t LL_SSP_GetRxFIFODataCount(SN_SPI0_Type *ssp)
{
    return ((LL_REG_READ(ssp->STATUS) & SSP_STATUS_RFVE) >> SSP_STATUS_RFVE_Pos);
}

/**
 * @brief  Get Tx FIFO data Count
 * @param[in]  ssp SSP instance
 * @retval Tx FIFO data Count
 */
__STATIC_INLINE uint32_t LL_SSP_GetTxFIFODataCount(SN_SPI0_Type *ssp)
{
    return ((LL_REG_READ(ssp->STATUS) & SSP_STATUS_TFVE) >> SSP_STATUS_TFVE_Pos);
}

/**
 * @brief  Enable SSP Rx FIFO Overrun Interrupt
 * @param[in]  ssp SSP Base Address
 * @retval None
 */
__STATIC_INLINE void LL_SSP_EnableIT_Overrun(SN_SPI0_Type *ssp)
{
    LL_REG_SBIT(ssp->ICTRL, SSP_ICTRL_RFORIEN);
}

/**
 * @brief  Disable SSP Rx FIFO Overrun Interrupt
 * @param[in]  ssp SSP Base Address
 * @retval None
 */
__STATIC_INLINE void LL_SSP_DisableIT_Overrun(SN_SPI0_Type *ssp)
{
    LL_REG_CBIT(ssp->ICTRL, SSP_ICTRL_RFORIEN);
}

/**
 * @brief  Judge if SSP Rx FIFO Overrun Interrupt is enabled
 * @param[in]  ssp SSP instance
 * @retval 0: disabled, 1: enabled.
 */
__STATIC_INLINE uint32_t LL_SSP_IsEnabledIT_Overrun(SN_SPI0_Type *ssp)
{
    return ((LL_REG_READ(ssp->ICTRL) & SSP_ICTRL_RFORIEN) == SSP_ICTRL_RFORIEN);
}

/**
 * @brief  Enable SSP Tx FIFO Underrun Interrupt
 * @param[in]  ssp SSP Base Address
 * @retval None
 */
__STATIC_INLINE void LL_SSP_EnableIT_Underrun(SN_SPI0_Type *ssp)
{
    LL_REG_SBIT(ssp->ICTRL, SSP_ICTRL_TFURIEN);
}

/**
 * @brief  Disable SSP FIFO Underrun Interrupt
 * @param[in]  ssp SSP Base Address
 * @retval None
 */
__STATIC_INLINE void LL_SSP_DisableIT_Underrun(SN_SPI0_Type *ssp)
{
    LL_REG_CBIT(ssp->ICTRL, SSP_ICTRL_TFURIEN);
}

/**
 * @brief  Judge if SSP FIFO Underrun Interrupt is enabled
 * @param[in]  ssp SSP instance
 * @retval 0: disabled, 1: enabled.
 */
__STATIC_INLINE uint32_t LL_SSP_IsEnabledIT_Underrun(SN_SPI0_Type *ssp)
{
    return ((LL_REG_READ(ssp->ICTRL) & SSP_ICTRL_TFURIEN) == SSP_ICTRL_TFURIEN);
}

/**
 * @brief  Enable SSP Receive FIFO Threshold Interrupt
 * @param[in]  ssp SSP Base Address
 * @retval None
 */
__STATIC_INLINE void LL_SSP_EnableIT_RFTHIEN(SN_SPI0_Type *ssp)
{
    LL_REG_SBIT(ssp->ICTRL, SSP_ICTRL_RFTHIEN);
}

/**
 * @brief  Disable SSP Receive FIFO Threshold Interrupt
 * @param[in]  ssp SSP Base Address
 * @retval None
 */
__STATIC_INLINE void LL_SSP_DisableIT_RFTHIEN(SN_SPI0_Type *ssp)
{
    LL_REG_CBIT(ssp->ICTRL, SSP_ICTRL_RFTHIEN);
}

/**
 * @brief  Judge if SSP Receive FIFO Threshold Interrupt is enabled
 * @param[in]  ssp SSP instance
 * @retval 0: disabled, 1: enabled.
 */
__STATIC_INLINE uint32_t LL_SSP_IsEnabledIT_RFTHIEN(SN_SPI0_Type *ssp)
{
    return ((LL_REG_READ(ssp->ICTRL) & SSP_ICTRL_RFTHIEN) == SSP_ICTRL_RFTHIEN);
}

/**
 * @brief  Enable SSP Transmit FIFO Threshold Interrupt
 * @param[in]  ssp SSP Base Address
 * @retval None
 */
__STATIC_INLINE void LL_SSP_EnableIT_TFTHIEN(SN_SPI0_Type *ssp)
{
    LL_REG_SBIT(ssp->ICTRL, SSP_ICTRL_TFTHIEN);
}

/**
 * @brief  Disable SSP Transmit FIFO Threshold Interrupt
 * @param[in]  ssp SSP Base Address
 * @retval None
 */
__STATIC_INLINE void LL_SSP_DisableIT_TFTHIEN(SN_SPI0_Type *ssp)
{
    LL_REG_CBIT(ssp->ICTRL, SSP_ICTRL_TFTHIEN);
}

/**
 * @brief  Judge if SSP Receive FIFO Threshold Interrupt is enabled
 * @param[in]  ssp SSP instance
 * @retval 0: disabled, 1: enabled.
 */
__STATIC_INLINE uint32_t LL_SSP_IsEnabledIT_TFTHIEN(SN_SPI0_Type *ssp)
{
    return ((LL_REG_READ(ssp->ICTRL) & SSP_ICTRL_TFTHIEN) == SSP_ICTRL_TFTHIEN);
}

/**
 * @brief  Enable SSP Receive DMA Request Interrupt
 * @param[in]  ssp SSP Base Address
 * @retval None
 */
__STATIC_INLINE void LL_SSP_EnableIT_RFDMAEN(SN_SPI0_Type *ssp)
{
    LL_REG_SBIT(ssp->ICTRL, SSP_ICTRL_RFDMAEN);
}

/**
 * @brief  Disable SSP Receive DMA Request Interrupt
 * @param[in]  ssp SSP Base Address
 * @retval None
 */
__STATIC_INLINE void LL_SSP_DisableIT_RFDMAEN(SN_SPI0_Type *ssp)
{
    LL_REG_CBIT(ssp->ICTRL, SSP_ICTRL_RFDMAEN);
}

/**
 * @brief  Judge if SSP SSP Receive DMA Request Interrupt is enabled
 * @param[in]  ssp SSP instance
 * @retval 0: disabled, 1: enabled.
 */
__STATIC_INLINE uint32_t LL_SSP_IsEnabledIT_RFDMAEN(SN_SPI0_Type *ssp)
{
    return ((LL_REG_READ(ssp->ICTRL) & SSP_ICTRL_RFDMAEN) == SSP_ICTRL_RFDMAEN);
}

/**
 * @brief  Enable SSP Transmit DMA Request Interrupt
 * @param[in]  ssp SSP Base Address
 * @retval None
 */
__STATIC_INLINE void LL_SSP_EnableIT_TFDMAEN(SN_SPI0_Type *ssp)
{
    LL_REG_SBIT(ssp->ICTRL, SSP_ICTRL_TFDMAEN);
}

/**
 * @brief  Disable SSP Transmit DMA Request Interrupt
 * @param[in]  ssp SSP Base Address
 * @retval None
 */
__STATIC_INLINE void LL_SSP_DisableIT_TFDMAEN(SN_SPI0_Type *ssp)
{
    LL_REG_CBIT(ssp->ICTRL, SSP_ICTRL_TFDMAEN);
}

/**
 * @brief  Judge if SSP SSP Transmit DMA Request Interrupt is enabled
 * @param[in]  ssp SSP instance
 * @retval 0: disabled, 1: enabled.
 */
__STATIC_INLINE uint32_t LL_SSP_IsEnabledIT_TFDMAEN(SN_SPI0_Type *ssp)
{
    return ((LL_REG_READ(ssp->ICTRL) & SSP_ICTRL_TFDMAEN) == SSP_ICTRL_TFDMAEN);
}

/**
 * @brief  Set Receive FIFO Threshold
 * @param[in]  ssp SSP Base Address
 * @param[in]  threshold Receive FIFO Threshold
 * @retval None
 */
__STATIC_INLINE void LL_SSP_SetRxFIFOThreshold(SN_SPI0_Type *ssp, uint32_t threshold)
{
    LL_REG_CBIT(ssp->ICTRL, SSP_ICTRL_RFTHOD);
    LL_REG_SBIT(ssp->ICTRL, threshold << SSP_ICTRL_RFTHOD_Pos);
}

/**
 * @brief  Get Receive FIFO Threshold
 * @param[in]  ssp SSP instance
 * @retval Receive FIFO Threshold
 */
__STATIC_INLINE uint32_t LL_SSP_GetRxFIFOThreshold(SN_SPI0_Type *ssp)
{
    return ((LL_REG_READ(ssp->ICTRL) & SSP_ICTRL_RFTHOD) >> SSP_ICTRL_RFTHOD_Pos);
}

/**
 * @brief  Set Transmit FIFO Threshold
 * @param[in]  ssp SSP Base Address
 * @param[in]  threshold Transmit FIFO Threshold
 * @retval None
 */
__STATIC_INLINE void LL_SSP_SetTxFIFOThreshold(SN_SPI0_Type *ssp, uint32_t threshold)
{
    LL_REG_CBIT(ssp->ICTRL, SSP_ICTRL_TFTHOD);
    LL_REG_SBIT(ssp->ICTRL, threshold << SSP_ICTRL_TFTHOD_Pos);
}

/**
 * @brief  Get Transmit FIFO Threshold
 * @param[in]  ssp SSP instance
 * @retval Transmit FIFO Threshold
 */
__STATIC_INLINE uint32_t LL_SSP_GetTxFIFOThreshold(SN_SPI0_Type *ssp)
{
    return ((LL_REG_READ(ssp->ICTRL) & SSP_ICTRL_TFTHOD) >> SSP_ICTRL_TFTHOD_Pos);
}

/**
 * @brief  RX FIFO threshold = RFTHOD + 1
 * @param[in]  ssp SSP Base Address
 * @retval None
 */
__STATIC_INLINE void LL_SSP_Enable_RFTHOD_UNIT(SN_SPI0_Type *ssp)
{
    LL_REG_SBIT(ssp->ICTRL, SSP_ICTRL_RFTHOD_UNIT);
}

/**
 * @brief  RX FIFO threshold = RFTHOD
 * @param[in]  ssp SSP Base Address
 * @retval None
 */
__STATIC_INLINE void LL_SSP_Disable_RFTHOD_UNIT(SN_SPI0_Type *ssp)
{
    LL_REG_CBIT(ssp->ICTRL, SSP_ICTRL_RFTHOD_UNIT);
}

/**
 * @brief  Judge if RX FIFO threshold == RFTHOD + 1
 * @param[in]  ssp SSP instance
 * @retval 0: disabled, 1: enabled.
 */
__STATIC_INLINE uint32_t LL_SSP_IsEnabled_RFTHOD_UNIT(SN_SPI0_Type *ssp)
{
    return ((LL_REG_READ(ssp->ICTRL) & SSP_ICTRL_RFTHOD_UNIT) == SSP_ICTRL_RFTHOD_UNIT);
}

/**
 * @brief  The interrupt will take place when transmit is done and TX valid entry is empty
 * @param[in]  ssp SSP Base Address
 * @retval None
 */
__STATIC_INLINE void LL_SSP_EnableIT_TXCIEN(SN_SPI0_Type *ssp)
{
    LL_REG_SBIT(ssp->ICTRL, SSP_ICTRL_TXCIEN);
}

/**
 * @brief  Disabled Transmit Data Complete Interrupt
 * @param[in]  ssp SSP Base Address
 * @retval None
 */
__STATIC_INLINE void LL_SSP_DisableIT_TXCIEN(SN_SPI0_Type *ssp)
{
    LL_REG_CBIT(ssp->ICTRL, SSP_ICTRL_TXCIEN);
}

/**
 * @brief  Judge if RX FIFO threshold == RFTHOD + 1
 * @param[in]  ssp SSP instance
 * @retval 0: disabled, 1: enabled.
 */
__STATIC_INLINE uint32_t LL_SSP_IsEnabledIT_TXCIEN(SN_SPI0_Type *ssp)
{
    return ((LL_REG_READ(ssp->ICTRL) & SSP_ICTRL_TXCIEN) == SSP_ICTRL_TXCIEN);
}

/**
 * @brief  Read RIS register
 * @param[in]  ssp SSP Base Address
 * @retval RIS register value
 */
__STATIC_INLINE uint32_t LL_SSP_GetRawInterruptStatus(SN_SPI0_Type *ssp)
{
    return (LL_REG_READ(ssp->RIS));
}

/**
 * @brief  Judge if Rx FIFO overrun
 * @param[in]  ssp SSP instance
 * @retval 0: not overrun, 1: overrun.
 */
__STATIC_INLINE uint32_t LL_SSP_IsActiveFlag_Overrun(SN_SPI0_Type *ssp)
{
    return ((LL_REG_READ(ssp->RIS) & SSP_RIS_RFORI) == SSP_RIS_RFORI);
}

/**
 * @brief  Judge if Tx FIFO underrun
 * @param[in]  ssp SSP instance
 * @retval 0: not underrun, 1: underrun.
 */
__STATIC_INLINE uint32_t LL_SSP_IsActiveFlag_Underrun(SN_SPI0_Type *ssp)
{
    return ((LL_REG_READ(ssp->RIS) & SSP_RIS_TFURI) == SSP_RIS_TFURI);
}

/**
 * @brief  Judge if trigger Rx FIFO Threshold Interrupt
 * @param[in]  ssp SSP instance
 * @retval 0: not trigger, 1: trigger.
 */
__STATIC_INLINE uint32_t LL_SSP_IsActiveFlag_RFTHI(SN_SPI0_Type *ssp)
{
    return ((LL_REG_READ(ssp->RIS) & SSP_RIS_RFTHI) == SSP_RIS_RFTHI);
}

/**
 * @brief  Judge if trigger Tx FIFO Threshold Interrupt
 * @param[in]  ssp SSP instance
 * @retval 0: not trigger, 1: trigger.
 */
__STATIC_INLINE uint32_t LL_SSP_IsActiveFlag_TFTHI(SN_SPI0_Type *ssp)
{
    return ((LL_REG_READ(ssp->RIS) & SSP_RIS_TFTHI) == SSP_RIS_TFTHI);
}

/**
 * @brief  Judge if trigger Transmit Data Complete Interrupt
 * @param[in]  ssp SSP instance
 * @retval 0: not trigger, 1: trigger.
 */
__STATIC_INLINE uint32_t LL_SSP_IsActiveFlag_TXCI(SN_SPI0_Type *ssp)
{
    return ((LL_REG_READ(ssp->RIS) & SSP_RIS_TXCI) == SSP_RIS_TXCI);
}

/**
 * @brief  Write Transmit data
 * @param[in]  ssp SSP instance
 * @param[in]  data Transmit data
 * @retval None
 */
__STATIC_INLINE void LL_SSP_WriteDataRegister(SN_SPI0_Type *ssp, uint32_t data)
{
    LL_REG_WRITE(ssp->DATA, data);
}

/**
 * @brief  Read Received data
 * @param[in]  ssp SSP instance
 * @retval Received data
 */
__STATIC_INLINE uint32_t LL_SSP_ReadDataRegister(SN_SPI0_Type *ssp)
{
    return LL_REG_READ(ssp->DATA);
}

/**
 * @brief  Set Padding Cycle Length for SPI mode
 * @param[in]  ssp SSP instance
 * @param[in]  padding padding cycle length
 * @retval None
 * @note   SPI only
 */
__STATIC_INLINE void LL_SSP_SetPaddingCycle(SN_SPI0_Type *ssp, uint32_t padding)
{
    LL_REG_CBIT(ssp->CTRL3, SSP_CTRL3_PCL);
    LL_REG_SBIT(ssp->CTRL3, padding << SSP_CTRL3_PCL_Pos);
}

/**
 * @brief  Get Padding Cycle Length for SPI mode
 * @param[in]  ssp SSP instance
 * @retval padding cycle length
 * @note   SPI only
 */
__STATIC_INLINE uint32_t LL_SSP_GetPaddingCycleLength(SN_SPI0_Type *ssp)
{
    return ((LL_REG_READ(ssp->CTRL3) & SSP_CTRL3_PCL) >> SSP_CTRL3_PCL_Pos);
}

/**
 * @brief  Enable different padding data length for I2S mode
 * @param[in]  ssp SSP instance
 * @retval None
 * @note   I2S only
 */
__STATIC_INLINE void LL_SSP_EnableDPDLEN(SN_SPI0_Type *ssp)
{
    LL_REG_SBIT(ssp->CTRL3, SSP_CTRL3_DPDLEN);
}

/**
 * @brief  Disbale different padding data length for I2S mode
 * @param[in]  ssp SSP instance
 * @retval None
 * @note   I2S only
 */
__STATIC_INLINE void LL_SSP_DisableDPDLEN(SN_SPI0_Type *ssp)
{
    LL_REG_CBIT(ssp->CTRL3, SSP_CTRL3_DPDLEN);
}

/**
 * @brief  If Enabled Different Padding Data Length mode for I2S mode
 * @param[in]  ssp SSP instance
 * @retval 1: enable, 0: disable
 */
__STATIC_INLINE uint32_t LL_SSP_IsEnableDPDLEN(SN_SPI0_Type *ssp)
{
    return ((LL_REG_READ(ssp->CTRL3) & SSP_CTRL3_DPDLEN) == SSP_CTRL3_DPDLEN);
}

/**
 * @brief  Set different padding data length for I2S mode
 * @param[in]  ssp SSP instance
 * @param[in]  padding padding length
 * @retval None
 * @note   I2S only
 */
__STATIC_INLINE void LL_SSP_SetDPDLength(SN_SPI0_Type *ssp, uint32_t padding)
{
    LL_REG_CBIT(ssp->CTRL3, SSP_CTRL3_DPDL);
    LL_REG_SBIT(ssp->CTRL3, padding << SSP_CTRL3_DPDL_Pos);
}

/**
 * @brief  Get Padding Length for I2S mode
 * @param[in]  ssp SSP instance
 * @retval padding length
 * @note   I2S only
 */
__STATIC_INLINE uint32_t LL_SSP_GetDPDLength(SN_SPI0_Type *ssp)
{
    return ((LL_REG_READ(ssp->CTRL3) & SSP_CTRL3_DPDL) >> SSP_CTRL3_DPDL_Pos);
}

#if defined(USE_FULL_LL_DRIVER)
LL_Status_T LL_SPI_Init(SN_SPI0_Type *spi, LL_SPI_Init_T *spi_init);
LL_Status_T LL_SPI_DeInit(SN_SPI0_Type *spi);
void        LL_SPI_StructInit(LL_SPI_Init_T *spi_init);

LL_Status_T LL_I2S_Init(SN_SPI0_Type *i2s, LL_I2S_Init_T *i2s_init);
LL_Status_T LL_I2S_DeInit(SN_SPI0_Type *i2s);
void        LL_I2S_StructInit(LL_I2S_Init_T *i2s_init);
#endif /* USE_FULL_LL_DRIVER */

#ifdef __cplusplus
}
#endif

#endif /* _LL_SSP_H_ */
