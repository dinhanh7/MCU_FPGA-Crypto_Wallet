# Python cold-wallet prototype for Anvil

CLI này mô phỏng đúng luồng ví lạnh trong tài liệu dự án:

1. Máy **online** truy vấn Anvil và tạo unsigned transaction.
2. Máy **offline** giữ encrypted keystore, kiểm tra và ký transaction.
3. Máy **online** gửi raw signed transaction lên Anvil.

Private key không được ghi trong source code hoặc truyền bằng đối số command line.
Keystore được mã hóa theo Ethereum Web3 Secret Storage và được đặt permission `600`.

> `send` là lệnh demo tiện dụng, không phải cold-wallet thực sự, vì nó giải mã private
> key trên cùng máy đang kết nối mạng. Với mô hình STM32 sau này, thay bước `sign` bằng
> việc gửi transaction digest xuống phần cứng và nhận chữ ký `r, s, v`.

## Môi trường

```bash
cd /media/abuntu/NVME_UBUNTU1/Working/MCU_FPGA/ETH/anvil_cold_wallet
conda activate crypto
python -m pip install -r requirements.txt
```

RPC Anvil hiện tại:

```bash
export ANVIL_RPC_URL=https://switched-revised-fort-finances.trycloudflare.com
```

Quick Tunnel sẽ đổi URL sau mỗi lần restart. Có thể truyền `--rpc-url URL` vào từng
lệnh thay vì export biến môi trường.

## Giao diện HTML

Chạy web server local:

```bash
cd /media/abuntu/NVME_UBUNTU1/Working/MCU_FPGA/ETH/anvil_cold_wallet
conda activate crypto
python app.py
```

Mở trình duyệt tại:

```text
http://127.0.0.1:5000
```

Web server cố ý chỉ cho phép bind loopback. Không dùng `0.0.0.0` hoặc đưa giao diện
ví qua TryCloudflare, vì form này xử lý private key và password keystore. JSON-RPC
Anvil vẫn có thể là URL Cloudflare public; bản thân giao diện ký phải chạy local.

Trên giao diện:

1. Kiểm tra RPC và Chain ID.
2. Tạo hoặc import encrypted keystore.
3. Chọn ví, nhập địa chỉ nhận và số ETH rồi build.
4. Kiểm tra toàn bộ transaction trước khi nhập password và ký.
5. Tải file unsigned/signed nếu muốn chuyển giữa máy online/offline.
6. Broadcast raw transaction và kiểm tra receipt.

### Dùng C offline signer trên web

Build C signer trước:

```bash
cd c_signer
./setup.sh
cd ..
```

Tải lại web và chọn mục `C offline signer` trong danh sách ví. Với signer này,
giao diện không hỏi password keystore: Python gửi các trường unsigned transaction
cho process C, C tự RLP/Keccak/ECDSA và trả raw transaction đã ký.

Trong thiết kế phần cứng thật, không gọi process C trên cùng máy web. Hãy tải
`unsigned_transaction.json`, chuyển nó qua USB/UART tới STM32, xác nhận trên màn hình
phần cứng, rồi chuyển raw signed transaction về máy online để broadcast.

Chi tiết build và giao thức C nằm tại `c_signer/README.md`.

### Dùng Gowin ACG525 FPGA signer trên web

Nạp bitstream UART, nối USB-UART 3.3 V và kiểm tra cổng serial xuất hiện. Mặc định
web dùng `/dev/ttyACM0`, có thể đổi bằng biến môi trường:

```bash
export FPGA_UART_PORT=/dev/ttyACM0
export FPGA_UART_TIMEOUT=3
python app.py
```

Khi mở trang, backend gửi `PING` và ký một challenge cố định để recover địa chỉ
Ethereum của private key đang nằm trong FPGA. Nếu thành công, dropdown signer sẽ có
thêm mục `Gowin ACG525 FPGA`. Chọn mục này để build giao dịch từ đúng địa chỉ FPGA,
sau đó bấm `Ký bằng FPGA qua UART`. MCU/Python chỉ tạo EIP-1559 signing hash; FPGA
trả `yParity`, `r`, `s`, và backend tự recover kiểm tra trước khi cho broadcast.

## Tạo ví mới

```bash
mkdir -p wallets
python wallet.py create --keystore wallets/alice.json
python wallet.py address --keystore wallets/alice.json
```

## Import private key test có sẵn

Private key được nhập bằng prompt ẩn, không đặt trực tiếp trong command:

```bash
python wallet.py import-key --keystore wallets/anvil-account.json
```

## Kiểm tra balance

```bash
python wallet.py balance --keystore wallets/alice.json
```

Hoặc kiểm tra một địa chỉ không cần keystore:

```bash
python wallet.py balance --address 0xYourAddress
```

## Luồng cold-wallet khuyến nghị

### 1. Build trên máy online

Máy online chỉ cần biết địa chỉ gửi, không cần private key:

```bash
python wallet.py build \
  --from-address 0xYourSenderAddress \
  --to 0xRecipientAddress \
  --amount 0.1 \
  --output unsigned_transaction.json
```

Chuyển `unsigned_transaction.json` sang máy offline bằng USB hoặc kênh phù hợp.

### 2. Kiểm tra và ký trên máy offline

```bash
python wallet.py inspect --unsigned unsigned_transaction.json
python wallet.py sign \
  --keystore wallets/alice.json \
  --unsigned unsigned_transaction.json \
  --output signed_transaction.json
```

Chuyển `signed_transaction.json` về máy online. Raw transaction đã ký có thể công
khai; nó không chứa private key.

Hoặc ký bằng C signer đã build:

```bash
python wallet.py sign-c \
  --signer c_signer/build/eth_signer \
  --unsigned unsigned_transaction.json \
  --output signed_transaction.json \
  --yes
```

### 3. Broadcast trên máy online

```bash
python wallet.py broadcast --signed signed_transaction.json
```

## Demo nhanh trên một máy

```bash
python wallet.py send \
  --keystore wallets/alice.json \
  --to 0xRecipientAddress \
  --amount 0.01
```

## Lưu ý bảo mật

- Chỉ dùng private key Anvil cho test; không dùng trên Ethereum Mainnet.
- Không commit thư mục `wallets/`, mnemonic, private key hoặc password.
- Máy ký offline không được cấu hình RPC và không cần Internet.
- Luôn kiểm tra địa chỉ nhận, số ETH, Chain ID, nonce và maximum fee trước khi ký.
- Quick Tunnel làm JSON-RPC Anvil công khai; bất kỳ ai biết URL đều có thể gọi RPC.
