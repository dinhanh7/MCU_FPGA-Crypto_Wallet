/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#include "MX25L6433F.h"

#if (configUSE_NORFLASH_NONBLOCKING_DMA == 1U)
static DMA_Handle_T _norflash_dma_tx;
static DMA_Handle_T _norflash_dma_rx;
#endif /* configUSE_NORFLASH_NONBLOCKING_DMA */

#if (configUSE_NORFLASH_MIDDLEWARE == 1U)

/* set the initial value of spi master */
static SPI_Handle_T spi_master_handle =
    {
        .instance = SN_SPI0,
        .init =
            {
                .frame_format = SPI_FFMT_SPI,
                .mode         = SPI_MODE_MASTER,
                .data_size    = 8,
                .fs_polarity  = SPI_FS_POLARITY_LOW,
                .clk_polarity = SPI_CLK_POLARITY_LOW,
                .clk_phase    = SPI_CLK_PHASE_1EDGE,
                .sclk_div     = 3,
                .first_bit    = SPI_FIRST_BIT_MSB,

            },
};

/**
 * @brief Initialize the NOR FLASH
 * @param hnor pointer to a NORFlash_Handle_T structure that contains the configuration information for SPI FLASH
 * @retval HAL_Status_T
 */
HAL_Status_T MX25L6433F_Init(NORFlash_Handle_T *hnor)
{
    /* check the init parameters */
    ASSERT_PARAM(IS_NOR_CLK_POL(hnor));
    ASSERT_PARAM(IS_NOR_CLK_PHA(hnor));
    ASSERT_PARAM(IS_NOR_CLK_DIV(hnor));
    ASSERT_PARAM(IS_NOR_MSB_LSB(hnor));
    ASSERT_PARAM(IS_NOR_GPIO_PIN(hnor));

    /* if the flash is in DPD mode, return error */
    RET_FLAG_TRUE(HAL_FLAG_HAS(hnor->flash_state, NOR_FLASH_STATE_DPD), HAL_ERROR);

    SPI_Handle_T *hspi      = &spi_master_handle;
    GPIO_Init_T   gpio_init = {0};
    HAL_Status_T  status    = HAL_OK;

    /* assign nor_handle */
    ASSIGN_NOR_HANDLE();

    /* clear error code */
    CLEAR_ERROR_CODE(hnor);

    /* init SPI FLASH inner state */
    hnor->flash_state = NOR_FLASH_STATE_IDLE;

    if (HAL_FLAG_HAS(hnor->state, HAL_STATE_RESET))
    {
        gpio_init.pin  = hnor->init.cs_gpio_pin;
        gpio_init.mode = GPIO_MODE_OUTPUT;
        /* initialize GPIO */
        status = HAL_GPIO_Init(hnor->init.cs_gpio_port, &gpio_init);
        if (status != HAL_OK)
        {
            HAL_FLAG_WRITE(hnor->state, HAL_STATE_RESET);
            hnor->error_code = NOR_ERROR_INIT_GPIO;
            return status;
        }
        /* pull high chip select pin */
        HAL_GPIO_WritePin(hnor->init.cs_gpio_port, hnor->init.cs_gpio_pin, GPIO_PIN_HIGH);

#if (configUSE_NORFLASH_NONBLOCKING_DMA == 1U)
        HAL_LINKDMA(hnor, hdmatx, _norflash_dma_tx);
        HAL_LINKDMA(hnor, hdmarx, _norflash_dma_rx);
        HAL_LINKDMA(hspi, hdmatx, *hnor->hdmatx);
        HAL_LINKDMA(hspi, hdmarx, *hnor->hdmarx);
#endif /* configUSE_NORFLASH_NONBLOCKING_DMA */
    }
    else
    {
        /* re-init error */
        hnor->error_code = HAL_ERROR_REINIT;
        return HAL_ERROR;
    }

    /* set the SPI configuration */
    hspi->instance          = hnor->init.instance;
    hspi->init.clk_polarity = hnor->init.clk_polarity;
    hspi->init.clk_phase    = hnor->init.clk_phase;
    hspi->init.fs_polarity  = hnor->init.fs_polarity;
    hspi->init.sclk_div     = hnor->init.sclk_div;
    hspi->init.first_bit    = hnor->init.first_bit;

#if (configUSE_SPI_REGISTER_CALLBACKS == 1U)
    /* Register SPI of SPI_FLASH mspinit */
    HAL_SPI_RegisterCallback(hspi, HAL_SPI_MSP_INIT_CB_ID, _norflash_spi_msp_init_callback);
    HAL_SPI_RegisterCallback(hspi, HAL_SPI_MSP_DEINIT_CB_ID, _norflash_spi_msp_deinit_callback);
#endif /* configUSE_SPI_REGISTER_CALLBACKS */

    /* initialize SPI */
    status = HAL_SPI_Init(hspi);
    if (status != HAL_OK)
    {
        HAL_FLAG_WRITE(hnor->state, HAL_STATE_RESET);
        hnor->error_code = NOR_ERROR_INIT_SPI;
        return status;
    }

#if (configUSE_NORFLASH_NONBLOCKING_DMA == 1U && configUSE_SPI_REGISTER_CALLBACKS == 1U)
    /* After SPI init finished, Register SPI of SPI_FLASH Callback */
    HAL_SPI_RegisterCallback(hspi, HAL_SPI_TX_COMPLETE_CB_ID, _norflash_dma_tx_writepage_cb);
    HAL_SPI_RegisterCallback(hspi, HAL_SPI_RX_COMPLETE_CB_ID, _norflash_dma_rx_complete_cb);
#endif

    /* change SPI FLASH state to ready */
    HAL_FLAG_WRITE(hnor->state, HAL_STATE_READY);

    return HAL_OK;
}

/**
 * @brief DeInitialize the SPI FLASH
 * @param hnor SPI FLASH handle
 * @retval HAL_Status_T
 */
HAL_Status_T MX25L6433F_DeInit(NORFlash_Handle_T *hnor)
{
    /* check parameters */
    ASSERT_PARAM(hnor);

    /* if the flash is in DPD mode, return error */
    RET_FLAG_TRUE(HAL_FLAG_HAS(hnor->flash_state, NOR_FLASH_STATE_DPD), HAL_ERROR);

    SPI_Handle_T *hspi = &spi_master_handle;

    HAL_FLAG_WRITE(hnor->state, HAL_STATE_BUSY);

    /* reset SPI FLASH inner state */
    HAL_FLAG_WRITE(hnor->flash_state, NOR_FLASH_STATE_IDLE);

#if (configUSE_NORFLASH_NONBLOCKING_DMA == 1U)

    /* 1.uninitialize SPI */
    HAL_SPI_DeInit(hspi);

    /* 2.unlink dma*/
    if (hnor->hdmatx)
    {
        /* unlink the tx DMA and SPI FLASH */
        hnor->hdmatx = NULL;
    }

    if (hnor->hdmarx)
    {
        /* unlink the rx DMA and SPI FLASH */
        hnor->hdmarx = NULL;
    }

    /* 3.Release SPI of SPI_FLASH Callback */
    HAL_SPI_UnRegisterCallback(hspi, HAL_SPI_TX_COMPLETE_CB_ID);
    HAL_SPI_UnRegisterCallback(hspi, HAL_SPI_RX_COMPLETE_CB_ID);
#endif

#if (configUSE_SPI_REGISTER_CALLBACKS == 1U)
    /* Release SPI of SPI_FLASH mspinit */
    HAL_SPI_UnRegisterCallback(hspi, HAL_SPI_MSP_INIT_CB_ID);
    HAL_SPI_UnRegisterCallback(hspi, HAL_SPI_MSP_DEINIT_CB_ID);
#endif /* configUSE_SPI_REGISTER_CALLBACKS */

    /* uninitialize GPIO */
    HAL_GPIO_DeInit(hnor->init.cs_gpio_port, hnor->init.cs_gpio_pin);

    /* change NOR FLASH state to reset */
    HAL_FLAG_WRITE(hnor->state, HAL_STATE_RESET);

    /* release nor_handle */
    RELEASE_NOR_HANDLE();

    return HAL_OK;
}

/**
 * @brief NORFLASH error callback
 * @param hspi SPI handle
 * @retval None
 */
void MX25L6433F_ErrorCallback(NORFlash_Handle_T *hnor)
{
    HAL_GPIO_WritePin(hnor->init.cs_gpio_port, hnor->init.cs_gpio_pin, GPIO_PIN_HIGH);
    hnor->error_code = NOR_ERROR_DMA;

    HAL_FLAG_WRITE(hnor->flash_state, (hnor->flash_state & ~NOR_FLASH_STATE_REMAIN_DATA));

#if (configUSE_NORFLASH_REGISTER_CALLBACKS == 1U)
    if (hnor->ErrorCallback)
        hnor->ErrorCallback(hnor);
#else
    HAL_NORFlash_ErrorCallback(hnor);
#endif
}

#if (configUSE_NORFLASH_BLOCKING == 1)
/**
 * @brief Program an amount of data to SPI FLASH in blocking mode
 * @param hnor SPI FLASH handle
 * @param start_addr start physical SPI FLASH address
 * @param data_buffer pointer to data buffer
 * @param write_size amount of data to be programmed
 * @param timeout timeout duration
 * @retval HAL_Status_T
 */
HAL_Status_T MX25L6433F_Program(NORFlash_Handle_T *hnor, uint32_t start_addr, uint8_t *data_buffer, uint32_t write_size, uint32_t timeout)
{
    /* check parameters */
    ASSERT_PARAM(hnor);
    ASSERT_PARAM(data_buffer);
    ASSERT_PARAM(write_size);
    ASSERT_PARAM((start_addr + (write_size - 1U)) <= NOR_ADDRESS_END);

    /* check the SPI FLASH controller state */
    RET_FLAG_TRUE(HAL_FLAG_LACKS(hnor->state, HAL_STATE_READY), HAL_ERROR);
    RET_FLAG_TRUE(HAL_FLAG_HAS(hnor->state, HAL_STATE_BUSY), HAL_BUSY);
    RET_FLAG_TRUE(HAL_FLAG_HAS(hnor->flash_state, NOR_FLASH_STATE_DPD), HAL_ERROR);

    SPI_Handle_T *hspi        = &spi_master_handle;
    uint32_t      tick_start  = HAL_GetTick();
    uint32_t      new_timeout = timeout;
    uint8_t       data[4];
    uint8_t       cmd;
    uint32_t      temp;
    uint32_t      prog_size = 0;
    HAL_Status_T  status    = HAL_OK;
    hnor->tx.buffer         = (void *)data_buffer;
    hnor->tx.nor_addr       = start_addr;
    hnor->tx.xfer_count     = write_size;

    /* process locked */
    TAKE_MUTEX(hnor);

    /* clear error code */
    CLEAR_ERROR_CODE(hnor);

    /* change SPI FLASH state */
    HAL_FLAG_WRITE(hnor->state, HAL_STATE_BUSY_TX);
    HAL_FLAG_WRITE(hnor->flash_state, NOR_FLASH_STATE_PROGRAM);

    while (hnor->tx.xfer_count > 0)
    {
        /* spi flash address != (page address * multiple) */
        if (hnor->tx.nor_addr & (NOR_PAGE_SIZE - 1))
        {
            temp = NOR_PAGE_SIZE - (hnor->tx.nor_addr & (NOR_PAGE_SIZE - 1));
            if (hnor->tx.xfer_count >= temp)
            {
                prog_size = temp;
                hnor->tx.xfer_count -= temp;
            }
            else
            {
                prog_size           = hnor->tx.xfer_count;
                hnor->tx.xfer_count = 0;
            }
        }
        /* spi flash address == (page address * multiple) */
        else
        {
            if (hnor->tx.xfer_count >= NOR_PAGE_SIZE)
            {
                prog_size = NOR_PAGE_SIZE;
                hnor->tx.xfer_count -= prog_size;
            }
            else
            {
                prog_size           = hnor->tx.xfer_count;
                hnor->tx.xfer_count = 0;
            }
        }

        /* Enable WEL bit for transmission preparation*/
        status = _wren_and_wait_wel_ready(hnor, new_timeout);
        if (status != HAL_OK)
        {
            goto error;
        }

        /* refresh timeout */
        status = NORFlash_Get_NewTimeout(&tick_start, new_timeout, &new_timeout);
        if (status != HAL_OK)
        {
            HAL_FLAG_WRITE(hnor->error_code, NOR_ERROR_TIMEOUT);
            goto error;
        }
        /* pull low chip select pin */
        HAL_GPIO_WritePin(hnor->init.cs_gpio_port, hnor->init.cs_gpio_pin, GPIO_PIN_LOW);
        /* send page program command */
        cmd     = NOR_CMD_PAGR_PROGRAM;
        data[0] = cmd;
        data[1] = hnor->tx.nor_addr >> 16;
        data[2] = hnor->tx.nor_addr >> 8;
        data[3] = hnor->tx.nor_addr;
        status  = HAL_SPI_Transmit(hspi, data, 4, new_timeout);
        if (status != HAL_OK)
        {
            HAL_FLAG_WRITE(hnor->error_code, hspi->error_code);
            goto error;
        }

        /* refresh timeout */
        status = NORFlash_Get_NewTimeout(&tick_start, new_timeout, &new_timeout);
        if (status != HAL_OK)
        {
            HAL_FLAG_WRITE(hnor->error_code, NOR_ERROR_TIMEOUT);
            goto error;
        }
        /* program data */
        status = HAL_SPI_Transmit(hspi, hnor->tx.buffer, prog_size, new_timeout);
        if (status != HAL_OK)
        {
            HAL_FLAG_WRITE(hnor->error_code, hspi->error_code);
            goto error;
        }
        /* pull high chip select pin */
        HAL_GPIO_WritePin(hnor->init.cs_gpio_port, hnor->init.cs_gpio_pin, GPIO_PIN_HIGH);

        /* Wait WIP bit ready */
        status = _wait_wip_ready(hnor, new_timeout);
        if (status != HAL_OK)
        {
            goto error;
        }

        /* Check if write operation succeed last round*/
        status = _check_p_e_success(hnor, new_timeout);
        if (status != HAL_OK)
        {
            goto error;
        }

        hnor->tx.buffer += prog_size;
        hnor->tx.nor_addr += prog_size;
    }

error:
    if ((status == HAL_ERROR) || (status == HAL_TIMEOUT))
    {
        /* pull high chip select pin */
        HAL_GPIO_WritePin(hnor->init.cs_gpio_port, hnor->init.cs_gpio_pin, GPIO_PIN_HIGH);
    }

    /* set SPI FLASH state to ready after transmit */
    HAL_FLAG_WRITE(hnor->state, HAL_STATE_READY);
    HAL_FLAG_WRITE(hnor->flash_state, NOR_FLASH_STATE_IDLE);

    /* process unlocked */
    GIVE_MUTEX(hnor);

    return status;
}

/**
 * @brief Read an amount of data from SPI FLASH in blocking mode
 * @param hnor SPI FLASH handle
 * @param Type RX read type, legal parameter is NOR_TYPE_RX_READ or NOR_TYPE_RX_FAST_READ
 * @param start_addr start physical SPI FLASH address
 * @param data_buffer pointer to data buffer
 * @param read_size amount of data to be read
 * @param timeout timeout duration
 * @retval HAL_Status_T
 */
HAL_Status_T MX25L6433F_Read(NORFlash_Handle_T *hnor, uint32_t rx_type, uint32_t start_addr, uint8_t *data_buffer, uint32_t read_size, uint32_t timeout)
{
    /* check parameters */
    ASSERT_PARAM(hnor);
    ASSERT_PARAM(data_buffer);
    ASSERT_PARAM(read_size);
    ASSERT_PARAM(((rx_type == NOR_TYPE_RX_READ) || (rx_type == NOR_TYPE_RX_FAST_READ)));
    ASSERT_PARAM((start_addr + (read_size - 1U)) <= NOR_ADDRESS_END);

    /* check the SPI FLASH controller state */
    RET_FLAG_TRUE(HAL_FLAG_LACKS(hnor->state, HAL_STATE_READY), HAL_ERROR);
    RET_FLAG_TRUE(HAL_FLAG_HAS(hnor->state, HAL_STATE_BUSY), HAL_BUSY);
    RET_FLAG_TRUE(HAL_FLAG_HAS(hnor->flash_state, NOR_FLASH_STATE_DPD), HAL_ERROR);

    SPI_Handle_T *hspi        = &spi_master_handle;
    uint32_t      tick_start  = HAL_GetTick();
    uint32_t      new_timeout = timeout;
    uint8_t       data[4];
    uint8_t       cmd;
    HAL_Status_T  status = HAL_OK;

    /* process locked */
    TAKE_MUTEX(hnor);

    /* change SPI FLASH state */
    HAL_FLAG_WRITE(hnor->state, HAL_STATE_BUSY_RX);
    HAL_FLAG_WRITE(hnor->flash_state, NOR_FLASH_STATE_READ);

    /* clear error code */
    CLEAR_ERROR_CODE(hnor);

    switch (rx_type)
    {
        case NOR_TYPE_RX_READ:
            /* send read command */
            cmd = NOR_CMD_READ;
            break;
        case NOR_TYPE_RX_FAST_READ:
            /* send fast_read command */
            cmd = NOR_CMD_FAST_READ;
            break;
    }

    /* pull low chip select pin */
    HAL_GPIO_WritePin(hnor->init.cs_gpio_port, hnor->init.cs_gpio_pin, GPIO_PIN_LOW);
    /* send read or fast_read command */
    data[0] = cmd;
    data[1] = start_addr >> 16;
    data[2] = start_addr >> 8;
    data[3] = start_addr;
    status  = HAL_SPI_Transmit(hspi, data, 4, new_timeout);
    if (status != HAL_OK)
    {
        HAL_FLAG_WRITE(hnor->error_code, hspi->error_code);
        goto error;
    }

    if (rx_type == NOR_TYPE_RX_FAST_READ)
    {
        /* refresh timeout */
        status = NORFlash_Get_NewTimeout(&tick_start, new_timeout, &new_timeout);
        if (status != HAL_OK)
        {
            HAL_FLAG_WRITE(hnor->error_code, NOR_ERROR_TIMEOUT);
            goto error;
        }
        /* dummy cycle */
        data[0] = 0;
        status  = HAL_SPI_Transmit(hspi, data, 1, new_timeout);
        if (status != HAL_OK)
        {
            HAL_FLAG_WRITE(hnor->error_code, hspi->error_code);
            goto error;
        }
    }

    /* refresh timeout */
    status = NORFlash_Get_NewTimeout(&tick_start, new_timeout, &new_timeout);
    if (status != HAL_OK)
    {
        HAL_FLAG_WRITE(hnor->error_code, NOR_ERROR_TIMEOUT);
        goto error;
    }
    /* read data */
    status = HAL_SPI_Receive(hspi, data_buffer, read_size, new_timeout);
    if (status != HAL_OK)
    {
        HAL_FLAG_WRITE(hnor->error_code, hspi->error_code);
        goto error;
    }
    /* pull high chip select pin */
    HAL_GPIO_WritePin(hnor->init.cs_gpio_port, hnor->init.cs_gpio_pin, GPIO_PIN_HIGH);

error:
    if ((status == HAL_ERROR) || (status == HAL_TIMEOUT))
    {
        /* pull high chip select pin */
        HAL_GPIO_WritePin(hnor->init.cs_gpio_port, hnor->init.cs_gpio_pin, GPIO_PIN_HIGH);
    }

    /* set SPI FLASH state to ready after receive */
    HAL_FLAG_WRITE(hnor->state, HAL_STATE_READY);
    HAL_FLAG_WRITE(hnor->flash_state, NOR_FLASH_STATE_IDLE);

    /* process unlocked */
    GIVE_MUTEX(hnor);

    return status;
}
#endif

#if (configUSE_NORFLASH_NONBLOCKING_DMA == 1U)
/**
 * @brief Program an amount of data to SPI FLASH in DMA mode
 * @param hnor SPI FLASH handle
 * @param start_addr start physical SPI FLASH address
 * @param data_buffer pointer to data buffer
 * @param write_size amount of data to be programmed
 * @retval HAL_Status_T
 */
HAL_Status_T MX25L6433F_Program_DMA(NORFlash_Handle_T *hnor, uint32_t start_addr, uint8_t *data_buffer, uint32_t write_size)
{
    /* check parameters */
    ASSERT_PARAM(hnor);
    ASSERT_PARAM(hnor->hdmatx);
    ASSERT_PARAM(data_buffer);
    ASSERT_PARAM(write_size);
    ASSERT_PARAM((start_addr + (write_size - 1U)) <= NOR_ADDRESS_END);

    /* check the SPI FLASH controller state */
    RET_FLAG_TRUE(HAL_FLAG_LACKS(hnor->state, HAL_STATE_READY), HAL_ERROR);
    RET_FLAG_TRUE(HAL_FLAG_HAS(hnor->state, HAL_STATE_BUSY), HAL_BUSY);
    RET_FLAG_TRUE(HAL_FLAG_HAS(hnor->flash_state, NOR_FLASH_STATE_DPD), HAL_ERROR);

    SPI_Handle_T *hspi       = &spi_master_handle;
    uint32_t      tick_start = HAL_GetTick();
    uint8_t       data[4];
    uint8_t       cmd;
    uint32_t      temp;
    uint32_t      prog_size = 0;
    HAL_Status_T  status    = HAL_OK;
    hnor->tx.buffer         = (void *)data_buffer;
    hnor->tx.nor_addr       = start_addr;
    hnor->tx.xfer_count     = write_size;

    /* process locked */
    TAKE_MUTEX(hnor);

    /* clear error code */
    CLEAR_ERROR_CODE(hnor);

    /* change SPI FLASH state */
    HAL_FLAG_WRITE(hnor->state, HAL_STATE_BUSY_TX);
    HAL_FLAG_WRITE(hnor->flash_state, NOR_FLASH_STATE_PROGRAM);

    /* remained data to be programmed to SPI FLASH */
    HAL_FLAG_SET(hnor->flash_state, NOR_FLASH_STATE_REMAIN_DATA);

    /* spi flash address != (page address * multiple) */
    if (hnor->tx.nor_addr & (NOR_PAGE_SIZE - 1))
    {
        temp = NOR_PAGE_SIZE - (hnor->tx.nor_addr & (NOR_PAGE_SIZE - 1));
        if (hnor->tx.xfer_count >= temp)
        {
            prog_size = temp;
            hnor->tx.xfer_count -= temp;
        }
        else
        {
            prog_size           = hnor->tx.xfer_count;
            hnor->tx.xfer_count = 0;
        }
    }
    /* spi flash address == (page address * multiple) */
    else
    {
        if (hnor->tx.xfer_count >= NOR_PAGE_SIZE)
        {
            prog_size = NOR_PAGE_SIZE;
            hnor->tx.xfer_count -= prog_size;
        }
        else
        {
            prog_size           = hnor->tx.xfer_count;
            hnor->tx.xfer_count = 0;
        }
    }

    status = _wren_and_wait_wel_ready(hnor, NOR_CMD_WAIT_TIME);
    if (status != HAL_OK)
    {
        goto error;
    }

    hnor->tx.nor_addr += prog_size;
    hnor->tx.buffer += prog_size;

    /* pull low chip select pin */
    HAL_GPIO_WritePin(hnor->init.cs_gpio_port, hnor->init.cs_gpio_pin, GPIO_PIN_LOW);
    /* send page program command */
    cmd     = NOR_CMD_PAGR_PROGRAM;
    data[0] = cmd;
    data[1] = start_addr >> 16;
    data[2] = start_addr >> 8;
    data[3] = start_addr;
    status  = HAL_SPI_Transmit(hspi, data, 4, NOR_CMD_WAIT_TIME);
    if (status != HAL_OK)
    {
        HAL_FLAG_WRITE(hnor->error_code, hspi->error_code);
        HAL_GPIO_WritePin(hnor->init.cs_gpio_port, hnor->init.cs_gpio_pin, GPIO_PIN_HIGH);
        goto error;
    }

    /* program data */
    status = HAL_SPI_Transmit_DMA(hspi, data_buffer, prog_size);
    if (status != HAL_OK)
    {
        HAL_FLAG_WRITE(hnor->error_code, hspi->error_code);
        HAL_GPIO_WritePin(hnor->init.cs_gpio_port, hnor->init.cs_gpio_pin, GPIO_PIN_HIGH);
        goto error;
    }

error:
    if ((status == HAL_ERROR) || (status == HAL_TIMEOUT))
    {
        /* pull high chip select pin */
        HAL_GPIO_WritePin(hnor->init.cs_gpio_port, hnor->init.cs_gpio_pin, GPIO_PIN_HIGH);
    }

    /* process unlocked */
    GIVE_MUTEX(hnor);

    return status;
}

/**
 * @brief Read an amount of data from SPI FLASH in DMA mode
 * @param hnor SPI FLASH handle
 * @param Type RX read type, legal parameter is NOR_TYPE_RX_READ or NOR_TYPE_RX_FAST_READ
 * @param start_addr start physical SPI FLASH address
 * @param data_buffer pointer to data buffer
 * @param read_size amount of data to be read
 * @retval HAL_Status_T
 */
HAL_Status_T MX25L6433F_Read_DMA(NORFlash_Handle_T *hnor, uint32_t rx_type, uint32_t start_addr, uint8_t *data_buffer, uint32_t read_size)
{
    /* check parameters */
    ASSERT_PARAM(hnor);
    ASSERT_PARAM(hnor->hdmarx);
    ASSERT_PARAM(data_buffer);
    ASSERT_PARAM(read_size);
    ASSERT_PARAM(((rx_type == NOR_TYPE_RX_READ) || (rx_type == NOR_TYPE_RX_FAST_READ)));
    ASSERT_PARAM((start_addr + (read_size - 1U)) <= NOR_ADDRESS_END);

    /* check the SPI FLASH controller state */
    RET_FLAG_TRUE(HAL_FLAG_LACKS(hnor->state, HAL_STATE_READY), HAL_ERROR);
    RET_FLAG_TRUE(HAL_FLAG_HAS(hnor->state, HAL_STATE_BUSY), HAL_BUSY);
    RET_FLAG_TRUE(HAL_FLAG_HAS(hnor->flash_state, NOR_FLASH_STATE_DPD), HAL_ERROR);

    SPI_Handle_T *hspi       = &spi_master_handle;
    uint32_t      tick_start = HAL_GetTick();
    uint8_t       data[4];
    uint8_t       cmd;
    HAL_Status_T  status = HAL_OK;

    /* process locked */
    TAKE_MUTEX(hnor);

    /* change SPI FLASH state */
    HAL_FLAG_WRITE(hnor->state, HAL_STATE_BUSY_RX);
    HAL_FLAG_WRITE(hnor->flash_state, NOR_FLASH_STATE_READ);

    /* clear error code */
    CLEAR_ERROR_CODE(hnor);

    switch (rx_type)
    {
        case NOR_TYPE_RX_READ:
            /* send read command */
            cmd = NOR_CMD_READ;
            break;
        case NOR_TYPE_RX_FAST_READ:
            /* send fast_read command */
            cmd = NOR_CMD_FAST_READ;
            break;
    }

    /* pull low chip select pin */
    HAL_GPIO_WritePin(hnor->init.cs_gpio_port, hnor->init.cs_gpio_pin, GPIO_PIN_LOW);
    /* send read or fast_read command */
    data[0] = cmd;
    data[1] = start_addr >> 16;
    data[2] = start_addr >> 8;
    data[3] = start_addr;
    status  = HAL_SPI_Transmit(hspi, data, 4, NOR_CMD_WAIT_TIME);
    if (status != HAL_OK)
    {
        HAL_FLAG_WRITE(hnor->error_code, hspi->error_code);
        goto error;
    }

    if (rx_type == NOR_TYPE_RX_FAST_READ)
    {
        /* dummy cycle */
        data[0] = 0;
        status  = HAL_SPI_Transmit(hspi, data, 1, NOR_CMD_WAIT_TIME);
        if (status != HAL_OK)
        {
            HAL_FLAG_WRITE(hnor->error_code, hspi->error_code);
            goto error;
        }
    }

    /* read data */
    status = HAL_SPI_Receive_DMA(hspi, data_buffer, read_size);
    if (status != HAL_OK)
    {
        HAL_FLAG_WRITE(hnor->error_code, hspi->error_code);
        goto error;
    }

error:
    if ((status == HAL_ERROR) || (status == HAL_TIMEOUT))
    {
        /* pull high chip select pin */
        HAL_GPIO_WritePin(hnor->init.cs_gpio_port, hnor->init.cs_gpio_pin, GPIO_PIN_HIGH);
    }

    /* process unlocked */
    GIVE_MUTEX(hnor);

    return status;
}
#endif

/**
 * @brief Protect Block from writing and erasing
 * @param hnor NOR FLASH handle
 * @param block_area block count to be protect @ref protect_area
 * @param timeout timeout duration
 * @retval HAL_Status_T
 */
HAL_Status_T MX25L6433F_ProtectBlock(NORFlash_Handle_T *hnor, uint32_t block_area, uint32_t timeout)
{
    /* check parameters */
    ASSERT_PARAM(hnor);

    RET_FLAG_TRUE(HAL_FLAG_HAS(hnor->flash_state, NOR_FLASH_STATE_DPD), HAL_ERROR);

    SPI_Handle_T *hspi        = &spi_master_handle;
    uint32_t      tick_start  = HAL_GetTick();
    uint32_t      new_timeout = timeout;
    uint8_t       status_reg;
    uint8_t       config_reg;
    uint8_t       data[2] = {0};
    uint8_t       cmd;
    HAL_Status_T  status = HAL_OK;

    /* process locked */
    TAKE_MUTEX(hnor);

    /* change SPI FLASH state */
    HAL_FLAG_WRITE(hnor->state, HAL_STATE_BUSY);
    HAL_FLAG_WRITE(hnor->flash_state, NOR_FLASH_STATE_PROGRAM);

    /* Read status reg */
    HAL_GPIO_WritePin(hnor->init.cs_gpio_port, hnor->init.cs_gpio_pin, GPIO_PIN_LOW);
    cmd    = NOR_CMD_READ_STATUS_REG;
    status = HAL_SPI_Transmit(hspi, &cmd, 1, new_timeout);
    if (status != HAL_OK)
    {
        HAL_FLAG_WRITE(hnor->error_code, hspi->error_code);
        goto error;
    }
    status = HAL_SPI_Receive(hspi, &status_reg, 1, new_timeout);
    if (status != HAL_OK)
    {
        HAL_FLAG_WRITE(hnor->error_code, hspi->error_code);
        goto error;
    }
    HAL_GPIO_WritePin(hnor->init.cs_gpio_port, hnor->init.cs_gpio_pin, GPIO_PIN_HIGH);
    /* end 'Read status reg' */

    /* Read configuration reg */
    HAL_GPIO_WritePin(hnor->init.cs_gpio_port, hnor->init.cs_gpio_pin, GPIO_PIN_LOW);
    cmd    = NOR_CMD_READ_CONFIG_REG;
    status = HAL_SPI_Transmit(hspi, &cmd, 1, new_timeout);
    if (status != HAL_OK)
    {
        HAL_FLAG_WRITE(hnor->error_code, hspi->error_code);
        goto error;
    }
    status = HAL_SPI_Receive(hspi, &config_reg, 1, new_timeout);
    if (status != HAL_OK)
    {
        HAL_FLAG_WRITE(hnor->error_code, hspi->error_code);
        goto error;
    }
    HAL_GPIO_WritePin(hnor->init.cs_gpio_port, hnor->init.cs_gpio_pin, GPIO_PIN_HIGH);
    /* end 'Read configuration reg' */

    /* Enable WEL bit for transmission preparation*/
    status = _wren_and_wait_wel_ready(hnor, new_timeout);
    if (status != HAL_OK)
    {
        goto error;
    }

    /* Wirte status and configuration reg */
    data[0] = status_reg & 0x03; // 0x03 = 00000011, BP0-BP3 bits = 0000, SRWD,QE bit set to 0 default
    data[1] = config_reg & 0x40; // 0x40 = 01000000, T/B bit = 0, ODS bit set to 0 default
    switch (block_area)
    {
        case NOR_BLOCK_0:
            data[0] |= 0x04; // (BP3,BP2,BP1,BP0) = 0001(00)
            data[1] |= 0x08; // T/B = 1
            break;
        case NOR_BLOCK_0_TO_1:
            data[0] |= 0x08; // (BP3,BP2,BP1,BP0) = 0010(00)
            data[1] |= 0x08; // T/B = 1
            break;
        case NOR_BLOCK_0_TO_3:
            data[0] |= 0x0C; // (BP3,BP2,BP1,BP0) = 0011(00)
            data[1] |= 0x08; // T/B = 1
            break;
        case NOR_BLOCK_0_TO_7:
            data[0] |= 0x10; // (BP3,BP2,BP1,BP0) = 0100(00)
            data[1] |= 0x08; // T/B = 1
            break;
        case NOR_BLOCK_0_TO_15:
            data[0] |= 0x14; // (BP3,BP2,BP1,BP0) = 0101(00)
            data[1] |= 0x08; // T/B = 1
            break;
        case NOR_BLOCK_0_TO_31:
            data[0] |= 0x18; // (BP3,BP2,BP1,BP0) = 0110(00)
            data[1] |= 0x08; // T/B = 1
            break;
        case NOR_BLOCK_0_TO_63:
            data[0] |= 0x1C; // (BP3,BP2,BP1,BP0) = 0111(00)
            data[1] |= 0x08; // T/B = 1
            break;
        case NOR_BLOCK_ALL:
            data[0] |= 0x20; // (BP3,BP2,BP1,BP0) = 1000(00)
            data[1] |= 0x08; // T/B = 1
            break;
        case NOR_BLOCK_127:
            data[0] |= 0x04; // (BP3,BP2,BP1,BP0) = 0001(00)
            break;
        case NOR_BLOCK_126_TO_127:
            data[0] |= 0x08; // (BP3,BP2,BP1,BP0) = 0010(00)
            break;
        case NOR_BLOCK_124_TO_127:
            data[0] |= 0x0C; // (BP3,BP2,BP1,BP0) = 0011(00)
            break;
        case NOR_BLOCK_120_TO_127:
            data[0] |= 0x10; // (BP3,BP2,BP1,BP0) = 0100(00)
            break;
        case NOR_BLOCK_112_TO_127:
            data[0] |= 0x14; // (BP3,BP2,BP1,BP0) = 0101(00)
            break;
        case NOR_BLOCK_96_TO_127:
            data[0] |= 0x18; // (BP3,BP2,BP1,BP0) = 0110(00)
            break;
        case NOR_BLOCK_64_TO_127:
            data[0] |= 0x1C; // (BP3,BP2,BP1,BP0) = 0111(00)
            break;
        default:
            return HAL_ERROR;
    }

    /* Write status reg */
    HAL_GPIO_WritePin(hnor->init.cs_gpio_port, hnor->init.cs_gpio_pin, GPIO_PIN_LOW);
    cmd    = NOR_CMD_WRITE_STATUS_REG;
    status = HAL_SPI_Transmit(hspi, &cmd, 1, new_timeout);
    if (status != HAL_OK)
    {
        HAL_FLAG_WRITE(hnor->error_code, hspi->error_code);
        goto error;
    }

    status = HAL_SPI_Transmit(hspi, data, 2, new_timeout);
    if (status != HAL_OK)
    {
        HAL_FLAG_WRITE(hnor->error_code, hspi->error_code);
        goto error;
    }
    HAL_GPIO_WritePin(hnor->init.cs_gpio_port, hnor->init.cs_gpio_pin, GPIO_PIN_HIGH);
    /* end 'Write status reg' */

    /* Wait WIP bit ready */
    status = _wait_wip_ready(hnor, NOR_CMD_WAIT_TIME);
    if (status != HAL_OK)
    {
        goto error;
    }

    /* Read status reg */
    HAL_GPIO_WritePin(hnor->init.cs_gpio_port, hnor->init.cs_gpio_pin, GPIO_PIN_LOW);
    cmd    = NOR_CMD_READ_STATUS_REG;
    status = HAL_SPI_Transmit(hspi, &cmd, 1, new_timeout);
    if (status != HAL_OK)
    {
        HAL_FLAG_WRITE(hnor->error_code, hspi->error_code);
        goto error;
    }
    status_reg = 0;
    status     = HAL_SPI_Receive(hspi, &status_reg, 1, new_timeout);
    if (status != HAL_OK)
    {
        HAL_FLAG_WRITE(hnor->error_code, hspi->error_code);
        goto error;
    }
    HAL_GPIO_WritePin(hnor->init.cs_gpio_port, hnor->init.cs_gpio_pin, GPIO_PIN_HIGH);
    /* end 'Read status reg' */

    if ((data[0] & 0xFC) != (status_reg & 0xFC)) // do not need compare wel bit and wip bit
    {
        goto error;
    }

error:
    if ((status == HAL_ERROR) || (status == HAL_TIMEOUT))
    {
        /* pull high chip select pin */
        HAL_GPIO_WritePin(hnor->init.cs_gpio_port, hnor->init.cs_gpio_pin, GPIO_PIN_HIGH);
    }

    /* set SPI FLASH state to ready after receive */
    HAL_FLAG_WRITE(hnor->state, HAL_STATE_READY);
    HAL_FLAG_WRITE(hnor->flash_state, NOR_FLASH_STATE_IDLE);

    /* process unlocked */
    GIVE_MUTEX(hnor);

    return status;
}

/**
 * @brief Protect all block from writing and erasing
 * @param hnor NOR FLASH handle
 * @param timeout timeout duration
 * @retval HAL_Status_T
 */
HAL_Status_T MX25L6433F_ProtectBlockAll(NORFlash_Handle_T *hnor, uint32_t timeout)
{
    /* check parameters */
    ASSERT_PARAM(hnor);

    RET_FLAG_TRUE(HAL_FLAG_HAS(hnor->flash_state, NOR_FLASH_STATE_DPD), HAL_ERROR);

    SPI_Handle_T *hspi        = &spi_master_handle;
    uint32_t      tick_start  = HAL_GetTick();
    uint32_t      new_timeout = timeout;
    uint8_t       status_reg;
    uint8_t       config_reg;
    uint8_t       data[2] = {0};
    uint8_t       cmd;
    HAL_Status_T  status = HAL_OK;

    /* process locked */
    TAKE_MUTEX(hnor);

    /* change SPI FLASH state */
    HAL_FLAG_WRITE(hnor->state, HAL_STATE_BUSY);
    HAL_FLAG_WRITE(hnor->flash_state, NOR_FLASH_STATE_PROGRAM);

    /* Enable WEL bit for transmission preparation*/
    status = _wren_and_wait_wel_ready(hnor, new_timeout);
    if (status != HAL_OK)
    {
        goto error;
    }

    /* Read status reg */
    HAL_GPIO_WritePin(hnor->init.cs_gpio_port, hnor->init.cs_gpio_pin, GPIO_PIN_LOW);
    cmd    = NOR_CMD_READ_STATUS_REG;
    status = HAL_SPI_Transmit(hspi, &cmd, 1, new_timeout);
    if (status != HAL_OK)
    {
        HAL_FLAG_WRITE(hnor->error_code, hspi->error_code);
        goto error;
    }
    status = HAL_SPI_Receive(hspi, &status_reg, 1, new_timeout);
    if (status != HAL_OK)
    {
        HAL_FLAG_WRITE(hnor->error_code, hspi->error_code);
        goto error;
    }
    HAL_GPIO_WritePin(hnor->init.cs_gpio_port, hnor->init.cs_gpio_pin, GPIO_PIN_HIGH);
    /* end 'Read status reg' */

    /* Read configuration reg */
    HAL_GPIO_WritePin(hnor->init.cs_gpio_port, hnor->init.cs_gpio_pin, GPIO_PIN_LOW);
    cmd    = NOR_CMD_READ_CONFIG_REG;
    status = HAL_SPI_Transmit(hspi, &cmd, 1, new_timeout);
    if (status != HAL_OK)
    {
        HAL_FLAG_WRITE(hnor->error_code, hspi->error_code);
        goto error;
    }
    status = HAL_SPI_Receive(hspi, &config_reg, 1, new_timeout);
    if (status != HAL_OK)
    {
        HAL_FLAG_WRITE(hnor->error_code, hspi->error_code);
        goto error;
    }
    HAL_GPIO_WritePin(hnor->init.cs_gpio_port, hnor->init.cs_gpio_pin, GPIO_PIN_HIGH);
    /* end 'Read configuration reg' */

    data[0] = status_reg | 0x20; // 0x20 = 00100000,  BP0-BP3 bits = 1000, SRWD,QE bit set to 0 default
    data[1] = config_reg;

    /* Write status reg */
    HAL_GPIO_WritePin(hnor->init.cs_gpio_port, hnor->init.cs_gpio_pin, GPIO_PIN_LOW);
    cmd    = NOR_CMD_WRITE_STATUS_REG;
    status = HAL_SPI_Transmit(hspi, &cmd, 1, new_timeout);
    if (status != HAL_OK)
    {
        HAL_FLAG_WRITE(hnor->error_code, hspi->error_code);
        goto error;
    }

    status = HAL_SPI_Transmit(hspi, data, 2, new_timeout);
    if (status != HAL_OK)
    {
        HAL_FLAG_WRITE(hnor->error_code, hspi->error_code);
        goto error;
    }
    HAL_GPIO_WritePin(hnor->init.cs_gpio_port, hnor->init.cs_gpio_pin, GPIO_PIN_HIGH);
    /* end 'Write status reg' */

    /* Wait WIP bit ready */
    status = _wait_wip_ready(hnor, NOR_CMD_WAIT_TIME);
    if (status != HAL_OK)
    {
        goto error;
    }

    /* Read status reg */
    HAL_GPIO_WritePin(hnor->init.cs_gpio_port, hnor->init.cs_gpio_pin, GPIO_PIN_LOW);
    cmd    = NOR_CMD_READ_STATUS_REG;
    status = HAL_SPI_Transmit(hspi, &cmd, 1, new_timeout);
    if (status != HAL_OK)
    {
        HAL_FLAG_WRITE(hnor->error_code, hspi->error_code);
        goto error;
    }
    status_reg = 0;
    status     = HAL_SPI_Receive(hspi, &status_reg, 1, new_timeout);
    if (status != HAL_OK)
    {
        HAL_FLAG_WRITE(hnor->error_code, hspi->error_code);
        goto error;
    }
    HAL_GPIO_WritePin(hnor->init.cs_gpio_port, hnor->init.cs_gpio_pin, GPIO_PIN_HIGH);
    /* end 'Read status reg' */

    if ((data[0] & 0xFC) != (status_reg & 0xFC)) // do not need compare wel bit and wip bit
        goto error;

error:
    if ((status == HAL_ERROR) || (status == HAL_TIMEOUT))
    {
        /* pull high chip select pin */
        HAL_GPIO_WritePin(hnor->init.cs_gpio_port, hnor->init.cs_gpio_pin, GPIO_PIN_HIGH);
    }

    /* set SPI FLASH state to ready after receive */
    HAL_FLAG_WRITE(hnor->state, HAL_STATE_READY);
    HAL_FLAG_WRITE(hnor->flash_state, NOR_FLASH_STATE_IDLE);

    /* process unlocked */
    GIVE_MUTEX(hnor);

    return status;
}

HAL_Status_T MX25L6433F_ProtectResumeAll(NORFlash_Handle_T *hnor, uint32_t timeout)
{
    /* check parameters */
    ASSERT_PARAM(hnor);

    RET_FLAG_TRUE(HAL_FLAG_HAS(hnor->flash_state, NOR_FLASH_STATE_DPD), HAL_ERROR);

    SPI_Handle_T *hspi        = &spi_master_handle;
    uint32_t      tick_start  = HAL_GetTick();
    uint32_t      new_timeout = timeout;
    uint8_t       status_reg;
    uint8_t       config_reg;
    uint8_t       data[2] = {0};
    uint8_t       cmd;
    HAL_Status_T  status = HAL_OK;

    /* process locked */
    TAKE_MUTEX(hnor);

    /* change SPI FLASH state */
    HAL_FLAG_WRITE(hnor->state, HAL_STATE_BUSY);
    HAL_FLAG_WRITE(hnor->flash_state, NOR_FLASH_STATE_PROGRAM);

    /* Read status reg */
    HAL_GPIO_WritePin(hnor->init.cs_gpio_port, hnor->init.cs_gpio_pin, GPIO_PIN_LOW);
    cmd    = NOR_CMD_READ_STATUS_REG;
    status = HAL_SPI_Transmit(hspi, &cmd, 1, new_timeout);
    if (status != HAL_OK)
    {
        HAL_FLAG_WRITE(hnor->error_code, hspi->error_code);
        goto error;
    }
    status = HAL_SPI_Receive(hspi, &status_reg, 1, new_timeout);
    if (status != HAL_OK)
    {
        HAL_FLAG_WRITE(hnor->error_code, hspi->error_code);
        goto error;
    }
    HAL_GPIO_WritePin(hnor->init.cs_gpio_port, hnor->init.cs_gpio_pin, GPIO_PIN_HIGH);
    /* end 'Read status reg' */

    /* Read configuration reg */
    HAL_GPIO_WritePin(hnor->init.cs_gpio_port, hnor->init.cs_gpio_pin, GPIO_PIN_LOW);
    cmd    = NOR_CMD_READ_CONFIG_REG;
    status = HAL_SPI_Transmit(hspi, &cmd, 1, new_timeout);
    if (status != HAL_OK)
    {
        HAL_FLAG_WRITE(hnor->error_code, hspi->error_code);
        goto error;
    }
    status = HAL_SPI_Receive(hspi, &config_reg, 1, new_timeout);
    if (status != HAL_OK)
    {
        HAL_FLAG_WRITE(hnor->error_code, hspi->error_code);
        goto error;
    }
    HAL_GPIO_WritePin(hnor->init.cs_gpio_port, hnor->init.cs_gpio_pin, GPIO_PIN_HIGH);
    /* end 'Read configuration reg' */

    /* Enable WEL bit for transmission preparation*/
    status = _wren_and_wait_wel_ready(hnor, new_timeout);
    if (status != HAL_OK)
    {
        goto error;
    }

    data[0] = status_reg & 0x03; // 0x03 = 00000011,  BP0-BP3 bits = 0000, SRWD,QE bit set to 0 default
    // data[0] = status_reg | 0x04;
    data[1] = config_reg & 0x40;

    /* Write status reg */
    HAL_GPIO_WritePin(hnor->init.cs_gpio_port, hnor->init.cs_gpio_pin, GPIO_PIN_LOW);
    cmd    = NOR_CMD_WRITE_STATUS_REG;
    status = HAL_SPI_Transmit(hspi, &cmd, 1, new_timeout);
    if (status != HAL_OK)
    {
        HAL_FLAG_WRITE(hnor->error_code, hspi->error_code);
        goto error;
    }

    status = HAL_SPI_Transmit(hspi, data, 2, new_timeout);
    if (status != HAL_OK)
    {
        HAL_FLAG_WRITE(hnor->error_code, hspi->error_code);
        goto error;
    }
    HAL_GPIO_WritePin(hnor->init.cs_gpio_port, hnor->init.cs_gpio_pin, GPIO_PIN_HIGH);
    /* end 'Write status reg' */

    /* Wait WIP bit ready */
    status = _wait_wip_ready(hnor, NOR_CMD_WAIT_TIME);
    if (status != HAL_OK)
    {
        goto error;
    }

    /* Read status reg */
    HAL_GPIO_WritePin(hnor->init.cs_gpio_port, hnor->init.cs_gpio_pin, GPIO_PIN_LOW);
    cmd    = NOR_CMD_READ_STATUS_REG;
    status = HAL_SPI_Transmit(hspi, &cmd, 1, new_timeout);
    if (status != HAL_OK)
    {
        HAL_FLAG_WRITE(hnor->error_code, hspi->error_code);
        goto error;
    }
    status_reg = 0;
    status     = HAL_SPI_Receive(hspi, &status_reg, 1, new_timeout);
    if (status != HAL_OK)
    {
        HAL_FLAG_WRITE(hnor->error_code, hspi->error_code);
        goto error;
    }
    status = _check_p_e_success(hnor, NOR_CMD_WAIT_TIME);
    if (status != HAL_OK)
    {
        __NOP();
    }
    HAL_GPIO_WritePin(hnor->init.cs_gpio_port, hnor->init.cs_gpio_pin, GPIO_PIN_HIGH);
    /* end 'Read status reg' */

    if ((data[0] & 0xFC) != (status_reg & 0xFC)) // do not need compare wel bit and wip bit
        goto error;

error:
    if ((status == HAL_ERROR) || (status == HAL_TIMEOUT))
    {
        /* pull high chip select pin */
        HAL_GPIO_WritePin(hnor->init.cs_gpio_port, hnor->init.cs_gpio_pin, GPIO_PIN_HIGH);
    }

    /* set SPI FLASH state to ready after receive */
    HAL_FLAG_WRITE(hnor->state, HAL_STATE_READY);
    HAL_FLAG_WRITE(hnor->flash_state, NOR_FLASH_STATE_IDLE);

    /* process unlocked */
    GIVE_MUTEX(hnor);

    return status;
}

/**
 * @brief Flash enters DPD
 * @note  Use RDP instruction (or reboot flash) to exit DPD
 * @param hnor NOR FLASH handle
 * @retval HAL_Status_T
 */
HAL_Status_T MX25L6433F_EnterDPD(NORFlash_Handle_T *hnor)
{
    /* check parameters */
    ASSERT_PARAM(hnor);

    /* check the NOR FLASH controller state */
    RET_FLAG_TRUE(HAL_FLAG_LACKS(hnor->state, HAL_STATE_READY), HAL_ERROR);
    RET_FLAG_TRUE(HAL_FLAG_HAS(hnor->state, HAL_STATE_BUSY), HAL_BUSY);
    RET_FLAG_TRUE(HAL_FLAG_HAS(hnor->flash_state, NOR_FLASH_STATE_DPD), HAL_ERROR);

    SPI_Handle_T *hspi       = &spi_master_handle;
    uint32_t      tick_start = HAL_GetTick();
    uint8_t       cmd;
    HAL_Status_T  status = HAL_OK;

    /* process locked */
    TAKE_MUTEX(hnor);

    /* change NOR FLASH state */
    HAL_FLAG_WRITE(hnor->state, HAL_STATE_BUSY_TX);
    HAL_FLAG_WRITE(hnor->flash_state, NOR_FLASH_STATE_ERASE);

    /* clear error code */
    CLEAR_ERROR_CODE(hnor);

    /* pull low chip select pin */
    HAL_GPIO_WritePin(hnor->init.cs_gpio_port, hnor->init.cs_gpio_pin, GPIO_PIN_LOW);
    /* send DP command */
    cmd    = NOR_CMD_ENTER_DPD;
    status = HAL_SPI_Transmit(hspi, &cmd, 1, NOR_CMD_WAIT_TIME);
    if (status != HAL_OK)
    {
        HAL_FLAG_WRITE(hnor->error_code, hspi->error_code);
        goto error;
    }

    /* pull high chip select pin */
    HAL_GPIO_WritePin(hnor->init.cs_gpio_port, hnor->init.cs_gpio_pin, GPIO_PIN_HIGH);

    /* DP instruction must wait 10us for #cs high */
    HAL_Delay(1);

error:
    if ((status == HAL_ERROR) || (status == HAL_TIMEOUT))
    {
        /* pull high chip select pin */
        HAL_GPIO_WritePin(hnor->init.cs_gpio_port, hnor->init.cs_gpio_pin, GPIO_PIN_HIGH);
    }

    /* set SPI FLASH state to ready after receive */
    HAL_FLAG_WRITE(hnor->state, HAL_STATE_READY);
    HAL_FLAG_WRITE(hnor->flash_state, NOR_FLASH_STATE_DPD);

    /* process unlocked */
    GIVE_MUTEX(hnor);

    return status;
}

/**
 * @brief Flash enters DPD
 * @note  Use RDP instruction (or reboot flash) to exit DPD
 * @param hnor NOR FLASH handle
 * @retval HAL_Status_T
 */
HAL_Status_T MX25L6433F_ExitDPD(NORFlash_Handle_T *hnor)
{
    /* check parameters */
    ASSERT_PARAM(hnor);

    SPI_Handle_T *hspi       = &spi_master_handle;
    uint32_t      tick_start = HAL_GetTick();
    uint8_t       data[4]    = {0};
    HAL_Status_T  status     = HAL_OK;

    /* check the NOR FLASH controller state */
    RET_FLAG_TRUE(HAL_FLAG_LACKS(hnor->state, HAL_STATE_READY), HAL_ERROR);
    RET_FLAG_TRUE(HAL_FLAG_HAS(hnor->state, HAL_STATE_BUSY), HAL_BUSY);
    RET_FLAG_TRUE(HAL_FLAG_HAS(hnor->flash_state, NOR_FLASH_STATE_IDLE), HAL_OK);

    /* process locked */
    TAKE_MUTEX(hnor);

    /* change NOR FLASH state */
    HAL_FLAG_WRITE(hnor->state, HAL_STATE_BUSY_TX);
    HAL_FLAG_WRITE(hnor->flash_state, NOR_FLASH_STATE_ERASE);

    /* clear error code */
    CLEAR_ERROR_CODE(hnor);

    /* pull low chip select pin */
    HAL_GPIO_WritePin(hnor->init.cs_gpio_port, hnor->init.cs_gpio_pin, GPIO_PIN_LOW);
    /* send RDP command */
    data[0] = NOR_CMD_EXIT_DPD;
    status  = HAL_SPI_Transmit(hspi, data, 4, NOR_CMD_WAIT_TIME);
    if (status != HAL_OK)
    {
        HAL_FLAG_WRITE(hnor->error_code, hspi->error_code);
        goto error;
    }
    /* Flash will output 1 byte ID, here just ignoring it */
    HAL_SPI_Receive(hspi, data, 1, NOR_CMD_WAIT_TIME);

    /* pull high chip select pin */
    HAL_GPIO_WritePin(hnor->init.cs_gpio_port, hnor->init.cs_gpio_pin, GPIO_PIN_HIGH);

    /* RDP instruction must wait 100us for #cs high */
    HAL_Delay(1);

error:
    if ((status == HAL_ERROR) || (status == HAL_TIMEOUT))
    {
        /* pull high chip select pin */
        HAL_GPIO_WritePin(hnor->init.cs_gpio_port, hnor->init.cs_gpio_pin, GPIO_PIN_HIGH);
    }

    /* set SPI FLASH state to ready after receive */
    HAL_FLAG_WRITE(hnor->state, HAL_STATE_READY);
    HAL_FLAG_WRITE(hnor->flash_state, NOR_FLASH_STATE_IDLE);

    /* process unlocked */
    GIVE_MUTEX(hnor);

    return status;
}

/**
 * @brief Read flash ID
 * @param hnor NOR FLASH handle
 * @retval HAL_Status_T
 */
uint32_t MX25L6433F_ReadID(NORFlash_Handle_T *hnor)
{
    /* check parameters */
    ASSERT_PARAM(hnor);

    SPI_Handle_T *hspi       = &spi_master_handle;
    uint32_t      tick_start = HAL_GetTick();
    uint8_t       data[4]    = {0};
    HAL_Status_T  status     = HAL_OK;

    /* check the NOR FLASH controller state */
    RET_FLAG_TRUE(HAL_FLAG_LACKS(hnor->state, HAL_STATE_READY), HAL_ERROR);
    RET_FLAG_TRUE(HAL_FLAG_HAS(hnor->state, HAL_STATE_BUSY), HAL_BUSY);

    /* process locked */
    TAKE_MUTEX(hnor);

    /* change NOR FLASH state */
    HAL_FLAG_WRITE(hnor->state, HAL_STATE_BUSY_TX);

    /* clear error code */
    CLEAR_ERROR_CODE(hnor);

    /* pull low chip select pin */
    HAL_GPIO_WritePin(hnor->init.cs_gpio_port, hnor->init.cs_gpio_pin, GPIO_PIN_LOW);
    /* send RDP command */
    data[0] = NOR_CMD_READ_ID;
    status  = HAL_SPI_Transmit(hspi, data, 4, NOR_CMD_WAIT_TIME);
    if (status != HAL_OK)
    {
        HAL_FLAG_WRITE(hnor->error_code, hspi->error_code);
        goto error;
    }
    /* Flash will output 1 byte ID */
    status = HAL_SPI_Receive(hspi, data, 1, NOR_CMD_WAIT_TIME);
    if (status != HAL_OK)
    {
        HAL_FLAG_WRITE(hnor->error_code, hspi->error_code);
        goto error;
    }

    /* pull high chip select pin */
    HAL_GPIO_WritePin(hnor->init.cs_gpio_port, hnor->init.cs_gpio_pin, GPIO_PIN_HIGH);

    /* set NOR FLASH state to ready after erase */
    HAL_FLAG_WRITE(hnor->state, HAL_STATE_READY);

    /* process unlocked */
    GIVE_MUTEX(hnor);

    return (uint32_t)(data[0]);

error:
    if ((status == HAL_ERROR) || (status == HAL_TIMEOUT))
    {
        /* pull high chip select pin */
        HAL_GPIO_WritePin(hnor->init.cs_gpio_port, hnor->init.cs_gpio_pin, GPIO_PIN_HIGH);
    }

    /* set NOR FLASH state to ready after erase */
    HAL_FLAG_WRITE(hnor->state, HAL_STATE_READY);

    /* process unlocked */
    GIVE_MUTEX(hnor);

    return 0;
}

/**
 * @brief Erase a NOR FLASH sector, one sector size is 4 kilobyte(KB)
 * @param hnor NOR FLASH handle
 * @param start_addr start physical SPI FLASH address
 * @param timeout timeout duration
 * @retval HAL_Status_T
 */
HAL_Status_T MX25L6433F_EraseSector(NORFlash_Handle_T *hnor, uint32_t start_addr, uint32_t timeout)
{
    /* check parameters */
    ASSERT_PARAM(hnor);
    ASSERT_PARAM(start_addr <= NOR_ADDRESS_END);

    /* check the NOR FLASH controller state */
    RET_FLAG_TRUE(HAL_FLAG_LACKS(hnor->state, HAL_STATE_READY), HAL_ERROR);
    RET_FLAG_TRUE(HAL_FLAG_HAS(hnor->state, HAL_STATE_BUSY), HAL_BUSY);
    RET_FLAG_TRUE(HAL_FLAG_HAS(hnor->flash_state, NOR_FLASH_STATE_DPD), HAL_ERROR);

    SPI_Handle_T *hspi        = &spi_master_handle;
    uint32_t      tick_start  = HAL_GetTick();
    uint32_t      new_timeout = timeout;
    uint8_t       data[4];
    uint8_t       cmd;
    HAL_Status_T  status = HAL_OK;

    /* process locked */
    TAKE_MUTEX(hnor);

    /* change NOR FLASH state */
    HAL_FLAG_WRITE(hnor->state, HAL_STATE_BUSY_TX);
    HAL_FLAG_WRITE(hnor->flash_state, NOR_FLASH_STATE_ERASE);

    /* clear error code */
    CLEAR_ERROR_CODE(hnor);

    /* Enable WEL bit for transmission preparation*/
    status = _wren_and_wait_wel_ready(hnor, new_timeout);
    if (status != HAL_OK)
    {
        goto error;
    }

    /* refresh timeout */
    status = NORFlash_Get_NewTimeout(&tick_start, new_timeout, &new_timeout);
    if (status != HAL_OK)
    {
        HAL_FLAG_WRITE(hnor->error_code, NOR_ERROR_TIMEOUT);
        goto error;
    }
    /* pull low chip select pin */
    HAL_GPIO_WritePin(hnor->init.cs_gpio_port, hnor->init.cs_gpio_pin, GPIO_PIN_LOW);
    /* send erase sector command */
    cmd     = NOR_CMD_ERASE_SECTOR;
    data[0] = cmd;
    data[1] = start_addr >> 16;
    data[2] = start_addr >> 8;
    data[3] = start_addr;
    status  = HAL_SPI_Transmit(hspi, data, 4, new_timeout);
    if (status != HAL_OK)
    {
        HAL_FLAG_WRITE(hnor->error_code, hspi->error_code);
        goto error;
    }
    /* pull high chip select pin */
    HAL_GPIO_WritePin(hnor->init.cs_gpio_port, hnor->init.cs_gpio_pin, GPIO_PIN_HIGH);

    status = _wait_wip_ready(hnor, new_timeout);
    if (status != HAL_OK)
    {
        goto error;
    }

    /* refresh timeout */
    status = NORFlash_Get_NewTimeout(&tick_start, new_timeout, &new_timeout);
    if (status != HAL_OK)
    {
        HAL_FLAG_WRITE(hnor->error_code, NOR_ERROR_TIMEOUT);
        goto error;
    }
    /* pull low chip select pin */
    HAL_GPIO_WritePin(hnor->init.cs_gpio_port, hnor->init.cs_gpio_pin, GPIO_PIN_LOW);
    /* send read security register command */
    cmd    = NOR_CMD_READ_SECURITY_REG;
    status = HAL_SPI_Transmit(hspi, &cmd, 1, new_timeout);
    if (status != HAL_OK)
    {
        HAL_FLAG_WRITE(hnor->error_code, hspi->error_code);
        goto error;
    }

    /* refresh timeout */
    status = NORFlash_Get_NewTimeout(&tick_start, new_timeout, &new_timeout);
    if (status != HAL_OK)
    {
        HAL_FLAG_WRITE(hnor->error_code, NOR_ERROR_TIMEOUT);
        goto error;
    }
    /* read security data */
    status = HAL_SPI_Receive(hspi, data, 1, new_timeout);
    if (status != HAL_OK)
    {
        HAL_FLAG_WRITE(hnor->error_code, hspi->error_code);
        goto error;
    }
    /* pull high chip select pin */
    HAL_GPIO_WritePin(hnor->init.cs_gpio_port, hnor->init.cs_gpio_pin, GPIO_PIN_HIGH);

error:
    if ((status == HAL_ERROR) || (status == HAL_TIMEOUT))
    {
        /* pull high chip select pin */
        HAL_GPIO_WritePin(hnor->init.cs_gpio_port, hnor->init.cs_gpio_pin, GPIO_PIN_HIGH);
    }

    if (status == HAL_OK)
    {
        if (data[0] & NOR_SECURITY_REG_BIT_E_FAIL)
        {
            status = HAL_ERROR;
        }
    }

    /* set NOR FLASH state to ready after erase */
    HAL_FLAG_WRITE(hnor->state, HAL_STATE_READY);
    HAL_FLAG_WRITE(hnor->flash_state, NOR_FLASH_STATE_IDLE);

    /* process unlocked */
    GIVE_MUTEX(hnor);

    return status;
}

/**
 * @brief Erase a SPI FLASH block, one block size is 32 kilobyte(KB)
 * @param hnor SPI FLASH handle
 * @param start_addr start physical SPI FLASH address
 * @param timeout timeout duration
 * @retval HAL_Status_T
 */
HAL_Status_T MX25L6433F_EraseBlock32(NORFlash_Handle_T *hnor, uint32_t start_addr, uint32_t timeout)
{
    /* check parameters */
    ASSERT_PARAM(hnor);
    ASSERT_PARAM(start_addr <= NOR_ADDRESS_END);

    /* check the SPI FLASH controller state */
    RET_FLAG_TRUE(HAL_FLAG_LACKS(hnor->state, HAL_STATE_READY), HAL_ERROR);
    RET_FLAG_TRUE(HAL_FLAG_HAS(hnor->state, HAL_STATE_BUSY), HAL_BUSY);
    RET_FLAG_TRUE(HAL_FLAG_HAS(hnor->flash_state, NOR_FLASH_STATE_DPD), HAL_ERROR);

    SPI_Handle_T *hspi        = &spi_master_handle;
    uint32_t      tick_start  = HAL_GetTick();
    uint32_t      new_timeout = timeout;
    uint8_t       data[4];
    uint8_t       cmd;
    HAL_Status_T  status = HAL_OK;

    /* process locked */
    TAKE_MUTEX(hnor);

    /* change SPI FLASH state */
    HAL_FLAG_WRITE(hnor->state, HAL_STATE_BUSY_TX);
    HAL_FLAG_WRITE(hnor->flash_state, NOR_FLASH_STATE_ERASE);

    /* clear error code */
    CLEAR_ERROR_CODE(hnor);

    /* Enable WEL bit for transmission preparation*/
    status = _wren_and_wait_wel_ready(hnor, new_timeout);
    if (status != HAL_OK)
    {
        goto error;
    }

    /* refresh timeout */
    status = NORFlash_Get_NewTimeout(&tick_start, new_timeout, &new_timeout);
    if (status != HAL_OK)
    {
        HAL_FLAG_WRITE(hnor->error_code, NOR_ERROR_TIMEOUT);
        goto error;
    }
    /* pull low chip select pin */
    HAL_GPIO_WritePin(hnor->init.cs_gpio_port, hnor->init.cs_gpio_pin, GPIO_PIN_LOW);
    /* send erase sector command */
    cmd     = NOR_CMD_ERASE_BLOCK32;
    data[0] = cmd;
    data[1] = start_addr >> 16;
    data[2] = start_addr >> 8;
    data[3] = start_addr;
    status  = HAL_SPI_Transmit(hspi, data, 4, new_timeout);
    if (status != HAL_OK)
    {
        HAL_FLAG_WRITE(hnor->error_code, hspi->error_code);
        goto error;
    }
    /* pull high chip select pin */
    HAL_GPIO_WritePin(hnor->init.cs_gpio_port, hnor->init.cs_gpio_pin, GPIO_PIN_HIGH);

    status = _wait_wip_ready(hnor, new_timeout);
    if (status != HAL_OK)
    {
        goto error;
    }

    /* refresh timeout */
    status = NORFlash_Get_NewTimeout(&tick_start, new_timeout, &new_timeout);
    if (status != HAL_OK)
    {
        HAL_FLAG_WRITE(hnor->error_code, NOR_ERROR_TIMEOUT);
        goto error;
    }
    /* pull low chip select pin */
    HAL_GPIO_WritePin(hnor->init.cs_gpio_port, hnor->init.cs_gpio_pin, GPIO_PIN_LOW);
    /* send read security register command */
    cmd    = NOR_CMD_READ_SECURITY_REG;
    status = HAL_SPI_Transmit(hspi, &cmd, 1, new_timeout);
    if (status != HAL_OK)
    {
        HAL_FLAG_WRITE(hnor->error_code, hspi->error_code);
        goto error;
    }

    /* refresh timeout */
    status = NORFlash_Get_NewTimeout(&tick_start, new_timeout, &new_timeout);
    if (status != HAL_OK)
    {
        HAL_FLAG_WRITE(hnor->error_code, NOR_ERROR_TIMEOUT);
        goto error;
    }
    /* read security data */
    status = HAL_SPI_Receive(hspi, data, 1, new_timeout);
    if (status != HAL_OK)
    {
        HAL_FLAG_WRITE(hnor->error_code, hspi->error_code);
        goto error;
    }
    /* pull high chip select pin */
    HAL_GPIO_WritePin(hnor->init.cs_gpio_port, hnor->init.cs_gpio_pin, GPIO_PIN_HIGH);

error:
    if ((status == HAL_ERROR) || (status == HAL_TIMEOUT))
    {
        /* pull high chip select pin */
        HAL_GPIO_WritePin(hnor->init.cs_gpio_port, hnor->init.cs_gpio_pin, GPIO_PIN_HIGH);
    }

    if (status == HAL_OK)
    {
        if (data[0] & NOR_SECURITY_REG_BIT_E_FAIL)
        {
            status = HAL_ERROR;
        }
    }

    /* set SPI FLASH state to ready after erase */
    HAL_FLAG_WRITE(hnor->state, HAL_STATE_READY);
    HAL_FLAG_WRITE(hnor->flash_state, NOR_FLASH_STATE_IDLE);

    /* process unlocked */
    GIVE_MUTEX(hnor);

    return status;
}

/**
 * @brief Erase a SPI FLASH block, one block size is 64 kilobyte(KB)
 * @param hnor SPI FLASH handle
 * @param start_addr start physical SPI FLASH address
 * @param timeout timeout duration
 * @retval HAL_Status_T
 */
HAL_Status_T MX25L6433F_EraseBlock64(NORFlash_Handle_T *hnor, uint32_t start_addr, uint32_t timeout)
{
    /* check parameters */
    ASSERT_PARAM(hnor);
    ASSERT_PARAM(start_addr <= NOR_ADDRESS_END);

    /* check the SPI FLASH controller state */
    RET_FLAG_TRUE(HAL_FLAG_LACKS(hnor->state, HAL_STATE_READY), HAL_ERROR);
    RET_FLAG_TRUE(HAL_FLAG_HAS(hnor->state, HAL_STATE_BUSY), HAL_BUSY);
    RET_FLAG_TRUE(HAL_FLAG_HAS(hnor->flash_state, NOR_FLASH_STATE_DPD), HAL_ERROR);

    SPI_Handle_T *hspi        = &spi_master_handle;
    uint32_t      tick_start  = HAL_GetTick();
    uint32_t      new_timeout = timeout;
    uint8_t       data[4];
    uint8_t       cmd;
    HAL_Status_T  status = HAL_OK;

    /* process locked */
    TAKE_MUTEX(hnor);

    /* change SPI FLASH state */
    HAL_FLAG_WRITE(hnor->state, HAL_STATE_BUSY_TX);
    HAL_FLAG_WRITE(hnor->flash_state, NOR_FLASH_STATE_ERASE);

    /* clear error code */
    CLEAR_ERROR_CODE(hnor);

    /* Enable WEL bit for transmission preparation*/
    status = _wren_and_wait_wel_ready(hnor, new_timeout);
    if (status != HAL_OK)
    {
        goto error;
    }

    /* refresh timeout */
    status = NORFlash_Get_NewTimeout(&tick_start, new_timeout, &new_timeout);
    if (status != HAL_OK)
    {
        HAL_FLAG_WRITE(hnor->error_code, NOR_ERROR_TIMEOUT);
        goto error;
    }
    /* pull low chip select pin */
    HAL_GPIO_WritePin(hnor->init.cs_gpio_port, hnor->init.cs_gpio_pin, GPIO_PIN_LOW);
    /* send erase sector command */
    cmd     = NOR_CMD_ERASE_BLOCK64;
    data[0] = cmd;
    data[1] = start_addr >> 16;
    data[2] = start_addr >> 8;
    data[3] = start_addr;
    status  = HAL_SPI_Transmit(hspi, data, 4, new_timeout);
    if (status != HAL_OK)
    {
        HAL_FLAG_WRITE(hnor->error_code, hspi->error_code);
        goto error;
    }
    /* pull high chip select pin */
    HAL_GPIO_WritePin(hnor->init.cs_gpio_port, hnor->init.cs_gpio_pin, GPIO_PIN_HIGH);

    status = _wait_wip_ready(hnor, new_timeout);
    if (status != HAL_OK)
    {
        goto error;
    }

    /* refresh timeout */
    status = NORFlash_Get_NewTimeout(&tick_start, new_timeout, &new_timeout);
    if (status != HAL_OK)
    {
        HAL_FLAG_WRITE(hnor->error_code, NOR_ERROR_TIMEOUT);
        goto error;
    }
    /* pull low chip select pin */
    HAL_GPIO_WritePin(hnor->init.cs_gpio_port, hnor->init.cs_gpio_pin, GPIO_PIN_LOW);
    /* send read security register command */
    cmd    = NOR_CMD_READ_SECURITY_REG;
    status = HAL_SPI_Transmit(hspi, &cmd, 1, new_timeout);
    if (status != HAL_OK)
    {
        HAL_FLAG_WRITE(hnor->error_code, hspi->error_code);
        goto error;
    }

    /* refresh timeout */
    status = NORFlash_Get_NewTimeout(&tick_start, new_timeout, &new_timeout);
    if (status != HAL_OK)
    {
        HAL_FLAG_WRITE(hnor->error_code, NOR_ERROR_TIMEOUT);
        goto error;
    }
    /* read security data */
    status = HAL_SPI_Receive(hspi, data, 1, new_timeout);
    if (status != HAL_OK)
    {
        HAL_FLAG_WRITE(hnor->error_code, hspi->error_code);
        goto error;
    }
    /* pull high chip select pin */
    HAL_GPIO_WritePin(hnor->init.cs_gpio_port, hnor->init.cs_gpio_pin, GPIO_PIN_HIGH);

error:
    if ((status == HAL_ERROR) || (status == HAL_TIMEOUT))
    {
        /* pull high chip select pin */
        HAL_GPIO_WritePin(hnor->init.cs_gpio_port, hnor->init.cs_gpio_pin, GPIO_PIN_HIGH);
    }

    if (status == HAL_OK)
    {
        if (data[0] & NOR_SECURITY_REG_BIT_E_FAIL)
        {
            status = HAL_ERROR;
        }
    }

    /* set SPI FLASH state to ready after erase */
    HAL_FLAG_WRITE(hnor->state, HAL_STATE_READY);
    HAL_FLAG_WRITE(hnor->flash_state, NOR_FLASH_STATE_IDLE);

    /* process unlocked */
    GIVE_MUTEX(hnor);

    return status;
}

/**
 * @brief Erase all SPI FLASH memory
 * @param hnor NOR FLASH handle
 * @param timeout timeout duration
 * @retval HAL_Status_T
 */
HAL_Status_T MX25L6433F_EraseChip(NORFlash_Handle_T *hnor, uint32_t timeout)
{
    /* check parameters */
    ASSERT_PARAM(hnor);

    /* check the SPI FLASH controller state */
    RET_FLAG_TRUE(HAL_FLAG_LACKS(hnor->state, HAL_STATE_READY), HAL_ERROR);
    RET_FLAG_TRUE(HAL_FLAG_HAS(hnor->state, HAL_STATE_BUSY), HAL_BUSY);
    RET_FLAG_TRUE(HAL_FLAG_HAS(hnor->flash_state, NOR_FLASH_STATE_DPD), HAL_ERROR);

    SPI_Handle_T *hspi        = &spi_master_handle;
    uint32_t      tick_start  = HAL_GetTick();
    uint32_t      new_timeout = timeout;
    uint8_t       data;
    uint8_t       cmd;
    HAL_Status_T  status = HAL_OK;

    /* process locked */
    TAKE_MUTEX(hnor);

    /* change SPI FLASH state */
    HAL_FLAG_WRITE(hnor->state, HAL_STATE_BUSY_TX);
    HAL_FLAG_WRITE(hnor->flash_state, NOR_FLASH_STATE_ERASE);

    /* clear error code */
    CLEAR_ERROR_CODE(hnor);

    /* Enable WEL bit for transmission preparation*/
    status = _wren_and_wait_wel_ready(hnor, new_timeout);
    if (status != HAL_OK)
    {
        goto error;
    }

    /* refresh timeout */
    status = NORFlash_Get_NewTimeout(&tick_start, new_timeout, &new_timeout);
    if (status != HAL_OK)
    {
        HAL_FLAG_WRITE(hnor->error_code, NOR_ERROR_TIMEOUT);
        goto error;
    }
    /* pull low chip select pin */
    HAL_GPIO_WritePin(hnor->init.cs_gpio_port, hnor->init.cs_gpio_pin, GPIO_PIN_LOW);
    /* send erase sector command */
    cmd    = NOR_CMD_ERASE_CHIP;
    status = HAL_SPI_Transmit(hspi, &cmd, 1, new_timeout);
    if (status != HAL_OK)
    {
        HAL_FLAG_WRITE(hnor->error_code, hspi->error_code);
        goto error;
    }
    /* pull high chip select pin */
    HAL_GPIO_WritePin(hnor->init.cs_gpio_port, hnor->init.cs_gpio_pin, GPIO_PIN_HIGH);

    status = _wait_wip_ready(hnor, new_timeout);
    if (status != HAL_OK)
    {
        goto error;
    }

    /* refresh timeout */
    status = NORFlash_Get_NewTimeout(&tick_start, new_timeout, &new_timeout);
    if (status != HAL_OK)
    {
        HAL_FLAG_WRITE(hnor->error_code, NOR_ERROR_TIMEOUT);
        goto error;
    }
    /* pull low chip select pin */
    HAL_GPIO_WritePin(hnor->init.cs_gpio_port, hnor->init.cs_gpio_pin, GPIO_PIN_LOW);
    /* send read security register command */
    cmd    = NOR_CMD_READ_SECURITY_REG;
    status = HAL_SPI_Transmit(hspi, &cmd, 1, new_timeout);
    if (status != HAL_OK)
    {
        HAL_FLAG_WRITE(hnor->error_code, hspi->error_code);
        goto error;
    }

    /* refresh timeout */
    status = NORFlash_Get_NewTimeout(&tick_start, new_timeout, &new_timeout);
    if (status != HAL_OK)
    {
        HAL_FLAG_WRITE(hnor->error_code, NOR_ERROR_TIMEOUT);
        goto error;
    }
    /* read security data */
    status = HAL_SPI_Receive(hspi, &data, 1, new_timeout);
    if (status != HAL_OK)
    {
        HAL_FLAG_WRITE(hnor->error_code, hspi->error_code);
        goto error;
    }
    /* pull high chip select pin */
    HAL_GPIO_WritePin(hnor->init.cs_gpio_port, hnor->init.cs_gpio_pin, GPIO_PIN_HIGH);

error:
    if ((status == HAL_ERROR) || (status == HAL_TIMEOUT))
    {
        /* pull high chip select pin */
        HAL_GPIO_WritePin(hnor->init.cs_gpio_port, hnor->init.cs_gpio_pin, GPIO_PIN_HIGH);
    }

    if (status == HAL_OK)
    {
        if (data & NOR_SECURITY_REG_BIT_E_FAIL)
        {
            status = HAL_ERROR;
        }
    }

    /* set SPI FLASH state to ready after erase */
    HAL_FLAG_WRITE(hnor->state, HAL_STATE_READY);
    HAL_FLAG_WRITE(hnor->flash_state, NOR_FLASH_STATE_IDLE);

    /* process unlocked */
    GIVE_MUTEX(hnor);

    return status;
}

/**
 * @brief Get the FLASH Capacity
 * @param capacity NORFlash_Capacity_T handle
 * @retval HAL_Status_T
 */
HAL_Status_T MX25L6433F_GetCapacity(NORFlash_Capacity_T *capacity)
{
    capacity->norflash_page_size    = NOR_PAGE_SIZE;
    capacity->norflash_sector_size  = NOR_SECTOR_SIZE;
    capacity->norflash_block32_size = NOR_BLOCK32_SIZE;
    capacity->norflash_block64_size = NOR_BLOCK64_SIZE;
    capacity->norflash_flash_size   = NOR_MEM_SIZE;
    return HAL_OK;
}

/**
 * @brief Return the SPI FLASH state
 * @param hnor SPI FLASH handle
 * @retval SPI FLASH state
 */
HAL_State_T MX25L6433F_GetState(NORFlash_Handle_T *hnor)
{
    ASSERT_PARAM(hnor);
    return hnor->state;
}

/**
 * @brief Return the SPI FLASH error code
 * @param hnor SPI FLASH handle
 * @retval SPI FLASH error code
 */
uint32_t MX25L6433F_GetError(NORFlash_Handle_T *hnor)
{
    ASSERT_PARAM(hnor);
    return hnor->error_code;
}

/**
 * @brief  Abort ongoing transfer (blocking mode).
 * @param  hnor NOR Flash handle.
 * @note   This procedure could be used for aborting any ongoing transfer (Tx and Rx),
 *         started in Interrupt or DMA mode.
 *         This procedure performs following operations :
 *           - Disable NOR Flash Interrupts (depending of transfer direction)
 *           - Disable the DMA transfer in the peripheral register (if enabled)
 *           - Abort DMA transfer by calling HAL_DMA_Abort (in case of transfer in DMA mode)
 *           - Set handle State to READY
 * @note   This procedure is executed in blocking mode : when exiting function, Abort is considered as completed.
 * @retval HAL status
 */
HAL_Status_T MX25L6433F_Abort(NORFlash_Handle_T *hnor)
{
    /* Check the valid of NOR Flash handle */
    ASSERT_PARAM(hnor);

    /* It will return error if you call the abort function in NOR Flash idle time or DPD mode*/
    RET_FLAG_TRUE(HAL_FLAG_HAS(hnor->state, HAL_STATE_READY), HAL_ERROR);
    RET_FLAG_TRUE(HAL_FLAG_HAS(hnor->flash_state, NOR_FLASH_STATE_DPD), HAL_ERROR);

    SPI_Handle_T *hspi   = &spi_master_handle;
    HAL_Status_T  status = HAL_OK;

    /* Process locked */
    TAKE_MUTEX(hnor);

    /* Pull high cs to abort transmission */
    HAL_GPIO_WritePin(hnor->init.cs_gpio_port, hnor->init.cs_gpio_pin, GPIO_PIN_HIGH);

#if (configUSE_NORFLASH_NONBLOCKING_DMA == 1U)
    if (HAL_FLAG_HAS(hnor->state, HAL_STATE_BUSY_TX))
    {
        hnor->tx.xfer_count = 0;
        NOR_DISABLE_DMA(hnor, NOR_SPI_TFDMAEN);
        NOR_DISABLE_TX_THOD(hnor);
        NOR_DISABLE_SPI_TX(hnor);
        status = HAL_DMA_Abort(hnor->hdmatx);
        if (status != HAL_OK)
        {
            HAL_FLAG_WRITE(hnor->state, HAL_STATE_READY);
            HAL_FLAG_WRITE(hnor->error_code, NOR_ERROR_DMA);
            HAL_FLAG_WRITE(hspi->state, HAL_STATE_READY);
            HAL_FLAG_WRITE(hspi->error_code, HAL_SPI_ERROR_DMA);
            return status;
        }
    }
    else if (HAL_FLAG_HAS(hnor->state, HAL_STATE_BUSY_RX))
    {
        hspi->rx.xfer_count = 0;
        NOR_DISABLE_DMA(hnor, NOR_SPI_RFDMAEN);
        NOR_DISABLE_RX_THOD(hnor);
        NOR_DISABLE_SPI_RX(hnor);
        status = HAL_DMA_Abort(hnor->hdmarx);
        if (status != HAL_OK)
        {
            HAL_FLAG_WRITE(hnor->state, HAL_STATE_READY);
            HAL_FLAG_WRITE(hnor->error_code, NOR_ERROR_DMA);
            HAL_FLAG_WRITE(hspi->state, HAL_STATE_READY);
            HAL_FLAG_WRITE(hspi->error_code, HAL_SPI_ERROR_DMA);
            hspi->error_code = HAL_SPI_ERROR_DMA;
            return status;
        }
    }
#endif

    /* Check error during Abort procedure */
    if (HAL_FLAG_HAS(hnor->error_code, NOR_ERROR_ABORT))
    {
        /* return HAL_ERROR in case of error during Abort procedure */
        status = HAL_ERROR;
    }
    else
    {
        /* Reset errorCode */
        HAL_FLAG_WRITE(hnor->error_code, HAL_ERROR_NONE);
        HAL_FLAG_WRITE(hspi->error_code, HAL_ERROR_NONE);
    }

    /* Change SPI FLash peripheral state */
    HAL_FLAG_WRITE(hnor->state, HAL_STATE_READY);
    HAL_FLAG_WRITE(hnor->flash_state, NOR_FLASH_STATE_IDLE);
    HAL_FLAG_WRITE(hspi->state, HAL_STATE_READY);

    /* Free the mutex */
    GIVE_MUTEX(hnor);

    return status;
}

/* Private functions ---------------------------------------------------------*/

#if (configUSE_NORFLASH_NONBLOCKING_DMA == 1U)
/**
 * @brief Write one page completed callback
 * @note  DMA transmission proceeds here, and will call tx API over and over again
 *        until all data are transferred completely
 * @param hspi SPI handle
 * @retval None
 */
static void _norflash_dma_tx_writepage_cb(SPI_Handle_T *hspi)
{
    HAL_Status_T       status = HAL_OK;
    NORFlash_Handle_T *hnor   = nor_handle;
    uint8_t            data[4];
    uint8_t            cmd;
    uint32_t           transfer_count = 0;
    uint8_t           *curr_buffer    = hnor->tx.buffer;     // start data buffer of operation this time to tranfer
    uint32_t           curr_addr      = hnor->tx.nor_addr;   // start writing address of flash this time to tranfer
    uint32_t           curr_count     = hnor->tx.xfer_count; // remaining tx count this time to tranfer

    /* Pull high cs to finish transmission of last round */
    HAL_GPIO_WritePin(hnor->init.cs_gpio_port, hnor->init.cs_gpio_pin, GPIO_PIN_HIGH);

    /* Wait WIP bit ready */
    status = _wait_wip_ready(hnor, NOR_CMD_WAIT_TIME);
    if (status != HAL_OK)
    {
        return;
    }

    /* Check if write operation succeed last round*/
    status = _check_p_e_success(hnor, NOR_CMD_WAIT_TIME);
    if (status != HAL_OK)
    {
        return;
    }

    if (hnor->tx.xfer_count == 0)
    {
        HAL_FLAG_WRITE(hnor->state, HAL_STATE_READY);
        HAL_FLAG_WRITE(hnor->flash_state, NOR_FLASH_STATE_IDLE);
#if (configUSE_NORFLASH_REGISTER_CALLBACKS == 1U)
        if (hnor->TxCpltCallback)
            hnor->TxCpltCallback(hnor);
#else
        HAL_NORFlash_TxCpltCallback(hnor);
#endif
    }
    else
    {
        if (hnor->tx.xfer_count < NOR_PAGE_SIZE)
        {
            /* Final round */
            transfer_count = hnor->tx.xfer_count;
        }
        else
        {
            /* If not in final round, it will write flash measured in page*/
            transfer_count = NOR_PAGE_SIZE;
        }

        /* update transfer information for next cb */
        hnor->tx.buffer += transfer_count;
        hnor->tx.nor_addr += transfer_count;
        hnor->tx.xfer_count -= transfer_count;

        /* Enable WEL bit for transmission preparation*/
        status = _wren_and_wait_wel_ready(hnor, NOR_CMD_WAIT_TIME);
        if (status != HAL_OK)
        {
            return;
        }

        /* pull low chip select pin */
        HAL_GPIO_WritePin(hnor->init.cs_gpio_port, hnor->init.cs_gpio_pin, GPIO_PIN_LOW);
        /* send page program command */
        cmd     = NOR_CMD_PAGR_PROGRAM;
        data[0] = cmd;
        data[1] = curr_addr >> 16;
        data[2] = curr_addr >> 8;
        data[3] = curr_addr;
        status  = HAL_SPI_Transmit(hspi, data, 4, NOR_CMD_WAIT_TIME);
        if (status != HAL_OK)
        {
            HAL_FLAG_WRITE(hnor->error_code, hspi->error_code);
            return;
        }

        /* program data */
        status = HAL_SPI_Transmit_DMA(hspi, curr_buffer, transfer_count);
        if (status != HAL_OK)
        {
            HAL_FLAG_WRITE(hnor->error_code, hspi->error_code);
            return;
        }
    }
}

/**
 * @brief RX transfer completed callback
 * @param hspi SPI handle
 * @retval None
 */
static void _norflash_dma_rx_complete_cb(SPI_Handle_T *hspi)
{
    NORFlash_Handle_T *hnor = nor_handle;

    /* Wait WIP bit ready */
    if (_wait_wip_ready(hnor, NOR_CMD_WAIT_TIME) != HAL_OK)
    {
        return;
    }

    /* Check if write operation succeed last round*/
    if (_check_p_e_success(hnor, NOR_CMD_WAIT_TIME) != HAL_OK)
    {
        return;
    }
    HAL_GPIO_WritePin(hnor->init.cs_gpio_port, hnor->init.cs_gpio_pin, GPIO_PIN_HIGH);

    /* set SPI FLASH state to ready after receive */
    HAL_FLAG_WRITE(hnor->state, HAL_STATE_READY);
    HAL_FLAG_WRITE(hnor->flash_state, NOR_FLASH_STATE_IDLE);

#if (configUSE_NORFLASH_REGISTER_CALLBACKS == 1U)
    if (hnor->RxCpltCallback)
        hnor->RxCpltCallback(hnor);
#else
    HAL_NORFlash_RxCpltCallback(hnor);
#endif
}

#endif /* configUSE_NORFLASH_NONBLOCKING_DMA */

/**
 * @brief  Overwrite HAL_SPI_MspInit.
 * @retval Null
 */
void _norflash_spi_msp_init_callback(SPI_Handle_T *hspi)
{
    if (hspi->instance == SN_SPI0)
    {
        // 1 by HAL_GPIO_SetAFIO API setting current IP AFIO pins if need
        HAL_GPIO_SetAFIO(SN_GPIO3, GPIO_PIN_0, GPIO_P30_SPI_SCK0);
        HAL_GPIO_SetAFIO(SN_GPIO1, GPIO_PIN_7, GPIO_P17_SPI_SEL0);
        HAL_GPIO_SetAFIO(SN_GPIO1, GPIO_PIN_2, GPIO_P12_SPI_MO0);
        HAL_GPIO_SetAFIO(SN_GPIO1, GPIO_PIN_1, GPIO_P11_SPI_MI0);

        if (__HAL_RCC_SSP0_IS_CLK_DISABLE())
        {
            // 2 by SCU register enable current IP clock
            __HAL_RCC_SSP0_CLK_ENABLE();
            // 3 by SCU register resetting current IP register to default value
            __HAL_RCC_SSP0_RESET();
        }
    }

    // assign the DMA handle to master hdmatx before SPI initialize
#if (configUSE_NORFLASH_NONBLOCKING_DMA == 1U)
    // 4 init DMA

    nor_handle->hdmatx->instance            = SN_DMA1;
    nor_handle->hdmatx->init.channel        = DMA_CHANNEL_0;
    nor_handle->hdmatx->init.ch_xfer_mode   = DMA_MEMORY_TO_PERIPH;
    nor_handle->hdmatx->init.ch_priority    = DMA_PRIORITY_LOW;
    nor_handle->hdmatx->init.ch_fifo_th     = DMA_FIFO_TH_1;
    nor_handle->hdmatx->init.ch_sync        = HAL_ENABLE;
    nor_handle->hdmatx->init.src_burst      = DMA_SRC_BURST_1;
    nor_handle->hdmatx->init.src_width      = DMA_XFR_WIDTH_8BITS; // if spi.data_size <=8, set the parameter is DMA_XFR_WIDTH_8BITS otherwise is DMA_XFR_WIDTH_16BITS
    nor_handle->hdmatx->init.src_addr_mode  = DMA_ADDR_MODE_INC;   // if the value of ch_xfer_mode is DMA_MEMORY_TO_PERIPH, set the parameter is DMA_ADDR_MODE_INC
    nor_handle->hdmatx->init.src_ahb_master = DMA_AHB_MASTER_PERIPH;
    nor_handle->hdmatx->init.dst_rs         = DMA_RS1_SSP0_TX;
    nor_handle->hdmatx->init.dst_width      = DMA_XFR_WIDTH_8BITS;
    nor_handle->hdmatx->init.dst_addr_mode  = DMA_ADDR_MODE_FIXED; // if the value of ch_xfer_mode is DMA_MEMORY_TO_PERIPH, set the parameter is DMA_ADDR_MODE_FIXED
    nor_handle->hdmatx->init.dst_ahb_master = DMA_AHB_MASTER_PERIPH;

    nor_handle->hdmarx->instance            = SN_DMA1;
    nor_handle->hdmarx->init.channel        = DMA_CHANNEL_1;
    nor_handle->hdmarx->init.ch_xfer_mode   = DMA_PERIPH_TO_MEMORY;
    nor_handle->hdmarx->init.ch_priority    = DMA_PRIORITY_LOW;
    nor_handle->hdmarx->init.ch_fifo_th     = DMA_FIFO_TH_1;
    nor_handle->hdmarx->init.ch_sync        = HAL_ENABLE;
    nor_handle->hdmarx->init.src_burst      = DMA_SRC_BURST_1;
    nor_handle->hdmarx->init.src_rs         = DMA_RS1_SSP0_RX;
    nor_handle->hdmarx->init.src_width      = DMA_XFR_WIDTH_8BITS; // if spi.data_size <=8, set the parameter is DMA_XFR_WIDTH_8BITS otherwise is DMA_XFR_WIDTH_16BITS
    nor_handle->hdmarx->init.src_addr_mode  = DMA_ADDR_MODE_FIXED; // if the value of ch_xfer_mode is DMA_MEMORY_TO_PERIPH, set the parameter is DMA_ADDR_MODE_INC
    nor_handle->hdmarx->init.src_ahb_master = DMA_AHB_MASTER_PERIPH;
    nor_handle->hdmarx->init.dst_width      = DMA_XFR_WIDTH_8BITS;
    nor_handle->hdmarx->init.dst_addr_mode  = DMA_ADDR_MODE_INC; // if the value of ch_xfer_mode is DMA_MEMORY_TO_PERIPH, set the parameter is DMA_ADDR_MODE_FIXED
    nor_handle->hdmarx->init.dst_ahb_master = DMA_AHB_MASTER_PERIPH;

    /* Register DMA of SPI_FLASH Callback */
    HAL_DMA_RegisterCallback(hspi->hdmatx, HAL_DMA_MSP_INIT_CB_ID, _norflash_dma_msp_init_cb);
    HAL_DMA_RegisterCallback(hspi->hdmatx, HAL_DMA_MSP_DEINIT_CB_ID, _norflash_dma_msp_deinit_cb);

    /* Register DMA of SPI_FLASH Callback */
    HAL_DMA_RegisterCallback(hspi->hdmarx, HAL_DMA_MSP_INIT_CB_ID, _norflash_dma_msp_init_cb);
    HAL_DMA_RegisterCallback(hspi->hdmarx, HAL_DMA_MSP_DEINIT_CB_ID, _norflash_dma_msp_deinit_cb);

    HAL_DMA_Init(nor_handle->hdmatx);
    HAL_DMA_Init(nor_handle->hdmarx);

#endif /* configUSE_NORFLASH_NONBLOCKING_DMA */
}

/**
 * @brief  Overwrite HAL_SPI_MspDeInit.
 * @retval Null
 */
void _norflash_spi_msp_deinit_callback(SPI_Handle_T *hspi)
{
    if (hspi->instance == SN_SPI0)
    {
        // 1 by SCU register disable current IP clock
        if (__HAL_RCC_SSP0_IS_CLK_ENABLE())
        {
            __HAL_RCC_SSP0_CLK_DISABLE();
        }

        // 2 by HAL_GPIO_SetAFIO API setting current IP AFIO pins to GPIO
        HAL_GPIO_SetAFIO(SN_GPIO3, GPIO_PIN_0, GPIO_P30_GPIO);
        HAL_GPIO_SetAFIO(SN_GPIO1, GPIO_PIN_7, GPIO_P17_GPIO);
        HAL_GPIO_SetAFIO(SN_GPIO1, GPIO_PIN_2, GPIO_P12_GPIO);
        HAL_GPIO_SetAFIO(SN_GPIO1, GPIO_PIN_1, GPIO_P11_GPIO);
    }

#if (configUSE_NORFLASH_NONBLOCKING_DMA == 1U)
    // 3 deinit DMA
    HAL_DMA_DeInit(nor_handle->hdmatx);
    HAL_DMA_DeInit(nor_handle->hdmarx);
#endif /* configUSE_NORFLASH_NONBLOCKING_DMA */
}

#if (configUSE_NORFLASH_NONBLOCKING_DMA == 1U)
/**
 * @brief  Overwrite HAL_DMA_MspInit.
 * @retval Null
 */
void _norflash_dma_msp_init_cb(DMA_Handle_T *hdma)
{
    if (hdma->instance == SN_DMA1)
    {
        if (__HAL_RCC_DMA1_IS_CLK_DISABLE())
        {
            // 1 by SCU register enable current IP clock
            __HAL_RCC_DMA1_CLK_ENABLE();

            // 2 by SCU register enable current IP clock
            __HAL_RCC_DMA1_RESET();
        }

        // 3 nvic enable irq
        if (hdma->init.channel == DMA_CHANNEL_0)
        {
            NVIC_ClearPendingIRQ(DMA1_CH0_IRQn);
            NVIC_EnableIRQ(DMA1_CH0_IRQn);
        }
        else if (hdma->init.channel == DMA_CHANNEL_1)
        {
            NVIC_ClearPendingIRQ(DMA1_CH1_IRQn);
            NVIC_EnableIRQ(DMA1_CH1_IRQn);
        }
    }
}

/**
 * @brief  Overwrite HAL_DMA_MspInit.
 * @retval Null
 */
void _norflash_dma_msp_deinit_cb(DMA_Handle_T *hdma)
{
    if (hdma->instance == SN_DMA1)
    {
        // 1 get all enable status of current dma
        uint32_t all_ch_en = 0;
        HAL_DMA_GetChannelStatus(hdma, &all_ch_en);

        // 2 by SCU register disable current IP clock
        if (__HAL_RCC_DMA1_IS_CLK_ENABLE() && all_ch_en == 0)
        {
            __HAL_RCC_DMA1_CLK_DISABLE();
        }

        // 3 nvic disable irq
        if (hdma->init.channel == DMA_CHANNEL_0)
        {
            NVIC_ClearPendingIRQ(DMA1_CH0_IRQn);
            NVIC_DisableIRQ(DMA1_CH0_IRQn);
        }
        else if (hdma->init.channel == DMA_CHANNEL_1)
        {
            NVIC_ClearPendingIRQ(DMA1_CH1_IRQn);
            NVIC_DisableIRQ(DMA1_CH1_IRQn);
        }
    }
}
#endif /* configUSE_NORFLASH_NONBLOCKING_DMA */

/**
 * @brief wait WIP bit of status reg turning to 0
 * @param hnor NORFLASH handle
 * @param max tolerant time of programming/reading in one operation
 * @retval None
 */
static HAL_Status_T _wren_and_wait_wel_ready(NORFlash_Handle_T *hnor, uint32_t timeout)
{
    SPI_Handle_T *hspi        = &spi_master_handle;
    uint32_t      tick_start  = HAL_GetTick();
    uint32_t      new_timeout = timeout;
    uint8_t       data[4];
    uint8_t       cmd;
    HAL_Status_T  status = HAL_OK;
    while (1)
    {
        /* refresh timeout */
        status = NORFlash_Get_NewTimeout(&tick_start, new_timeout, &new_timeout);
        if (status != HAL_OK)
        {
            HAL_FLAG_WRITE(hnor->error_code, NOR_ERROR_TIMEOUT);
            return HAL_ERROR;
        }
        /* pull low chip select pin */
        HAL_GPIO_WritePin(hnor->init.cs_gpio_port, hnor->init.cs_gpio_pin, GPIO_PIN_LOW);
        /* send write enable command */
        cmd    = NOR_CMD_WRITE_ENABLE;
        status = HAL_SPI_Transmit(hspi, &cmd, 1, new_timeout);
        if (status != HAL_OK)
        {
            HAL_FLAG_WRITE(hnor->error_code, hspi->error_code);
            return HAL_ERROR;
        }
        /* pull high chip select pin */
        HAL_GPIO_WritePin(hnor->init.cs_gpio_port, hnor->init.cs_gpio_pin, GPIO_PIN_HIGH);

        /* refresh timeout */
        status = NORFlash_Get_NewTimeout(&tick_start, new_timeout, &new_timeout);
        if (status != HAL_OK)
        {
            HAL_FLAG_WRITE(hnor->error_code, NOR_ERROR_TIMEOUT);
            return HAL_ERROR;
        }
        /* pull low chip select pin */
        HAL_GPIO_WritePin(hnor->init.cs_gpio_port, hnor->init.cs_gpio_pin, GPIO_PIN_LOW);
        /* send read status register command */
        cmd    = NOR_CMD_READ_STATUS_REG;
        status = HAL_SPI_Transmit(hspi, &cmd, 1, new_timeout);
        if (status != HAL_OK)
        {
            HAL_FLAG_WRITE(hnor->error_code, hspi->error_code);
            return HAL_ERROR;
        }

        /* refresh timeout */
        status = NORFlash_Get_NewTimeout(&tick_start, new_timeout, &new_timeout);
        if (status != HAL_OK)
        {
            HAL_FLAG_WRITE(hnor->error_code, NOR_ERROR_TIMEOUT);
            return HAL_ERROR;
        }
        /* read status data */
        status = HAL_SPI_Receive(hspi, data, 1, new_timeout);
        if (status != HAL_OK)
        {
            HAL_FLAG_WRITE(hnor->error_code, hspi->error_code);
            return HAL_ERROR;
        }
        /* pull high chip select pin */
        HAL_GPIO_WritePin(hnor->init.cs_gpio_port, hnor->init.cs_gpio_pin, GPIO_PIN_HIGH);

        /* check wel */
        if (data[0] & NOR_STATUS_REG_BIT_WEL)
        {
            break;
        }
    }
    return status;
}

/**
 * @brief wait WIP bit of status reg turning to 0
 * @param hnor NORFLASH handle
 * @param max tolerant time of programming/reading in one operation
 * @retval None
 */
static HAL_Status_T _wait_wip_ready(NORFlash_Handle_T *hnor, uint32_t timeout)
{
    SPI_Handle_T *hspi        = &spi_master_handle;
    uint32_t      tick_start  = HAL_GetTick();
    uint32_t      new_timeout = timeout;
    uint8_t       data[4];
    uint8_t       cmd;
    HAL_Status_T  status = HAL_OK;
    while (1)
    {
        /* refresh timeout */
        status = NORFlash_Get_NewTimeout(&tick_start, new_timeout, &new_timeout);
        if (status != HAL_OK)
        {
            HAL_FLAG_WRITE(hnor->error_code, NOR_ERROR_TIMEOUT);
            return HAL_TIMEOUT;
        }
        /* pull low chip select pin */
        HAL_GPIO_WritePin(hnor->init.cs_gpio_port, hnor->init.cs_gpio_pin, GPIO_PIN_LOW);
        /* send read status register command */
        cmd    = NOR_CMD_READ_STATUS_REG;
        status = HAL_SPI_Transmit(hspi, &cmd, 1, new_timeout);
        if (status != HAL_OK)
        {
            HAL_FLAG_WRITE(hnor->error_code, hspi->error_code);
            return HAL_ERROR;
        }

        /* refresh timeout */
        status = NORFlash_Get_NewTimeout(&tick_start, new_timeout, &new_timeout);
        if (status != HAL_OK)
        {
            HAL_FLAG_WRITE(hnor->error_code, NOR_ERROR_TIMEOUT);
            return HAL_TIMEOUT;
        }
        /* read status data */
        status = HAL_SPI_Receive(hspi, data, 1, new_timeout);
        if (status != HAL_OK)
        {
            HAL_FLAG_WRITE(hnor->error_code, hspi->error_code);
            return HAL_ERROR;
        }
        /* pull high chip select pin */
        HAL_GPIO_WritePin(hnor->init.cs_gpio_port, hnor->init.cs_gpio_pin, GPIO_PIN_HIGH);

        /* check wip */
        if (!(data[0] & NOR_STATUS_REG_BIT_WIP))
        {
            break;
        }
    }
    return HAL_OK;
}

/**
 * @brief check program
 * @param hnor NORFLASH handle
 * @param max tolerant time of programming/reading in one operation
 * @retval None
 */
static HAL_Status_T _check_p_e_success(NORFlash_Handle_T *hnor, uint32_t timeout)
{
    SPI_Handle_T *hspi        = &spi_master_handle;
    uint32_t      tick_start  = HAL_GetTick();
    uint32_t      new_timeout = timeout;
    uint8_t       data[4];
    uint8_t       cmd;
    HAL_Status_T  status = HAL_OK;

    /* pull low chip select pin */
    HAL_GPIO_WritePin(hnor->init.cs_gpio_port, hnor->init.cs_gpio_pin, GPIO_PIN_LOW);
    /* send read security register command */
    cmd    = NOR_CMD_READ_SECURITY_REG;
    status = HAL_SPI_Transmit(hspi, &cmd, 1, new_timeout);
    if (status != HAL_OK)
    {
        HAL_FLAG_WRITE(hnor->error_code, hspi->error_code);
        return HAL_ERROR;
    }

    /* refresh timeout */
    status = NORFlash_Get_NewTimeout(&tick_start, new_timeout, &new_timeout);
    if (status != HAL_OK)
    {
        HAL_FLAG_WRITE(hnor->error_code, NOR_ERROR_TIMEOUT);
        return HAL_ERROR;
    }
    /* read security data */
    status = HAL_SPI_Receive(hspi, data, 1, new_timeout);
    if (status != HAL_OK)
    {
        HAL_FLAG_WRITE(hnor->error_code, hspi->error_code);
        return HAL_ERROR;
    }
    /* pull high chip select pin */
    HAL_GPIO_WritePin(hnor->init.cs_gpio_port, hnor->init.cs_gpio_pin, GPIO_PIN_HIGH);

    if (hnor->flash_state == NOR_FLASH_STATE_PROGRAM)
    {
        if (data[0] & NOR_SECURITY_REG_BIT_P_FAIL)
        {
            return HAL_ERROR;
        }
    }
    else if (hnor->flash_state == NOR_FLASH_STATE_ERASE)
    {
        if (data[0] & NOR_SECURITY_REG_BIT_E_FAIL)
        {
            return HAL_ERROR;
        }
    }

    return status;
}
#endif /* configUSE_NORFLASH_MIDDLEWARE */
