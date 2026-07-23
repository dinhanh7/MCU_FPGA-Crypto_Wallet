/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#include "sn34f78x_hal_rtc.h"

#if (configUSE_RTC_PERIPHERAL == 1)

/**
 * @brief  Initializes the RTC peripheral
 * @param  hrtc Pointer to a RTC_Handle_T structure that contains the configuration information for RTC.
 * @retval HAL status
 */
HAL_Status_T HAL_RTC_Init(RTC_Handle_T *hrtc)
{
    uint32_t tickstart = 0U;
    /* Check the UART handle allocation */
    ASSERT_PARAM(hrtc);

    /* Check the parameters */
    ASSERT_PARAM(IS_RTC_CLK_SOURCE(hrtc->init.clk_source));

    /* Protect the process */
    TAKE_MUTEX(hrtc);

    if (hrtc->state == HAL_STATE_RESET)
    {
        hrtc->is_cali_flag = FALSE;
#if (configUSE_RTC_REGISTER_CALLBACKS == 1)
        /* Legacy weak AlarmEventCallback  */
        hrtc->AlarmEventCallback = HAL_RTC_AlarmEventCallback;
        /* Legacy weak SecondEventCallback  */
        hrtc->SecondEventCallback = HAL_RTC_SecondEventCallback;

        if (hrtc->MspInitCallback == NULL)
        {
            hrtc->MspInitCallback = HAL_RTC_MspInit;
        }

        /* Init the low level hardware */
        hrtc->MspInitCallback(hrtc);
#else
        /* Initialize RTC MSP */
        HAL_RTC_MspInit(hrtc);
#endif /* (configUSE_RTC_REGISTER_CALLBACKS) */
    }
    else
    {
        return HAL_ERROR;
    }

    /* In init, if RTC is already enabled, directly change state to ready (wake up from DPD multiple times but keep RTC state). */
    if (HAL_REG_READ(SN_ALWAYSON->POR_MISC_b.RTCCLKEN) == 0x1)
    {
        /* Set RTC state */
        hrtc->state = HAL_STATE_READY;

        /* Process Unlocked */
        GIVE_MUTEX(hrtc);

        return HAL_OK;
    }

    /* Set RTC state */
    hrtc->state = HAL_STATE_BUSY;

    /* 1、Configure RTC Clock ： ILRC(32KHZ) or ELS(32.768KHZ) */
    /* Adjust(calibration) RTC clock frequency:             */
    if (hrtc->init.clk_source == RTC_CLK_SOURCE_ILRC)
    {
        /* RTC Counter Clock Source Select */
        HAL_REG_WRITE(SN_ALWAYSON->POR_MISC_b.RTCCLKSEL, 1);
    }
    else
    {
        /* RTC Counter Clock Source Select */
        HAL_REG_WRITE(SN_ALWAYSON->POR_MISC_b.ELSEN, 1);

        /* check ELS ready flag */
        while (!HAL_REG_READ(SN_ALWAYSON->POR_MISC_b.ELSRDY))
            ;
        HAL_REG_WRITE(SN_ALWAYSON->POR_MISC_b.RTCCLKSEL, 0);

        /* Compensates for frequency drift caused by crystal error of external clock source */
        HAL_RTCEx_Calibration(hrtc, RTC_MEASURE_DATA);
    }

    /* 2、Enable RTC clock  register */
    HAL_REG_WRITE(SN_ALWAYSON->POR_MISC_b.RTCCLKEN, 1);

    while (HAL_REG_READ(SN_SCU->RTC_CTRL_b.RTC_REGRDY) != 1U)
    {
        /* Check timeout */
        BREAK_PROC_TIMEOUT(hrtc, tickstart, RTC_TIMEOUT_VALUE);
    }
    RET_FLAG_TRUE((hrtc->error_code == HAL_ERROR_TIMEOUT), HAL_ERROR);

    /* 3、disable RTC */
    HAL_REG_WRITE(SN_SCU->RTC_CTRL_b.RTC_EN, DISABLE);

    /* Get tick */
    tickstart = HAL_GetTick();

    /* wait until the RTC_STS bit is changed to rtc_sts and the setting of the RTC alarm is complete. */
    while (HAL_REG_READ(SN_SCU->RTC_CTRL_b.RTCEN_STS) != DISABLE)
    {
        /* Check timeout */
        BREAK_PROC_TIMEOUT(hrtc, tickstart, RTC_TIMEOUT_VALUE);
    }
    RET_FLAG_TRUE((hrtc->error_code == HAL_ERROR_TIMEOUT), HAL_ERROR);

    /* clear RTC_TIME and RTC_TIME */
    HAL_REG_CLEAR(SN_SCU->RTC_TIME1);
    HAL_REG_WRITE(SN_SCU->RTC_TIME2, RTC_TIME2_INIT_VAL);
    HAL_REG_WRITE(SN_SCU->RTC_ALM1, RTC_ALM1_INIT_VAL);
    HAL_REG_CLEAR(SN_SCU->RTC_ALM2);

    /* 4、Enable RTC */
    HAL_REG_WRITE(SN_SCU->RTC_CTRL_b.RTC_EN, ENABLE);

    /* Get tick */
    tickstart = HAL_GetTick();

    /* wait until the RTC_STS bit is changed to rtc_sts and the setting of the RTC alarm is complete. */
    while (HAL_REG_READ(SN_SCU->RTC_CTRL_b.RTCEN_STS) != ENABLE)
    {
        /* Check timeout */
        BREAK_PROC_TIMEOUT(hrtc, tickstart, RTC_TIMEOUT_VALUE);
    }
    RET_FLAG_TRUE((hrtc->error_code == HAL_ERROR_TIMEOUT), HAL_ERROR);

    /* Set RTC state */
    hrtc->state = HAL_STATE_READY;

    /* Process Unlocked */
    GIVE_MUTEX(hrtc);

    /* The current time is unknown. */

    return HAL_OK;
}

/**
 * @brief  DeInitializes the RTC peripheral
 * @param  hrtc Pointer to a RTC_Handle_T structure that contains the configuration information for RTC.
 * @note   This function does not reset the RTC Backup Data registers.
 * @retval HAL status
 */
HAL_Status_T HAL_RTC_DeInit(RTC_Handle_T *hrtc)
{
    uint32_t tickstart = 0U;

    /* Check the RTC peripheral state */
    ASSERT_PARAM(hrtc);

    hrtc->state = HAL_STATE_BUSY;

    /* disable SECOND */
    HAL_REG_WRITE(SN_SCU->RTC_CTRL_b.SECOUT_EN, DISABLE);

    /* disable ALARM */
    HAL_REG_WRITE(SN_SCU->RTC_CTRL_b.RTC_ALARM_EN, DISABLE);
    /* Get tick */
    tickstart = HAL_GetTick();
    /* wait until the RTC_STS bit is changed to rtc_sts or the setting of the RTC alarm is complete. */
    while (HAL_REG_READ(SN_SCU->RTC_CTRL_b.RTC_ALMEN_STS) != DISABLE)
    {
        /* Check timeout */
        BREAK_PROC_TIMEOUT(hrtc, tickstart, RTC_TIMEOUT_VALUE);
    }
    RET_FLAG_TRUE((hrtc->error_code == HAL_ERROR_TIMEOUT), HAL_ERROR);

    /* disable RTC */
    HAL_REG_WRITE(SN_SCU->RTC_CTRL_b.RTC_EN, DISABLE);
    /* Get tick */
    tickstart = HAL_GetTick();
    /* wait until the RTC_STS bit is changed to rtc_sts or the setting of the RTC alarm is complete. */
    while (HAL_REG_READ(SN_SCU->RTC_CTRL_b.RTCEN_STS) != DISABLE)
    {
        /* Check timeout */
        BREAK_PROC_TIMEOUT(hrtc, tickstart, RTC_TIMEOUT_VALUE);
    }
    RET_FLAG_TRUE((hrtc->error_code == HAL_ERROR_TIMEOUT), HAL_ERROR);

    /* 2024/1/4 Confirm with TW colleagues that the clock source should be reset to ILRC when deinit */
    HAL_REG_WRITE(SN_ALWAYSON->POR_MISC_b.RTCCLKSEL, 1);

    /* disable RTCCLK */
    HAL_REG_WRITE(SN_ALWAYSON->POR_MISC_b.RTCCLKEN, DISABLE);

#if (configUSE_RTC_REGISTER_CALLBACKS == 1)
    if (hrtc->MspDeInitCallback == NULL)
    {
        hrtc->MspDeInitCallback = HAL_RTC_MspDeInit;
    }

    /* DeInit the low level hardware: CLOCK, NVIC.*/
    hrtc->MspDeInitCallback(hrtc);
#else
    /* De-Initialize RTC MSP */
    HAL_RTC_MspDeInit(hrtc);
#endif /* (configUSE_RTC_REGISTER_CALLBACKS) */

    /* Set RTC state */
    hrtc->state = HAL_STATE_RESET;

    /* Process Unlocked */
    GIVE_MUTEX(hrtc);

    return HAL_OK;
}

/**
 * @brief  Initializes the RTC MSP.
 * @param  hrtc Pointer to a RTC_Handle_T structure that contains the configuration information for RTC.
 * @retval None
 */
__weak void HAL_RTC_MspInit(RTC_Handle_T *hrtc)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hrtc);

    /* NOTE: This function should not be modified, when the callback is needed,
             the HAL_RTC_MspInit could be implemented in the user file
     */
}

/**
 * @brief  DeInitializes the RTC MSP.
 * @param  hrtc Pointer to a RTC_Handle_T structure that contains the configuration information for RTC.
 * @retval None
 */
__weak void HAL_RTC_MspDeInit(RTC_Handle_T *hrtc)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hrtc);

    /* NOTE: This function should not be modified, when the callback is needed,
             the HAL_RTC_MspDeInit could be implemented in the user file
     */
}

/* Callbacks Register/UnRegister functions  ***********************************/
#if (configUSE_RTC_REGISTER_CALLBACKS == 1)

/**
 * @brief  Register a User RTC Callback
 *         To be used instead of the weak predefined callback
 * @param  hrtc Pointer to a RTC_Handle_T structure that contains the configuration information for RTC.
 * @param  callback_id ID of the callback to be registered
 *         This parameter can be one of the following values:
 *          @arg @ref HAL_RTC_ALARM_EVENT_CB_ID            Alarm Event Callback ID
 *          @arg @ref HAL_RTC_MSPINIT_CB_ID                Msp Init callback ID
 *          @arg @ref HAL_RTC_MSPDEINIT_CB_ID              Msp DeInit callback ID
 * @param  callback pointer to the Callback function
 * @retval HAL_Status_T
 */
HAL_Status_T HAL_RTC_RegisterCallback(RTC_Handle_T *hrtc, HAL_RTC_CallbackID_T callback_id, RTC_Callback_T callback)
{
    /* Check the RTC peripheral state */
    ASSERT_PARAM(hrtc);

    HAL_Status_T status = HAL_OK;
    RET_FLAG_FALSE(callback, HAL_ERROR);

    if (HAL_STATE_READY == hrtc->state)
    {
        switch (callback_id)
        {
            case HAL_RTC_ALARM_EVENT_CB_ID:
                hrtc->AlarmEventCallback = callback;
                break;

            case HAL_RTC_SECOND_EVENT_CB_ID:
                hrtc->SecondEventCallback = callback;
                break;

            case HAL_RTC_MSPINIT_CB_ID:
                hrtc->MspInitCallback = callback;
                break;

            case HAL_RTC_MSPDEINIT_CB_ID:
                hrtc->MspDeInitCallback = callback;
                break;

            default:
                /* Return error status */
                status = HAL_ERROR;
                break;
        }
    }
    else if (HAL_STATE_RESET == hrtc->state)
    {
        switch (callback_id)
        {
            case HAL_RTC_MSPINIT_CB_ID:
                hrtc->MspInitCallback = callback;
                break;

            case HAL_RTC_MSPDEINIT_CB_ID:
                hrtc->MspDeInitCallback = callback;
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
 * @brief  Unregister an RTC Callback
 *         RTC callabck is redirected to the weak predefined callback
 * @param  hrtc Pointer to a RTC_Handle_T structure that contains the configuration information for RTC.
 * @param  callback_id ID of the callback to be unregistered
 *         This parameter can be one of the following values:
 *          @arg @ref HAL_RTC_ALARM_EVENT_CB_ID Alarm Event Callback ID
 *          @arg @ref HAL_RTC_MSPINIT_CB_ID Msp Init callback ID
 *          @arg @ref HAL_RTC_MSPDEINIT_CB_ID Msp DeInit callback ID
 * @retval HAL_Status_T
 */
HAL_Status_T HAL_RTC_UnRegisterCallback(RTC_Handle_T *hrtc, HAL_RTC_CallbackID_T callback_id)
{
    HAL_Status_T status = HAL_OK;
    if (HAL_STATE_READY == hrtc->state)
    {
        switch (callback_id)
        {
            case HAL_RTC_ALARM_EVENT_CB_ID:
                hrtc->AlarmEventCallback = HAL_RTC_AlarmEventCallback;
                break;

            case HAL_RTC_SECOND_EVENT_CB_ID:
                hrtc->SecondEventCallback = HAL_RTC_SecondEventCallback;
                break;

            case HAL_RTC_MSPINIT_CB_ID:
                hrtc->MspInitCallback = HAL_RTC_MspInit;
                break;

            case HAL_RTC_MSPDEINIT_CB_ID:
                hrtc->MspDeInitCallback = HAL_RTC_MspDeInit;
                break;

            default:
                /* Return error status */
                status = HAL_ERROR;
                break;
        }
    }
    else if (HAL_STATE_RESET == hrtc->state)
    {
        switch (callback_id)
        {
            case HAL_RTC_MSPINIT_CB_ID:
                hrtc->MspInitCallback = HAL_RTC_MspInit;
                break;

            case HAL_RTC_MSPDEINIT_CB_ID:
                hrtc->MspDeInitCallback = HAL_RTC_MspDeInit;
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
#endif /* configUSE_RTC_REGISTER_CALLBACKS */

/*
================================================================================
            ##### Peripheral Control functions #####
================================================================================
*/

/**
 * @brief  Sets RTC current time.
 * @param  hrtc Pointer to a RTC_Handle_T structure that contains the configuration information for RTC.
 * @param  stime Pointer to Time structure
 * @param  format Specifies the format of the entered parameters.
 *          This parameter can be one of the following values:
 *            @arg RTC_FORMAT_BIN: Binary data format
 *            @arg RTC_FORMAT_BCD: BCD data format
 * @retval HAL status
 */
HAL_Status_T HAL_RTC_SetTime(RTC_Handle_T *hrtc, RTC_Time_T *stime, uint32_t format)
{
    /* Check the RTC peripheral state */
    ASSERT_PARAM(hrtc);
    /* Check the parameters          */
    ASSERT_PARAM(IS_RTC_FORMAT(format));

    if (format == RTC_FORMAT_BIN)
    {
        /* Check Hours,Minutes,Seconds Parameter */
        ASSERT_PARAM(IS_RTC_HOUR24(stime->hours));
        ASSERT_PARAM(IS_RTC_MINUTES(stime->minutes));
        ASSERT_PARAM(IS_RTC_SECONDS(stime->seconds));
    }
    else // format == RTC_FORMAT_BCD
    {
        /* Check Hours,Minutes,Seconds Parameter for RTC_FORMAT_BCD */
        /* eg: 21: 05:36 (Be equivalent to) 0x21: 0x05: 0x36 */
        ASSERT_PARAM(IS_RTC_HOUR24(_rtc_bcd_to_byte(stime->hours)));
        ASSERT_PARAM(IS_RTC_MINUTES(_rtc_bcd_to_byte(stime->minutes)));
        ASSERT_PARAM(IS_RTC_SECONDS(_rtc_bcd_to_byte(stime->seconds)));
    }

    HAL_Status_T status = HAL_OK;

    RET_STATE_NOT_READY(hrtc);

    /* Protect the process */
    TAKE_MUTEX(hrtc);
    hrtc->state = HAL_STATE_BUSY;

    /* Check and read the RTCEN_STS bit of the RTC control register to confirm if RTC is disabled. */
    /* Init tick_start for timeout management */
    status = _rtc_en_sts_check(hrtc, RTCEN_STS_DISABLE);

    if (status == HAL_OK)
    {
        /* 3. Set new time to registers Time1 and Time2 */
        if (format == RTC_FORMAT_BIN)
        {
            /* To prevent data overflow, reset data after setting 0 */
            HAL_REG_CBIT(SN_SCU->RTC_TIME1, KEEP_WEEKDAY_MASK);
            HAL_REG_WRITE(SN_SCU->RTC_TIME1_b.HOUR, stime->hours);
            HAL_REG_WRITE(SN_SCU->RTC_TIME1_b.MIN, stime->minutes);
            HAL_REG_WRITE(SN_SCU->RTC_TIME1_b.SEC, stime->seconds);
        }
        else // format == RTC_FORMAT_BCD
        {
            /* To prevent data overflow, reset data after setting 0 */
            HAL_REG_CBIT(SN_SCU->RTC_TIME1, KEEP_WEEKDAY_MASK);
            HAL_REG_WRITE(SN_SCU->BCD_RTC_TIME1_b.HOUR, (stime->hours & UNIT_BIT_MASK));
            HAL_REG_WRITE(SN_SCU->BCD_RTC_TIME1_b.HOUR_DEC, (stime->hours & DEC_BIT_MASK) >> RTC_TIME_DEC_Pos);
            HAL_REG_WRITE(SN_SCU->BCD_RTC_TIME1_b.MIN, (stime->minutes & UNIT_BIT_MASK));
            HAL_REG_WRITE(SN_SCU->BCD_RTC_TIME1_b.MIN_DEC, (stime->minutes & DEC_BIT_MASK) >> RTC_TIME_DEC_Pos);
            HAL_REG_WRITE(SN_SCU->BCD_RTC_TIME1_b.SEC, (stime->seconds & UNIT_BIT_MASK));
            HAL_REG_WRITE(SN_SCU->BCD_RTC_TIME1_b.SEC_DEC, (stime->seconds & DEC_BIT_MASK) >> RTC_TIME_DEC_Pos);
        }

        status = _rtc_en_sts_check(hrtc, RTCEN_STS_ENABLE);
    }
    if (status == HAL_OK)
    {
        hrtc->state = HAL_STATE_READY;
    }

    /* Release the process protection */
    GIVE_MUTEX(hrtc);

    return status;
}

/**
 * @brief  Gets RTC current time.
 * @param  hrtc Pointer to a RTC_Handle_T structure that contains the configuration information for RTC.
 * @param  time Pointer to Time structure
 * @param  format Specifies the format of the entered parameters.
 *          This parameter can be one of the following values:
 *            @arg RTC_FORMAT_BIN: Binary data format
 *            @arg RTC_FORMAT_BCD: BCD data format
 * @note  You must call HAL_RTC_GetDate() after HAL_RTC_GetTime() to unlock the
 *        values in the higher-order calendar shadow registers to ensure
 *        consistency between the time and date values.
 *        Reading RTC current time locks the values in calendar shadow registers
 *        until current date is read to ensure consistency between the time and
 *        date values.
 * @retval HAL status
 */
HAL_Status_T HAL_RTC_GetTime(RTC_Handle_T *hrtc, RTC_Time_T *time, uint32_t format)
{
    /* Check the RTC peripheral state */
    ASSERT_PARAM(hrtc);

    /* Check the parameters */
    ASSERT_PARAM(IS_RTC_FORMAT(format));

    /*  Two consecutive read current time */
    uint32_t cmp       = 0;
    uint8_t  tmp_hours = 99, tmp_minutes = 99, tmp_seconds = 99;

    /* Use Method 2: Read the same value twice */
    while (cmp == 0)
    {
        if (format == RTC_FORMAT_BIN)
        {
            time->hours   = HAL_REG_READ(SN_SCU->RTC_TIME1_b.HOUR);
            time->minutes = HAL_REG_READ(SN_SCU->RTC_TIME1_b.MIN);
            time->seconds = HAL_REG_READ(SN_SCU->RTC_TIME1_b.SEC);
        }
        else // format == RTC_FORMAT_BCD
        {
            time->hours   = HAL_REG_READ(SN_SCU->BCD_RTC_TIME1_b.HOUR) | (HAL_REG_READ(SN_SCU->BCD_RTC_TIME1_b.HOUR_DEC) << RTC_TIME_DEC_Pos);
            time->minutes = HAL_REG_READ(SN_SCU->BCD_RTC_TIME1_b.MIN) | (HAL_REG_READ(SN_SCU->BCD_RTC_TIME1_b.MIN_DEC) << RTC_TIME_DEC_Pos);
            time->seconds = HAL_REG_READ(SN_SCU->BCD_RTC_TIME1_b.SEC) | (HAL_REG_READ(SN_SCU->BCD_RTC_TIME1_b.SEC_DEC) << RTC_TIME_DEC_Pos);
        }

        if ((time->hours == tmp_hours) && (time->minutes == tmp_minutes) && (time->seconds == tmp_seconds))
        {
            cmp = 1;
        }
        tmp_hours   = time->hours;
        tmp_minutes = time->minutes;
        tmp_seconds = time->seconds;
    }

    return HAL_OK;
}

/**
 * @brief  Sets RTC current date.
 * @param  hrtc Pointer to a RTC_Handle_T structure that contains the configuration information for RTC.
 * @param  sdate Pointer to date structure
 * @param  format specifies the format of the entered parameters.
 *          This parameter can be one of the following values:
 *            @arg RTC_FORMAT_BIN: Binary data format
 *            @arg RTC_FORMAT_BCD: BCD data format
 * @retval HAL status
 */
HAL_Status_T HAL_RTC_SetDate(RTC_Handle_T *hrtc, RTC_Date_T *sdate, uint32_t format)
{
    /* Check the RTC peripheral state */
    ASSERT_PARAM(hrtc);

    /* Check the parameters */
    ASSERT_PARAM(IS_RTC_FORMAT(format));
    ASSERT_PARAM(IS_RTC_WEEKDAY(sdate->week_day));

    /* BIN, BCD compatible processing about months */
    if ((RTC_FORMAT_BIN == format) && (((sdate->month) & 0x10) == 0x10))
    {
        sdate->month = (sdate->month & 0xf) + 0xa;
    }

    if (format == RTC_FORMAT_BIN)
    {
        /* Check Hours,Minutes,Seconds Parameter */
        /*eg:  2023-3-5 */
        ASSERT_PARAM(IS_RTC_CENTURY(sdate->century));
        ASSERT_PARAM(IS_RTC_YEAR(sdate->year));
        ASSERT_PARAM(IS_RTC_MONTH(sdate->month));
        ASSERT_PARAM(IS_RTC_DATE(sdate->date));
    }
    else // format == RTC_FORMAT_BCD
    {
        /* eg: 0x2023 -- 0x12 -- 0x25 */
        ASSERT_PARAM(IS_RTC_CENTURY(_rtc_bcd_to_byte(sdate->century)));
        ASSERT_PARAM(IS_RTC_YEAR(_rtc_bcd_to_byte(sdate->year)));
        ASSERT_PARAM(IS_RTC_MONTH(_rtc_bcd_to_byte(sdate->month)));
        ASSERT_PARAM(IS_RTC_DATE(_rtc_bcd_to_byte(sdate->date)));
    }

    HAL_Status_T status = HAL_OK;

    RET_STATE_NOT_READY(hrtc);

    /* Protect the process */
    TAKE_MUTEX(hrtc);
    hrtc->state = HAL_STATE_BUSY;

    /* 1. Check and read the RTCEN_STS bit of the RTC control register to confirm if RTC is disabled. */
    /* Init tick_start for timeout management */
    status = _rtc_en_sts_check(hrtc, RTCEN_STS_DISABLE);

    if (status == HAL_OK)
    {
        /* 2. Set new time to registers Time1 and Time2 */
        if (format == RTC_FORMAT_BIN)
        {
            /* To prevent data overflow, reset data after setting 0 */
            HAL_REG_CLEAR(SN_SCU->RTC_TIME2);
            HAL_REG_CLEAR(SN_SCU->RTC_TIME1_b.WEEKDAY);
            HAL_REG_WRITE(SN_SCU->RTC_TIME2_b.CENTURY, sdate->century);
            HAL_REG_WRITE(SN_SCU->RTC_TIME2_b.YEAR, sdate->year);
            HAL_REG_WRITE(SN_SCU->RTC_TIME2_b.MONTH, sdate->month);
            HAL_REG_WRITE(SN_SCU->RTC_TIME2_b.DATE, sdate->date);
            HAL_REG_WRITE(SN_SCU->RTC_TIME1_b.WEEKDAY, sdate->week_day);
        }
        else // format == RTC_FORMAT_BCD
        {
            /* To prevent data overflow, reset data after setting 0 */
            HAL_REG_CLEAR(SN_SCU->BCD_RTC_TIME2);
            HAL_REG_CLEAR(SN_SCU->BCD_RTC_TIME1_b.WEEKDAY);
            HAL_REG_WRITE(SN_SCU->BCD_RTC_TIME2_b.CENTURY, (sdate->century & UNIT_BIT_MASK));
            HAL_REG_WRITE(SN_SCU->BCD_RTC_TIME2_b.CENTURY_DEC, (sdate->century & DEC_BIT_MASK) >> RTC_TIME_DEC_Pos);
            HAL_REG_WRITE(SN_SCU->BCD_RTC_TIME2_b.YEAR, (sdate->year & UNIT_BIT_MASK));
            HAL_REG_WRITE(SN_SCU->BCD_RTC_TIME2_b.YEAR_DEC, (sdate->year & DEC_BIT_MASK) >> RTC_TIME_DEC_Pos);
            HAL_REG_WRITE(SN_SCU->BCD_RTC_TIME2_b.MONTH, (sdate->month & UNIT_BIT_MASK));
            HAL_REG_WRITE(SN_SCU->BCD_RTC_TIME2_b.MONTH_DEC, (sdate->month & DEC_BIT_MASK) >> RTC_TIME_DEC_Pos);
            HAL_REG_WRITE(SN_SCU->BCD_RTC_TIME2_b.DATE, (sdate->date & UNIT_BIT_MASK));
            HAL_REG_WRITE(SN_SCU->BCD_RTC_TIME2_b.DATE_DEC, (sdate->date & DEC_BIT_MASK) >> RTC_TIME_DEC_Pos);

            HAL_REG_WRITE(SN_SCU->BCD_RTC_TIME1_b.WEEKDAY, sdate->week_day);
            /* Note: Writing a Reg once requires consideration of the MASK and Resvered BIT */
        }

        /* Set the RTC_EN bit and wait until the RTCEN_STS bit is changed to '1' and the RTC counter will be re-enabled */
        status = _rtc_en_sts_check(hrtc, RTCEN_STS_ENABLE);
    }
    if (status == HAL_OK)
    {
        hrtc->state = HAL_STATE_READY;
    }

    /* Release the process protection */
    GIVE_MUTEX(hrtc);

    return status;
}

/**
 * @brief  Gets RTC current date.
 * @param  hrtc Pointer to a RTC_Handle_T structure that contains the configuration information for RTC.
 * @param  date Pointer to Date structure
 * @param  format Specifies the format of the entered parameters.
 *          This parameter can be one of the following values:
 *            @arg RTC_FORMAT_BIN:  Binary data format
 *            @arg RTC_FORMAT_BCD:  BCD data format
 * @note  You must call HAL_RTC_GetDate() after HAL_RTC_GetTime() to unlock the
 *        values in the higher-order calendar shadow registers to ensure
 *        consistency between the time and date values.
 *        Reading RTC current time locks the values in calendar shadow registers
 *        until current date is read to ensure consistency between the time and
 *        date values.
 * @retval HAL status
 */
HAL_Status_T HAL_RTC_GetDate(RTC_Handle_T *hrtc, RTC_Date_T *date, uint32_t format)
{
    /* Check the RTC peripheral state */
    ASSERT_PARAM(hrtc);

    /* Check the parameters */
    ASSERT_PARAM(IS_RTC_FORMAT(format));

    /*  Two consecutive read current time */
    uint32_t cmp         = 0;
    uint8_t  tmp_century = 0xFF, tmp_year = 0xFF, tmp_month = 0xFF, tmp_date = 0xFF;
    while (cmp == 0)
    {
        if (format == RTC_FORMAT_BIN)
        {
            date->century  = HAL_REG_READ(SN_SCU->RTC_TIME2_b.CENTURY);
            date->year     = HAL_REG_READ(SN_SCU->RTC_TIME2_b.YEAR);
            date->month    = HAL_REG_READ(SN_SCU->RTC_TIME2_b.MONTH);
            date->date     = HAL_REG_READ(SN_SCU->RTC_TIME2_b.DATE);
            date->week_day = HAL_REG_READ(SN_SCU->RTC_TIME1_b.WEEKDAY);
        }
        else // format == RTC_FORMAT_BCD
        {
            date->century  = HAL_REG_READ(SN_SCU->BCD_RTC_TIME2_b.CENTURY) | (HAL_REG_READ(SN_SCU->BCD_RTC_TIME2_b.CENTURY_DEC) << RTC_TIME_DEC_Pos);
            date->year     = HAL_REG_READ(SN_SCU->BCD_RTC_TIME2_b.YEAR) | (HAL_REG_READ(SN_SCU->BCD_RTC_TIME2_b.YEAR_DEC) << RTC_TIME_DEC_Pos);
            date->month    = HAL_REG_READ(SN_SCU->BCD_RTC_TIME2_b.MONTH) | (HAL_REG_READ(SN_SCU->BCD_RTC_TIME2_b.MONTH_DEC) << RTC_TIME_DEC_Pos);
            date->date     = HAL_REG_READ(SN_SCU->BCD_RTC_TIME2_b.DATE) | (HAL_REG_READ(SN_SCU->BCD_RTC_TIME2_b.DATE_DEC) << RTC_TIME_DEC_Pos);
            date->week_day = HAL_REG_READ(SN_SCU->BCD_RTC_TIME1_b.WEEKDAY);
        }
        if ((date->century == tmp_century) && (date->year == tmp_year) && (date->month == tmp_month) && (date->date == tmp_date))
        {
            cmp = 1;
        }
        tmp_century = date->century;
        tmp_year    = date->year;
        tmp_month   = date->month;
        tmp_date    = date->date;
    }

    return HAL_OK;
}
/*
 ===============================================================================
                 ##### RTC Alarm functions #####
 ===============================================================================
 */

/**
 * @brief  Sets the specified RTC Alarm with Interrupt. (Can wake up the system from the Stop and Standby low power modes.)
 * @param  hrtc Pointer to a RTC_Handle_T structure that contains the configuration information for RTC.
 * @param  salarm Pointer to Alarm structure
 * @param  format Specifies the format of the entered parameters.
 *          This parameter can be one of the following values:
 *             @arg RTC_FORMAT_BIN: Binary data format
 *             @arg RTC_FORMAT_BCD: BCD data format
 * @note   The Alarm register can only be written when the corresponding Alarm
 *         is disabled (Use the HAL_RTC_DeactivateAlarm()).
 * @note   The HAL_RTC_SetTime() must be called before enabling the Alarm feature.
 * @retval HAL status
 */
HAL_Status_T HAL_RTC_SetAlarm(RTC_Handle_T *hrtc, RTC_Alarm_T *salarm, uint32_t format)
{
    /* Check the RTC peripheral state */
    ASSERT_PARAM(hrtc);

    RET_STATE_NOT_READY(hrtc);

    /* Protect the process */
    TAKE_MUTEX(hrtc);

    HAL_Status_T status = _rtc_alarm(hrtc, salarm, format);
    /* RTC ALARM Specifies the time */
    if (status == HAL_OK)
    {
        /* Disable RTC ALARM Interrupt */
        HAL_REG_WRITE(SN_SCU->IE_b.RTC_ALARM_EINT, 0);
        hrtc->state = HAL_STATE_READY;
    }

    /* Release the process protection */
    GIVE_MUTEX(hrtc);

    return status;
}

/**
 * @brief  Sets the specified RTC Alarm with Interrupt.
 * @param  hrtc Pointer to a RTC_Handle_T structure that contains the configuration information for RTC.
 * @param  salarm Pointer to Alarm structure
 * @param  format Specifies the format of the entered parameters.
 *          This parameter can be one of the following values:
 *             @arg RTC_FORMAT_BIN: Binary data format
 *             @arg RTC_FORMAT_BCD: BCD data format
 * @note   The Alarm register can only be written when the corresponding Alarm
 *         is disabled (Use the HAL_RTC_DeactivateAlarm()).
 * @note   The HAL_RTC_SetTime() must be called before enabling the Alarm feature.
 * @retval HAL status
 */
HAL_Status_T HAL_RTC_SetAlarm_IT(RTC_Handle_T *hrtc, RTC_Alarm_T *salarm, uint32_t format)
{
    /* Check the RTC peripheral state */
    ASSERT_PARAM(hrtc);

    RET_STATE_NOT_READY(hrtc);

    /* Protect the process */
    TAKE_MUTEX(hrtc);

    /* RTC ALARM Specifies the time */
    HAL_Status_T status = _rtc_alarm(hrtc, salarm, format);
    if (status == HAL_OK)
    {
        /* Enable RTC ALARM Interrupt */
        HAL_REG_WRITE(SN_SCU->IE_b.RTC_ALARM_EINT, 1);
        hrtc->state = HAL_STATE_READY;
    }

    /* Release the process protection */
    GIVE_MUTEX(hrtc);

    return status;
}

/**
 * @brief  Sets the specified RTC Second with Interrupt.
 * @param  hrtc Pointer to a RTC_Handle_T structure that contains the configuration information for RTC.
 * @retval HAL status
 */
HAL_Status_T HAL_RTC_ActivateSecond_IT(RTC_Handle_T *hrtc)
{
    /* Check the RTC peripheral state */
    ASSERT_PARAM(hrtc);

    RET_STATE_NOT_READY(hrtc);

    /* Protect the process */
    TAKE_MUTEX(hrtc);

    /* Clear the second interrupt pending bit */
    HAL_REG_WRITE(SN_SCU->RIS, SECOND_IRQ_FLAG_MASK);

    /* Set the RTC_SECOUT_EN bit and wait to check for success */
    HAL_REG_WRITE(SN_SCU->RTC_CTRL_b.SECOUT_EN, 1);

    /* Enable RTC second Interrupt */
    HAL_REG_WRITE(SN_SCU->IE_b.RTC_SEC_EINT, 1);
    hrtc->state = HAL_STATE_READY;

    /* Release the process protection */
    GIVE_MUTEX(hrtc);

    return HAL_OK;
}

/**
 * @brief  Deactivates the specified RTC Alarm.
 * @param  hrtc Pointer to a RTC_Handle_T structure that contains the configuration information for RTC.
 * @retval HAL status
 */
HAL_Status_T HAL_RTC_DeactivateAlarm(RTC_Handle_T *hrtc)
{
    /* Check the RTC peripheral state */
    ASSERT_PARAM(hrtc);

    HAL_Status_T status = HAL_OK;

    /* Process Locked */
    TAKE_MUTEX(hrtc);
    hrtc->state = HAL_STATE_BUSY;

    /* Disable Alarm */
    HAL_REG_CLEAR(SN_SCU->RTC_CTRL_b.RTC_ALARM_EN);

    /* In case interrupt mode is used, the interrupt source must be disabled */
    HAL_REG_CLEAR(SN_SCU->IE_b.RTC_ALARM_EINT);

    /* Wait till RTC ALARM status flag is set and if timeout is reached exit */
    status = _rtc_alm_sts_check(hrtc, RTC_ALM_STS_DISABLE);

    hrtc->state = HAL_STATE_READY;
    /* Release the process protection */
    GIVE_MUTEX(hrtc);

    return status;
}

/**
 * @brief  Deactivates the specified RTC Second.
 * @param  hrtc Pointer to a RTC_Handle_T structure that contains the configuration information for RTC.
 * @retval HAL status
 */
HAL_Status_T HAL_RTC_DeactivateSecond(RTC_Handle_T *hrtc)
{
    /* Check the RTC peripheral state */
    ASSERT_PARAM(hrtc);

    HAL_Status_T status = HAL_OK;

    /* Process Locked */
    TAKE_MUTEX(hrtc);
    hrtc->state = HAL_STATE_BUSY;

    /* Disable Second */
    HAL_REG_CLEAR(SN_SCU->RTC_CTRL_b.SECOUT_EN);

    /* The interrupt source must be disabled */
    HAL_REG_CLEAR(SN_SCU->IE_b.RTC_SEC_EINT);

    hrtc->state = HAL_STATE_READY;
    /* Release the process protection */
    GIVE_MUTEX(hrtc);

    return status;
}

/**
 * @brief  Gets the RTC Alarm value and masks.
 * @param  hrtc Pointer to a RTC_Handle_T structure that contains the configuration information for RTC.
 * @param  salarm Pointer to Date structure
 * @param  format Specifies the format of the entered parameters.
 *          This parameter can be one of the following values:
 *             @arg RTC_FORMAT_BIN: Binary data format
 *             @arg RTC_FORMAT_BCD: BCD data format
 * @retval HAL status
 */
HAL_Status_T HAL_RTC_GetAlarm(RTC_Handle_T *hrtc, RTC_Alarm_T *salarm, uint32_t format)
{
    /* Check the RTC peripheral state */
    ASSERT_PARAM(hrtc);

    /* Check the parameters */
    ASSERT_PARAM(IS_RTC_FORMAT(format));

    if (format == RTC_FORMAT_BIN)
    {
        /* Fill the structure with the read parameters */
        /* Note: According to the description of the register,
                the ALARM time field does not match the time and the corresponding field mask should be set.
                So, the data read from the ALARM register may be 0x7, 0x1F,0x3F, or 0x31, 0x63, (Don't Care Value).
        */
        salarm->alarm_time.hours   = HAL_REG_READ(SN_SCU->RTC_ALM1_b.HOUR);
        salarm->alarm_time.minutes = HAL_REG_READ(SN_SCU->RTC_ALM1_b.MIN);
        salarm->alarm_time.seconds = HAL_REG_READ(SN_SCU->RTC_ALM1_b.SEC);

        if (HAL_REG_READ(SN_SCU->RTC_ALM1_b.WEEKDAY) != 7)
        {
            salarm->alarm_date_weekday_sel = RTC_ALARMSEL_WEEKDAY;
            salarm->alarm_date_weekday     = HAL_REG_READ(SN_SCU->RTC_ALM1_b.WEEKDAY);
        }
        else // Date
        {
            salarm->alarm_date_weekday_sel = RTC_ALARMSEL_DATE;
            salarm->alarm_date_weekday     = HAL_REG_READ(SN_SCU->RTC_ALM2_b.DATE);
        }
    }
    else
    {
        salarm->alarm_time.hours   = HAL_REG_READ(SN_SCU->BCD_RTC_ALM1_b.HOUR) | (HAL_REG_READ(SN_SCU->BCD_RTC_ALM1_b.HOUR_DEC) << RTC_TIME_DEC_Pos);
        salarm->alarm_time.minutes = HAL_REG_READ(SN_SCU->BCD_RTC_ALM1_b.MIN) | (HAL_REG_READ(SN_SCU->BCD_RTC_ALM1_b.MIN_DEC) << RTC_TIME_DEC_Pos);
        salarm->alarm_time.seconds = HAL_REG_READ(SN_SCU->BCD_RTC_ALM1_b.SEC) | (HAL_REG_READ(SN_SCU->BCD_RTC_ALM1_b.SEC_DEC) << RTC_TIME_DEC_Pos);

        if (HAL_REG_READ(SN_SCU->BCD_RTC_ALM1_b.WEEKDAY) != 7)
        {
            salarm->alarm_date_weekday_sel = RTC_ALARMSEL_WEEKDAY;
            salarm->alarm_date_weekday     = HAL_REG_READ(SN_SCU->BCD_RTC_ALM1_b.WEEKDAY);
        }
        else
        {
            salarm->alarm_date_weekday_sel = RTC_ALARMSEL_DATE;
            salarm->alarm_date_weekday     = SN_SCU->BCD_RTC_ALM2_b.DATE | (SN_SCU->BCD_RTC_ALM2_b.DATE_DEC << RTC_TIME_DEC_Pos);
        }
    }

    return HAL_OK;
}

/**
 * @brief  Handles Alarm interrupt request.
 * @param  hrtc Pointer to a RTC_Handle_T structure that contains the configuration information for RTC.
 * @param  ris Interrupt status.
 * @retval None
 */
__weak ATTR_ISR void HAL_RTC_AlarmIRQHandler(RTC_Handle_T *hrtc, uint32_t ris)
{
    /* If it is DPD mode wake-up, and no IRQ processing of ALARM is required */
    if (HAL_REG_READ(SN_SCU->BTUP_STS_b.DPDWKF) != 1)
    {
        /* Get the Alarm  interrupt source enable status */
        if (IS_RTC_ALARM_IT(ris))
        {
/* Alarm  callback */
#if (configUSE_RTC_REGISTER_CALLBACKS == 1)
            if (hrtc->AlarmEventCallback == NULL)
            {
                return;
            }
            hrtc->AlarmEventCallback(hrtc);
#else
            HAL_RTC_AlarmEventCallback(hrtc);
#endif
        }

        /* Get the Second  interrupt source enable status */
        if (IS_RTC_SEC_IT(ris))
        {
/* Second  callback */
#if (configUSE_RTC_REGISTER_CALLBACKS == 1)
            if (hrtc->SecondEventCallback == NULL)
            {
                return;
            }
            hrtc->SecondEventCallback(hrtc);
#else
            HAL_RTC_SecondEventCallback(hrtc);
#endif
        }

        /* Change RTC state */
        hrtc->state = HAL_STATE_READY;
    }
}

/**
 * @brief  Handles Alarm Polling request.
 * @param  hrtc Pointer to a RTC_Handle_T structure that contains the configuration information for RTC.
 * @param  timeout Timeout duration
 * @retval HAL status
 */
HAL_Status_T HAL_RTC_PollForAlarmEvent(RTC_Handle_T *hrtc, uint32_t timeout)
{
    /* Check the RTC peripheral state */
    ASSERT_PARAM(hrtc);

    uint32_t tickstart = 0U;

    /* Clear current error code */
    CLEAR_ERROR_CODE(hrtc);

    /* Get tick */
    tickstart = HAL_GetTick();

    /* Wait till RTC ALARM flag is set and if timeout is reached exit */
    while (HAL_REG_READ(SN_SCU->RIS_b.INT_RTC_ALARM) == 0U)
    {
        /* Check timeout */
        BREAK_PROC_TIMEOUT(hrtc, tickstart, timeout);
    }

    RET_FLAG_TRUE((hrtc->error_code == HAL_ERROR_TIMEOUT), HAL_ERROR);

    /* Clear the Alarm flag */
    HAL_REG_WRITE(SN_SCU->RIS, ALARM_IRQ_FLAG_MASK);

    /* Change RTC state */
    hrtc->state = HAL_STATE_READY;

    return HAL_OK;
}

/**
 * @brief  Alarm callback.
 * @param  hrtc Pointer to a RTC_Handle_T structure that contains the configuration information for RTC.
 * @retval None
 */
__weak void HAL_RTC_AlarmEventCallback(RTC_Handle_T *hrtc)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hrtc);

    /* NOTE: This function should not be modified, when the callback is needed,
             the HAL_RTC_AlarmAEventCallback could be implemented in the user file
     */
}

/**
 * @brief  Second callback.
 * @param  hrtc Pointer to a RTC_Handle_T structure that contains the configuration information for RTC.
 * @retval None
 */
__weak void HAL_RTC_SecondEventCallback(RTC_Handle_T *hrtc)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hrtc);

    /* NOTE: This function should not be modified, when the callback is needed,
             the HAL_RTC_AlarmAEventCallback could be implemented in the user file
     */
}

/*
 ===============================================================================
                     ##### Peripheral State functions #####
 ===============================================================================
  */
/**
 * @brief  Returns the RTC state.
 * @param  hrtc Pointer to a RTC_Handle_T structure that contains the configuration information for RTC.
 * @retval HAL state
 */
HAL_State_T HAL_RTC_GetState(RTC_Handle_T *hrtc)
{
    /* Check the RTC peripheral state */
    ASSERT_PARAM(hrtc);
    return hrtc->state;
}

/**
 * @brief  Return the RTC error code
 * @param  hrtc Pointer to a RTC_Handle_T structure that contains the configuration information for the specified RTC module.
 * @retval RTC error code
 */
uint32_t HAL_RTC_GetError(RTC_Handle_T *hrtc)
{
    /* check the RTC handle */
    ASSERT_PARAM(hrtc);

    return hrtc->error_code;
}

/**
 * @brief  Writes a data in a specified RTC Backup data register.
 * @param  hrtc Pointer to a RTC_Handle_T structure that contains the configuration information for RTC.
 * @param  backup_register RTC Backup data Register number.
 *          This parameter can be: RTC_BKP_DRx (where x can be from 0 to 19)
 *                                 to specify the register.
 * @param  data Data to be written in the specified RTC Backup data register.
 * @retval None
 */
HAL_Status_T HAL_RTCEx_BKUPWrite(RTC_Handle_T *hrtc, uint32_t backup_register, uint32_t data)
{
    uint32_t tmp = 0U;

    if (!IS_RTC_BKP(backup_register))
    {
        return HAL_ERROR;
    }

    tmp = (uint32_t)&(SN_ALWAYSON->BKPREG0);
    tmp += (backup_register * 4U);

    /* Write the specified register */
    *(__IO uint32_t *)tmp = (uint32_t)data;

    return HAL_OK;
}

/**
 * @brief  Reads data from the specified RTC Backup data Register.
 * @param  hrtc Pointer to a RTC_Handle_T structure that contains the configuration information for RTC.
 * @param  backup_register RTC Backup data Register number.
 *          This parameter can be: RTC_BKP_DRx (where x can be from 0 to 19)
 *                                 to specify the register.
 * @retval Read value
 */
uint32_t HAL_RTCEx_BKUPRead(RTC_Handle_T *hrtc, uint32_t backup_register)
{
    uint32_t tmp = 0U;

    /* Check the parameters */
    ASSERT_PARAM(IS_RTC_BKP(backup_register));

    tmp = (uint32_t)&(SN_ALWAYSON->BKPREG0);
    tmp += (backup_register * 4U);

    /* Read the specified register */
    return (*(__IO uint32_t *)tmp);
}

/**
 * @brief  Compensates for frequency drift caused by crystal error of external clock source.
 * @param  hrtc Pointer to a RTC_Handle_T structure that contains the configuration information for RTC.
 * @param  measure_data Clock source error calibration value
 * @retval None
 */
void HAL_RTCEx_Calibration(RTC_Handle_T *hrtc, uint32_t measure_data)
{
    /* check the RTC handle */
    ASSERT_PARAM(hrtc);

    /* Check whether the clock source needs to be calibrated */
    if (hrtc->init.clk_source == RTC_CLK_SOURCE_ELS)
    {
        /* is_cali_flag Checks whether calibration is performed  */
        if (hrtc->is_cali_flag == FALSE)
        {
            /* No need for multiple calibration */
            _rtc_calibration_(measure_data);
            hrtc->is_cali_flag = TRUE;
        }
    }

    /* The ILRC clock source of the RTC requires no calibration, So return */
    return;
}

/**
 * @brief  Compensates for frequency drift caused by crystal error of external clock source.
 * @param  hrtc Pointer to a RTC_Handle_T structure that contains the configuration information for RTC.
 * @param  trim_p
 * @param  trim_f
 * @retval None
 */
void HAL_RTCEx_CalibrationWithTrim(RTC_Handle_T *hrtc, uint32_t trim_p, uint32_t trim_f)
{
    /* check the RTC handle */
    ASSERT_PARAM(hrtc);

    /* Check whether the clock source needs to be calibrated */
    if (hrtc->init.clk_source == RTC_CLK_SOURCE_ELS)
    {
        /* No need for multiple calibration */
        *(uint32_t *)(0x4001F014) = SN_SCU->CHIPID;
        SN_SCU->RTC_TRIM          = ((trim_p << 16) | trim_f);
    }

    /* The ILRC clock source of the RTC requires no calibration, So return */
    return;
}

/**
 * @brief RTC Enable
 * @param hrtc Pointer to a RTC_Handle_T structure that contains the configuration information for RTC.
 * @retval None
 */
HAL_Status_T HAL_RTC_Enable(RTC_Handle_T *hrtc)
{
    /* check the RTC handle */
    ASSERT_PARAM(hrtc);

    return _rtc_en_sts_check(hrtc, 0x1);
}

/**
 * @brief RTC Disable
 * @param hrtc Pointer to a RTC_Handle_T structure that contains the configuration information for RTC.
 * @retval None
 */
HAL_Status_T HAL_RTC_Disable(RTC_Handle_T *hrtc)
{
    /* check the RTC handle */
    ASSERT_PARAM(hrtc);

    return _rtc_en_sts_check(hrtc, 0x0);
}

/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/**
 * @brief  Converts from 2 digit BCD to Binary.
 * @param  value BCD value to be converted
 * @retval Converted word
 */
#if (configUSE_ASSERT == 1)
static uint8_t _rtc_bcd_to_byte(uint8_t value)
{
    uint32_t tmp = 0U;
    tmp          = ((uint8_t)(value & (uint8_t)0xF0) >> (uint8_t)0x4) * 10;
    return (tmp + (value & (uint8_t)0x0F));
}
#endif

/**
 * @brief Check and read the RTC_STS bit of the RTC control register.
 *
 * @param hrtc Pointer to a RTC_Handle_T structure that contains the configuration information for RTC.
 * @param rtc_sts RTC status
 * @return HAL_Status_T
 */
static HAL_Status_T _rtc_en_sts_check(RTC_Handle_T *hrtc, uint32_t rtc_sts)
{
    /* If RTC_EN is not disabled, try to wait */
    uint32_t     tickstart = 0U;
    HAL_Status_T status    = HAL_OK;

    /* Clear current error code */
    CLEAR_ERROR_CODE(hrtc);

    /* Set the RTC_EN bit and wait to check for success */
    HAL_REG_WRITE(SN_SCU->RTC_CTRL_b.RTC_EN, rtc_sts);

    /* Get tick */
    tickstart = HAL_GetTick();

    /* wait until the RTC_STS bit is changed to rtc_sts and the setting of the RTC alarm is complete. */
    while (HAL_REG_READ(SN_SCU->RTC_CTRL_b.RTCEN_STS) != rtc_sts)
    {
        /* Check timeout */
        BREAK_PROC_TIMEOUT(hrtc, tickstart, RTC_TIMEOUT_VALUE);
    }

    RET_FLAG_TRUE((hrtc->error_code == HAL_ERROR_TIMEOUT), HAL_ERROR);

    return status;
}

/**
 * @brief Check and read the RTC_ALM_STS bit of the RTC control register
 *
 * @param hrtc Pointer to a RTC_Handle_T structure that contains the configuration information for RTC.
 * @param rtc_alm_sts RTC Alarm status
 * @return HAL_Status_T
 */
static HAL_Status_T _rtc_alm_sts_check(RTC_Handle_T *hrtc, uint32_t rtc_alm_sts)
{
    /* If RTC_EN is not disabled, try to wait */
    uint32_t     tickstart = 0U;
    HAL_Status_T status    = HAL_OK;

    /* Clear current error code */
    CLEAR_ERROR_CODE(hrtc);

    /* Clear the Alarm interrupt pending bit */
    HAL_REG_WRITE(SN_SCU->RIS, ALARM_IRQ_FLAG_MASK);

    /* Set the RTC_ALARM_EN bit and wait to check for success */
    HAL_REG_WRITE(SN_SCU->RTC_CTRL_b.RTC_ALARM_EN, rtc_alm_sts);

    /* Get tick */
    tickstart = HAL_GetTick();

    /* wait until the RTC_STS bit is changed to rtc_alm_sts and the setting of the RTC alarm is complete. */
    while (HAL_REG_READ(SN_SCU->RTC_CTRL_b.RTC_ALMEN_STS) != rtc_alm_sts)
    {
        /* Check timeout */
        BREAK_PROC_TIMEOUT(hrtc, tickstart, RTC_TIMEOUT_VALUE);
    }

    RET_FLAG_TRUE((hrtc->error_code == HAL_ERROR_TIMEOUT), HAL_ERROR);

    return status;
}

/**
 * @brief set new trim value by calculation
 *
 * @param one_micro_second one micro second
 */
static void _rtc_calibration_(uint32_t one_micro_second)
{
    uint32_t new_trim_p, new_trim_f;
    /* The TRIM_P field is the fixed-point part and is always a constant */
    const uint32_t trim_p = 327;
    /* wNewTrimP = wELSOscFreq/100; */
    new_trim_p = (1000000 * (trim_p + 1)) / one_micro_second;
    /* wNewTrimF = (wELSOscFreq%100)*10; */
    new_trim_f = ((1000000 * (trim_p + 1)) % one_micro_second) / 1000;

    *(uint32_t *)(0x4001F014) = SN_SCU->CHIPID;
    SN_SCU->RTC_TRIM          = ((new_trim_p << 16) | new_trim_f);
}

/**
 * @brief RTC ALARM Specifies the time
 *
 */
static HAL_Status_T _rtc_alarm(RTC_Handle_T *hrtc, RTC_Alarm_T *salarm, uint32_t format)
{
    /* Check the parameters */
    ASSERT_PARAM(salarm);
    ASSERT_PARAM(IS_RTC_FORMAT(format));

    ASSERT_PARAM(IS_RTC_ALARM_DATE_WEEKDAY_SEL(salarm->alarm_date_weekday_sel));

    /* Check the data format (Binary or BCD) and store the Alarm time and date configuration accordingly */
    if (format == RTC_FORMAT_BIN)
    {
        ASSERT_PARAM(IS_RTC_HOUR24(salarm->alarm_time.hours));
        ASSERT_PARAM(IS_RTC_MINUTES(salarm->alarm_time.minutes));
        ASSERT_PARAM(IS_RTC_SECONDS(salarm->alarm_time.seconds));

        if (salarm->alarm_date_weekday_sel == RTC_ALARMSEL_DATE)
        {
            ASSERT_PARAM(IS_RTC_ALARM_DATE_WEEKDAY_DATE(salarm->alarm_date_weekday));
        }
        else
        {
            ASSERT_PARAM(IS_RTC_ALARM_DATE_WEEKDAY_WEEKDAY(salarm->alarm_date_weekday));
        }
    }
    else // format == RTC_FORMAT_BCD
    {
        ASSERT_PARAM(IS_RTC_HOUR24(_rtc_bcd_to_byte(salarm->alarm_time.hours)));
        ASSERT_PARAM(IS_RTC_MINUTES(_rtc_bcd_to_byte(salarm->alarm_time.minutes)));
        ASSERT_PARAM(IS_RTC_SECONDS(_rtc_bcd_to_byte(salarm->alarm_time.seconds)));

        if (salarm->alarm_date_weekday_sel == RTC_ALARMSEL_DATE)
        {
            ASSERT_PARAM(IS_RTC_ALARM_DATE_WEEKDAY_DATE(_rtc_bcd_to_byte(salarm->alarm_date_weekday)));
        }
        else
        {
            ASSERT_PARAM(IS_RTC_ALARM_DATE_WEEKDAY_WEEKDAY(_rtc_bcd_to_byte(salarm->alarm_date_weekday)));
        }
    }

    /* Change RTC state to BUSY */
    hrtc->state = HAL_STATE_BUSY;

    /* Check and read the RTC_ALM_STS bit of the RTC control register to confirm if RTC alarm is disabled */
    if (_rtc_alm_sts_check(hrtc, RTC_ALM_STS_DISABLE) != HAL_OK)
    {
        return HAL_ERROR;
    }

    /* Set new time to registers, Alarm Time1 and Alarm Time2       */
    if (format == RTC_FORMAT_BIN)
    {
        HAL_REG_WRITE(SN_SCU->RTC_ALM2_b.MONTH, 0); // Don't as match value, so,don’t care.

        /* Avoid : DATE and Week cannot be matched at the same time.!!! */
        if (salarm->alarm_date_weekday_sel == RTC_ALARMSEL_DATE)
        {
            HAL_REG_WRITE(SN_SCU->RTC_ALM2_b.DATE, salarm->alarm_date_weekday);
            HAL_REG_WRITE(SN_SCU->RTC_ALM1_b.WEEKDAY, 7);
        }
        else
        {
            HAL_REG_WRITE(SN_SCU->RTC_ALM1_b.WEEKDAY, salarm->alarm_date_weekday);
            HAL_REG_WRITE(SN_SCU->RTC_ALM2_b.DATE, 0);
        }

        HAL_REG_WRITE(SN_SCU->RTC_ALM1_b.HOUR, salarm->alarm_time.hours);
        HAL_REG_WRITE(SN_SCU->RTC_ALM1_b.MIN, salarm->alarm_time.minutes);
        HAL_REG_WRITE(SN_SCU->RTC_ALM1_b.SEC, salarm->alarm_time.seconds);
    }
    else
    {
        HAL_REG_WRITE(SN_SCU->BCD_RTC_ALM2_b.MONTH_DEC, 0); // Don't as match value, so,don’t care.
        HAL_REG_WRITE(SN_SCU->BCD_RTC_ALM2_b.MONTH, 0);     // Don't as match value, so,don’t care.

        /* Avoid:DATE and Week cannot be matched at the same time.!!! */
        if (salarm->alarm_date_weekday_sel == RTC_ALARMSEL_DATE)
        {
            HAL_REG_CLEAR(SN_SCU->BCD_RTC_ALM2_b.DATE);
            HAL_REG_CLEAR(SN_SCU->BCD_RTC_ALM2_b.DATE_DEC);
            HAL_REG_WRITE(SN_SCU->BCD_RTC_ALM2_b.DATE, salarm->alarm_date_weekday & UNIT_BIT_MASK);
            HAL_REG_WRITE(SN_SCU->BCD_RTC_ALM2_b.DATE_DEC, (salarm->alarm_date_weekday & DEC_BIT_MASK) >> RTC_TIME_DEC_Pos);
            HAL_REG_WRITE(SN_SCU->BCD_RTC_ALM1_b.WEEKDAY, 7);
        }
        else
        {
            HAL_REG_WRITE(SN_SCU->BCD_RTC_ALM1_b.WEEKDAY, salarm->alarm_date_weekday);
            HAL_REG_WRITE(SN_SCU->BCD_RTC_ALM2_b.DATE, 0);
            HAL_REG_WRITE(SN_SCU->BCD_RTC_ALM2_b.DATE_DEC, 0);
        }
        HAL_REG_CLEAR(SN_SCU->BCD_RTC_ALM1_b.HOUR);
        HAL_REG_CLEAR(SN_SCU->BCD_RTC_ALM1_b.HOUR_DEC);
        HAL_REG_CLEAR(SN_SCU->BCD_RTC_ALM1_b.MIN);
        HAL_REG_CLEAR(SN_SCU->BCD_RTC_ALM1_b.MIN_DEC);
        HAL_REG_CLEAR(SN_SCU->BCD_RTC_ALM1_b.SEC);
        HAL_REG_CLEAR(SN_SCU->BCD_RTC_ALM1_b.SEC_DEC);
        HAL_REG_WRITE(SN_SCU->BCD_RTC_ALM1_b.HOUR, salarm->alarm_time.hours & UNIT_BIT_MASK);
        HAL_REG_WRITE(SN_SCU->BCD_RTC_ALM1_b.HOUR_DEC, (salarm->alarm_time.hours & DEC_BIT_MASK) >> RTC_TIME_DEC_Pos);
        HAL_REG_WRITE(SN_SCU->BCD_RTC_ALM1_b.MIN, salarm->alarm_time.minutes & UNIT_BIT_MASK);
        HAL_REG_WRITE(SN_SCU->BCD_RTC_ALM1_b.MIN_DEC, (salarm->alarm_time.minutes & DEC_BIT_MASK) >> RTC_TIME_DEC_Pos);
        HAL_REG_WRITE(SN_SCU->BCD_RTC_ALM1_b.SEC, salarm->alarm_time.seconds & UNIT_BIT_MASK);
        HAL_REG_WRITE(SN_SCU->BCD_RTC_ALM1_b.SEC_DEC, (salarm->alarm_time.seconds & DEC_BIT_MASK) >> RTC_TIME_DEC_Pos);
    }

    /* Set the RTC_EN bit and wait until the RTCEN_STS bit is changed to '1' and the RTC counter will be re-enabled */
    if (_rtc_alm_sts_check(hrtc, RTC_ALM_STS_ENABLE) != HAL_OK)
    {
        return HAL_ERROR;
    }

    return HAL_OK;
}

#endif /* configUSE_RTC_PERIPHERAL */
