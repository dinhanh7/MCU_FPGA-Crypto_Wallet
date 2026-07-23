/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _HAL_FLASH_H_
#define _HAL_FLASH_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sn34f78x_hal_def.h"

/**
 * @brief Flash Procedure Definition
 * \ingroup flash_struct_define
 * @{
 */
typedef enum
{
    FLASH_PROC_NONE = 0U, /*!< None */
    FLASH_PROC_PAGEERASE, /*!< Page Erase */
    FLASH_PROC_SECTERASE, /*!< Sector Erase */
    FLASH_PROC_MASSERASE, /*!< Mass Erase */
    FLASH_PROC_PROGRAM    /*!< Program */
} FLASH_ProcedureTypeDef;
/**
 * @}
 */

/**
 * @brief Flash Process Structure Definition
 * \ingroup flash_struct_define
 * @{
 */
typedef struct
{
    __IO FLASH_ProcedureTypeDef ProcedureOnGoing; /*Internal variable to indicate which procedure is ongoing or not in IT context */
    __IO uint32_t               NbSectorsToErase; /*Internal variable to save the remaining sectors to erase in IT context */
    __IO uint32_t               Sector;           /*Internal variable to define the current sector which is erasing */
    __IO uint32_t               NbPagesToErase;   /*Internal variable to save the remaining sectors to erase in IT context */
    __IO uint32_t               Page;             /*Internal variable to define the current sector which is erasing */
    __IO uint32_t               Address;          /*Internal variable to save address selected for program */
    __IO uint32_t               MapAddress;       /*Internal variable to save remap address for program */
    __IO uint32_t               ErrorCode;        /* FLASH error code */
} FLASH_ProcessTypeDef;
/**
 * @}
 */

/* Exported constants --------------------------------------------------------*/
/**
 * @brief Flash Procedure Error Definition
 * \ingroup flash_error_code
 * @{
 */
#define HAL_FLASH_ERROR_NONE                      0x00000000U /*!< FLASH Success of Operation flag           */
#define HAL_FLASH_ERROR_INVALID_ADDR              0x00000001U /*!< Flash Invalid page address                */
#define HAL_FLASH_ERROR_INVALID_TYPE              0x00000002U /*!< Flash Invalid memory type                 */
#define HAL_FLASH_ERROR_ILLEGAL_SM_OP             0x00000003U /*!< Flash Illegal SM operation                */
#define HAL_FLASH_ERROR_ERASE_FAILED              0x00000004U /*!< Flash Erase failed                        */
#define HAL_FLASH_ERROR_PROGRAM_FAILED            0x00000005U /*!< Flash Program failed                      */
#define HAL_FLASH_ERROR_RDP_CHANGE_FAILED         0x00000006U /*!< Flash Read protect change failed          */
#define HAL_FLASH_ERROR_WRP_EN_DIS_FAILED         0x00000007U /*!< Flash Write protect enable/disable failed */
#define HAL_FLASH_ERROR_ARB_WRITE_FAILED          0x00000008U /*!< Flash Arbitrary write failed              */
#define HAL_FLASH_ERROR_INVALID_MAGIC_NUM         0x00000009U /*!< Flash Invalid magic number                */
#define HAL_FLASH_ERROR_BOOTFALG_WRITE_FAILED     0x0000000AU /*!< Flash Write boot flag failed              */
#define HAL_FLASH_ERROR_CUSTOMER_USE_WRITE_FAILED 0x0000000BU /*!< Flash Write customer-used data failed     */
#define HAL_FLASH_ERROR_INVALID_OFFSET            0x0000000CU /*!< Flash Invalid offset                      */
#define HAL_FLASH_ERROR_INVALID_LENGTH            0x0000000DU /*!< Flash Invalid length                      */
#define HAL_FLASH_ERROR_FAILURE                   0x0000FFFFU /*!< Flash Failed of Operation flag            */
/**
 * @}
 */

/**
 * \defgroup flash_busy_flag Flash Busy Status Definition
 * \ingroup flash_control
 * @{
 */
#define FLASH_PROC_STATUS_BSY 0x00000001 /*!< Flash Busy flag */
/**
 * @}
 */

/**
 * \defgroup flash_ist_status Flash IST Status Definition
 * \ingroup flash_control
 * @{
 */
#define FLASH_STATUS_DONE 0x00000001 /*!< FLASH STATUS Done flag */
#define FLASH_STATUS_FAIL 0x00000002 /*!< FLASH STATUS Fail flag */
/**
 * @}
 */

/**
 * \defgroup flash_it Flash Interrupt Definition
 * \ingroup flash_control
 * @{
 */
#define FLASH_IT_EOP 0x00000001 /*!< End of FLASH Operation Interrupt source */
/**
 * @}
 */

/* Exported macro ------------------------------------------------------------*/

/* Include FLASH HAL Extension module */
#include "hal_flash_ex.h"

/* Exported functions --------------------------------------------------------*/
/* Program operation functions  ***********************************************/
HAL_Status_T HAL_FLASH_Program_Page(uint32_t Address, uint8_t *Data);
HAL_Status_T HAL_FLASH_Program_Arbitrary(uint32_t Address, uint8_t *Data, uint32_t Size);
HAL_Status_T HAL_FLASH_Program_Page_IT(uint32_t Address, uint8_t *Data);
HAL_Status_T HAL_FLASH_Program_Arbitrary_IT(uint32_t Address, uint8_t *Data, uint32_t Size);
/* Flash IRQ handler method */
void HAL_FLASH_IRQHandler(void);
/* Callbacks in non blocking modes */
void HAL_FLASH_EndOfOperationCallback(uint32_t ReturnValue);
void HAL_FLASH_OperationErrorCallback(uint32_t ReturnValue);
/* Others */
uint32_t HAL_FLASH_GetError(void);
void     HAL_InitFlash(void);

#ifdef __cplusplus
}
#endif

#endif /* _HAL_FLASH_H_ */
