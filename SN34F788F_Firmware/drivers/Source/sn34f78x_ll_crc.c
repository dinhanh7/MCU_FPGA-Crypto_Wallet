/**
 * @file sn34f7_ll_crc.c
 * @author PD
 * @brief CRC LL driver.
 *          This file provides firmware functions to manage the following
 *          functionalities of the CRC peripheral:
 *              1. support
 *                  CRC-32 polynomial: X32+X26+X23+X22+X16+X12+X11+X10+X8+X7+X5+X4+X2+X+1
 *                  CRC-16 polynomial: X16+X15+X2+1
 *                  CRC-16-CCITT polynomial: X16+X12+X5+1
 *              2. handles 16-bit, 32-bit data size
 *              3. single input/output 32-bit data register
 *              4. input buffer to avoid bus stall during calculation
 *              5. CRC computation done in 4 IHRC clock cycles for the 32-bit data size
 *              6. polynomial representations of cyclic redundancy checks
 *
 * @version 1.0
 * @date 2024-03-21
 *
 * @copyright Copyright (c) 2024
 *
 */
#include "sn34f78x_ll_crc.h"

#if defined(USE_FULL_LL_DRIVER)
/**
 * @brief  Initialize CRC registers according to the specified parameters in crc_init.
 * @param  crc CRC Instance
 * @param  crc_init pointer to a @ref LL_CRC_Init_T structure
 *         that contains the configuration information for the specified CRC peripheral.
 * @retval An ErrorStatus enumeration value:
 *           - LL_OK:     CRC registers are initialized according to crc_init content
 *           - LL_ERROR:  Not applicable
 */
LL_Status_T LL_CRC_Init(SN_CRC_Type *crc, LL_CRC_Init_T *crc_init)
{
    /* Check the instance */
    RET_FLAG_FALSE(IS_LL_CRC_INSTANCE(crc), LL_ERROR);

    /* Check the parameters */
    RET_FLAG_FALSE(IS_LL_CRC_POLY(crc_init->polynomial), LL_ERROR);

    return LL_OK;
}

/**
 * @brief  De-initialize CRC registers (Registers restored to their default values).
 * @param  crc CRC Instance
 * @retval An ErrorStatus enumeration value:
 *           - LL_OK:     CRC registers are de-initialized
 *           - LL_ERROR:  Wrong CRC Instance
 */
LL_Status_T LL_CRC_DeInit(SN_CRC_Type *crc)
{
    /* Check the instance */
    RET_FLAG_FALSE(IS_LL_CRC_INSTANCE(crc), LL_ERROR);

    LL_CRC_SetPoly(crc, LL_CRC_POLY_CRC_16_CCITT);

    return LL_OK;
}

/**
 * @brief  Set each @ref LL_CRC_Init_T field to default value.
 * @param  crc_init pointer to a @ref LL_CRC_Init_T structure  whose fields will be set to default values.
 * @retval None
 */
void LL_CRC_StructInit(LL_CRC_Init_T *crc_init)
{
    if (crc_init != NULL)
    {
        crc_init->polynomial = LL_CRC_POLY_CRC_16_CCITT;
    }
}
#endif /* USE_FULL_LL_DRIVER */
