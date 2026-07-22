#!/usr/bin/env bash
set -euo pipefail
export PATH="$HOME/workspace/dinhanh_k68/bitcoin-core/bin:$PATH"
bitcoin-cli -regtest getblockchaininfo >/dev/null 2>&1 || bitcoind -regtest -daemon
for _ in $(seq 1 30); do
  if bitcoin-cli -regtest getblockchaininfo >/dev/null 2>&1; then break; fi
  sleep 1
done
if ! bitcoin-cli -regtest getblockchaininfo >/dev/null 2>&1; then echo "Bitcoin Core did not become ready" >&2; exit 1; fi
ensure_wallet() {
  local wallet="$1"
  if bitcoin-cli -regtest -rpcwallet="$wallet" getwalletinfo >/dev/null 2>&1; then return 0; fi
  bitcoin-cli -regtest loadwallet "$wallet" >/dev/null 2>&1 || bitcoin-cli -regtest createwallet "$wallet" >/dev/null
}
ensure_wallet miner
ensure_wallet receiver
echo "Bitcoin Core regtest is ready (wallets: miner, receiver)"
