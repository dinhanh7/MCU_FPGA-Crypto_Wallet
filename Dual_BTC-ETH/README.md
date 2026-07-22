# Dual BTC/ETH Hardware Wallet

Thư mục `Dual_BTC-ETH` là bản source đã gom của ví dual dùng:

- code C host đại diện MCU Sonix SN34F788F;
- một image FPGA Gowin ACG525 cho cả Bitcoin và Ethereum;
- một web localhost có hai namespace `/btc/` và `/eth/`.

## Cấu trúc

```text
Dual_BTC-ETH/
├── mcu/
│   ├── src/                    entrypoint dual và flow EIP-1559
│   ├── btc_signer/             parser/review/freeze/PSBT/BIP143 BTC
│   ├── vendor/secp256k1/       source libsecp256k1 recovery
│   └── test-data/              public key/PIN record chỉ dùng test
├── fpga/
│   ├── rtl/                    toàn bộ RTL BTC + datapath dùng chung ETH
│   ├── tb/                     testbench core/UART/dual
│   ├── gowin/                  project ACG525 SystemVerilog 2017
│   └── prebuilt/               bitstream đã P&R và report
├── web/
│   ├── app.py                  WSGI dispatcher dual
│   ├── bitcoin/                web/API PSBT và asset QR
│   ├── ethereum/               web/API EIP-1559 và test
│   └── runtime/                trạng thái runtime không commit
├── ARCHITECTURE.md
└── TEST_REPORT_2026-07-22.md
```

Không chứa Bitcoin blockchain data, Ethereum chain data, encrypted wallet,
private key người dùng, object file hoặc cache từ các project gốc. Source
libsecp256k1 được vendored để C có thể build không cần cây ETH bên ngoài.

## Build và test C MCU

```bash
cd mcu
make clean all
make test
```

`Makefile` tự build libsecp256k1 với recovery module rồi tạo
`mcu/bin/dual-mcu`.

Ví dụ:

```bash
bin/dual-mcu btc selftest
bin/dual-mcu eth address test-data/fpga_test_d1.pub
```

Ký thật qua UART dùng namespace `btc` hoặc `eth`; xem `bin/dual-mcu` không có
tham số để in usage đầy đủ.

## FPGA

Kiểm tra source local:

```bash
cd fpga
make lint synth-check
make dual
```

Mở `fpga/gowin/dual_signer_acg525.gprj` bằng Gowin IDE hoặc build:

```bash
cd fpga/gowin
QT_IM_MODULE=compose gw_sh build_dual_acg525.tcl
```

`QT_IM_MODULE=compose` tránh Gowin V1.9.12.03 nạp nhầm plugin IBus Qt của
hệ điều hành. Có thể bỏ biến này trên máy không gặp lỗi phiên bản Qt.

Project đặt SystemVerilog 2017, device `GW5A-LV25UG324C2/I1`, top
`acg525_dual_uart_top`. Mọi file trong `.gprj` đều nằm trong folder này.

Bitstream đã test nằm tại `fpga/prebuilt/dual_signer_acg525.fs`, SHA-256:

```text
ea282426b48b2a5a3dee11b6c7bd204975190964c257623197035eb6d58afb32
```

## Web

Web cần Bitcoin Core regtest và Anvil chạy ngoài project. Mặc định Bitcoin
Core được tìm tại `../BTC/bitcoin-core` (tương đối với folder này).

```bash
cd web
/home/abuntu/miniconda3/envs/crypto/bin/python app.py
```

Mở `http://127.0.0.1:8787`.

Trong trang `/eth/`, mục **NETWORK** cho phép nhập Anvil JSON-RPC local hoặc
server ngoài, chẳng hạn `http://192.168.1.100:8545` hay
`https://anvil.example/rpc/project-key`. Backend chỉ chấp nhận `http://` và
`https://`; xem hướng dẫn mạng và cảnh báo bảo mật tại `web/README.md`.

Biến môi trường:

- `BITCOIN_CORE_ROOT`, `BITCOIN_CLI`, `BITCOIN_DATA_DIR`;
- `ANVIL_RPC_URL`;
- `DUAL_MCU_BIN`, `FPGA_UART_PORT`, `FPGA_PUBLIC_KEY`;
- `MCU_PASSKEY_RECORD`;
- `WALLET_WEB_USERNAME`, `WALLET_WEB_PASSWORD`.

Các giá trị mặc định của `DUAL_MCU_BIN`, public key và PIN record đều trỏ vào
source local của folder này. Registry BTC chỉ chứa public key watch-only.

## Lệnh test tổng

```bash
make test       # C + protocol + FPGA lint/Yosys + web unit test
make fpga-sim   # ba simulation RTL; mất khoảng hơn một phút
```

## Cảnh báo

FPGA source và bitstream hiện chứa khóa test `d=1`; PIN record đi kèm cũng chỉ
dành cho test. Không dùng mainnet hoặc tài sản thật. Khi port sang SN34F788F,
PIN, màn hình, nút xác nhận và buffer freeze phải nằm trên MCU vật lý; web
không được trực tiếp có quyền kích hoạt signer.
