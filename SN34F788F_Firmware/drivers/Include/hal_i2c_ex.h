/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _HAL_I2C_EX_H_
#define _HAL_I2C_EX_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sn34f78x_hal_def.h"

/* Exported types ------------------------------------------------------------*/
/**
 * \brief I2C Extended Transfer Structure Definition
 * \ingroup i2c_struct_define
 */
typedef struct
{
    uint32_t       en : 1;        /**< I2C Slave Extended Transfer Enablement */
    uint8_t       *tx_buffer_ptr; /**< Pointer to I2C TX Transfer Buffer */
    uint16_t       tx_xfer_size;  /**< I2C TX Transfer Size */
    __IOM uint16_t tx_xfer_count; /**< I2C TX Transfer Counter */
    __IOM uint16_t tx_xfer_num;   /**< I2C TX Transfer Number */
    uint8_t       *rx_buffer_ptr; /**< Pointer to I2C RX Transfer Buffer */
    uint16_t       rx_xfer_size;  /**< I2C RX Transfer Size */
    __IOM uint16_t rx_xfer_count; /**< I2C RX Transfer Counter */
    __IOM uint16_t rx_xfer_num;   /**< I2C RX Transfer Number */
} I2CEx_XFER_T;

#ifdef __cplusplus
}
#endif

#endif /* _HAL_I2C_EX_H_ */
