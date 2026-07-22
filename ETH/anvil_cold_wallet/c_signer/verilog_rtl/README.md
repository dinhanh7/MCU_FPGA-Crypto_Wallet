# Ethereum hash signer RTL cho Gowin ACG525

Project Gowin hiện chỉ giữ phần ký recoverable ECDSA secp256k1 trong FPGA.
MCU chịu trách nhiệm kiểm tra giao dịch, RLP encode EIP-1559 và Keccak-256, sau
đó gửi đúng 32 byte `messageHash` xuống FPGA. FPGA trả về `yParity`, `r` và `s`.

`eip1559_rlp_encoder.sv` và `keccak256_stream.sv` vẫn được giữ làm mã tham
chiếu và regression của thiết kế đầy đủ cũ, nhưng không còn nằm trong project
Gowin `eth_signer_acg525.gprj`.

## Giao diện core

Top logic cần tích hợp với SPI/UART là `rtl/eth_hash_signer_core.sv`:

| Tín hiệu | Hướng | Ý nghĩa |
|---|---|---|
| `clk` | input | Clock đồng bộ |
| `reset_n` | input | Reset bất đồng bộ, active-low |
| `start` | input | Xung một chu kỳ để bắt đầu ký |
| `message_hash[255:0]` | input | Hash 32 byte, big-endian |
| `busy` | output | Bằng 1 trong khi đang ký |
| `done` | output | Xung một chu kỳ khi kết thúc |
| `error` | output | Kết quả không hợp lệ khi `done=1` |
| `error_code[3:0]` | output | `1`: private key sai; `2`: recovery ID không dùng được cho Ethereum |
| `y_parity` | output | Recovery parity 0 hoặc 1 |
| `signature_r[255:0]` | output | Thành phần chữ ký `r`, big-endian |
| `signature_s[255:0]` | output | Thành phần low-s `s`, big-endian |

Byte đầu tiên của hash MCU gửi phải ánh xạ vào `message_hash[255:248]`, byte
cuối cùng vào `message_hash[7:0]`. Chỉ phát `start` khi `busy=0`; core tự chốt
hash tại thời điểm bắt đầu và giữ output đến lần ký tiếp theo.

Nonce RFC6979 vẫn cần HMAC-SHA256 nên `sha256_stream.sv` vẫn nằm trong FPGA.
Đây không phải Keccak-256 của giao dịch.

## Cấu trúc Gowin

- `rtl/eth_hash_signer_core.sv`: core ký hash tổng hợp được.
- `rtl/rfc6979_nonce.sv`: RFC6979 và cơ chế retry giống libsecp256k1.
- `rtl/sha256_stream.sv`: SHA-256 phục vụ RFC6979.
- `rtl/secp256k1_point_mul.sv`: nhân điểm Jacobian secp256k1.
- `rtl/modmul256_controller.sv`: nhân modulo tuần tự.
- `rtl/modinv256_controller.sv`: nghịch đảo modulo dùng multiplier chung.
- `gowin/acg525_selftest_top.sv`: wrapper tự ký một hash cố định và báo LED.
- `gowin/eth_signer_acg525.gprj`: project cho `GW5A-LV25UG324C2/I1`.
- `tb/tb_eth_hash_signer_core.sv`: regression hash signer.
- `tests/compare_c_rtl_hash.py`: đối chiếu trực tiếp executable C với RTL.

`rtl/eth_signer_core.sv`, RLP và Keccak vẫn tồn tại để chứng minh đường giao
dịch đầy đủ cũ không bị hỏng, nhưng chúng không được nạp vào ACG525 nữa.

## Chạy test

```bash
cd /media/abuntu/NVME_UBUNTU1/Working/MCU_FPGA/ETH/anvil_cold_wallet/c_signer
cmake --build build -j2
cd verilog_rtl
make hash
make compare-c-rtl
make acg525
make full edge unit
make lint synth-check
```

Kết quả quan trọng:

```text
PASS: C and RTL match for 0x...
PASS: ACG525 power-on self-test completed in 33160980 cycles
PASS: complete synthesizable Ethereum signer matches C exactly
```

Self-test hash signer mất 33.160.980 chu kỳ, tương đương khoảng 663,22 ms ở
50 MHz. Thời gian thực tế có thể thay đổi nếu clock hệ thống khác 50 MHz.

## Build bằng Gowin EDA

Mở project:

```text
gowin/eth_signer_acg525.gprj
```

Project đã chọn part `GW5A-LV25UG324C2/I1`, top `acg525_selftest_top`, clock
50 MHz chân `T9`, LED busy chân `N16` và LED pass chân `C17`. Chạy **Run All**;
sau khi place-route thành công, Programmer chọn bitstream:

```text
gowin/impl/pnr/eth_signer_acg525.fs
```

Kết quả Gowin EDA V1.9.12.03 trên project hiện tại:

- Place-route và bitstream generation hoàn tất, không còn unrouted net.
- Timing 50 MHz đạt: Fmax 63,135 MHz, setup/hold TNS bằng 0.
- Logic `12.471/23.040` (55%), register `8.571/23.685` (37%).
- BSRAM `19/56` (34%), chỉ dùng 3 chân I/O.

Sau khi nạp wrapper self-test:

- `led_busy` sáng trong khi FPGA ký hash cố định.
- `led_pass` sáng và giữ nguyên nếu `yParity/r/s` khớp vector chuẩn.

Wrapper này chỉ là self-test, chưa có giao thức MCU. Khi tích hợp thật, thay
`acg525_selftest_top.sv` bằng wrapper SPI/UART của bo mạch và giữ nguyên
`eth_hash_signer_core` cùng handshake ở trên.

## Private key

`PRIVATE_KEY` là parameter 256 bit của `eth_hash_signer_core`. Top self-test dùng
key công khai `1`; tuyệt đối không nạp ETH thật vào địa chỉ của key này.

Hard-code private key trong bitstream chỉ phù hợp prototype. Thiết bị thật cần
quy trình nạp key riêng, chống đọc ngược và MCU/màn hình xác nhận độc lập nội
dung giao dịch trước khi gửi hash và cho phép FPGA ký.
