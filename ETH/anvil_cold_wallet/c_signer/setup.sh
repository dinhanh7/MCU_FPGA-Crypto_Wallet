#!/usr/bin/env bash
set -euo pipefail

cd "$(dirname "$0")"

if [[ ! -d vendor/secp256k1/.git ]]; then
  mkdir -p vendor
  git clone --depth 1 --branch v0.7.1 \
    https://github.com/bitcoin-core/secp256k1.git vendor/secp256k1
fi

if [[ ! -f include/private_key.h ]]; then
  cp include/private_key.example.h include/private_key.h
  chmod 600 include/private_key.h
  echo "Created include/private_key.h. Replace the example key before real use."
fi

cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel

echo "Built: $(pwd)/build/eth_signer"
