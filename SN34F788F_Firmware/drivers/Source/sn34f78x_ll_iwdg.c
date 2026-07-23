/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#include "sn34f78x_ll_iwdg.h"

#if defined(USE_FULL_LL_DRIVER)

/* Exported functions --------------------------------------------------------*/
LL_Status_T LL_IWDG_Init(SN_WDT_Type *iwdg, LL_IWDG_Init_T *iwdg_init)
{
    LL_Status_T status = LL_OK;

    /* Check the parameters */
    RET_FLAG_FALSE(IS_LL_IWDG_LOAD(iwdg_init->timer_constant), LL_ERROR);

    LL_IWDG_ReloadCounter(iwdg);
    /* Set reload value*/
    LL_IWDG_SetReloadCounter(iwdg, iwdg_init->timer_constant);
    /* Set mode*/
    if (iwdg_init->reset_en == LL_ENABLE)
        LL_IWDG_Enable_Reset(iwdg);
    if (iwdg_init->ie == LL_ENABLE)
        LL_IWDG_EnableIT_Timeout(iwdg);

    LL_IWDG_ReloadCounter(iwdg);
    LL_IWDG_Enable(iwdg);

    return status;
}

LL_Status_T LL_IWDG_DeInit(SN_WDT_Type *iwdg)
{
    LL_Status_T status = LL_OK;
    /* Disable IWDG */
    LL_IWDG_Disable(iwdg);

    return status;
}

void LL_IWDG_StructInit(LL_IWDG_Init_T *iwdg_init)
{
    /* Set IWDG_InitStruct fields to default values */
    iwdg_init->timer_constant = LL_IWDG_TIMER_MAX;
    iwdg_init->ie             = LL_DISABLE;
    iwdg_init->reset_en       = LL_DISABLE;
}

#endif /* USE_FULL_LL_DRIVER */
