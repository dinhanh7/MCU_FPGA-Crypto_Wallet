/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _MX25L6433F_H_
#define _MX25L6433F_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "../nor_flash.h"

/* Chip Command ----------------------------------------------------------*/
/**
 * @brief  flash chip cmd.
 * @note   current type : MX25L6433F
 * @{
 */
#if (configUSE_NORFLASH_TYPE == NORFLASH_MX25L6433F)

/**
 * @brief  flash command information.
 */
#define NOR_CMD_PAGR_PROGRAM      0x02U
#define NOR_CMD_READ              0x03U
#define NOR_CMD_WRITE_DISABLE     0x04U
#define NOR_CMD_READ_STATUS_REG   0x05U
#define NOR_CMD_WRITE_ENABLE      0x06U
#define NOR_CMD_FAST_READ         0x0BU
#define NOR_CMD_ERASE_SECTOR      0x20U
#define NOR_CMD_ERASE_BLOCK32     0x52U
#define NOR_CMD_ERASE_BLOCK64     0xD8U
#define NOR_CMD_ERASE_CHIP        0x60U
#define NOR_CMD_READ_SECURITY_REG 0x2BU
#define NOR_CMD_READ_CONFIG_REG   0x15U
#define NOR_CMD_WRITE_STATUS_REG  0x01U
#define NOR_CMD_ENTER_DPD         0xB9U
#define NOR_CMD_EXIT_DPD          0xABU
#define NOR_CMD_READ_ID           0xABU
/**
 * @brief  flash status reg information.
 */
#define NOR_STATUS_REG_BIT_WIP  (0x1UL << 0U)
#define NOR_STATUS_REG_BIT_WEL  (0x1UL << 1U)
#define NOR_STATUS_REG_BIT_BP0  (0x1UL << 2U)
#define NOR_STATUS_REG_BIT_BP1  (0x1UL << 3U)
#define NOR_STATUS_REG_BIT_BP2  (0x1UL << 4U)
#define NOR_STATUS_REG_BIT_BP3  (0x1UL << 5U)
#define NOR_STATUS_REG_BIT_QE   (0x1UL << 6U)
#define NOR_STATUS_REG_BIT_SRWD (0x1UL << 7U)

/**
 * @brief  flash security reg information.
 */
#define NOR_SECURITY_REG_BIT_OTP    (0x1UL << 0U)
#define NOR_SECURITY_REG_BIT_LDSO   (0x1UL << 1U)
#define NOR_SECURITY_REG_BIT_PSB    (0x1UL << 2U)
#define NOR_SECURITY_REG_BIT_ESB    (0x1UL << 3U)
#define NOR_SECURITY_REG_BIT_P_FAIL (0x1UL << 5U)
#define NOR_SECURITY_REG_BIT_E_FAIL (0x1UL << 6U)
#endif /* MX25L6433F */
/**
 * @}
 */

/* Private macros ------------------------------------------------------------*/

/**
 * @brief  spi flash inner state.
 * @param  __HANDLE__ specifies the SPI Flash Handle.
 * @param  __FLASH_STATE__ specifies the flash_state source to enable or disable.
 */
#define NOR_FLASH_STATE_IDLE                              (0x1U << 0U)
#define NOR_FLASH_STATE_READ                              (0x1U << 1U)
#define NOR_FLASH_STATE_PROGRAM                           (0x1U << 2U)
#define NOR_FLASH_STATE_ERASE                             (0x1U << 3U)
#define NOR_FLASH_STATE_REMAIN_DATA                       (0x1U << 4U)
#define NOR_FLASH_STATE_DPD                               (0x1U << 5U)
#define NOR_FLASH_STATE_SET(__HANDLE__, __FLASH_STATE__)  HAL_FLAG_SET((__HANDLE__)->flash_state, (__FLASH_STATE__))
#define NOR_FLASH_STATE_CLR(__HANDLE__, __FLASH_STATE__)  HAL_FLAG_CLR((__HANDLE__)->flash_state, (__FLASH_STATE__))
#define NOR_FLASH_STATE_HAS(__HANDLE__, __FLASH_STATE__)  HAL_FLAG_HAS((__HANDLE__)->flash_state, (__FLASH_STATE__))
#define NOR_FLASH_STATE_LACK(__HANDLE__, __FLASH_STATE__) HAL_FLAG_LACKS((__HANDLE__)->flash_state, (__FLASH_STATE__))

/**
 * @brief  Enable/Disable the specified SPI Flash DMA.
 * @param  __HANDLE__ specifies the SPI Handle.
 * @param  __REQUEST__ specifies the DMA Request source to enable or disable.
 */
#define NOR_SPI_TFDMAEN                          (1U << 5) // TX dma interrupt enable
#define NOR_SPI_RFDMAEN                          (1U << 4) // RX dma interrupt enable
#define NOR_ENABLE_DMA(__HANDLE__, __REQUEST__)  HAL_FLAG_SET((__HANDLE__)->init.instance->ICTRL, (__REQUEST__))
#define NOR_DISABLE_DMA(__HANDLE__, __REQUEST__) HAL_FLAG_CLR((__HANDLE__)->init.instance->ICTRL, (__REQUEST__))

/**
 * @brief  Clear spi fifo level.
 * @param  __HANDLE__ specifies the SPI Handle.
 */
#define NOR_DISABLE_TX_THOD(__HANDLE__) HAL_REG_WRITE((__HANDLE__)->init.instance->ICTRL_b.TFTHOD, 0)
#define NOR_DISABLE_RX_THOD(__HANDLE__) HAL_REG_WRITE((__HANDLE__)->init.instance->ICTRL_b.RFTHOD, 0)

/**
 * @brief  Enable/Disable Tx/Rx.
 * @param  __HANDLE__ specifies the SPI Handle.
 */
#define NOR_ENABLE_SPI_TX(__HANDLE__)  HAL_REG_WRITE((__HANDLE__)->init.instance->CTRL2_b.TXEN, 1)
#define NOR_ENABLE_SPI_RX(__HANDLE__)  HAL_REG_WRITE((__HANDLE__)->init.instance->CTRL2_b.RXEN, 1)
#define NOR_DISABLE_SPI_TX(__HANDLE__) HAL_REG_WRITE((__HANDLE__)->init.instance->CTRL2_b.TXEN, 0)
#define NOR_DISABLE_SPI_RX(__HANDLE__) HAL_REG_WRITE((__HANDLE__)->init.instance->CTRL2_b.RXEN, 0)

/**
 * @brief  Inner Cmd operation wait time.
 */
#define NOR_CMD_WAIT_TIME 0x3000U

/**
 * @brief Check spi flash instance
 * @param __HANDLE__ specifies the SPI FLASH Handle.
 */
#define IS_NOR_INSTANCE(__HANDLE__) (((__HANDLE__)->init.instance) == SN_SPI0 || \
                                     ((__HANDLE__)->init.instance) == SN_SPI1 || \
                                     ((__HANDLE__)->init.instance) == SN_SPI2)

/**
 * @brief check spi flash clock polarity
 * @param __HANDLE__ specifies the SPI FLASH Handle.
 */
#define IS_NOR_CLK_POL(__HANDLE__) (((__HANDLE__)->init.clk_polarity) == NOR_CLK_POLARITY_LOW || \
                                    ((__HANDLE__)->init.clk_polarity) == NOR_CLK_POLARITY_HIGH)

/**
 * @brief check spi flash clock phase
 * @param __HANDLE__ specifies the SPI FLASH Handle.
 */
#define IS_NOR_CLK_PHA(__HANDLE__) (((__HANDLE__)->init.clk_phase) == NOR_CLK_POLARITY_LOW || \
                                    ((__HANDLE__)->init.clk_phase) == NOR_CLK_POLARITY_HIGH)

/**
 * @brief check spi flash clock division value
 * @param __HANDLE__ specifies the SPI FLASH Handle.
 */
#define NOR_CLK_DIV_MAX            (512U)
#define IS_NOR_CLK_DIV(__HANDLE__) (((__HANDLE__)->init.sclk_div) <= NOR_CLK_DIV_MAX)

/**
 * @brief check spi flash first bit attribute
 * @param __HANDLE__ specifies the SPI FLASH Handle.
 */
#define IS_NOR_MSB_LSB(__HANDLE__) (((__HANDLE__)->init.first_bit) == NOR_FIRST_BIT_MSB || \
                                    ((__HANDLE__)->init.first_bit) == NOR_FIRST_BIT_LSB)

/**
 * @brief check gpio pin number to control spi flash chip select pin
 * @param __HANDLE__ specifies the SPI FLASH Handle.
 */
#define NOR_GPIO_PIN_GROUP1_MAX     (15U)
#define NOR_GPIO_PIN_GROUP2_MAX     (20U)
#define IS_NOR_GPIO_PIN(__HANDLE__) (((__HANDLE__)->init.cs_gpio_pin) <= NOR_GPIO_PIN_GROUP1_MAX || \
                                     ((__HANDLE__)->init.cs_gpio_pin) == NOR_GPIO_PIN_GROUP2_MAX)

#if (configUSE_NORFLASH_NONBLOCKING_DMA == 1)
static NORFlash_Handle_T *nor_handle = NULL;
#define ASSIGN_NOR_HANDLE()  (nor_handle = hnor)
#define RELEASE_NOR_HANDLE() (nor_handle = NULL)
#else
#define ASSIGN_NOR_HANDLE()  (__NOP())
#define RELEASE_NOR_HANDLE() (__NOP())
#endif /* configUSE_NORFLASH_NONBLOCKING_DMA */

/* Private function ----------------------------------------------------------*/

HAL_Status_T MX25L6433F_Init(NORFlash_Handle_T *hnor);
HAL_Status_T MX25L6433F_DeInit(NORFlash_Handle_T *hnor);

void MX25L6433F_ErrorCallback(NORFlash_Handle_T *hnor);

HAL_Status_T MX25L6433F_Program(NORFlash_Handle_T *hnor, uint32_t start_addr, uint8_t *pData, uint32_t write_size, uint32_t timeout);
HAL_Status_T MX25L6433F_Read(NORFlash_Handle_T *hnor, uint32_t rx_type, uint32_t start_addr, uint8_t *pData, uint32_t read_size, uint32_t timeout);

HAL_Status_T MX25L6433F_Program_DMA(NORFlash_Handle_T *hnor, uint32_t start_addr, uint8_t *pData, uint32_t write_size);
HAL_Status_T MX25L6433F_Read_DMA(NORFlash_Handle_T *hnor, uint32_t rx_type, uint32_t start_addr, uint8_t *pData, uint32_t read_size);

HAL_Status_T MX25L6433F_ProtectBlock(NORFlash_Handle_T *hnor, uint32_t blockArea, uint32_t timeout);
HAL_Status_T MX25L6433F_ProtectBlockAll(NORFlash_Handle_T *hnor, uint32_t timeout);
HAL_Status_T MX25L6433F_ProtectResumeAll(NORFlash_Handle_T *hnor, uint32_t timeout);

HAL_Status_T MX25L6433F_EnterDPD(NORFlash_Handle_T *hnor);
HAL_Status_T MX25L6433F_ExitDPD(NORFlash_Handle_T *hnor);
uint32_t     MX25L6433F_ReadID(NORFlash_Handle_T *hnor);

HAL_Status_T MX25L6433F_EraseSector(NORFlash_Handle_T *hnor, uint32_t start_addr, uint32_t timeout);
HAL_Status_T MX25L6433F_EraseBlock32(NORFlash_Handle_T *hnor, uint32_t start_addr, uint32_t timeout);
HAL_Status_T MX25L6433F_EraseBlock64(NORFlash_Handle_T *hnor, uint32_t start_addr, uint32_t timeout);
HAL_Status_T MX25L6433F_EraseChip(NORFlash_Handle_T *hnor, uint32_t timeout);

HAL_Status_T MX25L6433F_GetCapacity(NORFlash_Capacity_T *capacity);

HAL_State_T MX25L6433F_GetState(NORFlash_Handle_T *hnor);
uint32_t    MX25L6433F_GetError(NORFlash_Handle_T *hnor);

HAL_Status_T MX25L6433F_Abort(NORFlash_Handle_T *hnor);

#if (configUSE_NORFLASH_REGISTER_CALLBACKS == 1U)
void inline MX25L6433F_ResetCallback(NORFlash_Handle_T *hnor);
#endif /* configUSE_SPI_REGISTER_CALLBACKS */

#if (configUSE_NORFLASH_NONBLOCKING_DMA == 1U)
static void _norflash_dma_tx_writepage_cb(SPI_Handle_T *hspi);
static void _norflash_dma_rx_complete_cb(SPI_Handle_T *hspi);
static void _norflash_dma_msp_init_cb(DMA_Handle_T *hdma);
static void _norflash_dma_msp_deinit_cb(DMA_Handle_T *hdma);
#endif /* configUSE_NORFLASH_NONBLOCKING_DMA */

static void _norflash_spi_msp_init_callback(SPI_Handle_T *hspi);
static void _norflash_spi_msp_deinit_callback(SPI_Handle_T *hspi);

static HAL_Status_T _wren_and_wait_wel_ready(NORFlash_Handle_T *hnor, uint32_t timeout);
static HAL_Status_T _wait_wip_ready(NORFlash_Handle_T *hnor, uint32_t timeout);
static HAL_Status_T _check_p_e_success(NORFlash_Handle_T *hnor, uint32_t timeout);

#ifdef __cplusplus
}
#endif

#endif /* _MX25L6433F_H_ */
