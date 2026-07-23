/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#include "sn34f78x_hal.h"
#include "sn34f78x_hal_flash.h"

/** @addtogroup SN34F78X_HAL_Driver
 * @{
 */

/** @defgroup FLASHEx FLASHEx
 * @brief FLASH HAL Extension module driver
 * @{
 */

#if (configUSE_FLASH_PERIPHERAL == 1)

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/** @addtogroup FLASHEx_Private_Constants
 * @{
 */
#define FLASH_TIMEOUT_VALUE 50000U /* 50 s */
/**
 * @}
 */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/** @addtogroup FLASHEx_Private_Variables
 * @{
 */
extern FLASH_ProcessTypeDef pFlash;
/**
 * @}
 */

/* Private function prototypes -----------------------------------------------*/
/** @addtogroup FLASHEx_Private_Functions
 * @{
 */
/* Option bytes control */
static HAL_Status_T FLASH_CLKFreq_Config(uint32_t CLKFreq);
static HAL_Status_T FLASH_AUTOHold_Config(uint8_t AUTOHold);
static HAL_Status_T FLASH_Protect_GetStatus(uint8_t *WRPPage, uint8_t *SMWRP, uint32_t *RDPLevel);
static uint16_t     FLASH_GetMMWRP(uint8_t *WRPPage);
static uint16_t     FLASH_GetSMWRP(uint8_t *WRPPage);
static uint8_t      FLASH_GetRDP(uint32_t *RDPLevel);
static uint8_t      FLASH_GetBOOTFlag(uint8_t *BOOTbyte);
static uint8_t      FLASH_GetCLKFreq(uint32_t *CLKFreq);
static uint8_t      FLASH_GetAUTOHold(uint8_t *AUTOHold);
extern HAL_Status_T FLASH_WaitForLastOperation(uint32_t Timeout);
/**
 * @}
 */

/* Exported functions --------------------------------------------------------*/
/** @defgroup FLASHEx_Exported_Functions FLASHEx Exported Functions
 * @{
 */

/** @defgroup FLASHEx_Exported_Functions_Group1 Extended IO operation functions
 *  @brief   Extended IO operation functions
 *
@verbatim
 ===============================================================================
                ##### Extended programming operation functions #####
 ===============================================================================
    [..]
    This subsection provides a set of functions allowing to manage the Extension FLASH
    programming operations.

@endverbatim
  * @{
  */

#if (configUSE_FLASH_BLOCKING == 1)
/**
 * @brief  Perform a mass erase or erase the specified FLASH memory sectors
 * @param[in]  pEraseInit pointer to an FLASH_EraseInitTypeDef structure that
 *         contains the configuration information for the erasing.
 *
 * @param[out]  SectorError pointer to variable  that
 *         contains the configuration information on faulty sector in case of error
 *         (0xFFFFFFFFU means that all the sectors have been correctly erased)
 *
 * @retval HAL Status
 */
HAL_Status_T HAL_FLASHEx_Erase(FLASH_EraseInitTypeDef *pEraseInit, uint32_t *SectorError)
{
    HAL_Status_T status  = HAL_ERROR;
    uint32_t     address = 0U;

    /* Check the parameters */
    ASSERT_PARAM(IS_FLASH_TYPEERASE(pEraseInit->TypeErase));

    /* Wait for last operation to be completed */
    status         = FLASH_WaitForLastOperation((uint32_t)FLASH_TIMEOUT_VALUE);
    uint32_t index = 0U;

    if (pEraseInit->TypeErase == FLASH_TYPEERASE_MASSERASE)
    {
        /*Mass erase to be done*/
        FLASH_MassErase();

        /* Wait for last operation to be completed */
        status = FLASH_WaitForLastOperation((uint32_t)FLASH_TIMEOUT_VALUE);
    }
    else if (pEraseInit->TypeErase == FLASH_TYPEERASE_SECTORS)
    {
        address = pEraseInit->Sector + (pFlash.MapAddress / (FLASH_SECTOR_SIZE * FLASH_PAGE_SIZE));

        /* Check the parameters */
        ASSERT_PARAM(IS_FLASH_NBSECTORS(pEraseInit->NbSectors + address));

        /* Erase by sector by sector to be done*/
        for (index = address; index < (pEraseInit->NbSectors + address); index++)
        {
            FLASH_Erase_Sector(index);

            /* Wait for last operation to be completed */
            status = FLASH_WaitForLastOperation((uint32_t)FLASH_TIMEOUT_VALUE);

            if (status != HAL_OK)
            {
                /* In case of error, stop erase procedure and return the faulty sector*/
                *SectorError = index;
                break;
            }
        }
    }
    else // FLASH_TYPEERASE_PAGES
    {
        address = pEraseInit->Page + (pFlash.MapAddress / FLASH_PAGE_SIZE);

        /* Check the parameters */
        if ((pEraseInit->NbPages + address) > FLASH_PAGE_TOTAL)
        {
            *SectorError = pEraseInit->NbPages + address;
            return status;
        }

        ASSERT_PARAM(IS_FLASH_NBPAGES(pEraseInit->NbPages + address));

        /* Erase by sector by sector to be done*/
        for (index = address; index < (pEraseInit->NbPages + address); index++)
        {
            FLASH_Erase_Page(index);

            /* Wait for last operation to be completed */
            status = FLASH_WaitForLastOperation((uint32_t)FLASH_TIMEOUT_VALUE);

            if (status != HAL_OK)
            {
                /* In case of error, stop erase procedure and return the faulty sector*/
                *SectorError = index;
                break;
            }
        }
    }

    return status;
}

#endif // configUSE_FLASH_BLOCKING

#if (configUSE_FLASH_NONBLOCKING_IT == 1)
/**
 * @brief  Perform a mass erase or erase the specified FLASH memory sectors  with interrupt enabled
 * @param  pEraseInit pointer to an FLASH_EraseInitTypeDef structure that
 *         contains the configuration information for the erasing.
 *
 * @retval HAL Status
 */
HAL_Status_T HAL_FLASHEx_Erase_IT(FLASH_EraseInitTypeDef *pEraseInit)
{
    HAL_Status_T status  = HAL_OK;
    uint32_t     address = 0U;

    /* Check the parameters */
    ASSERT_PARAM(IS_FLASH_TYPEERASE(pEraseInit->TypeErase));

    /* Enable End of FLASH Operation interrupt */
    __HAL_FLASH_ENABLE_IT(FLASH_IT_EOP);

    if (pEraseInit->TypeErase == FLASH_TYPEERASE_MASSERASE)
    {
        /*Mass erase to be done*/
        pFlash.ProcedureOnGoing = FLASH_PROC_MASSERASE;
        FLASH_MassErase();
    }
    else if (pEraseInit->TypeErase == FLASH_TYPEERASE_SECTORS)
    {
        /* Erase by sector to be done*/

        address = pEraseInit->Sector + (pFlash.MapAddress / (FLASH_SECTOR_SIZE * FLASH_PAGE_SIZE));

        /* Check the parameters */
        ASSERT_PARAM(IS_FLASH_NBSECTORS(pEraseInit->NbSectors + address));

        pFlash.ProcedureOnGoing = FLASH_PROC_SECTERASE;
        pFlash.NbSectorsToErase = pEraseInit->NbSectors;
        pFlash.Sector           = address;

        /*Erase 1st sector and wait for IT*/
        FLASH_Erase_Sector(address);
    }
    else // FLASH_TYPEERASE_PAGES
    {
        /* Check the parameters */
        //        ASSERT_PARAM(IS_FLASH_NBPAGES(pEraseInit->NbPages + pEraseInit->Page));

        address = pEraseInit->Page + (pFlash.MapAddress / FLASH_PAGE_SIZE);

        pFlash.ProcedureOnGoing = FLASH_PROC_PAGEERASE;
        pFlash.NbPagesToErase   = pEraseInit->NbPages;
        pFlash.Page             = address;

        /*Erase 1st sector and wait for IT*/
        FLASH_Erase_Page(address);
    }

    return status;
}
#endif // configUSE_FLASH_NONBLOCKING_IT

/**
 * @brief  Config flash option
 * @param  pCfgInit pointer to an FLASH_OBInitStruct structure that
 *         contains the configuration information for the programming.
 *
 * @retval HAL Status
 */
HAL_Status_T HAL_FLASHEx_ConfigProgram(FLASH_ConfigProgramInitTypeDef *pCfgInit)
{
    HAL_Status_T status = HAL_ERROR;

    /* Check the parameters */
    ASSERT_PARAM(IS_OPTIONCFG(pCfgInit->OptionType));

    /*Clock Frequency configuration*/
    if ((pCfgInit->OptionType & OPTIONCFG_CLKFREQ) == OPTIONCFG_CLKFREQ)
    {
        status = FLASH_CLKFreq_Config(pCfgInit->CLKFreq);
    }

    /*Auto Hold configuration*/
    if ((pCfgInit->OptionType & OPTIONCFG_AUTOHOLD) == OPTIONCFG_AUTOHOLD)
    {
        status = FLASH_AUTOHold_Config(pCfgInit->AUTOHold);
    }

    return status;
}

/**
 * @brief   Get the Option byte configuration
 * @param  pOBInit pointer to an FLASH_OBInitStruct structure that
 *         contains the configuration information for the programming.
 *
 * @retval None
 */
void HAL_FLASHEx_GetConfig(FLASH_ConfigProgramInitTypeDef *pInit)
{
    pInit->OptionType = OPTIONCFG_WRP | OPTIONCFG_WRP | OPTIONCFG_BOOT | OPTIONCFG_CLKFREQ;

    /* Get MM WRP */
    FLASH_GetMMWRP(pInit->WRPALLZone);

    /* Get SM WRP */
    FLASH_GetSMWRP(&pInit->SMWRP);

    /* Get RDP Level */
    FLASH_GetRDP(&pInit->RDPLevel);

    /* Get Boot Flag */
    FLASH_GetBOOTFlag(&pInit->BOOTFlag);

    /* Get Boot Flag */
    FLASH_GetCLKFreq(&pInit->CLKFreq);

    /* Get Auto Hold */
    FLASH_GetAUTOHold(&pInit->AUTOHold);
}

/**
 * @brief
 * @param
 *
 *
 * @retval
 */
HAL_Status_T HAL_FLASHEx_ChecksumCalculate(uint32_t Address, uint32_t Size, uint16_t *CheckSum)
{
    HAL_Status_T status = HAL_OK;

    Address += pFlash.MapAddress;

    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation((uint32_t)FLASH_TIMEOUT_VALUE);

    if (status == HAL_OK)
    {
        HAL_REG_WRITE(SN_FLASH->MSG0, FLASH_OPERATION_CHECKSUM_CAL);
        HAL_REG_WRITE(SN_FLASH->MSG1, Address);
        HAL_REG_WRITE(SN_FLASH->MSG2, Size);

        HAL_REG_SBIT(SN_FLASH->PROC_START, FLASH_PROC_START);

        status = FLASH_WaitForLastOperation((uint32_t)FLASH_TIMEOUT_VALUE);

        if (status != HAL_OK)
            return status;

        *CheckSum = HAL_REG_READ(SN_FLASH->MSG3_b.STATUS_CODE2);
    }

    return status;
}
/**
 * @brief  Full erase of FLASH memory sectors
 * @retval None
 */
void FLASH_MassErase(void)
{
    HAL_REG_WRITE(SN_FLASH->MSG0_b.OPERATION, FLASH_OPERATION_ERASE);
    HAL_REG_WRITE(SN_FLASH->MSG0_b.TYPE, FLASH_TYPE_MM);
    HAL_REG_WRITE(SN_FLASH->MSG0_b.RANGE, FLASH_TYPEERASE_MASSERASE);
    HAL_REG_WRITE(SN_FLASH->MSG1, 0);
    HAL_REG_WRITE(SN_FLASH->MSG2, 0);

    HAL_REG_SBIT(SN_FLASH->PROC_START, FLASH_PROC_START);

    return;
}

/**
 * @brief  Erase the specified FLASH memory sector
 * @param  Sector FLASH sector to erase
 *         The value of this parameter depend on device used within the same series
 * @retval None
 */
void FLASH_Erase_Sector(uint32_t Sector)
{
    HAL_REG_WRITE(SN_FLASH->MSG0_b.OPERATION, FLASH_OPERATION_ERASE);
    HAL_REG_WRITE(SN_FLASH->MSG0_b.TYPE, FLASH_TYPE_MM);
    HAL_REG_WRITE(SN_FLASH->MSG0_b.RANGE, FLASH_TYPEERASE_SUBSECTORS);
    HAL_REG_WRITE(SN_FLASH->MSG1, Sector << 3);

    HAL_REG_SBIT(SN_FLASH->PROC_START, FLASH_PROC_START);

    return;
}

/**
 * @brief  Erase the specified FLASH memory page
 * @param  Page FLASH page to erase
 *         The value of this parameter depend on device used within the same series
 * @retval None
 */
void FLASH_Erase_Page(uint32_t Page)
{
    HAL_REG_WRITE(SN_FLASH->MSG0_b.OPERATION, FLASH_OPERATION_ERASE);
    HAL_REG_WRITE(SN_FLASH->MSG0_b.TYPE, FLASH_TYPE_MM);
    HAL_REG_WRITE(SN_FLASH->MSG0_b.RANGE, FLASH_TYPEERASE_PAGES);
    HAL_REG_WRITE(SN_FLASH->MSG1, Page);

    HAL_REG_SBIT(SN_FLASH->PROC_START, FLASH_PROC_START);

    return;
}

/**
 * @brief Config flash clock frequency
 * @param Clock frequency
 * @retval HAL Status
 */
static HAL_Status_T FLASH_CLKFreq_Config(uint32_t CLKFreq)
{
    HAL_Status_T status = HAL_OK;

    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation((uint32_t)FLASH_TIMEOUT_VALUE);

    if (status == HAL_OK)
    {
        HAL_REG_WRITE(SN_FLASH->MSG0_b.OPERATION, FLASH_OPERATION_FREQ_RW);
        HAL_REG_WRITE(SN_FLASH->MSG0_b.TYPE, FLASH_CFG_WRITE);
        HAL_REG_WRITE(SN_FLASH->MSG1, CLKFreq);

        HAL_REG_SBIT(SN_FLASH->PROC_START, FLASH_PROC_START);
    }
    return status;
}

/**
 * @brief AUTO Hold Function Config
 * @param Enable or Disable
 * @retval HAL Status
 */
static HAL_Status_T FLASH_AUTOHold_Config(uint8_t AUTOHold)
{
    HAL_Status_T status = HAL_OK;

    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation((uint32_t)FLASH_TIMEOUT_VALUE);

    if (status == HAL_OK)
    {
        SN_FLASH->AUTO_HOLD = AUTOHold;
    }
    return status;
}

/**
 * @brief  Set the read protection level.
 * @param  Level specifies the read protection level.
 *          This parameter can be one of the following values:
 *            @arg OB_RDP_LEVEL_0: No protection
 *            @arg OB_RDP_LEVEL_1: Read protection of the memory
 *            @arg OB_RDP_LEVEL_2: Full chip protection
 *
 * @note WARNING: When enabling OB_RDP level 2 it's no more possible to go back to level 1 or 0
 *
 * @retval HAL Status
 */
static HAL_Status_T FLASH_Protect_GetStatus(uint8_t *WRPPage, uint8_t *SMWRP, uint32_t *RDPLevel)
{
    HAL_Status_T status = HAL_OK;

    unsigned char *pBuf = (unsigned char *)FLASH_BUF_BASEADDR;
    unsigned       i;

    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation((uint32_t)FLASH_TIMEOUT_VALUE);

    HAL_REG_WRITE(SN_FLASH->MSG0, FLASH_OPERATION_PROT_STATUS);

    HAL_REG_SBIT(SN_FLASH->PROC_START, FLASH_PROC_START);

    status = FLASH_WaitForLastOperation((uint32_t)FLASH_TIMEOUT_VALUE);

    if (status == HAL_OK)
    {
        if (RDPLevel != NULL)
        {
            *RDPLevel = HAL_REG_READ(SN_FLASH->MSG3_b.STATUS_CODE2) >> 8;
            //			printf("RDP 0x%X\r\n", *RDPLevel);
        }

        if (SMWRP != NULL)
        {
            *SMWRP = HAL_REG_READ(SN_FLASH->MSG3_b.STATUS_CODE2) & 0x00FF;
            //			printf("SMWRP 0x%X\r\n", SMWRP);
        }

        if (WRPPage != NULL)
        {
            memset(WRPPage, 0, 8);
            for (i = 0; i < 8; i++)
                WRPPage[i] = *pBuf++;

            ////			printf("MMWRP : ");

            //    for (i = 0; i < 8; i++)
            //        printf("0x%x ", WRPPage[i]);

            //    printf("\r\n");
        }
    }

    return status;
}

/**
 * @brief  Set the read protection level.
 * @param  Level specifies the read protection level.
 *          This parameter can be one of the following values:
 *            @arg OB_RDP_LEVEL_0: No protection
 *            @arg OB_RDP_LEVEL_1: Read protection of the memory
 *            @arg OB_RDP_LEVEL_2: Full chip protection
 *
 * @note WARNING: When enabling OB_RDP level 2 it's no more possible to go back to level 1 or 0
 *
 * @retval HAL Status
 */

static uint8_t FLASH_GetRDP(uint32_t *RDPLevel)
{
    FLASH_Protect_GetStatus(NULL, NULL, RDPLevel);

    return HAL_OK;
}

/**
 * @brief  Return the FLASH MM Write Protection Option Bytes value.
 * @retval uint16_t FLASH Write Protection Option Bytes value
 */
static uint16_t FLASH_GetMMWRP(uint8_t *WRPPage)
{
    FLASH_Protect_GetStatus(WRPPage, NULL, NULL);
    return HAL_OK;
}

/**
 * @brief  Return the FLASH SM Write Protection Option Bytes value.
 * @retval uint16_t FLASH Write Protection Option Bytes value
 */
static uint16_t FLASH_GetSMWRP(uint8_t *SMWRP)
{
    FLASH_Protect_GetStatus(NULL, SMWRP, NULL);
    return HAL_OK;
}

/**
 * @brief  Returns the FLASH BOR level.
 * @retval uint8_t The FLASH BOR level:
 *           - OB_BOR_LEVEL3: Supply voltage ranges from 2.7 to 3.6 V
 *           - OB_BOR_LEVEL2: Supply voltage ranges from 2.4 to 2.7 V
 *           - OB_BOR_LEVEL1: Supply voltage ranges from 2.1 to 2.4 V
 *           - OB_BOR_OFF   : Supply voltage ranges from 1.62 to 2.1 V
 */
static uint8_t FLASH_GetBOOTFlag(uint8_t *BOOTFlag)
{
    HAL_Status_T status = HAL_OK;

    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation((uint32_t)FLASH_TIMEOUT_VALUE);

    if (status == HAL_OK)
    {
        HAL_REG_WRITE(SN_FLASH->MSG0_b.OPERATION, FLASH_OPERATION_BOOT_FLAG_RW);
        HAL_REG_WRITE(SN_FLASH->MSG0_b.TYPE, FLASH_CFG_READ);

        HAL_REG_SBIT(SN_FLASH->PROC_START, FLASH_PROC_START);

        /* Wait for last operation to be completed */
        status = FLASH_WaitForLastOperation((uint32_t)FLASH_TIMEOUT_VALUE);

        if (status != HAL_OK)
            return status;

        *BOOTFlag = SN_FLASH->MSG3_b.STATUS_CODE2 & 0x00FF;
    }
    return status;
}

/**
 * @brief
 * @retval
 *
 *
 *
 *
 */
static uint8_t FLASH_GetCLKFreq(uint32_t *CLKFreq)
{
    HAL_Status_T status = HAL_OK;

    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation((uint32_t)FLASH_TIMEOUT_VALUE);

    if (status == HAL_OK)
    {
        HAL_REG_WRITE(SN_FLASH->MSG0_b.OPERATION, FLASH_OPERATION_FREQ_RW);
        HAL_REG_WRITE(SN_FLASH->MSG0_b.TYPE, FLASH_CFG_READ);

        HAL_REG_SBIT(SN_FLASH->PROC_START, FLASH_PROC_START);

        /* Wait for last operation to be completed */
        status = FLASH_WaitForLastOperation((uint32_t)FLASH_TIMEOUT_VALUE);

        if (status != HAL_OK)
            return status;

        *CLKFreq = *(volatile unsigned long *)(0x600000);
    }
    return status;
}

/**
 * @brief
 * @retval
 *
 *
 *
 *
 */
static uint8_t FLASH_GetAUTOHold(uint8_t *AUTOHold)
{
    HAL_Status_T status = HAL_OK;

    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation((uint32_t)FLASH_TIMEOUT_VALUE);

    if (status == HAL_OK)
    {
        *AUTOHold = HAL_REG_READ(SN_FLASH->AUTO_HOLD_b.AUTO_HOLD);
    }
    return status;
}

/**
 * @}
 */

#endif /* configUSE_FLASH_PERIPHERAL */

/**
 * @}
 */

/**
 * @}
 */
