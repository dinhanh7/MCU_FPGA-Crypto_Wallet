# FPGA dual BTC/ETH cho ACG525

Image dùng chung một datapath RFC6979/ECDSA secp256k1:

- command `0x01`: ký Ethereum signing hash 32 byte, trả `yParity/r/s`;
- command `0x10`: tự tạo BIP143 digest Bitcoin, trả
  `freeze_id/digest/r/s`;
- command `0x00`: PING/PONG.

UART: 115200 8N1, CRC16-CCITT. Clock 50 MHz. Constraint hiện dùng clock `T9`,
RX `U8`, TX `V8` và LED `N16/C17`.

Toàn bộ dependency RTL đã được đưa vào `rtl/`; Gowin project không còn tham
chiếu sang cây ETH bên ngoài. Xem hướng dẫn build tại `../README.md`.
