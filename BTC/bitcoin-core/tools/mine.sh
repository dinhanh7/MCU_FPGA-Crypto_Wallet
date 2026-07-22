#!/usr/bin/env bash
set -euo pipefail
export PATH="$HOME/workspace/dinhanh_k68/bitcoin-core/bin:$PATH"
number="${1:-}"
case "$number" in ''|*[!0-9]*) echo "Usage: $0 NUMBER" >&2; exit 2;; esac
bitcoin-cli -regtest getblockchaininfo >/dev/null 2>&1 || "$HOME/workspace/dinhanh_k68/bitcoin-core/tools/start.sh"
if ! bitcoin-cli -regtest -rpcwallet=miner getwalletinfo >/dev/null 2>&1; then bitcoin-cli -regtest loadwallet miner >/dev/null 2>&1 || bitcoin-cli -regtest createwallet miner >/dev/null; fi
address=$(bitcoin-cli -regtest -rpcwallet=miner getnewaddress)
bitcoin-cli -regtest generatetoaddress "$number" "$address"
