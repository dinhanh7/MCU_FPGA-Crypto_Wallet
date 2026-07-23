/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _SN34F78X_LL_SPI_H_
#define _SN34F78X_LL_SPI_H_

/* Includes ------------------------------------------------------------------*/
#include "sn34f78x_ll.h"

/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/

// check whether SPI instance is correct
#define IS_LL_SPI_INSTANCE(__HANDLE__) ((__HANDLE__) == SN_SPI0 || \
                                        (__HANDLE__) == SN_SPI1 || \
                                        (__HANDLE__) == SN_SPI2)

// check whether SPI frame format is correct
#define IS_LL_SPI_FFMT(frame_format) ((frame_format) == LL_SPI_FFMT_SPI || \
                                      (frame_format) == LL_SPI_FFMT_MWR)

// check if spi master/slaver mode?
#define IS_LL_SPI_MODE(mode) (mode == LL_SPI_MODE_MASTER || \
                              mode == LL_SPI_MODE_SLAVE)

// check data length?
#define LL_SPI_DATA_LEN_MIN           (5U)
#define LL_SPI_DATA_LEN_MAX           (16U)
#define IS_LL_SPI_DATA_LEN(data_size) (data_size >= LL_SPI_DATA_LEN_MIN && \
                                       data_size <= LL_SPI_DATA_LEN_MAX)

// check is spi frame/sync polarity?
#define IS_LL_SPI_FS_POL(fs_polarity) (fs_polarity == LL_SSP_FS_POLARITY_LOW || \
                                       fs_polarity == LL_SSP_FS_POLARITY_HIGH)

// check is spi clock polarity?
#define IS_LL_SPI_CLK_POL(clk_polarity) (clk_polarity == LL_SSP_CLK_POLARITY_LOW || \
                                         clk_polarity == LL_SSP_CLK_POLARITY_HIGH)

// check is spi clock phase?
#define IS_LL_SPI_CLK_PHA(clk_phase) (clk_phase == LL_SSP_CLK_PHASE_1EDGE || \
                                      clk_phase == LL_SSP_CLK_PHASE_2EDGE)

// check is spi clock div?
#define LL_SPI0_CLK_DIV_MIN          (1U)
#define LL_SPI12_CLK_DIV_MIN         (2U)
#define LL_SPI_CLK_DIV_MAX           (512U)
#define IS_LL_SPI0_CLK_DIV(sclk_div) (sclk_div >= LL_SPI0_CLK_DIV_MIN && \
                                      sclk_div <= LL_SPI_CLK_DIV_MAX)

#define IS_LL_SPI12_CLK_DIV(sclk_div) (sclk_div >= LL_SPI12_CLK_DIV_MIN && \
                                       sclk_div <= LL_SPI_CLK_DIV_MAX)

// check is spi first bit?
#define IS_LL_SPI_MSB_LSB(first_bit) (first_bit == LL_SSP_FIRST_BIT_MSB || \
                                      first_bit == LL_SSP_FIRST_BIT_LSB)
/* Private functions prototypes ----------------------------------------------*/

#endif /* _SN34F78X_LL_SPI_H_ */
