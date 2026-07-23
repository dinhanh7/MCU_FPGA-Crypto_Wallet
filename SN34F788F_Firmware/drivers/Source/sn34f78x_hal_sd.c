/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#include "sn34f78x_hal_sd.h"

/* Global variables ---------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/** @defgroup SD_Private_Functions SD Private Functions
 * @{
 */
static uint32_t SD_InitCard(SD_Handle_T *hsd);
static uint32_t SD_PowerON(SD_Handle_T *hsd);
static uint32_t SD_SendSDStatus(SD_Handle_T *hsd, uint32_t *pSDstatus);
static uint32_t SD_SendStatus(SD_Handle_T *hsd, uint32_t *pCardStatus);
static uint32_t SD_WideBus_Enable(SD_Handle_T *hsd);
static uint32_t SD_WideBus_Disable(SD_Handle_T *hsd);
static uint32_t SD_FindSCR(SD_Handle_T *hsd, uint32_t *pSCR);
static void     SD_PowerOFF(SD_Handle_T *hsd);
bool            SDMMC_Fill_ADMA_Desc_Table(uint32_t total_data, uint32_t *data_addr);

/**
 * @}
 */

/**
  * @brief  Initializes the SD according to the specified parameters in the
            SD_HandleTypeDef and create the associated handle.
  * @param  hsd: Pointer to the SD handle
  * @retval HAL status
  */
HAL_Status_T HAL_SD_Init(SD_Handle_T *hsd)
{
    HAL_SD_CardStatusTypeDef CardStatus;
    uint32_t                 speedgrade;
    uint32_t                 unitsize;
    uint32_t                 tickstart;

    /* Check the SD handle allocation */
    if (hsd == NULL)
    {
        return HAL_ERROR;
    }

    /* Check the parameters */
    ASSERT_PARAM(IS_SDMMC_ALL_INSTANCE(hsd->instance));
    ASSERT_PARAM(IS_SDMMC_BUS_WIDE(hsd->Init.BusWide));
    ASSERT_PARAM(IS_SDMMC_CLKDIV(hsd->Init.ClockDiv));

    if (hsd->state == HAL_STATE_RESET)
    {
        /* Allocate lock resource and initialize it */
        HAL_FLAG_RESET(hsd->mutex);
#if (configUSE_SD_REGISTER_CALLBACKS == 1U)
        /* Reset Callback pointers in HAL_SD_STATE_RESET only */
        hsd->TxCpltCallback    = HAL_SD_TxCpltCallback;
        hsd->RxCpltCallback    = HAL_SD_RxCpltCallback;
        hsd->ErrorCallback     = HAL_SD_ErrorCallback;
        hsd->AbortCpltCallback = HAL_SD_AbortCallback;

        if (hsd->MspInitCallback == NULL)
        {
            hsd->MspInitCallback = HAL_SD_MspInit;
        }

        /* Init the low level hardware */
        hsd->MspInitCallback(hsd);
#else
        /* Init the low level hardware : GPIO, CLOCK, CORTEX...etc */
        HAL_SD_MspInit(hsd);
#endif /* configUSE_SD_REGISTER_CALLBACKS */
    }

    /* Change SD peripheral state */
    HAL_FLAG_SET(hsd->state, HAL_STATE_BUSY);

    /* Clear Error Code status */
    hsd->ErrorCode = HAL_SD_ERROR_NONE;

    /* Initialize the Card parameters */
    if (HAL_SD_InitCard(hsd) != HAL_OK)
    {
        return HAL_ERROR;
    }

    if (HAL_SD_GetCardStatus(hsd, &CardStatus) != HAL_OK)
    {
        return HAL_ERROR;
    }
    /* Get Initial Card Speed from Card Status*/
    speedgrade = CardStatus.UhsSpeedGrade;
    unitsize   = CardStatus.UhsAllocationUnitSize;
    if ((hsd->SdCard.CardType == CARD_SDHC_SDXC) && ((speedgrade != 0U) || (unitsize != 0U)))
    {
        hsd->SdCard.CardSpeed = CARD_ULTRA_HIGH_SPEED;
    }
    else
    {
        if (hsd->SdCard.CardType == CARD_SDHC_SDXC)
        {
            hsd->SdCard.CardSpeed = CARD_HIGH_SPEED;
        }
        else
        {
            hsd->SdCard.CardSpeed = CARD_NORMAL_SPEED;
        }
    }

    /* Get SCR value */
    if (SD_FindSCR(hsd, hsd->SCR) != HAL_OK)
    {
        return HAL_ERROR;
    }

#if (configUSE_SD_AUTOCMD_ENABLE == 1)
    /* Get the CMD 23 support */
    hsd->SdCard.Cmd23Support = (hsd->SCR[0] & SDMMC_CMD23_SUPPORT);
#endif

    /* Configure the bus wide */
    if (HAL_SD_ConfigWideBusOperation(hsd, hsd->Init.BusWide) != HAL_OK)
    {
        return HAL_ERROR;
    }

    /* Verify that SD card is ready to use after Initialization */
    tickstart = HAL_GetTick();
    while ((HAL_SD_GetCardState(hsd) != HAL_SD_CARD_TRANSFER))
    {
        if ((HAL_GetTick() - tickstart) >= SDMMC_DATATIMEOUT)
        {
            hsd->ErrorCode = HAL_SD_ERROR_TIMEOUT;
            hsd->state     = HAL_STATE_READY;
            return HAL_TIMEOUT;
        }
    }

    if (hsd->Init.ClockDiv == SDMMC_HIGH_TRANSFER_CLK_DIV)
    {
        /* Configure the bus speed */
        if (HAL_SD_ConfigSpeedBusOperation(hsd, SDMMC_SPEED_MODE_AUTO) != HAL_OK)
        {
            return HAL_ERROR;
        }

        /* Verify that SD card is ready to use after Initialization */
        tickstart = HAL_GetTick();
        while ((HAL_SD_GetCardState(hsd) != HAL_SD_CARD_TRANSFER))
        {
            if ((HAL_GetTick() - tickstart) >= SDMMC_DATATIMEOUT)
            {
                hsd->ErrorCode = HAL_SD_ERROR_TIMEOUT;
                hsd->state     = HAL_STATE_READY;
                return HAL_TIMEOUT;
            }
        }
    }
    else
    {
        SDMMC_Speed_Config(hsd->instance, hsd->Init.ClockDiv);
    }

    /* Initialize the error code */
    hsd->ErrorCode = HAL_SD_ERROR_NONE;

    /* Initialize the SD operation */
    hsd->Context = SD_CONTEXT_NONE;

    /* Initialize the SD state */
    hsd->state = HAL_STATE_READY;

    return HAL_OK;
}

/**
  * @brief  Initializes the SD Card.
  * @param  hsd: Pointer to SD handle
  * @note   This function initializes the SD card. It could be used when a card
            re-initialization is needed.
  * @retval HAL status
  */
HAL_Status_T HAL_SD_InitCard(SD_Handle_T *hsd)
{
    uint32_t  errorstate;
    SD_Init_T Init;

    /* Default SDIO peripheral configuration for SD card initialization */
    Init.ClockDiv = SDMMC_INIT_CLK_DIV;
    Init.BusWide  = SDMMC_BUS_WIDE_1B;

    /* Initialize SDIO peripheral interface with default configuration */
    (void)SDMMC_Init(hsd->instance, Init);

    /* Set Power State to ON */
    (void)SDMMC_PowerState_ON(hsd->instance);

    /* Identify card operating voltage */
    errorstate = SD_PowerON(hsd);
    if (errorstate != HAL_SD_ERROR_NONE)
    {
        hsd->state = HAL_STATE_READY;
        hsd->ErrorCode |= errorstate;
        return HAL_ERROR;
    }

    /* Card initialization */
    errorstate = SD_InitCard(hsd);
    if (errorstate != HAL_SD_ERROR_NONE)
    {
        hsd->state = HAL_STATE_READY;
        hsd->ErrorCode |= errorstate;
        return HAL_ERROR;
    }

    /* Set Block Size for Card */
    errorstate = SDMMC_CmdBlockLength(hsd->instance, BLOCKSIZE);
    if (errorstate != HAL_SD_ERROR_NONE)
    {
        /* Clear all the static flags */
        hsd->ErrorCode |= errorstate;
        hsd->state = HAL_STATE_READY;
        return HAL_ERROR;
    }

    return HAL_OK;
}

/**
 * @brief  De-Initializes the SD card.
 * @param  hsd: Pointer to SD handle
 * @retval HAL status
 */
HAL_Status_T HAL_SD_DeInit(SD_Handle_T *hsd)
{
    /* Check the SD handle allocation */
    if (hsd == NULL)
    {
        return HAL_ERROR;
    }

    hsd->state   = HAL_STATE_BUSY;
    hsd->Context = SD_CONTEXT_NONE;

    /* Set SD power state to off */
    SD_PowerOFF(hsd);

#if (configUSE_SD_REGISTER_CALLBACKS == 1U)
    if (hsd->MspDeInitCallback == NULL)
    {
        hsd->MspDeInitCallback = HAL_SD_MspDeInit;
    }

    /* DeInit the low level hardware */
    hsd->MspDeInitCallback(hsd);
#else
    /* De-Initialize the MSP layer */
    HAL_SD_MspDeInit(hsd);
#endif /* configUSE_SD_REGISTER_CALLBACKS */

    hsd->ErrorCode = HAL_SD_ERROR_NONE;
    hsd->state     = HAL_STATE_RESET;

    return HAL_OK;
}

/**
 * @brief  Initializes the SD MSP.
 * @param  hsd: Pointer to SD handle
 * @retval None
 */
__weak void HAL_SD_MspInit(SD_Handle_T *hsd)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hsd);

    /* NOTE : This function should not be modified, when the callback is needed,
              the HAL_SD_MspInit could be implemented in the user file
     */
}

/**
 * @brief  De-Initialize SD MSP.
 * @param  hsd: Pointer to SD handle
 * @retval None
 */
__weak void HAL_SD_MspDeInit(SD_Handle_T *hsd)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hsd);

    /* NOTE : This function should not be modified, when the callback is needed,
              the HAL_SD_MspDeInit could be implemented in the user file
     */
}

/**
 * @brief Tx Transfer completed callbacks
 * @param hsd: Pointer to SD handle
 * @retval None
 */
__weak void HAL_SD_TxCpltCallback(SD_Handle_T *hsd)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hsd);

    /* NOTE : This function should not be modified, when the callback is needed,
              the HAL_SD_TxCpltCallback can be implemented in the user file
     */
}

/**
 * @brief Rx Transfer completed callbacks
 * @param hsd: Pointer SD handle
 * @retval None
 */
__weak void HAL_SD_RxCpltCallback(SD_Handle_T *hsd)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hsd);

    /* NOTE : This function should not be modified, when the callback is needed,
              the HAL_SD_RxCpltCallback can be implemented in the user file
     */
}

/**
 * @brief SD error callbacks
 * @param hsd: Pointer SD handle
 * @retval None
 */
__weak void HAL_SD_ErrorCallback(SD_Handle_T *hsd)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hsd);

    /* NOTE : This function should not be modified, when the callback is needed,
              the HAL_SD_ErrorCallback can be implemented in the user file
     */
}

/**
 * @brief SD Abort callbacks
 * @param hsd: Pointer SD handle
 * @retval None
 */
__weak void HAL_SD_AbortCallback(SD_Handle_T *hsd)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hsd);

    /* NOTE : This function should not be modified, when the callback is needed,
              the HAL_SD_AbortCallback can be implemented in the user file
     */
}

#if (configUSE_SD_REGISTER_CALLBACKS == 1U)
/**
 * @brief  Register a User SD Callback
 *         To be used instead of the weak (surcharged) predefined callback
 * @param hsd : SD handle
 * @param CallbackID : ID of the callback to be registered
 *        This parameter can be one of the following values:
 *          @arg @ref HAL_SD_TX_CPLT_CB_ID    SD Tx Complete Callback ID
 *          @arg @ref HAL_SD_RX_CPLT_CB_ID    SD Rx Complete Callback ID
 *          @arg @ref HAL_SD_ERROR_CB_ID      SD Error Callback ID
 *          @arg @ref HAL_SD_ABORT_CB_ID      SD Abort Callback ID
 *          @arg @ref HAL_SD_MSP_INIT_CB_ID   SD MspInit Callback ID
 *          @arg @ref HAL_SD_MSP_DEINIT_CB_ID SD MspDeInit Callback ID
 * @param pCallback : pointer to the Callback function
 * @retval status
 */
HAL_Status_T HAL_SD_RegisterCallback(SD_Handle_T *hsd, HAL_SD_CallbackIDTypeDef CallbackID, pSD_Callback_T pCallback)
{
    HAL_Status_T status = HAL_OK;

    if (pCallback == NULL)
    {
        /* Update the error code */
        hsd->ErrorCode |= HAL_SD_ERROR_INVALID_CALLBACK;
        return HAL_ERROR;
    }

    if (hsd->state == HAL_STATE_READY)
    {
        switch (CallbackID)
        {
            case HAL_SD_TX_CPLT_CB_ID:
                hsd->TxCpltCallback = pCallback;
                break;
            case HAL_SD_RX_CPLT_CB_ID:
                hsd->RxCpltCallback = pCallback;
                break;
            case HAL_SD_ERROR_CB_ID:
                hsd->ErrorCallback = pCallback;
                break;
            case HAL_SD_ABORT_CB_ID:
                hsd->AbortCpltCallback = pCallback;
                break;
            case HAL_SD_MSP_INIT_CB_ID:
                hsd->MspInitCallback = pCallback;
                break;
            case HAL_SD_MSP_DEINIT_CB_ID:
                hsd->MspDeInitCallback = pCallback;
                break;
            default:
                /* Update the error code */
                hsd->ErrorCode |= HAL_SD_ERROR_INVALID_CALLBACK;
                /* update return status */
                status = HAL_ERROR;
                break;
        }
    }
    else if (hsd->state == HAL_STATE_RESET)
    {
        switch (CallbackID)
        {
            case HAL_SD_MSP_INIT_CB_ID:
                hsd->MspInitCallback = pCallback;
                break;
            case HAL_SD_MSP_DEINIT_CB_ID:
                hsd->MspDeInitCallback = pCallback;
                break;
            default:
                /* Update the error code */
                hsd->ErrorCode |= HAL_SD_ERROR_INVALID_CALLBACK;
                /* update return status */
                status = HAL_ERROR;
                break;
        }
    }
    else
    {
        /* Update the error code */
        hsd->ErrorCode |= HAL_SD_ERROR_INVALID_CALLBACK;
        /* update return status */
        status = HAL_ERROR;
    }

    return status;
}

/**
 * @brief  Unregister a User SD Callback
 *         SD Callback is redirected to the weak (surcharged) predefined callback
 * @param hsd : SD handle
 * @param CallbackID : ID of the callback to be unregistered
 *        This parameter can be one of the following values:
 *          @arg @ref HAL_SD_TX_CPLT_CB_ID    SD Tx Complete Callback ID
 *          @arg @ref HAL_SD_RX_CPLT_CB_ID    SD Rx Complete Callback ID
 *          @arg @ref HAL_SD_ERROR_CB_ID      SD Error Callback ID
 *          @arg @ref HAL_SD_ABORT_CB_ID      SD Abort Callback ID
 *          @arg @ref HAL_SD_MSP_INIT_CB_ID   SD MspInit Callback ID
 *          @arg @ref HAL_SD_MSP_DEINIT_CB_ID SD MspDeInit Callback ID
 * @retval status
 */
HAL_Status_T HAL_SD_UnRegisterCallback(SD_Handle_T *hsd, HAL_SD_CallbackIDTypeDef CallbackID)
{
    HAL_Status_T status = HAL_OK;

    if (hsd->state == HAL_STATE_READY)
    {
        switch (CallbackID)
        {
            case HAL_SD_TX_CPLT_CB_ID:
                hsd->TxCpltCallback = HAL_SD_TxCpltCallback;
                break;
            case HAL_SD_RX_CPLT_CB_ID:
                hsd->RxCpltCallback = HAL_SD_RxCpltCallback;
                break;
            case HAL_SD_ERROR_CB_ID:
                hsd->ErrorCallback = HAL_SD_ErrorCallback;
                break;
            case HAL_SD_ABORT_CB_ID:
                hsd->AbortCpltCallback = HAL_SD_AbortCallback;
                break;
            case HAL_SD_MSP_INIT_CB_ID:
                hsd->MspInitCallback = HAL_SD_MspInit;
                break;
            case HAL_SD_MSP_DEINIT_CB_ID:
                hsd->MspDeInitCallback = HAL_SD_MspDeInit;
                break;
            default:
                /* Update the error code */
                hsd->ErrorCode |= HAL_SD_ERROR_INVALID_CALLBACK;
                /* update return status */
                status = HAL_ERROR;
                break;
        }
    }
    else if (hsd->state == HAL_STATE_RESET)
    {
        switch (CallbackID)
        {
            case HAL_SD_MSP_INIT_CB_ID:
                hsd->MspInitCallback = HAL_SD_MspInit;
                break;
            case HAL_SD_MSP_DEINIT_CB_ID:
                hsd->MspDeInitCallback = HAL_SD_MspDeInit;
                break;
            default:
                /* Update the error code */
                hsd->ErrorCode |= HAL_SD_ERROR_INVALID_CALLBACK;
                /* update return status */
                status = HAL_ERROR;
                break;
        }
    }
    else
    {
        /* Update the error code */
        hsd->ErrorCode |= HAL_SD_ERROR_INVALID_CALLBACK;
        /* update return status */
        status = HAL_ERROR;
    }

    return status;
}
#endif /* configUSE_SD_REGISTER_CALLBACKS */

/**
 * @brief  Enquires cards about their operating voltage and configures clock
 *         controls and stores SD information that will be needed in future
 *         in the SD handle.
 * @param  hsd: Pointer to SD handle
 * @retval error state
 */
static uint32_t SD_PowerON(SD_Handle_T *hsd)
{
    __IO uint32_t count    = 0U;
    uint32_t      response = 0U, validvoltage = 0U;
    uint32_t      errorstate;

    /* CMD0: GO_IDLE_STATE */
    errorstate = SDMMC_CmdGoIdleState(hsd->instance);
    if (errorstate != HAL_SD_ERROR_NONE)
    {
        return errorstate;
    }

    /* CMD8: SEND_IF_COND: Command available only on V2.0 cards */
    errorstate = SDMMC_CmdOperCond(hsd->instance);
    if (errorstate != HAL_SD_ERROR_NONE)
    {
        hsd->SdCard.CardVersion = CARD_V1_X;
        /* CMD0: GO_IDLE_STATE */
        errorstate = SDMMC_CmdGoIdleState(hsd->instance);
        if (errorstate != HAL_SD_ERROR_NONE)
        {
            return errorstate;
        }
    }
    else
    {
        hsd->SdCard.CardVersion = CARD_V2_X;
    }

    if (hsd->SdCard.CardVersion == CARD_V2_X)
    {
        /* SEND CMD55 APP_CMD with RCA as 0 */
        errorstate = SDMMC_CmdAppCommand(hsd->instance, 0);
        if (errorstate != HAL_SD_ERROR_NONE)
        {
            return HAL_SD_ERROR_UNSUPPORTED_FEATURE;
        }
    }
    /* SD CARD */
    /* Send ACMD41 SD_APP_OP_COND with Argument 0x80100000 */
    while ((count < SDMMC_MAX_VOLT_TRIAL) && (validvoltage == 0U))
    {
        /* SEND CMD55 APP_CMD with RCA as 0 */
        errorstate = SDMMC_CmdAppCommand(hsd->instance, 0);
        if (errorstate != HAL_SD_ERROR_NONE)
        {
            return errorstate;
        }

        /* Send CMD41 */
        errorstate = SDMMC_CmdAppOperCommand(hsd->instance, SDMMC_VOLTAGE_WINDOW_SD | SDMMC_HIGH_CAPACITY | SD_SWITCH_1_8V_CAPACITY);
        if (errorstate != HAL_SD_ERROR_NONE)
        {
            return HAL_SD_ERROR_UNSUPPORTED_FEATURE;
        }

        /* Get command response */
        response = SDMMC_GetResponse(hsd->instance, SDMMC_RESP0);

        /* Get operating voltage*/
        validvoltage = (((response >> 31U) == 1U) ? 1U : 0U);

        count++;
    }

    if (count >= SDMMC_MAX_VOLT_TRIAL)
    {
        return HAL_SD_ERROR_INVALID_VOLTRANGE;
    }

    if ((response & SDMMC_HIGH_CAPACITY) == SDMMC_HIGH_CAPACITY) /* (response &= SD_HIGH_CAPACITY) */
    {
        hsd->SdCard.CardType = CARD_SDHC_SDXC;
    }
    else
    {
        hsd->SdCard.CardType = CARD_SDSC;
    }

    return HAL_SD_ERROR_NONE;
}

/**
 * @brief  Turns the SDIO output signals off.
 * @param  hsd: Pointer to SD handle
 * @retval None
 */
static void SD_PowerOFF(SD_Handle_T *hsd)
{
    /* Set Power State to OFF */
    (void)SDMMC_PowerState_OFF(hsd->instance);
}

/**
 * @brief  Returns information the information of the card which are stored on
 *         the CID register.
 * @param  hsd: Pointer to SD handle
 * @param  pCID: Pointer to a HAL_SD_CardCIDTypeDef structure that
 *         contains all CID register parameters
 * @retval HAL status
 */
HAL_Status_T HAL_SD_GetCardCID(SD_Handle_T *hsd, HAL_SD_CardCIDTypeDef *pCID)
{
    pCID->ManufacturerID = (uint8_t)((hsd->CID[0] & 0xFF000000U) >> 24U);

    pCID->OEM_AppliID = (uint16_t)((hsd->CID[0] & 0x00FFFF00U) >> 8U);

    pCID->ProdName1 = (((hsd->CID[0] & 0x000000FFU) << 24U) | ((hsd->CID[1] & 0xFFFFFF00U) >> 8U));

    pCID->ProdName2 = (uint8_t)(hsd->CID[1] & 0x000000FFU);

    pCID->ProdRev = (uint8_t)((hsd->CID[2] & 0xFF000000U) >> 24U);

    pCID->ProdSN = (((hsd->CID[2] & 0x00FFFFFFU) << 8U) | ((hsd->CID[3] & 0xFF000000U) >> 24U));

    pCID->Reserved1 = (uint8_t)((hsd->CID[3] & 0x00F00000U) >> 20U);

    pCID->ManufactDate = (uint16_t)((hsd->CID[3] & 0x000FFF00U) >> 8U);

    pCID->CID_CRC = (uint8_t)((hsd->CID[3] & 0x000000FEU) >> 1U);

    pCID->Reserved2 = 1U;

    return HAL_OK;
}

/**
 * @brief  Returns information the information of the card which are stored on
 *         the CSD register.
 * @param  hsd: Pointer to SD handle
 * @param  pCSD: Pointer to a HAL_SD_CardCSDTypeDef structure that
 *         contains all CSD register parameters
 * @retval HAL status
 */
HAL_Status_T HAL_SD_GetCardCSD(SD_Handle_T *hsd, HAL_SD_CardCSDTypeDef *pCSD)
{
    uint32_t *CSD = hsd->CSD;

    pCSD->CSDStruct = (uint8_t)((CSD[3] & 0x00C00000U) >> 22U);

    pCSD->SysSpecVersion = (uint8_t)((CSD[3] & 0x003C0000U) >> 18U);

    pCSD->Reserved1 = (uint8_t)((CSD[3] & 0x00030000U) >> 16U);

    pCSD->TAAC = (uint8_t)((CSD[3] & 0x0000FF00U) >> 8U);

    pCSD->NSAC = (uint8_t)(CSD[3] & 0x000000FFU);

    pCSD->MaxBusClkFrec = (uint8_t)((CSD[2] & 0xFF000000U) >> 24);

    pCSD->CardComdClasses = (uint16_t)((CSD[2] & 0x00FFF000U) >> 12U);

    pCSD->RdBlockLen = (uint8_t)((CSD[2] & 0x00000F00U) >> 8U);

    pCSD->PartBlockRead = (uint8_t)((CSD[2] & 0x00000080U) >> 7U);

    pCSD->WrBlockMisalign = (uint8_t)((CSD[2] & 0x00000040U) >> 6U);

    pCSD->RdBlockMisalign = (uint8_t)((CSD[2] & 0x00000020U) >> 5U);

    pCSD->DSRImpl = (uint8_t)((CSD[2] & 0x00000010U) >> 4U);

    pCSD->Reserved2 = 0U; /*!< Reserved */

    if (hsd->SdCard.CardType == CARD_SDSC)
    {
        pCSD->DeviceSize = (((CSD[2] & 0x000003)) << 10 | ((CSD[1] & 0xffc00000)) >> 22U);

        pCSD->MaxRdCurrentVDDMin = (uint8_t)((CSD[1] & 0x00380000U) >> 19U);

        pCSD->MaxRdCurrentVDDMax = (uint8_t)((CSD[1] & 0x00070000U) >> 16U);

        pCSD->MaxWrCurrentVDDMin = (uint8_t)((CSD[1] & 0x0000E000U) >> 13U);

        pCSD->MaxWrCurrentVDDMax = (uint8_t)((CSD[1] & 0x00001C00U) >> 10U);

        pCSD->DeviceSizeMul = (uint8_t)((CSD[1] & 0x00000380U) >> 7U);

        hsd->SdCard.BlockNbr = (pCSD->DeviceSize + 1U);
        hsd->SdCard.BlockNbr *= (1UL << ((pCSD->DeviceSizeMul & 0x07U) + 2U));

        /* SDSC cards may have RdBlockLen of 0xA(block size == 1024), in this case,
        it is necessary to check whether the block size can be changed based on the
        current PartBlockRead value */
        if (pCSD->PartBlockRead == 1U)
        {
            hsd->SdCard.BlockSize = 512;
        }
        else
        {
            /* There is no support for PartBlockRead = 0 at present, CMD16 will be +---------
            used in the driver to set the block size, if PartBlockRead = 0, the call
            to CMD16 will fail, resulting in SD init failure */
            hsd->SdCard.BlockSize = (1UL << (pCSD->RdBlockLen & 0x0FU));
        }

        hsd->SdCard.BlockNbr *= (1UL << (pCSD->RdBlockLen & 0x0FU)) / (hsd->SdCard.BlockSize);

        hsd->SdCard.LogBlockNbr  = (hsd->SdCard.BlockNbr) * ((hsd->SdCard.BlockSize) / 512U);
        hsd->SdCard.LogBlockSize = 512U;
    }
    else if (hsd->SdCard.CardType == CARD_SDHC_SDXC)
    {
        /* Byte 7 */
        pCSD->DeviceSize = (((CSD[1] & 0x3FFFFF00U) >> 8U));

        hsd->SdCard.BlockNbr     = ((pCSD->DeviceSize + 1U) * 1024U);
        hsd->SdCard.LogBlockNbr  = hsd->SdCard.BlockNbr;
        hsd->SdCard.BlockSize    = 512U;
        hsd->SdCard.LogBlockSize = hsd->SdCard.BlockSize;
    }
    else
    {
        /* Clear all the static flags */
        hsd->ErrorCode |= HAL_SD_ERROR_UNSUPPORTED_FEATURE;
        hsd->state = HAL_STATE_READY;
        return HAL_ERROR;
    }

    //  pCSD->EraseGrSize = (uint8_t)(CSD[1] & 0x00000004U );

    //  pCSD->EraseGrMul = (uint8_t)((CSD[0] & 0x00003F80U) >> 7U);

    pCSD->WrProtectGrSize = (uint8_t)((CSD[0] & 0x7F000000) >> 24);

    pCSD->WrProtectGrEnable = (uint8_t)((CSD[0] & 0x00800000U) >> 23U);

    //  pCSD->ManDeflECC = (uint8_t)((CSD[0] & 0x00600000U) >> 21U);

    pCSD->WrSpeedFact = (uint8_t)((CSD[0] & 0x001C0000U) >> 18U);

    pCSD->MaxWrBlockLen = (uint8_t)((CSD[0] & 0x0003C000U) >> 14U);

    pCSD->WriteBlockPaPartial = (uint8_t)((CSD[0] & 0x00002000U) >> 13U);

    //  pCSD->Reserved3 = 0;

    pCSD->ContentProtectAppli = (uint8_t)((CSD[0] & 0x00000100U) >> 8U);

    pCSD->FileFormatGroup = (uint8_t)((CSD[0] & 0x00000080U) >> 7U);

    pCSD->CopyFlag = (uint8_t)((CSD[0] & 0x00000040U) >> 6U);

    pCSD->PermWrProtect = (uint8_t)((CSD[0] & 0x00000020U) >> 5U);

    pCSD->TempWrProtect = (uint8_t)((CSD[0] & 0x00000010U) >> 4U);

    pCSD->FileFormat = (uint8_t)((CSD[0] & 0x0000000CU) >> 2U);

    return HAL_OK;
}

/**
 * @brief  Gets the SD status info.
 * @param  hsd: Pointer to SD handle
 * @param  pStatus: Pointer to the HAL_SD_CardStatusTypeDef structure that
 *         will contain the SD card status information
 * @retval HAL status
 */
HAL_Status_T HAL_SD_GetCardStatus(SD_Handle_T *hsd, HAL_SD_CardStatusTypeDef *pStatus)
{
    uint32_t     sd_status[16];
    uint32_t     errorstate;
    HAL_Status_T status = HAL_OK;

    errorstate = SD_SendSDStatus(hsd, sd_status);
    if (errorstate != HAL_SD_ERROR_NONE)
    {
        /* Clear all the static flags */
        hsd->ErrorCode |= errorstate;
        hsd->state = HAL_STATE_READY;
        status     = HAL_ERROR;
    }
    else
    {
        pStatus->DataBusWidth = (uint8_t)((sd_status[0] & 0xC0U) >> 6U);

        pStatus->SecuredMode = (uint8_t)((sd_status[0] & 0x20U) >> 5U);

        pStatus->CardType = (uint16_t)(((sd_status[0] & 0x00FF0000U) >> 8U) | ((sd_status[0] & 0xFF000000U) >> 24U));

        pStatus->ProtectedAreaSize = (((sd_status[1] & 0xFFU) << 24U) | ((sd_status[1] & 0xFF00U) << 8U) |
                                      ((sd_status[1] & 0xFF0000U) >> 8U) | ((sd_status[1] & 0xFF000000U) >> 24U));

        pStatus->SpeedClass = (uint8_t)(sd_status[2] & 0xFFU);

        pStatus->PerformanceMove = (uint8_t)((sd_status[2] & 0xFF00U) >> 8U);

        pStatus->AllocationUnitSize = (uint8_t)((sd_status[2] & 0xF00000U) >> 20U);

        pStatus->EraseSize = (uint16_t)(((sd_status[2] & 0xFF000000U) >> 16U) | (sd_status[3] & 0xFFU));

        pStatus->EraseTimeout = (uint8_t)((sd_status[3] & 0xFC00U) >> 10U);

        pStatus->EraseOffset = (uint8_t)((sd_status[3] & 0x0300U) >> 8U);

        pStatus->UhsSpeedGrade         = (uint8_t)((sd_status[3] & 0x00F0U) >> 4U);
        pStatus->UhsAllocationUnitSize = (uint8_t)(sd_status[3] & 0x000FU);
        pStatus->VideoSpeedClass       = (uint8_t)((sd_status[4] & 0xFF000000U) >> 24U);
    }

    /* Set Block Size for Card */
    errorstate = SDMMC_CmdBlockLength(hsd->instance, BLOCKSIZE);
    if (errorstate != HAL_SD_ERROR_NONE)
    {
        /* Clear all the static flags */
        hsd->ErrorCode = errorstate;
        hsd->state     = HAL_STATE_READY;
        status         = HAL_ERROR;
    }

    return status;
}

/**
 * @brief  Gets the SD card info.
 * @param  hsd: Pointer to SD handle
 * @param  pCardInfo: Pointer to the HAL_SD_CardInfoTypeDef structure that
 *         will contain the SD card status information
 * @retval HAL status
 */
HAL_Status_T HAL_SD_GetCardInfo(SD_Handle_T *hsd, HAL_SD_CardInfoTypeDef *pCardInfo)
{
    pCardInfo->CardType     = (uint32_t)(hsd->SdCard.CardType);
    pCardInfo->CardVersion  = (uint32_t)(hsd->SdCard.CardVersion);
    pCardInfo->Class        = (uint32_t)(hsd->SdCard.Class);
    pCardInfo->RelCardAdd   = (uint32_t)(hsd->SdCard.RelCardAdd);
    pCardInfo->BlockNbr     = (uint32_t)(hsd->SdCard.BlockNbr);
    pCardInfo->BlockSize    = (uint32_t)(hsd->SdCard.BlockSize);
    pCardInfo->LogBlockNbr  = (uint32_t)(hsd->SdCard.LogBlockNbr);
    pCardInfo->LogBlockSize = (uint32_t)(hsd->SdCard.LogBlockSize);

    return HAL_OK;
}

/**
 * @brief  Returns the current card's status.
 * @param  hsd: Pointer to SD handle
 * @param  pCardStatus: pointer to the buffer that will contain the SD card
 *         status (Card Status register)
 * @retval error state
 */
static uint32_t SD_SendStatus(SD_Handle_T *hsd, uint32_t *pCardStatus)
{
    uint32_t errorstate;

    if (pCardStatus == NULL)
    {
        return HAL_SD_ERROR_PARAM;
    }

    /* Send Status command */
    errorstate = SDMMC_CmdSendStatus(hsd->instance, (uint32_t)(hsd->SdCard.RelCardAdd << 16U), pCardStatus);
    if (errorstate != HAL_SD_ERROR_NONE)
    {
        return errorstate;
    }

    /* Get SD card status */
    *pCardStatus = SDMMC_GetResponse(hsd->instance, SDMMC_RESP0);

    return HAL_SD_ERROR_NONE;
}

/**
 * @brief  Initializes the sd card.
 * @param  hsd: Pointer to SD handle
 * @retval SD Card error state
 */
static uint32_t SD_InitCard(SD_Handle_T *hsd)
{
    HAL_SD_CardCSDTypeDef CSD;
    uint32_t              errorstate;
    uint16_t              sd_rca = 1U;

    if (hsd->SdCard.CardType != CARD_SECURED)
    {
        /* Send CMD2 ALL_SEND_CID */
        errorstate = SDMMC_CmdSendCID(hsd->instance);
        if (errorstate != HAL_SD_ERROR_NONE)
        {
            return errorstate;
        }
        else
        {
            /* Get Card identification number data */
            hsd->CID[0U] = SDMMC_GetResponse(hsd->instance, SDMMC_RESP0);
            hsd->CID[1U] = SDMMC_GetResponse(hsd->instance, SDMMC_RESP1);
            hsd->CID[2U] = SDMMC_GetResponse(hsd->instance, SDMMC_RESP2);
            hsd->CID[3U] = SDMMC_GetResponse(hsd->instance, SDMMC_RESP3);
        }
    }

    if (hsd->SdCard.CardType != CARD_SECURED)
    {
        /* Send CMD3 SET_REL_ADDR with argument 0 */
        /* SD Card publishes its RCA. */
        errorstate = SDMMC_CmdSetRelAdd(hsd->instance, &sd_rca);
        if (errorstate != HAL_SD_ERROR_NONE)
        {
            return errorstate;
        }
    }
    if (hsd->SdCard.CardType != CARD_SECURED)
    {
        /* Get the SD card RCA */
        hsd->SdCard.RelCardAdd = sd_rca;

        /* Send CMD9 SEND_CSD with argument as card's RCA */
        errorstate = SDMMC_CmdSendCSD(hsd->instance, (uint32_t)(hsd->SdCard.RelCardAdd << 16U));
        if (errorstate != HAL_SD_ERROR_NONE)
        {
            return errorstate;
        }
        else
        {
            /* Get Card Specific Data */
            hsd->CSD[0U] = SDMMC_GetResponse(hsd->instance, SDMMC_RESP0);
            hsd->CSD[1U] = SDMMC_GetResponse(hsd->instance, SDMMC_RESP1);
            hsd->CSD[2U] = SDMMC_GetResponse(hsd->instance, SDMMC_RESP2);
            hsd->CSD[3U] = SDMMC_GetResponse(hsd->instance, SDMMC_RESP3);
        }
    }

    /* Get CSD parameters */
    if (HAL_SD_GetCardCSD(hsd, &CSD) != HAL_OK)
    {
        return HAL_SD_ERROR_UNSUPPORTED_FEATURE;
    }

    /* Get the Card Class */
    hsd->SdCard.Class = CSD.CardComdClasses;

    /* Select the Card */
    errorstate = SDMMC_CmdSelDesel(hsd->instance, (uint32_t)(((uint32_t)hsd->SdCard.RelCardAdd) << 16U));
    if (errorstate != HAL_SD_ERROR_NONE)
    {
        return errorstate;
    }

    /* All cards are initialized */
    return HAL_SD_ERROR_NONE;
}

/**
 * @brief  Switches the SD card to High Speed mode.
 *         This API must be used after "Transfer State"
 * @note   This operation should be followed by the configuration
 *         of PLL to have SDMMCCK clock between 50 and 120 MHz
 * @param  hsd: SD handle
 * @retval SD Card error state
 */
uint32_t SD_HighSpeed(SD_Handle_T *hsd)
{
    uint32_t             errorstate = HAL_SD_ERROR_NONE;
    SDIO_DataInitTypeDef transfer_cfg;
    uint32_t             SD_hs[16] = {0};
    uint32_t             Timeout   = HAL_GetTick();

    if (hsd->SdCard.CardSpeed == CARD_NORMAL_SPEED)
    {
        /* Standard Speed Card <= 12.5Mhz  */
        return HAL_SD_ERROR_REQUEST_NOT_APPLICABLE;
    }

    if (hsd->SdCard.CardSpeed == CARD_HIGH_SPEED || hsd->SdCard.CardSpeed == CARD_ULTRA_HIGH_SPEED)
    {
        errorstate = SDMMC_CmdBlockLength(hsd->instance, 64U);

        if (errorstate != HAL_SD_ERROR_NONE)
        {
            return errorstate;
        }

        /* Transfer Init */
        memset(&transfer_cfg, 0, sizeof(SDIO_DataInitTypeDef));
        transfer_cfg.TransferDir = SDMMC_TRANSFER_DIR_TO_SDIO;
        transfer_cfg.BlockEnable = 0;
        transfer_cfg.BlockSize   = 64;
        SDMMC_ConfigData(hsd->instance, transfer_cfg);

        errorstate = SDMMC_CmdSwitch(hsd->instance, SDMMC_SDR25_SWITCH_PATTERN);
        if (errorstate != HAL_SD_ERROR_NONE)
        {
            return errorstate;
        }

        SDMMC_ReadData(hsd->instance, SD_hs, 64);

        /* Test if the switch mode HS is ok */
        if ((((uint8_t *)SD_hs)[13] & 2U) != 2U)
        {
            errorstate = SDMMC_ERROR_UNSUPPORTED_FEATURE;
        }
        else
        {
            hsd->Init.ClockDiv = SDMMC_HIGH_TRANSFER_CLK_DIV;
            (void)SDMMC_Speed_Config(hsd->instance, SDMMC_HIGH_TRANSFER_CLK_DIV);
        }
    }
    return errorstate;
}

/**
 * @brief  Enables wide bus operation for the requested card if supported by
 *         card.
 * @param  hsd: Pointer to SD handle
 * @param  WideMode: Specifies the SD card wide bus mode
 *          This parameter can be one of the following values:
 *            @arg SDMMC_BUS_WIDE_8B: 8-bit data transfer
 *            @arg SDMMC_BUS_WIDE_4B: 4-bit data transfer
 *            @arg SDMMC_BUS_WIDE_1B: 1-bit data transfer
 * @retval HAL status
 */
HAL_Status_T HAL_SD_ConfigWideBusOperation(SD_Handle_T *hsd, uint32_t WideMode)
{
    uint32_t     errorstate;
    HAL_Status_T status = HAL_OK;

    /* Check the sd handle */
    ASSERT_PARAM(hsd);

    /* Change State */
    hsd->state = HAL_STATE_BUSY;

    if (hsd->SdCard.CardType != CARD_SECURED)
    {
        if (WideMode == SDMMC_BUS_WIDE_8B)
        {
            hsd->ErrorCode |= HAL_SD_ERROR_UNSUPPORTED_FEATURE;
        }
        else if (WideMode == SDMMC_BUS_WIDE_4B)
        {
            errorstate = SD_WideBus_Enable(hsd);

            hsd->ErrorCode |= errorstate;
        }
        else if (WideMode == SDMMC_BUS_WIDE_1B)
        {
            errorstate = SD_WideBus_Disable(hsd);

            hsd->ErrorCode |= errorstate;
        }
        else
        {
            /* WideMode is not a valid argument*/
            hsd->ErrorCode |= HAL_SD_ERROR_PARAM;
        }
    }
    else
    {
        /* MMC Card does not support this feature */
        hsd->ErrorCode |= HAL_SD_ERROR_UNSUPPORTED_FEATURE;
    }

    if (hsd->ErrorCode != HAL_SD_ERROR_NONE)
    {
        /* Clear all the static flags */
        hsd->state = HAL_STATE_READY;
        status     = HAL_ERROR;
    }
    else
    {
        /* Configure the SDIO peripheral */
        hsd->Init.BusWide = WideMode;
        (void)SDMMC_BusWide_Config(hsd->instance, WideMode);
    }

    /* Set Block Size for Card */
    errorstate = SDMMC_CmdBlockLength(hsd->instance, BLOCKSIZE);
    if (errorstate != HAL_SD_ERROR_NONE)
    {
        /* Clear all the static flags */
        hsd->ErrorCode |= errorstate;
        status = HAL_ERROR;
    }

    /* Change State */
    hsd->state = HAL_STATE_READY;

    return status;
}

/**
 * @brief  Configure the speed bus mode
 * @param  hsd: Pointer to the SD handle
 * @param  SpeedMode: Specifies the SD card speed bus mode
 *          This parameter can be one of the following values:
 *            @arg SDMMC_SPEED_MODE_AUTO: Max speed mode supported by the card
 *            @arg SDMMC_SPEED_MODE_DEFAULT: Default Speed/SDR12 mode
 *            @arg SDMMC_SPEED_MODE_HIGH: High Speed/SDR25 mode
 *            @arg SDMMC_SPEED_MODE_ULTRA: Ultra high speed mode
 * @retval HAL status
 */

HAL_Status_T HAL_SD_ConfigSpeedBusOperation(SD_Handle_T *hsd, uint32_t SpeedMode)
{
    uint32_t     tickstart;
    uint32_t     errorstate;
    HAL_Status_T status = HAL_OK;

    /* Check the sd handle */
    ASSERT_PARAM(hsd);

    /* Check the parameters */
    ASSERT_PARAM(IS_SDMMC_SPEED_MODE(SpeedMode));
    /* Change State */
    hsd->state = HAL_STATE_BUSY;

#if (USE_SD_TRANSCEIVER != 0U)
    if (hsd->Init.TranceiverPresent == SDMMC_TRANSCEIVER_PRESENT)
    {
        switch (SpeedMode)
        {
            case SDMMC_SPEED_MODE_AUTO:
            {
                if ((hsd->SdCard.CardSpeed == CARD_ULTRA_HIGH_SPEED) ||
                    (hsd->SdCard.CardType == CARD_SDHC_SDXC))
                {
                    hsd->Instance->CLKCR |= SDMMC_CLKCR_BUSSPEED;
                    /* Enable Ultra High Speed */
                    if (SD_UltraHighSpeed(hsd) != HAL_SD_ERROR_NONE)
                    {
                        if (SD_HighSpeed(hsd) != HAL_SD_ERROR_NONE)
                        {
                            hsd->ErrorCode |= HAL_SD_ERROR_UNSUPPORTED_FEATURE;
                            status = HAL_ERROR;
                        }
                    }
                }
                else if (hsd->SdCard.CardSpeed == CARD_HIGH_SPEED)
                {
                    /* Enable High Speed */
                    if (SD_HighSpeed(hsd) != HAL_SD_ERROR_NONE)
                    {
                        hsd->ErrorCode |= HAL_SD_ERROR_UNSUPPORTED_FEATURE;
                        status = HAL_ERROR;
                    }
                }
                else
                {
                    /*Nothing to do, Use defaultSpeed */
                }
                break;
            }
            case SDMMC_SPEED_MODE_ULTRA:
            {
                if ((hsd->SdCard.CardSpeed == CARD_ULTRA_HIGH_SPEED) ||
                    (hsd->SdCard.CardType == CARD_SDHC_SDXC))
                {
                    /* Enable UltraHigh Speed */
                    if (SD_UltraHighSpeed(hsd) != HAL_SD_ERROR_NONE)
                    {
                        hsd->ErrorCode |= HAL_SD_ERROR_UNSUPPORTED_FEATURE;
                        status = HAL_ERROR;
                    }
                    hsd->Instance->CLKCR |= SDMMC_CLKCR_BUSSPEED;
                }
                else
                {
                    hsd->ErrorCode |= HAL_SD_ERROR_UNSUPPORTED_FEATURE;
                    status = HAL_ERROR;
                }
                break;
            }
            case SDMMC_SPEED_MODE_DDR:
            {
                if ((hsd->SdCard.CardSpeed == CARD_ULTRA_HIGH_SPEED) ||
                    (hsd->SdCard.CardType == CARD_SDHC_SDXC))
                {
                    /* Enable DDR Mode*/
                    if (SD_DDR_Mode(hsd) != HAL_SD_ERROR_NONE)
                    {
                        hsd->ErrorCode |= HAL_SD_ERROR_UNSUPPORTED_FEATURE;
                        status = HAL_ERROR;
                    }
                    hsd->Instance->CLKCR |= SDMMC_CLKCR_BUSSPEED | SDMMC_CLKCR_DDR;
                }
                else
                {
                    hsd->ErrorCode |= HAL_SD_ERROR_UNSUPPORTED_FEATURE;
                    status = HAL_ERROR;
                }
                break;
            }
            case SDMMC_SPEED_MODE_HIGH:
            {
                if ((hsd->SdCard.CardSpeed == CARD_ULTRA_HIGH_SPEED) ||
                    (hsd->SdCard.CardSpeed == CARD_HIGH_SPEED) ||
                    (hsd->SdCard.CardType == CARD_SDHC_SDXC))
                {
                    /* Enable High Speed */
                    if (SD_HighSpeed(hsd) != HAL_SD_ERROR_NONE)
                    {
                        hsd->ErrorCode |= HAL_SD_ERROR_UNSUPPORTED_FEATURE;
                        status = HAL_ERROR;
                    }
                }
                else
                {
                    hsd->ErrorCode |= HAL_SD_ERROR_UNSUPPORTED_FEATURE;
                    status = HAL_ERROR;
                }
                break;
            }
            case SDMMC_SPEED_MODE_DEFAULT:
                break;
            default:
                hsd->ErrorCode |= HAL_SD_ERROR_PARAM;
                status = HAL_ERROR;
                break;
        }
    }
    else
    {
        switch (SpeedMode)
        {
            case SDMMC_SPEED_MODE_AUTO:
            {
                if ((hsd->SdCard.CardSpeed == CARD_ULTRA_HIGH_SPEED) ||
                    (hsd->SdCard.CardSpeed == CARD_HIGH_SPEED) ||
                    (hsd->SdCard.CardType == CARD_SDHC_SDXC))
                {
                    /* Enable High Speed */
                    if (SD_HighSpeed(hsd) != HAL_SD_ERROR_NONE)
                    {
                        hsd->ErrorCode |= HAL_SD_ERROR_UNSUPPORTED_FEATURE;
                        status = HAL_ERROR;
                    }
                }
                else
                {
                    /*Nothing to do, Use defaultSpeed */
                }
                break;
            }
            case SDMMC_SPEED_MODE_HIGH:
            {
                if ((hsd->SdCard.CardSpeed == CARD_ULTRA_HIGH_SPEED) ||
                    (hsd->SdCard.CardSpeed == CARD_HIGH_SPEED) ||
                    (hsd->SdCard.CardType == CARD_SDHC_SDXC))
                {
                    /* Enable High Speed */
                    if (SD_HighSpeed(hsd) != HAL_SD_ERROR_NONE)
                    {
                        hsd->ErrorCode |= HAL_SD_ERROR_UNSUPPORTED_FEATURE;
                        status = HAL_ERROR;
                    }
                }
                else
                {
                    hsd->ErrorCode |= HAL_SD_ERROR_UNSUPPORTED_FEATURE;
                    status = HAL_ERROR;
                }
                break;
            }
            case SDMMC_SPEED_MODE_DEFAULT:
                break;
            case SDMMC_SPEED_MODE_ULTRA: /*not valid without transceiver*/
            default:
                hsd->ErrorCode |= HAL_SD_ERROR_PARAM;
                status = HAL_ERROR;
                break;
        }
    }
#else
    switch (SpeedMode)
    {
        case SDMMC_SPEED_MODE_AUTO:
        {
            if ((hsd->SdCard.CardSpeed == CARD_ULTRA_HIGH_SPEED) ||
                (hsd->SdCard.CardSpeed == CARD_HIGH_SPEED) ||
                (hsd->SdCard.CardType == CARD_SDHC_SDXC))
            {
                /* Enable High Speed */
                if (SD_HighSpeed(hsd) != HAL_SD_ERROR_NONE)
                {
                    hsd->ErrorCode |= HAL_SD_ERROR_UNSUPPORTED_FEATURE;
                    status = HAL_ERROR;
                }
            }
            else
            {
                /*Use defaultSpeed */
                hsd->Init.ClockDiv = SDMMC_TRANSFER_CLK_DIV;
                (void)SDMMC_Speed_Config(hsd->instance, SDMMC_TRANSFER_CLK_DIV);
            }
            break;
        }
        case SDMMC_SPEED_MODE_HIGH:
        {
            if ((hsd->SdCard.CardSpeed == CARD_ULTRA_HIGH_SPEED) ||
                (hsd->SdCard.CardSpeed == CARD_HIGH_SPEED) ||
                (hsd->SdCard.CardType == CARD_SDHC_SDXC))
            {
                /* Enable High Speed */
                if (SD_HighSpeed(hsd) != HAL_SD_ERROR_NONE)
                {
                    hsd->ErrorCode |= HAL_SD_ERROR_UNSUPPORTED_FEATURE;
                    status = HAL_ERROR;
                }
            }
            else
            {
                hsd->ErrorCode |= HAL_SD_ERROR_UNSUPPORTED_FEATURE;
                status = HAL_ERROR;
            }
            break;
        }
        case SDMMC_SPEED_MODE_DEFAULT:
            hsd->Init.ClockDiv = SDMMC_TRANSFER_CLK_DIV;
            (void)SDMMC_Speed_Config(hsd->instance, SDMMC_TRANSFER_CLK_DIV);
            break;
        case SDMMC_SPEED_MODE_ULTRA: /*not valid without transceiver*/
        default:
            hsd->ErrorCode |= HAL_SD_ERROR_PARAM;
            status = HAL_ERROR;
            break;
    }
#endif /* USE_SD_TRANSCEIVER */

    /* Verify that SD card is ready to use after Speed mode switch*/
    tickstart = HAL_GetTick();
    while ((HAL_SD_GetCardState(hsd) != HAL_SD_CARD_TRANSFER))
    {
        if ((HAL_GetTick() - tickstart) >= SDMMC_DATATIMEOUT)
        {
            hsd->ErrorCode = HAL_SD_ERROR_TIMEOUT;
            hsd->state     = HAL_STATE_READY;
            return HAL_TIMEOUT;
        }
    }

    /* Set Block Size for Card */
    errorstate = SDMMC_CmdBlockLength(hsd->instance, BLOCKSIZE);
    if (errorstate != HAL_SD_ERROR_NONE)
    {
        /* Clear all the static flags */
        hsd->ErrorCode |= errorstate;
        status = HAL_ERROR;
    }

    /* Change State */
    hsd->state = HAL_STATE_READY;
    return status;
}

/**
 * @brief return the SD state
 * @param hsd: Pointer to sd handle
 * @retval HAL state
 */
HAL_State_T HAL_SD_GetState(SD_Handle_T *hsd)
{
    /* Check the sd handle */
    ASSERT_PARAM(hsd);

    return hsd->state;
}

/**
 * @brief  Return the SD error code
 * @param  hsd : Pointer to a SD_HandleTypeDef structure that contains
 *              the configuration information.
 * @retval SD Error Code
 */
uint32_t HAL_SD_GetError(SD_Handle_T *hsd)
{
    /* Check the sd handle */
    ASSERT_PARAM(hsd);

    return hsd->ErrorCode;
}

/**
 * @brief  Gets the current sd card data state.
 * @param  hsd: pointer to SD handle
 * @retval Card state
 */
HAL_SD_CardState_T HAL_SD_GetCardState(SD_Handle_T *hsd)
{
    uint32_t cardstate;
    uint32_t errorstate;
    uint32_t resp1 = 0;

    /* Check the sd handle */
    ASSERT_PARAM(hsd);

    errorstate = SD_SendStatus(hsd, &resp1);
    if (errorstate != HAL_SD_ERROR_NONE)
    {
        hsd->ErrorCode |= errorstate;
    }

    cardstate = ((resp1 >> 9U) & 0x0FU);

    return (HAL_SD_CardState_T)cardstate;
}
#if (configUSE_SD_BLOCKING == 1)
/**
 * @brief  Reads block(s) from a specified address in a card. The Data transfer
 *         is managed by polling mode.
 * @note   This API should be followed by a check on the card state through
 *         HAL_SD_GetCardState().
 * @param  hsd: Pointer to SD handle
 * @param  pData: pointer to the buffer that will contain the received data
 * @param  BlockAdd: Block Address from where data is to be read
 * @param  NumberOfBlocks: Number of SD blocks to read
 * @param  Timeout: Specify timeout value
 * @retval HAL status
 */
HAL_Status_T HAL_SD_ReadBlocks(SD_Handle_T *hsd, uint8_t *pData, uint32_t BlockAdd, uint32_t NumberOfBlocks, uint32_t Timeout)
{
    uint32_t             errorstate;
    SDIO_DataInitTypeDef transfer_cfg;
    uint32_t             dataremaining;
    uint32_t             trans_sz;
    uint32_t             tickstart = HAL_GetTick();
    uint32_t             len;
    uint32_t             add = BlockAdd;

    /* Check the sd handle */
    ASSERT_PARAM(hsd);

    if (NULL == pData)
    {
        hsd->ErrorCode |= HAL_SD_ERROR_PARAM;
        return HAL_ERROR;
    }

    if (NumberOfBlocks == 0)
    {
        hsd->ErrorCode |= HAL_SD_ERROR_PARAM;
        return HAL_ERROR;
    }

    if (hsd->state == HAL_STATE_READY)
    {
        hsd->ErrorCode = HAL_SD_ERROR_NONE;

        if ((add + NumberOfBlocks) > (hsd->SdCard.LogBlockNbr))
        {
            hsd->ErrorCode |= HAL_SD_ERROR_ADDR_OUT_OF_RANGE;
            return HAL_ERROR;
        }

        hsd->state = HAL_STATE_BUSY;

        if (hsd->SdCard.CardType != CARD_SDHC_SDXC)
        {
            add *= 512U;
        }

        memset(&transfer_cfg, 0, sizeof(SDIO_DataInitTypeDef));
        transfer_cfg.TransferDir = SDMMC_TRANSFER_DIR_TO_SDIO;
        transfer_cfg.BlockSize   = 512;

        /* Read block(s) in polling mode */
        if (NumberOfBlocks > 1U)
        {
            hsd->Context                  = SD_CONTEXT_READ_MULTIPLE_BLOCK;
            transfer_cfg.MultiBlockEnable = 1;
            transfer_cfg.BlockEnable      = 1;
            transfer_cfg.BlockCount       = NumberOfBlocks;
#if (configUSE_SD_AUTOCMD_ENABLE == 1)
            if (hsd->SdCard.Cmd23Support)
                transfer_cfg.AutoCMD23Enable = 1;
            else
                transfer_cfg.AutoCMD12Enable = 1;
#endif
            SDMMC_ConfigData(hsd->instance, transfer_cfg);

            /* Read Multi Block command */
            errorstate = SDMMC_CmdReadMultiBlock(hsd->instance, add);
        }
        else
        {
            hsd->Context                  = SD_CONTEXT_READ_SINGLE_BLOCK;
            transfer_cfg.MultiBlockEnable = 0;
            SDMMC_ConfigData(hsd->instance, transfer_cfg);

            /* Read Single Block command */
            errorstate = SDMMC_CmdReadSingleBlock(hsd->instance, add);
        }

        if (errorstate != HAL_SD_ERROR_NONE)
        {
            /* Clear all the static flags */
            //      __HAL_SD_CLEAR_FLAG(hsd, SDMMC_STATIC_FLAGS);
            hsd->ErrorCode |= errorstate;
            hsd->state   = HAL_STATE_READY;
            hsd->Context = SD_CONTEXT_NONE;
            return HAL_ERROR;
        }

        dataremaining = BLOCKSIZE * NumberOfBlocks;
        trans_sz      = BLOCKSIZE;

        while (dataremaining)
        {
            /* Wait write buffer ready
             * This bit will be set when at least one block( 512 bytes ) of space is avalilable for read
             */
            while (!__SDMMC_GET_STA(SN_SDIO, SDMMC_STA_BUFREN) && !__SDMMC_GET_FLAG(SN_SDIO, SDMMC_INTR_STS_ERR))
            {
            };

            if (__SDMMC_GET_FLAG(SN_SDIO, SDMMC_INTR_STS_ERR))
            {
                /* Data CRC Error */
                if (__SDMMC_GET_FLAG(SN_SDIO, SDMMC_INTR_STS_ERR_DATA_CRC))
                {
                    hsd->ErrorCode |= HAL_SD_ERROR_DATA_CRC_FAIL;
                }
                /* Data End bit Error */
                if (__SDMMC_GET_FLAG(SN_SDIO, SDMMC_INTR_STS_ERR_DATA_TIMEOUT))
                {
                    hsd->ErrorCode |= SDMMC_ERROR_DATA_TIMEOUT;
                }

                /* Software Reset for DAT line */
                if (__SDMMC_GET_FLAG(SN_SDIO, SDMMC_INTR_STS_ERR_DAT_LINE))
                {
                    __SDMMC_CLEAR_FLAG(SN_SDIO, SDMMC_INTR_STS_ERR_DAT_LINE);

                    HAL_REG_SBIT(SN_SDIO->SWRST, SDMMC_SOFTRST_DAT);

                    while (HAL_REG_RBIT(SN_SDIO->SWRST, SDMMC_SOFTRST_DAT))
                        ;

                    /* Send stop transmission command */
                    errorstate = SDMMC_CmdStopTransfer(hsd->instance);
                    if (errorstate != HAL_SD_ERROR_NONE)
                    {
                        hsd->ErrorCode |= errorstate;
                        hsd->state   = HAL_STATE_READY;
                        hsd->Context = SD_CONTEXT_NONE;
                        return HAL_ERROR;
                    }
                }

                hsd->state   = HAL_STATE_READY;
                hsd->Context = SD_CONTEXT_NONE;
                return HAL_ERROR;
            }

            //            hsd->instance->RIS &= SDMMC_INTR_STS_BUFF_READ_READY;

            len = (dataremaining < trans_sz) ? dataremaining : trans_sz;
            dataremaining -= len;

            while (len)
            {
                *(uint32_t *)pData = SDMMC_ReadFIFO(hsd->instance);
                len -= 4;
                pData += 4;
            }

            if (((HAL_GetTick() - tickstart) >= Timeout) || (Timeout == 0U))
            {
                hsd->ErrorCode |= HAL_SD_ERROR_TIMEOUT;
                hsd->state   = HAL_STATE_READY;
                hsd->Context = SD_CONTEXT_NONE;

                /* Data Line Reset */
                __SDMMC_SWRST_SET(SN_SDIO, SDMMC_SOFTRST_DAT);
                while (__SDMMC_SWRST_GET_FLAG(SN_SDIO, SDMMC_SOFTRST_DAT))
                    ;
                ;
                if (hsd->SdCard.CardType != CARD_SECURED)
                {
                    /* Send stop transmission command */
                    errorstate = SDMMC_CmdStopTransfer(hsd->instance);
                    if (errorstate != HAL_SD_ERROR_NONE)
                    {
                        hsd->ErrorCode |= errorstate;
                        hsd->state   = HAL_STATE_READY;
                        hsd->Context = SD_CONTEXT_NONE;
                        return HAL_ERROR;
                    }
                }
                return HAL_ERROR;
            }
        }

        /* Wait data transfer complete */
        while (!(__SDMMC_GET_FLAG(hsd->instance, SDMMC_INTR_STS_TXR_COMPLETE)))
            ;

        __SDMMC_CLEAR_FLAG(hsd->instance, SDMMC_INTR_STS_TXR_COMPLETE);

#if (configUSE_SD_AUTOCMD_ENABLE == 0)
        if (NumberOfBlocks > 1U)
        {
            if (hsd->SdCard.CardType != CARD_SECURED)
            {
                /* Send stop transmission command */
                errorstate = SDMMC_CmdStopTransfer(hsd->instance);
                if (errorstate != HAL_SD_ERROR_NONE)
                {
                    hsd->ErrorCode |= errorstate;
                    hsd->state   = HAL_STATE_READY;
                    hsd->Context = SD_CONTEXT_NONE;
                    return HAL_ERROR;
                }
            }
        }
#endif

        hsd->state = HAL_STATE_READY;

        return HAL_OK;
    }
    else
    {
        hsd->ErrorCode |= HAL_SD_ERROR_BUSY;
        return HAL_ERROR;
    }
}

/**
 * @brief  Allows to write block(s) to a specified address in a card. The Data
 *         transfer is managed by polling mode.
 * @note   This API should be followed by a check on the card state through
 *         HAL_SD_GetCardState().
 * @param  hsd: Pointer to SD handle
 * @param  pData: pointer to the buffer that will contain the data to transmit
 * @param  BlockAdd: Block Address where data will be written
 * @param  NumberOfBlocks: Number of SD blocks to write
 * @param  Timeout: Specify timeout value
 * @retval HAL status
 */
HAL_Status_T HAL_SD_WriteBlocks(SD_Handle_T *hsd, uint8_t *pData, uint32_t BlockAdd, uint32_t NumberOfBlocks, uint32_t Timeout)
{
    uint32_t             errorstate;
    SDIO_DataInitTypeDef transfer_cfg;
    uint32_t             dataremaining;
    uint32_t             trans_sz;
    uint32_t             tickstart = HAL_GetTick();
    uint32_t             len;
    uint32_t             add = BlockAdd;

    /* Check the sd handle */
    ASSERT_PARAM(hsd);

    if (NULL == pData)
    {
        hsd->ErrorCode |= HAL_SD_ERROR_PARAM;
        return HAL_ERROR;
    }

    if (NumberOfBlocks == 0)
    {
        hsd->ErrorCode |= HAL_SD_ERROR_PARAM;
        return HAL_ERROR;
    }

    if (hsd->state == HAL_STATE_READY)
    {
        hsd->ErrorCode = HAL_SD_ERROR_NONE;

        if ((add + NumberOfBlocks) > (hsd->SdCard.LogBlockNbr))
        {
            hsd->ErrorCode |= HAL_SD_ERROR_ADDR_OUT_OF_RANGE;
            return HAL_ERROR;
        }

        hsd->state = HAL_STATE_BUSY;

        if (hsd->SdCard.CardType != CARD_SDHC_SDXC)
        {
            add *= 512U;
        }

        memset(&transfer_cfg, 0, sizeof(SDIO_DataInitTypeDef));
        transfer_cfg.TransferDir = SDMMC_TRANSFER_DIR_TO_CARD;
        transfer_cfg.BlockSize   = 512;

        /* Read block(s) in polling mode */
        if (NumberOfBlocks > 1U)
        {
            hsd->Context                  = SD_CONTEXT_WRITE_MULTIPLE_BLOCK;
            transfer_cfg.MultiBlockEnable = 1;
            transfer_cfg.BlockEnable      = 1;
            transfer_cfg.BlockCount       = NumberOfBlocks;
#if (configUSE_SD_AUTOCMD_ENABLE == 1)
            if (hsd->SdCard.Cmd23Support)
                transfer_cfg.AutoCMD23Enable = 1;
            else
                transfer_cfg.AutoCMD12Enable = 1;
#endif
            SDMMC_ConfigData(hsd->instance, transfer_cfg);

            /* Write Multi Block command */
            errorstate = SDMMC_CmdWriteMultiBlock(hsd->instance, add);
        }
        else
        {
            hsd->Context                  = SD_CONTEXT_WRITE_SINGLE_BLOCK;
            transfer_cfg.MultiBlockEnable = 0;
            SDMMC_ConfigData(hsd->instance, transfer_cfg);

            /* Read Single Block command */
            errorstate = SDMMC_CmdWriteSingleBlock(hsd->instance, add);
        }

        if (errorstate != HAL_SD_ERROR_NONE)
        {
            hsd->ErrorCode |= errorstate;
            hsd->state   = HAL_STATE_READY;
            hsd->Context = SD_CONTEXT_NONE;
            return HAL_ERROR;
        }

        dataremaining = BLOCKSIZE * NumberOfBlocks;
        trans_sz      = BLOCKSIZE;

        while (dataremaining)
        {
            /* Wait write buffer ready
             * This bit will be set when at least one block( 512 bytes ) of space is avalilable for written
             */
            while (!__SDMMC_GET_STA(hsd->instance, SDMMC_STA_BUFWEN) && !__SDMMC_GET_FLAG(hsd->instance, SDMMC_INTR_STS_ERR))
                ;

            /* Data CRC Error */
            if (__SDMMC_GET_FLAG(SN_SDIO, SDMMC_INTR_STS_ERR_DATA_CRC))
            {
                hsd->ErrorCode |= HAL_SD_ERROR_DATA_CRC_FAIL;
            }
            /* Data End bit Error */
            if (__SDMMC_GET_FLAG(SN_SDIO, SDMMC_INTR_STS_ERR_DATA_TIMEOUT))
            {
                hsd->ErrorCode |= SDMMC_ERROR_DATA_TIMEOUT;
            }

            if (__SDMMC_GET_FLAG(SN_SDIO, SDMMC_INTR_STS_ERR))
            {
                /* Software Reset for DAT line */
                if (__SDMMC_GET_FLAG(SN_SDIO, SDMMC_INTR_STS_ERR_DAT_LINE))
                {
                    __SDMMC_CLEAR_FLAG(SN_SDIO, SDMMC_INTR_STS_ERR_DAT_LINE);

                    HAL_REG_SBIT(SN_SDIO->SWRST, SDMMC_SOFTRST_DAT);

                    while (HAL_REG_RBIT(SN_SDIO->SWRST, SDMMC_SOFTRST_DAT))
                        ;

                    /* Send stop transmission command */
                    errorstate = SDMMC_CmdStopTransfer(hsd->instance);
                    if (errorstate != HAL_SD_ERROR_NONE)
                    {
                        hsd->ErrorCode |= errorstate;
                        hsd->state   = HAL_STATE_READY;
                        hsd->Context = SD_CONTEXT_NONE;
                        return HAL_ERROR;
                    }
                }

                hsd->state   = HAL_STATE_READY;
                hsd->Context = SD_CONTEXT_NONE;
                return HAL_ERROR;
            }

            //            hsd->instance->RIS &= SDMMC_INTR_STS_BUFF_WRITE_READY;

            len = (dataremaining < trans_sz) ? dataremaining : trans_sz;
            dataremaining -= len;

            /* Each loop send 512 bytes for one block */
            while (len)
            {
                SDMMC_WriteFIFO(hsd->instance, (uint32_t *)pData);
                len -= 4;
                pData += 4;
            }

            if (((HAL_GetTick() - tickstart) >= Timeout) || (Timeout == 0U))
            {
                hsd->ErrorCode |= HAL_SD_ERROR_TIMEOUT;
                hsd->state   = HAL_STATE_READY;
                hsd->Context = SD_CONTEXT_NONE;

                /* Data Line Reset */
                __SDMMC_SWRST_SET(SN_SDIO, SDMMC_SOFTRST_DAT);
                while (__SDMMC_SWRST_GET_FLAG(SN_SDIO, SDMMC_SOFTRST_DAT))
                    ;
                ;
                if (hsd->SdCard.CardType != CARD_SECURED)
                {
                    /* Send stop transmission command */
                    errorstate = SDMMC_CmdStopTransfer(hsd->instance);
                    if (errorstate != HAL_SD_ERROR_NONE)
                    {
                        hsd->ErrorCode |= errorstate;
                        hsd->state   = HAL_STATE_READY;
                        hsd->Context = SD_CONTEXT_NONE;
                        return HAL_ERROR;
                    }
                }
                return HAL_ERROR;
            }
        }

        /* Wait data transfer complete */
        while (__SDMMC_GET_STA(hsd->instance, SDMMC_STA_CMDINBDAT))
            ;

        /* Wait data transfer complete */
        while (!(__SDMMC_GET_FLAG(hsd->instance, SDMMC_INTR_STS_TXR_COMPLETE)))
            ;

        __SDMMC_CLEAR_FLAG(hsd->instance, SDMMC_INTR_STS_TXR_COMPLETE);

#if (configUSE_SD_AUTOCMD_ENABLE == 0)
        if (NumberOfBlocks > 1U)
        {
            if (hsd->SdCard.CardType != CARD_SECURED)
            {
                /* Send stop transmission command */
                errorstate = SDMMC_CmdStopTransfer(hsd->instance);
                if (errorstate != HAL_SD_ERROR_NONE)
                {
                    hsd->ErrorCode |= errorstate;
                    hsd->state   = HAL_STATE_READY;
                    hsd->Context = SD_CONTEXT_NONE;
                    return HAL_ERROR;
                }
            }
        }
#endif

        hsd->state = HAL_STATE_READY;

        return HAL_OK;
    }
    else
    {
        hsd->ErrorCode |= HAL_SD_ERROR_BUSY;
        return HAL_ERROR;
    }
}

#endif // configUSE_SD_BLOCKING

#if (configUSE_SD_NONBLOCKING_IT == 1)
/**
 * @brief  Reads block(s) from a specified address in a card. The Data transfer
 *         is managed in interrupt mode.
 * @note   This API should be followed by a check on the card state through
 *         HAL_SD_GetCardState().
 * @note   You could also check the IT transfer process through the SD Rx
 *         interrupt event.
 * @param  hsd: Pointer to SD handle
 * @param  pData: Pointer to the buffer that will contain the received data
 * @param  BlockAdd: Block Address from where data is to be read
 * @param  NumberOfBlocks: Number of blocks to read.
 * @retval HAL status
 */
HAL_Status_T HAL_SD_ReadBlocks_IT(SD_Handle_T *hsd, uint8_t *pData, uint32_t BlockAdd, uint32_t NumberOfBlocks)
{
    uint32_t             errorstate;
    SDIO_DataInitTypeDef transfer_cfg;

    /* Check the sd handle */
    ASSERT_PARAM(hsd);

    uint32_t add = BlockAdd;

    if (NULL == pData)
    {
        hsd->ErrorCode |= HAL_SD_ERROR_PARAM;
        return HAL_ERROR;
    }

    if (NumberOfBlocks == 0)
    {
        hsd->ErrorCode |= HAL_SD_ERROR_PARAM;
        return HAL_ERROR;
    }

    if (hsd->state == HAL_STATE_READY)
    {
        hsd->ErrorCode = HAL_SD_ERROR_NONE;

        if ((add + NumberOfBlocks) > (hsd->SdCard.LogBlockNbr))
        {
            hsd->ErrorCode |= HAL_SD_ERROR_ADDR_OUT_OF_RANGE;
            return HAL_ERROR;
        }

        hsd->state = HAL_STATE_BUSY;

        hsd->pRxBuffPtr = pData;
        hsd->RxXferSize = BLOCKSIZE * NumberOfBlocks;

        /* SD IRQ Gen Config */
        __SDMMC_ENABLE_IT_SIG(hsd->instance, SDMMC_IT_TRANSCPLIE | SDMMC_IT_BUFRIE | SDMMC_IT_CURLMTEIE | SDMMC_IT_CMDTOEIE | SDMMC_IT_CMDCRCEIE | SDMMC_IT_CMDEBEIE | SDMMC_IT_CMDINDEIE | SDMMC_IT_DATTOEIE | SDMMC_IT_DATCRCEIE | SDMMC_IT_DATEBEIE | SDMMC_IT_CURLMTEIE | SDMMC_IT_ATCMDEIE | SDMMC_IT_ADMAEIE);

        if (hsd->SdCard.CardType != CARD_SDHC_SDXC)
        {
            add *= 512U;
        }

        memset(&transfer_cfg, 0, sizeof(SDIO_DataInitTypeDef));
        transfer_cfg.TransferDir = SDMMC_TRANSFER_DIR_TO_SDIO;
        transfer_cfg.BlockSize   = 512;

        /* Read block(s) in polling mode */
        if (NumberOfBlocks > 1U)
        {
            hsd->Context                  = (SD_CONTEXT_READ_MULTIPLE_BLOCK | SD_CONTEXT_IT);
            transfer_cfg.MultiBlockEnable = 1;
            transfer_cfg.BlockEnable      = 1;
            transfer_cfg.BlockCount       = NumberOfBlocks;
#if (configUSE_SD_AUTOCMD_ENABLE == 1)
            if (hsd->SdCard.Cmd23Support)
                transfer_cfg.AutoCMD23Enable = 1;
            else
                transfer_cfg.AutoCMD12Enable = 1;
#endif
            SDMMC_ConfigData(hsd->instance, transfer_cfg);

            /* Read Multi Block command */
            errorstate = SDMMC_CmdReadMultiBlock(hsd->instance, add);
        }
        else
        {
            hsd->Context                  = (SD_CONTEXT_READ_SINGLE_BLOCK | SD_CONTEXT_IT);
            transfer_cfg.MultiBlockEnable = 0;
            SDMMC_ConfigData(hsd->instance, transfer_cfg);

            /* Read Single Block command */
            errorstate = SDMMC_CmdReadSingleBlock(hsd->instance, add);
        }

        if (errorstate != HAL_SD_ERROR_NONE)
        {
            hsd->ErrorCode |= errorstate;
            hsd->state   = HAL_STATE_READY;
            hsd->Context = SD_CONTEXT_NONE;
            return HAL_ERROR;
        }

        return HAL_OK;
    }
    else
    {
        hsd->ErrorCode |= HAL_SD_ERROR_BUSY;
        return HAL_ERROR;
    }
}

/**
 * @brief  Writes block(s) to a specified address in a card. The Data transfer
 *         is managed in interrupt mode.
 * @note   This API should be followed by a check on the card state through
 *         HAL_SD_GetCardState().
 * @note   You could also check the IT transfer process through the SD Tx
 *         interrupt event.
 * @param  hsd: Pointer to SD handle
 * @param  pData: Pointer to the buffer that will contain the data to transmit
 * @param  BlockAdd: Block Address where data will be written
 * @param  NumberOfBlocks: Number of blocks to write
 * @retval HAL status
 */
HAL_Status_T HAL_SD_WriteBlocks_IT(SD_Handle_T *hsd, uint8_t *pData, uint32_t BlockAdd, uint32_t NumberOfBlocks)
{
    uint32_t             errorstate;
    SDIO_DataInitTypeDef transfer_cfg;
    uint32_t             add = BlockAdd;

    /* Check the sd handle */
    ASSERT_PARAM(hsd);

    if (NULL == pData)
    {
        hsd->ErrorCode |= HAL_SD_ERROR_PARAM;
        return HAL_ERROR;
    }

    if (NumberOfBlocks == 0)
    {
        hsd->ErrorCode |= HAL_SD_ERROR_PARAM;
        return HAL_ERROR;
    }

    if (hsd->state == HAL_STATE_READY)
    {
        hsd->ErrorCode = HAL_SD_ERROR_NONE;

        if ((add + NumberOfBlocks) > (hsd->SdCard.LogBlockNbr))
        {
            hsd->ErrorCode |= HAL_SD_ERROR_ADDR_OUT_OF_RANGE;
            return HAL_ERROR;
        }

        hsd->state = HAL_STATE_BUSY;

        hsd->pTxBuffPtr = pData;
        hsd->TxXferSize = BLOCKSIZE * NumberOfBlocks;

        /* SD IRQ Gen Config */
        __SDMMC_ENABLE_IT_SIG(hsd->instance, SDMMC_IT_TRANSCPLIE | SDMMC_IT_BUFWIE | SDMMC_IT_CURLMTEIE | SDMMC_IT_CMDTOEIE | SDMMC_IT_CMDCRCEIE | SDMMC_IT_CMDEBEIE | SDMMC_IT_CMDINDEIE | SDMMC_IT_DATTOEIE | SDMMC_IT_DATCRCEIE | SDMMC_IT_DATEBEIE | SDMMC_IT_CURLMTEIE | SDMMC_IT_ATCMDEIE | SDMMC_IT_ADMAEIE);

        if (hsd->SdCard.CardType != CARD_SDHC_SDXC)
        {
            add *= 512U;
        }

        memset(&transfer_cfg, 0, sizeof(SDIO_DataInitTypeDef));
        transfer_cfg.TransferDir = SDMMC_TRANSFER_DIR_TO_CARD;
        transfer_cfg.BlockSize   = BLOCKSIZE;

        /* Read block(s) in polling mode */
        if (NumberOfBlocks > 1U)
        {
            hsd->Context                  = (SD_CONTEXT_WRITE_MULTIPLE_BLOCK | SD_CONTEXT_IT);
            transfer_cfg.MultiBlockEnable = 1;
            transfer_cfg.BlockEnable      = 1;
            transfer_cfg.BlockCount       = NumberOfBlocks;
#if (configUSE_SD_AUTOCMD_ENABLE == 1)
            if (hsd->SdCard.Cmd23Support)
                transfer_cfg.AutoCMD23Enable = 1;
            else
                transfer_cfg.AutoCMD12Enable = 1;
#endif
            SDMMC_ConfigData(hsd->instance, transfer_cfg);

            /* Write Multi Block command */
            errorstate = SDMMC_CmdWriteMultiBlock(hsd->instance, add);
        }
        else
        {
            hsd->Context                  = (SD_CONTEXT_WRITE_SINGLE_BLOCK | SD_CONTEXT_IT);
            transfer_cfg.MultiBlockEnable = 0;
            SDMMC_ConfigData(hsd->instance, transfer_cfg);

            /* Read Single Block command */
            errorstate = SDMMC_CmdWriteSingleBlock(hsd->instance, add);
        }

        if (errorstate != HAL_SD_ERROR_NONE)
        {
            hsd->ErrorCode |= errorstate;
            hsd->state   = HAL_STATE_READY;
            hsd->Context = SD_CONTEXT_NONE;
            return HAL_ERROR;
        }

        return HAL_OK;
    }
    else
    {
        hsd->ErrorCode |= HAL_SD_ERROR_BUSY;
        return HAL_ERROR;
    }
}

#endif // configUSE_SD_NONBLOCKING_IT

#if (configUSE_SD_NONBLOCKING_DMA == 1)
/**
 * @brief  Reads block(s) from a specified address in a card. The Data transfer
 *         is managed by DMA mode.
 * @note   This API should be followed by a check on the card state through
 *         HAL_SD_GetCardState().
 * @note   You could also check the DMA transfer process through the SD Rx
 *         interrupt event.
 * @param  hsd: Pointer SD handle
 * @param  pData: Pointer to the buffer that will contain the received data
 * @param  BlockAdd: Block Address from where data is to be read
 * @param  NumberOfBlocks: Number of blocks to read.
 * @retval HAL status
 */
HAL_Status_T HAL_SD_ReadBlocks_SDMA(SD_Handle_T *hsd, uint8_t *pData, uint32_t BlockAdd, uint32_t NumberOfBlocks)
{
    uint32_t             errorstate;
    SDIO_DataInitTypeDef transfer_cfg;
    uint32_t             add = BlockAdd;

    /* Check the sd handle */
    ASSERT_PARAM(hsd);

    if (NULL == pData)
    {
        hsd->ErrorCode |= HAL_SD_ERROR_PARAM;
        return HAL_ERROR;
    }

    if (NumberOfBlocks == 0)
    {
        hsd->ErrorCode |= HAL_SD_ERROR_PARAM;
        return HAL_ERROR;
    }

    if (hsd->state == HAL_STATE_READY)
    {
        hsd->ErrorCode = HAL_SD_ERROR_NONE;

        if ((add + NumberOfBlocks) > (hsd->SdCard.LogBlockNbr))
        {
            hsd->ErrorCode |= HAL_SD_ERROR_ADDR_OUT_OF_RANGE;
            return HAL_ERROR;
        }

        hsd->state = HAL_STATE_BUSY;

        /* SD IRQ Gen Config */
        __SDMMC_ENABLE_IT_SIG(hsd->instance, SDMMC_IT_TRANSCPLIE | SDMMC_IT_DMAIE | SDMMC_IT_BUFRIE | SDMMC_IT_CURLMTEIE | SDMMC_IT_CMDTOEIE | SDMMC_IT_CMDCRCEIE | SDMMC_IT_CMDEBEIE | SDMMC_IT_CMDINDEIE | SDMMC_IT_DATTOEIE | SDMMC_IT_DATCRCEIE | SDMMC_IT_DATEBEIE | SDMMC_IT_CURLMTEIE | SDMMC_IT_ATCMDEIE | SDMMC_IT_ADMAEIE);

        if (hsd->SdCard.CardType != CARD_SDHC_SDXC)
        {
            add *= 512U;
        }

        memset(&transfer_cfg, 0, sizeof(SDIO_DataInitTypeDef));
        transfer_cfg.TransferDir = SDMMC_TRANSFER_DIR_TO_SDIO;
        transfer_cfg.BlockSize   = 512;
        transfer_cfg.DMAEnable   = 1;
        transfer_cfg.DMAType     = SDMMC_TRANSFER_SDMA;
        transfer_cfg.ptr_Buf     = pData;

        /* Read block(s) in polling mode */
        if (NumberOfBlocks > 1U)
        {
            hsd->Context                  = (SD_CONTEXT_READ_MULTIPLE_BLOCK | SD_CONTEXT_SDMA);
            transfer_cfg.MultiBlockEnable = 1;
            transfer_cfg.BlockEnable      = 1;
            transfer_cfg.BlockCount       = NumberOfBlocks;
#if (configUSE_SD_AUTOCMD_ENABLE == 1)
            transfer_cfg.AutoCMD12Enable = 1;
#endif
            SDMMC_ConfigData(hsd->instance, transfer_cfg);

            /* Read Multi Block command */
            errorstate = SDMMC_CmdReadMultiBlock(hsd->instance, add);
        }
        else
        {
            hsd->Context                  = (SD_CONTEXT_READ_SINGLE_BLOCK | SD_CONTEXT_SDMA);
            transfer_cfg.MultiBlockEnable = 0;
            SDMMC_ConfigData(hsd->instance, transfer_cfg);

            /* Read Single Block command */
            errorstate = SDMMC_CmdReadSingleBlock(hsd->instance, add);
        }

        if (errorstate != HAL_SD_ERROR_NONE)
        {
            hsd->ErrorCode |= errorstate;
            hsd->state   = HAL_STATE_READY;
            hsd->Context = SD_CONTEXT_NONE;
            return HAL_ERROR;
        }
        return HAL_OK;
    }
    else
    {
        hsd->ErrorCode |= HAL_SD_ERROR_BUSY;
        return HAL_ERROR;
    }
}

/**
 * @brief  Writes block(s) to a specified address in a card. The Data transfer
 *         is managed by DMA mode.
 * @note   This API should be followed by a check on the card state through
 *         HAL_SD_GetCardState().
 * @note   You could also check the DMA transfer process through the SD Tx
 *         interrupt event.
 * @param  hsd: Pointer to SD handle
 * @param  pData: Pointer to the buffer that will contain the data to transmit
 * @param  BlockAdd: Block Address where data will be written
 * @param  NumberOfBlocks: Number of blocks to write
 * @retval HAL status
 */
HAL_Status_T HAL_SD_WriteBlocks_SDMA(SD_Handle_T *hsd, uint8_t *pData, uint32_t BlockAdd, uint32_t NumberOfBlocks)
{
    uint32_t             errorstate;
    SDIO_DataInitTypeDef transfer_cfg;
    uint32_t             add = BlockAdd;

    /* Check the sd handle */
    ASSERT_PARAM(hsd);

    if (NULL == pData)
    {
        hsd->ErrorCode |= HAL_SD_ERROR_PARAM;
        return HAL_ERROR;
    }

    if (NumberOfBlocks == 0)
    {
        hsd->ErrorCode |= HAL_SD_ERROR_PARAM;
        return HAL_ERROR;
    }

    if (hsd->state == HAL_STATE_READY)
    {
        hsd->ErrorCode = HAL_SD_ERROR_NONE;

        if ((add + NumberOfBlocks) > (hsd->SdCard.LogBlockNbr))
        {
            hsd->ErrorCode |= HAL_SD_ERROR_ADDR_OUT_OF_RANGE;
            return HAL_ERROR;
        }

        hsd->state = HAL_STATE_BUSY;

        /* SD IRQ Gen Config */
        __SDMMC_ENABLE_IT_SIG(hsd->instance, SDMMC_IT_TRANSCPLIE | SDMMC_IT_DMAIE | SDMMC_IT_BUFWIE | SDMMC_IT_CURLMTEIE | SDMMC_IT_CMDTOEIE | SDMMC_IT_CMDCRCEIE | SDMMC_IT_CMDEBEIE | SDMMC_IT_CMDINDEIE | SDMMC_IT_DATTOEIE | SDMMC_IT_DATCRCEIE | SDMMC_IT_DATEBEIE | SDMMC_IT_CURLMTEIE | SDMMC_IT_ATCMDEIE | SDMMC_IT_ADMAEIE);

        if (hsd->SdCard.CardType != CARD_SDHC_SDXC)
        {
            add *= 512U;
        }

        memset(&transfer_cfg, 0, sizeof(SDIO_DataInitTypeDef));
        transfer_cfg.TransferDir = SDMMC_TRANSFER_DIR_TO_CARD;
        transfer_cfg.BlockSize   = 512;
        transfer_cfg.DMAEnable   = 1;
        transfer_cfg.DMAType     = SDMMC_TRANSFER_SDMA;
        transfer_cfg.ptr_Buf     = pData;

        /* Read block(s) in polling mode */
        if (NumberOfBlocks > 1U)
        {
            hsd->Context                  = (SD_CONTEXT_WRITE_MULTIPLE_BLOCK | SD_CONTEXT_SDMA);
            transfer_cfg.MultiBlockEnable = 1;
            transfer_cfg.BlockEnable      = 1;
            transfer_cfg.BlockCount       = NumberOfBlocks;
#if (configUSE_SD_AUTOCMD_ENABLE == 1)
            transfer_cfg.AutoCMD12Enable = 1;
#endif
            SDMMC_ConfigData(hsd->instance, transfer_cfg);

            /* Write Multi Block command */
            errorstate = SDMMC_CmdWriteMultiBlock(hsd->instance, add);
        }
        else
        {
            hsd->Context                  = (SD_CONTEXT_WRITE_SINGLE_BLOCK | SD_CONTEXT_SDMA);
            transfer_cfg.MultiBlockEnable = 0;
            SDMMC_ConfigData(hsd->instance, transfer_cfg);

            /* Write Single Block command */
            errorstate = SDMMC_CmdWriteSingleBlock(hsd->instance, add);
        }

        if (errorstate != HAL_SD_ERROR_NONE)
        {
            hsd->ErrorCode |= errorstate;
            hsd->state   = HAL_STATE_READY;
            hsd->Context = SD_CONTEXT_NONE;
            return HAL_ERROR;
        }

        return HAL_OK;
    }
    else
    {
        hsd->ErrorCode |= HAL_SD_ERROR_BUSY;
        return HAL_ERROR;
    }
}

/**
 * @brief  Reads block(s) from a specified address in a card. The Data transfer
 *         is managed by ADMA mode.
 * @note   This API should be followed by a check on the card state through
 *         HAL_SD_GetCardState().
 * @note   You could also check the DMA transfer process through the SD Rx
 *         interrupt event.
 * @param  hsd: Pointer SD handle
 * @param  pData: Pointer to the buffer that will contain the received data
 * @param  BlockAdd: Block Address from where data is to be read
 * @param  NumberOfBlocks: Number of blocks to read.
 * @retval HAL status
 */
HAL_Status_T HAL_SD_ReadBlocks_ADMA(SD_Handle_T *hsd, uint8_t *pData, uint32_t BlockAdd, uint32_t NumberOfBlocks)
{
    uint32_t             errorstate;
    SDIO_DataInitTypeDef transfer_cfg;
    uint32_t             add = BlockAdd;

    /* Check the sd handle */
    ASSERT_PARAM(hsd);

    if (hsd->SdCard.CardType != CARD_SDHC_SDXC)
    {
        hsd->ErrorCode |= HAL_SD_ERROR_UNSUPPORTED_FEATURE;
        return HAL_ERROR;
    }

    if (NULL == pData)
    {
        hsd->ErrorCode |= HAL_SD_ERROR_PARAM;
        return HAL_ERROR;
    }

    if (NumberOfBlocks == 0)
    {
        hsd->ErrorCode |= HAL_SD_ERROR_PARAM;
        return HAL_ERROR;
    }

    if (hsd->state == HAL_STATE_READY)
    {
        hsd->ErrorCode = HAL_SD_ERROR_NONE;

        if ((add + NumberOfBlocks) > (hsd->SdCard.LogBlockNbr))
        {
            hsd->ErrorCode |= HAL_SD_ERROR_ADDR_OUT_OF_RANGE;
            return HAL_ERROR;
        }

        hsd->state = HAL_STATE_BUSY;

        /* SD IRQ Gen Config */
        __SDMMC_ENABLE_IT_SIG(hsd->instance, SDMMC_IT_TRANSCPLIE | SDMMC_IT_DMAIE | SDMMC_IT_BUFRIE | SDMMC_IT_CURLMTEIE | SDMMC_IT_CMDTOEIE | SDMMC_IT_CMDCRCEIE | SDMMC_IT_CMDEBEIE | SDMMC_IT_CMDINDEIE | SDMMC_IT_DATTOEIE | SDMMC_IT_DATCRCEIE | SDMMC_IT_DATEBEIE | SDMMC_IT_CURLMTEIE | SDMMC_IT_ATCMDEIE | SDMMC_IT_ADMAEIE);

        if (hsd->SdCard.CardType != CARD_SDHC_SDXC)
        {
            add *= 512U;
        }

        memset(&transfer_cfg, 0, sizeof(SDIO_DataInitTypeDef));
        transfer_cfg.TransferDir = SDMMC_TRANSFER_DIR_TO_SDIO;
        transfer_cfg.BlockSize   = 512;
        transfer_cfg.DMAEnable   = 1;
        transfer_cfg.DMAType     = SDMMC_TRANSFER_ADMA;
        SDMMC_Fill_ADMA_Desc_Table(NumberOfBlocks * hsd->SdCard.BlockSize, (uint32_t *)pData);

        /* Read block(s) in polling mode */
        if (NumberOfBlocks > 1U)
        {
            hsd->Context                  = (SD_CONTEXT_READ_MULTIPLE_BLOCK | SD_CONTEXT_ADMA);
            transfer_cfg.MultiBlockEnable = 1;
            transfer_cfg.BlockEnable      = 1;
            transfer_cfg.BlockCount       = NumberOfBlocks;
#if (configUSE_SD_AUTOCMD_ENABLE == 1)
            if (hsd->SdCard.Cmd23Support)
                transfer_cfg.AutoCMD23Enable = 1;
            else
                transfer_cfg.AutoCMD12Enable = 1;
#endif

            SDMMC_ConfigData(hsd->instance, transfer_cfg);

            /* Read Multi Block command */
            errorstate = SDMMC_CmdReadMultiBlock(hsd->instance, add);
        }
        else
        {
            hsd->Context                  = (SD_CONTEXT_READ_SINGLE_BLOCK | SD_CONTEXT_ADMA);
            transfer_cfg.MultiBlockEnable = 0;
            SDMMC_ConfigData(hsd->instance, transfer_cfg);

            /* Read Single Block command */
            errorstate = SDMMC_CmdReadSingleBlock(hsd->instance, add);
        }

        if (errorstate != HAL_SD_ERROR_NONE)
        {
            hsd->ErrorCode |= errorstate;
            hsd->state   = HAL_STATE_READY;
            hsd->Context = SD_CONTEXT_NONE;
            return HAL_ERROR;
        }

        return HAL_OK;
    }
    else
    {
        hsd->ErrorCode |= HAL_SD_ERROR_BUSY;
        return HAL_ERROR;
    }
}

/**
 * @brief  Writes block(s) to a specified address in a card. The Data transfer
 *         is managed by ADMA mode.
 * @note   This API should be followed by a check on the card state through
 *         HAL_SD_GetCardState().
 * @note   You could also check the DMA transfer process through the SD Tx
 *         interrupt event.
 * @param  hsd: Pointer to SD handle
 * @param  pData: Pointer to the buffer that will contain the data to transmit
 * @param  BlockAdd: Block Address where data will be written
 * @param  NumberOfBlocks: Number of blocks to write
 * @retval HAL status
 */
HAL_Status_T HAL_SD_WriteBlocks_ADMA(SD_Handle_T *hsd, uint8_t *pData, uint32_t BlockAdd, uint32_t NumberOfBlocks)
{
    uint32_t             errorstate;
    SDIO_DataInitTypeDef transfer_cfg;
    uint32_t             add = BlockAdd;

    /* Check the sd handle */
    ASSERT_PARAM(hsd);

    if (hsd->SdCard.CardType != CARD_SDHC_SDXC)
    {
        hsd->ErrorCode |= HAL_SD_ERROR_UNSUPPORTED_FEATURE;
        return HAL_ERROR;
    }

    if (NULL == pData)
    {
        hsd->ErrorCode |= HAL_SD_ERROR_PARAM;
        return HAL_ERROR;
    }

    if (NumberOfBlocks == 0)
    {
        hsd->ErrorCode |= HAL_SD_ERROR_PARAM;
        return HAL_ERROR;
    }

    if (hsd->state == HAL_STATE_READY)
    {
        hsd->ErrorCode = HAL_SD_ERROR_NONE;

        if ((add + NumberOfBlocks) > (hsd->SdCard.LogBlockNbr))
        {
            hsd->ErrorCode |= HAL_SD_ERROR_ADDR_OUT_OF_RANGE;
            return HAL_ERROR;
        }

        hsd->state = HAL_STATE_BUSY;

        /* SD IRQ Gen Config */
        __SDMMC_ENABLE_IT_SIG(hsd->instance, SDMMC_IT_TRANSCPLIE | SDMMC_IT_DMAIE | SDMMC_IT_BUFWIE | SDMMC_IT_CURLMTEIE | SDMMC_IT_CMDTOEIE | SDMMC_IT_CMDCRCEIE | SDMMC_IT_CMDEBEIE | SDMMC_IT_CMDINDEIE | SDMMC_IT_DATTOEIE | SDMMC_IT_DATCRCEIE | SDMMC_IT_DATEBEIE | SDMMC_IT_CURLMTEIE | SDMMC_IT_ATCMDEIE | SDMMC_IT_ADMAEIE);

        if (hsd->SdCard.CardType != CARD_SDHC_SDXC)
        {
            add *= 512U;
        }

        memset(&transfer_cfg, 0, sizeof(SDIO_DataInitTypeDef));
        transfer_cfg.TransferDir = SDMMC_TRANSFER_DIR_TO_CARD;
        transfer_cfg.BlockSize   = 512;
        transfer_cfg.DMAEnable   = 1;
        transfer_cfg.DMAType     = SDMMC_TRANSFER_ADMA;
        SDMMC_Fill_ADMA_Desc_Table(NumberOfBlocks * hsd->SdCard.BlockSize, (uint32_t *)pData);

        /* Read block(s) in polling mode */
        if (NumberOfBlocks > 1U)
        {
            hsd->Context                  = (SD_CONTEXT_WRITE_MULTIPLE_BLOCK | SD_CONTEXT_ADMA);
            transfer_cfg.MultiBlockEnable = 1;
            transfer_cfg.BlockEnable      = 1;
            transfer_cfg.BlockCount       = NumberOfBlocks;
#if (configUSE_SD_AUTOCMD_ENABLE == 1)
            if (hsd->SdCard.Cmd23Support)
                transfer_cfg.AutoCMD23Enable = 1;
            else
                transfer_cfg.AutoCMD12Enable = 1;

#endif

            SDMMC_ConfigData(hsd->instance, transfer_cfg);

            /* Write Multi Block command */
            errorstate = SDMMC_CmdWriteMultiBlock(hsd->instance, add);
            if (errorstate != HAL_SD_ERROR_NONE)
            {
                hsd->ErrorCode |= errorstate;
                hsd->state   = HAL_STATE_READY;
                hsd->Context = SD_CONTEXT_NONE;
                return HAL_ERROR;
            }
        }
        else
        {
            hsd->Context                  = (SD_CONTEXT_WRITE_SINGLE_BLOCK | SD_CONTEXT_ADMA);
            transfer_cfg.MultiBlockEnable = 0;
            SDMMC_ConfigData(hsd->instance, transfer_cfg);

            /* Read Single Block command */
            errorstate = SDMMC_CmdWriteSingleBlock(hsd->instance, add);
            if (errorstate != HAL_SD_ERROR_NONE)
            {
                hsd->ErrorCode |= errorstate;
                hsd->state   = HAL_STATE_READY;
                hsd->Context = SD_CONTEXT_NONE;
                return HAL_ERROR;
            }
        }

        return HAL_OK;
    }
    else
    {
        hsd->ErrorCode |= HAL_SD_ERROR_BUSY;
        return HAL_ERROR;
    }
}

#endif // configUSE_SD_NONBLOCKING_DMA

/**
 * @brief  Erases the specified memory area of the given SD card.
 * @note   This API should be followed by a check on the card state through
 *         HAL_SD_GetCardState().
 * @param  hsd: Pointer to SD handle
 * @param  BlockStartAdd: Start Block address
 * @param  BlockEndAdd: End Block address
 * @retval HAL status
 */
HAL_Status_T HAL_SD_Erase(SD_Handle_T *hsd, uint32_t BlockStartAdd, uint32_t BlockEndAdd)
{
    uint32_t errorstate;

    /* Check the sd handle */
    ASSERT_PARAM(hsd);

    if (hsd->state == HAL_STATE_READY)
    {
        hsd->ErrorCode = HAL_SD_ERROR_NONE;

        if (BlockEndAdd < BlockStartAdd)
        {
            hsd->ErrorCode |= HAL_SD_ERROR_PARAM;
            return HAL_ERROR;
        }

        if (BlockEndAdd > (hsd->SdCard.LogBlockNbr - 1))
        {
            hsd->ErrorCode |= HAL_SD_ERROR_ADDR_OUT_OF_RANGE;
            return HAL_ERROR;
        }

        hsd->state = HAL_STATE_BUSY;

        /* Check if the card command class supports erase command */
        if (((hsd->SdCard.Class) & SDMMC_CCCC_ERASE) == 0U)
        {
            /* Clear all the static flags */

            hsd->ErrorCode |= HAL_SD_ERROR_REQUEST_NOT_APPLICABLE;
            hsd->state = HAL_STATE_READY;
            return HAL_ERROR;
        }

        /* Get start and end block for high capacity cards */
        if (hsd->SdCard.CardType != CARD_SDHC_SDXC)
        {
            BlockStartAdd *= 512U;
            BlockEndAdd *= 512U;
        }

        /* According to sd-card spec 1.0 ERASE_GROUP_START (CMD32) and erase_group_end(CMD33) */
        if (hsd->SdCard.CardType != CARD_SECURED)
        {
            /* Send CMD32 SD_ERASE_GRP_START with argument as addr  */
            errorstate = SDMMC_CmdSDEraseStartAdd(hsd->instance, BlockStartAdd);
            if (errorstate != HAL_SD_ERROR_NONE)
            {
                /* Clear all the static flags */
                hsd->ErrorCode |= errorstate;
                hsd->state = HAL_STATE_READY;
                return HAL_ERROR;
            }

            /* Send CMD33 SD_ERASE_GRP_END with argument as addr  */
            errorstate = SDMMC_CmdSDEraseEndAdd(hsd->instance, BlockEndAdd);
            if (errorstate != HAL_SD_ERROR_NONE)
            {
                /* Clear all the static flags */
                hsd->ErrorCode |= errorstate;
                hsd->state = HAL_STATE_READY;
                return HAL_ERROR;
            }
        }

        /* Send CMD38 ERASE */
        errorstate = SDMMC_CmdErase(hsd->instance);
        if (errorstate != HAL_SD_ERROR_NONE)
        {
            /* Clear all the static flags */
            hsd->ErrorCode |= errorstate;
            hsd->state = HAL_STATE_READY;
            return HAL_ERROR;
        }

        hsd->state = HAL_STATE_READY;

        return HAL_OK;
    }
    else
    {
        return HAL_BUSY;
    }
}

#if (configUSE_SD_CARD_DETECT == 1)

uint32_t SD_CARD_DETECT_GPIO_Init(void)
{
    GPIO_Init_T GPIO_CARD_DECT;

    GPIO_CARD_DECT.pin  = SD_DETECT_PIN;
    GPIO_CARD_DECT.mode = GPIO_MODE_INPUT;
    GPIO_CARD_DECT.pull = GPIO_PULL_UP;

    if (HAL_OK != HAL_GPIO_Init(SD_DETECT_GPIO_PORT, &GPIO_CARD_DECT))
        return HAL_ERROR;

    return HAL_OK;
}

/**
 * @brief  Detects if SD card is correctly plugged in the memory slot or not.
 * @retval Returns if SD is detected or not
 */
uint8_t HAL_SD_IsDetected(void)
{
    __IO uint8_t status = HAL_OK;
    uint32_t     level;

    /* Check twice to avoid signal jitter*/
    for (;;)
    {
        level = HAL_GPIO_ReadPin(SD_DETECT_GPIO_PORT, SD_DETECT_PIN);

        HAL_Delay(50);

        if (level == HAL_GPIO_ReadPin(SD_DETECT_GPIO_PORT, SD_DETECT_PIN))
            break;
    }

    /* Check SD card detect pin */
    if (level == GPIO_PIN_HIGH)
    {
        status = HAL_ERROR;
    }

    return status;
}
#endif

/**
 * @brief  Send Status info command.
 * @param  hsd: pointer to SD handle
 * @param  pSDstatus: Pointer to the buffer that will contain the SD card status
 *         SD Status register)
 * @retval error state
 */
static uint32_t SD_SendSDStatus(SD_Handle_T *hsd, uint32_t *pSDstatus)
{
    SDIO_DataInitTypeDef transfer_cfg;
    uint32_t             errorstate;

    /* Check the sd handle */
    ASSERT_PARAM(hsd);

    /* Set block size for card if it is not equal to current block size for card */
    errorstate = SDMMC_CmdBlockLength(hsd->instance, 64U);
    if (errorstate != HAL_SD_ERROR_NONE)
    {
        hsd->ErrorCode |= HAL_SD_ERROR_NONE;
        return errorstate;
    }

    /* Send CMD55 */
    errorstate = SDMMC_CmdAppCommand(hsd->instance, (uint32_t)(hsd->SdCard.RelCardAdd << 16U));
    if (errorstate != HAL_SD_ERROR_NONE)
    {
        hsd->ErrorCode |= errorstate;
        return errorstate;
    }

    /* Transfer Init */
    memset(&transfer_cfg, 0, sizeof(SDIO_DataInitTypeDef));
    transfer_cfg.TransferDir = SDMMC_TRANSFER_DIR_TO_SDIO;
    transfer_cfg.BlockEnable = 0;
    transfer_cfg.BlockSize   = 64;
    SDMMC_ConfigData(hsd->instance, transfer_cfg);

    /* Send ACMD13 (SD_APP_STAUS)  with argument as card's RCA */
    errorstate = SDMMC_CmdStatusRegister(hsd->instance);
    if (errorstate != HAL_SD_ERROR_NONE)
    {
        hsd->ErrorCode |= errorstate;
        return errorstate;
    }

    SDMMC_ReadData(hsd->instance, pSDstatus, 64);

    return HAL_SD_ERROR_NONE;
}

/**
 * @brief  Enables the SDIO wide bus mode.
 * @param  hsd: pointer to SD handle
 * @retval error state
 */
static uint32_t SD_WideBus_Enable(SD_Handle_T *hsd)
{
    uint32_t errorstate;

    /* Check the sd handle */
    ASSERT_PARAM(hsd);

    /* If requested card supports wide bus operation */
    if ((hsd->SCR[0U] & SDMMC_WIDE_BUS_SUPPORT) != SDMMC_ALLZERO)
    {
        /* Send CMD55 APP_CMD with argument as card's RCA.*/
        errorstate = SDMMC_CmdAppCommand(hsd->instance, (uint32_t)(hsd->SdCard.RelCardAdd << 16U));
        if (errorstate != HAL_SD_ERROR_NONE)
        {
            return errorstate;
        }

        /* Send ACMD6 APP_CMD with argument as 2 for wide bus mode */
        errorstate = SDMMC_CmdBusWidth(hsd->instance, 2U);
        if (errorstate != HAL_SD_ERROR_NONE)
        {
            return errorstate;
        }

        return HAL_SD_ERROR_NONE;
    }
    else
    {
        return HAL_SD_ERROR_REQUEST_NOT_APPLICABLE;
    }
}

/**
 * @brief  Disables the SDIO wide bus mode.
 * @param  hsd: Pointer to SD handle
 * @retval error state
 */
static uint32_t SD_WideBus_Disable(SD_Handle_T *hsd)
{
    uint32_t errorstate;

    /* If requested card supports 1 bit mode operation */
    if ((hsd->SCR[0U] & SDMMC_SINGLE_BUS_SUPPORT) != SDMMC_ALLZERO)
    {
        /* Send CMD55 APP_CMD with argument as card's RCA */
        errorstate = SDMMC_CmdAppCommand(hsd->instance, (uint32_t)(hsd->SdCard.RelCardAdd << 16U));
        if (errorstate != HAL_SD_ERROR_NONE)
        {
            return errorstate;
        }

        /* Send ACMD6 APP_CMD with argument as 0 for single bus mode */
        errorstate = SDMMC_CmdBusWidth(hsd->instance, 0U);
        if (errorstate != HAL_SD_ERROR_NONE)
        {
            return errorstate;
        }

        return HAL_SD_ERROR_NONE;
    }
    else
    {
        return HAL_SD_ERROR_REQUEST_NOT_APPLICABLE;
    }
}

/**
 * @brief  Finds the SD card SCR register value.
 * @param  hsd: Pointer to SD handle
 * @param  pSCR: pointer to the buffer that will contain the SCR value
 * @retval error state
 */
static uint32_t SD_FindSCR(SD_Handle_T *hsd, uint32_t *pSCR)
{
    SDIO_DataInitTypeDef transfer_cfg;
    uint32_t             errorstate;

    /* Set Block Size To 8 Bytes */
    errorstate = SDMMC_CmdBlockLength(hsd->instance, 8U);
    if (errorstate != HAL_SD_ERROR_NONE)
    {
        return errorstate;
    }

    /* Send CMD55 */
    errorstate = SDMMC_CmdAppCommand(hsd->instance, (uint32_t)(hsd->SdCard.RelCardAdd << 16U));
    if (errorstate != HAL_SD_ERROR_NONE)
    {
        hsd->ErrorCode |= HAL_SD_ERROR_NONE;
        return errorstate;
    }

    /* Transfer Init */
    memset(&transfer_cfg, 0, sizeof(SDIO_DataInitTypeDef));
    transfer_cfg.TransferDir = SDMMC_TRANSFER_DIR_TO_SDIO;
    transfer_cfg.BlockEnable = 0;
    transfer_cfg.BlockSize   = 8;
    SDMMC_ConfigData(hsd->instance, transfer_cfg);

    /* Send ACMD51 SD_APP_SEND_SCR with argument as 0 */
    errorstate = SDMMC_CmdSendSCR(hsd->instance);
    if (errorstate != HAL_SD_ERROR_NONE)
    {
        return errorstate;
    }

    SDMMC_ReadData(hsd->instance, pSCR, 8);

    return HAL_SD_ERROR_NONE;
}

#if ((configUSE_SD_NONBLOCKING_IT == 1) || (configUSE_SD_NONBLOCKING_DMA == 1))

/**
 * @brief  Wrap up writing in non-blocking mode.
 * @param  hsd: pointer to a SD_HandleTypeDef structure that contains
 *              the configuration information.
 * @retval None
 */
static void SD_Write_IT(SD_Handle_T *hsd)
{
    uint32_t      len;
    uint32_t      dataremaining;
    uint32_t      trans_sz;
    uint32_t     *buffer;
    SN_SDIO_Type *SDIOx;

    SDIOx         = hsd->instance;
    buffer        = (uint32_t *)hsd->pTxBuffPtr;
    dataremaining = hsd->TxXferSize;

    trans_sz = 512;
    SDIOx->RIS &= SDMMC_INTR_STS_BUFF_WRITE_READY;

    while (dataremaining)
    {
        /* Wait write buffer ready
         * This bit will be set when at least one block( 512 bytes ) of space is avalilable for written
         */

        while (!__SDMMC_GET_STA(SDIOx, SDMMC_STA_BUFWEN) && !__SDMMC_GET_FLAG(SDIOx, SDMMC_INTR_STS_ERR))
        {
            hsd->pTxBuffPtr = (uint8_t *)buffer;
            hsd->TxXferSize = dataremaining;
            return;
        };

        len = (dataremaining < trans_sz) ? dataremaining : trans_sz;
        dataremaining -= len;

        /* Each loop send 512 bytes for one block */
        while (len)
        {
            SDMMC_WriteFIFO(SDIOx, buffer);
            len -= 4;
            buffer++;
        }
    }
}

/**
 * @brief  Wrap up reading in non-blocking mode.
 * @param  hsd: pointer to a SD_HandleTypeDef structure that contains
 *              the configuration information.
 * @retval None
 */
static void SD_Read_IT(SD_Handle_T *hsd)
{
    uint32_t      len;
    uint32_t      recv_len = 0;
    uint32_t      dataremaining;
    uint32_t      trans_sz;
    uint32_t     *buffer;
    SN_SDIO_Type *SDIOx;

    SDIOx         = hsd->instance;
    buffer        = (uint32_t *)hsd->pRxBuffPtr;
    dataremaining = hsd->RxXferSize;

    trans_sz = 512;
    SDIOx->RIS &= SDMMC_INTR_STS_BUFF_READ_READY;

    while (dataremaining)
    {
        /* Wait write buffer ready
         * This bit will be set when at least one block( 512 bytes ) of space is avalilable for read
         */
        while (!__SDMMC_GET_STA(SDIOx, SDMMC_STA_BUFREN) && !__SDMMC_GET_FLAG(SDIOx, SDMMC_INTR_STS_ERR))
        {
            hsd->pRxBuffPtr = (uint8_t *)buffer;
            hsd->RxXferSize = dataremaining;
            return;
        };

        len = (dataremaining < trans_sz) ? dataremaining : trans_sz;
        dataremaining -= len;

        while (len)
        {
            *buffer = SDMMC_ReadFIFO(SDIOx);
            len -= 4;
            recv_len += 4;
            buffer++;
        }
    }
}

/**
 * @brief  Wrap up reading in non-blocking mode.
 * @param  hsd: pointer to a SD_HandleTypeDef structure that contains
 *              the configuration information.
 * @retval None
 */
static void SD_Transfer_SDMA(SD_Handle_T *hsd)
{
    uint32_t      next_addr = 0;
    SN_SDIO_Type *SDIOx     = hsd->instance;

    next_addr = SDIOx->SDMAADD;

    SDIOx->SDMAADD = (uint32_t)((uintptr_t)next_addr);
}

/**
 * @brief  This function handles SD card interrupt request.
 * @param  hsd: Pointer to SD handle
 * @retval None
 */
ATTR_ISR void HAL_SD_IRQHandler(SD_Handle_T *hsd)
{
    uint32_t sts;
    uint32_t context = hsd->Context;

    SN_SDIO_Type *SDIOx = hsd->instance;

    /* Read Interrupt Status */
    sts = HAL_REG_READ(SDIOx->RIS);

    /* Clear Interrupt Status immediately */
    HAL_REG_WRITE(SDIOx->RIS, sts);

    /* buffer read ready */
    if (HAL_REG_RBIT(sts, SDMMC_INTR_STS_BUFF_READ_READY) && (context & SD_CONTEXT_IT) != 0U)
    {
        sts &= ~SDMMC_INTR_STS_BUFF_READ_READY;
        SD_Read_IT(hsd);
    }

    /* buffer write ready */
    if (HAL_REG_RBIT(sts, SDMMC_INTR_STS_BUFF_WRITE_READY) && (context & SD_CONTEXT_IT) != 0U)
    {
        sts &= ~SDMMC_INTR_STS_BUFF_WRITE_READY;
        SD_Write_IT(hsd);
    }

    /* SDMA DMA Flag detect */
    if (HAL_REG_RBIT(sts, SDMMC_INTR_STS_DMA))
    {
        /* SDMA Boundary interrupt */
        if ((context & SD_CONTEXT_SDMA) != 0U)
        {
            SD_Transfer_SDMA(hsd);
        }
    }

    /* Command complete */
    if (HAL_REG_RBIT(sts, SDMMC_INTR_STS_CMD_COMPLETE))
    {
    }

    /* Transfer complete */
    if (HAL_REG_RBIT(sts, SDMMC_INTR_STS_TXR_COMPLETE))
    {
        if ((context & (SD_CONTEXT_IT | SD_CONTEXT_SDMA | SD_CONTEXT_ADMA)) != 0U)
        {
#if (configUSE_SD_AUTOCMD_ENABLE == 0)
            if (((context & SD_CONTEXT_READ_MULTIPLE_BLOCK) != 0U) || ((context & SD_CONTEXT_WRITE_MULTIPLE_BLOCK) != 0U))
            {
                uint32_t errorstate;
                errorstate = SDMMC_CmdStopTransfer(hsd->instance);
                if (errorstate != HAL_SD_ERROR_NONE)
                {
                    hsd->ErrorCode |= errorstate;
#if (configUSE_SD_REGISTER_CALLBACKS == 1U)
                    hsd->ErrorCallback(hsd);
#else
                    HAL_SD_ErrorCallback(hsd);
#endif /* configUSE_SD_REGISTER_CALLBACKS */
                }
            }
#endif

            hsd->state   = HAL_STATE_READY;
            hsd->Context = SD_CONTEXT_NONE;

            if (((context & SD_CONTEXT_READ_SINGLE_BLOCK) != 0U) || ((context & SD_CONTEXT_READ_MULTIPLE_BLOCK) != 0U))
            {
#if (configUSE_SD_REGISTER_CALLBACKS == 1U)
                hsd->RxCpltCallback(hsd);
#else
                HAL_SD_RxCpltCallback(hsd);
#endif /* configUSE_SD_REGISTER_CALLBACKS */
            }
            else
            {
#if (configUSE_SD_REGISTER_CALLBACKS == 1U)
                hsd->TxCpltCallback(hsd);
#else
                HAL_SD_TxCpltCallback(hsd);
#endif /* configUSE_SD_REGISTER_CALLBACKS */
            }

            /* disable SD irq */
            __SDMMC_DISABLE_IT_SIG(hsd->instance, SDMMC_IT_TRANSCPLIE | SDMMC_IT_DMAIE | SDMMC_IT_BUFRIE | SDMMC_IT_BUFWIE | SDMMC_IT_CURLMTEIE | SDMMC_IT_CMDCRCEIE | SDMMC_IT_CMDEBEIE | SDMMC_IT_CMDINDEIE | SDMMC_IT_DATTOEIE | SDMMC_IT_DATCRCEIE | SDMMC_IT_CMDTOEIE | SDMMC_IT_DATEBEIE | SDMMC_IT_CURLMTEIE | SDMMC_IT_ATCMDEIE | SDMMC_IT_ADMAEIE);
        }
        else
        {
            // Do nothing
        }
    }

    /* Error */
    if (HAL_REG_RBIT(sts, SDMMC_INTR_STS_ERR))
    {
        /* error flags will be cleard after error recovery*/
        if (HAL_REG_RBIT(sts, SDMMC_INTR_STS_ERR_CMD_CRC)) /*	CMD CRC error */
        {
            hsd->ErrorCode |= SDMMC_ERROR_CMD_CRC_FAIL;
        }

        if (HAL_REG_RBIT(sts, SDMMC_INTR_STS_ERR_CMD_ENDBIT)) /* CMD End bit error */
        {
            hsd->ErrorCode |= SDMMC_ERROR_CMD_END_BIT;
        }

        if (HAL_REG_RBIT(sts, SDMMC_INTR_STS_ERR_CMD_INDEX)) /*	CMD Index error */
        {
            hsd->ErrorCode |= SDMMC_ERROR_CMD_INDEX;
        }

        if (HAL_REG_RBIT(sts, SDMMC_INTR_STS_ERR_CMD_TIMEOUT)) /*	CMD Timeout error */
        {
            hsd->ErrorCode |= SDMMC_ERROR_CMD_RSP_TIMEOUT;
        }

        if (HAL_REG_RBIT(sts, SDMMC_INTR_STS_ERR_DATA_TIMEOUT)) /*	DATA Timeout error */
        {
            hsd->ErrorCode |= SDMMC_ERROR_DATA_TIMEOUT;
        }

        if (HAL_REG_RBIT(sts, SDMMC_INTR_STS_ERR_DATA_CRC)) /*	DATA CRC error */
        {
            hsd->ErrorCode |= SDMMC_ERROR_DATA_CRC_FAIL;
        }

        if (HAL_REG_RBIT(sts, SDMMC_INTR_STS_ERR_DATA_ENDBIT)) /*	DATA End bit error */
        {
            hsd->ErrorCode |= SDMMC_ERROR_DATA_END_BIT;
        }

        if (HAL_REG_RBIT(sts, SDMMC_INTR_STS_ERR_AutoCMD)) /*	Auto CMD error */
        {
            hsd->ErrorCode |= SDMMC_ERROR_AUTO_CMD;
        }

        if (HAL_REG_RBIT(sts, SDMMC_INTR_STS_ERR_ADMA)) /*	Auto CMD error */
        {
            hsd->ErrorCode |= SDMMC_ERROR_ADMA;
        }
#if (configUSE_SD_REGISTER_CALLBACKS == 1U)
        hsd->ErrorCallback(hsd);
#else
        HAL_SD_ErrorCallback(hsd);
#endif /* configUSE_SD_REGISTER_CALLBACKS */
    }
}

#endif // ((configUSE_SD_NONBLOCKING_IT == 1) || (configUSE_SD_NONBLOCKING_DMA == 1))
