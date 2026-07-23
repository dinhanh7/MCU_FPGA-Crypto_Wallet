/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _SN34F78X_DEBUGGER_H_
#define _SN34F78X_DEBUGGER_H_

#include "sn34f78x_hal.h"

#define DEBUGGER_MIDDLEWARE_UART_MODE  0
#define DEBUGGER_MIDDLEWARE_ATCMD_MODE 1
#define DEBUGGER_MIDDLEWARE_SWO_MODE   2

#define DEFAULT_BUF_LENGTH            128
#define DEFAULT_BAUD_RATE             115200
#define DEFAULT_MONITOR_TASK_PRIORITY 3

#if (configUSE_DEBUGGER_OPTION == DEBUGGER_MIDDLEWARE_UART_MODE || configUSE_DEBUGGER_OPTION == DEBUGGER_MIDDLEWARE_ATCMD_MODE)
static int32_t      _get_chars_kernel(char *buf);
static HAL_Status_T _get_chars_end_with_sym(uint8_t *data, uint16_t max_size, char end_with, uint16_t *true_length);
#endif

#if (configUSE_DEBUGGER_OPTION == DEBUGGER_MIDDLEWARE_ATCMD_MODE)

typedef enum
{
    AT_CMD_DUMP = 0x1000,
    AT_CMD_UPDATE,
    AT_CMD_TASKINFO,
    AT_CMD_UPDATE_MULTI,
} CMD_BUILDID;

static HAL_Status_T _console_init(Debugger_Handle_T *hdbg);
static HAL_Status_T _console_deinit(void);

static HAL_Status_T _monitor_input_string(char *buf);

static void   _free_cmd_table(CMD_Handle_T *hAtCmd);
static void   _cmd_monitor_task(void *parameter);
static void   _characters_split(char *cmd, int16_t *param_index);
static int8_t _mem_para_split(char *cmd_para, uint32_t *address, uint32_t *value);
static int8_t _mem_para_split_all(char *cmd_para);
static int8_t _is_at_cmd(const char *buf);
static void   _dispatch_cmd(CMD_INFO_T *cmd);

static CMD_INFO_T *_get_cmd_by_name(const char *cmd_name);
static int8_t      _check_cmd_format(const char *buf);
static void        _set_ascii_to_buf(uint8_t ascii, uint8_t *buf);
static int8_t      _mem_update(char *cmd_para);
static int8_t      _mem_update_all(char *cmd_para);
static int8_t      _mem_dump(char *cmd_para);
static void        _printf_task_info(char *cmd_para);

#endif

#endif //_SN34F78X_DEBUGGER_H_
