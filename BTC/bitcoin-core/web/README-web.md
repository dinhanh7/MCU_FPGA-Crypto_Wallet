# Web Bitcoin Regtest – giao tiếp SN34F788F/ACG525

Web xuất PSBT nhị phân để chuyển qua USB/UART/thẻ nhớ. QR thích nghi vẫn là lựa
chọn phụ: PSBT nhỏ là một QR tĩnh, PSBT lớn là BBQr nhiều khung. Không có API ký.

## Cách sử dụng

### 1. Web → MCU

1. Chọn ví MCU nguồn.
2. Nhập địa chỉ nhận và số Bitcoin.
3. Nhấn **Tạo PSBT**.
4. Nhấn **Tải file .psbt** để chép sang MCU, hoặc dùng QR/BBQr.
5. Với nhiều khung, quét khung hiện tại rồi nhấn **QR tiếp theo**. QR không tự
   chuyển và sau khung cuối sẽ quay lại khung đầu.

### 2. Xác nhận trên MCU

Trong mô phỏng:

```bash
cd ~/workspace/dinhanh_k68/bitcoin-core/c-signer
bin/coldsign mcu-sign-file-fpga \
  keys/cold.pub keys/mcu.passkey /dev/ttyUSB0 \
  /media/usb/unsigned.psbt /media/usb/signed.psbt
```

MCU kiểm tra địa chỉ, số tiền, phí, tiền thừa và `freeze_id`; sau PIN/nút Đồng ý,
ACG525 tự tính BIP143, ký, verify và trả `r/s` để MCU ghép vào PSBT.

### 3. MCU → Web

1. Chọn file `signed.psbt` từ USB/thẻ nhớ và nhấn **Nạp file → Hoàn thiện →
   Phát**; hoặc nhấn **Bật camera quét QR đã ký**.
2. Hướng webcam vào QR trên MCU/màn hình mô phỏng.
3. Sau khi web nhận khung, nhấn **QR tiếp theo** trên màn hình MCU; lặp lại đến
   khi tiến độ báo đủ khung.
4. Nhấn **Hoàn thiện → Phát**.
5. Đào một block để xác nhận giao dịch Regtest.

Trình duyệt tạo và đọc QR cục bộ bằng các thư viện đã lưu trong thư mục web;
PSBT không được gửi tới dịch vụ tạo QR bên ngoài.

## API

```text
GET  /api/status
POST /api/wallet
POST /api/address
POST /api/mcu/register
POST /api/psbt/create
POST /api/psbt/broadcast
POST /api/cold/mine
```

Không tồn tại endpoint gọi C-signer. `/api/psbt/create` chỉ trả QR của PSBT chưa
ký; `/api/psbt/broadcast` chỉ nhận QR chứa PSBT đã có chữ ký hợp lệ để Bitcoin
Core hoàn thiện và phát.

## Thành phần QR

```text
qrcode.min.js  tạo QR BBQr trên web
jsQR.js         quét QR từ webcam
BBQr Z/P        Zlib + Base32, tối đa 100 byte cho toàn bộ mỗi khung
```

Webcam cần HTTPS hoặc localhost và người dùng phải cấp quyền camera. Đường
TryCloudflare đáp ứng điều kiện HTTPS.

Web hiện tạo giao dịch dùng một UTXO. Toàn bộ mỗi QR, gồm header BBQr 8 byte,
không vượt 100 byte. Khung động dùng payload 88 ký tự nên dài 96 byte; khung
cuối thường ngắn hơn. Giao diện hiển thị số byte thực tế của từng khung.
