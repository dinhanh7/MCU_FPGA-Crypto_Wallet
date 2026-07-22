# Ví lạnh dual Bitcoin/Ethereum: MCU + Gowin ACG525

> Bản tài liệu này đã được đóng gói vào `Dual_BTC-ETH`. Các đường dẫn lịch sử
> `DUAL/mcu`, `DUAL/web`, `BTC/.../fpga` và `ETH/...` tương ứng với `mcu/`,
> `web/`, `fpga/` và các source vendored bên trong project này. README ở thư
> mục gốc là hướng dẫn build/run chính cho bản đã gom.

Tài liệu này mô tả trạng thái tích hợp hiện tại của project ví lạnh dùng một
MCU Sonix SN34F788F và một FPGA Gowin ACG525 cho cả Bitcoin và Ethereum.

## Trạng thái hiện tại

| Thành phần | Trạng thái |
|---|---|
| FPGA dual BTC/ETH | Đã gộp, place-and-route thành công và đã nạp Flash |
| UART dual trên FPGA thật | Đã test PING, ký ETH và ký BTC thành công |
| Code C mô phỏng MCU Bitcoin | Đã đưa vào binary dual, giữ nguyên flow PSBT |
| Code C mô phỏng MCU Ethereum | Đã thêm RLP/Keccak/freeze/PIN/verify/raw tx |
| Một binary C MCU dual duy nhất | Đã có: `DUAL/mcu/bin/dual-mcu` |
| Một web BTC/ETH duy nhất | Đã có: `DUAL/web/app.py` |

Ba tầng FPGA, C đại diện MCU và web hiện đã dùng chung một flow dual. Phần C
vẫn là chương trình host mô phỏng hành vi SN34F788F, chưa phải firmware đã
port vào SDK và ngoại vi thật của MCU Sonix.

## Kiến trúc mục tiêu

```text
MÁY TRỰC TUYẾN
Web dual + Bitcoin Core + Ethereum JSON-RPC
    |
    | BTC: PSBT chưa ký
    | ETH: EIP-1559 transaction chưa ký
    v
MCU Sonix SN34F788F
    |-- parse và kiểm tra dữ liệu theo từng coin
    |-- tính người nhận, số tiền, phí và tiền thừa
    |-- khóa buffer/freeze_id
    |-- hiển thị trên màn hình
    |-- kiểm tra passkey và nút Đồng ý/Hủy
    |-- BTC: gửi các trường BIP143 đã duyệt
    `-- ETH: tạo RLP/Keccak signing hash và gửi hash 32 byte
    v
FPGA Gowin ACG525 - một image dual
    |-- BTC: tự tính BIP143 double-SHA256
    |-- ETH: nhận signing hash 32 byte từ MCU
    |-- dùng chung RFC6979 + ECDSA secp256k1
    |-- chuẩn hóa low-S và tự kiểm tra chữ ký
    `-- trả chữ ký về MCU
    v
MCU
    |-- BTC: ghép DER signature vào PSBT đã đóng băng
    `-- ETH: ghép yParity/r/s và tạo raw EIP-1559 transaction
    v
MÁY TRỰC TUYẾN
    |-- BTC: finalizepsbt -> testmempoolaccept -> broadcast
    `-- ETH: eth_sendRawTransaction
```

## Nguyên tắc gộp FPGA

Thiết kế lấy Bitcoin làm nền. FPGA không chứa hai signer độc lập vì hai khối
ECDSA đầy đủ không thể đặt tuyến hợp lý trên GW5A-25A.

Image dual chỉ có một datapath mật mã dùng chung:

- một bộ tạo nonce RFC6979;
- một point multiplier secp256k1;
- một modular multiplier, inverse và add/sub;
- một ECDSA signer;
- một bước tự kiểm tra phương trình chữ ký;
- một SHA-256 engine dùng chung cho BIP143 và RFC6979.

Đường Bitcoin giữ nguyên bộ tạo BIP143. Đường Ethereum bỏ RLP và Keccak khỏi
FPGA: MCU tạo signing hash 32 byte rồi FPGA đưa hash đó vào signer dùng chung.

Để giảm tài nguyên, hash Ethereum không nằm trong một thanh ghi 256-bit riêng.
Nó được nạp tuần tự qua UART vào bank trung gian đang có sẵn trong ECDSA core.
Bộ tuần tự hóa `r/s` ở chiều trả về cũng được dùng chung cho BTC và ETH.

## Giao thức UART dual

Thông số vật lý hiện tại:

- 115200 baud;
- 8 data bit, no parity, 1 stop bit;
- mức logic 3,3 V;
- clock FPGA 50 MHz;
- chân ACG525 hiện dùng: RX `U8`, TX `V8`.

Request frame:

```text
A5 5A | version | sequence | command | length_hi length_lo | payload | CRC16
```

Response frame:

```text
5A A5 | version | sequence | command | status | length_hi length_lo | payload | CRC16
```

CRC là CRC16-CCITT, polynomial `0x1021`, giá trị khởi tạo `0xFFFF`. CRC bao
phủ từ `version` đến hết payload, không bao gồm hai byte sync.

| Command | Chức năng | Payload request | Payload response khi thành công |
|---|---|---:|---:|
| `0x00` | PING | 0 byte | 4 byte `PONG` |
| `0x01` | ETH sign hash | 32 byte | 65 byte: `yParity || r || s` |
| `0x10` | BTC sign BIP143 | 114 byte cố định + raw outputs | 128 byte: `freeze_id || digest || r || s` |

Các status chính:

| Status | Ý nghĩa |
|---|---|
| `0x00` | Thành công |
| `0x01` | CRC sai |
| `0x02` | Độ dài/payload sai |
| `0x03` | Version hoặc command không hỗ trợ |
| `0x04` | Signer đang bận |
| `0x05` | Private key FPGA không hợp lệ |
| `0x06` | BTC BIP143 lỗi hoặc ETH recovery không biểu diễn được |
| `0x07` | FPGA tự kiểm tra chữ ký thất bại |

## Source code FPGA dual

Project chính nằm tại:

```text
BTC/bitcoin-core/c-signer/fpga/
```

Các file quan trọng:

```text
rtl/dual_coin_uart_signer_bridge.sv  top giao thức dual
rtl/btc_uart_signer_bridge.sv        parser/frame UART BTC + ETH
rtl/btc_fpga_signer_core.sv          chọn BIP143 hoặc direct ETH hash
rtl/btc_bip143_hash.sv               tạo digest Bitcoin BIP143
rtl/btc_ecdsa_sign_verify_core.sv    signer và self-verification dùng chung
tb/tb_dual_coin_uart_signer.sv       test ETH rồi BTC trên cùng image
gowin/acg525_dual_uart_top.sv         top-level cho kit ACG525
gowin/dual_signer_acg525.gprj         project Gowin
gowin/build_dual_acg525.tcl           build SystemVerilog 2017
```

Một số datapath secp256k1 và UART được tham chiếu trực tiếp từ source ETH để
không tạo hai bản mật mã khác nhau:

```text
ETH/anvil_cold_wallet/c_signer/verilog_rtl/rtl/
```

## Build và mô phỏng

```bash
cd /media/abuntu/NVME_UBUNTU1/Working/MCU_FPGA/BTC/bitcoin-core/c-signer/fpga
make core
make uart
make dual
make lint synth-check
```

Các test bắt buộc:

```text
PASS: FPGA computes BIP143, signs and self-verifies
PASS: SN34F788F protocol -> BIP143 -> verified ECDSA -> UART response
PASS: one UART image signs ETH hash and BTC BIP143 with shared ECDSA core
Yosys: Found and reported 0 problems
```

Build bằng Gowin EDA:

```bash
cd /media/abuntu/NVME_UBUNTU1/Working/MCU_FPGA/BTC/bitcoin-core/c-signer/fpga/gowin
gw_sh build_dual_acg525.tcl
```

Script build đặt:

```text
SystemVerilog standard: 2017
Device: GW5A-LV25UG324C2/I1
Top module: acg525_dual_uart_top
Clock constraint: 50 MHz
```

## Kết quả Gowin place-and-route

Kết quả của image đang chạy:

| Tài nguyên/thời gian | Kết quả |
|---|---:|
| Logic | 16671/23040 - 73% |
| Register | 9185/23685 - 39% |
| CLS | 9805/11520 - 86% |
| BSRAM | 22/56 - 40% |
| Constraint | 50,000 MHz |
| Fmax | 54,737 MHz |
| Setup violated endpoints | 0 |
| Hold violated endpoints | 0 |
| TNS setup/hold | 0/0 |

Bitstream:

```text
BTC/bitcoin-core/c-signer/fpga/gowin/impl/pnr/dual_signer_acg525.fs
```

SHA-256 của bitstream đã nạp:

```text
ea282426b48b2a5a3dee11b6c7bd204975190964c257623197035eb6d58afb32
```

## Trạng thái nạp Flash và kiểm thử phần cứng

Thiết bị được Programmer nhận dạng:

```text
FPGA family/device: GW5A-25A
JTAG ID:           0x0001281B
External SPI ID:   0xC84018
```

Image được ghi bằng operation:

```text
exFlash Erase,Program,Verify Arora V
```

Programmer đã báo `Program and Verify Flash successfully`. Sau đó FPGA được
phát lệnh `Reprogram` để boot lại từ Flash; status đọc lại là `0x70022020`.

UART thật được kiểm tra qua:

```text
/dev/serial/by-id/usb-1a86_USB_Single_Serial_5757024500-if00
```

Kết quả:

- PING nhận đúng `PONG`;
- ETH ký đúng `yParity/r/s` của known-answer vector;
- code C đại diện MCU gửi request BTC và nhận đúng
  `freeze_id/digest/r/s/CRC`;
- PING sau hai lần ký vẫn thành công, xác nhận parser trở lại trạng thái rảnh.

## Code C MCU dual

Project tích hợp nằm tại:

```text
DUAL/mcu/
├── Makefile
├── src/dual_mcu.c          entrypoint chọn coin
├── src/eth_mcu_fpga.c      flow EIP-1559 MCU -> FPGA -> raw tx
└── test-data/              public key và PIN record chỉ dùng test
```

Binary chỉ có một entrypoint và namespace rõ ràng:

```bash
cd /media/abuntu/NVME_UBUNTU1/Working/MCU_FPGA
make -C DUAL/mcu clean all

DUAL/mcu/bin/dual-mcu btc selftest
DUAL/mcu/bin/dual-mcu eth address DUAL/mcu/test-data/fpga_test_d1.pub
```

Nhánh BTC compile lại toàn bộ `coldsign.c` hiện có dưới entrypoint
`btc_mcu_main`, vì vậy parser PSBT, review output/change/fee, freeze, passkey,
BIP143 response check và ghép partial signature không bị rút gọn.

Nhánh ETH thực hiện theo thứ tự:

1. kiểm tra các trường EIP-1559 type 2 và encode RLP;
2. tạo Keccak-256 signing hash và SHA-256 `freeze_id`;
3. hiển thị from/to/value/chain/nonce/gas/max fee/freeze ID;
4. kiểm tra cùng định dạng PIN record `CSPIN1` của BTC và xác nhận Đồng ý/Hủy;
5. hash lại buffer đã freeze và từ chối nếu dữ liệu thay đổi;
6. gửi command `0x01` cùng hash 32 byte đến FPGA;
7. recover public key từ `yParity/r/s`, so với public key provisioned;
8. ghép raw EIP-1559 transaction và tính transaction hash.

Không có private key trong code C dual. File `fpga_test_d1.pub` chỉ là public
key nén tương ứng với khóa test đang provision trong FPGA.

Ví dụ ký ETH bằng FPGA thật:

```bash
DUAL/mcu/bin/dual-mcu eth sign-fpga \
  DUAL/mcu/test-data/fpga_test_d1.pub \
  BTC/bitcoin-core/c-signer/keys/mcu.passkey \
  /dev/serial/by-id/usb-1a86_USB_Single_Serial_5757024500-if00 \
  --chain-id 31337 --nonce 0 \
  --max-priority-fee-per-gas 1000000000 \
  --max-fee-per-gas 2000000000 --gas-limit 21000 \
  --to 0x1111111111111111111111111111111111111111 \
  --value 10000000000000000
```

## Web dual

Một process web duy nhất nằm tại `DUAL/web/app.py` và chỉ cho phép bind vào
loopback. Trang đầu chọn coin; hai ứng dụng được mount theo namespace:

| URL | Chức năng |
|---|---|
| `/` | Trang chọn BTC/ETH và mô tả flow chung |
| `/btc/` | Bitcoin Core regtest, PSBT/file/BBQr/finalize/broadcast |
| `/btc/api/...` | API Bitcoin |
| `/eth/` | Anvil EIP-1559 build/sign/broadcast |
| `/eth/api/...` | API Ethereum |

Chạy web:

```bash
cd /media/abuntu/NVME_UBUNTU1/Working/MCU_FPGA/DUAL/web
/home/abuntu/miniconda3/envs/crypto/bin/python app.py
```

Sau đó mở `http://127.0.0.1:8787`.

Endpoint ETH không còn gọi trực tiếp lệnh ký của UART Python. Nó chạy
`dual-mcu eth sign-fpga`, chuyển PIN người dùng vào code đại diện MCU và chỉ
nhận raw transaction sau khi C đã review/freeze và recover-verify chữ ký FPGA.
API trạng thái vẫn chỉ dùng PING để kiểm tra FPGA, không phát sinh chữ ký.

Các biến môi trường có thể cấu hình gồm `FPGA_UART_PORT`, `DUAL_MCU_BIN`,
`FPGA_PUBLIC_KEY`, `MCU_PASSKEY_RECORD` và `ANVIL_RPC_URL`.

## Code nguồn được tái sử dụng

### Bitcoin

```text
BTC/bitcoin-core/c-signer/
```

- `src/coldsign.c`: chương trình C trên host đại diện MCU;
- `src/fpga_protocol.c`: encode/decode UART BTC;
- `mcu/src/btc_mcu_flow.c`: flow thuần C để port sang SDK Sonix;
- `web/server.py`: web Bitcoin Core/PSBT/BBQr hiện tại.

### Ethereum

```text
ETH/anvil_cold_wallet/
```

- `wallet.py`: build và broadcast EIP-1559 transaction;
- `fpga_wallet.py`: tạo signing hash và ghép `yParity/r/s`;
- `fpga_uart.py`: client command ETH `0x01`;
- `app.py`, `templates/`, `static/`: web Ethereum hiện tại;
- `c_signer/src/eth_signer.c`: C reference cho RLP/Keccak/ECDSA.

Các nhánh gốc vẫn tồn tại để regression. `DUAL/mcu` liên kết lại code BTC gốc
và thư viện libsecp256k1 đã build trong nhánh ETH; `DUAL/web` mount chức năng
hai web dưới một WSGI process thay vì sao chép logic Bitcoin Core/Web3.

## Kết quả test tích hợp C và web

- `FPGA_PROTOCOL_TEST_OK`: encode/decode và CRC cho BTC `0x10` lẫn ETH `0x01`;
- `dual-mcu btc selftest`: `SELFTEST_OK`;
- địa chỉ từ public key FPGA: `0x7e5f4552091a69125d5dfcb7b8c2659029395bdf`;
- C tạo giao dịch EIP-1559 0.01 ETH, freeze, mở khóa, ký qua UART thật,
  recover đúng public key và xuất `signatureVerified: true`;
- HTTP test: `/`, `/btc/`, `/eth/` đều 200;
- `/btc/api/status` đọc được Bitcoin Core regtest ở block 230;
- `/eth/api/fpga-signer` PING thành công tại 115200 baud;
- `/eth/api/sign-fpga` chạy full C -> UART -> FPGA -> C -> web và Web3
  recover raw transaction đúng signer.

## Quy tắc an toàn

- FPGA hiện dùng **một private key provisioned chung** cho BTC và ETH.
- Image hiện tại chứa khóa thử nghiệm; không dùng với BTC/ETH thật.
- Không commit file private key RTL hoặc private key C.
- MCU phải kiểm tra và đóng băng transaction trước khi kích hoạt FPGA.
- FPGA không được nhận trực tiếp yêu cầu ký từ web online trong thiết kế thật.
- Ở bản thử nghiệm host này, web và code C chạy trên cùng máy để mô phỏng MCU;
  khi port sang SN34F788F, PIN/nút/màn hình phải nằm trên MCU vật lý và web chỉ
  được chuyển dữ liệu chưa ký/đã ký qua kênh transport.
- UART dùng TTL 3,3 V, không nối vào RS-232 mức điện áp ±12 V.
- Chỉ broadcast sau khi máy online tự kiểm tra chữ ký/raw transaction.

## Kết luận

Project hiện có một image FPGA dual, một binary C dual đại diện MCU và một web
localhost dual. Cả ETH hardware flow và trạng thái BTC/FPGA đã qua test tích
hợp. Công việc phần cứng còn lại là port các module C/policy/UI sang SDK Sonix
SN34F788F, thay public key/khóa test bằng quy trình provision an toàn và kiểm
thử transport với MCU thật.
