# Web dual

`app.py` mount:

- `/btc/` và `/btc/api/...` cho Bitcoin Core regtest/PSBT/BBQr;
- `/eth/` và `/eth/api/...` cho Anvil/EIP-1559;
- `/` là trang chọn coin.

ETH sign endpoint gọi `../mcu/bin/dual-mcu`, yêu cầu PIN và chỉ nhận raw
transaction sau khi C đã freeze/re-hash/recover-verify chữ ký FPGA.

Chạy:

```bash
/home/abuntu/miniconda3/envs/crypto/bin/python app.py
```

Server từ chối bind ngoài loopback.

## Anvil server ngoài

Tại mục **NETWORK** của trang `/eth/`, nhập trực tiếp một URL mà máy chạy web
truy cập được, ví dụ:

```text
http://192.168.1.100:8545
https://anvil.example/rpc/project-key
```

Web chấp nhận JSON-RPC qua `http://` và `https://`; URL đã kết nối thành công
được lưu trong local storage của trình duyệt. Có thể đặt giá trị mặc định bằng
biến `ANVIL_RPC_URL` trước khi khởi động web.

Nếu tự chạy Anvil trên máy khác trong LAN, Anvil phải lắng nghe trên interface
có thể truy cập, ví dụ `anvil --host 0.0.0.0 --port 8545`. Chỉ mở port trong
mạng tin cậy hoặc qua VPN/SSH tunnel. Không đặt username/password trực tiếp
trong URL và không công khai Anvil test ra Internet.
