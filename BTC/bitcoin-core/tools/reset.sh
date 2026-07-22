#!/usr/bin/env bash
set -euo pipefail
export PATH="$HOME/workspace/dinhanh_k68/bitcoin-core/bin:$PATH"
regtest_dir="$HOME/workspace/dinhanh_k68/bitcoin-core/data/regtest"
printf 'This removes only %s. Type RESET to continue: ' "$regtest_dir"
read -r answer
if [ "$answer" != "RESET" ]; then echo "Cancelled"; exit 0; fi
if bitcoin-cli -regtest getblockchaininfo >/dev/null 2>&1; then
  bitcoin-cli -regtest stop >/dev/null
  for _ in $(seq 1 30); do bitcoin-cli -regtest getblockchaininfo >/dev/null 2>&1 || break; sleep 1; done
fi
if [ -d "$regtest_dir" ]; then rm -rf -- "$regtest_dir"; echo "Removed only $regtest_dir"; else echo "Regtest directory does not exist"; fi
