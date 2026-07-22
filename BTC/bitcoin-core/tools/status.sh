#!/usr/bin/env bash
set -euo pipefail
export PATH="$HOME/workspace/dinhanh_k68/bitcoin-core/bin:$PATH"
echo "chain: $(bitcoin-cli -regtest getblockchaininfo | sed -n 's/.*"chain": "\([^"]*\)".*/\1/p')"
echo "block_height: $(bitcoin-cli -regtest getblockcount)"
for wallet in miner receiver; do
  if bitcoin-cli -regtest -rpcwallet="$wallet" getwalletinfo >/dev/null 2>&1; then echo "balance_$wallet: $(bitcoin-cli -regtest -rpcwallet="$wallet" getbalance);"; else echo "balance_$wallet: unavailable"; fi
done
echo "mempool: $(bitcoin-cli -regtest getrawmempool | tr -d '\n')"
