/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _NORFlash_MX25L6433F_H_
#define _NORFlash_MX25L6433F_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sn34f78x_hal_def.h"
#include "hal_gpio.h"

/**
 * @defgroup norflash_struct NORFlash_MX25L6433F_Init_T
 * \ingroup norflash_max25l6433f_struct
 * @{
 */
typedef struct _NORFlash_Init_T
{
    SN_SPI0_Type *instance; /**< NOR registers base address                */

    uint32_t fs_polarity;      /**< Specifies the Frame/Sync. polarity for the SPI mode.
                                    This parameter can be a value of @ref norflash_fs_polarity */
    uint32_t clk_polarity;     /**< Specify the serial clock steady state.
                                    This parameter can be a value of @ref norflash_clk_polarity */
    uint32_t clk_phase;        /**< Specify the clock active edge for the bit capture.
                                    This parameter can be a value of @ref norflash_clk_phase */
    uint32_t sclk_div;         /**< Specify the communication clock division value which will be
                                    used to configure the SCLK of NOR FLASH. */
    uint32_t first_bit;        /**< Specify whether MSB or LSB of the data will be transmitted or received first.
                                    This parameter can be a value of @ref norflash_first_bit */
    SN_GPIO_Type cs_gpio_port; /**< Specify the chip select pin of NOR FLASH is controlled by which gpio port.
                                    This parameter can be a value of SN_GPIOx (x = 0,1,2,3) */
    uint32_t cs_gpio_pin;      /**< Specify the chip select pin of NOR FLASH is controlled by which gpio pin.
                                    This parameter can be a value of GPIO_PIN_x (x = 0,1,2,....,14,15, 20) */

} NORFlash_MX25L6433F_Init_T;
/**
 * @}
 */

/**
 * @defgroup max25l6433f_memory_struct Memory information struct
 * \ingroup norflash_max25l6433f_struct
 * @{
 */

typedef struct _NORFlash_Capacity_T
{
    uint32_t norflash_page_size;    /**< MX25L6433F one page size             */
    uint32_t norflash_sector_size;  /**< MX25L6433F one sector size           */
    uint32_t norflash_block32_size; /**< MX25L6433F one block32 size          */
    uint32_t norflash_block64_size; /**< MX25L6433F one block64 size          */
    uint32_t norflash_flash_size;   /**< MX25L6433F whole flash size          */

} NORFlash_Capacity_T;
/**
 * @}
 */

/**
 * @defgroup max25l6433f_memory Memory information
 * \ingroup max25l6433f_related
 * @{
 */
#define MX25L6433F_PAGE_SIZE    0x100              /**< MX25L6433F one page size             */
#define MX25L6433F_SECTOR_SIZE  0x1000             /**< MX25L6433F one sector size           */
#define MX25L6433F_BLOCK32_SIZE 0x8000             /**< MX25L6433F one block32 size          */
#define MX25L6433F_BLOCK64_SIZE 0x10000            /**< MX25L6433F one block64 size          */
#define MX25L6433F_MEM_SIZE     0x800000           /**< MX25L6433F flash memory size         */
#define MX25L6433F_ADDRESS_END  (NOR_MEM_SIZE - 1) /**< MX25L6433F flash memory end address  */
/**
 * @}
 */

/**
 * @defgroup norflash_protect_area Protect area definition
 * \ingroup max25l6433f_related
 * @{
 */
#define NOR_BLOCK_0          (0x00000000U) /**< Block area 0 */
#define NOR_BLOCK_0_TO_1     (0x00000001U) /**< Block area 0 and 1 */
#define NOR_BLOCK_0_TO_3     (0x00000002U) /**< Block area 0 to 3 */
#define NOR_BLOCK_0_TO_7     (0x00000003U) /**< Block area 0 to 7 */
#define NOR_BLOCK_0_TO_15    (0x00000004U) /**< Block area 0 to 15 */
#define NOR_BLOCK_0_TO_31    (0x00000005U) /**< Block area 0 to 31 */
#define NOR_BLOCK_0_TO_63    (0x00000006U) /**< Block area 0 to 63 */
#define NOR_BLOCK_ALL        (0x00000007U) /**< Block area 0 to 63 */
#define NOR_BLOCK_127        (0x00000008U) /**< Block area 127 */
#define NOR_BLOCK_126_TO_127 (0x00000009U) /**< Block area 126 to 127 */
#define NOR_BLOCK_124_TO_127 (0x0000000AU) /**< Block area 124 to 127 */
#define NOR_BLOCK_120_TO_127 (0x0000000BU) /**< Block area 120 to 127 */
#define NOR_BLOCK_112_TO_127 (0x0000000CU) /**< Block area 112 to 127 */
#define NOR_BLOCK_96_TO_127  (0x0000000DU) /**< Block area 96 to 127 */
#define NOR_BLOCK_64_TO_127  (0x0000000EU) /**< Block area 64 to 127 */
/**
 * @}
 */

/**
 * @defgroup norflash_read_type Read type definition
 * \ingroup max25l6433f_related
 * @{
 */
#define NOR_TYPE_RX_READ      (0x1UL << 0U) /**< Read Data Bytes (READ) operation */
#define NOR_TYPE_RX_FAST_READ (0x1UL << 1U) /**< Read Data Bytes at Higher Speed (FAST_READ) operation */
/**
 * @}
 */

/**
 * @defgroup norflash_fs_polarity SPI protocol Frame/Sync active polarity
 * \ingroup max25l6433f_related
 * @{
 */
#define NOR_FS_POLARITY_LOW  (0x00000000U) /**< Frame/Sync active at low  level */
#define NOR_FS_POLARITY_HIGH (0x00000001U) /**< Frame/Sync active at high level */
/**
 * @}
 */

/**
 * @defgroup norflash_clk_polarity SPI protocol clock polarity
 * \ingroup max25l6433f_related
 * @{
 */
#define NOR_CLK_POLARITY_LOW  (0x00000000U) /**< SCLK active at low level */
#define NOR_CLK_POLARITY_HIGH (0x00000001U) /**< SCLK active at high level */
/**
 * @}
 */

/**
 * @defgroup norflash_clk_phase SPI protocol Clock Phase
 * \ingroup max25l6433f_related
 * @{
 */
#define NOR_CLK_PHASE_1EDGE (0x00000000U) /**< Data changes at clock falling edge, latches at clock rising edge when Clock Polarity = 0, \
                                               Data changes at clock rising edge, latches at clock falling edge when Clock Polarity = 1. */
#define NOR_CLK_PHASE_2EDGE (0x00000001U) /**< Data changes at clock rising edge, latches at clock falling edge when Clock Polarity = 0, \
                                               Data changes at clock falling edge, latches at clock rising edge when Clock Polarity = 1. */
/**
 * @}
 */

/**
 * @defgroup norflash_first_bit SPI protocol first transmitted bit
 * \ingroup max25l6433f_related
 * @{
 */
#define NOR_FIRST_BIT_MSB (0x00000000U) /**< MSB transmit first */
#define NOR_FIRST_BIT_LSB (0x00000001U) /**< LSB transmit first */
/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif // __NORFlash_MX25L6433F_H_
