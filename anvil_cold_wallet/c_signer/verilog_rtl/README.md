# Ethereum offline signer RTL cho ACG525

Đây là bản SystemVerilog **có thể tổng hợp**, thực hiện toàn bộ đường ký giao
dịch Ethereum EIP-1559 Type 2:

1. RLP encode giao dịch chưa ký.
2. Keccak-256 message hash.
3. RFC6979/HMAC-SHA256 tạo nonce xác định.
4. ECDSA secp256k1, chuẩn hóa low-s và `yParity`.
5. RLP encode giao dịch đã ký và Keccak-256 transaction hash.

`eth_signer_core` dùng bus nhị phân cố định và RAM dữ liệu, vì chuỗi JSON không
phải là giao diện chân FPGA. Testbench cung cấp đúng cùng nội dung transaction
với JSON đầu vào của signer C và so sánh bit-for-bit các output `address`,
`messageHash`, `r`, `s`, `yParity`, raw transaction và transaction hash.

## Cấu trúc

- `rtl/eth_signer_core.sv`: top core tổng hợp được.
- `rtl/eip1559_rlp_encoder.sv`: RLP EIP-1559 dạng stream.
- `rtl/keccak256_stream.sv`: Keccak-256 tuần tự, tối ưu diện tích.
- `rtl/sha256_stream.sv`: SHA-256 với cửa sổ schedule 16 word.
- `rtl/rfc6979_nonce.sv`: HMAC-SHA256/RFC6979.
- `rtl/secp256k1_point_mul.sv`: nhân điểm Jacobian secp256k1.
- `rtl/modmul256.sv`: nhân modulo 256 bit tuần tự.
- `rtl/modinv256_controller.sv`: nghịch đảo modulo dùng chung multiplier.
- `gowin/acg525_selftest_top.sv`: top phần cứng tự ký một vector cố định.
- `tb/`: unit test và full differential test.

Hai buffer `calldata` 2 KiB và raw transaction 4 KiB được khai báo theo mẫu
RAM đồng bộ và có thuộc tính block-RAM dành cho GW5A.

## Chạy test

```bash
cd /media/abuntu/NVME_UBUNTU1/Working/MCU_FPGA/ETH/anvil_cold_wallet/c_signer/verilog_rtl
make unit
make full
make acg525
make lint synth-check
```

`make full` phải kết thúc bằng:

```text
PASS: complete synthesizable Ethereum signer matches C exactly
```

`make acg525` mô phỏng đúng wrapper nguồn 50 MHz và phải kết thúc bằng:

```text
PASS: ACG525 power-on self-test completed in 1211103 cycles
```

Tương đương khoảng 24,22 ms ở 50 MHz cho vector self-test dùng private key 1.

## Build bằng Gowin EDA

Mở project:

```text
gowin/eth_signer_acg525.gprj
```

Project đã chọn đúng part `GW5A-LV25UG324C2/I1`, top
`acg525_selftest_top`, clock 50 MHz chân `T9`, LED busy chân `N16` và LED pass
chân `C17`. Chạy lần lượt **Synthesize**, **Place & Route**, rồi tạo bitstream.

Sau khi nạp FPGA:

- `led_busy` sáng trong khi ký.
- `led_pass` sáng và giữ nguyên nếu transaction hash khớp vector chuẩn.

Wrapper này cố ý không dùng UART. Khi bổ sung giao tiếp thật, giữ nguyên
`eth_signer_core` và nối frontend SPI/UART/USB vào các bus transaction cùng RAM
write/read của core.

## Private key

`PRIVATE_KEY` là parameter 256 bit của `eth_signer_core`. Top self-test đang dùng
key công khai `1`; tuyệt đối không nạp ETH thật vào địa chỉ của key này.

Hard-code private key trong bitstream chỉ phù hợp prototype: key có thể bị lộ
qua source, file build hoặc trích xuất bitstream. Thiết bị thật cần vùng key có
bảo vệ, quy trình nạp key riêng, chống đọc ngược và màn hình xác nhận độc lập
cho `to`, `value`, Chain ID và phí trước khi cho phép `start`.

## Giới hạn xác minh

Môi trường hiện tại có Icarus, Verilator và Yosys nên đã kiểm tra chức năng,
lint, cấu trúc tổng hợp và ánh xạ họ GW5A. Gowin EDA chưa được cài trên máy này;
vì vậy bước place-and-route/timing chính thức phải chạy khi mở `.gprj` bằng
Gowin EDA.
