/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#include "sn34f78x_ll_flash.h"

#if defined(USE_FULL_LL_DRIVER)

LL_FLASH_Process_T p_flash;
/* Exported functions --------------------------------------------------------*/

/**
 * @brief  Flash init when boot up.
 * @retval None
 */
void LL_InitFlash(void)
{
    LL_FLASH_Config_ProgramInit_T cfg_init;

    /* Setup Remap addr */
    if (LL_REG_READ(SN_SCU->PWRMODE_b.REMAP))
    {
        p_flash.MapAddress = LL_FLASH_USER_MAP_ADDR;
    }
    else
    {
        p_flash.MapAddress = LL_FLASH_BOOT_MAP_ADDR;
    }

    cfg_init.option_type = LL_OPTION_AUTOHOLD;
    cfg_init.auto_hold   = LL_ENABLE;
    LL_FLASH_ConfigProgram(&cfg_init);

    return;
}

/**
 * @brief  Config flash option
 * @param  cfg_init pointer to an FLASH_OBInitStruct structure that
 *         contains the configuration information for the programming.
 *
 * @retval LL Status
 */
LL_Status_T LL_FLASH_ConfigProgram(LL_FLASH_Config_ProgramInit_T *cfg_init)
{
    LL_Status_T status = LL_ERROR;

    /* Check the parameters */
    RET_FLAG_FALSE(IS_LL_OPTION_CFG(cfg_init->option_type), LL_ERROR);

    /*Clock Frequency configuration*/
    if ((cfg_init->option_type & LL_OPTION_CLKFREQ) == LL_OPTION_CLKFREQ)
    {
        status = _flash_clk_freq_config(cfg_init->clk_freq);
    }

    /*Auto Hold configuration*/
    if ((cfg_init->option_type & LL_OPTION_AUTOHOLD) == LL_OPTION_AUTOHOLD)
    {
        status = _flash_auto_hold_config(cfg_init->auto_hold);
    }

    return status;
}

/**
 * @brief  Wait for a FLASH operation to complete.
 * @param  timeout maximum flash operation timeout
 * @retval LL Status
 */
LL_Status_T _wait_for_last_operation(uint32_t timeout)
{
    uint32_t tick_start = 0U;

    p_flash.ErrorCode = LL_FLASH_ERROR_NONE;

    /* Wait for the FLASH operation to complete by polling on BUSY flag */
    /* Get tick */
    tick_start = LL_GetTick();

    while (LL_FLASH_IsActiveFlag_Busy() == LL_ENABLE)
    {
        if (timeout != 0xFFFFFFFFU)
        {
            if ((timeout == 0U) || ((LL_GetTick() - tick_start) > timeout))
            {
                return LL_ERROR;
            }
        }
    }

    /* Check FLASH End of Operation flag  */
    if (LL_FLASH_IsActiveFlag_DONE() == LL_ENABLE)
    {
        /* Clear FLASH End of Operation pending bit */
        LL_FLASH_ClearFlag_DONE();
    }
    else if (LL_FLASH_IsActiveFlag_FAIL() == LL_ENABLE)
    {
        /* Clear FLASH Fail of Operation pending bit */
        LL_FLASH_ClearFlag_FAIL();

        p_flash.ErrorCode = LL_FLASH_GetStatusCode();

        return LL_ERROR;
    }

    /* If there is no error flag set */
    return LL_OK;
}

/**
 * @brief  Get config
 * @param  pro_init pointer to an FLASH_OBInitStruct structure that
 *         contains the configuration information for the programming.
 * @retval LL Status
 */
void LL_FLASH_GetConfig(LL_FLASH_Config_ProgramInit_T *pro_init)
{
    pro_init->option_type = LL_OPTION_WRP | LL_OPTION_WRP | LL_OPTION_BOOT | LL_OPTION_CLKFREQ;

    /* Get MM WRP */
    _flash_get_mm_wrp(pro_init->wrp_all_zone);

    /* Get SM WRP */
    _flash_get_sm_wrp(&pro_init->sm_wrp);

    /* Get RDP Level */
    _flash_get_rdp(&pro_init->rdp_level);

    /* Get Boot Flag */
    _flash_get_boot_flag(&pro_init->boot_flag);

    /* Get Boot Flag */
    _flash_get_clk_freq(&pro_init->clk_freq);

    /* Get Auto Hold */
    _flash_get_auto_hold(&pro_init->auto_hold);
}

uint16_t _flash_get_mm_wrp(uint8_t *wrp_page)
{
    return _flash_protect_get_status(wrp_page, NULL, NULL);
}

uint16_t _flash_get_sm_wrp(uint8_t *sm_wrp)
{
    return _flash_protect_get_status(NULL, sm_wrp, NULL);
}

uint16_t _flash_get_rdp(uint32_t *rdp_level)
{
    return _flash_protect_get_status(NULL, NULL, rdp_level);
}

LL_Status_T _flash_protect_get_status(uint8_t *wrp_page, uint8_t *sm_wrp, uint32_t *rdp_level)
{
    LL_Status_T status = LL_OK;

    unsigned char *pBuf = (unsigned char *)LL_FLASH_BUF_BASEADDR;
    unsigned       i;

    /* Wait for last operation to be completed */
    status = _wait_for_last_operation((uint32_t)LL_FLASH_TIMEOUT_VALUE);

    LL_FLASH_SetOperation(LL_FLASH_OPERATION_PROT_STATUS);
    LL_FLASH_SetMemoryType(LL_FLASH_TYPE_MM);
    LL_FLASH_SetMemoryRange(LL_FLASH_MEM_RANGE_PAGE);

    LL_FLASH_StartProcess();

    status = _wait_for_last_operation((uint32_t)LL_FLASH_TIMEOUT_VALUE);

    if (status == LL_OK)
    {
        if (rdp_level != NULL)
        {
            *rdp_level = LL_FLASH_GetStatusCode2() >> 8;
        }

        if (sm_wrp != NULL)
        {
            *sm_wrp = LL_FLASH_GetStatusCode2() & 0x00FF;
        }

        if (wrp_page != NULL)
        {
            memset(wrp_page, 0, 8);
            for (i = 0; i < 8; i++)
                wrp_page[i] = *pBuf++;
        }
    }

    return status;
}

uint16_t _flash_get_boot_flag(uint8_t *boot_flag)
{
    LL_Status_T status = LL_OK;

    /* Wait for last operation to be completed */
    status = _wait_for_last_operation((uint32_t)LL_FLASH_TIMEOUT_VALUE);

    if (status == LL_OK)
    {
        LL_FLASH_SetOperation(LL_FLASH_OPERATION_BOOT_FLAG_RW);
        LL_FLASH_SetMemoryType(LL_FLASH_TYPE_MM);

        LL_FLASH_StartProcess();

        /* Wait for last operation to be completed */
        status = _wait_for_last_operation((uint32_t)LL_FLASH_TIMEOUT_VALUE);

        if (status != LL_OK)
            return status;

        *boot_flag = LL_FLASH_GetStatusCode2() & 0x00FF;
    }
    return status;
}

uint16_t _flash_get_clk_freq(uint32_t *clk_freq)
{
    LL_Status_T status = LL_OK;

    /* Wait for last operation to be completed */
    status = _wait_for_last_operation((uint32_t)LL_FLASH_TIMEOUT_VALUE);

    if (status == LL_OK)
    {
        LL_FLASH_SetOperation(LL_FLASH_OPERATION_FREQ_RW);
        LL_FLASH_SetMemoryType(LL_FLASH_TYPE_MM);

        LL_FLASH_StartProcess();

        /* Wait for last operation to be completed */
        status = _wait_for_last_operation((uint32_t)LL_FLASH_TIMEOUT_VALUE);

        if (status != LL_OK)
            return status;

        *clk_freq = *(volatile unsigned long *)(0x600000);
    }
    return status;
}

uint16_t _flash_get_auto_hold(uint8_t *auto_hold)
{
    LL_Status_T status = LL_OK;

    /* Wait for last operation to be completed */
    status = _wait_for_last_operation((uint32_t)LL_FLASH_TIMEOUT_VALUE);

    if (status == LL_OK)
    {
        *auto_hold = LL_FLASH_IsEnabled_AutoHold();
    }
    return status;
}

/**
 * @brief  Erase the specified FLASH memory sector
 * @param  sector FLASH sector to erase
 *         The value of this parameter depend on device used within the same series
 * @retval none
 */
void _flash_erase_sector(uint32_t sector)
{
    LL_FLASH_SetOperation(LL_FLASH_OPERATION_ERASE);
    LL_FLASH_SetMemoryType(LL_FLASH_TYPE_MM);
    LL_FLASH_SetMemoryRange(LL_FLASH_MEM_RANGE_SUB_SECTOR);
    LL_FLASH_SetMessage1(sector << 3);

    LL_FLASH_StartProcess();

    return;
}

/**
 * @brief  Erase the specified FLASH memory page
 * @param  page FLASH page to erase
 *         The value of this parameter depend on device used within the same series
 * @retval None
 */
void _flash_erase_page(uint32_t page)
{
    LL_FLASH_SetOperation(LL_FLASH_OPERATION_ERASE);
    LL_FLASH_SetMemoryType(LL_FLASH_TYPE_MM);
    LL_FLASH_SetMemoryRange(LL_FLASH_MEM_RANGE_PAGE);
    LL_FLASH_SetMessage1(page);

    LL_FLASH_StartProcess();

    return;
}

void _flash_erase_mass(void)
{
    LL_FLASH_SetOperation(LL_FLASH_OPERATION_ERASE);
    LL_FLASH_SetMemoryType(LL_FLASH_TYPE_MM);
    LL_FLASH_SetMemoryRange(LL_FLASH_MEM_RANGE_MASS);
    LL_FLASH_SetMessage1(0);
    LL_FLASH_SetMessage2(0);

    LL_FLASH_StartProcess();

    return;
}

LL_Status_T _flash_auto_hold_config(uint8_t auto_hold)
{
    LL_Status_T status = LL_OK;

    /* Wait for last operation to be completed */
    status = _wait_for_last_operation((uint32_t)LL_FLASH_TIMEOUT_VALUE);

    if (status == LL_OK)
    {
        if (auto_hold == LL_ENABLE)
            LL_FLASH_Enable_AutoHold();
        else
            LL_FLASH_Disable_AutoHold();
    }
    return status;
}

LL_Status_T _flash_clk_freq_config(uint32_t clk_freq)
{
    LL_Status_T status = LL_OK;

    /* Wait for last operation to be completed */
    status = _wait_for_last_operation((uint32_t)LL_FLASH_TIMEOUT_VALUE);

    if (status == LL_OK)
    {
        LL_FLASH_SetOperation(LL_FLASH_OPERATION_FREQ_RW);
        LL_FLASH_SetMemoryType(LL_FLASH_TYPE_SM);
        LL_FLASH_SetMessage1(clk_freq);

        LL_FLASH_StartProcess();
    }
    return status;
}

#endif /* USE_FULL_LL_DRIVER */
