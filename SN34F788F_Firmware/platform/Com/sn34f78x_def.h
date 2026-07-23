#ifndef _SN34F78X_DEF_H_
#define _SN34F78X_DEF_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SN34F780.h>

/* Exported types ------------------------------------------------------------*/
/**
 * \brief Define unused
 * \ingroup com_macro
 */
#ifndef UNUSED
#define UNUSED(x) ((void)(x)) /**< Unused symbol */
#endif

/**
 * \brief Define ignore
 * \ingroup com_macro
 */
#define IGNORE 0xFFFFFFFFU /**< Ignore parameter */

/**
 * \brief Define the common flag check macros
 * \ingroup com_macro
 * @{
 */
#define FLAG_EQU(flag1, flag2) ((flag1) == (flag2))                   /**< 'flag1' == 'flag2' */
#define FLAG_NEQ(flag1, flag2) ((flag1) != (flag2))                   /**< 'flag1' != 'flag2' */
#define FLAG_HAS(flag, e)      (((flag) & (e)) == (e) ? true : false) /**< check current 'flag' whether has 'e' */
#define FLAG_LACKS(flag, e)    (((flag) & (e)) == 0 ? true : false)   /**< check current 'flag' whether lacks 'e' */
#define FLAG_SET(flag, e)      ((flag) |= (e))                        /**< set 'flag' with 'e' */
#define FLAG_CLR(flag, e)      ((flag) &= ~(e))                       /**< clear 'e' from 'flag' */
#define FLAG_RESET(flag)       ((flag) = (0))                         /**< zero 'flag' content */
#define FLAG_WRITE(flag, v)    ((flag) = (v))                         /**< write 'v' to  'flag' */
#define FLAG_AND(flag, e)      ((flag) & (e))                         /**< do 'AND' operator between 'flag' and 'e' */
/**
 * @}
 */

/**
 * \brief Define the common register operation macros
 * \ingroup com_macro
 * @{
 */
#define REG_RBIT(reg, bit)      ((reg) & (bit))                              /**< read register bit */
#define REG_SBIT(reg, bit)      ((reg) |= (bit))                             /**< set register bit */
#define REG_CBIT(reg, bit)      ((reg) &= ~(bit))                            /**< clear register bit */
#define REG_MBIT(reg, msk, bit) ((reg) = ((reg) & ~(msk)) | ((bit) & (msk))) /**< modify register with bits */
#define REG_READ(reg)           ((reg))                                      /**< read register */
#define REG_WRITE(reg, val)     ((reg) = (val))                              /**< write val to register */
#define REG_CLEAR(reg)          ((reg) = (0x0))                              /**< clear register */
#define POSITION_VAL(VAL)       (__CLZ(__RBIT(VAL)))                         /**< calc current bit index */
/**
 * @}
 */

/**
 * \brief Macro for attribute
 * \ingroup com_macro
 * @{
 */
#define ATTR_WEAK     __attribute__((weak))           /**< function add weak attr */
#define ATTR_PACKED   __attribute__((packed))         /**< disable compiler optimizer about align in struct */
#define ATTR_ISR      __attribute__((section("ISR"))) /**< forces code into "ISR" section */
#define ATTR_ALIGN(n) __attribute__((aligned(n)))     /**< forces data align n byte */
#define ATTR_SEC(sec) __attribute__((section(sec)))   /**< forces code into section */
#define ATTR_IRQ      __attribute__((interrupt))      /**< for arm compiler 6.0 later IRQ */
/**
 * @}
 */

/**
 * \brief Return by flag
 * \ingroup com_macro
 * @{
 */
#define RET_FLAG_HAS(flag, e, res) \
    if (FLAG_HAS(flag, e))         \
    {                              \
        return res;                \
    } /**< If current flag has e, return the 'res' from process */

#define RET_FLAG_LACKS(flag, e, res) \
    if (FLAG_LACKS(flag, e))         \
    {                                \
        return res;                  \
    } /**< If current flag is lack of e, return the 'res' from process */

#define RET_FLAG_TRUE(flag, res) \
    if (flag)                    \
    {                            \
        return res;              \
    } /**< If current flag is true, return the 'res' from process */

#define RET_FLAG_FALSE(flag, res) \
    if (!(flag))                  \
    {                             \
        return res;               \
    } /**< If current flag is false, return the 'res' from process */
/**
 * @}
 */

#if defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
#define __weak __WEAK
#define __irq
#endif

#ifdef __cplusplus
}
#endif

#endif /* _SN34F78X_DEF_H_ */
