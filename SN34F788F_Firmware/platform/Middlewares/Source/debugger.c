/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#include "debugger.h"

#if (configUSE_DEBUGGER_MIDDLEWARE == 1)

static Debugger_Handle_T *_debug_handle = NULL;

#if (configUSE_DEBUGGER_OPTION == DEBUGGER_MIDDLEWARE_UART_MODE || configUSE_DEBUGGER_OPTION == DEBUGGER_MIDDLEWARE_ATCMD_MODE)
/**
 * @brief Get chars end with symbol
 *
 * @param data Data pointing address
 * @param size Data size
 * @param end_with End character
 * @param true_length Actual length
 * @return HAL_Status_T
 */
static HAL_Status_T _get_chars_end_with_sym(uint8_t *data, uint16_t size, char end_with, uint16_t *true_length)
{
    UART_Handle_T *huart = &_debug_handle->cur_uart;
    /* Set RX runtime info */
    huart->rx.buffer     = data;
    huart->rx.xfer_size  = size;
    huart->rx.xfer_count = size;
    huart->rx.abort      = 0;

#if (configUSE_DEBUGGER_OPTION == 1)
    xSemaphoreTake(_debug_handle->tx_semp, portMAX_DELAY);
#endif

    SN_UART0_Type *instance = (SN_UART0_Type *)huart->instance;
    /* Reset RX FIFO */
    HAL_REG_WRITE(instance->FIFOCTRL_b.RXFIFORST, 1);
    HAL_REG_WRITE(instance->FIFOCTRL_b.RXTL, 0);

#if (configUSE_DEBUGGER_OPTION == 1)
    xSemaphoreGive(_debug_handle->tx_semp);
#endif

    /* Release the process protection */
    while (huart->rx.xfer_count > 0U)
    {
        /* Check RDR (receiver data ready) flag */
        if (HAL_REG_READ(instance->LS_b.RDR) == 1)
        {
            /* Receive data */
            *(huart->rx.buffer) = HAL_REG_READ(instance->RB_b.RB);
            /* If end_with is '\n', Enter key alse end get_chars */
            if ((*(huart->rx.buffer) == end_with) || (end_with == '\n' && (*(huart->rx.buffer) == 0xD)))
            {
                huart->rx.xfer_count = 0;
            }
            else
            {
                huart->rx.xfer_count--;
                huart->rx.buffer++;
                (*true_length)++;
            }
        }
    }

    /* Update the RX state */
    huart->rx_state = HAL_STATE_READY;

    return HAL_OK;
}

/**
 * @brief Gets the string and writes it to buf
 *
 * @param buf Pointer to receive string buffer.
 * @return int32_t
 */
static int32_t _get_chars_kernel(char *buf)
{
    if (_debug_handle->init.debugger_mode != DEBUGGER_MODE_UART || _debug_handle->state == DEBUGGER_STATE_RESET)
    {
        return HAL_ERROR;
    }

    int32_t  result      = HAL_OK;
    uint16_t true_length = 0;
    if (_debug_handle->init.cmd_end == 0)
    {
        result      = HAL_UART_Receive(&_debug_handle->cur_uart, (uint8_t *)buf, _debug_handle->init.buff_size, HAL_MAX_DELAY);
        true_length = _debug_handle->init.buff_size;
    }
    else
    {
        result = _get_chars_end_with_sym((uint8_t *)buf, _debug_handle->init.buff_size, _debug_handle->init.cmd_end, &true_length);
    }

    if (result != HAL_OK)
    {
        return result;
    }
    buf[true_length] = '\0';

    return HAL_OK;
}

/**
 * @brief Printf string stream to UART.
 *
 * @param p Pointer to the FILE structure.
 * @param fmt Form control string.
 * @param ... This parameter is optional.
 * @return int32_t
 */
int32_t fprintf_chars(FILE *p, const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    int32_t result = printf_uart(fmt, args);
    va_end(args);

    return result;
}

#endif

#if (configUSE_DEBUGGER_OPTION == DEBUGGER_MIDDLEWARE_UART_MODE)
/**
 * @brief  Init Debugger.
 * @param  hdbg Pointer to struct \ref Debugger_Handle_T
 * @retval int32_t
 */
int32_t HAL_DBG_Init(Debugger_Handle_T *hdbg)
{
    if (hdbg->state == HAL_STATE_RESET)
    {
        if (0 == hdbg->init.buff_size)
        {
            hdbg->init.buff_size = DEFAULT_BUF_LENGTH;
        }

        if (hdbg->init.debugger_mode == DEBUGGER_MODE_UART)
        {
            hdbg->cur_uart.instance = hdbg->init.instance;
            if (0 == hdbg->init.baud_rate)
            {
                hdbg->init.baud_rate = DEFAULT_BAUD_RATE;
            }
            hdbg->cur_uart.init.baud_rate   = hdbg->init.baud_rate;
            hdbg->cur_uart.init.parity      = UART_PARITY_NONE;
            hdbg->cur_uart.init.stop_bits   = UART_STOP_BITS_1;
            hdbg->cur_uart.init.word_length = UART_WORD_LENGTH_8;

            if (HAL_UART_Init(&hdbg->cur_uart) != HAL_OK)
            {
                return HAL_ERROR;
            }
        }

        if (hdbg->rx_buff == NULL)
        {
            hdbg->rx_buff = malloc(sizeof(char) * hdbg->init.buff_size + 1);
            if (hdbg->rx_buff == NULL)
            {
                return HAL_ERROR;
            }
            memset(hdbg->rx_buff, 0, (sizeof(char) * (hdbg->init.buff_size + 1)));
        }

        if (hdbg->tx_buff == NULL)
        {
            hdbg->tx_buff = malloc(sizeof(char) * hdbg->init.buff_size + 1);
            if (hdbg->tx_buff == NULL)
            {
                return HAL_ERROR;
            }
            memset(hdbg->tx_buff, 0, (sizeof(char) * (hdbg->init.buff_size + 1)));
        }

        hdbg->state   = DEBUGGER_STATE_READY;
        _debug_handle = hdbg;
    }
    else
    {
        return HAL_ERROR;
    }

    return HAL_OK;
}

/**
 * @brief  De-init Debugger.
 * @retval int32_t
 */
int32_t HAL_DBG_DeInit(void)
{
    if (_debug_handle->state != DEBUGGER_STATE_READY)
    {
        return HAL_ERROR;
    }

    if (_debug_handle->tx_buff)
    {
        free(_debug_handle->tx_buff);
        _debug_handle->tx_buff = NULL;
    }
    if (_debug_handle->rx_buff)
    {
        free(_debug_handle->rx_buff);
        _debug_handle->rx_buff = NULL;
    }

    HAL_UART_DeInit(&_debug_handle->cur_uart);

    _debug_handle->state = DEBUGGER_STATE_RESET;
    return HAL_OK;
}

/**
 * @brief  Printf string to UART.
 * @param  fmt Include print string format.
 * @retval int32_t
 */
int32_t printf_uart(const char *fmt, ...)
{
    if (_debug_handle->init.debugger_mode != DEBUGGER_MODE_UART || _debug_handle->state == DEBUGGER_STATE_RESET)
    {
        return HAL_ERROR;
    }

    if (HAL_REG_RBIT(_debug_handle->state, DEBUGGER_STATE_BUSY_TX))
    {
        return HAL_BUSY;
    }
    HAL_REG_SBIT(_debug_handle->state, DEBUGGER_STATE_BUSY_TX);

    va_list args;
    int32_t result = HAL_OK;
    va_start(args, fmt);

    int32_t ret = vsnprintf(_debug_handle->tx_buff, _debug_handle->init.buff_size, fmt, args);
    va_end(args);

    uint16_t data_len = strlen(_debug_handle->tx_buff);
    uint8_t *data     = (uint8_t *)_debug_handle->tx_buff;

    if (ret < 0)
    {
        result = HAL_ERROR;
        goto __DEBUGGER_HAL_END__;
    }

    result = HAL_UART_Transmit(&_debug_handle->cur_uart, data, data_len, HAL_MAX_DELAY);
__DEBUGGER_HAL_END__:
    HAL_REG_CBIT(_debug_handle->state, DEBUGGER_STATE_BUSY_TX);
    return result;
}

/**
 * @brief  Get chars by UART.
 * @param  buf Pointer to receive string buffer.
 * @retval None
 */
HAL_Status_T get_chars(char *buf)
{
    HAL_Status_T status = HAL_OK;

    if (HAL_REG_RBIT(_debug_handle->state, DEBUGGER_STATE_BUSY_RX))
    {
        return HAL_BUSY;
    }
    HAL_REG_SBIT(_debug_handle->state, DEBUGGER_STATE_BUSY_RX);

    if (_get_chars_kernel(buf) != HAL_OK)
    {
        status = HAL_ERROR;
    }

    HAL_REG_CBIT(_debug_handle->state, DEBUGGER_STATE_BUSY_RX);

    return status;
}

#endif

#if (configUSE_DEBUGGER_OPTION == DEBUGGER_MIDDLEWARE_SWO_MODE)
/**
 * @brief  Init Debugger.
 * @param  hdbg Pointer to struct \ref Debugger_Handle_T
 * @retval int32_t
 */
int32_t HAL_DBG_Init(Debugger_Handle_T *hdbg)
{
    if (hdbg->state == HAL_STATE_RESET)
    {
        if (0 == hdbg->init.buff_size)
        {
            hdbg->init.buff_size = DEFAULT_BUF_LENGTH;
        }

        if (hdbg->tx_buff == NULL)
        {
            hdbg->tx_buff = malloc(sizeof(char) * hdbg->init.buff_size + 1);
            if (hdbg->tx_buff == NULL)
            {
                return HAL_ERROR;
            }
            memset(hdbg->tx_buff, 0, (sizeof(char) * (hdbg->init.buff_size + 1)));
        }

        hdbg->state   = DEBUGGER_STATE_READY;
        _debug_handle = hdbg;
    }
    return HAL_OK;
}

/**
 * @brief  De-init Debugger.
 * @retval int32_t
 */
int32_t HAL_DBG_DeInit(void)
{
    if (_debug_handle->state != DEBUGGER_STATE_READY)
        return HAL_ERROR;

    if (_debug_handle->tx_buff)
    {
        free(_debug_handle->tx_buff);
        _debug_handle->tx_buff = NULL;
    }

    _debug_handle->state = DEBUGGER_STATE_RESET;
    return HAL_OK;
}

/**
 * @brief  Printf string to SWO.
 * @param  fmt Include print string format.
 * @retval int32_t
 */
int32_t printf_swo(const char *fmt, ...)
{
    if (_debug_handle->init.debugger_mode != DEBUGGER_MODE_SWO || _debug_handle->state == DEBUGGER_STATE_RESET)
    {
        return HAL_ERROR;
    }

    if (HAL_REG_RBIT(_debug_handle->state, DEBUGGER_STATE_BUSY_TX))
    {
        return HAL_BUSY;
    }

    HAL_REG_SBIT(_debug_handle->state, DEBUGGER_STATE_BUSY_TX);

    int32_t  result = HAL_OK;
    va_list  args;
    uint8_t *pCur = NULL;
    va_start(args, fmt);
    int32_t ret = vsnprintf(_debug_handle->tx_buff, _debug_handle->init.buff_size, fmt, args);
    va_end(args);

    if (ret < 0)
    {
        result = HAL_ERROR;
        goto __DEBUGGER_SWO_HAL_END__;
    }

    pCur = (uint8_t *)_debug_handle->tx_buff;
    while (*pCur)
    {
        ITM_SendChar(*pCur++);
    }
__DEBUGGER_SWO_HAL_END__:
    HAL_REG_CBIT(_debug_handle->state, DEBUGGER_STATE_BUSY_TX);
    return result;
}

/**
 * @brief Printf string stream to SWO.
 *
 * @param p Pointer to the FILE structure.
 * @param fmt Form control string.
 * @param ... This parameter is optional.
 * @return int32_t
 */
int32_t fprintf_swo(FILE *p, const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    int32_t result = printf_swo(fmt, args);
    va_end(args);

    return result;
}

/* used for Debug Input */
volatile int32_t ITM_RxBuffer = ITM_RXBUFFER_EMPTY;

/**
 * @brief Get the char swo object
 *
 * @return int32_t
 */
int32_t getchar_swo(void)
{
    while (ITM_CheckChar() != 1)
    {
    }

    return ITM_ReceiveChar();
}

#endif

#if (configUSE_DEBUGGER_OPTION == DEBUGGER_MIDDLEWARE_ATCMD_MODE)
static xQueueHandle _buf_queue;   /**< The queue is used to put buf by get_chars_kernel */
static TaskHandle_t _parser_task; /**< Cmd Monitor AT CMD and parse tasks */

/**
 * @brief  Init Debugger.
 * @param  hdbg Pointer to struct \ref Debugger_Handle_T
 * @retval int32_t
 */
int32_t HAL_DBG_Init(Debugger_Handle_T *hdbg)
{
    if (hdbg->state == HAL_STATE_RESET)
    {
        if (0 == hdbg->init.buff_size)
        {
            hdbg->init.buff_size = DEFAULT_BUF_LENGTH;
        }

        if (hdbg->init.debugger_mode == DEBUGGER_MODE_UART)
        {
            hdbg->cur_uart.instance = hdbg->init.instance;
            if (0 == hdbg->init.baud_rate)
            {
                hdbg->init.baud_rate = DEFAULT_BAUD_RATE;
            }
            hdbg->cur_uart.init.baud_rate   = hdbg->init.baud_rate;
            hdbg->cur_uart.init.parity      = UART_PARITY_NONE;
            hdbg->cur_uart.init.stop_bits   = UART_STOP_BITS_1;
            hdbg->cur_uart.init.word_length = UART_WORD_LENGTH_8;

            if (HAL_UART_Init(&hdbg->cur_uart) != HAL_OK)
            {
                return HAL_ERROR;
            }
        }

        if (hdbg->tx_semp == NULL)
        {
            hdbg->tx_semp = xSemaphoreCreateBinary();
            if (hdbg->tx_semp == NULL)
            {
                return HAL_ERROR;
            }
            xSemaphoreGive(hdbg->tx_semp);
        }

        if (hdbg->rx_semp == NULL)
        {
            hdbg->rx_semp = xSemaphoreCreateBinary();
            if (hdbg->rx_semp == NULL)
            {
                return HAL_ERROR;
            }
            xSemaphoreGive(hdbg->rx_semp);
        }

        if (hdbg->rx_buff == NULL)
        {
            hdbg->rx_buff = pvPortMalloc(sizeof(char) * (hdbg->init.buff_size + 1));
            if (hdbg->rx_buff == NULL)
            {
                return HAL_ERROR;
            }
            memset(hdbg->rx_buff, 0, (sizeof(char) * (hdbg->init.buff_size + 1)));
        }
        if (hdbg->tx_buff == NULL)
        {
            hdbg->tx_buff = pvPortMalloc(sizeof(char) * (hdbg->init.buff_size + 1));
            if (hdbg->tx_buff == NULL)
            {
                return HAL_ERROR;
            }
            memset(hdbg->tx_buff, 0, (sizeof(char) * (hdbg->init.buff_size + 1)));
        }
        _buf_queue = xQueueCreate(1, sizeof(char) * (hdbg->init.buff_size + 1));
        if (0 == hdbg->init.monitor_task_priority)
        {
            hdbg->init.monitor_task_priority = DEFAULT_MONITOR_TASK_PRIORITY;
        }

        hdbg->state   = DEBUGGER_STATE_READY;
        _debug_handle = hdbg;

        if (HAL_OK != _console_init(hdbg))
        {
            return HAL_ERROR;
        }
    }
    return HAL_OK;
}

/**
 * @brief  De-init Debugger.
 * @retval int32_t
 */
int32_t HAL_DBG_DeInit(void)
{
    if (_debug_handle->state != DEBUGGER_STATE_READY)
    {
        return HAL_ERROR;
    }

    if (_console_deinit() != HAL_OK)
    {
        return HAL_ERROR;
    }
    if (_debug_handle->tx_buff)
    {
        vPortFree(_debug_handle->tx_buff);
        _debug_handle->tx_buff = NULL;
    }
    if (_debug_handle->rx_buff)
    {
        vPortFree(_debug_handle->rx_buff);
        _debug_handle->rx_buff = NULL;
    }
    if (_debug_handle->tx_semp != NULL)
    {
        vSemaphoreDelete(_debug_handle->tx_semp);
        _debug_handle->tx_semp = NULL;
    }
    if (_debug_handle->rx_semp != NULL)
    {
        vSemaphoreDelete(_debug_handle->rx_semp);
        _debug_handle->rx_semp = NULL;
    }

    if (_debug_handle->init.debugger_mode == DEBUGGER_MODE_UART)
    {
        HAL_UART_DeInit(&_debug_handle->cur_uart);
    }

    _debug_handle->state = DEBUGGER_STATE_RESET;
    return HAL_OK;
}

/**
 * @brief  Printf string to UART.
 * @param  fmt Include print string format.
 * @retval int32_t
 */
int32_t printf_uart(const char *fmt, ...)
{
    if (_debug_handle->init.debugger_mode != DEBUGGER_MODE_UART || _debug_handle->state == DEBUGGER_STATE_RESET)
    {
        return HAL_ERROR;
    }

    xSemaphoreTake(_debug_handle->tx_semp, portMAX_DELAY);

    va_list args;
    int32_t result = HAL_OK;
    va_start(args, fmt);

    int32_t ret = vsnprintf(_debug_handle->tx_buff, _debug_handle->init.buff_size, fmt, args);
    va_end(args);

    uint16_t data_len = strlen(_debug_handle->tx_buff);
    uint8_t *data     = (uint8_t *)_debug_handle->tx_buff;

    if (ret < 0)
    {
        result = HAL_ERROR;
        goto __DEBUGGER_HAL_END__;
    }

    result = HAL_UART_Transmit(&_debug_handle->cur_uart, data, data_len, HAL_MAX_DELAY);
__DEBUGGER_HAL_END__:

    xSemaphoreGive(_debug_handle->tx_semp);

    return result;
}

/**
 * @brief  Get chars by UART.
 * @param  buf Pointer to receive string buffer.
 * @retval None
 */
HAL_Status_T get_chars(char *buf)
{
    HAL_Status_T status = HAL_OK;

    xSemaphoreTake(_debug_handle->rx_semp, portMAX_DELAY);

    xQueueReceive(_buf_queue, buf, portMAX_DELAY);

    xSemaphoreGive(_debug_handle->rx_semp);

    return status;
}

/**
 * @brief  Register CMD function.
 * @param  cmd_name CMD function name.
 * @param  cmd_id CMD function id, it is recommended to be less than 0x1000.
 * @retval int32_t
 */
int32_t HAL_DBG_Reg_Cmd(char *cmd_name, uint32_t cmd_id)
{
    RET_FLAG_TRUE(_debug_handle->state == DEBUGGER_STATE_RESET, HAL_ERROR);
    RET_FLAG_FALSE(cmd_name, HAL_ERROR);

    /* Special character check: '\r\n',&,%....... */
    if (!_check_cmd_format(cmd_name))
    {
        return HAL_ERROR;
    }
    CMD_INFO_T *new_cmd = _get_cmd_by_name(cmd_name);
    /* Command table lookup */
    if (!new_cmd)
    {
        /* Create a command parsing table */
        new_cmd = pvPortMalloc(sizeof(CMD_INFO_T));
        if (!new_cmd)
        {
            return HAL_ERROR;
        }
        new_cmd->next = NULL;
        /* List header insertion */
        new_cmd->next                       = _debug_handle->cmd_handle.cmd_table;
        _debug_handle->cmd_handle.cmd_table = new_cmd;
    }
    new_cmd->cmd_name = cmd_name;
    new_cmd->cmd_id   = cmd_id;

    return HAL_OK;
}
/**
 * @brief Receiving terminal command
 *
 * @param hdbg  Pointer to struct \ref Debugger_Handle_T
 * @param cmd  Pointer to struct \ref CMD_INFO_T
 * @return HAL_Status_T
 */
int32_t HAL_DBG_ReceiveCmd(Debugger_Handle_T *hdbg, CMD_INFO_T *cmd)
{
    RET_FLAG_FALSE(hdbg, HAL_ERROR);
    RET_FLAG_FALSE(cmd, HAL_ERROR);
    RET_FLAG_TRUE(_debug_handle->state == DEBUGGER_STATE_RESET, HAL_ERROR);

    xQueueReceive(hdbg->cmd_handle.cmd_queue, cmd, portMAX_DELAY);

    return HAL_OK;
}
/**
 * @brief Example Initialize the command terminal
 *
 * @param hdbg  Pointer to struct \ref Debugger_Handle_T
 * @return HAL_Status_T
 */
static HAL_Status_T _console_init(Debugger_Handle_T *hdbg)
{
    if (hdbg->init.monitor_task_priority >= configMAX_PRIORITIES)
    {
        return HAL_ERROR;
    }
    if (hdbg->cmd_handle.cmd_semp == NULL)
    {
        hdbg->cmd_handle.cmd_semp = xSemaphoreCreateBinary();
        if (hdbg->cmd_handle.cmd_semp == NULL)
        {
            return HAL_ERROR;
        }
        xSemaphoreGive(hdbg->cmd_handle.cmd_semp);
    }

    hdbg->cmd_handle.cmd_table = NULL;

    HAL_DBG_Reg_Cmd("Dump", AT_CMD_DUMP);
    HAL_DBG_Reg_Cmd("Update", AT_CMD_UPDATE);
    HAL_DBG_Reg_Cmd("TaskInfo", AT_CMD_TASKINFO);
    HAL_DBG_Reg_Cmd("MultiUpdate", AT_CMD_UPDATE_MULTI);
    /* Create a task to monitor input string for AT+CMD */
    if (xTaskCreate(_cmd_monitor_task, "AT_CMD", 128, NULL, hdbg->init.monitor_task_priority, &_parser_task) != pdPASS)
    {
        return HAL_ERROR;
    }
    hdbg->cmd_handle.cmd_queue = xQueueCreate(1, sizeof(CMD_INFO_T));

    return HAL_OK;
}

/**
 * @brief Command terminal release
 *
 * @return HAL_Status_T
 */
static HAL_Status_T _console_deinit()
{
    vQueueDelete(_debug_handle->cmd_handle.cmd_queue);
    vQueueDelete(_buf_queue);

    vTaskDelete(_parser_task);

    _free_cmd_table(&_debug_handle->cmd_handle);
    _debug_handle->cmd_handle.cmd_table = NULL;

    if (_debug_handle->cmd_handle.cmd_semp != NULL)
    {
        vSemaphoreDelete(_debug_handle->cmd_handle.cmd_semp);
        _debug_handle->cmd_handle.cmd_semp = NULL;
    }
    return HAL_OK;
}

/**
 * @brief Release the AT CMD registry
 *
 * @param hAtCmd Pointer to struct \ref CMD_Handle_T
 */
static void _free_cmd_table(CMD_Handle_T *hAtCmd)
{
    CMD_INFO_T *cur_table = hAtCmd->cmd_table;

    while (cur_table != NULL)
    {
        vPortFree(cur_table);
        cur_table = cur_table->next;
    }
}

/**
 * @brief Monitor the tasks of AT cmd
 *
 * @param parameter Task parameter
 */
static void _cmd_monitor_task(void *parameter)
{
    while (1)
    {
        _monitor_input_string(_debug_handle->rx_buff);
        vTaskDelay(1);
    }
}

/**
 * @brief Input character monitoring
 *
 * @param buf Pointer to receive string buffer.
 * @return HAL_Status_T
 */
static HAL_Status_T _monitor_input_string(char *buf)
{
    HAL_Status_T status = HAL_OK;
    xSemaphoreTake(_debug_handle->cmd_handle.cmd_semp, portMAX_DELAY);
    if (_get_chars_kernel(buf) != HAL_OK)
    {
        printf_uart("Debugger monitor task error: Get chars fail\r\n");
        return HAL_ERROR;
    }
    if (_is_at_cmd(buf))
    {
        char *cmd = buf + 3;
        if (!_check_cmd_format(cmd))
        {
            printf_uart("CMD Error: Invalid CMD format - %s\r\n", buf);
            status = HAL_ERROR;
            goto Monitor_End;
        }
        int16_t para_index = 0;
        _characters_split(cmd, &para_index);

        CMD_INFO_T *cmd_info = _get_cmd_by_name(cmd);
        if (!cmd_info)
        {
            printf_uart("CMD Error: Invalid CMD name - %s\r\n", cmd);
            status = HAL_ERROR;
            goto Monitor_End;
        }
        cmd_info->param = cmd + para_index;
        _dispatch_cmd(cmd_info);
    }
    else
    {
        xQueueReset(_buf_queue);
        xQueueSend(_buf_queue, buf, portMAX_DELAY);
    }

Monitor_End:
    xSemaphoreGive(_debug_handle->cmd_handle.cmd_semp);
    return status;
}

/**
 * @brief Check if the character is "AT+"
 *
 * @param buf Pointer to receive string buffer.
 * @return int8_t
 */
static int8_t _is_at_cmd(const char *buf)
{
    if (memcmp(buf, "AT+", 3) == 0)
    {
        return 1;
    }
    return 0;
}

/**
 * @brief Check the format of the entered command
 *
 * @param buf Pointer to receive string buffer.
 * @return int8_t
 */
static int8_t _check_cmd_format(const char *buf)
{
    const char *cmd = buf;
    uint32_t    len = strlen(cmd);
    if (len > _debug_handle->init.buff_size)
    {
        return 0;
    }

    for (uint32_t i = 0; i < len - 1; i++)
    {
        if (cmd[i] == '_' || (cmd[i] <= '9' && cmd[i] >= '0') || (cmd[i] <= 'Z' && cmd[i] >= 'A') || (cmd[i] <= 'z' && cmd[i] >= 'a') || cmd[i] == ',' || cmd[i] == '=' || cmd[i] == _debug_handle->init.cmd_end)
        {
            continue;
        }
        return 0;
    }
    return 1;
}

/**
 * @brief Look for the command and perform response processing
 *
 * @param cmd_name Command name
 * @return CMD_INFO_T*
 */
static CMD_INFO_T *_get_cmd_by_name(const char *cmd_name)
{
    CMD_INFO_T *temp = _debug_handle->cmd_handle.cmd_table;
    while (temp != NULL)
    {
        if (strcmp(temp->cmd_name, cmd_name) == 0)
        {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

/**
 * @brief Characters split
 *
 * @param cmd Command
 * @param param_index Parameter index
 */
static void _characters_split(char *cmd, int16_t *param_index)
{
    char *buf = cmd;

    /* Confirm command length (excluding 'AT+') */
    uint32_t cmd_len = strlen(cmd);

    /* Take the value, then point to the next one */
    while ((*buf) != '=')
    {
        /* Prevent addresses from crossing boundaries */
        if ((*param_index) >= cmd_len)
            return;
        /* Confirm the address before increment */
        buf++;
        (*param_index)++;
    }
    /* Character substitution: '=' replaces '\0' */
    cmd[(*param_index)] = '\0';
    /* The index value of the parameter */
    (*param_index)++;
}

/**
 * @brief Dispatch command
 *
 * @param cmd  Pointer to struct \ref CMD_INFO_T
 */
static void _dispatch_cmd(CMD_INFO_T *cmd)
{
    xQueueReset(_debug_handle->cmd_handle.cmd_queue);
    switch (cmd->cmd_id)
    {
        case AT_CMD_DUMP:
            _mem_dump(cmd->param);
            break;
        case AT_CMD_UPDATE:
            _mem_update(cmd->param);
            break;
        case AT_CMD_TASKINFO:
            _printf_task_info(cmd->param);
            break;
        case AT_CMD_UPDATE_MULTI:
            _mem_update_all(cmd->param);
        default:
            xQueueSend(_debug_handle->cmd_handle.cmd_queue, cmd, portMAX_DELAY);
            break;
    }
}

/**
 * @brief The string is converted to an integer
 *
 * @param str  Pointer to string
 * @param value  Pointer to value
 * @return int8_t
 */
static int8_t _str_to_int_16(char *str, uint32_t *value)
{
    while (*str)
    {
        if (*str >= '0' && *str <= '9')
        {
            *value = (*value) * 16 + (*str - '0');
        }
        else if (*str >= 'a' && *str <= 'f')
        {
            *value = (*value) * 16 + (*str - 'a' + 0xa);
        }
        else if (*str >= 'A' && *str <= 'F')
        {
            *value = (*value) * 16 + (*str - 'A' + 0xa);
        }
        else
        {
            printf("str = %c\n", str);
            return 0;
        }

        str++;
    }
    return 1;
}

/**
 * @brief The string is converted to an integer
 *
 * @param str  Pointer to string
 * @param value  Pointer to value
 * @return int8_t
 */
static int8_t _str_to_int(char *str, uint32_t *value)
{
    int8_t bHex = 0;
    if (str[0] == '0' && (str[1] == 'x' || str[1] == 'X'))
    {
        bHex = 1;
        str  = str + 2;
    }
    while (*str)
    {
        if (bHex)
        {
            if (*str >= '0' && *str <= '9')
            {
                *value = (*value) * 16 + (*str - '0');
            }
            else if (*str >= 'a' && *str <= 'f')
            {
                *value = (*value) * 16 + (*str - 'a');
            }
            else if (*str >= 'A' && *str <= 'F')
            {
                *value = (*value) * 16 + (*str - 'A');
            }
            else
            {
                return 0;
            }
        }
        else
        {
            if (*str >= '0' && *str <= '9')
            {
                *value = (*value) * 10 + (*str - '0');
            }
            else
            {
                return 0;
            }
        }
        str++;
    }
    return 1;
}

/**
 * @brief Disassemble the terminal command argument and convert the string to an integer value.
 * @example "AT+CMD = 0X12340000, 23". The raw string conversion resolves to an integer value: address = 0x12340000, value = 23.
 * @param cmd_para  Memory String address.
 * @param address Command parameter address.
 * @param value Command parameter value.
 * @return int8_t
 */
static int8_t _mem_para_split_all(char *cmd_para)
{
    char    *buf         = cmd_para;
    uint32_t cmd_size    = strlen(cmd_para);
    uint32_t value_index = 0;
    char     cvalue[3]   = {0};
    uint32_t value       = 0;
    uint32_t address     = 0;
    uint32_t addrlen     = 0;

    uint32_t index[2] = {0};

    for (int i = 0; i < 2; i++)
    {
        while (*buf != ',')
        {
            /* Prevent access from crossing boundaries */
            if (value_index >= cmd_size)
                return 1;

            value_index++;
            buf++;
        }
        cmd_para[value_index++] = '\0';
        index[i]                = value_index;
        buf++;
    }

    /* Address of processing */
    if (!_str_to_int(cmd_para, &address))
    {
        return 0;
    }

    /* Address len of processing */
    if (!_str_to_int(&(cmd_para[index[0]]), &addrlen))
    {
        return 0;
    }

    while (*buf != '\0')
    {
        memcpy(cvalue, buf, 2);
        cvalue[2] = '\0';
        if (!_str_to_int_16(cvalue, &value))
        {
            return 0;
        }

        *(uint8_t *)address = value;
        address += 1;
        buf += 2;
    }
    return 1;
}

/**
 * @brief Disassemble the terminal command argument and convert the string to an integer value.
 * @example “AT+CMD = 0X12340000, 23�?. The raw string conversion resolves to an integer value�? address = 0x12340000, value = 23.
 * @param cmd_para  Memory String address.
 * @param address Command parameter address.
 * @param value Command parameter value.
 * @return int8_t
 */
static int8_t _mem_para_split(char *cmd_para, uint32_t *address, uint32_t *value)
{
    char    *buf         = cmd_para;
    uint32_t cmd_size    = strlen(cmd_para);
    uint32_t value_index = 0;

    while (*buf != ',')
    {
        /* Prevent access from crossing boundaries */
        if (value_index >= cmd_size)
            return 0;

        value_index++;
        buf++;
    }
    cmd_para[value_index++] = '\0';
    if (!_str_to_int(cmd_para, address))
    {
        return 0;
    }
    buf++;
    if (!_str_to_int(buf, value))
    {
        return 0;
    }
    return 1;
}

/**
 * @brief Memory update
 *
 * @param cmd_para  Command parameter
 * @return int8_t
 */
static int8_t _mem_update_all(char *cmd_para)
{
    if (!_mem_para_split_all(cmd_para))
    {
        printf_uart("CMD Error: Memory format should be Hex(0[xX]d+) or Dec number\r\n");
        return 0;
    }
    printf_uart("Update OK\r\n");
    return 1;
}

/**
 * @brief Memory update
 *
 * @param cmd_para  Command parameter
 * @return int8_t
 */
static int8_t _mem_update(char *cmd_para)
{
    uint32_t address      = 0;
    uint32_t update_value = 0;
    if (!_mem_para_split(cmd_para, &address, &update_value))
    {
        printf_uart("CMD Error: Memory format should be Hex(0[xX]d+) or Dec number\r\n");
        return 0;
    }
    uint32_t *data = (uint32_t *)address;
    *data          = update_value;
    printf_uart("Update OK\r\n");
    return 1;
}

/**
 * @brief Ascii value conversion char
 *
 * @param ascii  ASCII value
 * @param buf Pointer to receive string buffer.
 */
static void _set_ascii_to_buf(uint8_t ascii, uint8_t *buf)
{
    *buf++ = (ascii / 16) >= 10 ? (ascii / 16) + 'A' - 0xA : (ascii / 16) + '0';
    *buf   = (ascii % 16) >= 10 ? (ascii % 16) + 'A' - 0xA : (ascii % 16) + '0';
}

/**
 * @brief Memory dump
 *
 * @param cmd_para Command parameter
 * @return int8_t
 */
static int8_t _mem_dump(char *cmd_para)
{
    if (_debug_handle->init.debugger_mode != DEBUGGER_MODE_UART || _debug_handle->state == DEBUGGER_STATE_RESET)
    {
        return 0;
    }

    uint32_t address = 0;
    uint32_t len     = 0;
    if (!_mem_para_split(cmd_para, &address, &len))
    {
        printf_uart("CMD Error: Memory format should be Hex(0[xX]d+) or Dec number\r\n");
        return 0;
    }
    if (len == 0)
    {
        printf_uart("Data Error: Invalid access \r\n");
        return 0;
    }

    xSemaphoreTake(_debug_handle->tx_semp, portMAX_DELAY);
    int32_t result = HAL_OK;

    uint8_t *buf = pvPortMalloc(sizeof(uint8_t) * (len * 3) + 1);
    memset(buf, 0, (sizeof(uint8_t) * (len * 3) + 1));

    uint8_t *temp = buf;
    for (uint32_t i = 0; i < len; i++)
    {
        _set_ascii_to_buf(*(uint8_t *)(address + i), temp);
        temp = temp + 2;
        if ((i + 1) % 16 == 0 && (i != 0))
            *temp++ = '\n';
        else
            *temp++ = ' ';
    }
    *(--temp) = '\r';
    *(++temp) = '\n';

    result = HAL_UART_Transmit(&_debug_handle->cur_uart, buf, (sizeof(uint8_t) * (len * 3) + 1), HAL_MAX_DELAY);

    vPortFree(buf);
    buf = NULL;
    xSemaphoreGive(_debug_handle->tx_semp);
    return (result == HAL_OK);
}

/**
 * @brief Prints information about the specified Task.
 *
 * @param cmd_para Command parameter
 */
static void _printf_task_info(char *cmd_para)
{
    TaskHandle_t TaskHandle = xTaskGetHandle(cmd_para);
    if (!TaskHandle)
    {
        printf_uart("No task is named: %s\r\n", cmd_para);
    }
    else
    {
        TaskStatus_t TaskStatus;
        vTaskGetInfo(TaskHandle, &TaskStatus, pdTRUE, eInvalid);
        printf_uart("The task infomation: \r\n");
        printf_uart("\rThe task name: %s\r\n", TaskStatus.pcTaskName);
        printf_uart("\rThe task number: %d\r\n", (int)TaskStatus.xTaskNumber);
        printf_uart("\rThe task state: %d\r\n", TaskStatus.eCurrentState);
        printf_uart("\rThe task priority: %d\r\n", (int)TaskStatus.uxCurrentPriority);
        printf_uart("\rThe task base priority: %d\r\n", (int)TaskStatus.uxBasePriority);
        printf_uart("\rThe task total run time allocated to the task so far: %d\r\n", (int)TaskStatus.ulRunTimeCounter);
        printf_uart("\rThe task stack base address: %#x\r\n", (int)TaskStatus.pxStackBase);
        printf_uart("\rThe task stack high water mark: %d\r\n", TaskStatus.usStackHighWaterMark);
    }
}

#endif

#endif
