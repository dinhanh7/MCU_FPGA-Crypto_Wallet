/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#include "sn34f78x_hal_sd.h"

/* Private function prototypes -----------------------------------------------*/
static uint32_t SDMMC_GetCmdError(SN_SDIO_Type *SDIOx);
static uint32_t SDMMC_GetCmdResp1(SN_SDIO_Type *SDIOx, uint8_t SD_CMD);
static uint32_t SDMMC_GetCmdResp2(SN_SDIO_Type *SDIOx);
static uint32_t SDMMC_GetCmdResp3(SN_SDIO_Type *SDIOx);
static uint32_t SDMMC_GetCmdResp7(SN_SDIO_Type *SDIOx);
static uint32_t SDMMC_GetCmdResp6(SN_SDIO_Type *SDIOx, uint8_t SD_CMD, uint16_t *pRCA);

SDIO_Adma2DescTable ftsdc21_adma2_desc_table[16];

bool SDMMC_Fill_ADMA_Desc_Table(uint32_t total_data, uint32_t *data_addr)
{
    SDIO_Adma2DescTable *tptr;

    tptr = ftsdc21_adma2_desc_table;

    memset(tptr, 0, sizeof(SDIO_Adma2DescTable));

    tptr->addr = (uint32_t)data_addr;
    tptr->attr = SDMMC_ADMA2_TRAN | SDMMC_ADMA2_ENTRY_VALID | SDMMC_ADMA2_ENTRY_END | SDMMC_ADMA2_ENTRY_INT;
    tptr->lgth = total_data;

    return 0;
}

/**
 * @brief  Programmed I/O for read
 * @param   SDIOx: Pointer to SDMMC register base
 * @param   buffer: Data buffer pointer
 * @param   length: Data length
 * @retval None
 */
uint32_t SDMMC_ReadData(SN_SDIO_Type *SDIOx, uint32_t *buffer, uint32_t length)
{
    uint32_t trans_sz, len, recv_len = 0;

    trans_sz = 512;

    while (length)
    {
        /* Wait write buffer ready
         * This bit will be set when at least one block( 512 bytes ) of space is avalilable for read
         */
        while (!__SDMMC_GET_STA(SDIOx, SDMMC_STA_BUFREN) && !__SDMMC_GET_FLAG(SDIOx, SDMMC_INTR_STS_ERR))
        {
        };

        __SDMMC_CLEAR_FLAG(SDIOx, SDMMC_INTR_STS_BUFF_READ_READY);

        len = (length < trans_sz) ? length : trans_sz;
        length -= len;

        while (len)
        {
            *buffer = SDMMC_ReadFIFO(SDIOx);
            len -= 4;
            recv_len += 4;
            buffer++;
        }
    }
    while (!(__SDMMC_GET_FLAG(SDIOx, SDMMC_INTR_STS_TXR_COMPLETE)))
        ;

    __SDMMC_CLEAR_FLAG(SDIOx, SDMMC_INTR_STS_TXR_COMPLETE);

    return recv_len;
}

/**
 * @brief  Programmed I/O for write
 * @param   SDIOx: Pointer to SDMMC register base
 * @param   buffer: Data buffer pointer
 * @param   length: Data length
 * @retval None
 */
uint32_t SDMMC_WriteData(SN_SDIO_Type *SDIOx, uint32_t *buffer, uint32_t length)
{
    uint32_t trans_sz, len, snd_len = 0;
    trans_sz = 512;

    while (length)
    {
        /* Wait write buffer ready
         * This bit will be set when at least one block( 512 bytes ) of space is avalilable for written
         */

        while (!__SDMMC_GET_STA(SDIOx, SDMMC_STA_BUFWEN) && !__SDMMC_GET_FLAG(SDIOx, SDMMC_INTR_STS_ERR))
        {
        };

        __SDMMC_CLEAR_FLAG(SDIOx, SDMMC_INTR_STS_BUFF_WRITE_READY);

        len = (length < trans_sz) ? length : trans_sz;
        length -= len;

        while (len)
        {
            SDMMC_WriteFIFO(SDIOx, buffer);
            len -= 4;
            snd_len += 4;
            buffer++;
        }
    }
    while (!(__SDMMC_GET_FLAG(SDIOx, SDMMC_INTR_STS_TXR_COMPLETE)))
        ;

    __SDMMC_CLEAR_FLAG(SDIOx, SDMMC_INTR_STS_TXR_COMPLETE);

    return snd_len;
}

uint32_t SDMMC_TransferDMA(SN_SDIO_Type *SDIOx, uint32_t *buffer, uint32_t length)
{
    uint32_t len = 0, rcv_len = 0;
    uint32_t next_addr = 0;

    SDIOx->SDMAADD = (uint32_t)((uintptr_t)buffer);

    do
    {
        while (1)
        {
            if ((SDIOx->RIS_b.TRANSCPLIF || SDIOx->RIS_b.DMAIF))
                break;
        }

        /* Clear dma flag */
        SDIOx->RIS &= SDMMC_INTR_STS_DMA;
        /* Clear transfer complete flag */
        SDIOx->RIS &= SDMMC_INTR_STS_TXR_COMPLETE;

        next_addr = SDIOx->SDMAADD;

        /* Transfered bytes count */
        len = next_addr - (uint32_t)((uintptr_t)buffer);

        if (len > 0x1000)
            break;

        if (len & 0xFFF)
            break;

        rcv_len += len;
        /* Minus the total desired bytes count. SDMA stops at boundary
         * but it might already exceed our intended bytes */
        if ((int32_t)(length - len) < 0)
            length = 0;
        else
            length -= len;

        if (length == 0)
            break;

        //		/* Boundary Checking */
        //		if (next_addr & sdma_bound_mask) {
        //			fLib_printf(" ERR## ... SDMA interrupt not at %d "
        //				    "boundary, addr=0x%08x.\n",
        //				    (sdma_bound_mask + 1), next_addr);
        //			return 1;
        //		} else {
        //			if (dbg_print > 1)
        //				fLib_printf(" SDMA interrupt at "
        //					    "addr=0x%08x.\n", next_addr);
        //		}

        //		/* Remaining bytes less than SDMA boundary.
        //		 * For finite transfer, Wait for transfer complete interrupt.
        //		 * Infinite transfer, wait for DMA interrupt. */
        //		if ((length > trans_sz) ||
        //		    ((length <= trans_sz) && infinite_mode))
        //			cardInfo.cmplMask = WAIT_DMA_INTR;
        //		else
        //			cardInfo.cmplMask = WAIT_TRANS_COMPLETE;
        buffer         = (uint32_t *)((uintptr_t)next_addr);
        SDIOx->SDMAADD = (uint32_t)((uintptr_t)buffer);
    } while (1);

    return rcv_len;
}

uint32_t SDMMC_TransferSDMA(SN_SDIO_Type *SDIOx, uint32_t *buffer, uint32_t length)
{
    uint32_t len = 0, rcv_len = 0;
    uint32_t next_addr = 0;

    do
    {
        while (1)
        {
            if ((SDIOx->RIS_b.TRANSCPLIF || SDIOx->RIS_b.DMAIF))
                break;
        }

        /* Clear dma flag */
        SDIOx->RIS &= SDMMC_INTR_STS_DMA;
        /* Clear transfer complete flag */
        SDIOx->RIS &= SDMMC_INTR_STS_TXR_COMPLETE;

        next_addr = SDIOx->SDMAADD;
        /* Transfered bytes count */
        len = next_addr - (uint32_t)((uintptr_t)buffer);
        rcv_len += len;
        /* Minus the total desired bytes count. SDMA stops at boundary
         * but it might already exceed our intended bytes */
        if ((int32_t)(length - len) < 0)
            length = 0;
        else
            length -= len;

        if (!length)
            break;

        //		/* Boundary Checking */
        //		if (next_addr & sdma_bound_mask) {
        //			fLib_printf(" ERR## ... SDMA interrupt not at %d "
        //				    "boundary, addr=0x%08x.\n",
        //				    (sdma_bound_mask + 1), next_addr);
        //			return 1;
        //		} else {
        //			if (dbg_print > 1)
        //				fLib_printf(" SDMA interrupt at "
        //					    "addr=0x%08x.\n", next_addr);
        //		}

        //		/* Remaining bytes less than SDMA boundary.
        //		 * For finite transfer, Wait for transfer complete interrupt.
        //		 * Infinite transfer, wait for DMA interrupt. */
        //		if ((length > trans_sz) ||
        //		    ((length <= trans_sz) && infinite_mode))
        //			cardInfo.cmplMask = WAIT_DMA_INTR;
        //		else
        //			cardInfo.cmplMask = WAIT_TRANS_COMPLETE;
        buffer         = (uint32_t *)((uintptr_t)next_addr);
        SDIOx->SDMAADD = (uint32_t)((uintptr_t)buffer);
    } while (1);

    return rcv_len;
}

uint32_t SDMMC_TransferADMA(SN_SDIO_Type *SDIOx, uint32_t *buffer, uint32_t length)
{
    while (1)
    {
        if ((SDIOx->RIS_b.TRANSCPLIF || SDIOx->RIS_b.DMAIF))
            break;
    }

    /* Clear dma flag */
    SDIOx->RIS &= SDMMC_INTR_STS_DMA;
    /* Clear transfer complete flag */
    SDIOx->RIS &= SDMMC_INTR_STS_TXR_COMPLETE;

    return length;
}

int8_t SDMMC_ConfigData(SN_SDIO_Type *SDIOx, SDIO_DataInitTypeDef XfrCfg)
{
    while (__SDMMC_GET_STA(SDIOx, SDMMC_STA_DATACT))
        ;

    if (XfrCfg.BlockEnable)
    {
        *(uint16_t *)(SN_SDIO_BASE + 0xC) |= 0x1 << 1;
    }
    else
    {
        *(uint16_t *)(SN_SDIO_BASE + 0xC) &= ~(0x1 << 1);
    }

    if (XfrCfg.AutoCMD12Enable && XfrCfg.BlockEnable)
    {
        *(uint16_t *)(SN_SDIO_BASE + 0xC) &= ~(0x3 << 2);
        *(uint16_t *)(SN_SDIO_BASE + 0xC) |= 0x1 << 2;
    }
    else if (XfrCfg.AutoCMD23Enable)
    {
        *(uint16_t *)(SN_SDIO_BASE + 0xC) &= ~(0x3 << 2);
        *(uint16_t *)(SN_SDIO_BASE + 0xC) |= 0x1 << 3;
        SDIOx->SDMAADD = XfrCfg.BlockCount;
    }
    else
    {
        *(uint16_t *)(SN_SDIO_BASE + 0xC) &= ~(0x3 << 2);
    }

    if (XfrCfg.TransferDir)
    {
        *(uint16_t *)(SN_SDIO_BASE + 0xC) |= 0x1 << 4;
    }
    else
    {
        *(uint16_t *)(SN_SDIO_BASE + 0xC) &= ~(0x1 << 4);
    }
    if (XfrCfg.MultiBlockEnable)
    {
        *(uint16_t *)(SN_SDIO_BASE + 0xC) |= 0x1 << 5;
    }
    else
    {
        *(uint16_t *)(SN_SDIO_BASE + 0xC) &= ~(0x1 << 5);
    }

    SDIOx->BLKCTRL_b.BLKCNT = XfrCfg.BlockCount;
    SDIOx->BLKCTRL_b.SIZE   = XfrCfg.BlockSize & 0xFFF;

    if (XfrCfg.DMAEnable)
    {
        *(uint16_t *)(SN_SDIO_BASE + 0xC) |= 0x1;
    }
    else
    {
        *(uint16_t *)(SN_SDIO_BASE + 0xC) &= ~(0x1);
    }

    if (XfrCfg.DMAEnable)
    {
        switch (XfrCfg.DMAType)
        {
            case SDMMC_TRANSFER_SDMA:
                SDIOx->SDMAADD           = (uint32_t)XfrCfg.ptr_Buf;
                SDIOx->HSTCTRL_b.DMATYPE = SDMMC_TRANSFER_SDMA;
                break;

            case SDMMC_TRANSFER_ADMA:
                SDIOx->ADMAADD           = (uint32_t)ftsdc21_adma2_desc_table;
                SDIOx->HSTCTRL_b.DMATYPE = SDMMC_TRANSFER_ADMA;
                // creat ADMA Desriptor
                break;

            default:
                // setup general DMA
                break;
        }
    }

    return 1;
}

void SDMMC_Speed_Config(SN_SDIO_Type *SDIOx, uint32_t ClockDiv)
{
    uint32_t clk_src = 0, clk_freq = 0, clk_div = 0, clk_req = 0;

    /* Reset LCLKSET, disable SDCKLEN and Internal CLK first*/
    HAL_REG_WRITE(SDIOx->CLKCTRL, 0);

    HAL_REG_WRITE(SDIOx->CLKCTRL_b.INTCLKEN, 1);
    // Wait for internal clock stable
    while (HAL_REG_READ(SDIOx->CLKCTRL_b.INTCLKSTB) == 0)
        ;

    HAL_REG_WRITE(SDIOx->CLKCTRL_b.UCLKSET, 0);

    clk_src = __HAL_RCC_GET_SDIO_CLKSEL();
    if (clk_src == RCC_SDIO_CLKSEL_PLLCLK)
    {
        clk_freq = HAL_RCC_GetPLLCLKFreq();
    }
    else if (clk_src == RCC_SDIO_CLKSEL_HCLK)
    {
        clk_freq = HAL_RCC_GetHCLKFreq();
    }

    switch (ClockDiv)
    {
        case SDMMC_INIT_CLK_DIV:
            clk_req = SDMMC_INIT_SPEED;
            break;
        case SDMMC_TRANSFER_CLK_DIV:
            clk_req = SDMMC_TRANSFER_SPEED;
            break;
        case SDMMC_HIGH_TRANSFER_CLK_DIV:
            clk_req = SDMMC_HIGH_TRANSFER_PEED;
            break;
        default:
            break;
    }

    clk_div = (clk_freq / 2) / (2 * clk_req);

    HAL_REG_WRITE(SDIOx->CLKCTRL_b.LCLKSET, clk_div);

    HAL_REG_WRITE(SDIOx->CLKCTRL_b.SDCLKEN, 1);

    if (ClockDiv == SDMMC_HIGH_TRANSFER_CLK_DIV)
    {
        HAL_REG_WRITE(SDIOx->HSTCTRL_b.HSPEN, 1);
    }
    else
    {
        HAL_REG_WRITE(SDIOx->HSTCTRL_b.HSPEN, 0);
    }
}

void SDMMC_BusWide_Config(SN_SDIO_Type *SDIOx, uint32_t BusWide)
{
    switch (BusWide)
    {
        case SDMMC_BUS_WIDE_1B:
            HAL_REG_WRITE(SDIOx->HSTCTRL_b.EXDATWID, 0); //	8-bit disable
            HAL_REG_WRITE(SDIOx->HSTCTRL_b.DATWID, 0);   //	choose 1-bit
            break;

        case SDMMC_BUS_WIDE_4B:
            HAL_REG_WRITE(SDIOx->HSTCTRL_b.EXDATWID, 0); //	8-bit disable
            HAL_REG_WRITE(SDIOx->HSTCTRL_b.DATWID, 1);   //	choose 4-bit
            break;

        case SDMMC_BUS_WIDE_8B:
            HAL_REG_WRITE(SDIOx->HSTCTRL_b.EXDATWID, 1); //	8-bit disable
            break;

        default:
            break;
    }
}

/**
 * @brief  Initializes the SDMMC according to the specified
 *         parameters in the SDMMC_InitTypeDef and create the associated handle.
 * @param  SDIOx: Pointer to SDMMC register base
 * @param  Init: SDMMC initialization structure
 * @retval HAL status
 */
void SDMMC_Init(SN_SDIO_Type *SDIOx, SD_Init_T Init)
{
    /* Bus speed Config */
    SDMMC_Speed_Config(SDIOx, Init.ClockDiv);

    /* Bus Wide Config */
    SDMMC_BusWide_Config(SDIOx, Init.BusWide);

    /* SD IRQ flag Config */
    __SDMMC_ENABLE_IT(SDIOx, SDMMC_IT_TRANSCPLIE | SDMMC_IT_BLKGAPIE | SDMMC_IT_DMAIE | SDMMC_IT_BUFWIE | SDMMC_IT_BUFRIE | SDMMC_IT_CMDTOEIE | SDMMC_IT_CMDCRCEIE |
                                 SDMMC_IT_CMDEBEIE | SDMMC_IT_CMDINDEIE | SDMMC_IT_DATTOEIE | SDMMC_IT_DATCRCEIE | SDMMC_IT_DATEBEIE | SDMMC_IT_CURLMTEIE | SDMMC_IT_ATCMDEIE | SDMMC_IT_ADMAEIE);

    /* SD Status Clear */
    HAL_REG_WRITE(SDIOx->RIS, 0xFFFFFFFF);

    return;
}

/**
 * @brief  Read data (word) from Rx FIFO in blocking mode (polling)
 * @param  SDMMCx: Pointer to SDMMC register base
 * @retval HAL status
 */
uint32_t SDMMC_ReadFIFO(SN_SDIO_Type *SDIOx)
{
    /* Read data from Rx FIFO */
    return (SDIOx->BUFDP);
}

/**
 * @brief  Write data (word) to Tx FIFO in blocking mode (polling)
 * @param  SDMMCx: Pointer to SDMMC register base
 * @param  pWriteData: pointer to data to write
 * @retval HAL status
 */
uint32_t SDMMC_WriteFIFO(SN_SDIO_Type *SDIOx, uint32_t *pWriteData)
{
    /* Write data to FIFO */
    HAL_REG_WRITE(SDIOx->BUFDP, *pWriteData);

    return HAL_OK;
}

/**
 * @brief  Set SDMMC Power state to ON.
 * @param  SDIOx: Pointer to SDMMC register base
 * @retval None
 */
void SDMMC_PowerState_ON(SN_SDIO_Type *SDIOx)
{
    /* Set power state to ON */
    HAL_REG_WRITE(SDIOx->HSTCTRL_b.SDBUSPW, 1); // Bus Power On
    return;
}

/**
 * @brief  Set SDMMC Power state to OFF.
 * @param  SDIOx: Pointer to SDMMC register base
 * @retval None
 */
void SDMMC_PowerState_OFF(SN_SDIO_Type *SDIOx)
{
    /* Set power state to OFF */
    HAL_REG_WRITE(SDIOx->HSTCTRL_b.SDBUSPW, 0); // Bus Power Off
    return;
}

/**
 * @brief  Get SDMMC Power state.
 * @param  SDIOx: Pointer to SDMMC register base
 * @retval SDMMC Power state
 */
uint32_t SDMMC_GetPowerState(SN_SDIO_Type *SDIOx)
{
    //    return (HAL_REG_READ(SDIOx->HSTCTRL) & SDIO_HSTCTRL_SDBUSPW);
    return (HAL_REG_RBIT(SDIOx->HSTCTRL, (0x01 << 8)) >> 8);
}

static int SDMMC_SendCommand(SN_SDIO_Type *SDIOx, SDIO_CmdInit_T sdmmc_cmdinit)
{
    uint16_t *CmdReg = (uint16_t *)(SN_SDIO_BASE + 0xE);
    uint32_t  cmd;
    /* 8 is the number of required instructions cycles for the below loop statement. The SDMMC_CMDTIMEOUT is expressed in ms */
    uint32_t count = SDMMC_CMDTIMEOUT * (SystemCoreClock / 8U / 1000U);

    /* Wait cmd line stable */
    do
    {
        if (count-- == 0U)
        {
            /* Card is not SD V2.0 compliant */
            return SDMMC_ERROR_TIMEOUT;
        }
    } while (__SDMMC_GET_STA(SDIOx, SDMMC_STA_CMDINBCMD));

    /* Wait data line stable */
    if (sdmmc_cmdinit.DatPres)
    {
        count = SDMMC_CMDTIMEOUT * (SystemCoreClock / 8U / 1000U);
        do
        {
            if (count-- == 0U)
            {
                /* Card is not SD V2.0 compliant */
                return SDMMC_ERROR_TIMEOUT;
            }
        } while (__SDMMC_GET_STA(SDIOx, SDMMC_STA_CMDINBDAT));
    }

    /* Setup Command */
    cmd = ((sdmmc_cmdinit.CmdIndex & 0x3f) << 8) | ((sdmmc_cmdinit.CmdType & 0x3) << 6) |
          ((sdmmc_cmdinit.DatPres & 0x1) << 5) | (sdmmc_cmdinit.RspType & 0x1F);

    SDIOx->ARG1 = sdmmc_cmdinit.Argument;

    /* Write to SDMMC CMD register */
    *CmdReg = cmd;

    return HAL_OK;
}

/**
 * @brief  Return the response received from the card for the last command
 * @param  SDIOx: Pointer to SDMMC register base
 * @param  Response: Specifies the SDMMC response register.
 *          This parameter can be one of the following values:
 *            @arg SDMMC_RESP1: Response Register 1
 *            @arg SDMMC_RESP2: Response Register 2
 *            @arg SDMMC_RESP3: Response Register 3
 *            @arg SDMMC_RESP4: Response Register 4
 * @retval The Corresponding response register value
 */
uint32_t SDMMC_GetResponse(SN_SDIO_Type *SDIOx, uint32_t Response)
{
    uint32_t tmp;

    /* Get the response */
    tmp = (uint32_t)(&(SDIOx->RESP0)) + Response;

    return (*(__IO uint32_t *)tmp);
}

/**
 * @brief  Send the Go Idle State command and check the response.
 * @param  SDIOx: Pointer to SDIO register base
 * @retval HAL status
 */
uint32_t SDMMC_CmdGoIdleState(SN_SDIO_Type *SDIOx)
{
    SDIO_CmdInit_T sdmmc_cmdinit;
    uint32_t       errorstate;

    sdmmc_cmdinit.Argument = 0U;
    sdmmc_cmdinit.CmdIndex = SDMMC_CMD_GO_IDLE_STATE;
    sdmmc_cmdinit.RspType  = SDMMC_CMD_NO_RESPONSE;
    sdmmc_cmdinit.CmdType  = SDMMC_CMD_TYPE_NORMAL;
    sdmmc_cmdinit.DatPres  = 0;

    SDMMC_SendCommand(SDIOx, sdmmc_cmdinit);
    /* Check for error conditions */
    errorstate = SDMMC_GetCmdError(SDIOx);

    return errorstate;
}

/**
 * @brief  Send the Operating Condition command and check the response.
 * @param  SDIOx: Pointer to SDIO register base
 * @retval HAL status
 */
uint32_t SDMMC_CmdOperCond(SN_SDIO_Type *SDIOx)
{
    SDIO_CmdInit_T sdmmc_cmdinit;
    uint32_t       errorstate;

    /* Send CMD8 to verify SD card interface operating condition */
    /* Argument: - [31:12]: Reserved (shall be set to '0')
    - [11:8]: Supply Voltage (VHS) 0x1 (Range: 2.7-3.6 V)
    - [7:0]: Check Pattern (recommended 0xAA) */
    /* CMD Response: R7 */
    sdmmc_cmdinit.Argument = SDMMC_CHECK_PATTERN;
    sdmmc_cmdinit.CmdIndex = SDMMC_CMD_HS_SEND_EXT_CSD;
    sdmmc_cmdinit.RspType  = SDMMC_CMD_RTYPE_R1R5R6R7;
    sdmmc_cmdinit.CmdType  = SDMMC_CMD_TYPE_NORMAL;
    sdmmc_cmdinit.DatPres  = 0;

    errorstate = SDMMC_SendCommand(SDIOx, sdmmc_cmdinit);

    /* Check for error conditions */
    errorstate = SDMMC_GetCmdResp7(SDIOx);

    return errorstate;
}

/**
 * @brief  Send the Application command to verify that that the next command
 *         is an application specific com-mand rather than a standard command
 *         and check the response.
 * @param  SDIOx: Pointer to SDIO register base
 * @param  Argument: Command Argument
 * @retval HAL status
 */
uint32_t SDMMC_CmdAppCommand(SN_SDIO_Type *SDIOx, uint32_t Argument)
{
    SDIO_CmdInit_T sdmmc_cmdinit;
    uint32_t       errorstate;

    sdmmc_cmdinit.Argument = (uint32_t)Argument;
    sdmmc_cmdinit.CmdIndex = SDMMC_CMD_APP_CMD;
    sdmmc_cmdinit.RspType  = SDMMC_CMD_RTYPE_R1R5R6R7;
    sdmmc_cmdinit.CmdType  = SDMMC_CMD_TYPE_NORMAL;
    sdmmc_cmdinit.DatPres  = 0;

    errorstate = SDMMC_SendCommand(SDIOx, sdmmc_cmdinit);

    /* Check for error conditions */
    /* If there is a HAL_ERROR, it is a MMC card, else
    it is a SD card: SD card 2.0 (voltage range mismatch)
       or SD card 1.x */
    errorstate = SDMMC_GetCmdResp1(SDIOx, SDMMC_CMD_APP_CMD);

    return errorstate;
}

/**
 * @brief  Send the command asking the accessed card to send its operating
 *         condition register (OCR)
 * @param  SDIOx: Pointer to SDIO register base
 * @param  Argument: Command Argument
 * @retval HAL status
 */
uint32_t SDMMC_CmdAppOperCommand(SN_SDIO_Type *SDIOx, uint32_t Argument)
{
    SDIO_CmdInit_T sdmmc_cmdinit;
    uint32_t       errorstate;

    sdmmc_cmdinit.Argument = SDMMC_VOLTAGE_WINDOW_SD | Argument;
    sdmmc_cmdinit.CmdIndex = SDMMC_CMD_SD_APP_OP_COND;
    sdmmc_cmdinit.CmdType  = SDMMC_CMD_TYPE_NORMAL;
    sdmmc_cmdinit.DatPres  = 0;
    sdmmc_cmdinit.RspType  = SDMMC_CMD_RTYPE_R3R4;
    errorstate             = SDMMC_SendCommand(SDIOx, sdmmc_cmdinit);

    /* Check for error conditions */
    errorstate = SDMMC_GetCmdResp3(SDIOx);

    return errorstate;
}

/**
 * @brief  Send the Send CID command and check the response.
 * @param  SDIOx: Pointer to SDIO register base
 * @retval HAL status
 */
uint32_t SDMMC_CmdSendCID(SN_SDIO_Type *SDIOx)
{
    SDIO_CmdInit_T sdmmc_cmdinit;
    uint32_t       errorstate;

    /* Send CMD2 ALL_SEND_CID */
    sdmmc_cmdinit.Argument = 0U;
    sdmmc_cmdinit.CmdIndex = SDMMC_CMD_ALL_SEND_CID;
    sdmmc_cmdinit.CmdType  = SDMMC_CMD_TYPE_NORMAL;
    sdmmc_cmdinit.DatPres  = 0;
    sdmmc_cmdinit.RspType  = SDMMC_CMD_RTYPE_R2;

    errorstate = SDMMC_SendCommand(SDIOx, sdmmc_cmdinit);

    /* Check for error conditions */
    errorstate = SDMMC_GetCmdResp2(SDIOx);

    return errorstate;
}

/**
 * @brief  Send the Send CSD command and check the response.
 * @param  SDIOx: Pointer to SDIO register base
 * @param  pRCA: Card RCA
 * @retval HAL status
 */
uint32_t SDMMC_CmdSetRelAdd(SN_SDIO_Type *SDIOx, uint16_t *pRCA)
{
    SDIO_CmdInit_T sdmmc_cmdinit;
    uint32_t       errorstate;

    /* Send CMD3 SD_CMD_SET_REL_ADDR */
    sdmmc_cmdinit.Argument = 0U;
    sdmmc_cmdinit.CmdIndex = SDMMC_CMD_SET_REL_ADDR;
    sdmmc_cmdinit.CmdType  = SDMMC_CMD_TYPE_NORMAL;
    sdmmc_cmdinit.DatPres  = 0;
    sdmmc_cmdinit.RspType  = SDMMC_CMD_RTYPE_R1R5R6R7;

    errorstate = SDMMC_SendCommand(SDIOx, sdmmc_cmdinit);

    /* Check for error conditions */
    errorstate = SDMMC_GetCmdResp6(SDIOx, SDMMC_CMD_SET_REL_ADDR, pRCA);

    return errorstate;
}

/**
 * @brief  Send the Send CSD command and check the response.
 * @param  SDIOx: Pointer to SDIO register base
 * @param  Argument: Command Argument
 * @retval HAL status
 */
uint32_t SDMMC_CmdSendCSD(SN_SDIO_Type *SDIOx, uint32_t Argument)
{
    SDIO_CmdInit_T sdmmc_cmdinit;
    uint32_t       errorstate;

    /* Send CMD9 SEND_CSD */
    sdmmc_cmdinit.Argument = Argument;
    sdmmc_cmdinit.CmdIndex = SDMMC_CMD_SEND_CSD;
    sdmmc_cmdinit.CmdType  = SDMMC_CMD_TYPE_NORMAL;
    sdmmc_cmdinit.DatPres  = 0;
    sdmmc_cmdinit.RspType  = SDMMC_CMD_RTYPE_R2;

    SDMMC_SendCommand(SDIOx, sdmmc_cmdinit);

    /* Check for error conditions */
    errorstate = SDMMC_GetCmdResp2(SDIOx);

    return errorstate;
}

/**
 * @brief  Send the Select Deselect command and check the response.
 * @param  SDIOx: Pointer to SDIO register base
 * @param  addr: Address of the card to be selected
 * @retval HAL status
 */
uint32_t SDMMC_CmdSelDesel(SN_SDIO_Type *SDIOx, uint64_t Addr)
{
    SDIO_CmdInit_T sdmmc_cmdinit;
    uint32_t       errorstate;

    /* Send CMD7 SDMMC_SEL_DESEL_CARD */
    sdmmc_cmdinit.Argument = (uint32_t)Addr;
    sdmmc_cmdinit.CmdIndex = SDMMC_CMD_SEL_DESEL_CARD;
    sdmmc_cmdinit.CmdType  = SDMMC_CMD_TYPE_NORMAL;
    sdmmc_cmdinit.DatPres  = 0;
    sdmmc_cmdinit.RspType  = SDMMC_CMD_RTYPE_R1R5R6R7;

    errorstate = SDMMC_SendCommand(SDIOx, sdmmc_cmdinit);

    /* Check for error conditions */
    errorstate = SDMMC_GetCmdResp1(SDIOx, SDMMC_CMD_SEL_DESEL_CARD);

    return errorstate;
}

/**
 * @brief  Send the Bus Width command and check the response.
 * @param  SDIOx: Pointer to SDIO register base
 * @param  BusWidth: BusWidth
 * @retval HAL status
 */
uint32_t SDMMC_CmdBusWidth(SN_SDIO_Type *SDIOx, uint32_t BusWidth)
{
    SDIO_CmdInit_T sdmmc_cmdinit;
    uint32_t       errorstate;

    sdmmc_cmdinit.Argument = (uint32_t)BusWidth;
    sdmmc_cmdinit.CmdIndex = SDMMC_CMD_APP_SD_SET_BUSWIDTH;
    sdmmc_cmdinit.CmdType  = SDMMC_CMD_TYPE_NORMAL;
    sdmmc_cmdinit.DatPres  = 0;
    sdmmc_cmdinit.RspType  = SDMMC_CMD_RTYPE_R1R5R6R7;

    SDMMC_SendCommand(SDIOx, sdmmc_cmdinit);

    /* Check for error conditions */
    errorstate = SDMMC_GetCmdResp1(SDIOx, SDMMC_CMD_APP_SD_SET_BUSWIDTH);

    return errorstate;
}

/**
 * @brief  Send the Data Block Length command and check the response
 * @param  SDIOx: Pointer to SDIO register base
 * @retval HAL status
 */
uint32_t SDMMC_CmdBlockLength(SN_SDIO_Type *SDIOx, uint32_t BlockSize)
{
    SDIO_CmdInit_T sdmmc_cmdinit;
    uint32_t       errorstate;

    /* Set Block Size for Card */
    sdmmc_cmdinit.Argument = (uint32_t)BlockSize;
    sdmmc_cmdinit.CmdIndex = SDMMC_CMD_SET_BLOCKLEN;
    sdmmc_cmdinit.CmdType  = SDMMC_CMD_TYPE_NORMAL;
    sdmmc_cmdinit.DatPres  = 0;
    sdmmc_cmdinit.RspType  = SDMMC_CMD_RTYPE_R1R5R6R7;

    errorstate = SDMMC_SendCommand(SDIOx, sdmmc_cmdinit);
    /* Check for error conditions */
    errorstate = SDMMC_GetCmdResp1(SDIOx, SDMMC_CMD_SET_BLOCKLEN);

    return errorstate;
}

/**
 * @brief  Send the Status command and check the response.
 * @param  SDIOx: Pointer to SDIO register base
 * @param  Argument: Command Argument
 * @retval HAL status
 */
uint32_t SDMMC_CmdSendStatus(SN_SDIO_Type *SDIOx, uint32_t Argument, uint32_t *resp)
{
    SDIO_CmdInit_T sdmmc_cmdinit;
    uint32_t       errorstate;

    sdmmc_cmdinit.Argument = Argument;
    sdmmc_cmdinit.CmdIndex = SDMMC_CMD_SEND_STATUS;
    sdmmc_cmdinit.CmdType  = SDMMC_CMD_TYPE_NORMAL;
    sdmmc_cmdinit.DatPres  = 0;
    sdmmc_cmdinit.RspType  = SDMMC_CMD_RTYPE_R1R5R6R7;

    errorstate = SDMMC_SendCommand(SDIOx, sdmmc_cmdinit);
    /* Check for error conditions */
    errorstate = SDMMC_GetCmdResp1(SDIOx, SDMMC_CMD_SEND_STATUS);

    return errorstate;
}

/**
 * @brief  Send the Read Single Block command and check the response
 * @param  SDIOx: Pointer to SDIO register base
 * @retval HAL status
 */
uint32_t SDMMC_CmdReadSingleBlock(SN_SDIO_Type *SDIOx, uint32_t ReadAdd)
{
    SDIO_CmdInit_T sdmmc_cmdinit;
    uint32_t       errorstate;

    /* Set Block Size for Card */
    sdmmc_cmdinit.Argument = (uint32_t)ReadAdd;
    sdmmc_cmdinit.CmdIndex = SDMMC_CMD_READ_SINGLE_BLOCK;
    sdmmc_cmdinit.CmdType  = SDMMC_CMD_TYPE_NORMAL;
    sdmmc_cmdinit.DatPres  = 1;
    sdmmc_cmdinit.RspType  = SDMMC_CMD_RTYPE_R1R5R6R7;

    errorstate = SDMMC_SendCommand(SDIOx, sdmmc_cmdinit);
    /* Check for error conditions */
    errorstate = SDMMC_GetCmdResp1(SDIOx, SDMMC_CMD_READ_SINGLE_BLOCK);

    return errorstate;
}

/**
 * @brief  Send the Read Multi Block command and check the response
 * @param  SDIOx: Pointer to SDIO register base
 * @retval HAL status
 */
uint32_t SDMMC_CmdReadMultiBlock(SN_SDIO_Type *SDIOx, uint32_t ReadAdd)
{
    SDIO_CmdInit_T sdmmc_cmdinit;
    uint32_t       errorstate;

    /* Set Block Size for Card */
    sdmmc_cmdinit.Argument = (uint32_t)ReadAdd;
    sdmmc_cmdinit.CmdIndex = SDMMC_CMD_READ_MULT_BLOCK;
    sdmmc_cmdinit.CmdType  = SDMMC_CMD_TYPE_NORMAL;
    sdmmc_cmdinit.DatPres  = 1;
    sdmmc_cmdinit.RspType  = SDMMC_CMD_RTYPE_R1R5R6R7;

    errorstate = SDMMC_SendCommand(SDIOx, sdmmc_cmdinit);
    /* Check for error conditions */
    errorstate = SDMMC_GetCmdResp1(SDIOx, SDMMC_CMD_READ_MULT_BLOCK);

    return errorstate;
}

/**
 * @brief  Send the Write Single Block command and check the response
 * @param  SDIOx: Pointer to SDIO register base
 * @retval HAL status
 */
uint32_t SDMMC_CmdWriteSingleBlock(SN_SDIO_Type *SDIOx, uint32_t WriteAdd)
{
    SDIO_CmdInit_T sdmmc_cmdinit;
    uint32_t       errorstate;

    /* Set Block Size for Card */
    sdmmc_cmdinit.Argument = (uint32_t)WriteAdd;
    sdmmc_cmdinit.CmdIndex = SDMMC_CMD_WRITE_SINGLE_BLOCK;
    sdmmc_cmdinit.CmdType  = SDMMC_CMD_TYPE_NORMAL;
    sdmmc_cmdinit.DatPres  = 1;
    sdmmc_cmdinit.RspType  = SDMMC_CMD_RTYPE_R1R5R6R7;

    errorstate = SDMMC_SendCommand(SDIOx, sdmmc_cmdinit);
    /* Check for error conditions */
    errorstate = SDMMC_GetCmdResp1(SDIOx, SDMMC_CMD_WRITE_SINGLE_BLOCK);

    return errorstate;
}

/**
 * @brief  Set read/write block count
 * @param  SDIOx: Pointer to SDIO register base
 * @retval HAL status
 */
uint32_t SDMMC_CmdSetBlockCount(SN_SDIO_Type *SDIOx, uint32_t BlockCnt)
{
    SDIO_CmdInit_T sdmmc_cmdinit;
    uint32_t       errorstate;

    /* Set Block Size for Card */
    sdmmc_cmdinit.Argument = (uint32_t)BlockCnt;
    sdmmc_cmdinit.CmdIndex = SDMMC_CMD_SET_BLOCK_COUNT;
    sdmmc_cmdinit.CmdType  = SDMMC_CMD_TYPE_NORMAL;
    sdmmc_cmdinit.DatPres  = 0;
    sdmmc_cmdinit.RspType  = SDMMC_CMD_RTYPE_R1R5R6R7;

    errorstate = SDMMC_SendCommand(SDIOx, sdmmc_cmdinit);
    /* Check for error conditions */
    errorstate = SDMMC_GetCmdResp1(SDIOx, SDMMC_CMD_SET_BLOCK_COUNT);

    return errorstate;
}

/**
 * @brief  Send the Write Multi Block command and check the response
 * @param  SDIOx: Pointer to SDIO register base
 * @retval HAL status
 */
uint32_t SDMMC_CmdWriteMultiBlock(SN_SDIO_Type *SDIOx, uint32_t WriteAdd)
{
    SDIO_CmdInit_T sdmmc_cmdinit;
    uint32_t       errorstate;

    /* Set Block Size for Card */
    sdmmc_cmdinit.Argument = (uint32_t)WriteAdd;
    sdmmc_cmdinit.CmdIndex = SDMMC_CMD_WRITE_MULT_BLOCK;
    sdmmc_cmdinit.CmdType  = SDMMC_CMD_TYPE_NORMAL;
    sdmmc_cmdinit.DatPres  = 1;
    sdmmc_cmdinit.RspType  = SDMMC_CMD_RTYPE_R1R5R6R7;

    errorstate = SDMMC_SendCommand(SDIOx, sdmmc_cmdinit);
    /* Check for error conditions */
    errorstate = SDMMC_GetCmdResp1(SDIOx, SDMMC_CMD_WRITE_MULT_BLOCK);

    return errorstate;
}

/**
 * @brief  Send the Start Address Erase command for SD and check the response
 * @param  SDIOx: Pointer to SDIO register base
 * @retval HAL status
 */
uint32_t SDMMC_CmdSDEraseStartAdd(SN_SDIO_Type *SDIOx, uint32_t StartAdd)
{
    SDIO_CmdInit_T sdmmc_cmdinit;
    uint32_t       errorstate;

    /* Set Block Size for Card */
    sdmmc_cmdinit.Argument = (uint32_t)StartAdd;
    sdmmc_cmdinit.CmdIndex = SDMMC_CMD_SD_ERASE_GRP_START;
    sdmmc_cmdinit.CmdType  = SDMMC_CMD_TYPE_NORMAL;
    sdmmc_cmdinit.DatPres  = 0;
    sdmmc_cmdinit.RspType  = SDMMC_CMD_RTYPE_R1R5R6R7;

    errorstate = SDMMC_SendCommand(SDIOx, sdmmc_cmdinit);

    /* Check for error conditions */
    errorstate = SDMMC_GetCmdResp1(SDIOx, SDMMC_CMD_SD_ERASE_GRP_START);

    return errorstate;
}

/**
 * @brief  Send the End Address Erase command for SD and check the response
 * @param  SDIOx: Pointer to SDIO register base
 * @retval HAL status
 */
uint32_t SDMMC_CmdSDEraseEndAdd(SN_SDIO_Type *SDIOx, uint32_t EndAdd)
{
    SDIO_CmdInit_T sdmmc_cmdinit;
    uint32_t       errorstate;

    /* Set Block Size for Card */
    sdmmc_cmdinit.Argument = (uint32_t)EndAdd;
    sdmmc_cmdinit.CmdIndex = SDMMC_CMD_SD_ERASE_GRP_END;
    sdmmc_cmdinit.CmdType  = SDMMC_CMD_TYPE_NORMAL;
    sdmmc_cmdinit.DatPres  = 0;
    sdmmc_cmdinit.RspType  = SDMMC_CMD_RTYPE_R1R5R6R7;

    errorstate = SDMMC_SendCommand(SDIOx, sdmmc_cmdinit);

    /* Check for error conditions */
    errorstate = SDMMC_GetCmdResp1(SDIOx, SDMMC_CMD_SD_ERASE_GRP_END);

    return errorstate;
}

/**
 * @brief  Send the Erase command and check the response
 * @param  SDIOx: Pointer to SDIO register base
 * @retval HAL status
 */
uint32_t SDMMC_CmdErase(SN_SDIO_Type *SDIOx)
{
    SDIO_CmdInit_T sdmmc_cmdinit;
    uint32_t       errorstate;

    /* Set Block Size for Card */
    sdmmc_cmdinit.Argument = 0;
    sdmmc_cmdinit.CmdIndex = SDMMC_CMD_ERASE;
    sdmmc_cmdinit.CmdType  = SDMMC_CMD_TYPE_NORMAL;
    sdmmc_cmdinit.DatPres  = 0;
    sdmmc_cmdinit.RspType  = SDMMC_CMD_RTYPE_R1BR5B;

    errorstate = SDMMC_SendCommand(SDIOx, sdmmc_cmdinit);

    /* Check for error conditions */
    errorstate = SDMMC_GetCmdResp1(SDIOx, SDMMC_CMD_ERASE);

    return errorstate;
}

/**
 * @brief  Send the Stop Transfer command and check the response.
 * @param  SDIOx: Pointer to SDIO register base
 * @retval HAL status
 */
uint32_t SDMMC_CmdStopTransfer(SN_SDIO_Type *SDIOx)
{
    SDIO_CmdInit_T sdmmc_cmdinit;
    uint32_t       errorstate;

    /* Send CMD12 STOP_TRANSMISSION  */
    sdmmc_cmdinit.Argument = 0U;
    sdmmc_cmdinit.CmdIndex = SDMMC_CMD_STOP_TRANSMISSION;
    sdmmc_cmdinit.CmdType  = SDMMC_CMD_TYPE_ABORT;
    sdmmc_cmdinit.DatPres  = 0;
    sdmmc_cmdinit.RspType  = SDMMC_CMD_RTYPE_R1BR5B;

    errorstate = SDMMC_SendCommand(SDIOx, sdmmc_cmdinit);
    /* Check for error conditions */
    errorstate = SDMMC_GetCmdResp1(SDIOx, SDMMC_CMD_STOP_TRANSMISSION);

    return errorstate;
}

/**
 * @brief  Send the Status register command and check the response.
 * @param  SDIOx: Pointer to SDIO register base
 * @retval HAL status
 */
uint32_t SDMMC_CmdStatusRegister(SN_SDIO_Type *SDIOx)
{
    SDIO_CmdInit_T sdmmc_cmdinit;
    uint32_t       errorstate;

    /* Cmd Init */
    sdmmc_cmdinit.Argument = 0U;
    sdmmc_cmdinit.CmdIndex = SDMMC_CMD_SD_APP_STATUS;
    sdmmc_cmdinit.CmdType  = SDMMC_CMD_TYPE_NORMAL;
    sdmmc_cmdinit.DatPres  = 1;
    sdmmc_cmdinit.RspType  = SDMMC_CMD_RTYPE_R1R5R6R7;

    SDMMC_SendCommand(SDIOx, sdmmc_cmdinit);
    /* Check for error conditions */
    errorstate = SDMMC_GetCmdResp1(SDIOx, SDMMC_CMD_SD_APP_STATUS);

    return errorstate;
}

/**
 * @brief  Send the SD configuration register command and check the response.
 * @param  SDIOx: Pointer to SDIO register base
 * @retval HAL status
 */
uint32_t SDMMC_CmdSendSCR(SN_SDIO_Type *SDIOx)
{
    SDIO_CmdInit_T sdmmc_cmdinit;
    uint32_t       errorstate;

    /* Cmd Init */
    sdmmc_cmdinit.Argument = 0U;
    sdmmc_cmdinit.CmdIndex = SDMMC_CMD_SD_APP_SEND_SCR;
    sdmmc_cmdinit.CmdType  = SDMMC_CMD_TYPE_NORMAL;
    sdmmc_cmdinit.DatPres  = 1;
    sdmmc_cmdinit.RspType  = SDMMC_CMD_RTYPE_R1R5R6R7;

    SDMMC_SendCommand(SDIOx, sdmmc_cmdinit);
    /* Check for error conditions */
    errorstate = SDMMC_GetCmdResp1(SDIOx, SDMMC_CMD_SD_APP_SEND_SCR);

    return errorstate;
}

/**
 * @brief  Checks switchable function and switch card function. SDMMC_CMD_HS_SWITCH command
 * @param  SDIOx: Pointer to SDIO register base
 * @parame Argument: Argument used for the command
 * @retval HAL status
 */
uint32_t SDMMC_CmdSwitch(SN_SDIO_Type *SDIOx, uint32_t Argument)
{
    SDIO_CmdInit_T sdmmc_cmdinit;
    uint32_t       errorstate;

    /* Cmd Init */
    sdmmc_cmdinit.Argument = Argument;
    sdmmc_cmdinit.CmdIndex = SDMMC_CMD_HS_SWITCH;
    sdmmc_cmdinit.CmdType  = SDMMC_CMD_TYPE_NORMAL;
    sdmmc_cmdinit.DatPres  = 1;
    sdmmc_cmdinit.RspType  = SDMMC_CMD_RTYPE_R1R5R6R7;

    SDMMC_SendCommand(SDIOx, sdmmc_cmdinit);
    /* Check for error conditions */
    errorstate = SDMMC_GetCmdResp1(SDIOx, SDMMC_CMD_HS_SWITCH);

    return errorstate;
}

/* Private function ----------------------------------------------------------*/
/** @addtogroup SD_Private_Functions
 * @{
 */

/**
 * @brief  Checks for error conditions for CMD0.
 * @param  hsd: SD handle
 * @retval SD Card error state
 */
static uint32_t SDMMC_GetCmdError(SN_SDIO_Type *SDIOx)
{
    /* 8 is the number of required instructions cycles for the below loop statement.
    The SDMMC_CMDTIMEOUT is expressed in ms */
    uint32_t count = SDMMC_CMDTIMEOUT * (SystemCoreClock / 8U / 1000U);

    /*
     * Wait for cmd complete
     * CMDINBCMD is cleared When the command response is receive
     */
    do
    {
        if (count-- == 0U)
        {
            /* Card is not SD V2.0 compliant */
            return SDMMC_ERROR_TIMEOUT;
        }
    } while (__SDMMC_GET_STA(SDIOx, SDMMC_STA_CMDINBCMD) && !__SDMMC_GET_FLAG(SDIOx, SDMMC_INTR_STS_ERR));

    if (__SDMMC_GET_FLAG(SDIOx, SDMMC_INTR_STS_ERR))
    {
        /* Check CMD Line Error */
        /* When only data timeout error occured, sdc021 cannot issue CMD12, CMD13. */
        if (__SDMMC_GET_FLAG(SDIOx, SDMMC_INTR_STS_ERR_CMD_LINE))
        {
            /* Software Reset for CMD line */
            __SDMMC_SWRST_SET(SN_SDIO, SDMMC_SOFTRST_CMD);

            while (__SDMMC_SWRST_GET_FLAG(SN_SDIO, SDMMC_SOFTRST_CMD))
                ;
        }

        if (__SDMMC_GET_FLAG(SDIOx, SDMMC_INTR_STS_ERR_CMD_CRC)) /*	CMD CRC error */
        {
            __SDMMC_CLEAR_FLAG(SDIOx, SDMMC_INTR_STS_ERR_CMD_CRC);
            return SDMMC_ERROR_CMD_CRC_FAIL;
        }

        if (__SDMMC_GET_FLAG(SDIOx, SDMMC_INTR_STS_ERR_CMD_ENDBIT)) /* CMD End bit error */
        {
            __SDMMC_CLEAR_FLAG(SDIOx, SDMMC_INTR_STS_ERR_CMD_ENDBIT);
            return SDMMC_ERROR_CMD_END_BIT;
        }

        if (__SDMMC_GET_FLAG(SDIOx, SDMMC_INTR_STS_ERR_CMD_INDEX)) /*	CMD Index error */
        {
            __SDMMC_CLEAR_FLAG(SDIOx, SDMMC_INTR_STS_ERR_CMD_INDEX);
            return SDMMC_ERROR_CMD_INDEX;
        }

        if (__SDMMC_GET_FLAG(SDIOx, SDMMC_INTR_STS_ERR_CMD_TIMEOUT)) /*	CMD Timeout error */
        {
            __SDMMC_CLEAR_FLAG(SDIOx, SDMMC_INTR_STS_ERR_CMD_TIMEOUT);
            return SDMMC_ERROR_CMD_RSP_TIMEOUT;
        }
    }

    return SDMMC_ERROR_NONE;
}

/**
 * @brief  Checks for error conditions for R1 response.
 * @param  hsd: SD handle
 * @param  SD_CMD: The sent command index
 * @retval SD Card error state
 */
static uint32_t SDMMC_GetCmdResp1(SN_SDIO_Type *SDIOx, uint8_t SD_CMD)
{
    uint32_t response_r1;
    /* 8 is the number of required instructions cycles for the below loop statement.
    The SDMMC_CMDTIMEOUT is expressed in ms */
    uint32_t count = SDMMC_CMDTIMEOUT * (SystemCoreClock / 8U / 1000U);

    /*
     * Wait for cmd complete
     * CMDINBCMD is cleared When the command response is receive
     */
    do
    {
        if (count-- == 0U)
        {
            /* Card is not SD V2.0 compliant */
            return SDMMC_ERROR_TIMEOUT;
        }
    } while (__SDMMC_GET_STA(SDIOx, SDMMC_STA_CMDINBCMD) && !__SDMMC_GET_FLAG(SDIOx, SDMMC_INTR_STS_ERR));

    if (__SDMMC_GET_FLAG(SDIOx, SDMMC_INTR_STS_ERR))
    {
        /* Check CMD Line Error */
        /* When only data timeout error occured, sdc021 cannot issue CMD12, CMD13. */
        if (__SDMMC_GET_FLAG(SDIOx, SDMMC_INTR_STS_ERR_CMD_LINE))
        {
            /* Software Reset for CMD line */
            __SDMMC_SWRST_SET(SN_SDIO, SDMMC_SOFTRST_CMD);

            while (__SDMMC_SWRST_GET_FLAG(SN_SDIO, SDMMC_SOFTRST_CMD))
                ;
        }

        if (__SDMMC_GET_FLAG(SDIOx, SDMMC_INTR_STS_ERR_CMD_CRC)) /*	CMD CRC error */
        {
            __SDMMC_CLEAR_FLAG(SDIOx, SDMMC_INTR_STS_ERR_CMD_CRC);
            return SDMMC_ERROR_CMD_CRC_FAIL;
        }

        if (__SDMMC_GET_FLAG(SDIOx, SDMMC_INTR_STS_ERR_CMD_ENDBIT)) /* CMD End bit error */
        {
            __SDMMC_CLEAR_FLAG(SDIOx, SDMMC_INTR_STS_ERR_CMD_ENDBIT);
            return SDMMC_ERROR_CMD_END_BIT;
        }

        if (__SDMMC_GET_FLAG(SDIOx, SDMMC_INTR_STS_ERR_CMD_INDEX)) /*	CMD Index error */
        {
            __SDMMC_CLEAR_FLAG(SDIOx, SDMMC_INTR_STS_ERR_CMD_INDEX);
            return SDMMC_ERROR_CMD_INDEX;
        }

        if (__SDMMC_GET_FLAG(SDIOx, SDMMC_INTR_STS_ERR_CMD_TIMEOUT)) /*	CMD Timeout error */
        {
            __SDMMC_CLEAR_FLAG(SDIOx, SDMMC_INTR_STS_ERR_CMD_TIMEOUT);
            return SDMMC_ERROR_CMD_RSP_TIMEOUT;
        }
    }

    if (SD_CMD == SDMMC_CMD_STOP_TRANSMISSION || SD_CMD == SDMMC_CMD_ERASE)
    {
        count = SDMMC_CMDTIMEOUT * (SystemCoreClock / 8U / 1000U);
        /*
         * Wait for busy state
         */
        do
        {
            if ((count-- == 0U) || (__SDMMC_GET_FLAG(SDIOx, SDMMC_INTR_STS_ERR_DATA_TIMEOUT)))
            {
                __SDMMC_CLEAR_FLAG(SDIOx, SDMMC_INTR_STS_ERR_DATA_TIMEOUT);
                HAL_REG_SBIT(SN_SDIO->SWRST, SDMMC_SOFTRST_DAT);

                while (HAL_REG_RBIT(SN_SDIO->SWRST, SDMMC_SOFTRST_DAT))
                    ;

                do
                {
                    if (count-- == 0U)
                    {
                        return SDMMC_ERROR_TIMEOUT;
                    }
                } while (!(HAL_REG_RBIT(SN_SDIO->PRSSTA, SDMMC_STA_DAT0LV)));

                /* When Data 0 is High and busy state is finished*/
                break;
            }
        } while (!(__SDMMC_GET_FLAG(SDIOx, SDMMC_INTR_STS_TXR_COMPLETE)));

        __SDMMC_CLEAR_FLAG(SDIOx, SDMMC_INTR_STS_TXR_COMPLETE);
    }

    /* We have received response, retrieve it for analysis  */
    response_r1 = SDMMC_GetResponse(SDIOx, SDMMC_RESP0);

    if ((response_r1 & SDMMC_OCR_ERRORBITS) == SDMMC_ALLZERO)
    {
        return SDMMC_ERROR_NONE;
    }
    else if ((response_r1 & SDMMC_OCR_ADDR_OUT_OF_RANGE) == SDMMC_OCR_ADDR_OUT_OF_RANGE)
    {
        return SDMMC_ERROR_ADDR_OUT_OF_RANGE;
    }
    else if ((response_r1 & SDMMC_OCR_ADDR_MISALIGNED) == SDMMC_OCR_ADDR_MISALIGNED)
    {
        return SDMMC_ERROR_ADDR_MISALIGNED;
    }
    else if ((response_r1 & SDMMC_OCR_BLOCK_LEN_ERR) == SDMMC_OCR_BLOCK_LEN_ERR)
    {
        return SDMMC_ERROR_BLOCK_LEN_ERR;
    }
    else if ((response_r1 & SDMMC_OCR_ERASE_SEQ_ERR) == SDMMC_OCR_ERASE_SEQ_ERR)
    {
        return SDMMC_ERROR_ERASE_SEQ_ERR;
    }
    else if ((response_r1 & SDMMC_OCR_BAD_ERASE_PARAM) == SDMMC_OCR_BAD_ERASE_PARAM)
    {
        return SDMMC_ERROR_BAD_ERASE_PARAM;
    }
    else if ((response_r1 & SDMMC_OCR_WRITE_PROT_VIOLATION) == SDMMC_OCR_WRITE_PROT_VIOLATION)
    {
        return SDMMC_ERROR_WRITE_PROT_VIOLATION;
    }
    else if ((response_r1 & SDMMC_OCR_LOCK_UNLOCK_FAILED) == SDMMC_OCR_LOCK_UNLOCK_FAILED)
    {
        return SDMMC_ERROR_LOCK_UNLOCK_FAILED;
    }
    else if ((response_r1 & SDMMC_OCR_COM_CRC_FAILED) == SDMMC_OCR_COM_CRC_FAILED)
    {
        return SDMMC_ERROR_COM_CRC_FAILED;
    }
    else if ((response_r1 & SDMMC_OCR_ILLEGAL_CMD) == SDMMC_OCR_ILLEGAL_CMD)
    {
        return SDMMC_ERROR_ILLEGAL_CMD;
    }
    else if ((response_r1 & SDMMC_OCR_CARD_ECC_FAILED) == SDMMC_OCR_CARD_ECC_FAILED)
    {
        return SDMMC_ERROR_CARD_ECC_FAILED;
    }
    else if ((response_r1 & SDMMC_OCR_CC_ERROR) == SDMMC_OCR_CC_ERROR)
    {
        return SDMMC_ERROR_CC_ERR;
    }
    else if ((response_r1 & SDMMC_OCR_STREAM_READ_UNDERRUN) == SDMMC_OCR_STREAM_READ_UNDERRUN)
    {
        return SDMMC_ERROR_STREAM_READ_UNDERRUN;
    }
    else if ((response_r1 & SDMMC_OCR_STREAM_WRITE_OVERRUN) == SDMMC_OCR_STREAM_WRITE_OVERRUN)
    {
        return SDMMC_ERROR_STREAM_WRITE_OVERRUN;
    }
    else if ((response_r1 & SDMMC_OCR_CID_CSD_OVERWRITE) == SDMMC_OCR_CID_CSD_OVERWRITE)
    {
        return SDMMC_ERROR_CID_CSD_OVERWRITE;
    }
    else if ((response_r1 & SDMMC_OCR_WP_ERASE_SKIP) == SDMMC_OCR_WP_ERASE_SKIP)
    {
        return SDMMC_ERROR_WP_ERASE_SKIP;
    }
    else if ((response_r1 & SDMMC_OCR_CARD_ECC_DISABLED) == SDMMC_OCR_CARD_ECC_DISABLED)
    {
        return SDMMC_ERROR_CARD_ECC_DISABLED;
    }
    else if ((response_r1 & SDMMC_OCR_ERASE_RESET) == SDMMC_OCR_ERASE_RESET)
    {
        return SDMMC_ERROR_ERASE_RESET;
    }
    else if ((response_r1 & SDMMC_OCR_AKE_SEQ_ERROR) == SDMMC_OCR_AKE_SEQ_ERROR)
    {
        return SDMMC_ERROR_AKE_SEQ_ERR;
    }
    else
    {
        return SDMMC_ERROR_GENERAL_UNKNOWN_ERR;
    }
}

/**
 * @brief  Checks for error conditions for R2 (CID or CSD) response.
 * @param  hsd: SD handle
 * @retval SD Card error state
 */
static uint32_t SDMMC_GetCmdResp2(SN_SDIO_Type *SDIOx)
{
    /* 8 is the number of required instructions cycles for the below loop statement.
        The SDMMC_CMDTIMEOUT is expressed in ms */
    uint32_t count = SDMMC_CMDTIMEOUT * (SystemCoreClock / 8U / 1000U);

    /*
     * Wait for cmd complete
     * CMDINBCMD is cleared When the command response is receive
     */
    do
    {
        if (count-- == 0U)
        {
            /* Card is not SD V2.0 compliant */
            return SDMMC_ERROR_TIMEOUT;
        }
    } while (__SDMMC_GET_STA(SDIOx, SDMMC_STA_CMDINBCMD) && !__SDMMC_GET_FLAG(SDIOx, SDMMC_INTR_STS_ERR));

    if (__SDMMC_GET_FLAG(SDIOx, SDMMC_INTR_STS_ERR))
    {
        /* Check CMD Line Error */
        /* When only data timeout error occured, sdc021 cannot issue CMD12, CMD13. */
        if (__SDMMC_GET_FLAG(SDIOx, SDMMC_INTR_STS_ERR_CMD_LINE))
        {
            /* Software Reset for CMD line */
            __SDMMC_SWRST_SET(SN_SDIO, SDMMC_SOFTRST_CMD);

            while (__SDMMC_SWRST_GET_FLAG(SN_SDIO, SDMMC_SOFTRST_CMD))
                ;
        }

        if (__SDMMC_GET_FLAG(SDIOx, SDMMC_INTR_STS_ERR_CMD_CRC)) /*	CMD CRC error */
        {
            __SDMMC_CLEAR_FLAG(SDIOx, SDMMC_INTR_STS_ERR_CMD_CRC);
            return SDMMC_ERROR_CMD_CRC_FAIL;
        }

        if (__SDMMC_GET_FLAG(SDIOx, SDMMC_INTR_STS_ERR_CMD_ENDBIT)) /* CMD End bit error */
        {
            __SDMMC_CLEAR_FLAG(SDIOx, SDMMC_INTR_STS_ERR_CMD_ENDBIT);
            return SDMMC_ERROR_CMD_END_BIT;
        }

        if (__SDMMC_GET_FLAG(SDIOx, SDMMC_INTR_STS_ERR_CMD_INDEX)) /*	CMD Index error */
        {
            __SDMMC_CLEAR_FLAG(SDIOx, SDMMC_INTR_STS_ERR_CMD_INDEX);
            return SDMMC_ERROR_CMD_INDEX;
        }

        if (__SDMMC_GET_FLAG(SDIOx, SDMMC_INTR_STS_ERR_CMD_TIMEOUT)) /*	CMD Timeout error */
        {
            __SDMMC_CLEAR_FLAG(SDIOx, SDMMC_INTR_STS_ERR_CMD_TIMEOUT);
            return SDMMC_ERROR_CMD_RSP_TIMEOUT;
        }
    }

    return SDMMC_ERROR_NONE;
}

/**
 * @brief  Checks for error conditions for R3 (OCR) response.
 * @param  hsd: SD handle
 * @retval SD Card error state
 */
static uint32_t SDMMC_GetCmdResp3(SN_SDIO_Type *SDIOx)
{
    /* 8 is the number of required instructions cycles for the below loop statement.
      The SDMMC_CMDTIMEOUT is expressed in ms */
    uint32_t count = SDMMC_CMDTIMEOUT * (SystemCoreClock / 8U / 1000U);

    /*
     * Wait for cmd complete
     * CMDINBCMD is cleared When the command response is receive
     */
    do
    {
        if (count-- == 0U)
        {
            /* Card is not SD V2.0 compliant */
            return SDMMC_ERROR_TIMEOUT;
        }
    } while (__SDMMC_GET_STA(SDIOx, SDMMC_STA_CMDINBCMD) && !__SDMMC_GET_FLAG(SDIOx, SDMMC_INTR_STS_ERR));

    if (__SDMMC_GET_FLAG(SDIOx, SDMMC_INTR_STS_ERR))
    {
        /* Check CMD Line Error */
        /* When only data timeout error occured, sdc021 cannot issue CMD12, CMD13. */
        if (__SDMMC_GET_FLAG(SDIOx, SDMMC_INTR_STS_ERR_CMD_LINE))
        {
            /* Software Reset for CMD line */
            __SDMMC_SWRST_SET(SN_SDIO, SDMMC_SOFTRST_CMD);

            while (__SDMMC_SWRST_GET_FLAG(SN_SDIO, SDMMC_SOFTRST_CMD))
                ;
        }

        if (__SDMMC_GET_FLAG(SDIOx, SDMMC_INTR_STS_ERR_CMD_CRC)) /*	CMD CRC error */
        {
            __SDMMC_CLEAR_FLAG(SDIOx, SDMMC_INTR_STS_ERR_CMD_CRC);
            return SDMMC_ERROR_CMD_CRC_FAIL;
        }

        if (__SDMMC_GET_FLAG(SDIOx, SDMMC_INTR_STS_ERR_CMD_ENDBIT)) /* CMD End bit error */
        {
            __SDMMC_CLEAR_FLAG(SDIOx, SDMMC_INTR_STS_ERR_CMD_ENDBIT);
            return SDMMC_ERROR_CMD_END_BIT;
        }

        if (__SDMMC_GET_FLAG(SDIOx, SDMMC_INTR_STS_ERR_CMD_INDEX)) /*	CMD Index error */
        {
            __SDMMC_CLEAR_FLAG(SDIOx, SDMMC_INTR_STS_ERR_CMD_INDEX);
            return SDMMC_ERROR_CMD_INDEX;
        }

        if (__SDMMC_GET_FLAG(SDIOx, SDMMC_INTR_STS_ERR_CMD_TIMEOUT)) /*	CMD Timeout error */
        {
            __SDMMC_CLEAR_FLAG(SDIOx, SDMMC_INTR_STS_ERR_CMD_TIMEOUT);
            return SDMMC_ERROR_CMD_RSP_TIMEOUT;
        }
    }

    return SDMMC_ERROR_NONE;
}

/**
 * @brief  Checks for error conditions for R6 (RCA) response.
 * @param  hsd: SD handle
 * @param  SD_CMD: The sent command index
 * @param  pRCA: Pointer to the variable that will contain the SD card relative
 *         address RCA
 * @retval SD Card error state
 */
static uint32_t SDMMC_GetCmdResp6(SN_SDIO_Type *SDIOx, uint8_t SD_CMD, uint16_t *pRCA)
{
    uint32_t response_r1;

    /* 8 is the number of required instructions cycles for the below loop statement.
        The SDMMC_CMDTIMEOUT is expressed in ms */
    uint32_t count = SDMMC_CMDTIMEOUT * (SystemCoreClock / 8U / 1000U);

    /*
     * Wait for cmd complete
     * CMDINBCMD is cleared When the command response is receive
     */
    do
    {
        if (count-- == 0U)
        {
            /* Card is not SD V2.0 compliant */
            return SDMMC_ERROR_TIMEOUT;
        }
    } while (__SDMMC_GET_STA(SDIOx, SDMMC_STA_CMDINBCMD) && !__SDMMC_GET_FLAG(SDIOx, SDMMC_INTR_STS_ERR));

    if (__SDMMC_GET_FLAG(SDIOx, SDMMC_INTR_STS_ERR))
    {
        /* Check CMD Line Error */
        /* When only data timeout error occured, sdc021 cannot issue CMD12, CMD13. */
        if (__SDMMC_GET_FLAG(SDIOx, SDMMC_INTR_STS_ERR_CMD_LINE))
        {
            /* Software Reset for CMD line */
            __SDMMC_SWRST_SET(SN_SDIO, SDMMC_SOFTRST_CMD);

            while (__SDMMC_SWRST_GET_FLAG(SN_SDIO, SDMMC_SOFTRST_CMD))
                ;
        }

        if (__SDMMC_GET_FLAG(SDIOx, SDMMC_INTR_STS_ERR_CMD_CRC)) /*	CMD CRC error */
        {
            __SDMMC_CLEAR_FLAG(SDIOx, SDMMC_INTR_STS_ERR_CMD_CRC);
            return SDMMC_ERROR_CMD_CRC_FAIL;
        }

        if (__SDMMC_GET_FLAG(SDIOx, SDMMC_INTR_STS_ERR_CMD_ENDBIT)) /* CMD End bit error */
        {
            __SDMMC_CLEAR_FLAG(SDIOx, SDMMC_INTR_STS_ERR_CMD_ENDBIT);
            return SDMMC_ERROR_CMD_END_BIT;
        }

        if (__SDMMC_GET_FLAG(SDIOx, SDMMC_INTR_STS_ERR_CMD_INDEX)) /*	CMD Index error */
        {
            __SDMMC_CLEAR_FLAG(SDIOx, SDMMC_INTR_STS_ERR_CMD_INDEX);
            return SDMMC_ERROR_CMD_INDEX;
        }

        if (__SDMMC_GET_FLAG(SDIOx, SDMMC_INTR_STS_ERR_CMD_TIMEOUT)) /*	CMD Timeout error */
        {
            __SDMMC_CLEAR_FLAG(SDIOx, SDMMC_INTR_STS_ERR_CMD_TIMEOUT);
            return SDMMC_ERROR_CMD_RSP_TIMEOUT;
        }
    }

    /* We have received response, retrieve it.  */
    response_r1 = SDMMC_GetResponse(SDIOx, SDMMC_RESP0);

    if ((response_r1 & (SDMMC_R6_GENERAL_UNKNOWN_ERROR | SDMMC_R6_ILLEGAL_CMD | SDMMC_R6_COM_CRC_FAILED)) == SDMMC_ALLZERO)
    {
        *pRCA = (uint16_t)(response_r1 >> 16);

        return SDMMC_ERROR_NONE;
    }
    else if ((response_r1 & SDMMC_R6_ILLEGAL_CMD) == SDMMC_R6_ILLEGAL_CMD)
    {
        return SDMMC_ERROR_ILLEGAL_CMD;
    }
    else if ((response_r1 & SDMMC_R6_COM_CRC_FAILED) == SDMMC_R6_COM_CRC_FAILED)
    {
        return SDMMC_ERROR_COM_CRC_FAILED;
    }
    else
    {
        return SDMMC_ERROR_GENERAL_UNKNOWN_ERR;
    }
}

/**
 * @brief  Checks for error conditions for R7 response.
 * @param  hsd: SD handle
 * @retval SD Card error state
 */
static uint32_t SDMMC_GetCmdResp7(SN_SDIO_Type *SDIOx)
{
    /* 8 is the number of required instructions cycles for the below loop statement.
    The SDMMC_CMDTIMEOUT is expressed in ms */
    uint32_t count = SDMMC_CMDTIMEOUT * (SystemCoreClock / 8U / 1000U);

    /*
     * Wait for cmd complete
     * CMDINBCMD is cleared When the command response is receive
     */
    do
    {
        if (count-- == 0U)
        {
            /* Card is not SD V2.0 compliant */
            return SDMMC_ERROR_TIMEOUT;
        }
    } while (__SDMMC_GET_STA(SDIOx, SDMMC_STA_CMDINBCMD) && !__SDMMC_GET_FLAG(SDIOx, SDMMC_INTR_STS_ERR));

    if (__SDMMC_GET_FLAG(SDIOx, SDMMC_INTR_STS_ERR))
    {
        /* Check CMD Line Error */
        /* When only data timeout error occured, sdc021 cannot issue CMD12, CMD13. */
        if (__SDMMC_GET_FLAG(SDIOx, SDMMC_INTR_STS_ERR_CMD_LINE))
        {
            /* Software Reset for CMD line */
            __SDMMC_SWRST_SET(SN_SDIO, SDMMC_SOFTRST_CMD);

            while (__SDMMC_SWRST_GET_FLAG(SN_SDIO, SDMMC_SOFTRST_CMD))
                ;
        }

        if (__SDMMC_GET_FLAG(SDIOx, SDMMC_INTR_STS_ERR_CMD_CRC)) /*	CMD CRC error */
        {
            __SDMMC_CLEAR_FLAG(SDIOx, SDMMC_INTR_STS_ERR_CMD_CRC);
            return SDMMC_ERROR_CMD_CRC_FAIL;
        }

        if (__SDMMC_GET_FLAG(SDIOx, SDMMC_INTR_STS_ERR_CMD_ENDBIT)) /* CMD End bit error */
        {
            __SDMMC_CLEAR_FLAG(SDIOx, SDMMC_INTR_STS_ERR_CMD_ENDBIT);
            return SDMMC_ERROR_CMD_END_BIT;
        }

        if (__SDMMC_GET_FLAG(SDIOx, SDMMC_INTR_STS_ERR_CMD_INDEX)) /*	CMD Index error */
        {
            __SDMMC_CLEAR_FLAG(SDIOx, SDMMC_INTR_STS_ERR_CMD_INDEX);
            return SDMMC_ERROR_CMD_INDEX;
        }

        if (__SDMMC_GET_FLAG(SDIOx, SDMMC_INTR_STS_ERR_CMD_TIMEOUT)) /*	CMD Timeout error */
        {
            __SDMMC_CLEAR_FLAG(SDIOx, SDMMC_INTR_STS_ERR_CMD_TIMEOUT);
            return SDMMC_ERROR_CMD_RSP_TIMEOUT;
        }
    }

    return SDMMC_ERROR_NONE;
}
