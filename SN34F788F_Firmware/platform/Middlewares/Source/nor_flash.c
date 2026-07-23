/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#include "nor_flash.h"
#include "./NORFlash/MX25L6433F.h"

#if (configUSE_NORFLASH_REGISTER_CALLBACKS == 1)
static void _norflash_reset_callback(NORFlash_Handle_T *hnor);
#endif /* configUSE_NORFLASH_REGISTER_CALLBACKS == 1 */

/**
 * @brief Initialize the NOR FLASH
 * @param TransferType assigns which IP type to participate in the transmission
 * @param hnor pointer to a NORFlash_Handle_T structure that contains the configuration information for NOR FLASH
 * @retval HAL_Status_T
 */
HAL_Status_T HAL_NORFlash_Init(NORFlash_Handle_T *hnor)
{
    HAL_Status_T status = HAL_OK;

    /* check the instance */
    ASSERT_PARAM(IS_NOR_INSTANCE(hnor));

    /* check the SPI FLASH handle */
    ASSERT_PARAM(hnor);

    /* process locked */
    TAKE_MUTEX(hnor);

#if (configUSE_NORFLASH_REGISTER_CALLBACKS == 1)
    _norflash_reset_callback(hnor);
#endif /* configUSE_NORFLASH_REGISTER_CALLBACKS == 1 */

#if (configUSE_NORFLASH_TYPE == NORFLASH_MX25L6433F)
    status = MX25L6433F_Init(hnor);
#endif

    /* process unlocked */
    GIVE_MUTEX(hnor);

    return status;
}

/**
 * @brief DeInitialize the SPI FLASH
 * @param hnor SPI FLASH handle
 * @retval HAL_Status_T
 */
HAL_Status_T HAL_NORFlash_DeInit(NORFlash_Handle_T *hnor)
{
    HAL_Status_T status = HAL_OK;

    /* check the SPI FLASH handle */
    ASSERT_PARAM(hnor);

    /* check the instance */
    ASSERT_PARAM(IS_NOR_INSTANCE(hnor));

    /* process locked */
    TAKE_MUTEX(hnor);

#if (configUSE_NORFLASH_TYPE == NORFLASH_MX25L6433F)
    status = MX25L6433F_DeInit(hnor);
#else
    return HAL_ERROR;
#endif

    /* process unlocked */
    GIVE_MUTEX(hnor);

    return status;
}

/**
 * @brief TX transfer completed callback
 * @param hnor NOR FLASH handle
 * @retval None
 */
__weak void ATTR_ISR HAL_NORFlash_TxCpltCallback(NORFlash_Handle_T *hnor)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hnor);

    /* NOTE : This function should not be modified, when the callback is needed,
              the HAL_NORFlash_TxCpltCallback could be implemented in the user file
     */
}

/**
 * @brief RX transfer completed callback
 * @param hnor NOR FLASH handle
 * @retval None
 */
__weak void ATTR_ISR HAL_NORFlash_RxCpltCallback(NORFlash_Handle_T *hnor)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hnor);

    /* NOTE : This function should not be modified, when the callback is needed,
              the HAL_NORFlash_RxCpltCallback could be implemented in the user file
     */
}

/**
 * @brief NOR error callback
 * @param hnor NOR FLASH handle
 * @retval None
 */
__weak void ATTR_ISR HAL_NORFlash_ErrorCallback(NORFlash_Handle_T *hnor)
{
#if (configUSE_NORFLASH_TYPE == NORFLASH_MX25L6433F)
    MX25L6433F_ErrorCallback(hnor);
#endif
}

#if (configUSE_NORFLASH_REGISTER_CALLBACKS == 1U)
/**
 * @brief Register an NOR FLASH callback instead of the predefined weak function
 * @param hnor NOR FLASH handle
 * @param cb_id ID of the callback to be registered
 * @param pcb pointer to the registered callback
 * @retval HAL_Status_T
 */
HAL_Status_T HAL_NORFlash_RegisterCallback(NORFlash_Handle_T *hnor, NOR_CallbackID_T cb_id, NOR_Callback_T pcb)
{
    HAL_Status_T status = HAL_OK;

    /* Check the valid of SPI FLASH handle */
    ASSERT_PARAM(hnor);

    if (pcb == NULL)
    {
        hnor->error_code = NOR_ERROR_INVALID_CALLBACK;
        return HAL_ERROR;
    }

    /* Process locked */
    TAKE_MUTEX(hnor);

    if (HAL_FLAG_EQU(hnor->state, HAL_STATE_READY))
    {
        switch (cb_id)
        {
            case NOR_TX_COMPLETE_CB_ID:
                hnor->TxCpltCallback = pcb;
                break;
            case NOR_RX_COMPLETE_CB_ID:
                hnor->RxCpltCallback = pcb;
                break;
            case NOR_ERROR_CB_ID:
                hnor->ErrorCallback = pcb;
                break;
            default:
                /* Update the error code */
                hnor->error_code = NOR_ERROR_INVALID_CALLBACK;
                /* Return error status */
                status = HAL_ERROR;
                break;
        }
    }
    else
    {
        status = HAL_ERROR;
    }

    /* Process unlocked */
    GIVE_MUTEX(hnor);

    return status;
}

/**
 * @brief Unregister an NOR FLASH callback
 *        NOR FLASH callback is redirected to the predefined weak function
 * @param hnor NOR FLASH handle
 * @param cb_id ID of the callback to be unregistered
 * @param pcb pointer to the registered callback
 * @retval HAL_Status_T
 */
HAL_Status_T HAL_NORFLASH_UnRegisterCallback(NORFlash_Handle_T *hnor, NOR_CallbackID_T cb_id)
{
    HAL_Status_T status = HAL_OK;

    /* Check the valid of NOR FLASH handle */
    ASSERT_PARAM(hnor);

    /* Process locked */
    TAKE_MUTEX(hnor);

    if (HAL_FLAG_EQU(hnor->state, HAL_STATE_READY))
    {
        switch (cb_id)
        {
            case NOR_TX_COMPLETE_CB_ID:
                hnor->TxCpltCallback = HAL_NORFlash_TxCpltCallback;
                break;
            case NOR_RX_COMPLETE_CB_ID:
                hnor->RxCpltCallback = HAL_NORFlash_RxCpltCallback;
                break;
            case NOR_ERROR_CB_ID:
                hnor->ErrorCallback = HAL_NORFlash_ErrorCallback;
                break;
            default:
                /* Update the error code */
                hnor->error_code = NOR_ERROR_INVALID_CALLBACK;
                /* Return error status */
                status = HAL_ERROR;
                break;
        }
    }
    else
    {
        status = HAL_ERROR;
    }

    /* Process unlocked */
    GIVE_MUTEX(hnor);

    return status;
}
#endif /* configUSE_NORFLASH_REGISTER_CALLBACKS */

#if (configUSE_NORFLASH_BLOCKING == 1)
/**
 * @brief Program an amount of data to NOR FLASH in blocking mode
 * @param hnor NOR FLASH handle
 * @param start_addr start physical NOR FLASH address
 * @param data_buffer pointer to data buffer
 * @param write_size amount of data to be programmed
 * @param timeout timeout duration
 * @retval HAL_Status_T
 */
HAL_Status_T HAL_NORFlash_Program(NORFlash_Handle_T *hnor, uint32_t start_addr, uint8_t *data_buffer, uint32_t write_size, uint32_t timeout)
{
    HAL_Status_T status = HAL_OK;

#if (configUSE_NORFLASH_TYPE == NORFLASH_MX25L6433F)
    /* Operation Address cann't excceed flash max size */
    if (start_addr + write_size >= NOR_MEM_SIZE)
        return HAL_ERROR;

    status = MX25L6433F_Program(hnor, start_addr, data_buffer, write_size, timeout);
#endif

    return status;
}

/**
 * @brief Read an amount of data from NOR FLASH in blocking mode
 * @param hnor NOR FLASH handle
 * @param rx_type read type, legal parameter is NOR_TYPE_RX_READ or NOR_TYPE_RX_FAST_READ
 * @param start_addr start physical NOR FLASH address
 * @param data_buffer pointer to data buffer
 * @param read_size amount of data to be read
 * @param timeout timeout duration
 * @retval HAL_Status_T
 */
HAL_Status_T HAL_NORFlash_Read(NORFlash_Handle_T *hnor, uint32_t rx_type, uint32_t start_addr, uint8_t *data_buffer, uint32_t read_size, uint32_t timeout)
{
    HAL_Status_T status = HAL_OK;

#if (configUSE_NORFLASH_TYPE == NORFLASH_MX25L6433F)
    /* Operation Address cann't excceed flash max size */
    if (start_addr + read_size >= NOR_MEM_SIZE)
        return HAL_ERROR;

    status = MX25L6433F_Read(hnor, rx_type, start_addr, data_buffer, read_size, timeout);
#endif

    return status;
}
#endif /* configUSE_NORFLASH_BLOCKING */

#if (configUSE_NORFLASH_TYPE == NORFLASH_MX25L6433F)

#if (configUSE_NORFLASH_BLOCKING == 1)
/**
 * @brief Erase a NOR FLASH block, one block size is 32 kilobyte(KB)
 * @param hnor NOR FLASH handle
 * @param start_addr start physical NOR FLASH address
 * @param timeout timeout duration
 * @retval HAL_Status_T
 */
HAL_Status_T HAL_NORFlash_EraseBlock32(NORFlash_Handle_T *hnor, uint32_t start_addr, uint32_t timeout)
{
    HAL_Status_T status = HAL_OK;

#if (configUSE_NORFLASH_TYPE == NORFLASH_MX25L6433F)
    /* Operation Address cann't excceed flash max size */
    if (start_addr >= NOR_MEM_SIZE)
        return HAL_ERROR;

    status = MX25L6433F_EraseBlock32(hnor, start_addr, timeout);
#endif

    return status;
}

/**
 * @brief Erase a NOR FLASH block, one block size is 64 kilobyte(KB)
 * @param hnor NOR FLASH handle
 * @param start_addr start physical NOR FLASH address
 * @param timeout timeout duration
 * @retval HAL_Status_T
 */
HAL_Status_T HAL_NORFlash_EraseBlock64(NORFlash_Handle_T *hnor, uint32_t start_addr, uint32_t timeout)
{
    HAL_Status_T status = HAL_OK;

#if (configUSE_NORFLASH_TYPE == NORFLASH_MX25L6433F)
    /* Operation Address cann't excceed flash max size */
    if (start_addr >= NOR_MEM_SIZE)
        return HAL_ERROR;

    status = MX25L6433F_EraseBlock64(hnor, start_addr, timeout);
#endif

    return status;
}

/**
 * @brief Protect Block from writing and erasing
 * @param hnor NOR FLASH handle
 * @param block_area block count to be protect @ref protect_area
 * @param timeout timeout duration
 * @retval HAL_Status_T
 */
HAL_Status_T HAL_NORFlash_ProtectBlock(NORFlash_Handle_T *hnor, uint32_t block_area, uint32_t timeout)
{
    HAL_Status_T status = HAL_OK;
#if (configUSE_NORFLASH_TYPE == NORFLASH_MX25L6433F)
    status = MX25L6433F_ProtectBlock(hnor, block_area, timeout);
#endif
    return status;
}

/**
 * @brief Protect all block from writing and erasing
 * @param hnor NOR FLASH handle
 * @param timeout timeout duration
 * @retval HAL_Status_T
 */
HAL_Status_T HAL_NORFlash_ProtectBlockAll(NORFlash_Handle_T *hnor, uint32_t timeout)
{
    HAL_Status_T status = HAL_OK;
#if (configUSE_NORFLASH_TYPE == NORFLASH_MX25L6433F)
    status = MX25L6433F_ProtectBlockAll(hnor, timeout);
#endif
    return status;
}

/**
 * @brief Resume all block protecting, so it can be programmed
 * @param hnor NOR FLASH handle
 * @param timeout timeout duration
 * @retval HAL_Status_T
 */
HAL_Status_T HAL_NORFlash_ProtectResumeAll(NORFlash_Handle_T *hnor, uint32_t timeout)
{
    HAL_Status_T status = HAL_OK;
#if (configUSE_NORFLASH_TYPE == NORFLASH_MX25L6433F)
    status = MX25L6433F_ProtectResumeAll(hnor, timeout);
#endif
    return status;
}

/**
 * @brief Enter DPD, this mode blocks every instruction except RDP and RES
 * @note  flash current will change from 10uA to 3uA approximately
 * @param hnor NOR FLASH handle
 * @retval HAL_Status_T
 */
HAL_Status_T HAL_NORFlash_EnterDPD(NORFlash_Handle_T *hnor)
{
    HAL_Status_T status = HAL_OK;
#if (configUSE_NORFLASH_TYPE == NORFLASH_MX25L6433F)
    status = MX25L6433F_EnterDPD(hnor);
#endif
    return status;
}

/**
 * @brief Exit DPD, return standby mode
 * @param hnor NOR FLASH handle
 * @retval HAL_Status_T
 */
HAL_Status_T HAL_NORFlash_ExitDPD(NORFlash_Handle_T *hnor)
{
    HAL_Status_T status = HAL_OK;
#if (configUSE_NORFLASH_TYPE == NORFLASH_MX25L6433F)
    status = MX25L6433F_ExitDPD(hnor);
#endif
    return status;
}

/**
 * @brief Exit DPD, return standby mode
 * @param hnor NOR FLASH handle
 * @retval HAL_Status_T
 */
uint32_t HAL_NORFlash_ReadID(NORFlash_Handle_T *hnor)
{
    uint32_t Flash_ID = 0;
#if (configUSE_NORFLASH_TYPE == NORFLASH_MX25L6433F)
    Flash_ID = MX25L6433F_ReadID(hnor);
#endif
    return Flash_ID;
}

#endif /* configUSE_NORFLASH_BLOCKING */

#if (configUSE_NORFLASH_NONBLOCKING_DMA == 1U)

/**
 * @brief Program an amount of data to NOR FLASH in DMA mode
 * @param hnor NOR FLASH handle
 * @param start_addr start physical NOR FLASH address
 * @param data_buffer pointer to data buffer
 * @param write_size amount of data to be programmed
 * @retval HAL_Status_T
 */
HAL_Status_T HAL_NORFlash_Program_DMA(NORFlash_Handle_T *hnor, uint32_t start_addr, uint8_t *data_buffer, uint32_t write_size)
{
    HAL_Status_T status = HAL_OK;

#if (configUSE_NORFLASH_TYPE == NORFLASH_MX25L6433F)
    /* Operation Address cann't excceed flash max size */
    if (start_addr + write_size >= NOR_MEM_SIZE)
        return HAL_ERROR;

    status = MX25L6433F_Program_DMA(hnor, start_addr, data_buffer, write_size);
#endif

    return status;
}

/**
 * @brief Read an amount of data from NOR FLASH in DMA mode
 * @param hnor NOR FLASH handle
 * @param rx_type read type, legal parameter is NOR_TYPE_RX_READ or NOR_TYPE_RX_FAST_READ
 * @param start_addr start physical NOR FLASH address
 * @param data_buffer pointer to data buffer
 * @param read_size amount of data to be read
 * @retval HAL_Status_T
 */
HAL_Status_T HAL_NORFlash_Read_DMA(NORFlash_Handle_T *hnor, uint32_t rx_type, uint32_t start_addr, uint8_t *data_buffer, uint32_t read_size)
{
    HAL_Status_T status = HAL_OK;

#if (configUSE_NORFLASH_TYPE == NORFLASH_MX25L6433F)
    /* Operation Address cann't excceed flash max size */
    if (start_addr + read_size >= NOR_MEM_SIZE)
        return HAL_ERROR;

    status = MX25L6433F_Read_DMA(hnor, rx_type, start_addr, data_buffer, read_size);
#endif

    return status;
}

#endif /* configUSE_NORFLASH_NONBLOCKING_DMA */

#endif /* configUSE_NORFLASH_TYPE == NORFLASH_MX25L6433F */

/**
 * @brief Erase a NOR FLASH sector, one sector size is 4 kilobyte(KB)
 * @param hnor NOR FLASH handle
 * @param start_addr start physical NOR FLASH address
 * @param timeout timeout duration
 * @retval HAL_Status_T
 */
HAL_Status_T HAL_NORFlash_EraseSector(NORFlash_Handle_T *hnor, uint32_t start_addr, uint32_t timeout)
{
    HAL_Status_T status = HAL_OK;

#if (configUSE_NORFLASH_TYPE == NORFLASH_MX25L6433F)
    /* Operation Address cann't excceed flash max size */
    if (start_addr >= NOR_MEM_SIZE)
        return HAL_ERROR;

    status = MX25L6433F_EraseSector(hnor, start_addr, timeout);
#endif

    return status;
}

/**
 * @brief Erase all NOR FLASH memory
 * @param hnor NOR FLASH handle
 * @param timeout timeout duration
 * @retval HAL_Status_T
 */
HAL_Status_T HAL_NORFlash_EraseChip(NORFlash_Handle_T *hnor, uint32_t timeout)
{
    HAL_Status_T status = HAL_OK;

#if (configUSE_NORFLASH_TYPE == NORFLASH_MX25L6433F)
    status = MX25L6433F_EraseChip(hnor, timeout);
#endif

    return status;
}

/**
 * @brief Get the FLASH Capacity
 * @retval HAL_Status_T
 */
HAL_Status_T HAL_NORFlash_GetCapacity(NORFlash_Capacity_T *capacity)
{
    HAL_Status_T status = HAL_OK;
#if (configUSE_NORFLASH_TYPE == NORFLASH_MX25L6433F)
    status = MX25L6433F_GetCapacity(capacity);
#endif
    return status;
}

/**
 * @brief Get the NOR FLASH state
 * @param timeout timeout duration
 * @retval HAL_Status_T
 */
HAL_State_T HAL_NORFlash_GetState(NORFlash_Handle_T *hnor)
{
    HAL_State_T status = HAL_STATE_READY;

#if (configUSE_NORFLASH_TYPE == NORFLASH_MX25L6433F)
    status = MX25L6433F_GetState(hnor);
#endif

    return status;
}

/**
 * @brief Return the NOR FLASH error code
 * @param hnor NOR FLASH handle
 * @retval NOR FLASH error code
 */
uint32_t HAL_NORFlash_GetError(NORFlash_Handle_T *hnor)
{
    uint32_t error_code = HAL_OK;

#if (configUSE_NORFLASH_TYPE == NORFLASH_MX25L6433F)
    error_code = MX25L6433F_GetError(hnor);
#endif

    return error_code;
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
HAL_Status_T HAL_NORFlash_Abort(NORFlash_Handle_T *hnor)
{
    HAL_Status_T status = HAL_OK;
    status              = MX25L6433F_Abort(hnor);
    return status;
}

#if (configUSE_NORFLASH_REGISTER_CALLBACKS == 1U)
/**
 * @brief Reset NOR FLASH callbacks to inner weak functions
 * @param hnor NOR FLASH handle
 */
void HAL_NORFlash_ResetCallback(NORFlash_Handle_T *hnor)
{
    hnor->TxCpltCallback = HAL_NORFlash_TxCpltCallback;
    hnor->RxCpltCallback = HAL_NORFlash_RxCpltCallback;
    hnor->ErrorCallback  = HAL_NORFlash_ErrorCallback;
}
#endif /* configUSE_NORFLASH_REGISTER_CALLBACKS */

/**
 * @brief Get remain time
 * @param tick_start start timer
 * @param old_timeout previous remain timeout
 * @param new_timeout remain timeout
 * @retval None
 */
HAL_Status_T NORFlash_Get_NewTimeout(uint32_t *tick_start, uint32_t old_timeout, uint32_t *new_timeout)
{
    uint32_t cur_tick = HAL_GetTick();
    uint32_t tick_diff;

    if (old_timeout == 0)
    {
        return HAL_TIMEOUT;
    }

    tick_diff = cur_tick - *tick_start;
    if (old_timeout < tick_diff)
    {
        return HAL_TIMEOUT;
    }

    *tick_start  = cur_tick;
    *new_timeout = (old_timeout - tick_diff);
    return HAL_OK;
}

#if (configUSE_NORFLASH_REGISTER_CALLBACKS == 1U)
static void _norflash_reset_callback(NORFlash_Handle_T *hnor)
{
    hnor->TxCpltCallback = HAL_NORFlash_TxCpltCallback;
    hnor->RxCpltCallback = HAL_NORFlash_RxCpltCallback;
    hnor->ErrorCallback  = HAL_NORFlash_ErrorCallback;
}
#endif /* configUSE_NORFLASH_REGISTER_CALLBACKS */
