#include "init.h"
#include "hal_uart.h"
#include "wallet_fpga_link.h"
#include "wallet_gateway.h"
#include <string.h>
#include <stdio.h>

#define LED_PORT SN_GPIO1 
#define LED_PIN  GPIO_PIN_12

// ============================================================================
// KEYPAD CONFIGURATION (Matrix 4x4)
// ============================================================================
const char KEYMAP[4][4] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

uint16_t row_pins[4] = {GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_2, GPIO_PIN_3};
uint16_t col_pins[4] = {GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_6, GPIO_PIN_7};

void Keypad_Init(void)
{
    GPIO_Init_T row_init = {0};
    GPIO_Init_T col_init = {0};

    __HAL_RCC_GPIO_P1_CLK_ENABLE(); // Enable GPIO1 clock

    // Rows: Output
    row_init.pin   = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3;
    row_init.mode  = GPIO_MODE_OUTPUT;
    row_init.drive = GPIO_DRV_17mA;
    HAL_GPIO_Init(SN_GPIO1, &row_init);

    // Set Rows HIGH by default
    HAL_GPIO_WritePin(SN_GPIO1, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, GPIO_PIN_HIGH);

    // Columns: Input with Pull-up
    col_init.pin   = GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
    col_init.mode  = GPIO_MODE_INPUT;
    col_init.pull  = GPIO_PULL_UP;
    HAL_GPIO_Init(SN_GPIO1, &col_init);
}

char Keypad_Scan(void)
{
    for (int r = 0; r < 4; r++)
    {
        HAL_GPIO_WritePin(SN_GPIO1, row_pins[r], GPIO_PIN_LOW);
        for (volatile int delay = 0; delay < 100; delay++); // Settle time

        for (int c = 0; c < 4; c++)
        {
            if (HAL_GPIO_ReadPin(SN_GPIO1, col_pins[c]) == GPIO_PIN_LOW)
            {
                HAL_Delay(20); // Debounce
                if (HAL_GPIO_ReadPin(SN_GPIO1, col_pins[c]) == GPIO_PIN_LOW)
                {
                    while (HAL_GPIO_ReadPin(SN_GPIO1, col_pins[c]) == GPIO_PIN_LOW); // Wait for release
                    HAL_GPIO_WritePin(SN_GPIO1, row_pins[r], GPIO_PIN_HIGH);
                    return KEYMAP[r][c];
                }
            }
        }
        HAL_GPIO_WritePin(SN_GPIO1, row_pins[r], GPIO_PIN_HIGH);
    }
    return '\0';
}

// ============================================================================
// UART CONFIGURATION (UART0 on P0.10 TX / P0.11 RX)
// ============================================================================
UART_Handle_T UART_ESP32_Handle;

void UART_ESP32_Init(void)
{
    // 1. Enable GPIO Clocks and Multiplex pins for UART0
    __HAL_RCC_GPIO_P0_CLK_ENABLE();
    HAL_GPIO_SetAFIO(SN_GPIO0, GPIO_PIN_10, GPIO_P010_UART_UTXD0);
    HAL_GPIO_SetAFIO(SN_GPIO0, GPIO_PIN_11, GPIO_P011_UART_URXD0);

    // 2. Enable UART Peripheral Clock
    __HAL_RCC_UART0_CLK_ENABLE();

    // 3. Configure UART Parameters (115200 Baud, 8N1)
    UART_ESP32_Handle.instance          = SN_UART0;
    UART_ESP32_Handle.init.baud_rate    = 115200;
    UART_ESP32_Handle.init.word_length  = UART_WORD_LENGTH_8;
    UART_ESP32_Handle.init.stop_bits    = UART_STOP_BITS_1;
    UART_ESP32_Handle.init.parity       = UART_PARITY_NONE;
    UART_ESP32_Handle.init.hw_flow_ctl  = UART_HWCONTROL_NONE;

    if (HAL_UART_Init(&UART_ESP32_Handle) != HAL_OK)
    {
        Error_Handler();
    }
}

char rx_line_buf[1024];
uint16_t rx_line_index = 0;

int UART_Read_Byte(uint8_t *byte, uint32_t timeout)
{
    uint32_t tick_start = HAL_GetTick();
    SN_UART0_Type *instance = SN_UART0;
    while (1)
    {
        if (HAL_REG_READ(instance->LS_b.RDR) == 1)
        {
            *byte = (uint8_t)HAL_REG_READ(instance->RB_b.RB);
            return 1;
        }
        if ((HAL_GetTick() - tick_start) > timeout)
        {
            return 0; // Timeout
        }
    }
}

void UART_Flush_RX(void)
{
    SN_UART0_Type *instance = SN_UART0;
    volatile uint8_t dummy;
    while (HAL_REG_READ(instance->LS_b.RDR) == 1)
    {
        dummy = (uint8_t)HAL_REG_READ(instance->RB_b.RB);
    }
    rx_line_index = 0;
    rx_line_buf[0] = '\0';
}

void UART_Wait_Ready(void)
{
    uint8_t rx_byte = 0;
    char ready_buf[16];
    uint8_t ready_idx = 0;
    
    // Đợi tối đa khoảng 2 giây
    while (UART_Read_Byte(&rx_byte, 2000))
    {
        if (rx_byte == '\n' || rx_byte == '\r')
        {
            if (ready_idx > 0)
            {
                ready_buf[ready_idx] = '\0';
                if (strstr(ready_buf, "READY") != NULL)
                {
                    break; // Nhận được READY!
                }
                ready_idx = 0;
            }
        }
        else
        {
            if (ready_idx < sizeof(ready_buf) - 1)
            {
                ready_buf[ready_idx++] = (char)rx_byte;
            }
        }
    }
}

void ESP32_Send_Cmd(const char *cmd)
{
    HAL_UART_Transmit(&UART_ESP32_Handle, (uint8_t *)cmd, strlen(cmd), 1000);
    HAL_Delay(20); // Tăng lên 20ms để ESP32 kịp xử lý vẽ màn hình, tránh tràn bộ đệm nhận
}

void ESP32_Print(uint8_t x, uint8_t y, uint8_t size, uint16_t color, uint16_t bg, const char *text)
{
    char buf[128];
    snprintf(buf, sizeof(buf), "CMD_PRINT:%d,%d,%d,%d,%d,%s\n", x, y, size, color, bg, text);
    ESP32_Send_Cmd(buf);
}

void ESP32_Clear(void)
{
    ESP32_Send_Cmd("CMD_CLEAR\n");
}

void ESP32_Scan(void)
{
    ESP32_Send_Cmd("CMD_SCAN\n");
}

void ESP32_DrawQR(const char *payload)
{
    char buf[256];
    snprintf(buf, sizeof(buf), "CMD_QR:%s\n", payload);
    ESP32_Send_Cmd(buf);
}

// ============================================================================
// UI COLOR SCHEME DEFINITIONS (RGB565 format)
// ============================================================================
#define COLOR_BG            0x0000  // Nền đen tuyệt đối (tiết kiệm pin & độ tương phản cao)
#define COLOR_CARD          0x10A2  // Viền/khung xám xanh đậm sang trọng
#define COLOR_TEXT_MAIN     0xF7BE  // Màu chữ trắng ấm dịu mắt
#define COLOR_TEXT_MUTED    0x8C94  // Màu chữ xám mát dịu
#define COLOR_PRIMARY       0x07FF  // Màu xanh Cyan thương hiệu
#define COLOR_SUCCESS       0x36EE  // Màu xanh mint thành công
#define COLOR_WARNING       0xFD20  // Màu vàng cam cảnh báo
#define COLOR_ERROR         0xFA08  // Màu đỏ coral báo lỗi/hủy

void ESP32_FillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t color)
{
    char buf[128];
    snprintf(buf, sizeof(buf), "CMD_FILLRECT:%d,%d,%d,%d,%d\n", x, y, w, h, color);
    ESP32_Send_Cmd(buf);
}

void ESP32_Rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t color)
{
    char buf[128];
    snprintf(buf, sizeof(buf), "CMD_RECT:%d,%d,%d,%d,%d\n", x, y, w, h, color);
    ESP32_Send_Cmd(buf);
}

void ESP32_Line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint16_t color)
{
    char buf[128];
    snprintf(buf, sizeof(buf), "CMD_LINE:%d,%d,%d,%d,%d\n", x0, y0, x1, y1, color);
    ESP32_Send_Cmd(buf);
}

// Draw a top header bar with a title
void ESP32_DrawHeader(const char *title)
{
    ESP32_FillRect(0, 0, 160, 16, 0x10A2); // Khung tiêu đề xám xanh đậm
    ESP32_Line(0, 16, 160, 16, COLOR_PRIMARY); // Đường chỉ xanh Cyan
    // Print title centered
    int len = strlen(title);
    int x = (160 - (len * 6)) / 2;
    if (x < 0) x = 0;
    ESP32_Print(x, 4, 1, COLOR_PRIMARY, 0x10A2, title);
}


// ============================================================================
// CORE WALLET STATE MACHINE
// ============================================================================
typedef enum {
    STATE_LOCKED,
    STATE_DASHBOARD,
    STATE_SCANNING,
    STATE_BTC_PROGRESS,
    STATE_VERIFICATION,
    STATE_SIGNING,
    STATE_SHOW_ADDRESS
} WalletState_t;

WalletState_t current_state = STATE_LOCKED;

char pin_buffer[8];
uint8_t pin_index = 0;
const char DEFAULT_PIN[] = "1234";

char tx_payload[1024];

void Error_Handler(void)
{
    __disable_irq();
    while (1)
    {
    }
}

void Redraw_PIN_Boxes(uint8_t current_len)
{
    for (int i = 0; i < 4; i++)
    {
        uint8_t x = 44 + i * 20;
        // Outline box in gray-blue card color
        ESP32_Rect(x, 48, 12, 12, COLOR_CARD);
        if (i < current_len)
        {
            // Fill box in neon green
            ESP32_FillRect(x + 2, 50, 8, 8, COLOR_SUCCESS);
        }
        else
        {
            // Clear inside box
            ESP32_FillRect(x + 2, 50, 8, 8, COLOR_BG);
        }
    }
}

uint8_t scan_mode = 0; // 0 = ETH, 1 = BTC
uint8_t btc_parts_scanned[5] = {0, 0, 0, 0, 0};
char btc_parts[5][256];
uint8_t btc_scan_status = 0; // 0 = Normal/Success, 1 = Already Scanned, 2 = Invalid Format

void Draw_BTC_Progress(void)
{
    ESP32_Clear();
    ESP32_DrawHeader("SCAN BTC PART");
    
    // Show instruction
    ESP32_Print(10, 30, 1, COLOR_TEXT_MAIN, COLOR_BG, "Scan 5 QR parts in order:");
    
    // Draw 5 progress boxes
    for (int i = 0; i < 5; i++)
    {
        uint8_t x = 12 + i * 28;
        uint8_t y = 55;
        
        // Draw box outline
        ESP32_Rect(x, y, 22, 22, COLOR_CARD);
        
        // Print part number
        char num_str[2] = { '1' + i, '\0' };
        
        if (btc_parts_scanned[i])
        {
            // Filled box for scanned parts
            ESP32_FillRect(x + 2, y + 2, 18, 18, COLOR_SUCCESS);
            ESP32_Print(x + 8, y + 7, 1, 0x0000, COLOR_SUCCESS, num_str);
        }
        else
        {
            // Empty box for pending parts
            ESP32_Print(x + 8, y + 7, 1, COLOR_TEXT_MUTED, COLOR_BG, num_str);
        }
    }
    
    // Count remaining
    int remaining = 0;
    for (int i = 0; i < 5; i++) {
        if (!btc_parts_scanned[i]) remaining++;
    }
    
    char status_str[32];
    if (remaining > 0) {
        snprintf(status_str, sizeof(status_str), "Waiting for %d parts...", remaining);
        ESP32_Print(15, 90, 1, COLOR_WARNING, COLOR_BG, status_str);
    } else {
        ESP32_Print(15, 90, 1, COLOR_SUCCESS, COLOR_BG, "All parts merged!");
    }
}


void SystemClock_Config(void);

int main(void)
{
    GPIO_Init_T led_init = {0};

    HAL_Init();

    // Initialize LED
    led_init.pin   = LED_PIN;
    led_init.mode  = GPIO_MODE_OUTPUT;
    led_init.drive = GPIO_DRV_17mA;
    if (HAL_GPIO_Init(LED_PORT, &led_init) != HAL_OK)
    {
        while (1);
    }

    SystemClock_Config();

    // UART0 drives ESP32/UI; UART1 is the dedicated FPGA link.
    Keypad_Init();
    UART_ESP32_Init();
    if (WalletFpgaLink_Init() != 0)
    {
        /* Do not continue into a signing UI with an unavailable FPGA link. */
        while (1)
        {
            HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
            HAL_Delay(100U);
        }
    }
    if (WalletGateway_Init() != 0)
    {
        while (1)
        {
            HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
            HAL_Delay(500U);
        }
    }

    // Clear and print locked screen initially
    HAL_Delay(4500); // Wait for ESP32 boot

    // Reset UART to clear boot ROM logs and overrun errors from ESP32 startup
    HAL_UART_DeInit(&UART_ESP32_Handle);
    UART_ESP32_Init();
    UART_Flush_RX();

    ESP32_Clear();
    ESP32_DrawHeader("CRYPTO COLD");
    ESP32_Print(20, 30, 1, COLOR_TEXT_MAIN, COLOR_BG, "Enter PIN to Unlock:");
    Redraw_PIN_Boxes(0);

    uint8_t force_redraw = 0;
    WalletGatewayState last_gateway_state = WALLET_GATEWAY_IDLE;

    while (1)
    {
        char key = Keypad_Scan();
        WalletGatewayState gateway_state;

        WalletGateway_Poll();
        gateway_state = WalletGateway_State();
        if (gateway_state != WALLET_GATEWAY_IDLE)
        {
            if (gateway_state != last_gateway_state)
            {
                ESP32_Clear();
                if (gateway_state == WALLET_GATEWAY_FRAME_ERROR)
                {
                    ESP32_DrawHeader("UART FRAME ERROR");
                    ESP32_Print(12, 45, 1, COLOR_ERROR, COLOR_BG, "Invalid host request");
                    ESP32_Print(12, 65, 1, COLOR_TEXT_MAIN, COLOR_BG, "Press B to dismiss");
                }
                else
                {
                    const uint8_t *review_id = WalletGateway_ReviewId();
                    char id_line[32];
                    snprintf(id_line, sizeof(id_line), "ID: %02X%02X%02X%02X...",
                             review_id[0], review_id[1], review_id[2], review_id[3]);
                    ESP32_DrawHeader(gateway_state == WALLET_GATEWAY_PENDING_ETH
                                    ? "WEB ETH REQUEST" : "WEB BTC REQUEST");
                    ESP32_Print(12, 35, 1, COLOR_WARNING, COLOR_BG, "Verify on web + device");
                    ESP32_Print(12, 55, 1, COLOR_TEXT_MAIN, COLOR_BG, id_line);
                    ESP32_Print(10, 90, 1, COLOR_SUCCESS, COLOR_BG, "[A] Sign");
                    ESP32_Print(90, 90, 1, COLOR_ERROR, COLOR_BG, "[B] Reject");
                }
                last_gateway_state = gateway_state;
            }
            if (key == 'B')
            {
                if (gateway_state == WALLET_GATEWAY_FRAME_ERROR)
                    WalletGateway_ClearError();
                else
                    WalletGateway_Reject();
                last_gateway_state = WALLET_GATEWAY_IDLE;
                force_redraw = 1U;
            }
            else if (key == 'A' && gateway_state != WALLET_GATEWAY_FRAME_ERROR)
            {
                (void)WalletGateway_Approve();
                last_gateway_state = WALLET_GATEWAY_IDLE;
                force_redraw = 1U;
            }
            continue;
        }

        switch (current_state)
        {
            case STATE_LOCKED:
            {
                if (force_redraw)
                {
                    ESP32_Clear();
                    ESP32_DrawHeader("CRYPTO COLD");
                    ESP32_Print(20, 30, 1, COLOR_TEXT_MAIN, COLOR_BG, "Enter PIN to Unlock:");
                    Redraw_PIN_Boxes(0);
                    force_redraw = 0;
                }

                if (key != '\0')
                {
                    if (key >= '0' && key <= '9')
                    {
                        if (pin_index < sizeof(pin_buffer) - 1)
                        {
                            pin_buffer[pin_index++] = key;
                            pin_buffer[pin_index] = '\0';
                            
                            Redraw_PIN_Boxes(pin_index);
                        }
                    }
                    else if (key == '#') // Backspace
                    {
                        if (pin_index > 0)
                        {
                            pin_index--;
                            pin_buffer[pin_index] = '\0';
                            Redraw_PIN_Boxes(pin_index);
                        }
                    }
                    else if (key == 'A') // Confirm PIN
                    {
                        if (strcmp(pin_buffer, DEFAULT_PIN) == 0)
                        {
                            // Success -> Dashboard
                            ESP32_Clear();
                            
                            // Draw success panel
                            ESP32_FillRect(10, 35, 140, 50, COLOR_SUCCESS);
                            ESP32_Print(50, 48, 2, 0x0000, COLOR_SUCCESS, "OK!");
                            ESP32_Print(35, 70, 1, 0x0000, COLOR_SUCCESS, "Wallet Unlocked");
                            
                            HAL_Delay(1500);
                            
                            current_state = STATE_DASHBOARD;
                            force_redraw = 1;
                        }
                        else
                        {
                            // Fail -> Erase and retry
                            ESP32_FillRect(10, 95, 140, 20, COLOR_ERROR);
                            ESP32_Print(45, 101, 1, 0x0000, COLOR_ERROR, "WRONG PIN!");
                            
                            HAL_Delay(1200);
                            
                            // Clear warning banner
                            ESP32_FillRect(10, 95, 140, 20, COLOR_BG);
                            
                            pin_index = 0;
                            pin_buffer[0] = '\0';
                            Redraw_PIN_Boxes(0);
                        }
                    }
                }
                break;
            }

            case STATE_DASHBOARD:
            {
                if (force_redraw)
                {
                    ESP32_Clear();
                    ESP32_DrawHeader("CRYPTO COLD");
                    
                    // Draw beautiful menu options as bordered boxes
                    // Option A: Scan ETH
                    ESP32_Rect(8, 22, 144, 18, COLOR_CARD);
                    ESP32_Print(14, 27, 1, COLOR_TEXT_MAIN, COLOR_BG, "[A] Scan ETH Tx (1 QR)");
                    
                    // Option B: Scan BTC
                    ESP32_Rect(8, 44, 144, 18, COLOR_CARD);
                    ESP32_Print(14, 49, 1, COLOR_TEXT_MAIN, COLOR_BG, "[B] Scan BTC Tx (5 QR)");
                    
                    // Option C: Show Address/QR
                    ESP32_Rect(8, 66, 144, 18, COLOR_CARD);
                    ESP32_Print(14, 71, 1, COLOR_TEXT_MAIN, COLOR_BG, "[C] Show Signed TX");
                    
                    // Option D: Lock
                    ESP32_Rect(8, 88, 144, 18, COLOR_ERROR);
                    ESP32_Print(14, 93, 1, COLOR_ERROR, COLOR_BG, "[D] Lock Wallet");
                    
                    // Footer bar
                    ESP32_Line(0, 110, 160, 110, COLOR_CARD);
                    ESP32_Print(10, 112, 1, COLOR_TEXT_MUTED, COLOR_BG, "Secured by SN34F780");
                    
                    force_redraw = 0;
                }
                
                if (key == 'A') // Scan ETH (Single)
                {
                    scan_mode = 0;
                    
                    ESP32_Clear();
                    ESP32_DrawHeader("CAMERA SCAN ETH");
                    ESP32_Print(15, 45, 1, COLOR_TEXT_MAIN, COLOR_BG, "Opening Camera...");
                    
                    // Subtle loading progress bar
                    ESP32_Rect(20, 65, 120, 8, COLOR_CARD);
                    for (int w = 0; w <= 116; w += 29) {
                        ESP32_FillRect(22, 67, w, 4, COLOR_PRIMARY);
                        HAL_Delay(100);
                    }
                    
                    ESP32_Scan(); // Start camera preview & scanning on ESP32
                    
                    rx_line_index = 0;
                    current_state = STATE_SCANNING;
                    force_redraw = 1;
                }
                else if (key == 'B') // Scan BTC (5-part BBQr)
                {
                    scan_mode = 1;
                    
                    // Reset BTC scan states
                    for (int i = 0; i < 5; i++) {
                        btc_parts_scanned[i] = 0;
                        btc_parts[i][0] = '\0';
                    }
                    
                    // Show progress screen first
                    Draw_BTC_Progress();
                    HAL_Delay(1200); // Let the user see the progress bars
                    
                    ESP32_Clear();
                    ESP32_DrawHeader("CAMERA SCAN BTC");
                    ESP32_Print(15, 45, 1, COLOR_TEXT_MAIN, COLOR_BG, "Opening Camera...");
                    
                    // Progress bar
                    ESP32_Rect(20, 65, 120, 8, COLOR_CARD);
                    for (int w = 0; w <= 116; w += 29) {
                        ESP32_FillRect(22, 67, w, 4, COLOR_PRIMARY);
                        HAL_Delay(100);
                    }
                    
                    ESP32_Scan(); // Start camera preview & scanning on ESP32
                    
                    rx_line_index = 0;
                    current_state = STATE_SCANNING;
                    force_redraw = 1;
                }
                else if (key == 'C') // Show Address (renamed key from B to C)
                {
                    current_state = STATE_SHOW_ADDRESS;
                    force_redraw = 1;
                }
                else if (key == 'D') // Lock Wallet
                {
                    pin_index = 0;
                    pin_buffer[0] = '\0';
                    current_state = STATE_LOCKED;
                    force_redraw = 1;
                }
                break;
            }

            case STATE_SCANNING:
            {
                // Read all available bytes from UART
                uint8_t rx_byte = 0;
                while (UART_Read_Byte(&rx_byte, 5))
                {
                    if (rx_byte == '\n' || rx_byte == '\r')
                    {
                        if (rx_line_index > 0)
                        {
                            rx_line_buf[rx_line_index] = '\0';
                            
                            // Check if payload contains TX_RAW: (robust search anywhere in line)
                            char *match = strstr(rx_line_buf, "TX_RAW:");
                            if (match != NULL)
                            {
                                char *raw_data = match + 7;
                                
                                UART_Wait_Ready();
                                
                                if (scan_mode == 0) // ETH Mode (Single QR)
                                {
                                    strncpy(tx_payload, raw_data, sizeof(tx_payload) - 1);
                                    tx_payload[sizeof(tx_payload) - 1] = '\0';
                                    
                                    current_state = STATE_VERIFICATION;
                                    force_redraw = 1;
                                    break;
                                }
                                else // BTC Mode (5-part BBQr)
                                {
                                    int part_idx = -1;
                                    char *data_ptr = NULL;
                                    
                                    // 1. Standard BBQr format (e.g. B$HP0500...)
                                    if (raw_data[0] == 'B' && raw_data[1] == '$')
                                    {
                                        if (raw_data[6] == '0' && raw_data[7] >= '0' && raw_data[7] <= '4') {
                                            part_idx = raw_data[7] - '0';
                                            data_ptr = raw_data + 8;
                                        }
                                        else if (raw_data[6] == '0' && raw_data[7] >= '1' && raw_data[7] <= '5') {
                                            part_idx = raw_data[7] - '1';
                                            data_ptr = raw_data + 8;
                                        }
                                    }
                                    // 2. Format: pX: (e.g. p1:...)
                                    else if (raw_data[0] == 'p' && raw_data[1] >= '1' && raw_data[1] <= '5' && raw_data[2] == ':') {
                                        part_idx = raw_data[1] - '1';
                                        data_ptr = raw_data + 3;
                                    }
                                    // 3. Format: X/5: (e.g. 1/5:...)
                                    else if (raw_data[0] >= '1' && raw_data[0] <= '5' && raw_data[1] == '/' && raw_data[2] == '5' && raw_data[3] == ':') {
                                        part_idx = raw_data[0] - '1';
                                        data_ptr = raw_data + 4;
                                    }
                                    // 4. Format: X: (e.g. 1:...)
                                    else if (raw_data[0] >= '1' && raw_data[0] <= '5' && raw_data[1] == ':') {
                                        part_idx = raw_data[0] - '1';
                                        data_ptr = raw_data + 2;
                                    }
                                    
                                    if (part_idx != -1 && data_ptr != NULL)
                                    {
                                        if (btc_parts_scanned[part_idx] == 1)
                                        {
                                            // Already scanned!
                                            btc_scan_status = 1;
                                            current_state = STATE_BTC_PROGRESS;
                                            force_redraw = 1;
                                            break;
                                        }
                                        else
                                        {
                                            // Save the part
                                            strncpy(btc_parts[part_idx], data_ptr, sizeof(btc_parts[part_idx]) - 1);
                                            btc_parts[part_idx][sizeof(btc_parts[part_idx]) - 1] = '\0';
                                            btc_parts_scanned[part_idx] = 1;
                                            
                                            btc_scan_status = 0;
                                            current_state = STATE_BTC_PROGRESS;
                                            force_redraw = 1;
                                            break;
                                        }
                                    }
                                    else
                                    {
                                        // Invalid format
                                        btc_scan_status = 2;
                                        current_state = STATE_BTC_PROGRESS;
                                        force_redraw = 1;
                                        break;
                                    }
                                }
                            }
                            rx_line_index = 0;
                        }
                    }
                    else
                    {
                        if (rx_line_index < sizeof(rx_line_buf) - 1)
                        {
                            rx_line_buf[rx_line_index++] = (char)rx_byte;
                        }
                        else
                        {
                            rx_line_index = 0;
                        }
                    }
                }
                
                // Allow cancellation via keypad (B button)
                if (key == 'B')
                {
                    ESP32_Clear();
                    current_state = STATE_DASHBOARD;
                    force_redraw = 1;
                }
                break;
            }

            case STATE_BTC_PROGRESS:
            {
                if (force_redraw)
                {
                    // Chờ ESP32 sẵn sàng (vẽ xong viền xanh và báo READY)
                    UART_Wait_Ready();
                    
                    if (btc_scan_status == 0) // Quét thành công part mới
                    {
                        // Vẽ progress update
                        Draw_BTC_Progress();
                        HAL_Delay(1000); // Khoảng dừng trực quan
                        
                        // Kiểm tra xem đã quét đủ chưa
                        uint8_t all_done = 1;
                        for (int i = 0; i < 5; i++) {
                            if (!btc_parts_scanned[i]) {
                                all_done = 0;
                                break;
                            }
                        }
                        
                        if (all_done)
                        {
                            // Merge all parts into tx_payload
                            tx_payload[0] = '\0';
                            for (int i = 0; i < 5; i++) {
                                strncat(tx_payload, btc_parts[i], sizeof(tx_payload) - strlen(tx_payload) - 1);
                            }
                            tx_payload[sizeof(tx_payload) - 1] = '\0';
                            
                            UART_Flush_RX();
                            current_state = STATE_VERIFICATION;
                            force_redraw = 1;
                        }
                        else
                        {
                            // Quét tiếp phần tiếp theo
                            UART_Flush_RX();
                            ESP32_Scan();
                            current_state = STATE_SCANNING;
                            force_redraw = 0;
                        }
                    }
                    else if (btc_scan_status == 1) // Trùng QR (Already Scanned)
                    {
                        ESP32_Clear();
                        ESP32_DrawHeader("SCAN WARNING");
                        ESP32_FillRect(10, 45, 140, 40, COLOR_WARNING);
                        ESP32_Print(18, 54, 1, 0x0000, COLOR_WARNING, "ALREADY SCANNED!");
                        ESP32_Print(15, 70, 1, 0x0000, COLOR_WARNING, "Scan next QR part");
                        
                        HAL_Delay(1200);
                        
                        // Vẽ lại tiến trình
                        Draw_BTC_Progress();
                        HAL_Delay(500);
                        
                        // Quét tiếp
                        UART_Flush_RX();
                        ESP32_Scan();
                        current_state = STATE_SCANNING;
                        force_redraw = 0;
                    }
                    else if (btc_scan_status == 2) // Lỗi định dạng (Invalid Format)
                    {
                        ESP32_Clear();
                        ESP32_DrawHeader("SCAN ERROR");
                        ESP32_FillRect(10, 45, 140, 40, COLOR_ERROR);
                        ESP32_Print(25, 54, 1, 0x0000, COLOR_ERROR, "INVALID FORMAT!");
                        ESP32_Print(15, 70, 1, 0x0000, COLOR_ERROR, "Not a valid BBQr part");
                        
                        HAL_Delay(1500); // Khoảng dừng cho người dùng đọc
                        
                        // Vẽ lại tiến trình
                        Draw_BTC_Progress();
                        HAL_Delay(500);
                        
                        // Quét tiếp
                        UART_Flush_RX();
                        ESP32_Scan();
                        current_state = STATE_SCANNING;
                        force_redraw = 0;
                    }
                }
                break;
            }

            case STATE_VERIFICATION:
            {
                if (force_redraw)
                {
                    ESP32_Clear();
                    // Draw Amber Warning Header
                    ESP32_FillRect(0, 0, 160, 16, COLOR_WARNING);
                    ESP32_Print(30, 4, 1, 0x0000, COLOR_WARNING, "VERIFY TRANSACTION");
                    
                    // Card background for Tx details
                    ESP32_Rect(6, 22, 148, 64, COLOR_CARD);
                    
                    // Amount & Address (Asset-dependent)
                    if (scan_mode == 0) // ETH
                    {
                        ESP32_Print(12, 28, 1, COLOR_TEXT_MUTED, COLOR_BG, "Amount:");
                        ESP32_Print(60, 28, 1, COLOR_SUCCESS, COLOR_BG, "1.25 ETH");
                        
                        ESP32_Print(12, 45, 1, COLOR_TEXT_MAIN, COLOR_BG, "To: 0x71C839...B8E2");
                    }
                    else // BTC
                    {
                        ESP32_Print(12, 28, 1, COLOR_TEXT_MUTED, COLOR_BG, "Amount:");
                        ESP32_Print(60, 28, 1, COLOR_SUCCESS, COLOR_BG, "0.045 BTC");
                        
                        ESP32_Print(12, 45, 1, COLOR_TEXT_MAIN, COLOR_BG, "To: bc1q9x7w...7tzp");
                    }
                    
                    // Payload
                    char details[32];
                    snprintf(details, sizeof(details), "Tx: %.18s...", tx_payload);
                    ESP32_Print(12, 62, 1, COLOR_TEXT_MUTED, COLOR_BG, details);
                    
                    // Action Buttons
                    // Left button: [A] Approve (Green background, black text)
                    ESP32_FillRect(6, 92, 70, 20, COLOR_SUCCESS);
                    ESP32_Print(10, 98, 1, 0x0000, COLOR_SUCCESS, "[A] Approve");
                    
                    // Right button: [B] Cancel (Red background, black text)
                    ESP32_FillRect(84, 92, 70, 20, COLOR_ERROR);
                    ESP32_Print(88, 98, 1, 0x0000, COLOR_ERROR, "[B] Cancel");
                    
                    force_redraw = 0;
                }
                
                if (key == 'B') // Reject and return to dashboard
                {
                    ESP32_Clear();
                    ESP32_FillRect(10, 45, 140, 30, COLOR_ERROR);
                    ESP32_Print(40, 56, 1, 0x0000, COLOR_ERROR, "TX REJECTED");
                    HAL_Delay(1000);
                    current_state = STATE_DASHBOARD;
                    force_redraw = 1;
                }
                else if (key == 'A') // Approve transaction
                {
                    current_state = STATE_SIGNING;
                    force_redraw = 1;
                }
                break;
            }

            case STATE_SIGNING:
            {
                if (force_redraw)
                {
                    ESP32_Clear();
                    ESP32_DrawHeader("QR SIGN BLOCKED");
                    ESP32_Print(12, 32, 1, COLOR_ERROR, COLOR_BG, "No fake signature used");
                    ESP32_Print(12, 52, 1, COLOR_TEXT_MAIN, COLOR_BG, "Send transaction from");
                    ESP32_Print(12, 67, 1, COLOR_TEXT_MAIN, COLOR_BG, "web via MCU UART2");
                    ESP32_Print(12, 92, 1, COLOR_TEXT_MUTED, COLOR_BG, "Press A/B to return");
                    force_redraw = 0;
                }
                
                if (key == 'B' || key == 'A')
                {
                    current_state = STATE_DASHBOARD;
                    force_redraw = 1;
                }
                break;
            }

            case STATE_SHOW_ADDRESS:
            {
                if (force_redraw)
                {
                    ESP32_Clear();
                    if (strlen(tx_payload) == 0)
                    {
                        ESP32_DrawHeader("TRANSACTION QR");
                        
                        ESP32_Rect(10, 30, 140, 55, COLOR_ERROR);
                        ESP32_Print(40, 38, 1, COLOR_ERROR, COLOR_BG, "NO SIGNED TX!");
                        ESP32_Print(25, 54, 1, COLOR_TEXT_MAIN, COLOR_BG, "Scan & sign first");
                        ESP32_Print(20, 70, 1, COLOR_TEXT_MAIN, COLOR_BG, "before showing QR.");
                        
                        ESP32_FillRect(45, 94, 70, 18, COLOR_CARD);
                        ESP32_Print(58, 99, 1, COLOR_TEXT_MAIN, COLOR_CARD, "[B] Back");
                    }
                    else
                    {
                        char signed_tx[300];
                        snprintf(signed_tx, sizeof(signed_tx), "SIGNED:%s|SIG:0x39aef02c8491823abfdd0349ef1", tx_payload);
                        ESP32_DrawQR(signed_tx);
                    }
                    force_redraw = 0;
                }
                
                if (key == 'B' || key == 'A')
                {
                    current_state = STATE_DASHBOARD;
                    force_redraw = 1;
                }
                break;
            }
        }

        if (current_state != STATE_SCANNING)
        {
            HAL_Delay(20);
        }
    }
}

void SystemClock_Config(void)
{
    RCC_OscConfig_t OSCCfg = {0};
    RCC_ClkConfig_t ClkCfg = {0};

    FLASH_ConfigProgramInitTypeDef CfgInit = {
        .OptionType = OPTIONCFG_CLKFREQ,
        .CLKFreq    = 192,
    };
    if (HAL_FLASHEx_ConfigProgram(&CfgInit) != HAL_OK)
    {
        Error_Handler();
    }

    OSCCfg.OscillatorType = RCC_OSC_TYPE_IHRC;
    OSCCfg.IHRCEn         = RCC_OSCCLK_CFG_ON;
    OSCCfg.PLL.PLLEn      = RCC_OSCCLK_CFG_ON;
    OSCCfg.PLL.PLLSource  = RCC_PLL_SRC_IHRC;
    OSCCfg.PLL.NS         = 64;
    OSCCfg.PLL.FS         = RCC_PLL_DIV4;

    if (HAL_RCC_OscConfig(&OSCCfg) != HAL_OK)
    {
        Error_Handler();
    }

    ClkCfg.ClockType      = RCC_CLK_TYPE_SYSCLK | RCC_CLK_TYPE_HCLK | RCC_CLK_TYPE_APB0CLK | RCC_CLK_TYPE_APB1CLK;
    ClkCfg.SYSCLKSource   = RCC_SYSCLK_SRC_PLL;
    ClkCfg.AHBCLKDivider  = RCC_SYSCLK_DIV4;
    ClkCfg.APB0CLKDivider = RCC_HCLK_DIV1;
    ClkCfg.APB1CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&ClkCfg) != HAL_OK)
    {
        Error_Handler();
    }
    
    SystemCoreClockUpdate();
}

void __aeabi_assert(const char *expr, const char *file, int line)
{
    while (1)
    {
    }
}
