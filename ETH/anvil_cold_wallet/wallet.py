#!/usr/bin/env python3
"""Prototype Ethereum cold-wallet CLI for an Anvil JSON-RPC network.

The recommended flow is deliberately split into three steps:
  1. build an unsigned transaction on an online machine;
  2. sign it on an offline machine that owns the encrypted keystore;
  3. broadcast the raw signed transaction on an online machine.
"""

from __future__ import annotations

import argparse
import getpass
import json
import os
import subprocess
import sys
from decimal import Decimal, InvalidOperation
from pathlib import Path
from typing import Any

from eth_account import Account
from eth_utils import to_checksum_address
from web3 import Web3
from web3.exceptions import TransactionNotFound


DEFAULT_RPC_URL = os.environ.get("ANVIL_RPC_URL", "http://127.0.0.1:8545")
PASSWORD_ENV = "COLD_WALLET_PASSWORD"


class WalletError(RuntimeError):
    """User-facing wallet error."""


def write_json(path: Path, value: dict[str, Any], private: bool = False) -> None:
    path = path.expanduser().resolve()
    path.parent.mkdir(parents=True, exist_ok=True)
    temporary = path.with_suffix(path.suffix + ".tmp")
    temporary.write_text(json.dumps(value, indent=2) + "\n", encoding="utf-8")
    if private:
        temporary.chmod(0o600)
    temporary.replace(path)
    if private:
        path.chmod(0o600)


def read_json(path: Path) -> dict[str, Any]:
    try:
        value = json.loads(path.expanduser().read_text(encoding="utf-8"))
    except FileNotFoundError as exc:
        raise WalletError(f"File not found: {path}") from exc
    except json.JSONDecodeError as exc:
        raise WalletError(f"Invalid JSON in {path}: {exc}") from exc
    if not isinstance(value, dict):
        raise WalletError(f"Expected a JSON object in {path}")
    return value


def read_password(*, confirm: bool = False) -> str:
    from_environment = os.environ.get(PASSWORD_ENV)
    if from_environment is not None:
        if not from_environment:
            raise WalletError(f"{PASSWORD_ENV} must not be empty")
        return from_environment

    password = getpass.getpass("Keystore password: ")
    if not password:
        raise WalletError("Password must not be empty")
    if confirm and password != getpass.getpass("Confirm password: "):
        raise WalletError("Passwords do not match")
    return password


def keystore_address(path: Path) -> str:
    keystore = read_json(path)
    address = keystore.get("address")
    if not isinstance(address, str):
        raise WalletError(f"Keystore has no address: {path}")
    if not address.startswith("0x"):
        address = "0x" + address
    return to_checksum_address(address)


def decrypt_keystore(path: Path, password: str) -> bytes:
    try:
        return bytes(Account.decrypt(read_json(path), password))
    except ValueError as exc:
        raise WalletError("Could not decrypt keystore; check the password") from exc


def connect(rpc_url: str) -> Web3:
    web3 = Web3(Web3.HTTPProvider(rpc_url, request_kwargs={"timeout": 15}))
    if not web3.is_connected():
        raise WalletError(f"Cannot connect to JSON-RPC: {rpc_url}")
    return web3


def parse_eth(amount: str) -> tuple[Decimal, int]:
    try:
        decimal_amount = Decimal(amount)
    except InvalidOperation as exc:
        raise WalletError(f"Invalid ETH amount: {amount}") from exc
    if not decimal_amount.is_finite() or decimal_amount <= 0:
        raise WalletError("ETH amount must be greater than zero")
    wei = int(decimal_amount * Decimal(10**18))
    if wei <= 0 or Decimal(wei) != decimal_amount * Decimal(10**18):
        raise WalletError("ETH amount supports at most 18 decimal places")
    return decimal_amount, wei


def build_transaction(
    web3: Web3,
    from_address: str,
    to_address: str,
    amount: str,
    gas_limit: int | None = None,
) -> dict[str, Any]:
    sender = to_checksum_address(from_address)
    recipient = to_checksum_address(to_address)
    if sender == recipient:
        raise WalletError(
            "Địa chỉ nhận trùng với ví gửi; hãy chọn một địa chỉ ví khác"
        )
    decimal_amount, value = parse_eth(amount)

    nonce = web3.eth.get_transaction_count(sender, "pending")
    chain_id = web3.eth.chain_id
    pending_block = web3.eth.get_block("pending")
    base_fee = int(pending_block.get("baseFeePerGas") or web3.eth.gas_price)
    try:
        priority_fee = int(web3.eth.max_priority_fee)
    except Exception:
        priority_fee = int(web3.to_wei(1, "gwei"))
    max_fee = base_fee * 2 + priority_fee

    estimate_fields = {"from": sender, "to": recipient, "value": value}
    gas = gas_limit or int(web3.eth.estimate_gas(estimate_fields))
    transaction = {
        "type": 2,
        "chainId": chain_id,
        "nonce": nonce,
        "to": recipient,
        "value": value,
        "data": "0x",
        "gas": gas,
        "maxFeePerGas": max_fee,
        "maxPriorityFeePerGas": priority_fee,
    }
    maximum_fee_wei = gas * max_fee
    return {
        "format": "anvil-cold-wallet-unsigned-v1",
        "from": sender,
        "transaction": transaction,
        "display": {
            "amountEth": str(decimal_amount),
            "maximumFeeEth": str(web3.from_wei(maximum_fee_wei, "ether")),
        },
    }


def show_unsigned(payload: dict[str, Any]) -> None:
    transaction = payload.get("transaction")
    if not isinstance(transaction, dict):
        raise WalletError("Unsigned file has no transaction object")
    try:
        data = transaction.get("data", "0x")
        if data != "0x":
            raise WalletError(
                "This prototype only signs plain ETH transfers with empty data"
            )
        print(f"From:       {payload['from']}")
        print(f"To:         {transaction['to']}")
        print(f"Amount:     {Web3.from_wei(int(transaction['value']), 'ether')} ETH")
        print(f"Chain ID:   {transaction['chainId']}")
        print(f"Nonce:      {transaction['nonce']}")
        print(f"Gas limit:  {transaction['gas']}")
        max_fee = int(transaction["gas"]) * int(transaction["maxFeePerGas"])
        print(f"Max fee:    {Web3.from_wei(max_fee, 'ether')} ETH")
        print("Data:       0x (plain ETH transfer)")
    except (KeyError, TypeError, ValueError) as exc:
        raise WalletError("Unsigned transaction is missing required fields") from exc


def sign_payload(
    unsigned: dict[str, Any], keystore_path: Path, password: str
) -> dict[str, Any]:
    if unsigned.get("format") != "anvil-cold-wallet-unsigned-v1":
        raise WalletError("Unsupported unsigned transaction format")
    private_key = decrypt_keystore(keystore_path, password)
    account = Account.from_key(private_key)
    expected_sender = to_checksum_address(str(unsigned.get("from", "")))
    if account.address != expected_sender:
        raise WalletError(
            f"Wrong signer: transaction requires {expected_sender}, "
            f"keystore contains {account.address}"
        )
    transaction = unsigned.get("transaction")
    if not isinstance(transaction, dict):
        raise WalletError("Unsigned file has no transaction object")

    signed = Account.sign_transaction(transaction, private_key)
    raw_hex = Web3.to_hex(signed.raw_transaction)
    recovered = Account.recover_transaction(raw_hex)
    if recovered != account.address:
        raise WalletError("Internal signature verification failed")
    return {
        "format": "anvil-cold-wallet-signed-v1",
        "from": account.address,
        "transactionHash": Web3.to_hex(signed.hash),
        "rawTransaction": raw_hex,
        "unsigned": unsigned,
    }


def c_signer_address(signer_path: Path) -> str:
    signer = signer_path.expanduser().resolve()
    if not signer.is_file():
        raise WalletError(f"C signer binary not found: {signer}")
    try:
        result = subprocess.run(
            [str(signer), "address"],
            check=True,
            capture_output=True,
            text=True,
            timeout=10,
        )
    except (subprocess.CalledProcessError, OSError, subprocess.TimeoutExpired) as exc:
        raise WalletError(f"Could not query C signer: {exc}") from exc
    try:
        return to_checksum_address(result.stdout.strip())
    except ValueError as exc:
        raise WalletError("C signer returned an invalid address") from exc


def sign_payload_c(
    unsigned: dict[str, Any], signer_path: Path, *, assume_yes: bool
) -> dict[str, Any]:
    if unsigned.get("format") != "anvil-cold-wallet-unsigned-v1":
        raise WalletError("Unsupported unsigned transaction format")
    transaction = unsigned.get("transaction")
    if not isinstance(transaction, dict):
        raise WalletError("Unsigned file has no transaction object")
    if transaction.get("data") != "0x":
        raise WalletError("C signer prototype only permits plain ETH transfers")

    signer = signer_path.expanduser().resolve()
    expected_sender = to_checksum_address(str(unsigned.get("from", "")))
    actual_sender = c_signer_address(signer)
    if expected_sender != actual_sender:
        raise WalletError(
            f"Wrong C signer: transaction requires {expected_sender}, "
            f"compiled signer is {actual_sender}"
        )

    arguments = [
        str(signer),
        "sign",
        "--chain-id",
        str(transaction["chainId"]),
        "--nonce",
        str(transaction["nonce"]),
        "--max-priority-fee-per-gas",
        str(transaction["maxPriorityFeePerGas"]),
        "--max-fee-per-gas",
        str(transaction["maxFeePerGas"]),
        "--gas-limit",
        str(transaction["gas"]),
        "--to",
        str(transaction["to"]),
        "--value",
        str(transaction["value"]),
        "--data",
        str(transaction["data"]),
    ]
    if assume_yes:
        arguments.append("--yes")
    try:
        result = subprocess.run(
            arguments,
            check=True,
            capture_output=assume_yes,
            text=True,
            timeout=60,
        )
    except subprocess.CalledProcessError as exc:
        detail = (exc.stderr or "C signer rejected the request").strip()
        raise WalletError(detail) from exc
    except (OSError, subprocess.TimeoutExpired) as exc:
        raise WalletError(f"Could not execute C signer: {exc}") from exc

    if not assume_yes:
        raise WalletError(
            "Interactive C signing writes JSON to stdout; use --yes with the Python adapter "
            "or run the C binary directly for manual offline confirmation"
        )
    try:
        c_output = json.loads(result.stdout)
    except json.JSONDecodeError as exc:
        raise WalletError("C signer returned invalid JSON") from exc

    raw_transaction = c_output.get("rawTransaction")
    if not isinstance(raw_transaction, str):
        raise WalletError("C signer returned no raw transaction")
    recovered = to_checksum_address(Account.recover_transaction(raw_transaction))
    if recovered != expected_sender:
        raise WalletError("C signature recovery does not match the expected sender")
    transaction_hash = Web3.to_hex(Web3.keccak(hexstr=raw_transaction))
    if transaction_hash.lower() != str(c_output.get("transactionHash", "")).lower():
        raise WalletError("C signer transaction hash verification failed")
    return {
        "format": "anvil-cold-wallet-signed-v1",
        "from": recovered,
        "transactionHash": transaction_hash,
        "rawTransaction": raw_transaction,
        "unsigned": unsigned,
        "signature": {
            "implementation": "C/libsecp256k1",
            "messageHash": c_output.get("messageHash"),
            "yParity": c_output.get("yParity"),
            "r": c_output.get("r"),
            "s": c_output.get("s"),
        },
    }


def broadcast_payload(web3: Web3, signed: dict[str, Any]) -> dict[str, Any]:
    if signed.get("format") != "anvil-cold-wallet-signed-v1":
        raise WalletError("Unsupported signed transaction format")
    raw_transaction = signed.get("rawTransaction")
    if not isinstance(raw_transaction, str):
        raise WalletError("Signed file has no rawTransaction")
    expected_hash = signed.get("transactionHash")
    local_hash = Web3.keccak(hexstr=raw_transaction).hex()
    normalized_expected = str(expected_hash).lower().removeprefix("0x")
    normalized_local = local_hash.lower().removeprefix("0x")
    if expected_hash and normalized_expected != normalized_local:
        raise WalletError("Signed transaction hash does not match raw transaction")

    transaction_hash = "0x" + normalized_local
    already_mined = False
    try:
        receipt = web3.eth.get_transaction_receipt(transaction_hash)
        already_mined = True
    except TransactionNotFound:
        submitted_hash = web3.eth.send_raw_transaction(raw_transaction)
        receipt = web3.eth.wait_for_transaction_receipt(submitted_hash, timeout=60)
    return {
        "transactionHash": Web3.to_hex(receipt.transactionHash),
        "status": int(receipt.status),
        "blockNumber": int(receipt.blockNumber),
        "gasUsed": int(receipt.gasUsed),
        "alreadyMined": already_mined,
    }


def command_create(args: argparse.Namespace) -> None:
    path = Path(args.keystore)
    if path.expanduser().exists() and not args.force:
        raise WalletError(f"Refusing to overwrite existing keystore: {path}")
    password = read_password(confirm=True)
    account = Account.create()
    write_json(path, Account.encrypt(account.key, password), private=True)
    print(f"Created encrypted keystore: {path.expanduser().resolve()}")
    print(f"Address: {account.address}")


def command_import(args: argparse.Namespace) -> None:
    path = Path(args.keystore)
    if path.expanduser().exists() and not args.force:
        raise WalletError(f"Refusing to overwrite existing keystore: {path}")
    private_key = getpass.getpass("Private key (hidden): ").strip()
    try:
        account = Account.from_key(private_key)
    except ValueError as exc:
        raise WalletError("Invalid private key") from exc
    password = read_password(confirm=True)
    write_json(path, Account.encrypt(account.key, password), private=True)
    print(f"Imported encrypted keystore: {path.expanduser().resolve()}")
    print(f"Address: {account.address}")


def command_address(args: argparse.Namespace) -> None:
    print(keystore_address(Path(args.keystore)))


def command_balance(args: argparse.Namespace) -> None:
    address = (
        to_checksum_address(args.address)
        if args.address
        else keystore_address(Path(args.keystore))
    )
    web3 = connect(args.rpc_url)
    balance = web3.eth.get_balance(address)
    print(f"Address:  {address}")
    print(f"Balance:  {web3.from_wei(balance, 'ether')} ETH")
    print(f"Chain ID: {web3.eth.chain_id}")


def command_build(args: argparse.Namespace) -> None:
    sender = (
        to_checksum_address(args.from_address)
        if args.from_address
        else keystore_address(Path(args.keystore))
    )
    unsigned = build_transaction(
        connect(args.rpc_url), sender, args.to, args.amount, args.gas_limit
    )
    write_json(Path(args.output), unsigned)
    show_unsigned(unsigned)
    print(f"Unsigned transaction: {Path(args.output).expanduser().resolve()}")


def command_inspect(args: argparse.Namespace) -> None:
    show_unsigned(read_json(Path(args.unsigned)))


def command_sign(args: argparse.Namespace) -> None:
    unsigned = read_json(Path(args.unsigned))
    show_unsigned(unsigned)
    if not args.yes:
        confirmation = input("Sign this transaction? Type 'yes': ").strip().lower()
        if confirmation != "yes":
            raise WalletError("Signing cancelled")
    signed = sign_payload(unsigned, Path(args.keystore), read_password())
    write_json(Path(args.output), signed, private=True)
    print(f"Signed transaction: {Path(args.output).expanduser().resolve()}")
    print(f"Transaction hash:   {signed['transactionHash']}")


def command_c_address(args: argparse.Namespace) -> None:
    print(c_signer_address(Path(args.signer)))


def command_sign_c(args: argparse.Namespace) -> None:
    unsigned = read_json(Path(args.unsigned))
    show_unsigned(unsigned)
    if not args.yes:
        raise WalletError(
            "For manual confirmation, run c_signer/build/eth_signer directly. "
            "The adapter requires --yes after reviewing this transaction."
        )
    signed = sign_payload_c(unsigned, Path(args.signer), assume_yes=True)
    write_json(Path(args.output), signed, private=True)
    print(f"C-signed transaction: {Path(args.output).expanduser().resolve()}")
    print(f"Transaction hash:     {signed['transactionHash']}")


def command_broadcast(args: argparse.Namespace) -> None:
    result = broadcast_payload(connect(args.rpc_url), read_json(Path(args.signed)))
    print(f"Transaction hash: {result['transactionHash']}")
    print(f"Status:           {result['status']}")
    print(f"Block:            {result['blockNumber']}")
    print(f"Gas used:         {result['gasUsed']}")


def command_send(args: argparse.Namespace) -> None:
    web3 = connect(args.rpc_url)
    keystore = Path(args.keystore)
    unsigned = build_transaction(
        web3, keystore_address(keystore), args.to, args.amount, args.gas_limit
    )
    show_unsigned(unsigned)
    if not args.yes:
        confirmation = input("Sign and broadcast? Type 'yes': ").strip().lower()
        if confirmation != "yes":
            raise WalletError("Transaction cancelled")
    signed = sign_payload(unsigned, keystore, read_password())
    result = broadcast_payload(web3, signed)
    print(f"Transaction hash: {result['transactionHash']}")
    print(f"Status:           {result['status']}")
    print(f"Block:            {result['blockNumber']}")
    print(f"Gas used:         {result['gasUsed']}")


def rpc_option(parser: argparse.ArgumentParser) -> None:
    parser.add_argument(
        "--rpc-url",
        default=DEFAULT_RPC_URL,
        help=f"Anvil JSON-RPC URL (default: {DEFAULT_RPC_URL})",
    )


def transaction_options(parser: argparse.ArgumentParser) -> None:
    parser.add_argument("--to", required=True, help="Recipient Ethereum address")
    parser.add_argument("--amount", required=True, help="Amount in ETH")
    parser.add_argument("--gas-limit", type=int, help="Override estimated gas limit")


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(
        description="Encrypted Python cold-wallet prototype for Ethereum/Anvil"
    )
    subparsers = parser.add_subparsers(dest="command", required=True)

    create = subparsers.add_parser("create", help="Create a new encrypted wallet")
    create.add_argument("--keystore", required=True)
    create.add_argument("--force", action="store_true")
    create.set_defaults(handler=command_create)

    import_key = subparsers.add_parser(
        "import-key", help="Import a private key into an encrypted keystore"
    )
    import_key.add_argument("--keystore", required=True)
    import_key.add_argument("--force", action="store_true")
    import_key.set_defaults(handler=command_import)

    address = subparsers.add_parser("address", help="Show keystore address")
    address.add_argument("--keystore", required=True)
    address.set_defaults(handler=command_address)

    balance = subparsers.add_parser("balance", help="Query an address balance")
    source = balance.add_mutually_exclusive_group(required=True)
    source.add_argument("--keystore")
    source.add_argument("--address")
    rpc_option(balance)
    balance.set_defaults(handler=command_balance)

    build = subparsers.add_parser("build", help="Build an unsigned transaction online")
    source = build.add_mutually_exclusive_group(required=True)
    source.add_argument("--keystore")
    source.add_argument("--from-address")
    transaction_options(build)
    build.add_argument("--output", default="unsigned_transaction.json")
    rpc_option(build)
    build.set_defaults(handler=command_build)

    inspect = subparsers.add_parser("inspect", help="Display an unsigned transaction")
    inspect.add_argument("--unsigned", required=True)
    inspect.set_defaults(handler=command_inspect)

    sign = subparsers.add_parser("sign", help="Sign a transaction offline")
    sign.add_argument("--keystore", required=True)
    sign.add_argument("--unsigned", required=True)
    sign.add_argument("--output", default="signed_transaction.json")
    sign.add_argument("--yes", action="store_true", help="Skip transaction confirmation")
    sign.set_defaults(handler=command_sign)

    c_address = subparsers.add_parser("c-address", help="Show compiled C signer address")
    c_address.add_argument("--signer", required=True)
    c_address.set_defaults(handler=command_c_address)

    sign_c = subparsers.add_parser("sign-c", help="Sign using the separate C signer")
    sign_c.add_argument("--signer", required=True)
    sign_c.add_argument("--unsigned", required=True)
    sign_c.add_argument("--output", default="signed_transaction.json")
    sign_c.add_argument("--yes", action="store_true")
    sign_c.set_defaults(handler=command_sign_c)

    broadcast = subparsers.add_parser(
        "broadcast", help="Broadcast a signed raw transaction online"
    )
    broadcast.add_argument("--signed", required=True)
    rpc_option(broadcast)
    broadcast.set_defaults(handler=command_broadcast)

    send = subparsers.add_parser(
        "send", help="Build, sign and broadcast in one online process (demo only)"
    )
    send.add_argument("--keystore", required=True)
    transaction_options(send)
    send.add_argument("--yes", action="store_true", help="Skip transaction confirmation")
    rpc_option(send)
    send.set_defaults(handler=command_send)
    return parser


def main() -> int:
    try:
        args = build_parser().parse_args()
        args.handler(args)
        return 0
    except (WalletError, ValueError) as exc:
        print(f"Error: {exc}", file=sys.stderr)
        return 1
    except KeyboardInterrupt:
        print("\nCancelled", file=sys.stderr)
        return 130


if __name__ == "__main__":
    raise SystemExit(main())
