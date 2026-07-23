/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#include "sn34f78x_hal_eth.h"

/** @addtogroup SN34F78x_HAL_Driver
 * @{
 */

/** @defgroup ETH ETH
 * @brief ETH HAL module driver
 * @{
 */

#if (configUSE_FLASH_PERIPHERAL == 1)

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/** @defgroup ETH_Private_Constants ETH Private Constants
 * @{
 */
#define ETH_TIMEOUT_SWRESET            500U
#define ETH_TIMEOUT_LINKED_STATE       5000U
#define ETH_TIMEOUT_AUTONEGO_COMPLETED 5000U

/**
 * @}
 */
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/** @defgroup ETH_Private_Functions ETH Private Functions
 * @{
 */
static void         ETH_MACDMAConfig(ETH_Handle_T *heth);
static void         ETH_MACAddressConfig(ETH_Handle_T *heth, uint8_t *Addr);
static void         ETH_MACReceptionEnable(ETH_Handle_T *heth);
static void         ETH_MACReceptionDisable(ETH_Handle_T *heth);
static void         ETH_MACTransmissionEnable(ETH_Handle_T *heth);
static void         ETH_MACTransmissionDisable(ETH_Handle_T *heth);
static void         ETH_DMATransmissionEnable(ETH_Handle_T *heth);
static void         ETH_DMATransmissionDisable(ETH_Handle_T *heth);
static void         ETH_DMAReceptionEnable(ETH_Handle_T *heth);
static void         ETH_DMAReceptionDisable(ETH_Handle_T *heth);
static HAL_Status_T ETH_PHYAutoNeo(ETH_Handle_T *heth);
static uint32_t     ETH_WakeUpCRC32(uint8_t *WakeupFrame, uint32_t Size, uint32_t *Mask);

#if (configUSE_ETH_REGISTER_CALLBACKS == 1)
static void ETH_InitCallbacksToDefault(ETH_Handle_T *heth);
#endif /* configUSE_ETH_REGISTER_CALLBACKS */
#if (ETH_DEBUG == 1)
static void ETH_DUMP_MAC_REG(ETH_Handle_T *heth);
static void ETH_DUMP_PHY_REG(ETH_Handle_T *heth);
static void ETH_SHOW_LINK_STATUS(ETH_Handle_T *heth);
// static int ETH_DuplexSpeedCfg(ETH_Handle_T *heth, uint32_t duplex, uint32_t speed);
#endif

/**
 * @}
 */
/* Private functions ---------------------------------------------------------*/

/** @defgroup ETH_Exported_Functions ETH Exported Functions
 * @{
 */

/** @defgroup ETH_Exported_Functions_Group1 Initialization and de-initialization functions
  *  @brief   Initialization and Configuration functions
  *
  @verbatim
  ===============================================================================
            ##### Initialization and de-initialization functions #####
  ===============================================================================
  [..]  This section provides functions allowing to:
      (+) Initialize and configure the Ethernet peripheral
      (+) De-initialize the Ethernet peripheral

  @endverbatim
  * @{
  */

/**
 * @brief  Initializes the Ethernet MAC and DMA according to default
 *         parameters.
 * @param  heth pointer to a ETH_Handle_T structure that contains
 *         the configuration information for ETHERNET module
 * @retval HAL status
 */
HAL_Status_T HAL_ETH_Init(ETH_Handle_T *heth)
{
    uint16_t        phyreg    = 0U;
    uint32_t        tickstart = 0U;
    uint16_t        lpa;
    uint16_t        lpagb = 0;
    int             common_adv;
    int             common_adv_gb = 0;
    uint16_t        status, adv;
    SN_ETHMAC_Type *SN_ETH = NULL;

    /* Check the ETH peripheral state */
    if (heth == NULL)
    {
        return HAL_ERROR;
    }

    /* Check parameters */
    ASSERT_PARAM(IS_ETH_AUTONEGOTIATION(heth->Init.AutoNegotiation));
    ASSERT_PARAM(IS_ETH_CHECKSUM_MODE(heth->Init.ChecksumMode));
    ASSERT_PARAM(IS_ETH_MEDIA_INTERFACE(heth->Init.MediaInterface));

    if (heth->State == HAL_ETH_STATE_RESET)
    {
#if (configUSE_ETH_REGISTER_CALLBACKS == 1)
        ETH_InitCallbacksToDefault(heth);

        if (heth->MspInitCallback == NULL)
        {
            /* Init the low level hardware : GPIO, CLOCK, NVIC. */
            heth->MspInitCallback = HAL_ETH_MspInit;
        }
        heth->MspInitCallback(heth);

#else
        /* Init the low level hardware : GPIO, CLOCK, NVIC. */
        HAL_ETH_MspInit(heth);
#endif /* configUSE_ETH_REGISTER_CALLBACKS */
    }

    SN_ETH = heth->Instance;

    /* Select MII or RMII Mode*/
    HAL_REG_WRITE(SN_ETH->GIS_b.IF, heth->Init.MediaInterface);

    /* Ethernet Software reset */
    /* Set the SWR bit: resets all MAC subsystem internal registers and logic */
    /* After reset all the registers holds their respective reset values */
    HAL_REG_WRITE(SN_ETH->MACCR, ETH_MACCR_SW_RST);

    /* Get tick */
    tickstart = HAL_GetTick();

    /* Wait for software reset */
    while (HAL_REG_RBIT(SN_ETH->MACCR, ETH_MACCR_SW_RST))
    {
        /* Check for the Timeout */
        if ((HAL_GetTick() - tickstart) > ETH_TIMEOUT_SWRESET)
        {
            heth->State = HAL_ETH_STATE_TIMEOUT;

            /* Note: The SWR is not performed if the ETH_RX_CLK or the ETH_TX_CLK are
               not available, please check your external PHY or the IO configuration */
            return HAL_TIMEOUT;
        }
    }

    /*-------------------- PHY initialization and configuration ----------------*/
    /* Put the PHY in reset mode */
    if ((HAL_ETH_WritePHYRegister(heth, PHY_BCR, BCR_RESET)) != HAL_OK)
    {
        /* Config MAC and DMA */
        ETH_MACDMAConfig(heth);

        /* Set the ETH peripheral state to READY */
        heth->State = HAL_ETH_STATE_READY;

        /* Return HAL_ERROR */
        return HAL_ERROR;
    }

    /* Wait phy reset complete */
    do
    {
        HAL_ETH_ReadPHYRegister(heth, PHY_BSR, &phyreg);
    } while ((phyreg & BCR_RESET));

    /* Interrupt mask for duplex and speed change */
    HAL_ETH_WritePHYRegister(heth, PHY_DAVICOM_SPEC_INT, 0x0C00);

    if ((heth->Init).AutoNegotiation != ETH_AUTONEGOTIATION_DISABLE)
    {
        /* Get tick */
        tickstart = HAL_GetTick();

        /* We wait for linked status */
        do
        {
            HAL_ETH_ReadPHYRegister(heth, PHY_BSR, &phyreg);

            /* Check for the Timeout */
            if ((HAL_GetTick() - tickstart) > ETH_TIMEOUT_LINKED_STATE)
            {
                ETH_PRINT(ETH_DBG_INFO, "[INFO] Link Disconect\r\n");
                HAL_ETH_WritePHYRegister(heth, PHY_BCR, ((uint16_t)((heth->Init).DuplexMode >> 18) | (uint16_t)((heth->Init).Speed >> 11)));
                break;
            }
        } while (((phyreg & BSR_LSTATUS) != BSR_LSTATUS));

        if ((phyreg & BSR_LSTATUS) == BSR_LSTATUS)
        {
            if (ETH_PHYAutoNeo(heth) != HAL_OK)
            {
                /* Config MAC and DMA */
                ETH_MACDMAConfig(heth);

                heth->State = HAL_ETH_STATE_READY;

                return HAL_TIMEOUT;
            }

            /* Read the result of the auto-negotiation */
            HAL_ETH_ReadPHYRegister(heth, PHY_BSR, &status);

            HAL_ETH_ReadPHYRegister(heth, PHY_LPA, &lpa);

            if (lpa & LPA_LPACK)
            {
                if (status & BSR_ESTATEN)
                {
                    HAL_ETH_ReadPHYRegister(heth, PHY_ESTATUS, &status);
                    if (status & (ESTATUS_1000_TFULL | ESTATUS_1000_THALF))
                    {
                        HAL_ETH_ReadPHYRegister(heth, PHY_STAT1000, &lpagb);

                        HAL_ETH_ReadPHYRegister(heth, PHY_CTRL1000, &adv);

                        common_adv_gb = lpagb & (adv << 2);
                    }
                }

                HAL_ETH_ReadPHYRegister(heth, PHY_LPA, &lpa);

                HAL_ETH_ReadPHYRegister(heth, PHY_ADVERTISE, &adv);

                common_adv = lpa & adv;

                if (common_adv_gb & (LPA_1000FULL | LPA_1000HALF))
                {
                    (heth->Init).Speed = ETH_MACCR_SPEED_1000M;

                    if (common_adv & LPA_100FULL)
                        (heth->Init).DuplexMode = ETH_MACCR_FULLDUPLEX;
                    else
                        (heth->Init).DuplexMode = ETH_MACCR_HALFDUPLEX;
                }
                else if (common_adv & (LPA_100FULL | LPA_100HALF))
                {
                    (heth->Init).Speed = ETH_MACCR_SPEED_100M;

                    if (common_adv & LPA_100FULL)
                        (heth->Init).DuplexMode = ETH_MACCR_FULLDUPLEX;
                    else
                        (heth->Init).DuplexMode = ETH_MACCR_HALFDUPLEX;
                }
                else
                {
                    (heth->Init).Speed = ETH_MACCR_SPEED_10M;

                    if (common_adv & LPA_10FULL)
                        (heth->Init).DuplexMode = ETH_MACCR_FULLDUPLEX;
                    else
                        (heth->Init).DuplexMode = ETH_MACCR_HALFDUPLEX;
                }
            }
            else
            {
                uint16_t bcr;
                HAL_ETH_ReadPHYRegister(heth, PHY_BCR, &bcr);

                if (bcr & BCR_FULLDPLX)
                    (heth->Init).DuplexMode = ETH_MACCR_FULLDUPLEX;
                else
                    (heth->Init).DuplexMode = ETH_MACCR_HALFDUPLEX;

                if (bcr & BCR_SPEED1000)
                    (heth->Init).Speed = ETH_MACCR_SPEED_1000M;
                else if (bcr & BCR_SPEED100)
                    (heth->Init).Speed = ETH_MACCR_SPEED_100M;
                else
                    (heth->Init).Speed = ETH_MACCR_SPEED_10M;
            }
        }
    }
    else /* AutoNegotiation Disable */
    {
        /* Check parameters */
        ASSERT_PARAM(IS_ETH_SPEED(heth->Init.Speed));
        ASSERT_PARAM(IS_ETH_DUPLEX_MODE(heth->Init.DuplexMode));

        /* Set MAC Speed and Duplex Mode */
        if (HAL_ETH_WritePHYRegister(heth, PHY_BCR, ((uint16_t)((heth->Init).DuplexMode >> 18) | (uint16_t)((heth->Init).Speed >> 11))) != HAL_OK)
        {
            /* Config MAC and DMA */
            ETH_MACDMAConfig(heth);

            /* Set the ETH peripheral state to READY */
            heth->State = HAL_ETH_STATE_READY;

            /* Return HAL_ERROR */
            return HAL_ERROR;
        }

        /* Delay to assure PHY configuration */
        HAL_Delay(PHY_CONFIG_DELAY);
    }

    /* Config MAC and DMA */
    ETH_MACDMAConfig(heth);

    /* Set ETH HAL State to Ready */
    heth->State = HAL_ETH_STATE_READY;

    /* Return function status */
    return HAL_OK;
}

/**
 * @brief  De-Initializes the ETH peripheral.
 * @param  heth pointer to a ETH_Handle_T structure that contains
 *         the configuration information for ETHERNET module
 * @retval HAL status
 */
HAL_Status_T HAL_ETH_DeInit(ETH_Handle_T *heth)
{
    /* Set the ETH peripheral state to BUSY */
    heth->State = HAL_ETH_STATE_BUSY;

#if (configUSE_ETH_REGISTER_CALLBACKS == 1)
    if (heth->MspDeInitCallback == NULL)
    {
        heth->MspDeInitCallback = HAL_ETH_MspDeInit;
    }
    /* De-Init the low level hardware : GPIO, CLOCK, NVIC. */
    heth->MspDeInitCallback(heth);
#else
    /* De-Init the low level hardware : GPIO, CLOCK, NVIC. */
    HAL_ETH_MspDeInit(heth);
#endif

    /* Set ETH HAL state to Disabled */
    heth->State = HAL_ETH_STATE_RESET;

    /* Return function status */
    return HAL_OK;
}

/**
 * @brief  Initializes the DMA Tx descriptors in chain mode.
 * @param  heth pointer to a ETH_Handle_T structure that contains
 *         the configuration information for ETHERNET module
 * @param  DMATxDescTab Pointer to the first Tx desc list
 * @param  TxBuff Pointer to the first TxBuffer list
 * @param  TxBuffCount Number of the used Tx desc in the list
 * @retval HAL status
 */
HAL_Status_T HAL_ETH_DMATxDescListInit(ETH_Handle_T *heth, ETH_DMADescTypeDef *DMATxDescTab, uint8_t *TxBuff, uint32_t TxBuffCount)
{
    uint32_t            i = 0U;
    ETH_DMADescTypeDef *dmatxdesc;

    /* Set the ETH peripheral state to BUSY */
    heth->State = HAL_ETH_STATE_BUSY;

    /* Set the DMATxDescToSet pointer with the first one of the DMATxDescTab list */
    heth->TxDesc            = DMATxDescTab;
    heth->TxDMADescBaseAddr = DMATxDescTab;

    /* initial nptxdes rings */
    memset(DMATxDescTab, 0, TxBuffCount * sizeof(ETH_DMADescTypeDef));

    /* Fill each DMATxDesc descriptor with the right values */
    for (i = 0U; i < TxBuffCount; i++)
    {
        /* Get the pointer on the member (i) of the Tx Desc list */
        dmatxdesc = DMATxDescTab + i;

        /* Initialize the next descriptor with the Next Descriptor Polling Enable */
        if (i < (TxBuffCount - 1))
        {
            /* Set next descriptor address register with next descriptor base address */
            dmatxdesc->Buffer2NextDescAddr = (uint32_t)(DMATxDescTab + i + 1);
        }
        else
        {
            /* For last descriptor, set next descriptor address register equal to the first descriptor base address */
            dmatxdesc->StatusAndVDBC |= ETH_TXDES0_EDOTR;
            dmatxdesc->Buffer2NextDescAddr = (uint32_t)DMATxDescTab;
        }
    }

    /* Set Transmit Descriptor List Address Register */
    (heth->Instance)->NPTXR_BADR = (uint32_t)DMATxDescTab;

    /* Set ETH HAL State to Ready */
    heth->State = HAL_ETH_STATE_READY;

    /* Return function status */
    return HAL_OK;
}

/**
 * @brief  Initializes the DMA Rx descriptors in chain mode.
 * @param  heth pointer to a ETH_Handle_T structure that contains
 *         the configuration information for ETHERNET module
 * @param  DMARxDescTab Pointer to the first Rx desc list
 * @param  RxBuff Pointer to the first RxBuffer list
 * @param  RxBuffCount Number of the used Rx desc in the list
 * @retval HAL status
 */
HAL_Status_T HAL_ETH_DMARxDescListInit(ETH_Handle_T *heth, ETH_DMADescTypeDef *DMARxDescTab, uint8_t *RxBuff, uint32_t RxBuffCount)
{
    uint32_t            i = 0U;
    ETH_DMADescTypeDef *DMARxDesc;

    /* Set the ETH peripheral state to BUSY */
    heth->State = HAL_ETH_STATE_BUSY;

    /* Set the Ethernet RxDesc pointer with the first one of the DMARxDescTab list */
    heth->RxDesc            = DMARxDescTab;
    heth->RxDMADescBaseAddr = DMARxDescTab;

    /* initial nptxdes rings */
    memset(DMARxDescTab, 0, RxBuffCount * sizeof(ETH_DMADescTypeDef));

    /* Fill each DMARxDesc descriptor with the right values */
    for (i = 0U; i < RxBuffCount; i++)
    {
        /* Get the pointer on the member (i) of the Rx Desc list */
        DMARxDesc = DMARxDescTab + i;

        /* Set Buffer1 address pointer */
        DMARxDesc->Buffer1Addr = (uint32_t)(&RxBuff[i * ETH_RX_BUF_SIZE]);
        memset((void *)((uintptr_t)DMARxDesc->Buffer1Addr), 0, ETH_RX_BUF_SIZE);

        /* Initialize the next descriptor with the Next Descriptor Polling Enable */
        if (i < (RxBuffCount - 1U))
        {
            /* Set next descriptor address register with next descriptor base address */
            DMARxDesc->Buffer2NextDescAddr = (uint32_t)(DMARxDescTab + i + 1U);
        }
        else
        {
            /* For last descriptor, set next descriptor address register equal to the first descriptor base address */
            DMARxDesc->StatusAndVDBC |= ETH_RXDES0_EDORR;
            DMARxDesc->Buffer2NextDescAddr = (uint32_t)(DMARxDescTab);
        }
    }

    /* Set Receive Descriptor List Address Register */
    (heth->Instance)->RXR_BADR = (uint32_t)DMARxDescTab;

    /* Set Receive buffer size */
    (heth->Instance)->RBSZ = (uint32_t)ETH_RX_BUF_SIZE;

    /* Set ETH HAL State to Ready */
    heth->State = HAL_ETH_STATE_READY;

    /* Return function status */
    return HAL_OK;
}

/**
 * @brief  Initializes the ETH MSP.
 * @param  heth pointer to a ETH_Handle_T structure that contains
 *         the configuration information for ETHERNET module
 * @retval None
 */
__weak void HAL_ETH_MspInit(ETH_Handle_T *heth)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(heth);
    /* NOTE : This function Should not be modified, when the callback is needed,
    the HAL_ETH_MspInit could be implemented in the user file
    */
}

/**
 * @brief  DeInitializes ETH MSP.
 * @param  heth pointer to a ETH_Handle_T structure that contains
 *         the configuration information for ETHERNET module
 * @retval None
 */
__weak void HAL_ETH_MspDeInit(ETH_Handle_T *heth)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(heth);
    /* NOTE : This function Should not be modified, when the callback is needed,
    the HAL_ETH_MspDeInit could be implemented in the user file
    */
}

#if (configUSE_ETH_REGISTER_CALLBACKS == 1)
/**
 * @brief  Register a User ETH Callback
 *         To be used instead of the weak predefined callback
 * @param heth eth handle
 * @param CallbackID ID of the callback to be registered
 *        This parameter can be one of the following values:
 *          @arg @ref HAL_ETH_TX_COMPLETE_CB_ID Tx Complete Callback ID
 *          @arg @ref HAL_ETH_RX_COMPLETE_CB_ID Rx Complete Callback ID
 *          @arg @ref HAL_ETH_DMA_ERROR_CB_ID   DMA Error Callback ID
 *          @arg @ref HAL_ETH_MSPINIT_CB_ID     MspInit callback ID
 *          @arg @ref HAL_ETH_MSPDEINIT_CB_ID   MspDeInit callback ID
 * @param pCallback pointer to the Callback function
 * @retval status
 */
HAL_Status_T HAL_ETH_RegisterCallback(ETH_Handle_T *heth, HAL_ETH_CallbackIDTypeDef CallbackID, pETH_CallbackTypeDef pCallback)
{
    HAL_Status_T status = HAL_OK;

    if (pCallback == NULL)
    {
        return HAL_ERROR;
    }

    if (heth->State == HAL_ETH_STATE_READY)
    {
        switch (CallbackID)
        {
            case HAL_ETH_TX_COMPLETE_CB_ID:
                heth->TxCpltCallback = pCallback;
                break;

            case HAL_ETH_RX_COMPLETE_CB_ID:
                heth->RxCpltCallback = pCallback;
                break;

            case HAL_ETH_DMA_ERROR_CB_ID:
                heth->DMAErrorCallback = pCallback;
                break;

            case HAL_ETH_MSPINIT_CB_ID:
                heth->MspInitCallback = pCallback;
                break;

            case HAL_ETH_MSPDEINIT_CB_ID:
                heth->MspDeInitCallback = pCallback;
                break;

            case HAL_ETH_WAKEUP_CB_ID:
                heth->WakeUpCallback = pCallback;
                break;

            case HAL_ETH_LINKCHG_CB_ID:
                heth->LinkChgCallback = pCallback;
                break;

            default:
                /* Return error status */
                status = HAL_ERROR;
                break;
        }
    }
    else if (heth->State == HAL_ETH_STATE_RESET)
    {
        switch (CallbackID)
        {
            case HAL_ETH_MSPINIT_CB_ID:
                heth->MspInitCallback = pCallback;
                break;

            case HAL_ETH_MSPDEINIT_CB_ID:
                heth->MspDeInitCallback = pCallback;
                break;

            default:
                /* Return error status */
                status = HAL_ERROR;
                break;
        }
    }
    else
    {
        /* Return error status */
        status = HAL_ERROR;
    }

    return status;
}

/**
 * @brief  Unregister an ETH Callback
 *         ETH callabck is redirected to the weak predefined callback
 * @param heth eth handle
 * @param CallbackID ID of the callback to be unregistered
 *        This parameter can be one of the following values:
 *          @arg @ref HAL_ETH_TX_COMPLETE_CB_ID Tx Complete Callback ID
 *          @arg @ref HAL_ETH_RX_COMPLETE_CB_ID Rx Complete Callback ID
 *          @arg @ref HAL_ETH_DMA_ERROR_CB_ID      DMA Error Callback ID
 *          @arg @ref HAL_ETH_MSPINIT_CB_ID     MspInit callback ID
 *          @arg @ref HAL_ETH_MSPDEINIT_CB_ID   MspDeInit callback ID
 * @retval status
 */
HAL_Status_T HAL_ETH_UnRegisterCallback(ETH_Handle_T *heth, HAL_ETH_CallbackIDTypeDef CallbackID)
{
    HAL_Status_T status = HAL_OK;

    if (heth->State == HAL_ETH_STATE_READY)
    {
        switch (CallbackID)
        {
            case HAL_ETH_TX_COMPLETE_CB_ID:
                heth->TxCpltCallback = HAL_ETH_TxCpltCallback;
                break;

            case HAL_ETH_RX_COMPLETE_CB_ID:
                heth->RxCpltCallback = HAL_ETH_RxCpltCallback;
                break;

            case HAL_ETH_DMA_ERROR_CB_ID:
                heth->DMAErrorCallback = HAL_ETH_ErrorCallback;
                break;

            case HAL_ETH_MSPINIT_CB_ID:
                heth->MspInitCallback = HAL_ETH_MspInit;
                break;

            case HAL_ETH_MSPDEINIT_CB_ID:
                heth->MspDeInitCallback = HAL_ETH_MspDeInit;
                break;

            case HAL_ETH_WAKEUP_CB_ID:
                heth->WakeUpCallback = HAL_ETH_WakeUpCallback;
                break;

            case HAL_ETH_LINKCHG_CB_ID:
                heth->LinkChgCallback = HAL_ETH_LinkChgCallback;
                break;

            default:
                /* Return error status */
                status = HAL_ERROR;
                break;
        }
    }
    else if (heth->State == HAL_ETH_STATE_RESET)
    {
        switch (CallbackID)
        {
            case HAL_ETH_MSPINIT_CB_ID:
                heth->MspInitCallback = HAL_ETH_MspInit;
                break;

            case HAL_ETH_MSPDEINIT_CB_ID:
                heth->MspDeInitCallback = HAL_ETH_MspDeInit;
                break;

            default:
                /* Return error status */
                status = HAL_ERROR;
                break;
        }
    }
    else
    {
        /* Return error status */
        status = HAL_ERROR;
    }

    return status;
}
#endif /* configUSE_ETH_REGISTER_CALLBACKS */

/**
 * @}
 */

/** @defgroup ETH_Exported_Functions_Group2 IO operation functions
  *  @brief   Data transfers functions
  *
  @verbatim
  ==============================================================================
                          ##### IO operation functions #####
  ==============================================================================
  [..]  This section provides functions allowing to:
        (+) Transmit a frame
            HAL_ETH_TransmitFrame();
        (+) Receive a frame
            HAL_ETH_GetReceivedFrame();
            HAL_ETH_GetReceivedFrame_IT();
        (+) Read from an External PHY register
            HAL_ETH_ReadPHYRegister();
        (+) Write to an External PHY register
            HAL_ETH_WritePHYRegister();

  @endverbatim

  * @{
  */
/**
 * @brief  Sends an Ethernet frame.
 * @param  heth pointer to a ETH_Handle_T structure that contains
 *         the configuration information for ETHERNET module
 * @param  FrameLength Amount of data to be sent
 * @retval HAL status
 */
HAL_Status_T HAL_ETH_TransmitFrame(ETH_Handle_T *heth, uint32_t FrameLength)
{
    __IO ETH_DMADescTypeDef *DmaTxDesc;
    DmaTxDesc = heth->TxDesc;

    /* Set the ETH peripheral state to BUSY */
    heth->State = HAL_ETH_STATE_BUSY;

    /* in this function, tx is always completed by one descriptor */
    if (DmaTxDesc->StatusAndVDBC & ETH_TXDES0_TXDMA_OWN)
    {
        ETH_PRINT(ETH_DBG_ERR, "??? => Tx descriptor 0x%x own by HW\r\n", (uintptr_t)DmaTxDesc);
        return HAL_ERROR;
    }

    /* initial txdes */
    //	DmaTxDesc->xdes3 = (unsigned int) p->payload;
    DmaTxDesc->StatusAndVLAN = ETH_TXDES1_TXIC | ETH_TXDES1_TX2FIC;

#ifndef ETH_CHECKSUM_OFFLOAD_DISABLE
    /*
     * If checksum offlad function is enabled, software must be certain that the transmit packet is an IPV4/6 packet.
     */

    struct eth_hdr *ethhdr;
    ethhdr = (struct eth_hdr *)DmaTxDesc->Buffer1Addr;

    if (PP_HTONS(ethhdr->type) == ETHTYPE_IP)
        DmaTxDesc->StatusAndVLAN |= ETH_TXDES1_IPCS_EN | ETH_TXDES1_TCPCS_EN | ETH_TXDES1_UDPCS_EN;
#endif
    /* reset tStatusAndVDBC */
    DmaTxDesc->StatusAndVDBC &= ETH_TXDES0_EDOTR;

    DmaTxDesc->StatusAndVDBC |= ETH_TXDES0_BUF_SIZE(FrameLength) |
                                ETH_TXDES0_FTS |
                                ETH_TXDES0_LTS |
                                ETH_TXDES0_TXDMA_OWN;

    heth->TxDesc = (ETH_DMADescTypeDef *)((uintptr_t)DmaTxDesc->Buffer2NextDescAddr);

    if (0 == ((heth->Instance)->APTC & ETH_APTC_TX_CNT(0xf)))
        /* kick off normal priority txdma */
        (heth->Instance)->NPTXPD = 0xffffffff;

    /* Set ETH HAL State to Ready */
    heth->State = HAL_ETH_STATE_READY;

    /* Return function status */
    return HAL_OK;
}

/**
 * @brief  Checks for received frames.
 * @param  heth pointer to a ETH_Handle_T structure that contains
 *         the configuration information for ETHERNET module
 * @retval HAL status
 */
HAL_Status_T HAL_ETH_GetReceivedFrame(ETH_Handle_T *heth)
{
    ETH_DMADescTypeDef *DMARxDesc = NULL;

    /* Check the ETH state to BUSY */
    heth->State = HAL_ETH_STATE_BUSY;

    DMARxDesc = heth->RxDesc;

    if ((DMARxDesc->StatusAndVDBC & ETH_RXDES0_RXPKT_RDY))
    {
        /* Get the Frame Length of the received packet: substruct 4 bytes of the CRC */
        /* Check if one segment in frame */
        if ((heth->RxDesc->StatusAndVDBC & ETH_RXDES0_FRS) || (heth->RxDesc->StatusAndVDBC & ETH_RXDES0_LRS))
        {
            /* Last segment */
            heth->RxFrameInfos.LSRxDesc = heth->RxDesc;

            heth->RxFrameInfos.FSRxDesc = heth->RxDesc;

            /* Increment segment count */
            heth->RxFrameInfos.SegCount = 1U;

            /* Get the Frame Length of the received packet: substruct 4 bytes of the CRC */
            heth->RxFrameInfos.length = ETH_RXDES0_VDBC(heth->RxDesc->StatusAndVDBC) - 4U;

            /* Get the address of the buffer start address */
            heth->RxFrameInfos.buffer = ((heth->RxFrameInfos).FSRxDesc)->Buffer1Addr;

            /* Point to next descriptor */
            heth->RxDesc = (ETH_DMADescTypeDef *)(heth->RxDesc->Buffer2NextDescAddr);

            /* Set HAL State to Ready */
            heth->State = HAL_ETH_STATE_READY;

            /* Return function status */
            return HAL_OK;
        }
        /* Check if first segment in frame */
        else if (heth->RxDesc->StatusAndVDBC & ETH_RXDES0_FRS)
        {
            heth->RxFrameInfos.FSRxDesc = heth->RxDesc;
            heth->RxFrameInfos.SegCount = 1U;
            /* Point to next descriptor */
            heth->RxDesc = (ETH_DMADescTypeDef *)(heth->RxDesc->Buffer2NextDescAddr);
        }
        /* Check if intermediate segment */
        /* ((heth->RxDesc->Status & ETH_DMARXDESC_LS) == (uint32_t)RESET)&& ((heth->RxDesc->Status & ETH_DMARXDESC_FS) == (uint32_t)RESET)) */
        else if (!(heth->RxDesc->StatusAndVDBC & (ETH_RXDES0_FRS | ETH_RXDES0_LRS)))
        {
            /* Increment segment count */
            (heth->RxFrameInfos.SegCount)++;
            /* Point to next descriptor */
            heth->RxDesc = (ETH_DMADescTypeDef *)(heth->RxDesc->Buffer2NextDescAddr);
        }
        /* Should be last segment */
        else
        {
            /* Last segment */
            heth->RxFrameInfos.LSRxDesc = heth->RxDesc;

            /* Increment segment count */
            (heth->RxFrameInfos.SegCount)++;

            /* Check if last segment is first segment: one segment contains the frame */
            if ((heth->RxFrameInfos.SegCount) == 1U)
            {
                heth->RxFrameInfos.FSRxDesc = heth->RxDesc;
            }

            /* Get the Frame Length of the received packet: substruct 4 bytes of the CRC */
            heth->RxFrameInfos.length = ETH_RXDES0_VDBC(heth->RxDesc->StatusAndVDBC) - 4U;

            /* Get the address of the buffer start address */
            heth->RxFrameInfos.buffer = ((heth->RxFrameInfos).FSRxDesc)->Buffer1Addr;

            /* Point to next descriptor */
            heth->RxDesc = (ETH_DMADescTypeDef *)(heth->RxDesc->Buffer2NextDescAddr);

            /* Set HAL State to Ready */
            heth->State = HAL_ETH_STATE_READY;

            /* Return function status */
            return HAL_OK;
        }
    }

    /* Set ETH HAL State to Ready */
    heth->State = HAL_ETH_STATE_READY;

    /* Return function status */
    return HAL_ERROR;
}

/**
 * @brief  Gets the Received frame in interrupt mode.
 * @param  heth pointer to a ETH_Handle_T structure that contains
 *         the configuration information for ETHERNET module
 * @retval HAL status
 */
HAL_Status_T HAL_ETH_GetReceivedFrame_IT(ETH_Handle_T *heth)
{
    uint32_t descriptorscancounter = 0U;

    /* Check the ETH state to BUSY */
    heth->State = HAL_ETH_STATE_BUSY;

    /* Set ETH HAL State to BUSY */
    heth->State = HAL_ETH_STATE_BUSY;

    heth->RxFrameInfos.SegCount = 0;

    /* Scan descriptors owned by CPU */
    while (((heth->RxDesc->StatusAndVDBC & ETH_RXDES0_RXPKT_RDY)) && (descriptorscancounter < ETH_RXBUFNB))
    {
        /* Just for security */
        descriptorscancounter++;
        /* Check if one segment in frame */
        if ((heth->RxDesc->StatusAndVDBC & ETH_RXDES0_FRS) || (heth->RxDesc->StatusAndVDBC & ETH_RXDES0_LRS))
        {
            /* Last segment */
            heth->RxFrameInfos.LSRxDesc = heth->RxDesc;

            heth->RxFrameInfos.FSRxDesc = heth->RxDesc;

            /* Increment segment count */
            heth->RxFrameInfos.SegCount = 1U;

            /* Get the Frame Length of the received packet: substruct 4 bytes of the CRC */
            heth->RxFrameInfos.length = ETH_RXDES0_VDBC(heth->RxDesc->StatusAndVDBC) - 4U;

            /* Get the address of the buffer start address */
            heth->RxFrameInfos.buffer = ((heth->RxFrameInfos).FSRxDesc)->Buffer1Addr;

            /* Point to next descriptor */
            heth->RxDesc = (ETH_DMADescTypeDef *)(heth->RxDesc->Buffer2NextDescAddr);

            /* Set HAL State to Ready */
            heth->State = HAL_ETH_STATE_READY;

            /* Return function status */
            return HAL_OK;
        }
        /* Check if first segment in frame */
        else if (heth->RxDesc->StatusAndVDBC & ETH_RXDES0_FRS)
        {
            heth->RxFrameInfos.FSRxDesc = heth->RxDesc;
            heth->RxFrameInfos.SegCount = 1U;
            /* Point to next descriptor */
            heth->RxDesc = (ETH_DMADescTypeDef *)(heth->RxDesc->Buffer2NextDescAddr);
        }
        /* Check if intermediate segment */
        /* ((heth->RxDesc->Status & ETH_DMARXDESC_LS) == (uint32_t)RESET)&& ((heth->RxDesc->Status & ETH_DMARXDESC_FS) == (uint32_t)RESET)) */
        else if (!(heth->RxDesc->StatusAndVDBC & (ETH_RXDES0_FRS | ETH_RXDES0_LRS)))
        {
            /* Increment segment count */
            (heth->RxFrameInfos.SegCount)++;
            /* Point to next descriptor */
            heth->RxDesc = (ETH_DMADescTypeDef *)(heth->RxDesc->Buffer2NextDescAddr);
        }
        /* Should be last segment */
        else
        {
            /* Last segment */
            heth->RxFrameInfos.LSRxDesc = heth->RxDesc;

            /* Increment segment count */
            (heth->RxFrameInfos.SegCount)++;

            /* Check if last segment is first segment: one segment contains the frame */
            if ((heth->RxFrameInfos.SegCount) == 1U)
            {
                heth->RxFrameInfos.FSRxDesc = heth->RxDesc;
            }

            /* Get the Frame Length of the received packet: substruct 4 bytes of the CRC */
            heth->RxFrameInfos.length = ETH_RXDES0_VDBC(heth->RxDesc->StatusAndVDBC) - 4U;

            /* Get the address of the buffer start address */
            heth->RxFrameInfos.buffer = ((heth->RxFrameInfos).FSRxDesc)->Buffer1Addr;

            /* Point to next descriptor */
            heth->RxDesc = (ETH_DMADescTypeDef *)(heth->RxDesc->Buffer2NextDescAddr);

            /* Set HAL State to Ready */
            heth->State = HAL_ETH_STATE_READY;

            /* Return function status */
            return HAL_OK;
        }
    }

    /* Set HAL State to Ready */
    heth->State = HAL_ETH_STATE_READY;

    /* Return function status */
    return HAL_ERROR;
}

void ETH_PhyProcessing(ETH_Handle_T *heth)
{
    SN_ETHMAC_Type *SN_ETH = heth->Instance;
    int             maccr;
#if 1
    uint16_t status, adv;
    uint16_t lpa;
    uint16_t lpagb = 0;
    uint16_t retry = 10240;
    int      common_adv;
    int      common_adv_gb = 0;

read_bsr:
    /* Do a fake read */
    HAL_ETH_ReadPHYRegister(heth, PHY_BSR, &status);

    /* Read link and autonegotiation status */
    HAL_ETH_ReadPHYRegister(heth, PHY_BSR, &status);

    if ((status & BSR_LSTATUS) == 0)
    {
        if (retry > 0)
        {
            retry--;
            goto read_bsr;
        }
        return;
    }

    HAL_ETH_ReadPHYRegister(heth, PHY_LPA, &lpa);

    maccr = HAL_REG_READ(SN_ETH->MACCR);
    maccr &= ~(ETH_MACCR_SPEED_MASK | ETH_MACCR_FULLDUP);

    if (lpa & LPA_LPACK)
    {
        if (status & BSR_ESTATEN)
        {
            HAL_ETH_ReadPHYRegister(heth, PHY_ESTATUS, &status);
            if (status & (ESTATUS_1000_TFULL | ESTATUS_1000_THALF))
            {
                HAL_ETH_ReadPHYRegister(heth, PHY_STAT1000, &lpagb);

                HAL_ETH_ReadPHYRegister(heth, PHY_CTRL1000, &adv);

                common_adv_gb = lpagb & (adv << 2);
            }
        }

        HAL_ETH_ReadPHYRegister(heth, PHY_LPA, &lpa);

        HAL_ETH_ReadPHYRegister(heth, PHY_ADVERTISE, &adv);

        common_adv = lpa & adv;

        if (common_adv_gb & (LPA_1000FULL | LPA_1000HALF))
        {
            maccr |= ETH_MACCR_SPEED_1000M;

            if (common_adv_gb & LPA_1000FULL)
                maccr |= ETH_MACCR_FULLDUP;
        }
        else if (common_adv & (LPA_100FULL | LPA_100HALF))
        {
            maccr |= ETH_MACCR_SPEED_100M;

            if (common_adv & LPA_100FULL)
                maccr |= ETH_MACCR_FULLDUP;
        }
        else
        {
            maccr |= ETH_MACCR_SPEED_10M;
            if (common_adv & LPA_10FULL)
                maccr |= ETH_MACCR_FULLDUP;
        }
    }
    else
    {
        uint16_t bcr;
        HAL_ETH_ReadPHYRegister(heth, PHY_BCR, &bcr);

        if (bcr & BCR_FULLDPLX)
            maccr |= ETH_MACCR_FULLDUP;

        if (bcr & BCR_SPEED1000)
            maccr |= ETH_MACCR_SPEED_1000M;
        else if (bcr & BCR_SPEED100)
            maccr |= ETH_MACCR_SPEED_100M;
        else
            maccr |= ETH_MACCR_SPEED_10M;
    }
#else
    maccr = mac_regs->maccr;
    maccr &= ~(ETH_MACCR_SPEED_MASK);
    maccr |= (ETH_MACCR_FULLDUP | ETH_MACCR_SPEED_1000);
#endif
    HAL_REG_WRITE(SN_ETH->MACCR, maccr);

#if (configUSE_ETH_DEUBG == 1)
    ETH_SHOW_LINK_STATUS(heth);
#endif

    return;
}

/**
 * @brief  This function handles ETH interrupt request.
 * @param  heth pointer to a ETH_Handle_T structure that contains
 *         the configuration information for ETHERNET module
 * @retval HAL status
 */
void HAL_ETH_IRQHandler(ETH_Handle_T *heth)
{
    uint32_t        int_status;
    SN_ETHMAC_Type *SN_ETH = heth->Instance;

    /* disable all interrupts for pollings */
    HAL_REG_WRITE(SN_ETH->IE, 0);
    int_status = HAL_REG_READ(SN_ETH->IS);
    /* write 1 clear to reset all interrupts */
    HAL_REG_WRITE(SN_ETH->IS, int_status);

    if (int_status & ETH_INT_HPTXBUF_UNAVA)
    {
    }

    if (int_status & ETH_INT_PHYSTS_CHG)
    {
        uint16_t phyreg;
        ETH_PRINT(ETH_DBG_INFO, "[INFO] Link status change");
        HAL_ETH_ReadPHYRegister(heth, PHY_DAVICOM_SPEC_INT, &phyreg);
//        ETH_PhyProcessing(heth);
#if (configUSE_ETH_REGISTER_CALLBACKS == 1)
        /*Call registered Receive complete callback*/
        heth->LinkChgCallback(heth);
#else
        /* Receive complete callback */
        HAL_ETH_LinkChgCallback(heth);
#endif /* USE_HAL_ETH_REGISTER_CALLBACKS */
    }

    if (int_status & ETH_INT_AHB_ERR)
    {
        ETH_PRINT(ETH_DBG_WARN, "[WARN] Bus error");
        //		ahb_err_handler();
    }

    if (int_status & ETH_INT_TPKT_LOST)
    {
        ETH_PRINT(ETH_DBG_WARN, "[WARN] Packets transmitted lost");
        //		tpkt_lost_handler();
    }

    if (int_status & ETH_INT_NPTXBUF_UNAVA)
    {
        //		nptxbuf_unava_handler();
    }

    if (int_status & ETH_INT_TPKT2E)
    {
#if (configUSE_ETH_REGISTER_CALLBACKS == 1)
        /*  Call resgistered Transfer complete callback*/
        heth->TxCpltCallback(heth);
#else
        /* Transfer complete callback */
        HAL_ETH_TxCpltCallback(heth);
#endif /* configUSE_ETH_REGISTER_CALLBACKS */

        /* Set HAL State to Ready */
        heth->State = HAL_ETH_STATE_READY;
    }

    if (int_status & ETH_INT_RPKT_LOST)
    {
        //		rpkt_lost_handler();
        ETH_PRINT(ETH_DBG_WARN, "[WARN] Rx fifo full");
    }

    if (int_status & ETH_INT_RXBUF_UNAVA)
    {
        //		rxbuf_unava_handler();
        ETH_PRINT(ETH_DBG_WARN, "[WARN] Rx buffer unavailable");
    }

    if (int_status & ETH_INT_RPKT2F)
    {
        //		rpkt2f_handler();
    }

    if (int_status & ETH_INT_RPKT2B)
    {
#if (configUSE_ETH_REGISTER_CALLBACKS == 1)
        /*Call registered Receive complete callback*/
        heth->RxCpltCallback(heth);
#else
        /* Receive complete callback */
        HAL_ETH_RxCpltCallback(heth);
#endif /* USE_HAL_ETH_REGISTER_CALLBACKS */

        /* Set HAL State to Ready */
        heth->State = HAL_ETH_STATE_READY;
    }

    /* enable interrupt */
    HAL_REG_WRITE(SN_ETH->IE, ETH_INT_DEFAULT);
}

/**
 * @brief  This function handles ETH WOL interrupt request.
 * @param  heth pointer to a ETH_Handle_T structure that contains
 *         the configuration information for ETHERNET module
 */
void HAL_ETH_WOL_IRQHandler(ETH_Handle_T *heth)
{
    uint32_t        wol_sts;
    SN_ETHMAC_Type *SN_ETH = heth->Instance;

    wol_sts = HAL_REG_READ(SN_ETH->WOLST);

#if (configUSE_ETH_REGISTER_CALLBACKS == 1)
    /*Call registered Receive wakeup callback*/
    heth->WakeUpCallback(heth);
#else
    /* Receive complete callback */
    HAL_ETH_WakeUpCallback(heth);
#endif /* USE_HAL_ETH_REGISTER_CALLBACKS */

    SN_ETH->WOLST = wol_sts;
}

/**
 * @brief  Tx Transfer completed callbacks.
 * @param  heth pointer to a ETH_Handle_T structure that contains
 *         the configuration information for ETHERNET module
 * @retval None
 */
__weak void HAL_ETH_TxCpltCallback(ETH_Handle_T *heth)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(heth);
    /* NOTE : This function Should not be modified, when the callback is needed,
    the HAL_ETH_TxCpltCallback could be implemented in the user file
    */
}

/**
 * @brief  Rx Transfer completed callbacks.
 * @param  heth pointer to a ETH_Handle_T structure that contains
 *         the configuration information for ETHERNET module
 * @retval None
 */
__weak void HAL_ETH_RxCpltCallback(ETH_Handle_T *heth)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(heth);
    /* NOTE : This function Should not be modified, when the callback is needed,
    the HAL_ETH_TxCpltCallback could be implemented in the user file
    */
}

/**
 * @brief  Ethernet transfer error callbacks
 * @param  heth pointer to a ETH_Handle_T structure that contains
 *         the configuration information for ETHERNET module
 * @retval None
 */
__weak void HAL_ETH_ErrorCallback(ETH_Handle_T *heth)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(heth);
    /* NOTE : This function Should not be modified, when the callback is needed,
    the HAL_ETH_TxCpltCallback could be implemented in the user file
    */
}

/**
 * @brief  ETH wake up callback
 * @param  heth: pointer to a ETH_Handle_T structure that contains
 *         the configuration information for ETHERNET module
 * @retval None
 */
__weak void HAL_ETH_WakeUpCallback(ETH_Handle_T *heth)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(heth);
    /* NOTE : This function Should not be modified, when the callback is needed,
              the HAL_ETH_WakeUpCallback could be implemented in the user file
     */
}

/**
 * @brief  ETH link change callback
 * @param  heth: pointer to a ETH_Handle_T structure that contains
 *         the configuration information for ETHERNET module
 * @retval None
 */
__weak void HAL_ETH_LinkChgCallback(ETH_Handle_T *heth)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(heth);
    /* NOTE : This function Should not be modified, when the callback is needed,
              the HAL_ETH_LinkChgCallback could be implemented in the user file
     */
}

/**
 * @brief  Reads a PHY register
 * @param  heth pointer to a ETH_Handle_T structure that contains
 *         the configuration information for ETHERNET module
 * @param  PHYReg PHY register address, is the index of one of the 32 PHY register.
 *                This parameter can be one of the following values:
 *                   PHY_BCR: Transceiver Basic Control Register,
 *                   PHY_BSR: Transceiver Basic Status Register.
 *                   More PHY register could be read depending on the used PHY
 * @param  RegValue PHY register value
 * @retval HAL status
 */
HAL_Status_T HAL_ETH_ReadPHYRegister(ETH_Handle_T *heth, uint16_t PHYReg, uint16_t *RegValue)
{
    uint32_t        tmpreg1   = 0U;
    uint32_t        tickstart = 0U;
    SN_ETHMAC_Type *SN_ETH    = heth->Instance;

    /* Check parameters */
    ASSERT_PARAM(IS_ETH_PHY_ADDRESS(heth->Init.PhyAddress));

    /* Check the ETH peripheral state */
    if (heth->State == HAL_ETH_STATE_BUSY_RD)
    {
        return HAL_BUSY;
    }
    /* Set ETH HAL State to BUSY_RD */
    heth->State = HAL_ETH_STATE_BUSY_RD;

    /* Get the ETHERNET MACMIIAR value */
    tmpreg1 = (ETH_PHYCR_PHYRD | ETH_PHYCR_REGAD(PHYReg) |
               ETH_PHYCR_PHYAD(heth->Init.PhyAddress) | ETH_PHYCR_OP(ETH_MDIO_OP_RD) |
               ETH_PHYCR_SOF(ETH_MDIO_SOF) | 0x34);

    /* Set the MII Busy bit         */

    /* Write the result value into the MII Address register */
    HAL_REG_WRITE(SN_ETH->PHYCTRL, tmpreg1);

    /* Get tick */
    tickstart = HAL_GetTick();

    /* Check for the Busy flag */
    while ((tmpreg1 & ETH_PHYCR_PHYRD) == ETH_PHYCR_PHYRD)
    {
        /* Check for the Timeout */
        if ((HAL_GetTick() - tickstart) > PHY_READ_TO)
        {
            heth->State = HAL_ETH_STATE_READY;

            return HAL_TIMEOUT;
        }

        tmpreg1 = HAL_REG_READ(SN_ETH->PHYCTRL);
    }

    /* Get MACMIIDR value */
    *RegValue = (uint16_t)ETH_PHYDATA_MIIRDATA(SN_ETH->PHYDATA);

    /* Set ETH HAL State to READY */
    heth->State = HAL_ETH_STATE_READY;

    /* Return function status */
    return HAL_OK;
}

/**
 * @brief  Writes to a PHY register.
 * @param  heth pointer to a ETH_Handle_T structure that contains
 *         the configuration information for ETHERNET module
 * @param  PHYReg PHY register address, is the index of one of the 32 PHY register.
 *          This parameter can be one of the following values:
 *             PHY_BCR: Transceiver Control Register.
 *             More PHY register could be written depending on the used PHY
 * @param  RegValue the value to write
 * @retval HAL status
 */
HAL_Status_T HAL_ETH_WritePHYRegister(ETH_Handle_T *heth, uint16_t PHYReg, uint16_t RegValue)
{
    SN_ETHMAC_Type *SN_ETH = heth->Instance;

    uint32_t tmpreg1   = 0U;
    uint32_t tickstart = 0U;

    /* Check parameters */
    ASSERT_PARAM(IS_ETH_PHY_ADDRESS(heth->Init.PhyAddress));

    /* Check the ETH peripheral state */
    if (heth->State == HAL_ETH_STATE_BUSY_WR)
    {
        return HAL_BUSY;
    }
    /* Set ETH HAL State to BUSY_WR */
    heth->State = HAL_ETH_STATE_BUSY_WR;

    /* Prepare the MII register address value */
    tmpreg1 = (ETH_PHYCR_PHYWR | ETH_PHYCR_REGAD(PHYReg) |
               ETH_PHYCR_PHYAD(heth->Init.PhyAddress) | ETH_PHYCR_OP(ETH_MDIO_OP_WR) |
               ETH_PHYCR_SOF(ETH_MDIO_SOF) | 0x34);

    /* Give the value to the MII data register */
    HAL_REG_WRITE(SN_ETH->PHYDATA, RegValue);

    /* Write the result value into the MII Address register */
    HAL_REG_WRITE(SN_ETH->PHYCTRL, tmpreg1);

    /* Get tick */
    tickstart = HAL_GetTick();

    /* Check for the Busy flag */
    while ((tmpreg1 & ETH_PHYCR_PHYWR) == ETH_PHYCR_PHYWR)
    {
        /* Check for the Timeout */
        if ((HAL_GetTick() - tickstart) > PHY_WRITE_TO)
        {
            heth->State = HAL_ETH_STATE_READY;

            return HAL_TIMEOUT;
        }

        tmpreg1 = HAL_REG_READ(SN_ETH->PHYCTRL);
    }

    /* Set ETH HAL State to READY */
    heth->State = HAL_ETH_STATE_READY;

    /* Return function status */
    return HAL_OK;
}

/**
 * @}
 */

/** @defgroup ETH_Exported_Functions_Group3 Peripheral Control functions
 *  @brief    Peripheral Control functions
 *
@verbatim
 ===============================================================================
                  ##### Peripheral Control functions #####
 ===============================================================================
    [..]  This section provides functions allowing to:
      (+) Enable MAC and DMA transmission and reception.
          HAL_ETH_Start();
      (+) Disable MAC and DMA transmission and reception.
          HAL_ETH_Stop();
      (+) Set the MAC configuration in runtime mode
          HAL_ETH_ConfigMAC();
      (+) Set the DMA configuration in runtime mode
          HAL_ETH_ConfigDMA();

@endverbatim
  * @{
  */

/**
 * @brief  Enables Ethernet MAC and DMA reception/transmission
 * @param  heth pointer to a ETH_Handle_T structure that contains
 *         the configuration information for ETHERNET module
 * @retval HAL status
 */
HAL_Status_T HAL_ETH_Start(ETH_Handle_T *heth)
{
    /* Process Locked */
    //  __HAL_LOCK(heth);

    /* Set the ETH peripheral state to BUSY */
    heth->State = HAL_ETH_STATE_BUSY;

    /* Set interrupt timer control */
    HAL_REG_WRITE((heth->Instance)->TXITC, ETH_TXITC_DEFAULT);
    HAL_REG_WRITE((heth->Instance)->RXITC, ETH_RXITC_DEFAULT);

    /* Set auto polling timer control */
    HAL_REG_WRITE((heth->Instance)->APTC, ETH_APTC_DEFAULT);

    /* Enable transmit state machine of the MAC for transmission on the MII */
    ETH_MACTransmissionEnable(heth);

    /* Enable receive state machine of the MAC for reception from the MII */
    ETH_MACReceptionEnable(heth);

    /* Start DMA transmission */
    ETH_DMATransmissionEnable(heth);

    /* Start DMA reception */
    ETH_DMAReceptionEnable(heth);

    /* Set the ETH state to READY*/
    heth->State = HAL_ETH_STATE_READY;

#if (ETH_DEBUG == 1)
    ETH_DUMP_MAC_REG(heth);
    ETH_DUMP_PHY_REG(heth);
    ETH_SHOW_LINK_STATUS(heth);
#endif

    /* Return function status */
    return HAL_OK;
}

/**
 * @brief  Stop Ethernet MAC and DMA reception/transmission
 * @param  heth pointer to a ETH_Handle_T structure that contains
 *         the configuration information for ETHERNET module
 * @retval HAL status
 */
HAL_Status_T HAL_ETH_Stop(ETH_Handle_T *heth)
{
    /* Set the ETH peripheral state to BUSY */
    heth->State = HAL_ETH_STATE_BUSY;

    /* Stop DMA transmission */
    ETH_DMATransmissionDisable(heth);

    /* Stop DMA reception */
    ETH_DMAReceptionDisable(heth);

    /* Disable receive state machine of the MAC for reception from the MII */
    ETH_MACReceptionDisable(heth);

    /* Disable transmit state machine of the MAC for transmission on the MII */
    ETH_MACTransmissionDisable(heth);

    /* Set the ETH state*/
    heth->State = HAL_ETH_STATE_READY;

    /* Return function status */
    return HAL_OK;
}

/**
 * @brief  Set ETH MAC Configuration.
 * @param  heth pointer to a ETH_Handle_T structure that contains
 *         the configuration information for ETHERNET module
 * @param  macconf MAC Configuration structure
 * @retval HAL status
 */
HAL_Status_T HAL_ETH_ConfigMAC(ETH_Handle_T *heth)
{
    uint32_t maccr;

    /* Set the ETH peripheral state to BUSY */
    heth->State = HAL_ETH_STATE_BUSY;

    ASSERT_PARAM(IS_ETH_SPEED(heth->Init.Speed));
    ASSERT_PARAM(IS_ETH_DUPLEX_MODE(heth->Init.DuplexMode));

    SN_ETHMAC_Type *SN_ETH = heth->Instance;

    /* Ethernet MAC config speed and duplex mode **************************************/

    maccr = HAL_REG_READ(SN_ETH->MACCR);
    maccr &= ~(ETH_MACCR_SPEED_MASK | ETH_MACCR_FULLDUP);

    maccr |= (heth->Init).Speed | (heth->Init).DuplexMode;

    /* Write to ETHERNET MACCR */
    HAL_REG_WRITE(SN_ETH->MACCR, maccr);

    /* Set the ETH state to Ready */
    heth->State = HAL_ETH_STATE_READY;

    /* Return function status */
    return HAL_OK;
}

/**
 * @brief  Enters the Power down mode.
 * @param  heth: pointer to a ETH_Handle_T structure that contains
 *         the configuration information for ETHERNET module
 * @param  pPowerDownConfig: a pointer to ETH_PowerDownConfigTypeDef structure
 *         that contains the Power Down configuration
 * @retval None.
 */
void HAL_ETH_EnterPowerDownMode(ETH_Handle_T *heth, ETH_PowerDownConfigTypeDef *pPowerDownConfig)
{
    SN_ETHMAC_Type *SN_ETH = heth->Instance;

    /* set TXDMA_EN (50h.0) = 0 */
    HAL_REG_WRITE(SN_ETH->MACCR, SN_ETH->MACCR & ~ETH_MACCR_TXDMA);

    /* poll the dma/fifo state register (3Ch) to wait for tx fifo empty*/
    while (!HAL_REG_RBIT(SN_ETH->DMAFIFOS, ETH_DMAFIFOS_TXFIFO_EMPTY))
        ;

    /* set TXMAC_EN (50h.2) = 0 and RXMAC_EN (50h.3) to terminate the tx/rx */
    HAL_REG_WRITE(SN_ETH->MACCR, SN_ETH->MACCR & (~(ETH_MACCR_TXMAC | ETH_MACCR_RXMAC)));

    /* poll the dma/fifo state register (3Ch) to wait for rx fifo empty */
    while (!HAL_REG_RBIT(SN_ETH->DMAFIFOS, ETH_DMAFIFOS_RXFIFO_EMPTY))
        ;

    /* set RXDMA_EN (50h.1) = 0 to stop reception */
    HAL_REG_WRITE(SN_ETH->MACCR, SN_ETH->MACCR & ~ETH_MACCR_RXDMA);

    /* write 32'hFFFF_FFFF to clear wake-on-lan status register (74h) */
    HAL_REG_WRITE(SN_ETH->WOLST, 0xFFFFFFFF);

    /* set RXMAC_EN (50h.3) = 1 to enable the reception (RCV_ALL has to be enable, too) */
    HAL_REG_SBIT(SN_ETH->MACCR, ETH_MACCR_RXMAC | ETH_MACCR_ALLADDR);

    /* Enable ethernet clk in sleep mode */
    __HAL_RCC_ETHERNET_CLK_SLEEP_ENABLE();

    /* Enalble wol irq */
    NVIC_SetPriority(ETH_WOL_IRQn, 4);
    NVIC_ClearPendingIRQ(ETH_WOL_IRQn);
    NVIC_EnableIRQ(ETH_WOL_IRQn);
}

/**
 * @brief  Exits from the Power down mode.
 * @param  heth: pointer to a ETH_Handle_T structure that contains
 *         the configuration information for ETHERNET module
 * @retval None.
 */
void HAL_ETH_ExitPowerDownMode(ETH_Handle_T *heth)
{
    SN_ETHMAC_Type *SN_ETH = heth->Instance;

    /* set RXMAC_EN (50h.3) = 0 to terminate the reception */
    HAL_REG_WRITE(SN_ETH->MACCR, SN_ETH->MACCR & ~ETH_MACCR_RXMAC);
    /* read the Wake-On-LAN status register (74h) to check which wake-up event happened */

    /* program the Wake-On-LAN register (70h) to let FTMAC110 exit the power-saving mode */
    HAL_REG_WRITE(SN_ETH->WOLCTRL, SN_ETH->WOLCTRL & ~0x7f);
    /* set SW_RST (50h.31) = 1 to reset ftgmac030 */
    HAL_REG_WRITE(SN_ETH->MACCR, ETH_MACCR_SW_RST);

    /* Wait for software reset */
    while (HAL_REG_RBIT(SN_ETH->MACCR, ETH_MACCR_SW_RST))
    {
    }

    /* Enable ethernet clk in sleep mode */
    __HAL_RCC_ETHERNET_CLK_SLEEP_DISABLE();

    /* Enalble wol irq */
    NVIC_DisableIRQ(ETH_WOL_IRQn);

    /* re-initialize FTMAC110 to transmit and receive packets */
    HAL_ETH_Init(heth);

    HAL_REG_WRITE(heth->Instance->NPTXR_BADR, (uint32_t)heth->TxDMADescBaseAddr);
    heth->TxDesc = heth->TxDMADescBaseAddr;
    HAL_REG_WRITE(heth->Instance->RXR_BADR, (uint32_t)heth->RxDMADescBaseAddr);
    heth->RxDesc = heth->RxDMADescBaseAddr;

    HAL_ETH_Start(heth);
}

/**
 * @brief  Set the WakeUp filter.
 * @param  heth: pointer to a ETH_Handle_T structure that contains
 *         the configuration information for ETHERNET module
 * @param  pFilter: pointer to filter registers values
 * @param  Count: number of filter registers, must be from 1 to 8.
 * @retval None.
 */
HAL_Status_T HAL_ETH_SetWakeUpFilter(ETH_Handle_T *heth, ETH_PowerDownConfigTypeDef *pPowerDownConfig)
{
    SN_ETHMAC_Type *SN_ETH = heth->Instance;

    if (pPowerDownConfig->LinkDetect)
    {
        HAL_REG_SBIT(SN_ETH->WOLCTRL, (ETH_WOLCR_LINKCHG0 | ETH_WOLCR_LINKCHG1));
    }

    if (pPowerDownConfig->MagicPacket)
    {
        HAL_REG_SBIT(SN_ETH->WOLCTRL, ETH_WOLCR_MAGICPKT);
    }

    if (pPowerDownConfig->WakeUpFrame)
    {
        /* Set frame select */
        HAL_REG_SBIT(SN_ETH->WOLCTRL_b.WAKEUP_SEL, pPowerDownConfig->FrameNumber);

        /* Set frame mask */
        HAL_REG_WRITE(SN_ETH->WFBM1, pPowerDownConfig->WakeUpFrameMask[0]);
        HAL_REG_WRITE(SN_ETH->WFBM2, pPowerDownConfig->WakeUpFrameMask[1]);
        HAL_REG_WRITE(SN_ETH->WFBM3, pPowerDownConfig->WakeUpFrameMask[2]);
        HAL_REG_WRITE(SN_ETH->WFBM4, pPowerDownConfig->WakeUpFrameMask[3]);

        /* calculate crc value */
        HAL_REG_WRITE(SN_ETH->WFCRC, ETH_WakeUpCRC32(pPowerDownConfig->pFrame, pPowerDownConfig->FrameSize,
                                                     pPowerDownConfig->WakeUpFrameMask));
        HAL_REG_SBIT(SN_ETH->WOLCTRL, ETH_WOLCR_WAKEUP1 << pPowerDownConfig->FrameNumber);
    }

    return HAL_OK;
}

/**
 * @}
 */

/** @defgroup ETH_Exported_Functions_Group4 Peripheral State functions
  *  @brief   Peripheral State functions
  *
  @verbatim
  ===============================================================================
                         ##### Peripheral State functions #####
  ===============================================================================
  [..]
  This subsection permits to get in run-time the status of the peripheral
  and the data flow.
       (+) Get the ETH handle state:
           HAL_ETH_GetState();


  @endverbatim
  * @{
  */

/**
 * @brief  Return the ETH HAL state
 * @param  heth pointer to a ETH_Handle_T structure that contains
 *         the configuration information for ETHERNET module
 * @retval HAL state
 */
HAL_ETH_StateTypeDef HAL_ETH_GetState(ETH_Handle_T *heth)
{
    /* Return ETH state */
    return heth->State;
}

/**
 * @}
 */

/**
 * @}
 */

/** @addtogroup ETH_Private_Functions
 * @{
 */

/**
 * @brief  Configures Ethernet MAC and DMA with default parameters.
 * @param  heth pointer to a ETH_Handle_T structure that contains
 *         the configuration information for ETHERNET module
 * @param  err Ethernet Init error
 * @retval HAL status
 */
static void ETH_MACDMAConfig(ETH_Handle_T *heth)
{
    uint32_t        tmpreg1 = 0U;
    SN_ETHMAC_Type *SN_ETH  = heth->Instance;

    /* Ethernet MAC default initialization **************************************/
    //  macinit.LoopbackMode = ETH_LOOPBACKMODE_DISABLE;
    //  if (heth->Init.ChecksumMode == ETH_CHECKSUM_BY_HARDWARE)
    //  {
    //    macinit.ChecksumOffload = ETH_CHECKSUMOFFLAOD_ENABLE;
    //  }
    //  else
    //  {
    //    macinit.ChecksumOffload = ETH_CHECKSUMOFFLAOD_DISABLE;
    //  }

    tmpreg1 = ETH_MACCR_RX_RUNT |
              ETH_MACCR_CRC_APD |
              (heth->Init).DuplexMode |
              ETH_MACCR_HPTXR |
              ETH_MACCR_ALLADDR |
              (heth->Init).Speed;

    /* Write to ETHERNET MACCR */
    HAL_REG_WRITE(SN_ETH->MACCR, tmpreg1);

    /* Initialize MAC address in ethernet MAC */
    ETH_MACAddressConfig(heth, heth->Init.MACAddr);

    /* Enable interrupt */
    HAL_REG_WRITE((heth->Instance)->IE, ETH_INT_DEFAULT);
}

/**
 * @brief  Calculate wake up frame crc32
 * @param  WakeupFrame pointer to a wakeup frame that contains
 *         32 bytes data
 * @param  Size The size of wakeup frame
 * @param  Mask Pointer to mask of the wakeup frame
 * @retval CRC32 Value
 */

uint8_t _8_bit_reverse(uint8_t value)
{
    uint8_t r_value = 0x0;
    int     i;
    for (i = 7; i >= 0; i--)
    {
        r_value |= ((value & (1 << i)) >> i) << (7 - i);
    }
    return r_value;
}

static uint32_t ETH_WakeUpCRC32(uint8_t *WakeupFrame, uint32_t Size, uint32_t *Mask)
{
    int            perByte;
    int            perBit;
    const uint32_t poly      = 0x04C11DB7;
    uint32_t       crc_value = 0xFFFFFFFF;
    uint8_t        c;
    for (perByte = 0; perByte < Size; perByte++)
    {
        if (perByte >= 128)
        {
            break;
        }

        if ((1 << (perByte % 32)) & Mask[perByte >> 5])
        {
            c = *(WakeupFrame++);
            for (perBit = 0; perBit < 8; perBit++)
            {
                crc_value = (crc_value << 1) ^
                            ((((crc_value >> 31) ^ c) & 0x01) ? poly : 0);
                c >>= 1;
            }
        }
        else
        {
            WakeupFrame++;
        }
    }

    /* covert CRC */
    crc_value = ~crc_value;
    crc_value = _8_bit_reverse(crc_value & 0xff) |
                (_8_bit_reverse((crc_value >> 8) & 0xff) << 8) |
                (_8_bit_reverse((crc_value >> 16) & 0xff) << 16) |
                (_8_bit_reverse((crc_value >> 24) & 0xff) << 24);

    return (crc_value);
}

/**
 * @brief  Configures the selected MAC address.
 * @param  heth pointer to a ETH_Handle_T structure that contains
 *         the configuration information for ETHERNET module
 * @param  MacAddr The MAC address to configure
 *          This parameter can be one of the following values:
 *             @arg ETH_MAC_Address0: MAC Address0
 *             @arg ETH_MAC_Address1: MAC Address1
 *             @arg ETH_MAC_Address2: MAC Address2
 *             @arg ETH_MAC_Address3: MAC Address3
 * @param  Addr Pointer to MAC address buffer data (6 bytes)
 * @retval HAL status
 */
static void ETH_MACAddressConfig(ETH_Handle_T *heth, uint8_t *Addr)
{
    SN_ETHMAC_Type *SN_ETH = heth->Instance;

    /* Prevent unused argument(s) compilation warning */
    UNUSED(heth);

    SN_ETH->MAC_LADR = (uint32_t)(*(uint32_t *)Addr) & 0xffffffff;
    SN_ETH->MAC_MADR = (uint32_t)(*(uint32_t *)(Addr + 4)) & 0xffff;
}

/**
 * @brief  Enables the MAC transmission.
 * @param  heth pointer to a ETH_Handle_T structure that contains
 *         the configuration information for ETHERNET module
 * @retval None
 */
static void ETH_MACTransmissionEnable(ETH_Handle_T *heth)
{
    /* Enable the MAC transmission */
    (heth->Instance)->MACCR |= ETH_MACCR_TXMAC;
}

/**
 * @brief  Disables the MAC transmission.
 * @param  heth pointer to a ETH_Handle_T structure that contains
 *         the configuration information for ETHERNET module
 * @retval None
 */
static void ETH_MACTransmissionDisable(ETH_Handle_T *heth)
{
    /* Disable the MAC transmission */
    (heth->Instance)->MACCR &= ~ETH_MACCR_TXMAC;
}

/**
 * @brief  Enables the MAC reception.
 * @param  heth pointer to a ETH_Handle_T structure that contains
 *         the configuration information for ETHERNET module
 * @retval None
 */
static void ETH_MACReceptionEnable(ETH_Handle_T *heth)
{
    /* Enable the MAC reception */
    (heth->Instance)->MACCR |= ETH_MACCR_RXMAC;
}

/**
 * @brief  Disables the MAC reception.
 * @param  heth pointer to a ETH_Handle_T structure that contains
 *         the configuration information for ETHERNET module
 * @retval None
 */
static void ETH_MACReceptionDisable(ETH_Handle_T *heth)
{
    /* Disable the MAC reception */
    (heth->Instance)->MACCR &= ~ETH_MACCR_RXMAC;
}

/**
 * @brief  Enables the DMA transmission.
 * @param  heth pointer to a ETH_Handle_T structure that contains
 *         the configuration information for ETHERNET module
 * @retval None
 */
static void ETH_DMATransmissionEnable(ETH_Handle_T *heth)
{
    /* Enable the DMA transmission */
    (heth->Instance)->MACCR |= ETH_MACCR_TXDMA;
}

/**
 * @brief  Disables the DMA transmission.
 * @param  heth pointer to a ETH_Handle_T structure that contains
 *         the configuration information for ETHERNET module
 * @retval None
 */
static void ETH_DMATransmissionDisable(ETH_Handle_T *heth)
{
    /* Disable the DMA transmission */
    (heth->Instance)->MACCR &= ~ETH_MACCR_TXDMA;
}

/**
 * @brief  Enables the DMA reception.
 * @param  heth pointer to a ETH_Handle_T structure that contains
 *         the configuration information for ETHERNET module
 * @retval None
 */
static void ETH_DMAReceptionEnable(ETH_Handle_T *heth)
{
    /* Enable the DMA reception */
    (heth->Instance)->MACCR |= ETH_MACCR_RXDMA;
}

/**
 * @brief  Disables the DMA reception.
 * @param  heth pointer to a ETH_Handle_T structure that contains
 *         the configuration information for ETHERNET module
 * @retval None
 */
static void ETH_DMAReceptionDisable(ETH_Handle_T *heth)
{
    /* Disable the DMA reception */
    (heth->Instance)->MACCR &= ~ETH_MACCR_RXDMA;
}

static HAL_Status_T ETH_PHYAutoNeo(ETH_Handle_T *heth)
{
    /* kick off phy auto negotiation */
    uint16_t bcr, bsr;
    uint32_t retry = 100;

    HAL_ETH_ReadPHYRegister(heth, PHY_BCR, &bcr);

    bcr |= (BCR_ANENABLE | BCR_ANRESTART);

    /* Don't isolate the PHY if we're negotiating */
    bcr &= ~BCR_ISOLATE;

    HAL_ETH_WritePHYRegister(heth, PHY_BCR, bcr);

    do
    {
        HAL_Delay(100);
        HAL_ETH_ReadPHYRegister(heth, PHY_BSR, &bsr);

        if (bsr & BSR_ANEGCOMPLETE)
        {
            return HAL_OK;
        }
    } while (retry-- > 0);
    return HAL_ERROR;
}

// static HAL_Status_T ETH_DuplexSpeedCfg(ETH_Handle_T *heth, uint32_t duplex, uint32_t speed)
HAL_Status_T ETH_DuplexSpeedCfg(ETH_Handle_T *heth, uint32_t duplex, uint32_t speed)
{
    uint16_t        adv;
    uint16_t        lpa;
    SN_ETHMAC_Type *SN_ETH = heth->Instance;

    SN_ETH->IE &= ~ETH_INT_PHYSTS_CHG;

    if (speed == ETH_MACCR_SPEED_1000M)
    {
        HAL_ETH_ReadPHYRegister(heth, PHY_STAT1000, &lpa);

        HAL_ETH_ReadPHYRegister(heth, PHY_CTRL1000, &adv);

        adv &= ~(ADVERTISE_1000FULL | ADVERTISE_1000HALF);

        if (duplex == ETH_MODE_FULLDUPLEX)
            adv |= ADVERTISE_1000FULL;
        else
            adv |= ADVERTISE_1000HALF;

        if (!(lpa & (adv << 2)))
        {
            printf("partner doesnot support 1000 Full/Half duplex.\n");
            goto error;
        }

        HAL_ETH_WritePHYRegister(heth, PHY_CTRL1000, adv);
    }
    else
    {
        HAL_ETH_ReadPHYRegister(heth, PHY_CTRL1000, &adv);
        adv &= ~(ADVERTISE_1000FULL | ADVERTISE_1000HALF); // clear [8]_1000baseT, [9]_1000baseT_FDX
        HAL_ETH_WritePHYRegister(heth, PHY_CTRL1000, adv); // R9_MSCR

        HAL_ETH_ReadPHYRegister(heth, PHY_LPA, &lpa);

        HAL_ETH_ReadPHYRegister(heth, PHY_ADVERTISE, &adv);

        adv &= ~ADVERTISE_ALL;

        if (speed == ETH_MACCR_SPEED_100M)
        {
            if (duplex == ETH_MODE_FULLDUPLEX)
                adv |= ADVERTISE_100FULL;
            else
                adv |= ADVERTISE_100HALF;
        }
        else if (speed == ETH_MACCR_SPEED_10M)
        {
            if (duplex == ETH_MODE_FULLDUPLEX)
                adv |= ADVERTISE_10FULL;
            else
                adv |= ADVERTISE_10HALF;
        }
        else
        {
            printf("Argumnent value speed error!\n");
            goto error;
        }

        if (!(lpa & adv))
        {
            printf("partner doesnot support configure "
                   "speed duplex. adv %04x lpa %04x.\n",
                   adv, lpa);
            goto error;
        }

        HAL_ETH_WritePHYRegister(heth, PHY_ADVERTISE, adv); // R9_MSCR
    }

    /* Restart auto negotiation */
    ETH_PHYAutoNeo(heth);

    ETH_PhyProcessing(heth);
    return HAL_OK;

error:
    SN_ETH->IE |= ETH_INT_PHYSTS_CHG;

    return HAL_ERROR;
}

#if (configUSE_ETH_REGISTER_CALLBACKS == 1)
static void ETH_InitCallbacksToDefault(ETH_Handle_T *heth)
{
    /* Init the ETH Callback settings */
    heth->TxCpltCallback   = HAL_ETH_TxCpltCallback;  /* Legacy weak TxCpltCallback   */
    heth->RxCpltCallback   = HAL_ETH_RxCpltCallback;  /* Legacy weak RxCpltCallback   */
    heth->DMAErrorCallback = HAL_ETH_ErrorCallback;   /* Legacy weak DMAErrorCallback */
    heth->WakeUpCallback   = HAL_ETH_WakeUpCallback;  /* Legacy weak WakeUpCallback   */
    heth->LinkChgCallback  = HAL_ETH_LinkChgCallback; /* Legacy weak WakeUpCallback   */
}
#endif

#if (ETH_DEBUG == 1)
static void ETH_DUMP_MAC_REG(ETH_Handle_T *heth)
{
    uint32_t            i, j, *ptr;
    ETH_DMADescTypeDef *DMADesc;

    printf("\r\n\r\n=== MAC Registers value ===");
    for (i = 0; i < 0x100; i += 4)
    {
        ptr = (uint32_t *)(SN_ETHMAC_BASE + i);

        if ((i & 0xf) == 0)
            printf("\r\n%03x:  ", i);

        printf("%08x  ", *ptr);
    }

    printf("\r\n\r\n=== RX Ring Buffer Info ===");
    DMADesc = (ETH_DMADescTypeDef *)(heth->Instance)->RXR_BADR;

    for (i = 0; i < ETH_RXBUFNB; i++)
    {
        printf("\r\nBuffer %d : addr %08x \r\n", i, DMADesc);

        for (j = 0; j < 4; j++)
        {
            printf("%08x ", *((uint32_t *)DMADesc + j));
        }

        DMADesc = (ETH_DMADescTypeDef *)DMADesc->Buffer2NextDescAddr;
    }

    printf("\r\n\r\n=== NP TX Ring Buffer Info ===");
    DMADesc = (ETH_DMADescTypeDef *)(heth->Instance)->NPTXR_BADR;

    for (i = 0; i < ETH_TXBUFNB; i++)
    {
        printf("\r\nBuffer %d : addr %08x \r\n", i, DMADesc);

        for (j = 0; j < 4; j++)
        {
            printf("%08x ", *((uint32_t *)DMADesc + j));
        }

        DMADesc = (ETH_DMADescTypeDef *)DMADesc->Buffer2NextDescAddr;
    }

    if (HAL_REG_READ(heth->Instance->MACCR) & ETH_MACCR_HT_EN)
    {
        printf("\r\n\r\n=== HP TX Ring Buffer Info ===");
        DMADesc = (ETH_DMADescTypeDef *)(heth->Instance)->HPTXR_BADR;

        for (i = 0; i < ETH_TXBUFNB; i++)
        {
            printf("\r\nBuffer %d : addr %08x \r\n", i, DMADesc);

            for (j = 0; j < 4; j++)
            {
                printf("%08x ", *((uint32_t *)DMADesc + j));
            }

            DMADesc = (ETH_DMADescTypeDef *)DMADesc->Buffer2NextDescAddr;
        }
    }
}

static void ETH_DUMP_PHY_REG(ETH_Handle_T *heth)
{
    uint16_t i, phyreg;

    printf("\r\n\r\n=== PHY Registers value ===\r\n");
    for (i = 0; i < 0x20; i++)
    {
        HAL_ETH_ReadPHYRegister(heth, i, &phyreg);
        printf("%02x : %04x\r\n", i, phyreg);
    }
}

static void ETH_SHOW_LINK_STATUS(ETH_Handle_T *heth)
{
    uint32_t common_adv;
    uint32_t common_adv_gb = 0;
    uint32_t retry         = 1024;
    uint16_t status;
    uint16_t lpa;
    uint16_t lpagb;
    uint16_t adv;
    uint16_t bcr;
    uint16_t dscr;

    printf("\r\n\r\n=== Link Status ===\r\n");

read_bsr:
    /* Do a fake read */
    HAL_ETH_ReadPHYRegister(heth, PHY_BSR, &status);

    /* Read link and autonegotiation status */
    HAL_ETH_ReadPHYRegister(heth, PHY_BSR, &status);

    if ((status & BSR_LSTATUS) == 0)
    {
        if (retry > 0)
        {
            retry--;
            goto read_bsr;
        }
        printf("Link status : DisConect\r\n");
        return;
    }

    printf("Link status : Conect\r\n");

    HAL_ETH_ReadPHYRegister(heth, PHY_LPA, &lpa);

    if (lpa & LPA_LPACK)
    {
        if (status & BSR_ESTATEN)
        {
            HAL_ETH_ReadPHYRegister(heth, PHY_ESTATUS, &status);
            if (status & (ESTATUS_1000_TFULL | ESTATUS_1000_THALF))
            {
                HAL_ETH_ReadPHYRegister(heth, PHY_STAT1000, &lpagb);

                HAL_ETH_ReadPHYRegister(heth, PHY_CTRL1000, &adv);

                common_adv_gb = lpagb & adv << 2;
            }
        }

        HAL_ETH_ReadPHYRegister(heth, PHY_LPA, &lpa);

        HAL_ETH_ReadPHYRegister(heth, PHY_ADVERTISE, &adv);

        common_adv = lpa & adv;

        if (common_adv_gb & (LPA_1000FULL | LPA_1000HALF))
        {
            printf("SPEED-LPA: 1000Mbps ");
            if (common_adv_gb & LPA_1000FULL)
                printf("Full-duplex\r\n");
            else
                printf("Half-duplex\r\n");
        }
        else if (common_adv & (LPA_100FULL | LPA_100HALF))
        {
            printf("SPEED-LPA: 100Mbps ");
            if (common_adv & LPA_100FULL)
                printf("Full-duplex\r\n");
            else
                printf("Half-duplex\r\n");
        }
        else
        {
            printf("SPEED-LPA: 10Mbps ");
            if (common_adv & LPA_10FULL)
                printf("Full-duplex\r\n");
            else
                printf("Half-duplex\r\n");
        }
    }
    else
    {
        HAL_ETH_ReadPHYRegister(heth, PHY_BCR, &bcr);
        if (bcr & BCR_SPEED1000)
            printf("SPEED: 1000Mbps ");
        else if (bcr & BCR_SPEED100)
            printf("SPEED: 100Mbps ");
        else
            printf("SPEED: 10Mbps ");

        if (bcr & BCR_FULLDPLX)
            printf("FULL-duplex\r\n");
        else
            printf("Half-duplex\r\n");
    }

    HAL_ETH_ReadPHYRegister(heth, PHY_DSCR, &dscr);
    if (dscr & RMII_ENABLE)
        INFO_PRINT("RMII Enable\r\n");
    else
        INFO_PRINT("Normal MII Enable\r\n");
}
#endif

/**
 * @}
 */

#endif /* HAL_ETH_MODULE_ENABLED */
/**
 * @}
 */

/**
 * @}
 */
