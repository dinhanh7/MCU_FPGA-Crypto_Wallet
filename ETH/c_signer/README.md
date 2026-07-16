# C offline Ethereum signer

Chương trình C này giữ private key trong `include/private_key.h`, build giao dịch
Ethereum EIP-1559 Type 2, thực hiện Keccak-256, RLP và ký recoverable ECDSA trên
đường cong secp256k1. Output là JSON chứa `r`, `s`, `yParity`, raw transaction và
transaction hash.

## Build

```bash
cd /media/abuntu/NVME_UBUNTU1/Working/MCU_FPGA/ETH/anvil_cold_wallet/c_signer
chmod +x setup.sh
./setup.sh
```

`setup.sh` tải release `v0.7.1` của thư viện chính thức
`bitcoin-core/secp256k1`, bật recovery module và build `build/eth_signer`.

## Private key

File thực tế bị `.gitignore`:

```text
include/private_key.h
```

Định dạng:

```c
#define ETH_PRIVATE_KEY_HEX \
    "64_hex_characters_without_0x"
```

Sau khi đổi key phải build lại. Xem địa chỉ signer:

```bash
./build/eth_signer address
```

## Ký giao dịch

```bash
./build/eth_signer sign \
  --chain-id 31338 \
  --nonce 0 \
  --max-priority-fee-per-gas 1000000000 \
  --max-fee-per-gas 2000000000 \
  --gas-limit 21000 \
  --to 0x1111111111111111111111111111111111111111 \
  --value 100000000000000000 \
  --data 0x
```

Không truyền `--yes` trên thiết bị offline thật: signer sẽ in toàn bộ nội dung và
yêu cầu nhập `yes`. `--yes` chỉ dành cho automated integration test.

## Giới hạn và hướng STM32

- Phiên bản này dùng Linux CLI và `/dev/urandom` để randomize context.
- Trên STM32, thay CLI bằng USB/UART framing và dùng TRNG của MCU.
- Không lưu key dạng hằng số trong firmware production. Dùng flash readout
  protection, OTP hoặc secure element.
- Màn hình phần cứng phải hiển thị `to`, `value`, Chain ID và fee trước khi ký.
