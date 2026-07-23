/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#include "sn34f78x_ll_i2s.h"

#if defined(USE_FULL_LL_DRIVER)

/* Exported functions --------------------------------------------------------*/
/**
 * @brief  Initialize I2S registers according to the specified parameters in i2s_init.
 * @param  i2s I2S base Address
 * @param  i2s_init pointer to a @ref LL_I2S_Init_T structure
 *         that contains the configuration information for the specified I2S peripheral.
 * @retval An ErrorStatus enumeration value:
 *           - LL_OK:     I2S registers are initialized according to i2s_init content
 *           - LL_ERROR:  Not applicable
 */
LL_Status_T LL_I2S_Init(SN_SPI0_Type *i2s, LL_I2S_Init_T *i2s_init)
{
    /* Check the parameters */
    RET_FLAG_FALSE(IS_LL_I2S_INSTANCE(i2s), LL_ERROR);
    RET_FLAG_FALSE(IS_LL_I2S_MODE(i2s_init->mode), LL_ERROR);
    RET_FLAG_FALSE(IS_LL_I2S_STD(i2s_init->standard), LL_ERROR);
    RET_FLAG_FALSE(IS_LL_I2S_CH_LEN(i2s_init->channel_len), LL_ERROR);
    RET_FLAG_FALSE(IS_LL_I2S_DATA_LEN(i2s_init->data_format), LL_ERROR);
    RET_FLAG_FALSE(IS_LL_I2S_AUDIO_FREQ(i2s_init->audio_freq), LL_ERROR);
    RET_FLAG_FALSE(IS_LL_I2S_CPOL(i2s_init->cpol), LL_ERROR);

    /* switch to i2s mode */
    LL_SSP_SetToPhilipsI2S(i2s);

    /* Set tx rx data independent */
    LL_SSP_SetTxRxIndependent(i2s);

    /* Master/Slave mode select */
    switch (i2s_init->mode)
    {
        case LL_SSP_MODE_MASTER_MONO: /* i2s Master, mono */
        {
            LL_SSP_SetSELToMaster(i2s);
            LL_SSP_SetClkToMaster(i2s);
            LL_SSP_SetOpMode(i2s, LL_SSP_MODE_MASTER_MONO);
        }
        break;

        case LL_SSP_MODE_MASTER_STEREO: /* i2s Master, stereo */
        {
            LL_SSP_SetSELToMaster(i2s);
            LL_SSP_SetClkToMaster(i2s);
            LL_SSP_SetOpMode(i2s, LL_SSP_MODE_MASTER_STEREO);
        }
        break;

        case LL_SSP_MODE_SLAVE_MONO: /* i2s Slave, mono */
        {
            LL_SSP_SetSELToSlave(i2s);
            LL_SSP_SetClkToSlave(i2s);
            LL_SSP_SetOpMode(i2s, LL_SSP_MODE_SLAVE_MONO);
        }
        break;

        case LL_SSP_MODE_SLAVE_STEREO: /* i2s Slave, stereo */
        {
            LL_SSP_SetSELToSlave(i2s);
            LL_SSP_SetClkToSlave(i2s);
            LL_SSP_SetOpMode(i2s, LL_SSP_MODE_SLAVE_STEREO);
        }
        break;

        default:
            break;
    }

    /* WS active mode select */
    LL_SSP_SetWSPolarity(i2s, i2s_init->cpol);

    /* data format select */
    switch (i2s_init->standard)
    {
        case LL_I2S_STANDARD_PHILIPS:
        {
            LL_SSP_SetPaddingDataBack(i2s);
            LL_SSP_SetWsDataDistance(i2s, LL_SSP_WS_DATA_1SCLK);
        }
        break;

        case LL_I2S_STANDARD_LSB:
        {
            LL_SSP_SetPaddingDataFront(i2s);
            LL_SSP_SetWsDataDistance(i2s, LL_SSP_WS_DATA_0SCLK);
        }
        break;

        case LL_I2S_STANDARD_MSB:
        {
            LL_SSP_SetPaddingDataBack(i2s);
            LL_SSP_SetWsDataDistance(i2s, LL_SSP_WS_DATA_0SCLK);
        }
        break;

        default:
            break;
    }
    /* i2s word length */
    LL_SSP_SetPaddingLength(i2s, i2s_init->channel_len - i2s_init->data_format);
    LL_SSP_SetSerialDataLength(i2s, i2s_init->data_format);

    /* i2s divider calculate */
    uint32_t source_clock = 0;
    switch ((uint32_t)i2s)
    {
        case SN_SPI0_BASE:
        {
            if (LL_RCC_GetSSP0ClockSource() == LL_RCC_PERIPHERAL_CLKSRC_HCLK)
            {
                source_clock = LL_RCC_GetHCLKFreq();
            }
            else // LL_RCC_PERIPHERAL_CLKSRC_PLL
            {
                source_clock = LL_RCC_GetPLLCLKFreq();
            }
        }
        break;

        case SN_SPI1_BASE:
        {
            if (LL_RCC_GetSSP1ClockSource() == LL_RCC_PERIPHERAL_CLKSRC_HCLK)
            {
                source_clock = LL_RCC_GetHCLKFreq();
            }
            else // LL_RCC_PERIPHERAL_CLKSRC_PLL
            {
                source_clock = LL_RCC_GetPLLCLKFreq();
            }
        }
        break;

        case SN_SPI2_BASE:
        {
            if (LL_RCC_GetSSP2ClockSource() == LL_RCC_PERIPHERAL_CLKSRC_HCLK)
            {
                source_clock = LL_RCC_GetHCLKFreq();
            }
            else // LL_RCC_PERIPHERAL_CLKSRC_PLL
            {
                source_clock = LL_RCC_GetPLLCLKFreq();
            }
        }
        break;
    }
    uint32_t rel_sclk = i2s_init->audio_freq * i2s_init->channel_len * 2;
    uint32_t div_sclk = (source_clock / rel_sclk - 2) >> 1;

    LL_SSP_SetSclkDivider(i2s, div_sclk);
    LL_SSP_EnableTXDOE(i2s);

    /* i2s DPDL setting */
    LL_SSP_DisableDPDLEN(i2s);
    LL_SSP_SetDPDLength(i2s, 4);

    /* Clear fifo status */
    LL_SSP_Reset(i2s);

    return LL_OK;
}

/**
 * @brief  De-initialize I2S registers .
 * @param  i2s I2S base Address
 * @retval An ErrorStatus enumeration value:
 *           - LL_OK:     I2S registers are de-initialized
 *           - LL_ERROR:  de-initialized fail
 */
LL_Status_T LL_I2S_DeInit(SN_SPI0_Type *i2s)
{
    /* Disable I2S */
    LL_SSP_Disable(i2s);

    /* Reset I2S */
    LL_SSP_Reset(i2s);

    return LL_OK;
}

/**
 * @brief  Set each @ref LL_SSP_Init_T field to default value.
 * @param  i2s_init pointer to a @ref LL_SSP_Init_T structure  whose fields will be set to default values.
 * @retval None
 */
void LL_I2S_StructInit(LL_I2S_Init_T *i2s_init)
{
    if (i2s_init != NULL)
    {
        i2s_init->mode        = LL_SSP_MODE_MASTER_MONO;
        i2s_init->standard    = LL_I2S_STANDARD_PHILIPS;
        i2s_init->channel_len = 16;
        i2s_init->data_format = LL_I2S_DATA_LEN_8B;
        i2s_init->audio_freq  = LL_I2S_AUDIO_FREQ_48K;
        i2s_init->cpol        = LL_I2S_CPOL_LOW;
    }
}
#endif /* USE_FULL_LL_DRIVER */
