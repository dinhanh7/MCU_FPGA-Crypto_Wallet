/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#include "sn34f78x_ll_tpm.h"

#if defined(USE_FULL_LL_DRIVER)

/* Exported functions --------------------------------------------------------*/
/**
 * @brief  Initialize TPM registers according to the specified parameters in tpm_init.
 * @param  tpm TPM base Address
 * @param  tpm_init pointer to a @ref LL_TPM_Init_T structure
 *         that contains the configuration information for the specified TPM peripheral.
 * @retval An ErrorStatus enumeration value:
 *           - LL_OK:     TPM registers are initialized according to tpm_init content
 *           - LL_ERROR:  Not applicable
 */
LL_Status_T LL_TPM_Init(void *tpm, LL_TPM_Init_T *tpm_init)
{
    /* Check the parameters */
    RET_FLAG_FALSE(IS_LL_TPM_INSTANCE(tpm), LL_ERROR);
    RET_FLAG_FALSE(IS_LL_TPM_PRE(tpm_init->pre_scaler), LL_ERROR);
    RET_FLAG_FALSE(IS_LL_TPM_MODE(tpm_init->ctm), LL_ERROR);
    RET_FLAG_FALSE(IS_LL_TPM_COUNTING_MODE(tpm_init->cm), LL_ERROR);

    // Get tpm_index for function_table judgement
    uint32_t tpm_index = _get_tpm_index(tpm);

    switch ((uint32_t)tpm)
    {
        case SN_CT16B0_BASE:
        {
            /* Reset counting */
            LL_TPM0_Reset((SN_CT16B0_Type *)tpm);
            /* Counting mode selection */
            if (function_table[tpm_index] & LL_TPM_CM)
            {
                LL_TPM0_SetCountingMode((SN_CT16B0_Type *)tpm, tpm_init->cm);
            }

            /* Counting mode selection */
            if (function_table[tpm_index] & LL_TPM_CTM)
            {
                LL_TPM0_SetCounterTimeMode((SN_CT16B0_Type *)tpm, tpm_init->ctm);
                /* Enable capture when TPM CTM is not Timer */
                if (tpm_init->ctm != LL_TPM0_CTM_TIMER)
                {
                    LL_TPM0_Enable_CAP0EN((SN_CT16B0_Type *)tpm);
                }
            }
            /* Set prescaler */
            LL_TPM0_SetPrescaler((SN_CT16B0_Type *)tpm, tpm_init->pre_scaler);
            break;
        }

        case (uint32_t)SN_CT16B1_BASE:
        case SN_CT16B2_BASE:
        case SN_CT16B5_BASE:
        {
            /* Reset counting */
            LL_TPM125_Reset((SN_CT16B2_Type *)tpm);
            /* Counting mode selection */
            if (function_table[tpm_index] & LL_TPM_CM)
            {
                LL_TPM125_SetCountingMode((SN_CT16B2_Type *)tpm, tpm_init->cm);
            }

            /* Counting mode selection */
            if (function_table[tpm_index] & LL_TPM_CTM)
            {
                LL_TPM125_SetCounterTimeMode((SN_CT16B2_Type *)tpm, tpm_init->ctm);
                /* Enable capture when TPM CTM is not Timer */
                if (tpm_init->ctm != LL_TPM125_CTM_TIMER)
                {
                    LL_TPM125_Enable_CAP0EN((SN_CT16B2_Type *)tpm);
                }
            }
            /* Set prescaler */
            LL_TPM125_SetPrescaler((SN_CT16B2_Type *)tpm, tpm_init->pre_scaler);
            break;
        }

        case SN_CT16B3_BASE:
        {
            /* Reset counting */
            LL_TPM3_Reset(tpm);

            /* TPM3 not supports cm (counting mode) */

            /* TPM3 not supports ctm (counting Timer mode) */

            /* Set prescaler */
            LL_TPM3_SetPrescaler((SN_CT16B3_Type *)tpm, tpm_init->pre_scaler);

            break;
        }

        case SN_CT16B4_BASE:
        {
            /* Reset counting */
            LL_TPM4_Reset(tpm);

            /* TPM4 not supports cm */

            /* Counting Timer mode selection */
            if (function_table[tpm_index] & LL_TPM_CTM)
            {
                LL_TPM4_SetCounterTimeMode((SN_CT16B4_Type *)tpm, tpm_init->ctm);
                /* Enable capture when TPM CTM is not Timer */
                if (tpm_init->ctm != LL_TPM4_CTM_TIMER)
                {
                    LL_TPM4_Enable_CAP0EN((SN_CT16B4_Type *)tpm);
                }
            }
            /* Set prescaler */
            LL_TPM4_SetPrescaler((SN_CT16B4_Type *)tpm, tpm_init->pre_scaler);
            break;
        }

        case SN_CT16B6_BASE:
        case SN_CT16B7_BASE:
        {
            /* Reset counting */
            LL_TPM67_Reset(tpm);

            /* TPM6 and TPM7 not supports cm and ctm */

            /* Set prescaler */
            LL_TPM67_SetPrescaler((SN_CT16B6_Type *)tpm, tpm_init->pre_scaler);

            break;
        }

        case SN_CT16B8_BASE:
        {
            /* Reset counting */
            LL_TPM8_Reset(tpm);

            /* TPM8 not supports cm and ctm */

            /* Set prescaler */
            LL_TPM8_SetPrescaler((SN_CT16B8_Type *)tpm, tpm_init->pre_scaler);

            break;
        }

        default:
            return LL_ERROR;
    }

    return LL_OK;
}

/**
 * @brief  De-initialize TPM registers .
 * @param  tpm TPM base Address
 * @retval An ErrorStatus enumeration value:
 *           - LL_OK:     TPM registers are de-initialized
 *           - LL_ERROR:  de-initialized fail
 */
LL_Status_T LL_TPM_DeInit(void *tpm)
{
    /* Check the parameters */
    RET_FLAG_FALSE(IS_LL_TPM_INSTANCE(tpm), LL_ERROR);

    /* Disable TPM */
    switch ((uint32_t)tpm)
    {
        case SN_CT16B0_BASE:
            LL_TPM0_Disable((SN_CT16B0_Type *)tpm);
            // Reset GPIO related pin
            for (int i = 0; i <= LL_TPM_CHANNEL_3; i++)
            {
                LL_TPM0_SetPWMNnSelection(tpm, i, LL_TPM0_PWMN_SEL_GPIO);
            }
            break;

        case SN_CT16B1_BASE:
        case SN_CT16B2_BASE:
        case SN_CT16B5_BASE:
            LL_TPM125_Disable((SN_CT16B2_Type *)tpm);
            break;

        case SN_CT16B3_BASE:
            LL_TPM3_Disable((SN_CT16B3_Type *)tpm);
            // Reset GPIO related pin
            for (int i = 0; i <= LL_TPM_CHANNEL_1; i++)
            {
                LL_TPM0_SetPWMNnSelection(tpm, i, LL_TPM0_PWMN_SEL_GPIO);
            }
            break;

        case SN_CT16B4_BASE:
            LL_TPM4_Disable((SN_CT16B4_Type *)tpm);
            // Reset GPIO related pin
            for (int i = 0; i <= LL_TPM_CHANNEL_1; i++)
            {
                LL_TPM0_SetPWMNnSelection(tpm, i, LL_TPM0_PWMN_SEL_GPIO);
            }
            break;

        case SN_CT16B6_BASE:
        case SN_CT16B7_BASE:
            LL_TPM67_Disable((SN_CT16B6_Type *)tpm);
            break;

        case SN_CT16B8_BASE:
            LL_TPM8_Disable((SN_CT16B8_Type *)tpm);
            break;

        default:
            return LL_ERROR;
    }

    return LL_OK;
}

/**
 * @brief  Set each @ref LL_TPM_Init_T field to default value.
 * @param  tpm_init pointer to a @ref LL_TPM_Init_T structure  whose fields will be set to default values.
 * @retval None
 */
void LL_TPM_StructInit(void *tpm, LL_TPM_Init_T *tpm_init)
{
    if (tpm_init != NULL)
    {
        tpm_init->pre_scaler = 190;

        switch ((uint32_t)tpm)
        {
            case SN_CT16B0_BASE:
                tpm_init->ctm = LL_TPM0_CTM_TIMER;
                tpm_init->cm  = LL_TPM0_CM_COUNTING_UP;
                break;

            case SN_CT16B1_BASE:
            case SN_CT16B2_BASE:
            case SN_CT16B5_BASE:
                tpm_init->ctm = LL_TPM125_CTM_TIMER;
                tpm_init->cm  = LL_TPM125_CM_COUNTING_UP;
                break;

            case SN_CT16B4_BASE:
                tpm_init->ctm = LL_TPM4_CTM_TIMER;
                break;

            case SN_CT16B3_BASE:
            case SN_CT16B6_BASE:
            case SN_CT16B7_BASE:
            case SN_CT16B8_BASE:
            default:
                break;
        }
    }
}

// #if (configUSE_TPM_OC == 1)
/**
 * @brief  OC config, involve Timer\EMC\Forced
 * @param  tpm instance.
 * @param  config OC config structure
 * @param  channel TPM channel
 * @retval LL_Status_T
 * @note
 *      1.SN_CT16B6&SN_CT16B7 not support EMC and Forced
 *      2.If config->mode is Forced, the function can not used after TPM start
 */
LL_Status_T LL_TPM_OC_ConfigChannel(void *tpm, LL_TPM_Init_T *tpm_init, LL_TPM_OC_T *config, uint8_t channel)
{
    /* Check the parameters */
    RET_FLAG_FALSE(IS_LL_TPM_INSTANCE(tpm), LL_ERROR);
    RET_FLAG_FALSE(IS_LL_TPM_CHANNEL_COUNTING(config), LL_ERROR);
    RET_FLAG_FALSE(IS_LL_TPM_CHANNEL(channel), LL_ERROR);
    RET_FLAG_FALSE(IS_LL_OC_MODE(config->mode), LL_ERROR);

    uint32_t tpm_index = _get_tpm_index(tpm);

    /* Check instance support OC or Timer */
    if ((channel_table[tpm_index][channel] & (LL_CH_OC | LL_CH_TIMER)) == 0)
    {
        return LL_ERROR;
    }

    /* Check TPM CTM is not counter mode when channel mode is not TPM_OCMODE_TIMER */
    if ((function_table[tpm_index] & LL_TPM_CTM) && (config->mode != LL_TPM_OCMODE_TIMER))
    {
        if (tpm_init->ctm != 0)
            return LL_ERROR;
    }

    /* Check channel support output */
    if ((config->mode != LL_TPM_OCMODE_TIMER) && (!(channel_table[tpm_index][channel] & LL_CH_OC)))
    {
        return LL_ERROR;
    }

    switch (config->mode)
    {
        case LL_TPM_OCMODE_TIMER:
            /* Set channel base config */
            _ll_config_channel_base(tpm, config, channel);
            break;
        case LL_TPM_OCMODE_LOW:
        case LL_TPM_OCMODE_HIGH:
        case LL_TPM_OCMODE_TOGGLE:
        {
            /* Output pin by EMC */
            LL_REG_SBIT(*REG_PWM_ENABLE(tpm), 0 << channel);

            switch ((uint32_t)tpm)
            {
                case SN_CT16B0_BASE:
                    // LL_TPM0_Enable_PWMn(tpm, channel);
                    LL_TPM0_SetPWMnFunction(tpm, config->mode - LL_TPM_OCMODE_TIMER, channel);
                    if (config->idle_state == LL_ENABLE)
                        LL_TPM0_SetPWMnHigh(tpm, channel);
                    else
                        LL_TPM0_SetPWMnLow(tpm, channel);

                    break;

                case SN_CT16B1_BASE:
                case SN_CT16B2_BASE:
                case SN_CT16B5_BASE:
                    LL_TPM125_SetPWMnFunction(tpm, config->mode - LL_TPM_OCMODE_TIMER, channel);
                    if (config->idle_state == LL_ENABLE)
                        LL_TPM125_SetPWMnHigh(tpm, channel);
                    else
                        LL_TPM125_SetPWMnLow(tpm, channel);
                    break;

                case SN_CT16B3_BASE:
                    LL_TPM3_SetPWMnFunction(tpm, config->mode - LL_TPM_OCMODE_TIMER, channel);
                    if (config->idle_state == LL_ENABLE)
                        LL_TPM3_SetPWMnHigh(tpm, channel);
                    else
                        LL_TPM3_SetPWMnLow(tpm, channel);
                    break;

                case SN_CT16B4_BASE:
                    LL_TPM4_SetPWMnFunction(tpm, config->mode - LL_TPM_OCMODE_TIMER, channel);
                    if (config->idle_state == LL_ENABLE)
                        LL_TPM4_SetPWMnHigh(tpm, channel);
                    else
                        LL_TPM4_SetPWMnLow(tpm, channel);

                    break;
                case SN_CT16B6_BASE:
                case SN_CT16B7_BASE:
                    break;

                case SN_CT16B8_BASE:
                    LL_TPM8_SetPWMnFunction(tpm, config->mode - LL_TPM_OCMODE_TIMER, channel);
                    if (config->idle_state == LL_ENABLE)
                        LL_TPM8_SetPWMnHigh(tpm, channel);
                    else
                        LL_TPM8_SetPWMnLow(tpm, channel);
                default:
                    break;
            }
            /* Set channel base config */
            _ll_config_channel_base(tpm, config, channel);
            break;
        }
        case LL_TPM_OCMODE_FORCED_LOW:
        case LL_TPM_OCMODE_FORCED_HIGH:
            /* Set channel PWM config */
            _ll_config_channel_pwm(tpm, config, channel);
            break;
        default:
            break;
    }
    return LL_OK;
}

// #endif // configUSE_TPM_OC

// #if (configUSE_TPM_PWM == 1)
/**
 * @brief  PWM config
 * @param  tpm instance
 * @param  config PWM config structure
 * @param  channel TPM channel to configure
 * @retval LL_Status_T
 * @note SN_CT16B6&SN_CT16B7 not support PWM
 */
LL_Status_T LL_TPM_PWM_ConfigChannel(void *tpm, LL_TPM_Init_T *tpm_init, LL_TPM_OC_T *config, uint32_t channel)
{
    /* Check the parameters */
    RET_FLAG_FALSE(IS_LL_TPM_INSTANCE(tpm), LL_ERROR);
    RET_FLAG_FALSE(IS_LL_TPM_CHANNEL_COUNTING(config), LL_ERROR);
    RET_FLAG_FALSE(IS_LL_TPM_CHANNEL(channel), LL_ERROR);
    RET_FLAG_FALSE(IS_PWM_MODE(config->mode), LL_ERROR);

    uint32_t tpm_index = _get_tpm_index(tpm);

    /* Check instance support PWM */
    RET_FLAG_FALSE((channel_table[tpm_index][channel] & LL_CH_PWM), LL_ERROR);

    /* Not suport PWM mode 2 when center mode */
    RET_FLAG_TRUE(CHECK_PWM2_IN_CENTER_MODE(tpm_init->cm, config->mode), LL_ERROR);

    /* Check TPM CTM is not counter mode */
    if (function_table[tpm_index] & LL_TPM_CTM)
    {
        if (tpm_init->ctm != 0)
            return LL_ERROR;
    }

    /*  Set channel PWM config */
    _ll_config_channel_pwm(tpm, config, channel);

    return LL_OK;
}

/**
 * @brief  PWMN config
 * @param  tpm instance
 * @param  config PWMN config structure
 * @param  channel channel
 * @retval LL_Status_T
 * @note
 *      1.Only SN_CT16B0&SN_CT16B3&SN_CT16B4 support PWMN
 *      2.PWMN config can not be used after TPM start
 */
LL_Status_T LL_TPM_PWM_ConfigPWMN(void *tpm, LL_TPM_PWMN_T *config, uint32_t channel)
{
    /* Check the parameters */
    RET_FLAG_FALSE(IS_LL_TPM_INSTANCE(tpm), LL_ERROR);
    RET_FLAG_FALSE(IS_LL_TPM_PWMN_DB(config->db), LL_ERROR);
    RET_FLAG_FALSE(IS_LL_TPM_CHANNEL(channel), LL_ERROR);

    uint32_t tpm_index = _get_tpm_index(tpm);

    /* Check instance support PWMN */
    RET_FLAG_FALSE((channel_table[tpm_index][channel] & LL_CH_PWMN), LL_ERROR);

    switch ((uint32_t)tpm)
    {
        case SN_CT16B0_BASE:
            LL_TPM0_SetPWMNnSelection(tpm, config->mode, channel);
            LL_TPM0_SetDBPeriod(tpm, channel, config->db);
            break;

        case SN_CT16B3_BASE:
            LL_TPM3_SetPWMNnSelection(tpm, config->mode, channel);
            LL_TPM3_SetDBPeriod(tpm, channel, config->db);
            break;

        case SN_CT16B4_BASE:
            LL_TPM4_SetPWMNnSelection(tpm, config->mode, channel);
            LL_TPM4_SetDBPeriod(tpm, channel, config->db);
            break;

        case SN_CT16B1_BASE:
        case SN_CT16B2_BASE:
        case SN_CT16B5_BASE:
        case SN_CT16B6_BASE:
        case SN_CT16B7_BASE:
        case SN_CT16B8_BASE:
        default:
            break;
    }

    return LL_OK;
}

// #endif // configUSE_TPM_PWM

// #if (configUSE_TPM_CAP == 1)
/**
 * @brief  Capture config
 * @param  tpm instance
 * @param  config capture config structure
 * @retval LL_Status_T
 */
LL_Status_T LL_TPM_CAP_Config(void *tpm, LL_TPM_Init_T *tpm_init, LL_TPM_Capture_T *config)
{
    /* Check the parameters */
    RET_FLAG_FALSE(IS_LL_TPM_INSTANCE(tpm), LL_ERROR);
    RET_FLAG_FALSE(IS_LL_TPM_CAP(config), LL_ERROR);

    uint32_t tpm_index = _get_tpm_index(tpm);

    /* Check instance support Capture */
    RET_FLAG_FALSE((function_table[tpm_index] & LL_TPM_CAPTURE0), LL_ERROR);
    /* Check TPM CTM is not counter mode */
    RET_FLAG_FALSE(LL_FLAG_EQU(tpm_init->ctm, 0), LL_ERROR);

    switch ((uint32_t)tpm)
    {
        case SN_CT16B0_BASE:
            if (config->falling == LL_ENABLE)
                LL_TPM0_Enable_CAP0FE(tpm);
            else
                LL_TPM0_Disable_CAP0FE(tpm);

            if (config->rising == LL_ENABLE)
                LL_TPM0_Enable_CAP0RE(tpm);
            else
                LL_TPM0_Disable_CAP0RE(tpm);

            break;

        case SN_CT16B1_BASE:
        case SN_CT16B2_BASE:
        case SN_CT16B5_BASE:
            if (config->falling == LL_ENABLE)
                LL_TPM125_Enable_CAP0FE(tpm);
            else
                LL_TPM125_Disable_CAP0FE(tpm);

            if (config->rising == LL_ENABLE)
                LL_TPM125_Enable_CAP0RE(tpm);
            else
                LL_TPM125_Disable_CAP0RE(tpm);
            break;

        case SN_CT16B4_BASE:
            if (config->falling == LL_ENABLE)
                LL_TPM4_Enable_CAP0FE(tpm);
            else
                LL_TPM4_Disable_CAP0FE(tpm);

            if (config->rising == LL_ENABLE)
                LL_TPM4_Enable_CAP0RE(tpm);
            else
                LL_TPM4_Disable_CAP0RE(tpm);
            break;

        case SN_CT16B3_BASE:
        case SN_CT16B6_BASE:
        case SN_CT16B7_BASE:
        case SN_CT16B8_BASE:
        default:
            break;
    }

    return LL_OK;
}
// #endif // configUSE_TPM_CAP

/**
 * @brief  PWM break function config
 * @param  htpm Pointer to a TPM_Handle_T structure that contains the configuration information for the specified TPM.
 * @param  config PWM breakfunction config structural
 * @retval LL_Status_T
 * @note
 *      1.Only SN_CT16B0 support break function
 */
LL_Status_T LL_TPM_ConfigBreak(void *tpm, LL_TPM_Break_T *config)
{
    /* Check the parameters */
    RET_FLAG_FALSE(IS_LL_TPM_INSTANCE(tpm), LL_ERROR);
    RET_FLAG_FALSE(IS_LL_TPM_BREAK_COUNTING(config), LL_ERROR);

    uint32_t tpm_index = _get_tpm_index(tpm);

    /* Check instance support break funtion */
    RET_FLAG_FALSE((function_table[tpm_index] & LL_TPM_BREAK), LL_ERROR);

    /* set break function config */
    switch ((uint32_t)tpm)
    {
        case SN_CT16B0_BASE:
            LL_TPM0_SetBRKSelection(tpm, config->break_mode & 0xF);
            if (((config->break_mode & 0x10) >> 4) == LL_ENABLE)
                LL_TPM0_SetBRKTriggerLevelHigh(tpm);
            else
                LL_TPM0_SetBRKTriggerLevelLow(tpm);

            LL_TPM0_SetBRKDebounceTime(tpm, config->break_db);

            if (config->break_ie == LL_ENABLE)
                LL_TPM0_EnableIT_BRKIE(tpm);
            else
                LL_TPM0_DisableIT_BRKIE(tpm);
            break;

        case SN_CT16B1_BASE:
        case SN_CT16B2_BASE:
        case SN_CT16B3_BASE:
        case SN_CT16B4_BASE:
        case SN_CT16B5_BASE:
        case SN_CT16B6_BASE:
        case SN_CT16B7_BASE:
        case SN_CT16B8_BASE:
        default:
            break;
    }

    return LL_OK;
}

/**
 * @brief  PWM load mode function config
 * @param  tpm instance.
 * @param  config PWM load mode function config structural
 * @retval LL_Status_T
 * @note
 *      1.Only SN_CT16B0/SN_CT16B1/SN_CT16B2/SN_CT16B5 support this function
 */
LL_Status_T LL_TPM_ConfigLoadMode(void *tpm, LL_TPM_LoadMode_T *config)
{
    RET_FLAG_FALSE(IS_LL_TPM_INSTANCE(tpm), LL_ERROR);

    uint32_t tpm_index = _get_tpm_index(tpm);

    /* Check instance support load mode funtion */
    RET_FLAG_FALSE((channel_table[tpm_index][config->channel] & LL_CH_LOAD), LL_ERROR);

    /* Set load mode */
    switch ((uint32_t)tpm)
    {
        case SN_CT16B0_BASE:
            LL_TPM0_SetMRnLoadMode(tpm, config->load_mode, config->channel);
            break;
        case SN_CT16B1_BASE:
        case SN_CT16B2_BASE:
        case SN_CT16B5_BASE:
            LL_TPM125_SetMRnLoadMode(tpm, config->load_mode, config->channel);
            break;

        default:
            return LL_ERROR;
    }
    return LL_OK;
}

/**
 * @brief  Set channel base config
 * @param  tpm Pointer to a  structure that contains the configuration information for the specified TPM.
 * @param  config OC config structural
 * @param  channel TPM channel
 * @retval None
 */
static void _ll_config_channel_base(void *tpm, LL_TPM_OC_T *config, uint32_t channel)
{
    uint32_t tpm_index = _get_tpm_index(tpm);

    switch ((uint32_t)tpm)
    {
        case SN_CT16B0_BASE:
            LL_TPM0_SetTCMatchValueToMRn(tpm, config->period, channel);
            if (config->reset == LL_ENABLE)
                LL_TPM0_Enable_MRnRST(tpm, channel);
            else
                LL_TPM0_Disable_MRnRST(tpm, channel);

            if (config->stop == LL_ENABLE)
                LL_TPM0_Enable_MRnSTOP(tpm, channel);
            else
                LL_TPM0_Disable_MRnSTOP(tpm, channel);

            break;

        case SN_CT16B1_BASE:
        case SN_CT16B2_BASE:
        case SN_CT16B5_BASE:
            LL_TPM125_SetTCMatchValueToMRn(tpm, config->period, channel);
            if (config->reset == LL_ENABLE)
                LL_TPM125_Enable_MRnRST(tpm, channel);
            else
                LL_TPM125_Disable_MRnRST(tpm, channel);

            if (config->stop == LL_ENABLE)
                LL_TPM125_Enable_MRnSTOP(tpm, channel);
            else
                LL_TPM125_Disable_MRnSTOP(tpm, channel);
            break;

        case SN_CT16B3_BASE:
            LL_TPM3_SetTCMatchValueToMRn(tpm, config->period, channel);
            if (config->reset == LL_ENABLE)
                LL_TPM3_Enable_MRnRST(tpm, channel);
            else
                LL_TPM3_Disable_MRnRST(tpm, channel);

            if (config->stop == LL_ENABLE)
                LL_TPM3_Enable_MRnSTOP(tpm, channel);
            else
                LL_TPM3_Disable_MRnSTOP(tpm, channel);
            break;

        case SN_CT16B4_BASE:
            LL_TPM4_SetTCMatchValueToMRn(tpm, config->period, channel);
            if (config->reset == LL_ENABLE)
                LL_TPM4_Enable_MRnRST(tpm, channel);
            else
                LL_TPM4_Disable_MRnRST(tpm, channel);

            if (config->stop == LL_ENABLE)
                LL_TPM4_Enable_MRnSTOP(tpm, channel);
            else
                LL_TPM4_Disable_MRnSTOP(tpm, channel);
            break;

        case SN_CT16B6_BASE:
        case SN_CT16B7_BASE:
            LL_TPM67_SetTCMatchValueToMR0(tpm, config->period);
            if (config->reset == LL_ENABLE)
                LL_TPM67_Enable_MR0RST(tpm);
            else
                LL_TPM67_Disable_MR0RST(tpm);

            if (config->stop == LL_ENABLE)
                LL_TPM67_Enable_MR0STOP(tpm);
            else
                LL_TPM67_Disable_MR0STOP(tpm);
            break;

        case SN_CT16B8_BASE:
            LL_TPM8_SetTCMatchValueToMRn(tpm, config->period, channel);
            if (config->reset == LL_ENABLE)
                LL_TPM8_Enable_MRnRST(tpm, channel);
            else
                LL_TPM8_Disable_MRnRST(tpm, channel);

            if (config->stop == LL_ENABLE)
                LL_TPM8_Enable_MRnSTOP(tpm, channel);
            else
                LL_TPM8_Disable_MRnSTOP(tpm, channel);
            break;

        default:
            break;
    }
}

// #if ((configUSE_TPM_PWM == 1) || (configUSE_TPM_OC == 1))
/**
 * @brief  Set channel PWM config
 * @param  tpm Pointer to a TPM_Handle_T structure that contains the configuration information for the specified TPM.
 * @param  config OC config structural
 * @param  channel TPM channel
 * @retval None
 */
static void _ll_config_channel_pwm(void *tpm, LL_TPM_OC_T *config, uint32_t channel)
{
    switch ((uint32_t)tpm)
    {
        case SN_CT16B0_BASE:
            LL_TPM0_Enable_PWMn(tpm, channel);
            LL_TPM0_SetPWMnMode(tpm, config->mode - LL_TPM_OCMODE_PWM1, channel);
            break;

        case SN_CT16B1_BASE:
        case SN_CT16B2_BASE:
        case SN_CT16B5_BASE:
            LL_TPM125_Enable_PWMn(tpm, channel);
            LL_TPM125_SetPWMnMode(tpm, config->mode - LL_TPM_OCMODE_PWM1, channel);
            break;

        case SN_CT16B3_BASE:
            LL_TPM3_Enable_PWMn(tpm, channel);
            LL_TPM3_SetPWMnMode(tpm, config->mode - LL_TPM_OCMODE_PWM1, channel);
            break;

        case SN_CT16B4_BASE:
            LL_TPM4_Enable_PWMn(tpm, channel);
            LL_TPM4_SetPWMnMode(tpm, config->mode - LL_TPM_OCMODE_PWM1, channel);
            break;

        case SN_CT16B6_BASE:
        case SN_CT16B7_BASE:

            break;

        case SN_CT16B8_BASE:
            LL_TPM8_Enable_PWMn(tpm, channel);
            LL_TPM8_SetPWMnMode(tpm, config->mode - LL_TPM_OCMODE_PWM1, channel);
            break;

        default:
            break;
    }

    /* Set channel base config */
    _ll_config_channel_base(tpm, config, channel);
}

// #endif

#endif /* USE_FULL_LL_DRIVER */
