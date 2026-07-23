#ifndef _SN34F78X_LL_DEF_H_
#define _SN34F78X_LL_DEF_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sn34f78x_def.h"

/* Exported types --------------------------------------------------------*/
/**
 * \brief Define the return status for LL API calling.
 * \ingroup execution_status
 */
typedef enum
{
    LL_OK      = 0x00U, /**< Operation succeeded */
    LL_ERROR   = 0x01U, /**< Operation failed */
    LL_BUSY    = 0x02U, /**< Driver is busy */
    LL_TIMEOUT = 0x03U, /**< Operation timeout */
} LL_Status_T;

/* Exported constants --------------------------------------------------------*/
/**
 * \brief Define LL enable / disable value
 * \ingroup ll_macro
 * @{
 */
#define LL_DISABLE 0x0U /**< General disable value */
#define LL_ENABLE  0x1U /**< General enable  value */
/**
 * @}
 */

/**
 * \brief LL max delay
 * \ingroup ll_macro
 * @{
 */
#define LL_MAX_DELAY 0xFFFFFFFFU /**< Max delay for timeout usage */
/**
 * @}
 */

/**
 * \brief Define the LL flag check macros
 * \ingroup ll_macro
 * @{
 */
#define LL_FLAG_EQU(flag1, flag2) FLAG_EQU(flag1, flag2) /**< 'flag1' == 'flag2' */
#define LL_FLAG_NEQ(flag1, flag2) FLAG_NEQ(flag1, flag2) /**< 'flag1' != 'flag2' */
#define LL_FLAG_HAS(flag, e)      FLAG_HAS(flag, e)      /**< check current 'flag' whether has 'e' */
#define LL_FLAG_LACKS(flag, e)    FLAG_LACKS(flag, e)    /**< check current 'flag' whether lacks 'e' */
#define LL_FLAG_SET(flag, e)      FLAG_SET(flag, e)      /**< set 'flag' with 'e' */
#define LL_FLAG_CLR(flag, e)      FLAG_CLR(flag, e)      /**< clear 'e' from 'flag' */
#define LL_FLAG_RESET(flag)       FLAG_RESET(flag)       /**< zero 'flag' content */
#define LL_FLAG_WRITE(flag, v)    FLAG_WRITE(flag, v)    /**< write 'v' to  'flag' */
#define LL_FLAG_AND(flag, e)      FLAG_AND(flag, e)      /**< do 'AND' operator between 'flag' and 'e' */
/**
 * @}
 */

/**
 * \brief Define the LL register operation macros
 * \ingroup ll_macro
 * @{
 */
#define LL_REG_RBIT(reg, bit)      REG_RBIT(reg, bit)      /**< read register bit */
#define LL_REG_SBIT(reg, bit)      REG_SBIT(reg, bit)      /**< set register bit */
#define LL_REG_CBIT(reg, bit)      REG_CBIT(reg, bit)      /**< clear register bit */
#define LL_REG_MBIT(reg, msk, bit) REG_MBIT(reg, msk, bit) /**< modify register with bits */
#define LL_REG_READ(reg)           REG_READ(reg)           /**< read register */
#define LL_REG_WRITE(reg, val)     REG_WRITE(reg, val)     /**< write val to register */
#define LL_REG_CLEAR(reg)          REG_CLEAR(reg)          /**< clear register */
/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* _SN34F78X_LL_DEF_H_ */
