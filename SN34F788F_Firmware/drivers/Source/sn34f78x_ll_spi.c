/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#include "sn34f78x_ll_spi.h"

#if defined(USE_FULL_LL_DRIVER)

/* Exported functions --------------------------------------------------------*/
/**
 * @brief  Initialize SPI registers according to the specified parameters in spi_init.
 * @param  spi SPI base Address
 * @param  spi_init pointer to a @ref LL_SPI_Init_T structure
 *         that contains the configuration information for the specified SPI peripheral.
 * @retval An ErrorStatus enumeration value:
 *           - LL_OK:     SPI registers are initialized according to spi_init content
 *           - LL_ERROR:  Not applicable
 */
LL_Status_T LL_SPI_Init(SN_SPI0_Type *spi, LL_SPI_Init_T *spi_init)
{
    /* Check the parameters */
    RET_FLAG_FALSE(IS_LL_SPI_INSTANCE(spi), LL_ERROR);
    RET_FLAG_FALSE(IS_LL_SPI_MODE(spi_init->mode), LL_ERROR);
    RET_FLAG_FALSE(IS_LL_SPI_FFMT(spi_init->frame_format), LL_ERROR);
    RET_FLAG_FALSE(IS_LL_SPI_DATA_LEN(spi_init->data_size), LL_ERROR);
    RET_FLAG_FALSE(IS_LL_SPI_FS_POL(spi_init->fs_polarity), LL_ERROR);
    RET_FLAG_FALSE(IS_LL_SPI_CLK_POL(spi_init->clk_polarity), LL_ERROR);
    RET_FLAG_FALSE(IS_LL_SPI_CLK_PHA(spi_init->clk_phase), LL_ERROR);
    RET_FLAG_FALSE(IS_LL_SPI_MSB_LSB(spi_init->first_bit), LL_ERROR);

    // Slave mode is not necessary for div setting
    if (spi_init->mode == LL_SPI_MODE_MASTER)
    {
        if (spi == SN_SPI0)
        {
            RET_FLAG_FALSE(IS_LL_SPI0_CLK_DIV(spi_init->sclk_div), LL_ERROR);
        }
        else
        {
            RET_FLAG_FALSE(IS_LL_SPI12_CLK_DIV(spi_init->sclk_div), LL_ERROR);
        }
    }

    /* spi/micro-wire mode select */
    switch (spi_init->frame_format)
    {
        case 0:               // default is spi mode currently
        case LL_SPI_FFMT_SPI: /* switch to SPI mode */
            LL_SSP_SetToMotorolaSPI(spi);

            LL_SSP_SetSELPolarity(spi, spi_init->fs_polarity);

            LL_SSP_SetPaddingCycle(spi, 0);

            /* Set Clock Idle status */
            LL_SSP_SetClockPolarity(spi, spi_init->clk_polarity);

            /* Set Clock phase */
            LL_SSP_SetClkPhase(spi, spi_init->clk_phase);

            break;
        case LL_SPI_FFMT_MWR: /* switch to micro-wire mode */
            LL_SSP_SetToMicrowireSPI(spi);
            if (spi_init->fs_polarity == LL_SSP_FS_POLARITY_LOW)
                LL_SSP_SetWSPolarity(spi, LL_I2S_CPOL_LOW);
            else
                LL_SSP_SetWSPolarity(spi, LL_I2S_CPOL_HIGH);
            break;

        default:
            return LL_ERROR;
    }

    /* Master/Slave mode select */
    switch (spi_init->mode)
    {
        case LL_SPI_MODE_MASTER: /* SPI Master (Output on MOSI, Input on MISO), default */
            LL_SSP_SetOpMode(spi, LL_SSP_MODE_MASTER_MONO);
            LL_SSP_SetSELToMaster(spi);
            LL_SSP_SetClkToMaster(spi);
            LL_SSP_SetSclkDivider(spi, spi_init->sclk_div);
            LL_SSP_SetContinuousTransferMode(spi);
            break;

        case LL_SPI_MODE_SLAVE: /* SPI Slave  (Output on MISO, Input on MOSI) */
            LL_SSP_SetOpMode(spi, LL_SSP_MODE_SLAVE_MONO);
            LL_SSP_SetSELToSlave(spi);
            LL_SSP_SetClkToSlave(spi);
            LL_SSP_SetSclkDivider(spi, spi_init->sclk_div);
            LL_SSP_EnableTXDOE(spi);
            break;

        default:
            return LL_ERROR;
    }

    /* Config bit order */
    switch (spi_init->first_bit)
    {
        default:
            return LL_ERROR;
        case LL_SSP_FIRST_BIT_MSB: // default
            LL_SSP_SetTransferBitOrder(spi, LL_SSP_FIRST_BIT_MSB);
            break;
        case LL_SSP_FIRST_BIT_LSB:
            LL_SSP_SetTransferBitOrder(spi, LL_SSP_FIRST_BIT_LSB);
            LL_SSP_EnableTXDOE(spi);
            break;
    }

    if (spi_init->data_size >= 5 && spi_init->data_size <= 16)
    {
        /* config data bits */
        LL_SSP_SetSerialDataLength(spi, spi_init->data_size);
    }
    else
        return LL_ERROR;

    /* Clear fifo status */
    LL_SSP_Reset(spi);

    /* Enable SPI */
    LL_SSP_Enable(spi);

    return LL_OK;
}

/**
 * @brief  De-initialize SPI registers .
 * @param  spi SPI base Address
 * @retval An ErrorStatus enumeration value:
 *           - LL_OK:     SPI registers are de-initialized
 *           - LL_ERROR:  de-initialized fail
 */
LL_Status_T LL_SPI_DeInit(SN_SPI0_Type *spi)
{
    /* Disable SPI */
    LL_SSP_Disable(spi);

    /* Reset SPI */
    LL_SSP_Reset(spi);

    return LL_OK;
}

/**
 * @brief  Set each @ref LL_SSP_Init_T field to default value.
 * @param  spi_init pointer to a @ref LL_SSP_Init_T structure  whose fields will be set to default values.
 * @retval None
 */
void LL_SPI_StructInit(LL_SPI_Init_T *spi_init)
{
    if (spi_init != NULL)
    {
        spi_init->frame_format = LL_SPI_FFMT_SPI;
        spi_init->mode         = LL_SPI_MODE_MASTER;
        spi_init->data_size    = LL_SPI_DATASIZE_8BIT;
        spi_init->fs_polarity  = LL_SSP_FS_POLARITY_LOW;
        spi_init->clk_polarity = LL_SSP_CLK_POLARITY_LOW;
        spi_init->clk_phase    = LL_SSP_CLK_PHASE_1EDGE;
        spi_init->first_bit    = LL_SSP_FIRST_BIT_MSB;
        spi_init->sclk_div     = 8;
        spi_init->tx_default   = 0;
    }
}
#endif /* USE_FULL_LL_DRIVER */
