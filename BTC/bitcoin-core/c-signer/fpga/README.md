# Bitcoin BIP143 signer RTL cho Gowin ACG525

RTL này dành cho `GW5A-LV25UG324C2/I1` trên ACG525. FPGA nhận các trường của
giao dịch đã được SN34F788F review, không nhận digest tạo sẵn.

Luồng core:

1. Double-SHA256 outpoint, sequence và raw outputs.
2. Tạo preimage BIP143 P2WPKH/SIGHASH_ALL dài 182 byte và double-SHA256.
3. Tạo nonce RFC6979, ký ECDSA secp256k1 và chuẩn hóa low-S.
4. Tự verify bằng `((z + r*d)/s)G`; chỉ status OK khi `x mod n == r`.
5. Echo `freeze_id`, digest và `r/s` về MCU qua UART có CRC16.

## File chính

- `rtl/btc_bip143_hash.sv`: BIP143 và bốn double-SHA256.
- `rtl/btc_ecdsa_sign_verify_core.sv`: ký và verify dùng chung datapath.
- `rtl/btc_uart_signer_bridge.sv`: framing UART 115200 8N1.
- `gowin/acg525_btc_uart_top.sv`: top clock 50 MHz.
- `gowin/btc_signer_acg525.gprj`: project Gowin EDA.

Các datapath secp256k1/SHA dùng chung source đã được kiểm thử trong
`ETH/anvil_cold_wallet/c_signer/verilog_rtl/rtl`; project Gowin tham chiếu các
file đó bằng đường dẫn tương đối để không tạo hai bản crypto khác nhau.

## Test

```bash
cd BTC/bitcoin-core/c-signer/fpga
make core
make uart
make lint synth-check
```

Kết quả hiện tại:

```text
PASS: FPGA computes BIP143, signs and self-verifies in 64235335 cycles
PASS: SN34F788F protocol -> BIP143 -> verified ECDSA -> UART response
```

64.235.335 chu kỳ tương đương khoảng 1,285 giây ở 50 MHz. Yosys ánh xạ GW5A
thành công và báo không có lỗi cấu trúc. Cần chạy place-route/STA bằng Gowin EDA
trên máy có toolchain chính thức trước khi nạp bitstream.

## Provision và chân bo

Sao chép `rtl/btc_private_key_rtl.example.svh` thành
`rtl/btc_private_key_rtl.svh`, thay test key bằng key được provision và không
commit file thật. Public key tương ứng phải được nạp vào MCU.

Constraint đi kèm dùng clock/LED/UART giống project ACG525 hiện có:
clock `T9`, UART TX `V8`, UART RX `U8`, LED `N16/C17`. Hãy đối chiếu revision
PCB và mức điện áp trước khi nối SN34F788F.
