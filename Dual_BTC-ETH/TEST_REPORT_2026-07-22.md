# Báo cáo test dual BTC/ETH ngày 2026-07-22

## Phạm vi

- C reference cho MCU Sonix SN34F788F;
- RTL dual và giao thức UART;
- FPGA Gowin ACG525 thật tại `/dev/ttyACM0`;
- Bitcoin Core regtest;
- Ethereum Anvil chain ID 31337;
- web dual tại `/btc/` và `/eth/`.

## Xác nhận bản đóng gói cuối

Bản trong chính folder `Dual_BTC-ETH` được build lại bằng Gowin
V1.9.12.03, SystemVerilog 2017, device `GW5A-LV25UG324C2/I1` và top
`acg525_dual_uart_top`:

- synthesis: pass;
- placement và routing: pass, không có `PR0004` hoặc unrouted net;
- timing: 28.528 endpoint được phân tích, 0 setup violation, 0 hold
  violation, TNS setup/hold đều 0;
- clock constraint: 50 MHz; actual Fmax: 54,737 MHz;
- logic: 16.671/23.040 (73%); register: 9.185/23.685 (39%); BSRAM:
  22/56 (40%); CLS: 9.805/11.520 (86%);
- bitstream SHA-256:
  `ea282426b48b2a5a3dee11b6c7bd204975190964c257623197035eb6d58afb32`.

JTAG nhận đúng một device ID `0x0001281B`; external Flash ID `0xC84018`.
Bitstream được nạp bằng `exFlash Erase,Program,Verify Arora V`, programmer báo
`Program and Verify Flash successfully`, sau đó lệnh `Reprogram` hoàn tất.

Regression chạy lại từ folder đóng gói:

- C protocol, BTC self-test và địa chỉ ETH: pass;
- Verilator lint và Yosys hierarchy/proc/check: pass;
- ba mô phỏng RTL (BTC core, BTC UART và dual ETH rồi BTC): pass;
- 13 test web/ETH: 11 pass, 2 integration test cần Anvil bị skip trong lần
  chạy unit độc lập; đường ký chính `dual-mcu + FPGA` đã được test phần cứng
  riêng ở dưới;
- URL Anvil ngoài `http://192.168.2.4:8546` qua endpoint
  `/eth/api/status`: pass, trả Chain ID 31337 và client `anvil/v1.7.1`;
- URL không an toàn `file:///etc/passwd`: bị từ chối HTTP 400 đúng thiết kế.

Kiểm tra phần cứng qua `/dev/ttyACM0`:

- PING/PONG: pass;
- MCU ký hash ETH, recover đúng địa chỉ FPGA và
  `signatureVerified=true`: pass;
- BTC web → MCU → FPGA → Core: chuyển 0,0001 BTC regtest, Core trả
  `complete=true`, `mempool_allowed=true`, TXID
  `534f3c3c5101097dad2a66e96e6ac7d875389c37bf27f48bb79c56f8c45944b4`,
  sau đó mine 1 block: pass;
- ETH web → MCU → FPGA → Anvil: chuyển 0,01 ETH, MCU xác minh chữ ký,
  receipt status 1, TX hash
  `0x66175f6c3e552cf7fdbb7807c6a49a0df6d8d2e61f0c6388676bd43ac17de3be`:
  pass.

## Regression

- `dual-mcu` build với `-Werror`: pass;
- BTC C self-test và protocol CRC BTC/ETH: pass;
- 11 test Ethereum Python/web/UART/QR: pass;
- Verilator BTC core: pass;
- Verilator MCU UART BTC: pass;
- Verilator dual ETH rồi BTC trên cùng image: pass;
- Verilator lint: pass;
- Yosys hierarchy/proc/check: 0 lỗi.

Test ETH cũ đã được cập nhật để phản ánh kiến trúc mới: web bắt buộc gọi
`dual-mcu` và cung cấp PIN, không mock đường ký UART trực tiếp. Assertion chain
ID cũng lấy từ RPC đang test thay vì hard-code 31338.

## BTC trên FPGA thật

Giao dịch file PSBT:

- source: `fpga_d1_e2e`;
- số tiền: 0.001 BTC;
- phí: 1.000 sat;
- `FPGA_SIGN_OK` và `signature_verified_by_fpga=true`;
- Core `finalizepsbt`: `complete=true`;
- Core `testmempoolaccept`: `allowed=true`;
- TXID: `4c88a031a6d072d205fd98e3146fbbceb979921c045fb1dc0e824f0797a4c3fc`;
- đã mine và có 1 confirmation trong vòng test.

Giao dịch BBQr:

- số tiền: 0.0002 BTC;
- unsigned: 4 frame, signed: 6 frame, frame dài nhất 96 ký tự;
- TXID: `200d62674e154756efbe9a5fb1be09b08cba102e64c70acd471108d31fbb8429`;
- broadcast và mine thành công;
- frame BBQr sai bị HTTP 400.

Sai PIN, Hủy và dùng public key không khớp đều bị từ chối trước khi tạo output
hoặc trước khi kích hoạt FPGA không hợp lệ.

Trạng thái cuối BTC: regtest block 232, mempool rỗng.

## ETH trên FPGA thật

Flow C trực tiếp:

- EIP-1559 0.01 ETH;
- C tạo RLP/Keccak và freeze ID;
- FPGA ký recoverable ECDSA;
- C recover đúng `0x7E5F4552091A69125d5DfCb7b8C2659029395Bdf`;
- `signatureVerified=true`;
- TX hash: `0x5d0c471ee5df6b67ec5fde17b8f62d4309b64bbfd1a0fe6b7cfc178b16f23480`;
- Anvil receipt status 1.

Flow web dual:

- build EIP-1559 0.02 ETH;
- web gọi `dual-mcu`, không gọi lệnh sign UART trực tiếp;
- MCU verify chữ ký thành công;
- TX hash: `0xdc244c514c5e2e1c097e01870fd966c46d09f892e8e871f85c607ca258ea3441`;
- Anvil receipt status 1 tại block 5;
- rebroadcast trả đúng transaction cũ với `alreadyMined=true`.

Sai PIN bị HTTP 400 và C báo transaction đã xóa, FPGA không được gọi. Public
key cấu hình sai bị C phát hiện qua bước recovery verification.

## Web/API

- `/`, `/btc/`, `/eth/`: HTTP 200;
- FPGA status/PING: online, 115200 baud;
- BTC status/create/register/address/broadcast/mine: pass;
- BTC file PSBT và BBQr: pass;
- ETH status/wallet/balance/build/sign/broadcast/rebroadcast: pass;
- static JS/CSS/QR assets: HTTP 200;
- security headers và optional Basic Auth: pass trong unit test.

Anvil được tạo tạm cho test và đã tắt. Web test cũng đã tắt. Bitcoin Core
regtest được giữ chạy như trước.

## Giới hạn

Đây là test regtest/Anvil với khóa FPGA thử nghiệm `d=1`. C vẫn là reference
host đại diện MCU, chưa phải firmware chạy trên SN34F788F. Không sử dụng kết
quả này để chuyển tài sản mainnet.
