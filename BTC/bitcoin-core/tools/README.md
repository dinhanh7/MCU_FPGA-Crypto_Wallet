# Bitcoin Core Regtest + môi trường phát triển

## Đã cài đặt

- Ubuntu 22.04.5 LTS, CPU `x86_64`
- Bitcoin Core `v31.1.0`
- Các binary trong PATH:
  - `bitcoind`
  - `bitcoin-cli`
  - `bitcoin-tx`
  - `bitcoin-wallet`
- Miniforge/Conda tại:
  `/home/edabk_llm_manba/miniforge3`
- Môi trường Conda `dinhanh` tại:
  `/home/edabk_llm_manba/workspace/dinhanh_k68`
- tmux session: `btc`

Bitcoin Core archive được tải từ trang chính thức và đã xác minh SHA256 trước khi cài đặt.

## Cấu hình Bitcoin Core

File cấu hình:

`$HOME/workspace/dinhanh_k68/bitcoin-core/data/bitcoin.conf`

Node chỉ chạy Regtest:

- Network: `regtest`
- RPC bind: `127.0.0.1`
- RPC endpoint: `127.0.0.1:18443`
- Regtest data directory: `$HOME/workspace/dinhanh_k68/bitcoin-core/data/regtest`
- RPC không mở ra Internet
- Không cấu hình mainnet, testnet hoặc signet

Có thêm `fallbackfee=0.00001000` để cho phép tạo giao dịch trong regtest khi fee estimation chưa có đủ dữ liệu.

## Wallet

Hai wallet đã được tạo/load tự động:

- `miner`
- `receiver`

Kết quả kiểm thử gần nhất:

- Chain: `regtest`
- Block height: `204`
- Receiver balance: `1.00000000 BTC`
- Miner balance: `5198.99999859 BTC`
- Mempool: trống
- Receiver address:
  `bcrt1q8f0m7a0hvlh8g4mw5cpvmnve0jupl4rkgqa7uv`
- Test TXID:
  `ab5f2eae47c7ee19e4db176ba69fd39a5678aad544845269f262a59fd9dcd0fa`

## Khởi động và dừng node

Thêm binary vào PATH trong shell hiện tại:

```bash
export PATH="$HOME/workspace/dinhanh_k68/bitcoin-core/bin:$PATH"
```

Khởi động:

```$HOME/workspace/dinhanh_k68/bitcoin-core/tools/start.sh```

Dừng:

```$HOME/workspace/dinhanh_k68/bitcoin-core/tools/stop.sh```

Xem trạng thái:

```$HOME/workspace/dinhanh_k68/bitcoin-core/tools/status.sh```

## Đào block

Đào một hoặc nhiều block vào ví `miner`:

```bash
$HOME/workspace/dinhanh_k68/bitcoin-core/tools/mine.sh 1
$HOME/workspace/dinhanh_k68/bitcoin-core/tools/mine.sh 10
```

Script tự tạo/load wallet `miner` nếu cần.

## Kiểm tra thủ công

```bash
bitcoin-cli -regtest getblockchaininfo
bitcoin-cli -regtest getblockcount
bitcoin-cli -regtest getrawmempool
bitcoin-cli -regtest -rpcwallet=miner getbalance
bitcoin-cli -regtest -rpcwallet=receiver getbalance
```

Tạo địa chỉ:

```bash
bitcoin-cli -regtest -rpcwallet=miner getnewaddress
bitcoin-cli -regtest -rpcwallet=receiver getnewaddress
```

Gửi BTC Regtest:

```bash
bitcoin-cli -regtest -rpcwallet=miner sendtoaddress RECEIVER_ADDRESS 1
```

Xác nhận giao dịch:

```bash
$HOME/workspace/dinhanh_k68/bitcoin-core/tools/mine.sh 1
```

## Sử dụng tmux

Kết nối vào session đã tạo:

```tmux attach -t btc```

Tách khỏi session mà không dừng tiến trình:

`Ctrl-b`, sau đó nhấn `d`.

Kiểm tra session:

```tmux list-sessions
tmux capture-pane -pt btc:bitcoin
```

## Sử dụng Conda

Khởi tạo Conda:

```source "$HOME/miniforge3/etc/profile.d/conda.sh"```

Kích hoạt môi trường `dinhanh`:

```conda activate "$HOME/workspace/dinhanh_k68"
```

Kiểm tra:

```echo "$CONDA_PREFIX"
python --version
```

Kết quả mong đợi:

```
/home/edabk_llm_manba/workspace/dinhanh_k68
Python 3.11.15
```

## Reset Regtest

Lệnh reset chỉ xóa dữ liệu Regtest, không xóa toàn bộ `~/.bitcoin`:

```$HOME/workspace/dinhanh_k68/bitcoin-core/tools/reset.sh```

Khi được hỏi, nhập chính xác:

```
RESET
```

Nhập giá trị khác sẽ hủy thao tác.
