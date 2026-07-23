/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#include "sn34f78x_hal_flash.h"

#if (configUSE_FLASH_PERIPHERAL == 1)

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/** @addtogroup FLASH_Private_Constants
 * @{
 */
#define FLASH_TIMEOUT_VALUE 50000U /* 50 s */
/**
 * @}
 */
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/** @addtogroup FLASH_Private_Variables
 * @{
 */
/* Variable used for Erase sectors under interruption */
FLASH_ProcessTypeDef pFlash;
/**
 * @}
 */

/* Private function prototypes -----------------------------------------------*/
/** @addtogroup FLASH_Private_Functions
 * @{
 */
/* Program operations */
static void FLASH_SetErrorCode(void);

HAL_Status_T FLASH_WaitForLastOperation(uint32_t Timeout);
/**
 * @}
 */

/* Exported functions --------------------------------------------------------*/
/** @defgroup FLASH_Exported_Functions FLASH Exported Functions
 * @{
 */

/** @defgroup FLASH_Exported_Functions_Group1 Programming operation functions
 *  @brief   Programming operation functions
 *
@verbatim
 ===============================================================================
                  ##### Programming operation functions #####
 ===============================================================================
    [..]
    This subsection provides a set of functions allowing to manage the FLASH
    program operations.

@endverbatim
  * @{
  */

#if ((configUSE_FLASH_BLOCKING == 1) || (configUSE_FLASH_NONBLOCKING_IT == 1))
/**
 * @brief  Program a page (512 bytes) at a specified address.
 * @note   This function must be used when the device voltage range is from
 *         2.7V to 3.6V and Vpp in the range 7V to 9V.
 *
 * @note   If an erase and a program operations are requested simultaneously,
 *         the erase operation is performed before the program one.
 *
 * @param  Address specifies the address to be programmed.
 * @param  Data specifies the data buffer address to program.
 * @retval None
 */
static void FLASH_Program_Setup(uint32_t Address, uint32_t size)
{
    uint8_t  op     = FLASH_OPERATION_ARB_WRITE;
    uint16_t offset = 0;

    if (size == FLASH_PROGRAM_SIZE_PAGE)
    {
        /* Page Program*/
        op = FLASH_OPERATION_PROGRAM;
    }
    else
    {
        /* Arbitrary write */
        offset = Address & 0x1FF; // offset of a page
        HAL_REG_WRITE(SN_FLASH->MSG2, (offset << 9 | size - 1));
    }

    HAL_REG_WRITE(SN_FLASH->MSG0_b.OPERATION, op);
    HAL_REG_WRITE(SN_FLASH->MSG0_b.TYPE, FLASH_TYPE_MM);
    HAL_REG_WRITE(SN_FLASH->MSG1, Address >> 9);
}

/**
 * @brief  Program a page (512 bytes) at a specified address.
 * @note   This function must be used when the device voltage range is from
 *         2.7V to 3.6V and Vpp in the range 7V to 9V.
 *
 * @note   If an erase and a program operations are requested simultaneously,
 *         the erase operation is performed before the program one.
 *
 * @param  Address specifies the address to be programmed.
 * @param  Data specifies the data buffer address to program.
 * @retval None
 */
static void FLASH_Program_Page(uint32_t Address, uint32_t *Data)
{
    unsigned char *pBuf = (unsigned char *)FLASH_BUF_BASEADDR;
    char          *buf  = (char *)Data;
    unsigned       i;
    unsigned       size = FLASH_PROGRAM_SIZE_PAGE;

    /* Check the parameters */
    ASSERT_PARAM(IS_FLASH_ADDRESS((int32_t)Address));

    /* Write data to data buffer*/
    for (i = 0; i < size; i++)
        *pBuf++ = *buf++;

    /* Program setup */
    FLASH_Program_Setup(Address, size);

    /* Program start */
    HAL_REG_SBIT(SN_FLASH->PROC_START, FLASH_PROC_START);
}

/**
 * @brief  Program arbitrary size at a specified address.
 *
 * @param  Address specifies the address to be programmed.
 * @param  Data specifies the data buffer address to program.
 * @param  Size specifies the data size you want to program.
 * @retval None
 */
static void FLASH_Program_Arbitrary(uint32_t Address, uint8_t *Data, uint32_t size)
{
    unsigned char *pBuf = (unsigned char *)FLASH_BUF_BASEADDR;
    char          *buf  = (char *)Data;
    unsigned       i;

    HAL_Status_T status = HAL_ERROR;

    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation((uint32_t)FLASH_TIMEOUT_VALUE);

    if (status == HAL_OK)
    {
        Address += pFlash.MapAddress;

        /* Check the parameters */
        ASSERT_PARAM(IS_FLASH_ADDRESS((int32_t)Address));

        /* Write data to data buffer*/
        for (i = 0; i < size; i++)
            *pBuf++ = *buf++;

        FLASH_Program_Setup(Address, size);

        /* Program start */
        HAL_REG_SBIT(SN_FLASH->PROC_START, FLASH_PROC_START);
    }
}
#endif

#if (configUSE_FLASH_BLOCKING == 1)
/**
 * @brief  Program page at a specified address.
 * @param  Address specifies the address to be programmed.
 * @param  Data specifies the data buffer address to program.
 *
 * @retval HAL_Status_T HAL Status
 */
HAL_Status_T HAL_FLASH_Program_Page(uint32_t Address, uint8_t *Data)
{
    HAL_Status_T status = HAL_ERROR;

    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation((uint32_t)FLASH_TIMEOUT_VALUE);

    Address += pFlash.MapAddress;

    if (status == HAL_OK)
    {
        /*Program a page (512-byte) at a specified address.*/
        FLASH_Program_Page(Address, (uint32_t *)Data);

        /* Wait for last operation to be completed */
        status = FLASH_WaitForLastOperation((uint32_t)FLASH_TIMEOUT_VALUE);
    }

    return status;
}

/**
 * @brief  Program arbitrary size at a specified address
 * @param  Address  specifies the address to be programmed.
 * @param  Data specifies the data buffer address to program.
 * @param  Size specifies the data size you want to program.
 *
 * @retval HAL_Status_T HAL Status
 */
HAL_Status_T HAL_FLASH_Program_Arbitrary(uint32_t Address, uint8_t *Data, uint32_t Size)
{
    HAL_Status_T status = HAL_ERROR;

    /* length + offset should be  less or equal 512 for arbitrary write */
    if (((Address & 0x1FF) + Size) > FLASH_PAGE_SIZE)
        return HAL_ERROR;

    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation((uint32_t)FLASH_TIMEOUT_VALUE);

    if (status == HAL_OK)
    {
        /*Program arbitrary size at a specified address.*/
        FLASH_Program_Arbitrary(Address, Data, Size);

        /* Wait for last operation to be completed */
        status = FLASH_WaitForLastOperation((uint32_t)FLASH_TIMEOUT_VALUE);
    }

    return status;
}

#endif // configUSE_FLASH_BLOCKING

#if (configUSE_FLASH_NONBLOCKING_IT == 1)
/**
 * @brief  Program page at a specified address with interrupt enabled.
 * @param  Address  specifies the address to be programmed.
 * @param  Data specifies the data buffer address to program.
 *
 * @retval HAL Status
 */
HAL_Status_T HAL_FLASH_Program_Page_IT(uint32_t Address, uint8_t *Data)
{
    HAL_Status_T status = HAL_OK;

    /* Enable End of FLASH Operation interrupt */
    __HAL_FLASH_ENABLE_IT(FLASH_IT_EOP);

    pFlash.ProcedureOnGoing = FLASH_PROC_PROGRAM;

    pFlash.Address = Address;

    Address += pFlash.MapAddress;

    /*Program a page (512-byte) at a specified address.*/
    FLASH_Program_Page(Address, (uint32_t *)(uint32_t)Data);

    return status;
}

/**
 * @brief  Program arbitrary size at a specified address  with interrupt enabled.
 * @param  Address  specifies the address to be programmed.
 * @param  Data specifies the data buffer address to program.
 * @param  Size specifies the data size you want to program.
 *
 * @retval HAL Status
 */
HAL_Status_T HAL_FLASH_Program_Arbitrary_IT(uint32_t Address, uint8_t *Data, uint32_t Size)
{
    HAL_Status_T status = HAL_OK;

    /* length + offset should be less or equal 512 for arbitrary write */
    if (((Address & 0x1FF) + Size) > FLASH_PAGE_SIZE)
        return HAL_ERROR;

    /* Enable End of FLASH Operation interrupt */
    __HAL_FLASH_ENABLE_IT(FLASH_IT_EOP);

    pFlash.ProcedureOnGoing = FLASH_PROC_PROGRAM;

    pFlash.Address = Address;

    /*Program arbitrary size at a specified address.*/
    FLASH_Program_Arbitrary(Address, Data, Size);

    return status;
}
/**
 * @brief This function handles FLASH interrupt request.
 * @retval None
 */
ATTR_ISR void HAL_FLASH_IRQHandler(void)
{
    uint32_t addresstmp = 0U;

    /* Check FLASH End of Operation flag  */
    if (__HAL_FLASH_GET_STATUS(FLASH_STATUS_DONE))
    {
        /* Clear FLASH End of Operation pending bit */
        __HAL_FLASH_CLEAR_STATUS(FLASH_STATUS_DONE);

        if (pFlash.ProcedureOnGoing == FLASH_PROC_SECTERASE)
        {
            /*Nb of sector to erased can be decreased*/
            pFlash.NbSectorsToErase--;

            /* Check if there are still sectors to erase*/
            if (pFlash.NbSectorsToErase != 0U)
            {
                addresstmp = pFlash.Sector;
                /*Indicate user which sector has been erased*/
                HAL_FLASH_EndOfOperationCallback(addresstmp);

                /*Increment sector number*/
                pFlash.Sector++;
                addresstmp = pFlash.Sector;
                FLASH_Erase_Sector(addresstmp);
            }
            else
            {
                /*No more sectors to Erase, user callback can be called.*/
                /*Reset Sector and stop Erase sectors procedure*/
                pFlash.Sector = addresstmp = 0xFFFFFFFFU;
                pFlash.ProcedureOnGoing    = FLASH_PROC_NONE;

                /* FLASH EOP interrupt user callback */
                HAL_FLASH_EndOfOperationCallback(addresstmp);
            }
        }
        else if (pFlash.ProcedureOnGoing == FLASH_PROC_PAGEERASE)
        {
            /*Nb of sector to erased can be decreased*/
            pFlash.NbPagesToErase--;

            /* Check if there are still sectors to erase*/
            if (pFlash.NbPagesToErase != 0U)
            {
                addresstmp = pFlash.Page;
                /*Indicate user which sector has been erased*/
                HAL_FLASH_EndOfOperationCallback(addresstmp);

                /*Increment sector number*/
                pFlash.Page++;
                addresstmp = pFlash.Page;
                FLASH_Erase_Page(addresstmp);
            }
            else
            {
                /*No more sectors to Erase, user callback can be called.*/
                /*Reset Sector and stop Erase sectors procedure*/
                pFlash.Sector = addresstmp = 0xFFFFFFFFU;
                pFlash.ProcedureOnGoing    = FLASH_PROC_NONE;

                /* FLASH EOP interrupt user callback */
                HAL_FLASH_EndOfOperationCallback(addresstmp);
            }
        }
        else
        {
            if (pFlash.ProcedureOnGoing == FLASH_PROC_MASSERASE)
            {
                /* FLASH EOP interrupt user callback */
                addresstmp = 0xFFFFFFFFU;
                HAL_FLASH_EndOfOperationCallback(addresstmp);
            }
            else
            {
                /*Program ended. Return the selected address*/
                /* FLASH EOP interrupt user callback */
                HAL_FLASH_EndOfOperationCallback(pFlash.Address);
            }
            pFlash.ProcedureOnGoing = FLASH_PROC_NONE;
        }
    }
    else if (__HAL_FLASH_GET_STATUS(FLASH_STATUS_FAIL))
    {
        if (pFlash.ProcedureOnGoing == FLASH_PROC_SECTERASE)
        {
            /*return the faulty sector*/
            addresstmp    = pFlash.Sector;
            pFlash.Sector = 0xFFFFFFFFU;
        }
        else if (pFlash.ProcedureOnGoing == FLASH_PROC_MASSERASE)
        {
            /*return the faulty bank*/
            addresstmp = pFlash.Sector;
        }
        else
        {
            /*return the faulty address*/
            addresstmp = pFlash.Address;
        }

        /*Save the error code*/
        FLASH_SetErrorCode();

        /* Clear FLASH Fail of Operation pending bit */
        __HAL_FLASH_CLEAR_STATUS(FLASH_STATUS_FAIL);

        HAL_FLASH_OperationErrorCallback(addresstmp);

        pFlash.ProcedureOnGoing = FLASH_PROC_NONE;
    }

    if (pFlash.ProcedureOnGoing == FLASH_PROC_NONE)
    {
        /* Disable End of FLASH Operation interrupt */
        __HAL_FLASH_DISABLE_IT(FLASH_IT_EOP);
    }
}

#endif // configUSE_FLASH_NONBLOCKING_IT

#if (configUSE_FLASH_REGISTER_CALLBACKS == 1)
/**
 * @brief  FLASH end of operation interrupt callback
 * @param  ReturnValue The value saved in this parameter depends on the ongoing procedure
 *                  Mass Erase: Bank number which has been requested to erase
 *                  Sectors Erase: Sector which has been erased
 *                    (if 0xFFFFFFFFU, it means that all the selected sectors have been erased)
 *                  Program: Address which was selected for data program
 * @retval None
 */
__weak void HAL_FLASH_EndOfOperationCallback(uint32_t ReturnValue)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(ReturnValue);
    /* NOTE : This function Should not be modified, when the callback is needed,
              the HAL_FLASH_EndOfOperationCallback could be implemented in the user file
     */
}

/**
 * @brief  FLASH operation error interrupt callback
 * @param  ReturnValue The value saved in this parameter depends on the ongoing procedure
 *                 Mass Erase: Bank number which has been requested to erase
 *                 Sectors Erase: Sector number which returned an error
 *                 Program: Address which was selected for data program
 * @retval None
 */
__weak void HAL_FLASH_OperationErrorCallback(uint32_t ReturnValue)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(ReturnValue);
    /* NOTE : This function Should not be modified, when the callback is needed,
              the HAL_FLASH_OperationErrorCallback could be implemented in the user file
     */
}

/**
 * @}
 */
#endif

/** @defgroup FLASH_Exported_Functions_Group2 Peripheral Control functions
 *  @brief   management functions
 *
@verbatim
 ===============================================================================
                      ##### Peripheral Control functions #####
 ===============================================================================
    [..]
    This subsection provides a set of functions allowing to control the FLASH
    memory operations.

@endverbatim
  * @{
  */

/**
 * @}
 */

/** @defgroup FLASH_Exported_Functions_Group3 Peripheral State and Errors functions
 *  @brief   Peripheral Errors functions
 *
@verbatim
 ===============================================================================
                ##### Peripheral Errors functions #####
 ===============================================================================
    [..]
    This subsection permits to get in run-time Errors of the FLASH peripheral.

@endverbatim
  * @{
  */

/**
 * @brief  Get the specific FLASH error flag.
 * @retval FLASH_ErrorCode: The returned value can be a combination of:
 *            @arg HAL_FLASH_ERROR_RD: FLASH Read Protection error flag (PCROP)
 *            @arg HAL_FLASH_ERROR_PGS: FLASH Programming Sequence error flag
 *            @arg HAL_FLASH_ERROR_PGP: FLASH Programming Parallelism error flag
 *            @arg HAL_FLASH_ERROR_PGA: FLASH Programming Alignment error flag
 *            @arg HAL_FLASH_ERROR_WRP: FLASH Write protected error flag
 *            @arg HAL_FLASH_ERROR_OPERATION: FLASH operation Error flag
 */
uint32_t HAL_FLASH_GetError(void)
{
    return pFlash.ErrorCode;
}

/**
 * @}
 */

/**
 * @brief  Wait for a FLASH operation to complete.
 * @param  Timeout maximum flash operationtimeout
 * @retval HAL Status
 */
HAL_Status_T FLASH_WaitForLastOperation(uint32_t Timeout)
{
    uint32_t tickstart = 0U;

    /* Clear Error Code */
    pFlash.ErrorCode = HAL_FLASH_ERROR_NONE;

    /* Wait for the FLASH operation to complete by polling on BUSY flag */
    /* Get tick */
    tickstart = HAL_GetTick();

    while (__HAL_FLASH_GET_BUSY(FLASH_PROC_STATUS_BSY))
    {
        if (Timeout != HAL_MAX_DELAY)
        {
            if ((Timeout == 0U) || ((HAL_GetTick() - tickstart) > Timeout))
            {
                return HAL_ERROR;
            }
        }
    }

    /* Check FLASH End of Operation flag  */
    if (__HAL_FLASH_GET_STATUS(FLASH_STATUS_DONE))
    {
        /* Clear FLASH End of Operation pending bit */
        __HAL_FLASH_CLEAR_STATUS(FLASH_STATUS_DONE);
    }
    else if (__HAL_FLASH_GET_STATUS(FLASH_STATUS_FAIL))
    {
        /*Save the error code*/
        FLASH_SetErrorCode();

        /* Clear FLASH Fail of Operation pending bit */
        __HAL_FLASH_CLEAR_STATUS(FLASH_STATUS_FAIL);

        return HAL_ERROR;
    }

    /* If there is no error flag set */
    return HAL_OK;
}

/**
 * @brief  Set the specific FLASH error flag.
 * @retval None
 */
static void FLASH_SetErrorCode(void)
{
    pFlash.ErrorCode = HAL_REG_READ(SN_FLASH->MSG3_b.STATUS_CODE);
}

/**
 * @brief  Flash init when boot up.
 * @retval None
 */
void HAL_InitFlash(void)
{
    FLASH_ConfigProgramInitTypeDef CfgInit;

    /* Setup Remap addr */
    if (__HAL_RCC_GET_REMAP())
    {
        pFlash.MapAddress = FLASH_USER_MAP_ADDR;
    }
    else
    {
        pFlash.MapAddress = FLASH_BOOT_MAP_ADDR;
    }

    CfgInit.OptionType = OPTIONCFG_AUTOHOLD;
    CfgInit.AUTOHold   = HAL_ENABLE;
    HAL_FLASHEx_ConfigProgram(&CfgInit);

    return;
}

/**
 * @}
 */

#endif /* configUSE_FLASH_PERIPHERAL */
