/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _SN34F78X_LL_I2S_H_
#define _SN34F78X_LL_I2S_H_

/* Includes ------------------------------------------------------------------*/
#include "sn34f78x_ll.h"

/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/

// check whether I2S instance is correct
#define IS_LL_I2S_INSTANCE(__HANDLE__) ((__HANDLE__) == SN_SPI0 || \
                                        (__HANDLE__) == SN_SPI1 || \
                                        (__HANDLE__) == SN_SPI2)

// check if i2s master/slave mode?
#define IS_LL_I2S_MODE(mode) (mode == LL_SSP_MODE_SLAVE_MONO || mode == LL_SSP_MODE_SLAVE_STEREO || \
                              mode == LL_SSP_MODE_MASTER_MONO || mode == LL_SSP_MODE_MASTER_STEREO)

// check whether I2S standard is correct
#define IS_LL_I2S_STD(standard) ((standard) == LL_I2S_STANDARD_PHILIPS || \
                                 (standard) == LL_I2S_STANDARD_MSB ||     \
                                 (standard) == LL_I2S_STANDARD_LSB)

// check whether i2s channel length is in range?
#define IS_LL_I2S_CH_LEN(channel_len) (channel_len >= 16U && \
                                       channel_len <= 32U)

// check whether i2s data length is in range?
#define IS_LL_I2S_DATA_LEN(data_format) (data_format == LL_I2S_DATA_LEN_8B ||  \
                                         data_format == LL_I2S_DATA_LEN_16B || \
                                         data_format == LL_I2S_DATA_LEN_24B || \
                                         data_format == LL_I2S_DATA_LEN_32B)

// check whether i2s audio frequency is correct?
#define IS_LL_I2S_AUDIO_FREQ(audio_freq) ((audio_freq) == LL_I2S_AUDIO_FREQ_16K || \
                                          (audio_freq) == LL_I2S_AUDIO_FREQ_48K)

// check whether i2s cpol is correct?
#define IS_LL_I2S_CPOL(cpol) ((cpol) == LL_I2S_CPOL_LOW || \
                              (cpol) == LL_I2S_CPOL_HIGH)

/* Private functions prototypes ----------------------------------------------*/

#endif /* _SN34F78X_LL_I2S_H_ */
