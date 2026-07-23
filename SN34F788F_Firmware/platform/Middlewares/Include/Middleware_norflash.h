/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _MIDDLEWARE_NORFLASH_H_
#define _MIDDLEWARE_NORFLASH_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "./NORFlash/NORFlash_MX25L6433F.h"

/* Exported types ------------------------------------------------------------*/
/**
 * \brief NOR FLASH callback type enumeration
 * \ingroup norflash_common_struct
 */
typedef enum _HAL_NOR_CallbackID_T
{
    NOR_TX_COMPLETE_CB_ID = 0x00U, /**< NORFLASH tx completed callback ID         */
    NOR_RX_COMPLETE_CB_ID = 0x01U, /**< NORFLASH rx completed callback ID         */
    NOR_ERROR_CB_ID       = 0x02U, /**< NORFLASH error callback ID                */
    NOR_MSP_INIT_CB_ID    = 0x03U, /**< NORFLASH msp init callback ID             */
    NOR_MSP_DEINIT_CB_ID  = 0x04U, /**< NORFLASH msp deinit callback ID           */
} NOR_CallbackID_T;

/**
 * \brief   NOR FLASH tx runtime information
 * \ingroup norflash_common_struct
 */
typedef struct _NOR_TX_T
{
    uint8_t       *buffer;     /**< Pointer to NOR tx transfer buffer                  */
    __IOM uint32_t xfer_count; /**< NOR tx transfer down counter                       */
    uint32_t       nor_addr;   /**< NOR Flash current address of programming operation */
} NOR_TX_T;

/**
 * \brief NOR FLASH handle structure definition
 * \ingroup norflash_common_struct
 */
typedef struct _NORFlash_Handle_T
{
#if (configUSE_NORFLASH_TYPE == NORFLASH_MX25L6433F)
    NORFlash_MX25L6433F_Init_T init; /**< NOR Flash initialization parameters       */
#endif                               /* NORFLASH_MX25L6433F */

    HAL_MUTEX         mutex;       /**< NOR Flash process protection              */
    __IOM HAL_State_T state;       /**< NOR Flash communication state             */
    __IOM uint32_t    flash_state; /**< NOR Flash inner state                     */
    NOR_TX_T          tx;          /**< NOR Flash tx transfer runtime information */
    __IOM uint32_t    error_code;  /**< NOR Flash error code                      */

#if (configUSE_NORFLASH_NONBLOCKING_DMA == 1U)
    DMA_Handle_T *hdmatx; /**< NOR FLASH tx DMA handle parameters */
    DMA_Handle_T *hdmarx; /**< NOR FLASH rx DMA handle parameters */
#endif                    /* configUSE_NORFLASH_NONBLOCKING_DMA */

#if (configUSE_NORFLASH_REGISTER_CALLBACKS == 1U)
    void (*TxCpltCallback)(struct _NORFlash_Handle_T *hnor); /**< NOR FLASH tx completed callback. When receiving this callback, HAL_NORFlash_GetState should be called to get SPI FLASH status */
    void (*RxCpltCallback)(struct _NORFlash_Handle_T *hnor); /**< NOR FLASH rx completed callback */
    void (*ErrorCallback)(struct _NORFlash_Handle_T *hnor);  /**< NOR FLASH error callback. When receiving this callback, HAL_NORFlash_GetState should be called to get SPI FLASH status */
#endif                                                       /* configUSE_NORFLASH_REGISTER_CALLBACKS */
} NORFlash_Handle_T;

/**
 * \brief NOR FLASH callback pointer definition
 * \ingroup norflash_exported_types_gr
 */
typedef void (*NOR_Callback_T)(NORFlash_Handle_T *hnor); /**< pointer to a NOR FLASH callback function */

/**
 * \ingroup norflash_error_code
 * @{
 */
#define NOR_ERROR_INVALID_CALLBACK (HAL_ERROR_SPECIFY + 0x01)    /**<NOR Flash invalid Callback error  */
#define NOR_ERROR_TX_UNDERFLOW     (HAL_ERROR_SPECIFY + 0x02)    /**<NOR Flash Tx Underflow error      */
#define NOR_ERROR_RX_OVERFLOW      (HAL_ERROR_SPECIFY + 0x03)    /**<NOR Flash rx Overflow error       */
#define NOR_ERROR_DMA              (HAL_ERROR_SPECIFY + 0x04)    /**<NOR Flash DMA error               */
#define NOR_ERROR_DMA_TX           (HAL_ERROR_SPECIFY + 0x05)    /**<NOR Flash DMA Tx error            */
#define NOR_ERROR_DMA_RX           (HAL_ERROR_SPECIFY + 0x06)    /**<NOR Flash DMA Rx error            */
#define NOR_ERROR_ABORT            (HAL_ERROR_SPECIFY + 0x07)    /**<NOR Flash Abort error             */
#define NOR_ERROR_INIT_GPIO        (HAL_SPI_ERROR_ABORT + 0x01U) /**<NOR Flash Init GPIO error         */
#define NOR_ERROR_INIT_SPI         (HAL_SPI_ERROR_ABORT + 0x02U) /**<NOR Flash Init SPI error          */
#define NOR_ERROR_TIMEOUT          (HAL_ERROR_TIMEOUT)           /**<NOR Flash Timeout error           */
/**
 * @}
 */

/**
 * @brief NOR FLASH Memory Information
 * \ingroup norflash_mem_info
 * @{
 */
#if (configUSE_NORFLASH_TYPE == NORFLASH_MX25L6433F)
#define NOR_PAGE_SIZE    MX25L6433F_PAGE_SIZE    /**< current flash one page size             */
#define NOR_SECTOR_SIZE  MX25L6433F_SECTOR_SIZE  /**< current flash one sector size           */
#define NOR_BLOCK32_SIZE MX25L6433F_BLOCK32_SIZE /**< current flash one block32 size          */
#define NOR_BLOCK64_SIZE MX25L6433F_BLOCK64_SIZE /**< current flash one block64 size          */
#define NOR_MEM_SIZE     MX25L6433F_MEM_SIZE     /**< current flash flash memory size         */
#define NOR_ADDRESS_END  (NOR_MEM_SIZE - 1)      /**< current flash flash memory end address  */
#endif                                           /* configUSE_NORFLASH_TYPE == NORFLASH_MX25L6433F */
/**
 * @}
 */

/* Exported functions --------------------------------------------------------*/
/* Initialization/de-initialization functions */
HAL_Status_T HAL_NORFlash_Init(NORFlash_Handle_T *hnor);
HAL_Status_T HAL_NORFlash_DeInit(NORFlash_Handle_T *hnor);

/* Callbacks Register/UnRegister functions -----------------------------------*/
HAL_Status_T HAL_NORFlash_RegisterCallback(NORFlash_Handle_T *hnor, NOR_CallbackID_T cb_id, NOR_Callback_T pcb);
HAL_Status_T HAL_NORFlash_UnRegisterCallback(NORFlash_Handle_T *hnor, NOR_CallbackID_T cb_id);

/* NOR FLASH state function */
HAL_State_T HAL_NORFlash_GetState(NORFlash_Handle_T *hnor);

/* NOR FLASH error code function */
uint32_t HAL_NORFlash_GetError(NORFlash_Handle_T *hnor);

/* NOR FLASH capacity response function */
HAL_Status_T HAL_NORFlash_GetCapacity(NORFlash_Capacity_T *capacity);

/* I/O operation functions */
HAL_Status_T HAL_NORFlash_Program(NORFlash_Handle_T *hnor, uint32_t start_addr, uint8_t *data_buffer, uint32_t write_size, uint32_t timeout);
HAL_Status_T HAL_NORFlash_Read(NORFlash_Handle_T *hnor, uint32_t rx_type, uint32_t start_addr, uint8_t *data_buffer, uint32_t read_size, uint32_t timeout);
HAL_Status_T HAL_NORFlash_EraseSector(NORFlash_Handle_T *hnor, uint32_t start_addr, uint32_t timeout);
HAL_Status_T HAL_NORFlash_EraseChip(NORFlash_Handle_T *hnor, uint32_t timeout);
uint32_t     HAL_NORFlash_ReadID(NORFlash_Handle_T *hnor);

#if (configUSE_NORFLASH_TYPE == NORFLASH_MX25L6433F)
HAL_Status_T HAL_NORFlash_Program_DMA(NORFlash_Handle_T *hnor, uint32_t start_addr, uint8_t *data_buffer, uint32_t write_size);
HAL_Status_T HAL_NORFlash_Read_DMA(NORFlash_Handle_T *hnor, uint32_t rx_type, uint32_t start_addr, uint8_t *data_buffer, uint32_t read_size);
HAL_Status_T HAL_NORFlash_EraseBlock32(NORFlash_Handle_T *hnor, uint32_t start_addr, uint32_t timeout);
HAL_Status_T HAL_NORFlash_EraseBlock64(NORFlash_Handle_T *hnor, uint32_t start_addr, uint32_t timeout);
HAL_Status_T HAL_NORFlash_ProtectBlock(NORFlash_Handle_T *hnor, uint32_t block_area, uint32_t timeout);
HAL_Status_T HAL_NORFlash_ProtectBlockAll(NORFlash_Handle_T *hnor, uint32_t timeout);
HAL_Status_T HAL_NORFlash_ProtectResumeAll(NORFlash_Handle_T *hnor, uint32_t timeout);
HAL_Status_T HAL_NORFlash_EnterDPD(NORFlash_Handle_T *hnor);
HAL_Status_T HAL_NORFlash_ExitDPD(NORFlash_Handle_T *hnor);
#endif

/* Transfer Abort functions ***************************************************/
#if (configUSE_NORFLASH_TYPE == NORFLASH_MX25L6433F)
HAL_Status_T HAL_NORFlash_Abort(NORFlash_Handle_T *hnor);
#endif

/* Callbacks operation functions */
void HAL_NORFlash_TxCpltCallback(NORFlash_Handle_T *hnor);
void HAL_NORFlash_RxCpltCallback(NORFlash_Handle_T *hnor);
void HAL_NORFlash_ErrorCallback(NORFlash_Handle_T *hnor);

/* Callbacks reset functions */
void HAL_NORFlash_ResetCallback(NORFlash_Handle_T *hnor);

#ifdef __cplusplus
}
#endif

#endif // _MIDDLEWARE_NORFLASH_H_
