/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _MIDDLEWARE_DEBUGGER_H_
#define _MIDDLEWARE_DEBUGGER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "sn34f78x_hal.h"

#if (configUSE_DEBUGGER_OPTION == 1)
#include "FreeRTOS.h"
#include "semphr.h"
#include "queue.h"
#endif

/* Exported types ------------------------------------------------------------*/
/**
 * @brief Command info Structure Definition
 * \ingroup cmd_struct_define
 */
typedef struct _cmd_info
{
    char             *cmd_name; /**< Cmd name of current function */
    uint32_t          cmd_id;   /**< Cmd enum id of current function, it must be less than 0x1000 */
    char             *param;    /**< Cmd parameters of current function */
    struct _cmd_info *next;     /**< Points to the next _cmd_info parameter */
} CMD_INFO_T;

/**
 * @brief Command Handle Structure Definition
 * \ingroup cmd_struct_define
 */
typedef struct _cmd_handle
{
    CMD_INFO_T *cmd_table; /**< The table holds the registered command item */
#if (configUSE_DEBUGGER_OPTION == 1)
    SemaphoreHandle_t cmd_semp;  /**< Semaphore for AT+CMD process */
    xQueueHandle      cmd_queue; /**< The queue for cmd info @ref CMD_INFO_T. for loading the latest information, the size of queue is 1 */
#endif
} CMD_Handle_T;

/**
 * @brief debugger_mode Debugger Mode
 * \ingroup debugger_struct_define
 *
 */
typedef enum
{
    DEBUGGER_MODE_SWO  = 0x1U, /**< Under SWO MODE TX Message */
    DEBUGGER_MODE_UART = 0x2U, /**< Under Uart MODE TX/RX Message */
} Debugger_Mode_T;

/**
 * @brief Debugger State Code
 * \ingroup debugger_struct_define
 */
typedef enum
{
    DEBUGGER_STATE_RESET   = 0x0U, /**< Debugger is not yet initialized */
    DEBUGGER_STATE_READY   = 0x1U, /**< Debugger initialized and ready for use */
    DEBUGGER_STATE_BUSY_TX = 0x2U, /**< Debugger Data Transmission process is ongoing */
    DEBUGGER_STATE_BUSY_RX = 0x4U, /**< Debugger Data Reception process is ongoing */
} Debugger_State_T;

/**
 * @brief for debugger init
 * \ingroup debugger_struct_define
 */
typedef struct _debugger_init
{
    Debugger_Mode_T debugger_mode; /**< debugger mode;This parameter can be a value of @ref Debugger_Mode_T */
    uint16_t        buff_size;     /**< print buffer size default is 128 byte */
#if (configUSE_DEBUGGER_OPTION == 0 || configUSE_DEBUGGER_OPTION == 1)
    void    *instance;  /**< under DEBUGGER_MODE_UART, set the uart instance */
    uint32_t baud_rate; /**< Under DEBUGGER_MODE_UART, set the uart baudrate */
    char     cmd_end;   /**< Config the end symbol for get_chars API */
#endif

#if (configUSE_DEBUGGER_OPTION == 1)
    uint8_t monitor_task_priority; /**< Allows the user to set the priority of the monitor task   */
#endif

} Debugger_INIT_T;

/**
 * @brief  Debugger handle Structure definition
 * \ingroup debugger_struct_define
 */
typedef struct _debugger_handle
{
    Debugger_INIT_T       init;    /**< Debugger initialization parameters   */
    char                 *tx_buff; /**< Buffer for TX process				 */
    __IO Debugger_State_T state;   /**< Debugger State during process		 */

#if (configUSE_DEBUGGER_OPTION == 0 || configUSE_DEBUGGER_OPTION == 1)
    UART_Handle_T cur_uart; /**< Define the uart handler under DEBUGGER_MODE_UART */
#endif

#if (configUSE_DEBUGGER_OPTION == 0 || configUSE_DEBUGGER_OPTION == 1 || configUSE_DEBUGGER_OPTION == 3)
    char *rx_buff; /**< Buffer for RX process */
#endif

#if (configUSE_DEBUGGER_OPTION == 1)
    SemaphoreHandle_t tx_semp;    /**< Semaphore for TX process under RTOS */
    SemaphoreHandle_t rx_semp;    /**< Semaphore for RX process under RTOS */
    CMD_Handle_T      cmd_handle; /**< Register CMD informations under Console */
#endif
} Debugger_Handle_T;

/* Exported Macro --------------------------------------------------------*/
#if configUSE_DEBUGGER_LEVEL >= 0
#define EMERG_PRINT(fmt, args...) printf(fmt "\r\n", ##args)
#else
#define EMERG_PRINT(fmt, args...)
#endif

#if configUSE_DEBUGGER_LEVEL >= 1
#define ALERT_PRINT(fmt, args...) printf(fmt "\r\n", ##args)
#else
#define ALERT_PRINT(fmt, args...)
#endif

#if configUSE_DEBUGGER_LEVEL >= 2
#define CRIT_PRINT(fmt, args...) printf(fmt "\r\n", ##args)
#else
#define CRIT_PRINT(fmt, args...)
#endif

#if configUSE_DEBUGGER_LEVEL >= 3
#define ERR_PRINT(fmt, args...) printf(fmt "\r\n", ##args)
#else
#define ERR_PRINT(fmt, args...)
#endif

#if configUSE_DEBUGGER_LEVEL >= 4
#define WARN_PRINT(fmt, args...) printf(fmt "\r\n", ##args)
#else
#define WARN_PRINT(fmt, args...)
#endif

#if configUSE_DEBUGGER_LEVEL >= 5
#define NOTE_PRINT(fmt, args...) printf(fmt "\r\n", ##args)
#else
#define NOTE_PRINT(fmt, args...)
#endif

#if configUSE_DEBUGGER_LEVEL >= 6
#define INFO_PRINT(fmt, args...) printf(fmt "\r\n", ##args)
#else
#define INFO_PRINT(fmt, args...)
#endif

#if configUSE_DEBUGGER_LEVEL >= 7
#define DEBUG_PRINT(fmt, args...) printf(fmt "\r\n", ##args)
#else
#define DEBUG_PRINT(fmt, args...)
#endif

/* Exported functions --------------------------------------------------------*/
/* Initialization/De-initialization functions *********************************/
int32_t HAL_DBG_Init(Debugger_Handle_T *hdbg);
int32_t HAL_DBG_DeInit(void);

/* Debugger printf by UART while configUSE_DEBUGGER_OPTION == 0 || configUSE_DEBUGGER_OPTION == 1 */
int32_t      printf_uart(const char *fmt, ...);
HAL_Status_T get_chars(char *buf);

/* Debugger console by UART while configUSE_DEBUGGER_OPTION == 1 */
int32_t HAL_DBG_Reg_Cmd(char *cmd_name, uint32_t cmd_id);
int32_t HAL_DBG_ReceiveCmd(Debugger_Handle_T *hdbg, CMD_INFO_T *cmd);

/* Debugger printf by SWO while configUSE_DEBUGGER_OPTION == 2 */
int32_t printf_swo(const char *fmt, ...);

#endif // _MIDDLEWARE_DEBUGGER_H_
