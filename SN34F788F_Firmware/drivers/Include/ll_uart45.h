/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _LL_UART45_H_
#define _LL_UART45_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "ll_uart.h"

/* Exported types ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
/**
 * @brief  Get the data received by UART RX
 * @param  uart UART Instance
 * @retval UART RX data
 */
__STATIC_INLINE uint32_t LL_UART45_ReceiveData(SN_UART4_Type *uart)
{
    return (LL_REG_READ(uart->RB) & UART_RB_RB);
}

/**
 * @brief  The data is sent through the UART TX
 * @param  uart UART Instance
 * @param  data real data
 * @retval none
 */
__STATIC_INLINE void LL_UART45_TransmitData(SN_UART4_Type *uart, uint8_t data)
{
    LL_REG_WRITE(uart->TH, data);
}

/**
 * @brief  Setting the DLL of UART, generally set at the same time as the DLM in
 *  UARTn_DLM, is used to complete the baud rate setting of UART
 * @param  uart UART Instance
 * @param  dll real dll
 * @retval none
 */
__STATIC_INLINE void LL_UART45_SetDLL(SN_UART4_Type *uart, uint32_t dll)
{
    LL_REG_CBIT(uart->DLL, UART_DLL_DLL);
    LL_REG_SBIT(uart->DLL, dll & UART_DLL_DLL);
}

/**
 * @brief  Gets the DLL for the UART setup
 * @param  uart UART Instance
 * @retval UART DLL
 */
__STATIC_INLINE uint32_t LL_UART45_GetDLL(SN_UART4_Type *uart)
{
    return (LL_REG_READ(uart->DLL) & UART_DLL_DLL);
}

/**
 * @brief  Setting the DLM of UART, generally set at the same time as the DLM in
 *  UARTn_DLL, is used to complete the baud rate setting of UART
 * @param  uart UART Instance
 * @param  dlm real dlm
 * @retval none
 */
__STATIC_INLINE void LL_UART45_SetDLM(SN_UART4_Type *uart, uint32_t dlm)
{
    LL_REG_CBIT(uart->DLM, UART_DLM_DLM);
    LL_REG_SBIT(uart->DLM, dlm & UART_DLM_DLM);
}

/**
 * @brief  Gets the DLM for the UART setup
 * @param  uart UART Instance
 * @retval UART DLM
 */
__STATIC_INLINE uint32_t LL_UART45_GetDLM(SN_UART4_Type *uart)
{
    return (LL_REG_READ(uart->DLM) & UART_DLM_DLM);
}

/**
 * @brief  This function is used to enable the UART RDA interrupt
 * @note   RDA interrupt trigger condition is RX data in FIFO reached trigger level
 * @param  uart UART Instance
 * @retval none
 */
__STATIC_INLINE void LL_UART45_EnableIT_RDA(SN_UART4_Type *uart)
{
    LL_REG_SBIT(uart->IE, UART_IE_RDAIE);
}

/**
 * @brief  This function is used to disable the UART RDA interrupt
 * @note   RDA interrupt trigger condition is RX data in FIFO reached trigger level
 * @param  uart UART Instance
 * @retval none
 */
__STATIC_INLINE void LL_UART45_DisableIT_RDA(SN_UART4_Type *uart)
{
    LL_REG_CBIT(uart->IE, UART_IE_RDAIE);
}

/**
 * @brief  This function is used to get the RDA interrupt status
 * @note   RDA interrupt trigger condition is RX data in FIFO reached trigger level
 * @param  uart UART Instance
 * @retval RDA interrupt status
 */
__STATIC_INLINE uint32_t LL_UART45_IsEnabledIT_RDA(SN_UART4_Type *uart)
{
    return (LL_REG_READ(uart->IE) & UART_IE_RDAIE);
}

/**
 * @brief  This function is used to enable the UART THRE interrupt
 * @param  uart UART Instance
 * @retval none
 */
__STATIC_INLINE void LL_UART45_EnableIT_THRE(SN_UART4_Type *uart)
{
    LL_REG_SBIT(uart->IE, UART_IE_THREIE);
}

/**
 * @brief  This function is used to disable the UART THRE interrupt
 * @param  uart UART Instance
 * @retval none
 */
__STATIC_INLINE void LL_UART45_DisableIT_THRE(SN_UART4_Type *uart)
{
    LL_REG_CBIT(uart->IE, UART_IE_THREIE);
}

/**
 * @brief  This function is used to get the THRE interrupt status
 * @param  uart UART Instance
 * @retval THRE interrupt status
 */
__STATIC_INLINE uint32_t LL_UART45_IsEnabledIT_THRE(SN_UART4_Type *uart)
{
    return (LL_REG_READ(uart->IE) & UART_IE_THREIE);
}

/**
 * @brief  This function is used to enable the UART RLS interrupt
 * @note   The outage sources of RLS are overflow error (OE), parity error (PE),
 * Frame error (FE) or interruption interruption (BI)
 * @param  uart UART Instance
 * @retval none
 */
__STATIC_INLINE void LL_UART45_EnableIT_RLS(SN_UART4_Type *uart)
{
    LL_REG_SBIT(uart->IE, UART_IE_RLSIE);
}

/**
 * @brief  This function is used to disable the UART RLS interrupt
 * @note   The outage sources of RLS are overflow error (OE), parity error (PE),
 * Frame error (FE) or interruption interruption (BI)
 * @param  uart UART Instance
 * @retval none
 */
__STATIC_INLINE void LL_UART45_DisableIT_RLS(SN_UART4_Type *uart)
{
    LL_REG_CBIT(uart->IE, UART_IE_RLSIE);
}

/**
 * @brief  This function is used to get the RLS interrupt status
 * @note   The outage sources of RLS are overflow error (OE), parity error (PE),
 * Frame error (FE) or interruption interruption (BI)
 * @param  uart UART Instance
 * @retval RLS interrupt status
 */
__STATIC_INLINE uint32_t LL_UART45_IsEnabledIT_RLS(SN_UART4_Type *uart)
{
    return (LL_REG_READ(uart->IE) & UART_IE_RLSIE);
}

/**
 * @brief  This function is used to get the current interrupt identification
 * @param  uart UART Instance
 * @retval Interrupt Identification
 */
__STATIC_INLINE uint32_t LL_UART45_GetInterruptIdentification(SN_UART4_Type *uart)
{
    return (LL_REG_RBIT(uart->II, UART_II_INTSTATUS | UART_II_INTID));
}

/**
 * @brief  This function is used to get the TX FIFO full status
 * @note   0: TX FIFO is not Full
 *         1: TX FIFO is Full
 * @param  uart UART Instance
 * @retval TX FIFO full status
 */
__STATIC_INLINE uint32_t LL_UART45_IsActiveFlag_TXFIFOFULL(SN_UART4_Type *uart)
{
    return (LL_REG_RBIT(uart->II, UART_II_TXFIFOFULL) >> UART_II_TXFIFOFULL_Pos);
}

/**
 * @brief  This function is used to enable the FIFO
 * @param  uart UART Instance
 * @retval none
 */
__STATIC_INLINE void LL_UART45_Enable_FIFO(SN_UART4_Type *uart)
{
    LL_REG_SBIT(uart->FIFOCTRL, UART_FIFOCTRL_FIFOEN);
}

/**
 * @brief  This function is used to disable the FIFO
 * @param  uart UART Instance
 * @retval none
 */
__STATIC_INLINE void LL_UART45_Disable_FIFO(SN_UART4_Type *uart)
{
    LL_REG_CBIT(uart->FIFOCTRL, UART_FIFOCTRL_FIFOEN);
}

/**
 * @brief  This function is used to get the FIFO status
 * @param  uart UART Instance
 * @retval FIFO status
 */
__STATIC_INLINE uint32_t LL_UART45_IsEnabled_FIFO(SN_UART4_Type *uart)
{
    return (LL_REG_READ(uart->II) & UART_II_FIFOEN);
}

/**
 * @brief  This function is used to reset RX FIFO
 * @note   0: No effect
 *         1: : Clear all bytes in RX FIFO and resets the counter to 0, and will return to zero
 *         automatically. The shift register is not cleared, so any reception active will continue
 * @param  uart UART Instance
 * @retval none
 */
__STATIC_INLINE void LL_UART45_RXFIFOReset(SN_UART4_Type *uart)
{
    LL_REG_WRITE(uart->FIFOCTRL, UART_FIFOCTRL_RXFIFORST | UART_FIFOCTRL_FIFOEN);
}

/**
 * @brief  This function is used to reset TX FIFO
 * @note   0: No effect
 *         1: : Clear all bytes in TX FIFO and resets the counter to 0, and will return to zero
 *         automatically. The shift register is not cleared, so any reception active will continue
 * @param  uart UART Instance
 * @retval none
 */
__STATIC_INLINE void LL_UART45_TXFIFOReset(SN_UART4_Type *uart)
{
    LL_REG_WRITE(uart->FIFOCTRL, UART_FIFOCTRL_TXFIFORST | UART_FIFOCTRL_FIFOEN);
}

/**
 * @brief  This function is used to set trigger level of RX FIFO interrupt @ref uart_Trigger_level_RX_FIFO_interrupt
 * @note   00: 1 character
 *         01: 4 characters
 *         10: 8 characters
 *         11: 14 characters
 * @param  uart UART Instance
 * @param  level trigger level @ref uart_Trigger_level_RX_FIFO_interrupt
 * @retval none
 */
__STATIC_INLINE void LL_UART45_SetRXTriggerLevel(SN_UART4_Type *uart, uint32_t level)
{
    LL_REG_WRITE(uart->FIFOCTRL, (level & UART_FIFOCTRL_RXTL) | UART_FIFOCTRL_FIFOEN);
}

/**
 * @brief  This function is used to set Prescale value
 * @note   This 5-bit register adds a second programmable division factor to obtain
 * the desired baud rate. The division factor is the value hold in this register, so
 * the maximum factor is 31 and the minimum is 0. When PRE is 0, there is no input
 * clock to divisor latch unit. Therefore, programming DLL and DLM is useless when PRE is 0
 * @param  uart UART Instance
 * @param  prescale prescale vale
 * @retval none
 */
__STATIC_INLINE void LL_UART45_SetPrescaleValue(SN_UART4_Type *uart, uint32_t prescale)
{
    LL_REG_WRITE(uart->PRE, prescale & UART_PRE);
}

/**
 * @brief  This function is used to get Prescale value
 * @note   This 5-bit register adds a second programmable division factor to obtain
 * the desired baud rate. The division factor is the value hold in this register, so
 * the maximum factor is 31 and the minimum is 0. When PRE is 0, there is no input
 * clock to divisor latch unit. Therefore, programming DLL and DLM is useless when PRE is 0
 * @param  uart UART Instance
 * @retval Prescale value
 */
__STATIC_INLINE uint32_t LL_UART45_GetPrescaleValue(SN_UART4_Type *uart)
{
    return (LL_REG_READ(uart->PRE) & UART_PRE);
}

/**
 * @brief  This function is used to set word length selection
 * @note   0: 5-bit character
 *         1: 6-bit character
 *         2: 7-bit character
 *         3: 8-bit character
 * @param  uart UART Instance
 * @param  length word length @ref uart_word_length
 * @retval none
 */
__STATIC_INLINE void LL_UART45_SetWordLength(SN_UART4_Type *uart, uint32_t length)
{
    LL_REG_CBIT(uart->LC, UART_LC_WLS);
    LL_REG_SBIT(uart->LC, length & UART_LC_WLS);
}

/**
 * @brief  This function is used to get word length selection
 * @note   0: 5-bit character
 *         1: 6-bit character
 *         2: 7-bit character
 *         3: 8-bit character
 * @param  uart UART Instance
 * @retval word length selection @ref uart_word_length
 */
__STATIC_INLINE uint32_t LL_UART45_GetWordLength(SN_UART4_Type *uart)
{
    return (LL_REG_READ(uart->LC) & UART_LC_WLS);
}

/**
 * @brief  This function is used to set stop bit
 * @note   0: 1 stop bit
 *         1: 2 stop bit (1.5 stop bit if WLS=0)
 * @param  uart UART Instance
 * @param  stop_bit stop bits @ref uart_stop_bits
 * @retval none
 */
__STATIC_INLINE void LL_UART45_SetStopBits(SN_UART4_Type *uart, uint32_t stop_bit)
{
    LL_REG_CBIT(uart->LC, UART_LC_SBS);
    LL_REG_SBIT(uart->LC, stop_bit & UART_LC_SBS);
}

/**
 * @brief  This function is used to get stop bit
 * @note   0: 1 stop bit
 *         1: 2 stop bit (1.5 stop bit if WLS=0)
 * @param  uart UART Instance
 * @retval stop bit @ref uart_stop_bits
 */
__STATIC_INLINE uint32_t LL_UART45_GetStopBits(SN_UART4_Type *uart)
{
    return (LL_REG_READ(uart->LC) & UART_LC_SBS);
}

/**
 * @brief  This function is used to set Parity
 * @note   0: Odd parity. Number of 1s in the transmitted character and the attached parity bit will be odd.
 *         1: Even parity. Number of 1s in the transmitted character and the attached parity bit will be even.
 * @param  uart UART Instance
 * @param  parity parity mode @ref uart_parity
 * @retval none
 */
__STATIC_INLINE void LL_UART45_SetParity(SN_UART4_Type *uart, uint32_t parity)
{
    LL_REG_CBIT(uart->LC, UART_LC_PS | UART_LC_PE);
    LL_REG_SBIT(uart->LC, parity & (UART_LC_PS | UART_LC_PE));
}

/**
 * @brief  This function is used to get Parity
 * @note   0: Odd parity. Number of 1s in the transmitted character and the attached parity bit will be odd.
 *         1: Even parity. Number of 1s in the transmitted character and the attached parity bit will be even.
 * @param  uart UART Instance
 * @retval Parity @ref uart_parity
 */
__STATIC_INLINE uint32_t LL_UART45_GetParity(SN_UART4_Type *uart)
{
    return (LL_REG_READ(uart->LC) & (UART_LC_PS | UART_LC_PE));
}

/**
 * @brief  This function is used to enable Break Control
 * @note    Enable. UTXD is forced to logic 0. If several characters are stored in the
 *          transmit FIFO, they will be removed from this FIFO and passed sequentially
 *          to the Transmitter Shift register which serializes them, even if BC bit is set.
 *          This fact can be useful to establish the break time making use of the THRE
 *          and TEMT flags in the UARTn_LS register. Firmware can follow the
 *          sequence below to assure no erroneous or extraneous characters will be
 *          transmitted because of the break:
 *          - Set break when transmitter is idle
 *          - Write a character with any value to TH.
 *          - Wait for the transmitter to become idle
 *          - Clear break when normal transmission has to be restored
 * @param  uart UART Instance
 * @retval none
 */
__STATIC_INLINE void LL_UART45_Enable_BreakControl(SN_UART4_Type *uart)
{
    LL_REG_SBIT(uart->LC, UART_LC_BC);
}

/**
 * @brief  This function is used to disable Break Control
 * @param  uart UART Instance
 * @retval none
 */
__STATIC_INLINE void LL_UART45_Disable_BreakControl(SN_UART4_Type *uart)
{
    LL_REG_CBIT(uart->LC, UART_LC_BC);
}

/**
 * @brief  This function is used to get Break Control status
 * @note    0:Disable.
 *          1:Enable. UTXD is forced to logic 0. If several characters are stored in the
 *          transmit FIFO, they will be removed from this FIFO and passed sequentially
 *          to the Transmitter Shift register which serializes them, even if BC bit is set.
 *          This fact can be useful to establish the break time making use of the THRE
 *          and TEMT flags in the UARTn_LS register. Firmware can follow the
 *          sequence below to assure no erroneous or extraneous characters will be
 *          transmitted because of the break:
 *          - Set break when transmitter is idle
 *          - Write a character with any value to TH.
 *          - Wait for the transmitter to become idle
 *          - Clear break when normal transmission has to be restored
 * @param  uart UART Instance
 * @retval Break Control status
 */
__STATIC_INLINE uint32_t LL_UART45_IsEnabled_BreakControl(SN_UART4_Type *uart)
{
    return (LL_REG_READ(uart->LC) & UART_LC_BC);
}

/**
 * @brief  This function is used to enable Divisor Latch access
 * @note   0: Disable access to Divisor Latch
 *         1: Enable access to Divisor Latch
 * @param  uart UART Instance
 * @retval none
 */
__STATIC_INLINE void LL_UART45_Enable_DivisorLatchAccess(SN_UART4_Type *uart)
{
    LL_REG_SBIT(uart->LC, UART_LC_DLAB);
}

/**
 * @brief  This function is used to disable Divisor Latch access
 * @note   0: Disable access to Divisor Latch
 *         1: Enable access to Divisor Latch
 * @param  uart UART Instance
 * @retval none
 */
__STATIC_INLINE void LL_UART45_Disable_DivisorLatchAccess(SN_UART4_Type *uart)
{
    LL_REG_CBIT(uart->LC, UART_LC_DLAB);
}

/**
 * @brief  This function is used to get Divisor Latch access status
 * @note   0: Disable access to Divisor Latch
 *         1: Enable access to Divisor Latch
 * @param  uart UART Instance
 * @retval Divisor Latch access status
 */
__STATIC_INLINE uint32_t LL_UART45_IsEnabled_DivisorLatchAccess(SN_UART4_Type *uart)
{
    return (LL_REG_READ(uart->LC) & UART_LC_DLAB);
}

/**
 * @brief  This function is used to enable Loopback mode
 * @note   When UART is set in UART loopback mode, UTXDn/URXDn/UCTSn/URTSn
 * pins are not used and UTXDn pin is set to inactive state.
 * When UART is set in SIR loopback mode, IRDA_RXDL pin is not used but
 * IRDA_TXDn pin is still in active state
 * @param  uart UART Instance
 * @retval none
 */
__STATIC_INLINE void LL_UART45_Enable_LoopbackMode(SN_UART4_Type *uart)
{
    LL_REG_SBIT(uart->MC, UART_MC_LMS);
}

/**
 * @brief  This function is used to disable Loopback mode
 * @note   When UART is set in UART loopback mode, UTXDn/URXDn/UCTSn/URTSn
 * pins are not used and UTXDn pin is set to inactive state.
 * When UART is set in SIR loopback mode, IRDA_RXDL pin is not used but
 * IRDA_TXDn pin is still in active state
 * @param  uart UART Instance
 * @retval none
 */
__STATIC_INLINE void LL_UART45_Disable_LoopbackMode(SN_UART4_Type *uart)
{
    LL_REG_CBIT(uart->MC, UART_MC_LMS);
}

/**
 * @brief  This function is used to get Loopback mode status
 * @note   When UART is set in UART loopback mode, UTXDn/URXDn/UCTSn/URTSn
 * pins are not used and UTXDn pin is set to inactive state.
 * When UART is set in SIR loopback mode, IRDA_RXDL pin is not used but
 * IRDA_TXDn pin is still in active state
 * @param  uart UART Instance
 * @retval Loopback mode status
 */
__STATIC_INLINE uint32_t LL_UART45_IsEnabled_LoopbackMode(SN_UART4_Type *uart)
{
    return (LL_REG_READ(uart->MC) & UART_MC_LMS);
}

/**
 * @brief  Get Line Status
 * @param  uart UART Instance
 * @retval Line Status
 */
__STATIC_INLINE uint32_t LL_UART45_GetLineStatus(SN_UART4_Type *uart)
{
    return (REG_READ(uart->LS));
}

/**
 * @brief  This function is used to get Receiver data ready flag
 * @param  uart UART Instance
 * @retval Receiver data ready flag
 */
__STATIC_INLINE uint32_t LL_UART45_IsActiveFlag_RDR(SN_UART4_Type *uart)
{
    return (LL_REG_READ(uart->LS) & UART_LS_RDR);
}

/**
 * @brief  This function is used to get Overrun error flag
 * @note   The overrun error condition is set as soon as it occurs. A UARTn_LS register
 * read clears OE bit. OE=1 when UART RSR has a new character assembled
 * and the UARTn_RB FIFO is full. In this case, the UARTn_RB FIFO will not
 * be overwritten and the character in the UARTn_RS register will be lost
 * @param  uart UART Instance
 * @retval Overrun error flag
 */
__STATIC_INLINE uint32_t LL_UART45_IsActiveFlag_OE(SN_UART4_Type *uart)
{
    return (LL_REG_READ(uart->LS) & UART_LS_OE);
}

/**
 * @brief  This function is used to get Parity error flag
 * @note   When the parity bit of a received character is in the wrong state, a parity error
 * occurs. A UARTn_LS register read clears PE bit. Time of parity error
 * detection is dependent on FIFOEN bit in UARTn_FIFOCTRL register.
 * @note   A parity error is associated with the character at the top of the
 * UARTn_RB FIFO
 * @param  uart UART Instance
 * @retval Parity error flag
 */
__STATIC_INLINE uint32_t LL_UART45_IsActiveFlag_PE(SN_UART4_Type *uart)
{
    return (LL_REG_READ(uart->LS) & UART_LS_PE);
}

/**
 * @brief  This function is used to get Framing error flag
 * @note   When the stop bit of a received character is logic 0, a framing error occurs. A
 * UARTn_LS register read clears FE bit. The time of the framing error
 * detection is dependent on FIFOEN bit in UARTn_FIFOCTRL register.
 * Upon detection of a framing error, the RX will attempt to re-synchronize to
 * the data and assume that the bad stop bit is actually an early start bit.
 * However, it cannot be assumed that the next received byte will be correct
 * even if there is no Framing Error
 * @note   A framing error is associated with the character at the top of the
 * UARTn_RB FIFO
 * @param  uart UART Instance
 * @retval Framing error flag
 */
__STATIC_INLINE uint32_t LL_UART45_IsActiveFlag_FE(SN_UART4_Type *uart)
{
    return (LL_REG_READ(uart->LS) & UART_LS_FE);
}

/**
 * @brief  This function is used to get Break interrupt flag
 * @note   When RXD1 is held in the spacing state (all zeros) for one full character
 * transmission (start, data, parity, stop), a break interrupt occurs. Once the
 * break condition has been detected, the receiver goes idle until RXD1 goes to
 * marking state (all ones). A UARTn_LS register read clears BI bit. The time of
 * break detection is dependent on FIFOEN bit in UARTn_FIFOCTRL register
 * @note   The break interrupt is associated with the character at the top of the
 * UARTn_RB FIFO
 * @param  uart UART Instance
 * @retval Break interrupt flag
 */
__STATIC_INLINE uint32_t LL_UART45_IsActiveFlag_BI(SN_UART4_Type *uart)
{
    return (LL_REG_READ(uart->LS) & UART_LS_BI);
}

/**
 * @brief  This function is used to get THR empty flag
 * @note   THRE indicates that the UART is ready to accept a new character for
 * transmission. In addition, this bit causes the UART to issue THRE interrupt to
 * if THREIE=1. THRE=1 when a character is transferred from the THR into the
 * TSR. The bit is reset to logic 0 concurrently with the loading of the
 * Transmitter Holding Register by the CPU.
 * @param  uart UART Instance
 * @retval THR empty flag
 */
__STATIC_INLINE uint32_t LL_UART45_IsActiveFlag_THRE(SN_UART4_Type *uart)
{
    return (LL_REG_READ(uart->LS) & UART_LS_THRE);
}

/**
 * @brief  This function is used to get Transmitter Empty flag
 * @note   0: THR and/or TSR contains valid data
 *         1: THR and TSR are both empty
 * @param  uart UART Instance
 * @retval Transmitter Empty flag
 */
__STATIC_INLINE uint32_t LL_UART45_IsActiveFlag_TEMT(SN_UART4_Type *uart)
{
    return (LL_REG_READ(uart->LS) & UART_LS_TEMT);
}

/**
 * @brief  This function is used to get Receiver FIFO error flag
 * @note   0: UARTn_RB contains no UART RX errors or FIFOEN=0
 *         1: UARTn_RB contains at least 1 UART RX error. The character with a RX
 *          error such as framing error, parity error, or break interrupt, is loaded into the
 *          UARTn_RB register. This bit is cleared when the UARTn_LS register is read
 *          and there are no subsequent errors in the UART FIFO
 * @param  uart UART Instance
 * @retval Receiver FIFO error flag
 */
__STATIC_INLINE uint32_t LL_UART45_IsActiveFlag_RXFE(SN_UART4_Type *uart)
{
    return (LL_REG_READ(uart->LS) & UART_LS_RXFE);
}

/**
 * @brief  This function is used to set Pad Information
 * @param  uart UART Instance
 * @param  pad Pad Information
 * @retval none
 */
__STATIC_INLINE void LL_UART45_SetPadInformation(SN_UART4_Type *uart, uint32_t pad)
{
    LL_REG_WRITE(uart->SP, pad);
}

/**
 * @brief  This function is used to get Pad Information
 * @param  uart UART Instance
 * @retval Pad Information
 */
__STATIC_INLINE uint32_t LL_UART45_GetPadInformation(SN_UART4_Type *uart)
{
    return (LL_REG_READ(uart->SP) & UART_SP_PID);
}

/**
 * @brief  This function is used to set UART mode
 * @note   0: UART mode
 *         1: IRDA SIR mode
 *         Other: Reserved
 * @param  uart UART Instance
 * @param  mode uart mode @ref uart_mode
 * @retval none
 */
__STATIC_INLINE void LL_UART45_SetUARTMode(SN_UART4_Type *uart, uint32_t mode)
{
    LL_REG_CBIT(uart->MD, UART_MD_MODE);
    LL_REG_SBIT(uart->MD, (mode << UART_MD_MODE_Pos) & UART_MD_MODE);
}

/**
 * @brief  This function is used to get UART mode
 * @note   0: UART mode
 *         1: IRDA SIR mode
 *         Other: Reserved
 * @param  uart UART Instance
 * @retval real UART mode @ref uart_mode
 */
__STATIC_INLINE uint32_t LL_UART45_GetUARTMode(SN_UART4_Type *uart)
{
    return ((LL_REG_READ(uart->MD) & UART_MD_MODE)) >> UART_MD_MODE_Pos;
}

/**
 * @brief  This function is used to set IRDA SIR pulse invert
 * @note   0: IRDA SIR pulse is not inverted
 *         1: IRDA SIR pulse is inverted
 * @param  uart UART Instance
 * @param  pulse IRDA SIR pulse invert @ref uart_IRDA_SIR_pulse_invert
 * @retval none
 */
__STATIC_INLINE void LL_UART45_SetIRDASIRPulseInvert(SN_UART4_Type *uart, uint32_t pulse)
{
    LL_REG_CBIT(uart->MD, UART_MD_IRDAINVTX);
    LL_REG_SBIT(uart->MD, (pulse << UART_MD_IRDAINVTX_Pos) & UART_MD_IRDAINVTX);
}

/**
 * @brief  This function is used to set IRDA SIR pulse invert
 * @note   0: IRDA SIR pulse is not inverted
 *         1: IRDA SIR pulse is inverted
 * @param  uart UART Instance
 * @retval IRDA SIR pulse invert @ref uart_IRDA_SIR_pulse_invert
 */
__STATIC_INLINE uint32_t LL_UART45_GetIRDASIRPulseInvert(SN_UART4_Type *uart)
{
    return ((LL_REG_READ(uart->MD) & UART_MD_IRDAINVTX) >> UART_MD_IRDAINVTX_Pos);
}

/**
 * @brief  This function is used to enable IrDA TX
 * @param  uart UART Instance
 * @retval none
 */
__STATIC_INLINE void LL_UART45_Enable_TX(SN_UART4_Type *uart)
{
    LL_REG_SBIT(uart->AUX, UART_AUX_TXEN);
}

/**
 * @brief  This function is used to disable IrDA TX
 * @param  uart UART Instance
 * @retval none
 */
__STATIC_INLINE void LL_UART45_Disable_TX(SN_UART4_Type *uart)
{
    LL_REG_CBIT(uart->AUX, UART_AUX_TXEN);
}

/**
 * @brief  This function is used to get IrDA TX enable status
 * @param  uart UART Instance
 * @retval IrDA TX enable status
 */
__STATIC_INLINE uint32_t LL_UART45_IsEnabled_TX(SN_UART4_Type *uart)
{
    return (LL_REG_READ(uart->AUX) & UART_AUX_TXEN);
}

/**
 * @brief  This function is used to enable IrDA RX
 * @param  uart UART Instance
 * @retval none
 */
__STATIC_INLINE void LL_UART45_Enable_RX(SN_UART4_Type *uart)
{
    LL_REG_SBIT(uart->AUX, UART_AUX_RXEN);
}

/**
 * @brief  This function is used to disable IrDA RX
 * @param  uart UART Instance
 * @retval none
 */
__STATIC_INLINE void LL_UART45_Disable_RX(SN_UART4_Type *uart)
{
    LL_REG_CBIT(uart->AUX, UART_AUX_RXEN);
}

/**
 * @brief  This function is used to get IrDA RX enable status
 * @param  uart UART Instance
 * @retval IrDA RX enable status
 */
__STATIC_INLINE uint32_t LL_UART45_IsEnabled_RX(SN_UART4_Type *uart)
{
    return (LL_REG_READ(uart->AUX) & UART_AUX_RXEN);
}

/**
 * @brief  This function is used to set IRDA SIR pulse width
 * @note   0: Pulse width = 3/16
 *         1: Pulse width = 3/ PB16XCLK (When PB16XCLK = 1.8432MHz, pulse width = 1.6us)
 * @param  uart UART Instance
 * @param  width IRDA SIR pulse width  @ref uart_IRDA_SIR_pulse_width
 * @retval none
 */
__STATIC_INLINE void LL_UART45_SetIRDASIRPulseWidth(SN_UART4_Type *uart, uint32_t width)
{
    LL_REG_CBIT(uart->AUX, UART_AUX_SIRPW);
    LL_REG_SBIT(uart->AUX, (width << UART_AUX_SIRPW_Pos) & UART_AUX_SIRPW);
}

/**
 * @brief  This function is used to get IRDA SIR pulse width
 * @note   0: Pulse width = 3/16
 *         1: Pulse width = 3/ PB16XCLK (When PB16XCLK = 1.8432MHz, pulse width = 1.6us)
 * @param  uart UART Instance
 * @retval IRDA SIR pulse width   @ref uart_IRDA_SIR_pulse_width
 */
__STATIC_INLINE uint32_t LL_UART45_GetIRDASIRPulseWidth(SN_UART4_Type *uart)
{
    return ((LL_REG_READ(uart->AUX) & UART_AUX_SIRPW) >> UART_AUX_SIRPW_Pos);
}

/**
 * @brief  This function is used to get The number of data bytes in RX FIFO
 * @param  uart UART Instance
 * @retval The number of data bytes in RX FIFO
 */
__STATIC_INLINE uint32_t LL_UART45_GetRXFIFOCount(SN_UART4_Type *uart)
{
    return (LL_REG_READ(uart->RXFFCNT) & UART_RXFFCNT_COUNT);
}

#ifdef __cplusplus
}
#endif

#endif /* _LL_UART45_H_ */
