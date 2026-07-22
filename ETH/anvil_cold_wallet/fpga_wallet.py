from __future__ import annotations

from typing import Any, Protocol

from eth_account import Account
from eth_account._utils.legacy_transactions import (
    encode_transaction,
    serializable_unsigned_transaction_from_dict,
)
from eth_utils import to_checksum_address
from web3 import Web3

from wallet import WalletError


class HashSigner(Protocol):
    def sign_hash(self, message_hash: bytes) -> dict[str, int]: ...


def transaction_for_fpga(unsigned: dict[str, Any]) -> dict[str, Any]:
    if unsigned.get("format") != "anvil-cold-wallet-unsigned-v1":
        raise WalletError("Unsupported unsigned transaction format")
    transaction = unsigned.get("transaction")
    if not isinstance(transaction, dict):
        raise WalletError("Unsigned file has no transaction object")
    normalized = dict(transaction)
    if normalized.get("type") != 2:
        raise WalletError("FPGA signer only supports EIP-1559 type-2 transactions")
    normalized.setdefault("data", "0x")
    normalized.setdefault("accessList", [])
    return normalized


def signing_hash(unsigned: dict[str, Any]) -> bytes:
    transaction = transaction_for_fpga(unsigned)
    serializable = serializable_unsigned_transaction_from_dict(transaction)
    return bytes(serializable.hash())


def assemble_signed_transaction(
    unsigned: dict[str, Any], y_parity: int, signature_r: int, signature_s: int
) -> dict[str, Any]:
    if y_parity not in (0, 1):
        raise WalletError("FPGA returned invalid yParity")
    transaction = transaction_for_fpga(unsigned)
    serializable = serializable_unsigned_transaction_from_dict(transaction)
    raw_transaction = encode_transaction(
        serializable, (y_parity, signature_r, signature_s)
    )
    raw_hex = Web3.to_hex(raw_transaction)
    recovered = to_checksum_address(Account.recover_transaction(raw_hex))
    expected_sender = to_checksum_address(str(unsigned.get("from", "")))
    if recovered != expected_sender:
        raise WalletError(
            f"FPGA signature recovered {recovered}, expected {expected_sender}"
        )
    message_hash = bytes(serializable.hash())
    return {
        "format": "anvil-cold-wallet-signed-v1",
        "from": recovered,
        "transactionHash": Web3.to_hex(Web3.keccak(raw_transaction)),
        "rawTransaction": raw_hex,
        "unsigned": unsigned,
        "signature": {
            "implementation": "Gowin ACG525 UART",
            "messageHash": Web3.to_hex(message_hash),
            "yParity": y_parity,
            "r": f"0x{signature_r:064x}",
            "s": f"0x{signature_s:064x}",
        },
    }


def sign_payload_fpga(unsigned: dict[str, Any], signer: HashSigner) -> dict[str, Any]:
    message_hash = signing_hash(unsigned)
    signature = signer.sign_hash(message_hash)
    return assemble_signed_transaction(
        unsigned,
        int(signature["yParity"]),
        int(signature["r"]),
        int(signature["s"]),
    )
