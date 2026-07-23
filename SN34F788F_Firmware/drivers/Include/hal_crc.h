/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _HAL_CRC_H_
#define _HAL_CRC_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sn34f78x_hal_def.h"

/* Exported types ------------------------------------------------------------*/
/**
 * @brief CRC Instance definition
 * \ingroup crc_struct_define
 */
typedef struct
{
    uint32_t polynomial; /**< Specifies the CRC calculation polynomial value.
                              This parameter can be a value of @ref crc_polynomial */
} CRC_Init_T;

/**
 * @brief  CRC Handle Structure definition
 * \ingroup crc_struct_define
 */
typedef struct __CRC_Handle
{
    SN_CRC_Type *instance;   /**< Register Base Address */
    CRC_Init_T   init;       /**< CRC Required Parameters */
    HAL_MUTEX    mutex;      /**< CRC Process Protection */
    HAL_State_T  state;      /**< CRC State */
    uint32_t     error_code; /**< CRC Error Code */
} CRC_Handle_T;

/* Exported constants --------------------------------------------------------*/
/**
 * \defgroup crc_polynomial CRC Polynomial
 * \ingroup crc_control
 * @{
 */
#define CRC_POLY_CRC_16_CCITT 0U /**< CRC Polynomial CRC-16-CCITT */
#define CRC_POLY_CRC_16       1U /**< CRC Polynomial CRC-16 */
#define CRC_POLY_CRC_32       2U /**< CRC Polynomial CRC-32 */
/**
 * @}
 */

/* Exported functions --------------------------------------------------------*/
/* Initialization/de-initialization functions  ********************************/
HAL_Status_T HAL_CRC_Init(CRC_Handle_T *hcrc);
HAL_Status_T HAL_CRC_DeInit(CRC_Handle_T *hcrc);
void         HAL_CRC_MspInit(CRC_Handle_T *hcrc);
void         HAL_CRC_MspDeInit(CRC_Handle_T *hcrc);

/* Peripheral Control functions ***********************************************/
HAL_Status_T HAL_CRC_UpdatePoly(CRC_Handle_T *hcrc, uint32_t polynomial);

/* IO Operation functions *****************************************************/
uint32_t HAL_CRC_Calculate(CRC_Handle_T *hcrc, uint8_t *buf, uint32_t buf_size);

/* Peripheral State and Errors functions **************************************/
HAL_State_T HAL_CRC_GetState(CRC_Handle_T *hcrc);
int32_t     HAL_CRC_GetError(CRC_Handle_T *hcrc);

#ifdef __cplusplus
}
#endif

#endif /* _HAL_CRC_H_ */
