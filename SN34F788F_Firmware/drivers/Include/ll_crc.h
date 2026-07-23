/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _LL_CRC_H_
#define _LL_CRC_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sn34f78x_ll_def.h"
#include "ll_reg_msk.h"

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
/**
 * \defgroup crc_polynomial CRC Polynomial
 * \ingroup crc_control
 * @{
 */
#define LL_CRC_POLY_CRC16CCITT CRC_CTRL_POLY_CRC_16_CCITT /**< CRC Polynomial CRC-16-CCITT */
#define LL_CRC_POLY_CRC16      CRC_CTRL_POLY_CRC_16       /**< CRC Polynomial CRC-16 */
#define LL_CRC_POLY_CRC32      CRC_CTRL_POLY_CRC_32       /**< CRC Polynomial CRC-32 */
/**
 * @}
 */

/* Exported functions --------------------------------------------------------*/
/**
 * @brief  Set the CRC polynomial.
 * @param  polynomial This parameter can be one of @ref crc_polynomial
 * @retval None
 */
__STATIC_INLINE void LL_CRC_SetPolynomial(uint32_t polynomial)
{
    LL_REG_CBIT(SN_CRC->CTRL, CRC_CTRL_POLY);
    LL_REG_SBIT(SN_CRC->CTRL, polynomial);
}

/**
 * @brief  Return the CRC polynomial.
 * @retval CRC Polynomial, \ref crc_polynomial
 */
__STATIC_INLINE uint32_t LL_CRC_GetPolynomial(void)
{
    return LL_REG_RBIT(SN_CRC->CTRL, CRC_CTRL_POLY);
}

/**
 * @brief  Write given 32-bit data to the CRC calculator.
 * @param  data value to be provided to CRC calculator between 0 and 0xFFFFFFFF
 * @retval None
 */
__STATIC_INLINE void LL_CRC_FeedData32(uint32_t data)
{
    LL_REG_WRITE(SN_CRC->DATA, data);
}

/**
 * @brief  Return current CRC calculation result. 32 bits value is returned.
 * @retval Current CRC calculation result as stored in CRC_Data register (32 bits).
 */
__STATIC_INLINE uint32_t LL_CRC_ReadData32(void)
{
    return LL_REG_READ(SN_CRC->DATA);
}

/**
 * @brief  Reset the initial seed value and BUSY bit to 0.
 * @retval None
 */
__STATIC_INLINE void LL_CRC_Reset(void)
{
    LL_REG_SBIT(SN_CRC->CTRL, CRC_CTRL_RESET);
}

/**
 * @brief  Return if CRC reset is ongoing.
 * @retval State of bit (1 or 0).
 */
__STATIC_INLINE uint32_t LL_CRC_IsResetOngoing(void)
{
    return (LL_REG_RBIT(SN_CRC->CTRL, CRC_CTRL_RESET) == (CRC_CTRL_RESET));
}

/**
 * @brief  Return if CRC calculation is in process.
 * @retval State of bit (1 or 0).
 */
__STATIC_INLINE uint32_t LL_CRC_IsBusy(void)
{
    return (LL_REG_RBIT(SN_CRC->CTRL, CRC_CTRL_BUSY) == (CRC_CTRL_BUSY));
}

#ifdef __cplusplus
}
#endif

#endif /* _LL_CRC_H_ */
