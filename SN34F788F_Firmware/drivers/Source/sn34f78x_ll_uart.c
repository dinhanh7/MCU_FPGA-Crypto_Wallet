/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#include "sn34f78x_ll_uart.h"

#if defined(USE_FULL_LL_DRIVER)

/* Exported functions --------------------------------------------------------*/
/**
 * @brief  Initialize UART registers according to the specified parameters in uart_init.
 * @param  uart UART Instance
 * @param  uart_init pointer to a LL_UART_Init_T structure that contains the configuration information for the specified UART peripheral.
 * @retval An LL_Status_T enumeration value:
 *          - LL_OK: UART registers are initialized according to UART_InitStruct content
 *          - LL_ERROR: Problem occurred during UART Registers initialization
 */
LL_Status_T LL_UART_Init(void *uart, LL_UART_Init_T *uart_init)
{
    /* Check the instance */
    RET_FLAG_FALSE(IS_LL_UART_INSTANCE(uart), LL_ERROR);

    /* Check the parameters */
    RET_FLAG_FALSE(IS_LL_UART_BAUD_RATE(uart_init->baud_rate), LL_ERROR);
    RET_FLAG_FALSE(IS_LL_UART_WORD_LENGTH(uart_init->word_length), LL_ERROR);
    RET_FLAG_FALSE(IS_LL_UART_STOP_BITS(uart_init->stop_bits), LL_ERROR);
    RET_FLAG_FALSE(IS_LL_UART_PARITY(uart_init->parity), LL_ERROR);
    RET_FLAG_FALSE(IS_LL_UART_HWCONTROL(uart_init->hw_flow_ctl), LL_ERROR);

    uint32_t io_irda_uclk = 0;
    uint32_t clk_sel      = 0;

    switch ((uint32_t)uart)
    {
        case SN_UART0_BASE:
        {
            clk_sel = LL_RCC_GetUART0ClockSource();
            break;
        }
        case SN_UART1_BASE:
        {
            clk_sel = LL_RCC_GetUART1ClockSource();
            break;
        }
        case SN_UART2_BASE:
        {
            clk_sel = LL_RCC_GetUART2ClockSource();
            break;
        }
        case SN_UART3_BASE:
        {
            clk_sel = LL_RCC_GetUART3ClockSource();
            break;
        }
        case SN_UART4_BASE:
        {
            clk_sel = LL_RCC_GetUART4ClockSource();
            break;
        }
        case SN_UART5_BASE:
        {
            clk_sel = LL_RCC_GetUART5ClockSource();
            break;
        }
    }

    if (clk_sel == LL_RCC_PERIPHERAL_CLKSRC_PLL)
        io_irda_uclk = LL_RCC_GetPLLCLKFreq();
    else if (clk_sel == LL_RCC_PERIPHERAL_CLKSRC_HCLK)
        io_irda_uclk = LL_RCC_GetHCLKFreq();

    if (uart_init->baud_rate > (io_irda_uclk >> 4))
    {
        return LL_ERROR;
    }

    uint32_t temp = uart_init->baud_rate * 16;

    /* 1. DL : Round the calculation */
    uint32_t DL_10 = ((io_irda_uclk * 10) / temp);
    uint32_t DL    = DL_10 / 10;
    if (DL_10 % 10 >= 5)
    {
        DL++;
    }

    /* Baud rate check (calculation error value:  Deviation < |3.125%|) */
    uint32_t rel_baud_rate = io_irda_uclk / (DL * 16);
    if (rel_baud_rate > uart_init->baud_rate)
    {
        if (((rel_baud_rate - uart_init->baud_rate) * 1000 / uart_init->baud_rate) >= 31)
            return LL_ERROR;
    }
    else if (rel_baud_rate < uart_init->baud_rate)
    {
        if (((uart_init->baud_rate - rel_baud_rate) * 1000 / uart_init->baud_rate) >= 31)
            return LL_ERROR;
    }

    /* check the inequation "56 * F_PCLK > F_B16XCLK" */
    uint32_t f_b16xclk = io_irda_uclk / (DL * 1); // PRE = 1
    uint32_t f_pclk    = 1;
    if (((void *)uart == SN_UART0) || ((SN_UART4_Type *)uart == SN_UART5))
    {
        f_pclk = LL_RCC_GetAPB1CLKFreq();
    }
    else
    {
        f_pclk = LL_RCC_GetAPB0CLKFreq();
    }
    if (56 * f_pclk <= f_b16xclk)
    {
        return LL_ERROR;
    }

    switch ((uint32_t)uart)
    {
        case SN_UART0_BASE:
        case SN_UART1_BASE:
        case SN_UART2_BASE:
        case SN_UART3_BASE:
        {
            /* set Word Length*/
            LL_UART0123_SetWordLength(uart, uart_init->word_length);
            /* set stop bit*/
            LL_UART0123_SetStopBits(uart, uart_init->stop_bits);

            /* CTS or RTS only support UART0/1/2/3 */
            if (uart_init->hw_flow_ctl == LL_UART_HWCONTROL_NONE)
            {
                LL_UART0123_Disable_CTS(uart);
                LL_UART0123_Disable_RTS(uart);
            }
            else if (uart_init->hw_flow_ctl == LL_UART_HWCONTROL_RTS)
            {
                LL_UART0123_Enable_RTS(uart);
            }
            else if (uart_init->hw_flow_ctl == LL_UART_HWCONTROL_CTS)
            {
                LL_UART0123_Enable_CTS(uart);
            }
            else if (uart_init->hw_flow_ctl == LL_UART_HWCONTROL_RTS_CTS)
            {
                LL_UART0123_Enable_RTS(uart);
                LL_UART0123_Enable_CTS(uart);
            }

            /* Set the parity bit */
            LL_UART0123_SetParity(uart, uart_init->parity);

            /* set Baud rate */
            LL_UART0123_Enable_DivisorLatchAccess(uart);
            LL_UART0123_SetPrescaleValue(uart, 1);
            LL_UART0123_SetDLL(uart, DL & 0xFFU);
            LL_UART0123_SetDLM(uart, (DL >> 8) & 0xFFU);
            LL_UART0123_Disable_DivisorLatchAccess(uart);

            break;
        }

        case SN_UART4_BASE:
        case SN_UART5_BASE:
        {
            /* set Word Length*/
            LL_UART45_SetWordLength(uart, uart_init->word_length);
            /* set stop bit*/
            LL_UART45_SetStopBits(uart, uart_init->stop_bits);

            /* Set the parity bit */
            LL_UART45_SetParity(uart, uart_init->parity);

            /* set Baud rate */
            LL_UART45_Enable_DivisorLatchAccess(uart);
            LL_UART45_SetPrescaleValue(uart, 1);
            LL_UART45_SetDLL(uart, DL & 0xFFU);
            LL_UART45_SetDLM(uart, (DL >> 8) & 0xFFU);
            LL_UART45_Disable_DivisorLatchAccess(uart);

            break;
        }
    }

    return LL_OK;
}

/**
 * @brief  De-initialize UART registers (Registers restored to their default values).
 * @param  uart UART Instance
 * @retval An ErrorStatus enumeration value:
 *          - SUCCESS: UART registers are de-initialized
 *          - ERROR: UART registers are not de-initialized
 */
LL_Status_T LL_UART_DeInit(void *uart)
{
    /* Check the instance */
    RET_FLAG_FALSE(IS_LL_UART_INSTANCE(uart), LL_ERROR);

    return LL_OK;
}

/**
 * @brief Set each @ref LL_UART_Init_T field to default value.
 * @param uart_init Pointer to a @ref LL_UART_Init_T structure whose fields will be set to default values.
 * @retval None
 */
void LL_UART_StructInit(LL_UART_Init_T *uart_init)
{
    if (uart_init != NULL)
    {
        uart_init->baud_rate   = 115200;
        uart_init->word_length = LL_UART_WORD_LENGTH_8;
        uart_init->stop_bits   = LL_UART_STOP_BITS_1;
        uart_init->parity      = LL_UART_PARITY_NONE;
        uart_init->hw_flow_ctl = LL_UART_HWCONTROL_NONE;
    }
}
/** @} */

#endif /* USE_FULL_LL_DRIVER */
