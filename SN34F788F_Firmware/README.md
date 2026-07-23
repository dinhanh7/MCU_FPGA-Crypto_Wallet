# SN34F788F Firmware

Firmware cho vi điều khiển **SONiX SN34F788F** của ví phần cứng BTC/ETH. Dự án
được đóng gói độc lập để mở và biên dịch bằng Keil MDK-ARM, bao gồm mã ứng
dụng, driver, lớp nền tảng và các tệp CMSIS cần thiết.

## Yêu cầu

- Keil MDK-ARM.
- SONiX `SN34F7_DFP` 2.0.4.
- ARM CMSIS 5.9.0.

Các pack có thể được cài bằng **Pack Installer** của Keil.

## Mở và build

1. Mở `polling.uvprojx` bằng Keil uVision.
2. Chọn target `Target 1`.
3. Nhấn **Build** hoặc phím `F7`.
4. Lấy tệp kết quả trong thư mục output do Keil cấu hình.

Trong `polling.uvprojx`, thiết bị đích được đặt là `SN34F788F`. Tên output và
một số tệp hỗ trợ của SDK vẫn dùng tiền tố `SN34F780`; đây là tên dùng chung
trong device pack của dòng SN34F78, không phải là thiết bị đích của dự án.

Firmware build sẵn, nếu có, nằm trong `firmware/`.

## Cấu trúc thư mục

- `src/`, `include/`: mã nguồn và header của ứng dụng SN34F788F.
- `drivers/`: driver và HAL được đưa vào nội bộ dự án.
- `platform/`: các thành phần Com và Middleware của SDK.
- `RTE/`: startup, system và cấu hình runtime do Keil quản lý.
- `esp32_cam_tft/`: sketch và thư viện cho ESP32-CAM/TFT đi kèm.
- `docs/`: tài liệu phần cứng và camera.
- `firmware/`: các ảnh firmware đã build.
- `tests/`: mã hoặc dữ liệu phục vụ kiểm thử.

Các đường dẫn source/include trong `polling.uvprojx` là đường dẫn tương đối,
vì vậy có thể di chuyển toàn bộ thư mục dự án mà không cần sửa lại đường dẫn
SDK thủ công.
