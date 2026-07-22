# SN34F788F + Gowin ACG525 – ví lạnh PSBT

Kiến trúc triển khai đã tách khóa và phép ký khỏi MCU:

```text
Bitcoin Core → PSBT chưa ký → SN34F788F review/freeze/PIN/nút
             → trường giao dịch + freeze_id qua UART
             → ACG525 tự tính BIP143, ký ECDSA, tự verify
             → freeze_id + digest + r/s về SN34F788F
             → MCU ghép partial signature → Bitcoin Core finalize/broadcast
```

`coldsign` vẫn là chương trình host mô phỏng MCU. Lệnh triển khai
`mcu-sign-*-fpga` chỉ đọc public key; private key chỉ tồn tại trong RTL FPGA.
Code C thuần để ghép với SDK Sonix nằm trong `mcu/`, RTL nằm trong `fpga/`.

Máy trực tuyến có thể chuyển PSBT nhị phân bằng USB/UART/thẻ nhớ. QR/BBQr được
giữ làm transport tùy chọn. Web không có khóa bí mật và không có API ký.

> Đây là mô hình nghiên cứu Regtest. Terminal mô phỏng keypad/nút bấm, đầu đọc
> QR đưa chuỗi đã giải mã vào stdin và trang HTML/SVG mô phỏng LCD MCU. Không
> dùng mã này với Mainnet.

## Luồng QR

```text
Web + Bitcoin Core tạo PSBT chưa ký
  ↓ QR tĩnh hoặc BBQr động
MCU ráp đủ khung → giải nén PSBT → kiểm tra → freeze_id
  ↓ passkey + DONG Y
ACG525: BIP143 → RFC6979 → ECDSA → low-S → tự kiểm tra
  ↓
MCU: kiểm tra response → DER → ghép chữ ký → signed PSBT
  ↓ webcam
Web đọc signed PSBT → finalizepsbt → testmempoolaccept → sendrawtransaction
```

BBQr là định dạng công khai của Coinkite dành cho dữ liệu Bitcoin lớn qua QR:
https://github.com/coinkite/BBQr

Thiết lập hiện tại dùng:

```text
Encoding: Z (raw DEFLATE wbits=10 + Base32 RFC4648)
File type: P (PSBT)
Header: B$ZP + tổng khung base36 + chỉ số khung base36
Mỗi QR: tối đa 100 byte tính cả header 8 byte
PSBT cực nhỏ vừa giới hạn: một QR tĩnh B$ZP0100
PSBT lớn hơn: chia payload thành khung 88 ký tự (96 byte cả header)
Nhiều khung không tự chạy; người dùng nhấn `QR tiếp theo`
Khung có thể được quét không đúng thứ tự
```

## Biên dịch

```bash
cd ~/workspace/dinhanh_k68/bitcoin-core/c-signer
make clean all test
```

`SELFTEST_OK` là kết quả bắt buộc. QR được tạo bằng bản C của thư viện Nayuki
QR Code Generator trong `src/qrcodegen.c`/`.h`.

## Thiết lập mã mở khóa

```bash
bin/coldsign mcu-setup-passkey keys/mcu.passkey
```

Mã gồm 6–12 chữ số. Tệp lưu salt và PBKDF2-HMAC-SHA256 verifier 200.000 vòng,
không lưu mã rõ.

## Ký qua QR

Ví dụ nguồn `coldwatch`:

```bash
bin/coldsign mcu-sign-qr \
  keys/cold.key \
  keys/mcu.passkey \
  transfer/signed-qr.html
```

Lệnh trên tự ký trong C và chỉ dành cho regression cũ. Với FPGA thật dùng:

```bash
bin/coldsign mcu-sign-qr-fpga \
  keys/cold.pub \
  keys/mcu.passkey \
  /dev/ttyUSB0 \
  transfer/signed-qr.html
```

Với PSBT chép qua USB/thẻ nhớ:

```bash
bin/coldsign mcu-sign-file-fpga \
  keys/cold.pub keys/mcu.passkey /dev/ttyUSB0 \
  /media/usb/unsigned.psbt /media/usb/signed.psbt
```

UART MCU–FPGA là `115200 8N1`, mức điện áp `3,3 V` và không được nối trực tiếp
với RS-232 ±12 V.

Sau khi thấy `QR_SCAN_READY`, hướng camera MCU vào QR trên web. Sau mỗi lần
quét thành công, nhấn **QR tiếp theo** trên web. Các khung có thể đến sai thứ
tự và khung trùng được bỏ qua an toàn.

Khi nhận đủ QR, C-signer:

1. Ráp payload theo chỉ số, giải mã Base32 và giải nén PSBT trong MCU.
2. Kiểm tra PSBT v0, đầu vào, người nhận, phí và tiền thừa.
3. Tạo `freeze_id = SHA256(PSBT)`.
4. Hiển thị giao dịch.
5. Cho tối đa ba lần nhập passkey.
6. Chỉ ký khi nhập chính xác `DONG Y`.
7. Tính lại `freeze_id`, gửi các trường giao dịch sang ACG525.
8. FPGA tự tính BIP143, ký, verify; MCU kiểm tra response và ghép vào PSBT.
9. Nén signed PSBT, tự chọn QR tĩnh/động và tạo `signed-qr.html` mode `0600`.

Mở trang mô phỏng LCD:

```bash
google-chrome --new-window \
  ~/workspace/dinhanh_k68/bitcoin-core/c-signer/transfer/signed-qr.html
```

Trang luôn giữ nguyên khung hiện tại. Nếu có nhiều khung, nhấn **QR tiếp theo**
để chuyển; sau khung cuối sẽ quay lại khung đầu.
Chương trình từ chối bất kỳ khung đầu vào nào dài hơn 100 byte.

## Hủy và sai mã

- Ba lần sai mã: exit code `3`, không tạo QR đầu ra.
- Nhập khác `DONG Y`: exit code `4`, không tạo QR đầu ra.
- PSBT thay đổi sau khi hiển thị: từ chối ký.
- Tệp HTML đầu ra đã tồn tại: không ghi đè.
- Bộ đệm PSBT, chữ ký và dữ liệu nhạy cảm được làm sạch trước khi thoát.

## Điểm ký

Lệnh chính của flow QR:

```text
mcu-sign-file-fpga → nhận PSBT → review/freeze → passkey → DONG Y
                   → ACG525 BIP143/sign/verify → signed PSBT
```

Các lệnh ký trực tiếp `sign-digest`, `sign-p2wpkh`, `sign-psbt` vẫn bị vô hiệu
hóa. `mcu-sign-qr`/`mcu-sign-file` giữ làm mô hình host, không dùng khi triển
khai khóa trong FPGA.

## Protocol MCU–FPGA

Request dùng sync `A5 5A`, version, sequence, command `10`, độ dài, payload và
CRC16-CCITT. Payload chứa `freeze_id`, version, outpoint, sequence, pubkey-hash,
giá trị prevout, raw outputs, locktime và SIGHASH_ALL. Không có digest do MCU
tạo trong request.

Response dùng sync `5A A5`, echo sequence/command, status, `freeze_id`, digest
BIP143 và `r/s`, sau đó CRC16. MCU từ chối nếu CRC, sequence, freeze hoặc digest
không khớp PSBT đã review. Giới hạn hiện tại là 512 byte raw outputs, phù hợp
policy tối đa 16 output P2WPKH của C-signer.

## Firmware SN34F788F

- `mcu/src/btc_fpga_link.c`: client protocol thuần C, không POSIX/OpenSSL.
- `mcu/src/btc_mcu_flow.c`: freeze → LCD → ba lần PIN → nút → FPGA → re-freeze.
- `mcu/sn34f788f_port.example.c`: adapter cần nối với UART/LCM/keypad/MPU/SHA256
  của project Sonix/Keil.

SN34F788F có đủ UART, SDIO, LCD-TFT và RAM cho policy này, nhưng tên chân và
driver phụ thuộc PCB. File `sn34f788f_port.example.c` cố ý không ghi trực tiếp
thanh ghi giả định; map các hàm `board_*` bằng SDK và schematic của bo thật.

## Khi đưa lên MCU thật

- Nhận binary PSBT từ USB/UART/SDIO; QR chỉ cần nếu sản phẩm có camera.
- Thay terminal bằng LCD-TFT/LCM, keypad và nút vật lý.
- Chỉ lưu public key trên MCU; provision private key vào FPGA.
- Dùng MPU để đặt buffer PSBT read-only trong lúc review và chờ FPGA.
- Lưu bền vững bộ đếm nhập sai để khởi động lại không xóa giới hạn thử.
- Không xuất cổng UART FPGA ra ngoài vỏ thiết bị; FPGA hiện tin MCU là nguồn yêu
  cầu ký và không thể tự biết người dùng đã nhìn LCD.
