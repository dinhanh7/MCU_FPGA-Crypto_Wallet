#!/usr/bin/env bash
set -euo pipefail
export PATH="$HOME/workspace/dinhanh_k68/bitcoin-core/bin:$PATH"
if bitcoin-cli -regtest getblockchaininfo >/dev/null 2>&1; then bitcoin-cli -regtest stop; else echo "Bitcoin Core regtest is not running"; fi
