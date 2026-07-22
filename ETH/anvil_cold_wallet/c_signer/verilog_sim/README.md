# Ethereum offline signer — SystemVerilog simulation model

Đây là bản SystemVerilog behavioral, dùng để mô phỏng và kiểm tra trong QuestaSim.
Nó không gọi C/DPI và tự thực hiện toàn bộ:

- decimal uint256 và hexadecimal parsing;
- Ethereum RLP;
- EIP-2718/EIP-1559 Type 2 transaction;
- Ethereum Keccak-256;
- SHA-256, HMAC-SHA256 và RFC6979;
- ECDSA recoverable trên secp256k1, low-S normalization;
- tạo địa chỉ Ethereum, raw transaction và transaction hash;
- JSON output `ethereum-c-signer-v1` giống bản C.

Model này cố ý sử dụng `string`, queue, phép `%` 512-bit và các task behavioral để
dễ kiểm chứng. Nó **không phải synthesizable RTL** và chưa có bảo vệ side-channel.

## Cấu trúc

```text
rtl/eth_signer_pkg.sv       toàn bộ thuật toán
rtl/eth_signer_model.sv     API address/sign dạng string
rtl/private_key_sim.svh     private key test, bị gitignore
tb/tb_eth_signer.sv         known-answer test khớp C signer
tb/tb_cli.sv                giao diện plusarg text cho simulator
questa/run_tests.do         chạy regression trong QuestaSim
questa/run_cli.do           chạy một giao dịch tùy chọn
tests/compare_with_c.py     differential test nhiều giao dịch
```

## Chạy QuestaSim

```tcl
cd /media/abuntu/NVME_UBUNTU1/Working/MCU_FPGA/ETH/anvil_cold_wallet/c_signer/verilog_sim/questa
vsim -c -do run_tests.do
```

Test thành công khi xuất hiện:

```text
PASS: SHA-256, Keccak-256, address, RFC6979, secp256k1, RLP and JSON match C signer.
```

Chạy CLI mô phỏng:

```tcl
cd .../verilog_sim/questa
vsim -c -do "do run_cli.do \
  +CHAIN_ID=31338 \
  +NONCE=3 \
  +MAX_PRIORITY_FEE_PER_GAS=1000000000 \
  +MAX_FEE_PER_GAS=1021491012 \
  +GAS_LIMIT=21000 \
  +TO=0x1d6D332F0aB9C6CFd95FAc2ba2b8CeFD39F012De \
  +VALUE=2000000000000000000 \
  +DATA=0x"
```

Lệnh `address`:

```tcl
vsim -c -do "do run_cli.do +COMMAND=address"
```

## API SystemVerilog

```systemverilog
eth_signer_model #(.PRIVATE_KEY(256'h...)) signer();

string output_json;
signer.sign(
  "31338", "3", "1000000000", "1021491012", "21000",
  "0x1d6D332F0aB9C6CFd95FAc2ba2b8CeFD39F012De",
  "2000000000000000000", "0x", output_json
);
```

Input vẫn là text giống các đối số CLI của C. Output là JSON text với các trường
`from`, `messageHash`, `yParity`, `r`, `s`, `rawTransaction` và `transactionHash`.

## Kiểm tra differential trên máy hiện tại

Verilator đã được cài trong môi trường Conda `crypto`:

```bash
cd /media/abuntu/NVME_UBUNTU1/Working/MCU_FPGA/ETH/anvil_cold_wallet/c_signer/verilog_sim
make test
make compare
```

`make compare` ký bốn giao dịch khác nhau bằng cả SystemVerilog và C/libsecp256k1,
sau đó so sánh toàn bộ JSON bit-for-bit.
