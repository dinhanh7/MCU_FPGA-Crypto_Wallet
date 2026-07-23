/**
 * ESP32-CAM to ST7735S TFT Display Live Preview & QR Decoder
 * 
 * KHÔNG cần cài đặt bất kỳ thư viện hiển thị nào (Không cần Adafruit GFX, Adafruit ST7735).
 * Sử dụng thư viện giải mã QR quirc được tích hợp sẵn trong thư mục dự án.
 * 
 * Sơ đồ đấu nối dây (Wiring):
 * Mạch nạp thẻ nhớ SD trên ESP32-CAM sẽ bị vô hiệu hóa để lấy chân chạy màn hình SPI.
 * 
 *   ST7735S TFT      <--->    ESP32-CAM Pin
 *   ----------------------------------------
 *   GND              <--->    GND
 *   VCC              <--->    5V (hoặc 3.3V tùy màn hình)
 *   SCL (SCK)        <--->    GPIO 14
 *   SDA (MOSI)       <--->    GPIO 13
 *   CS               <--->    GPIO 15
 *   DC (A0)          <--->    GPIO 12
 *   RESET (RST)      <--->    GPIO 2
 * 
 * Giao tiếp UART với vi điều khiển Sonix SN34F788F:
 *   ESP32-CAM TX (GPIO 1)  ----->   Sonix RX (UART RX pin)
 *   ESP32-CAM GND          <---->   Sonix GND
 *   (Sử dụng chung cổng Serial mặc định tốc độ 115200 bps để gửi chuỗi giải mã)
 */

#include "esp_camera.h"
#include <SPI.h>
#include "quirc.h" // Nhập thư viện quét QR quirc bản địa
#include "qrcode.h" // Thư viện tạo QR code bản địa
#define GLCDFONTDECL(x) const uint8_t x[]
#include "System5x7.h" // Font chữ hiển thị trên màn hình

// -----------------------------------------------------------------------------
// 1. Cấu hình chân cho màn hình TFT ST7735S
// -----------------------------------------------------------------------------
#define TFT_SCK   14  // Chân Clock SPI (Trùng chân CLK thẻ SD)
#define TFT_MOSI  13  // Chân Data Out SPI (Trùng chân CMD thẻ SD)
#define TFT_CS    15  // Chân Chip Select (Trùng chân D3 thẻ SD)
#define TFT_DC    12  // Chân Data/Command (Trùng chân D2 thẻ SD)
#define TFT_RST    2  // Chân Reset màn hình (Trùng chân D0 thẻ SD)

// Các lệnh điều khiển chip ST7735S
#define ST7735_SWRESET 0x01
#define ST7735_SLPOUT  0x11
#define ST7735_COLMOD  0x3A
#define ST7735_MADCTL  0x36
#define ST7735_DISPON  0x29
#define ST7735_CASET   0x2A
#define ST7735_RASET   0x2B
#define ST7735_RAMWR   0x2C

// -----------------------------------------------------------------------------
// 2. Cấu hình chân camera cho AI-Thinker ESP32-CAM
// -----------------------------------------------------------------------------
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27

#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

// Cấu hình tính năng đổi màu (Byte Swap)
// Nếu màu màn hình bị ngược (ví dụ: da người bị xanh dương hoặc màu sai lệch),
// hãy chuyển biến này thành true.
const bool ENABLE_BYTE_SWAP = false; 

// Cấu hình lật camera 180 độ để đồng bộ với hướng màn hình
const bool ENABLE_VFLIP = true;
const bool ENABLE_HMIRROR = true; 

// Đối tượng giải mã QR
struct quirc *q = NULL;

// Bảng tra cứu (LUT) để chuyển đổi nhanh Grayscale sang RGB565 (đã hoán đổi byte)
uint16_t gray_to_rgb565_lut[256];

// -----------------------------------------------------------------------------
// 3. Driver điều khiển ST7735S siêu nhẹ (Low-level Driver)
// -----------------------------------------------------------------------------
void writeCommand(uint8_t cmd) {
  digitalWrite(TFT_DC, LOW);
  digitalWrite(TFT_CS, LOW);
  SPI.transfer(cmd);
  digitalWrite(TFT_CS, HIGH);
}

void writeData(uint8_t data) {
  digitalWrite(TFT_DC, HIGH);
  digitalWrite(TFT_CS, LOW);
  SPI.transfer(data);
  digitalWrite(TFT_CS, HIGH);
}

void st7735_init() {
  pinMode(TFT_DC, OUTPUT);
  pinMode(TFT_CS, OUTPUT);
  pinMode(TFT_RST, OUTPUT);

  // Reset cứng màn hình bằng phần cứng
  digitalWrite(TFT_RST, HIGH);
  delay(10);
  digitalWrite(TFT_RST, LOW);
  delay(20);
  digitalWrite(TFT_RST, HIGH);
  delay(150);

  writeCommand(ST7735_SWRESET); // Reset phần mềm
  delay(150);

  writeCommand(ST7735_SLPOUT);  // Thoát chế độ Sleep
  delay(120);

  writeCommand(ST7735_COLMOD);  // Thiết lập định dạng màu
  writeData(0x05);             // 16-bit màu (RGB565)
  delay(10);

  writeCommand(ST7735_MADCTL);  // Cấu hình hướng quét màn hình
  writeData(0x60);             // Xoay 180 độ (Landscape), thứ tự màu RGB
  delay(10);

  writeCommand(ST7735_DISPON);  // Bật hiển thị màn hình
  delay(120);
}

void setAddrWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {
  writeCommand(ST7735_CASET);   // Giới hạn cột X
  writeData(0x00);
  writeData(x0);
  writeData(0x00);
  writeData(x1);

  writeCommand(ST7735_RASET);   // Giới hạn hàng Y (Dịch offset 4 dòng để căn giữa)
  writeData(0x00);
  writeData(y0 + 4); 
  writeData(0x00);
  writeData(y1 + 4);

  writeCommand(ST7735_RAMWR);   // Lệnh bắt đầu ghi vào GRAM
}

void drawRGB565_Fast(uint16_t *buf, int w, int h) {
  // Đặt cửa sổ vẽ ảnh
  setAddrWindow(0, 0, w - 1, h - 1);

  digitalWrite(TFT_DC, HIGH); // Chuyển sang chế độ Data
  digitalWrite(TFT_CS, LOW);  // Chọn màn hình

  // Truyền toàn bộ vùng nhớ ảnh qua SPI với tốc độ tối đa của phần cứng
  SPI.writeBytes((uint8_t *)buf, w * h * 2);

  digitalWrite(TFT_CS, HIGH); // Bỏ chọn màn hình
}


// Hàm đẩy ảnh Grayscale (8-bit) lên màn hình TFT ST7735S (Chuyển đổi nhanh bằng LUT)
void drawGrayscale_Fast(uint8_t *gray_buf, int w, int h) {
  setAddrWindow(0, 0, w - 1, h - 1);
  digitalWrite(TFT_DC, HIGH);
  digitalWrite(TFT_CS, LOW);

  uint16_t row_buf[w];
  for (int y = 0; y < h; y++) {
    for (int x = 0; x < w; x++) {
      row_buf[x] = gray_to_rgb565_lut[gray_buf[y * w + x]];
    }
    SPI.writeBytes((uint8_t *)row_buf, w * 2);
  }
  digitalWrite(TFT_CS, HIGH);
}

// Hàm vẽ nhanh một màu đơn lên toàn màn hình (Dùng để báo lỗi hoặc báo quét thành công)
void fillScreenFast(uint16_t color) {
  // Đặt cửa sổ địa chỉ bao phủ toàn bộ 160 cột (0-159) và 128 hàng vật lý (0-127)
  writeCommand(ST7735_CASET);
  writeData(0x00); writeData(0);
  writeData(0x00); writeData(159);

  writeCommand(ST7735_RASET);
  writeData(0x00); writeData(0);
  writeData(0x00); writeData(127);

  writeCommand(ST7735_RAMWR);

  digitalWrite(TFT_DC, HIGH);
  digitalWrite(TFT_CS, LOW);

  uint16_t color_swapped = (color >> 8) | (color << 8);
  uint16_t line_buf[160];
  for(int i = 0; i < 160; i++) line_buf[i] = color_swapped;

  // Ghi toàn bộ 128 dòng bằng 1 phiên truyền dữ liệu duy nhất
  for(int i = 0; i < 128; i++) {
    SPI.writeBytes((uint8_t *)line_buf, 160 * 2);
  }
  digitalWrite(TFT_CS, HIGH);
}


// Vẽ một điểm ảnh (pixel) đơn lẻ lên màn hình TFT
void drawPixel(int16_t x, int16_t y, uint16_t color) {
  if (x < 0 || x >= 160 || y < 0 || y >= 120) return;
  setAddrWindow(x, y, x, y);
  uint16_t color_swapped = (color >> 8) | (color << 8);
  digitalWrite(TFT_DC, HIGH);
  digitalWrite(TFT_CS, LOW);
  SPI.writeBytes((uint8_t *)&color_swapped, 2);
  digitalWrite(TFT_CS, HIGH);
}

// Vẽ một đường thẳng bất kỳ sử dụng thuật toán Bresenham cực nhanh
void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) {
  int16_t dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
  int16_t dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
  int16_t err = dx + dy, e2;
  
  while (true) {
    drawPixel(x0, y0, color);
    if (x0 == x1 && y0 == y1) break;
    e2 = 2 * err;
    if (e2 >= dy) { err += dy; x0 += sx; }
    if (e2 <= dx) { err += dx; y0 += sy; }
  }
}

// Hàm vẽ đường ngang nhanh
void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) {
  if (x < 0 || x >= 160 || y < 0 || y >= 120 || w <= 0) return;
  if (x + w > 160) w = 160 - x;
  setAddrWindow(x, y, x + w - 1, y);
  uint16_t color_swapped = (color >> 8) | (color << 8);
  uint16_t temp[160];
  for (int i = 0; i < w; i++) temp[i] = color_swapped;
  
  digitalWrite(TFT_DC, HIGH);
  digitalWrite(TFT_CS, LOW);
  SPI.writeBytes((uint8_t *)temp, w * 2);
  digitalWrite(TFT_CS, HIGH);
}

// Hàm vẽ đường dọc nhanh
void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {
  if (x < 0 || x >= 160 || y < 0 || y >= 120 || h <= 0) return;
  if (y + h > 120) h = 120 - y;
  setAddrWindow(x, y, x, y + h - 1);
  uint16_t color_swapped = (color >> 8) | (color << 8);
  uint16_t temp[120];
  for (int i = 0; i < h; i++) temp[i] = color_swapped;
  
  digitalWrite(TFT_DC, HIGH);
  digitalWrite(TFT_CS, LOW);
  SPI.writeBytes((uint8_t *)temp, h * 2);
  digitalWrite(TFT_CS, HIGH);
}

// Hàm vẽ hình chữ nhật rỗng
void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
  drawFastHLine(x, y, w, color);
  drawFastHLine(x, y + h - 1, w, color);
  drawFastVLine(x, y, h, color);
  drawFastVLine(x + w - 1, y, h, color);
}

// Hàm vẽ hình chữ nhật đặc (Filled Rectangle)
void drawFillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
  if (x < 0 || x >= 160 || y < 0 || y >= 120 || w <= 0 || h <= 0) return;
  if (x + w > 160) w = 160 - x;
  if (y + h > 120) h = 120 - y;
  
  uint16_t color_swapped = (color >> 8) | (color << 8);
  uint16_t row_buf[160];
  for (int i = 0; i < w; i++) {
    row_buf[i] = color_swapped;
  }
  
  for (int row = 0; row < h; row++) {
    setAddrWindow(x, y + row, x + w - 1, y + row);
    digitalWrite(TFT_DC, HIGH);
    digitalWrite(TFT_CS, LOW);
    SPI.writeBytes((uint8_t *)row_buf, w * 2);
    digitalWrite(TFT_CS, HIGH);
  }
}


// Hàm vẽ hồng tâm / góc ngắm quét QR ở giữa màn hình (kích thước 60x60)
void drawScanTarget(uint16_t color) {
  int x0 = 50, x1 = 110;
  int y0 = 30, y1 = 90;
  int len = 10; // Chiều dài các góc vuông của hồng tâm
  
  // Góc trên bên trái
  drawFastHLine(x0, y0, len, color);
  drawFastVLine(x0, y0, len, color);
  
  // Góc trên bên phải
  drawFastHLine(x1 - len + 1, y0, len, color);
  drawFastVLine(x1, y0, len, color);
  
  // Góc dưới bên trái
  drawFastHLine(x0, y1, len, color);
  drawFastVLine(x0, y1 - len + 1, len, color);
  
  // Góc dưới bên phải
  drawFastHLine(x1 - len + 1, y1, len, color);
  drawFastVLine(x1, y1 - len + 1, len, color);
}

enum DeviceState {
  STATE_REMOTE,
  STATE_SCANNING
};

DeviceState device_state = STATE_REMOTE; // Bắt đầu ở chế độ chờ lệnh từ Sonix

// Vẽ một ký tự lên màn hình TFT ST7735
void drawChar(uint8_t x, uint8_t y, char c, uint16_t color, uint16_t bg, uint8_t size) {
  if (x >= 160 || y >= 120) return;
  if (c < 32 || c > 127) c = ' ';
  
  uint16_t c_idx = c - 32;
  uint32_t font_offset = 6 + c_idx * 5;
  
  for (int i = 0; i < 5; i++) {
    uint8_t line = System5x7[font_offset + i];
    for (int j = 0; j < 8; j++) {
      if (line & 0x01) {
        if (size == 1) {
          drawPixel(x + i, y + j, color);
        } else {
          setAddrWindow(x + i * size, y + j * size, x + (i + 1) * size - 1, y + (j + 1) * size - 1);
          digitalWrite(TFT_DC, HIGH);
          digitalWrite(TFT_CS, LOW);
          uint16_t color_swapped = (color >> 8) | (color << 8);
          for (int s = 0; s < size * size; s++) {
            SPI.writeBytes((uint8_t *)&color_swapped, 2);
          }
          digitalWrite(TFT_CS, HIGH);
        }
      } else if (bg != color) {
        if (size == 1) {
          drawPixel(x + i, y + j, bg);
        } else {
          setAddrWindow(x + i * size, y + j * size, x + (i + 1) * size - 1, y + (j + 1) * size - 1);
          digitalWrite(TFT_DC, HIGH);
          digitalWrite(TFT_CS, LOW);
          uint16_t bg_swapped = (bg >> 8) | (bg << 8);
          for (int s = 0; s < size * size; s++) {
            SPI.writeBytes((uint8_t *)&bg_swapped, 2);
          }
          digitalWrite(TFT_CS, HIGH);
        }
      }
      line >>= 1;
    }
  }
}

// Vẽ chuỗi ký tự
void drawString(uint8_t x, uint8_t y, const char *str, uint16_t color, uint16_t bg, uint8_t size) {
  uint8_t start_x = x;
  while (*str) {
    if (*str == '\n' || *str == '\r') {
      x = start_x;
      y += 8 * size;
    } else {
      drawChar(x, y, *str, color, bg, size);
      x += 6 * size;
      if (x + 6 * size >= 160) {
        x = start_x;
        y += 8 * size;
      }
    }
    str++;
  }
}

// Tạo và vẽ mã QR Code lên màn hình TFT
void draw_QR_Code(const char *text) {
  fillScreenFast(0xFFFF); // Xóa màn hình trắng (vùng đệm biên quét QR tốt hơn)

  QRCode qrcode;
  uint8_t qrcodeData[150]; 
  
  int8_t result = qrcode_initText(&qrcode, qrcodeData, 3, ECC_LOW, text);
  if (result < 0) {
    uint8_t qrcodeData7[300];
    result = qrcode_initText(&qrcode, qrcodeData7, 7, ECC_LOW, text);
    if (result < 0) {
      return;
    }
  }

  int qr_size = qrcode.size;
  int scale = 120 / qr_size;
  if (scale < 1) scale = 1;
  
  int offset_x = (160 - (qr_size * scale)) / 2;
  int offset_y = (120 - (qr_size * scale)) / 2;

  int qr_w = qr_size * scale;
  int qr_h = qr_size * scale;
  setAddrWindow(offset_x, offset_y, offset_x + qr_w - 1, offset_y + qr_h - 1);

  digitalWrite(TFT_DC, HIGH);
  digitalWrite(TFT_CS, LOW);

  uint16_t row_buffer[120];
  for (int y = 0; y < qr_size; y++) {
    for (int x = 0; x < qr_size; x++) {
      uint16_t color = qrcode_getModule(&qrcode, x, y) ? 0x0000 : 0xFFFF;
      uint16_t color_swapped = (color >> 8) | (color << 8);
      for (int s = 0; s < scale; s++) {
        row_buffer[x * scale + s] = color_swapped;
      }
    }
    for (int s = 0; s < scale; s++) {
      SPI.writeBytes((uint8_t *)row_buffer, qr_w * 2);
    }
  }
  digitalWrite(TFT_CS, HIGH);
}

// Xử lý tập lệnh điều khiển nhận từ Sonix qua UART
void process_UART_commands() {
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();
    if (cmd.startsWith("CMD_SCAN")) {
      device_state = STATE_SCANNING;
      fillScreenFast(0x0000);
      drawScanTarget(0x07FF); // Tông màu Cyan neon tương lai
    } 
    else if (cmd.startsWith("CMD_CLEAR")) {
      device_state = STATE_REMOTE;
      fillScreenFast(0x0000);
    } 
    else if (cmd.startsWith("CMD_QR:")) {
      device_state = STATE_REMOTE;
      String payload = cmd.substring(7);
      draw_QR_Code(payload.c_str());
    } 
    else if (cmd.startsWith("CMD_FILLRECT:")) {
      device_state = STATE_REMOTE;
      // Định dạng: CMD_FILLRECT:x,y,w,h,color
      String params = cmd.substring(13);
      int idx1 = params.indexOf(',');
      int idx2 = params.indexOf(',', idx1 + 1);
      int idx3 = params.indexOf(',', idx2 + 1);
      int idx4 = params.indexOf(',', idx3 + 1);
      
      if (idx1 != -1 && idx2 != -1 && idx3 != -1 && idx4 != -1) {
        int x = params.substring(0, idx1).toInt();
        int y = params.substring(idx1 + 1, idx2).toInt();
        int w = params.substring(idx2 + 1, idx3).toInt();
        int h = params.substring(idx3 + 1, idx4).toInt();
        uint16_t color = (uint16_t)params.substring(idx4 + 1).toInt();
        drawFillRect(x, y, w, h, color);
      }
    }
    else if (cmd.startsWith("CMD_RECT:")) {
      device_state = STATE_REMOTE;
      // Định dạng: CMD_RECT:x,y,w,h,color
      String params = cmd.substring(9);
      int idx1 = params.indexOf(',');
      int idx2 = params.indexOf(',', idx1 + 1);
      int idx3 = params.indexOf(',', idx2 + 1);
      int idx4 = params.indexOf(',', idx3 + 1);
      
      if (idx1 != -1 && idx2 != -1 && idx3 != -1 && idx4 != -1) {
        int x = params.substring(0, idx1).toInt();
        int y = params.substring(idx1 + 1, idx2).toInt();
        int w = params.substring(idx2 + 1, idx3).toInt();
        int h = params.substring(idx3 + 1, idx4).toInt();
        uint16_t color = (uint16_t)params.substring(idx4 + 1).toInt();
        drawRect(x, y, w, h, color);
      }
    }
    else if (cmd.startsWith("CMD_LINE:")) {
      device_state = STATE_REMOTE;
      // Định dạng: CMD_LINE:x0,y0,x1,y1,color
      String params = cmd.substring(9);
      int idx1 = params.indexOf(',');
      int idx2 = params.indexOf(',', idx1 + 1);
      int idx3 = params.indexOf(',', idx2 + 1);
      int idx4 = params.indexOf(',', idx3 + 1);
      
      if (idx1 != -1 && idx2 != -1 && idx3 != -1 && idx4 != -1) {
        int x0 = params.substring(0, idx1).toInt();
        int y0 = params.substring(idx1 + 1, idx2).toInt();
        int x1 = params.substring(idx2 + 1, idx3).toInt();
        int y1 = params.substring(idx3 + 1, idx4).toInt();
        uint16_t color = (uint16_t)params.substring(idx4 + 1).toInt();
        drawLine(x0, y0, x1, y1, color);
      }
    }
    else if (cmd.startsWith("CMD_PRINT:")) {
      device_state = STATE_REMOTE;
      // Định dạng: CMD_PRINT:x,y,size,color,bg,text
      String params = cmd.substring(10);
      int idx1 = params.indexOf(',');
      int idx2 = params.indexOf(',', idx1 + 1);
      int idx3 = params.indexOf(',', idx2 + 1);
      int idx4 = params.indexOf(',', idx3 + 1);
      int idx5 = params.indexOf(',', idx4 + 1);
      
      if (idx1 != -1 && idx2 != -1 && idx3 != -1 && idx4 != -1 && idx5 != -1) {
        int x = params.substring(0, idx1).toInt();
        int y = params.substring(idx1 + 1, idx2).toInt();
        int size = params.substring(idx2 + 1, idx3).toInt();
        uint16_t color = (uint16_t)params.substring(idx3 + 1, idx4).toInt();
        uint16_t bg = (uint16_t)params.substring(idx4 + 1, idx5).toInt();
        String text = params.substring(idx5 + 1);
        
        drawString(x, y, text.c_str(), color, bg, size);
      }
    }
  }
}

// -----------------------------------------------------------------------------
// 4. Các hàm Setup và Loop chính
// -----------------------------------------------------------------------------
void setup() {
  // Khởi tạo Bảng tra cứu LUT chuyển đổi Grayscale -> RGB565 (đã hoán đổi byte)
  for (int i = 0; i < 256; i++) {
    uint8_t r = i >> 3;
    uint8_t g = i >> 2;
    uint8_t b = i >> 3;
    uint16_t pixel = (r << 11) | (g << 5) | b;
    gray_to_rgb565_lut[i] = (pixel >> 8) | (pixel << 8);
  }

  // Khởi chạy UART truyền kết quả sang Sonix
  Serial.setRxBufferSize(1024);
  Serial.begin(115200);

  // Khởi chạy bộ ngoại vi SPI phần cứng
  SPI.begin(TFT_SCK, -1, TFT_MOSI, TFT_CS); 
  SPI.setFrequency(20000000); // Đặt xung nhịp SPI lên 20MHz để hoạt động ổn định và giảm nhiễu camera

  // Khởi chạy driver màn hình
  st7735_init();
  fillScreenFast(0x0000); // Clear đen
  drawString(10, 10, "Booting...", 0xFFFF, 0x0000, 1);

  // Khởi tạo bộ giải mã QR quirc (160x120)
  q = quirc_new();
  if (!q || quirc_resize(q, 160, 120) < 0) {
    fillScreenFast(0xF800); // Màn hình Đỏ
    drawString(10, 10, "QR Init Fail!", 0xFFFF, 0xF800, 1);
    while (true) { delay(1000); }
  }
  drawString(10, 30, "QR Init: OK", 0x07E0, 0x0000, 1);

  // Cấu hình thông số Camera OV2640
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  
  config.xclk_freq_hz = 10000000;          // Tần số XCLK 10MHz để ổn định camera
  // Định dạng ảnh thang độ xám (Grayscale 8-bit)
  config.pixel_format = PIXFORMAT_GRAYSCALE; 

  // Độ phân giải QQVGA (160x120) phù hợp màn hình ST7735S và giải mã QR trong RAM
  config.frame_size = FRAMESIZE_QQVGA; 
  config.jpeg_quality = 12;
  config.fb_count = 1;

  // Khởi tạo Camera
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    fillScreenFast(0xF800); // Màn hình Đỏ
    drawString(10, 10, "Camera Init Fail!", 0xFFFF, 0xF800, 1);
    char err_str[32];
    snprintf(err_str, sizeof(err_str), "Err: 0x%x", err);
    drawString(10, 30, err_str, 0xFFFF, 0xF800, 1);
    while (true) { delay(1000); }
  }
  drawString(10, 50, "Camera Init: OK", 0x07E0, 0x0000, 1);

  // Giảm phơi sáng để chống lóa (cháy sáng) khi đưa camera vào màn hình điện thoại
  sensor_t * s = esp_camera_sensor_get();
  if (s != NULL) {
    if (s->set_ae_level != NULL) {
      s->set_ae_level(s, -2); // Đặt mức phơi sáng thấp (-2) để nhìn rõ nội dung màn hình phát sáng
    }
    if (s->set_contrast != NULL) {
      s->set_contrast(s, 2);  // Tăng tương phản tối đa (+2) để tách bạch đen/trắng của QR
    }
    if (s->set_sharpness != NULL) {
      s->set_sharpness(s, 2); // Tăng độ sắc nét tối đa (+2) giúp quirc nhận diện góc cạnh QR tốt hơn
    }
    // Xoay cảm biến camera 180 độ để đồng bộ với hướng màn hình mới
    if (s->set_vflip != NULL) {
      s->set_vflip(s, ENABLE_VFLIP ? 1 : 0);
    }
    if (s->set_hmirror != NULL) {
      s->set_hmirror(s, ENABLE_HMIRROR ? 1 : 0);
    }
  }

  drawString(10, 70, "Waiting for MCU...", 0x07FF, 0x0000, 1);
}

void loop() {
  process_UART_commands();

  if (device_state == STATE_SCANNING) {
    // 1. Chụp 1 khung hình thang độ xám (Grayscale) trực tiếp từ Camera
    camera_fb_t * fb = esp_camera_fb_get();
    if (!fb) {
      delay(10);
      return;
    }

    // 2. Đẩy hình ảnh dạng Grayscale hiển thị lên màn hình TFT ST7735S
    drawGrayscale_Fast(fb->buf, fb->width, fb->height);

    // Vẽ hồng tâm quét màu cyan neon
    drawScanTarget(0x07FF); // Màu Cyan


    // 3. Sao chép ảnh xám trực tiếp vào bộ đệm quirc để giải mã QR (Không cần chuyển đổi hệ màu)
    int w = fb->width;
    int h = fb->height;
    int gray_w, gray_h;
    uint8_t *gray_buf = quirc_begin(q, &gray_w, &gray_h);
    
    if (gray_buf && gray_w == w && gray_h == h) {
      memcpy(gray_buf, fb->buf, w * h); // Copy cực nhanh bằng memcpy
      
      // Kết thúc nạp dữ liệu và chạy thuật toán nhận diện
      quirc_end(q);
      
      // Quét kiểm tra kết quả giải mã
      int count = quirc_count(q);
      
      for (int i = 0; i < count; i++) {
        static struct quirc_code code;
        static struct quirc_data data;
        
        quirc_extract(q, i, &code);
        quirc_decode_error_t err = quirc_decode(&code, &data);
        if (err != QUIRC_SUCCESS) {
          // Thử lật gương (mirrored QR code) nếu quét trực tiếp thất bại
          quirc_flip(&code);
          err = quirc_decode(&code, &data);
        }
        
        // Vẽ viền đỏ xung quanh mã QR khi phát hiện thấy (đổi sang xanh lá nếu giải mã thành công)
        uint16_t box_color = (err == QUIRC_SUCCESS) ? 0x07E0 : 0xF800; // Xanh lá vs Đỏ
        drawLine(code.corners[0].x, code.corners[0].y, code.corners[1].x, code.corners[1].y, box_color);
        drawLine(code.corners[1].x, code.corners[1].y, code.corners[2].x, code.corners[2].y, box_color);
        drawLine(code.corners[2].x, code.corners[2].y, code.corners[3].x, code.corners[3].y, box_color);
        drawLine(code.corners[3].x, code.corners[3].y, code.corners[0].x, code.corners[0].y, box_color);
        
        if (err == QUIRC_SUCCESS) {
          // Đã giải mã thành công mã QR!
          // 1. Gửi chuỗi ký tự qua cổng UART sang chip Sonix
          Serial.printf("TX_RAW:%s\n", (char *)data.payload); 
          
          // 2. Phản hồi trực quan: Vẽ khung viền xanh lá cây dày 3 pixel xung quanh và đổi hồng tâm sang xanh lá
          for (int b = 0; b < 3; b++) {
            drawRect(b, b, 160 - 2 * b, 120 - 2 * b, 0x07E0); // Xanh lá cây
          }
          drawScanTarget(0x07E0); // Hồng tâm xanh lá
          
          // Giải phóng bộ đệm camera
          esp_camera_fb_return(fb);

          // Đợi một chút cho phần cứng UART gửi xong TX_RAW và vẽ ổn định màn hình
          delay(50);

          // 3. Gửi tín hiệu READY báo hiệu đã sẵn sàng nhận lệnh vẽ
          Serial.println("READY");

          // Quay lại chế độ chờ lệnh từ Sonix
          device_state = STATE_REMOTE;
          return;
        }
      }
    }

    // 4. Giải phóng bộ đệm khung hình của camera
    esp_camera_fb_return(fb);
  }

  // Tránh watchdog timeout bằng cách nhường CPU cho các tác vụ nền
  delay(1);
}
