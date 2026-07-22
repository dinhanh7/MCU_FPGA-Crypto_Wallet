#!/usr/bin/env python3
"""Local-only HTML interface for the Anvil cold-wallet prototype."""

from __future__ import annotations

import argparse
import json
import os
import re
import secrets
import subprocess
import threading
from pathlib import Path
from typing import Any

from eth_account import Account
from eth_utils import to_checksum_address
from flask import Flask, Response, jsonify, render_template, request
from web3.exceptions import Web3Exception

from fpga_uart import FpgaUartError, FpgaUartSigner
from wallet import (
    DEFAULT_RPC_URL,
    WalletError,
    broadcast_payload,
    build_transaction,
    c_signer_address,
    connect,
    keystore_address,
    sign_payload,
    sign_payload_c,
    write_json,
)


PROJECT_DIR = Path(__file__).resolve().parent
WALLET_NAME = re.compile(r"^[A-Za-z0-9_-]{1,50}$")
FPGA_SIGNER_NAME = "__fpga_signer__"
def create_app(wallet_dir: Path | None = None) -> Flask:
    app = Flask(__name__)
    app.config["WALLET_DIR"] = (wallet_dir or PROJECT_DIR / "wallets").resolve()
    app.config["WEB_USERNAME"] = os.environ.get("WALLET_WEB_USERNAME", "wallet")
    app.config["WEB_PASSWORD"] = os.environ.get("WALLET_WEB_PASSWORD", "")
    app.config["C_SIGNER_BIN"] = Path(
        os.environ.get(
            "C_SIGNER_BIN", str(PROJECT_DIR / "c_signer" / "build" / "eth_signer")
        )
    ).resolve()
    app.config["DUAL_MCU_BIN"] = Path(
        os.environ.get(
            "DUAL_MCU_BIN", str(PROJECT_DIR.parents[1] / "mcu/bin/dual-mcu")
        )
    ).resolve()
    app.config["FPGA_PUBLIC_KEY"] = Path(
        os.environ.get(
            "FPGA_PUBLIC_KEY",
            str(PROJECT_DIR.parents[1] / "mcu/test-data/fpga_test_d1.pub"),
        )
    ).resolve()
    app.config["MCU_PASSKEY_RECORD"] = Path(
        os.environ.get(
            "MCU_PASSKEY_RECORD",
            str(PROJECT_DIR.parents[1] / "mcu/test-data/test.passkey"),
        )
    ).resolve()
    app.config["FPGA_UART_PORT"] = os.environ.get("FPGA_UART_PORT", "/dev/ttyACM0")
    app.config["FPGA_UART_TIMEOUT"] = float(os.environ.get("FPGA_UART_TIMEOUT", "3"))
    app.config["FPGA_UART_LOCK"] = threading.Lock()
    app.config["FPGA_SIGNER_ADDRESS"] = None
    app.config["WALLET_DIR"].mkdir(parents=True, exist_ok=True)

    @app.before_request
    def require_basic_auth():
        expected_password = app.config["WEB_PASSWORD"]
        if not expected_password:
            return None
        authorization = request.authorization
        valid = (
            authorization is not None
            and secrets.compare_digest(
                authorization.username or "", app.config["WEB_USERNAME"]
            )
            and secrets.compare_digest(
                authorization.password or "", expected_password
            )
        )
        if valid:
            return None
        return Response(
            "Authentication required",
            401,
            {"WWW-Authenticate": 'Basic realm="Anvil Cold Wallet"'},
        )

    @app.after_request
    def security_headers(response):
        response.headers["Cache-Control"] = "no-store, max-age=0"
        response.headers["Pragma"] = "no-cache"
        response.headers["X-Content-Type-Options"] = "nosniff"
        response.headers["X-Frame-Options"] = "DENY"
        response.headers["Referrer-Policy"] = "no-referrer"
        response.headers["Content-Security-Policy"] = (
            "default-src 'self'; script-src 'self'; style-src 'self'; "
            "img-src 'self' data:; connect-src 'self'; frame-ancestors 'none'"
        )
        return response

    @app.errorhandler(WalletError)
    @app.errorhandler(FpgaUartError)
    @app.errorhandler(ValueError)
    @app.errorhandler(KeyError)
    @app.errorhandler(Web3Exception)
    def expected_error(error):
        return jsonify({"ok": False, "error": str(error)}), 400

    @app.get("/")
    def index():
        return render_template("index.html", default_rpc_url=DEFAULT_RPC_URL)

    @app.post("/api/status")
    def api_status():
        body = json_body()
        web3 = connect(required_text(body, "rpcUrl"))
        return success(
            chainId=web3.eth.chain_id,
            blockNumber=web3.eth.block_number,
            clientVersion=web3.client_version,
        )

    @app.get("/api/wallets")
    def api_wallets():
        wallets = []
        for path in sorted(app.config["WALLET_DIR"].glob("*.json")):
            try:
                wallets.append({"name": path.stem, "address": keystore_address(path)})
            except WalletError:
                continue
        return success(wallets=wallets)

    @app.get("/api/c-signer")
    def api_c_signer():
        signer = app.config["C_SIGNER_BIN"]
        if not signer.is_file():
            return success(available=False, path=str(signer))
        return success(
            available=True,
            name="C offline signer",
            address=c_signer_address(signer),
            path=str(signer),
        )

    @app.get("/api/fpga-signer")
    def api_fpga_signer():
        port = app.config["FPGA_UART_PORT"]
        try:
            address = probe_fpga_signer(app)
        except (FpgaUartError, OSError, ValueError) as exc:
            app.config["FPGA_SIGNER_ADDRESS"] = None
            return success(
                available=False,
                name="Gowin ACG525 FPGA",
                port=port,
                baudRate=115200,
                error=str(exc),
            )
        return success(
            available=True,
            name="Gowin ACG525 FPGA",
            address=address,
            port=port,
            baudRate=115200,
        )

    @app.post("/api/wallets/create")
    def api_create_wallet():
        body = json_body()
        path = wallet_path(app, required_text(body, "name"), must_exist=False)
        refuse_overwrite(path)
        password = required_text(body, "password")
        account = Account.create()
        write_json(path, Account.encrypt(account.key, password), private=True)
        return success(name=path.stem, address=account.address)

    @app.post("/api/wallets/import")
    def api_import_wallet():
        body = json_body()
        path = wallet_path(app, required_text(body, "name"), must_exist=False)
        refuse_overwrite(path)
        password = required_text(body, "password")
        private_key = required_text(body, "privateKey")
        try:
            account = Account.from_key(private_key)
        except ValueError as exc:
            raise WalletError("Private key không hợp lệ") from exc
        write_json(path, Account.encrypt(account.key, password), private=True)
        return success(name=path.stem, address=account.address)

    @app.post("/api/balance")
    def api_balance():
        body = json_body()
        address = resolve_address(app, body)
        web3 = connect(required_text(body, "rpcUrl"))
        balance_wei = web3.eth.get_balance(address)
        return success(
            address=address,
            balanceWei=str(balance_wei),
            balanceEth=str(web3.from_wei(balance_wei, "ether")),
            chainId=web3.eth.chain_id,
        )

    @app.post("/api/build")
    def api_build():
        body = json_body()
        sender = resolve_address(app, body)
        web3 = connect(required_text(body, "rpcUrl"))
        unsigned = build_transaction(
            web3,
            sender,
            required_text(body, "to"),
            required_text(body, "amount"),
        )
        return success(unsigned=unsigned)

    @app.post("/api/sign")
    def api_sign():
        body = json_body()
        unsigned = body.get("unsigned")
        if not isinstance(unsigned, dict):
            raise WalletError("Thiếu unsigned transaction")
        path = wallet_path(app, required_text(body, "wallet"), must_exist=True)
        signed = sign_payload(unsigned, path, required_text(body, "password"))
        return success(signed=signed)

    @app.post("/api/sign-c")
    def api_sign_c():
        body = json_body()
        unsigned = body.get("unsigned")
        if not isinstance(unsigned, dict):
            raise WalletError("Thiếu unsigned transaction")
        signed = sign_payload_c(
            unsigned, app.config["C_SIGNER_BIN"], assume_yes=True
        )
        return success(signed=signed)

    @app.post("/api/sign-fpga")
    def api_sign_fpga():
        body = json_body()
        unsigned = body.get("unsigned")
        if not isinstance(unsigned, dict):
            raise WalletError("Thiếu unsigned transaction")
        try:
            signed = sign_with_fpga(app, unsigned)
        except (FpgaUartError, OSError) as exc:
            raise WalletError(
                f"Không giao tiếp được FPGA tại {app.config['FPGA_UART_PORT']}: {exc}"
            ) from exc
        return success(signed=signed)

    @app.post("/api/broadcast")
    def api_broadcast():
        body = json_body()
        signed = body.get("signed")
        if not isinstance(signed, dict):
            raise WalletError("Thiếu signed transaction")
        web3 = connect(required_text(body, "rpcUrl"))
        receipt = broadcast_payload(web3, signed)
        balances = transaction_balances(web3, signed)
        return success(receipt=receipt, balances=balances)

    return app


def success(**values: Any):
    return jsonify({"ok": True, **values})


def json_body() -> dict[str, Any]:
    body = request.get_json(silent=True)
    if not isinstance(body, dict):
        raise WalletError("Request phải là JSON object")
    return body


def required_text(body: dict[str, Any], field: str) -> str:
    value = body.get(field)
    if not isinstance(value, str) or not value.strip():
        raise WalletError(f"Thiếu trường: {field}")
    return value.strip()


def wallet_path(
    app: Flask, name: str, *, must_exist: bool
) -> Path:
    normalized = name.removesuffix(".json")
    if not WALLET_NAME.fullmatch(normalized):
        raise WalletError("Tên ví chỉ được chứa chữ, số, dấu gạch ngang và gạch dưới")
    path = app.config["WALLET_DIR"] / f"{normalized}.json"
    if must_exist and not path.is_file():
        raise WalletError(f"Không tìm thấy ví: {normalized}")
    return path


def refuse_overwrite(path: Path) -> None:
    if path.exists():
        raise WalletError(f"Ví đã tồn tại: {path.stem}")


def resolve_address(app: Flask, body: dict[str, Any]) -> str:
    wallet = body.get("wallet")
    address = body.get("address")
    if isinstance(wallet, str) and wallet.strip():
        if wallet.strip() == "__c_signer__":
            return c_signer_address(app.config["C_SIGNER_BIN"])
        if wallet.strip() == FPGA_SIGNER_NAME:
            return cached_fpga_signer_address(app)
        return keystore_address(wallet_path(app, wallet.strip(), must_exist=True))
    if isinstance(address, str) and address.strip():
        return to_checksum_address(address.strip())
    raise WalletError("Cần chọn ví hoặc nhập địa chỉ")


def probe_fpga_signer(app: Flask) -> str:
    public_key = app.config["FPGA_PUBLIC_KEY"]
    with app.config["FPGA_UART_LOCK"]:
        with FpgaUartSigner(
            app.config["FPGA_UART_PORT"],
            timeout=app.config["FPGA_UART_TIMEOUT"],
        ) as signer:
            signer.ping()
        try:
            result = subprocess.run(
                [str(app.config["DUAL_MCU_BIN"]), "eth", "address", str(public_key)],
                check=True,
                capture_output=True,
                text=True,
                timeout=10,
            )
            address = to_checksum_address(result.stdout.strip())
        except (subprocess.CalledProcessError, subprocess.TimeoutExpired, OSError, ValueError) as exc:
            raise WalletError(f"Không đọc được danh tính public của dual MCU: {exc}") from exc
    app.config["FPGA_SIGNER_ADDRESS"] = address
    return address


def cached_fpga_signer_address(app: Flask) -> str:
    address = app.config.get("FPGA_SIGNER_ADDRESS")
    if isinstance(address, str) and address:
        return address
    try:
        return probe_fpga_signer(app)
    except (FpgaUartError, OSError, ValueError) as exc:
        raise WalletError(
            f"Không giao tiếp được FPGA tại {app.config['FPGA_UART_PORT']}: {exc}"
        ) from exc


def sign_with_fpga(app: Flask, unsigned: dict[str, Any]) -> dict[str, Any]:
    if unsigned.get("format") != "anvil-cold-wallet-unsigned-v1":
        raise WalletError("Unsupported unsigned transaction format")
    transaction = unsigned.get("transaction")
    if not isinstance(transaction, dict) or transaction.get("type") != 2:
        raise WalletError("Dual MCU chỉ hỗ trợ giao dịch EIP-1559 type 2")
    passkey = required_text(json_body(), "passkey")
    if not re.fullmatch(r"[0-9]{6,12}", passkey):
        raise WalletError("Mã mở khóa MCU phải gồm 6-12 chữ số")
    arguments = [
        str(app.config["DUAL_MCU_BIN"]), "eth", "sign-fpga",
        str(app.config["FPGA_PUBLIC_KEY"]),
        str(app.config["MCU_PASSKEY_RECORD"]),
        str(app.config["FPGA_UART_PORT"]),
        "--chain-id", str(transaction["chainId"]),
        "--nonce", str(transaction["nonce"]),
        "--max-priority-fee-per-gas", str(transaction["maxPriorityFeePerGas"]),
        "--max-fee-per-gas", str(transaction["maxFeePerGas"]),
        "--gas-limit", str(transaction["gas"]),
        "--to", str(transaction["to"]),
        "--value", str(transaction["value"]),
        "--data", str(transaction.get("data", "0x")),
        "--yes",
    ]
    with app.config["FPGA_UART_LOCK"]:
        try:
            result = subprocess.run(
                arguments,
                input=passkey + "\n",
                check=True,
                capture_output=True,
                text=True,
                timeout=120,
            )
        except subprocess.CalledProcessError as exc:
            raise WalletError((exc.stderr or "Dual MCU từ chối giao dịch").strip()) from exc
        except (subprocess.TimeoutExpired, OSError) as exc:
            raise WalletError(f"Không chạy được dual MCU: {exc}") from exc
    try:
        output = json.loads(result.stdout)
        raw_transaction = str(output["rawTransaction"])
        recovered = to_checksum_address(Account.recover_transaction(raw_transaction))
        expected = to_checksum_address(str(unsigned.get("from", "")))
        if recovered != expected or not output.get("signatureVerified"):
            raise WalletError(f"Chữ ký recover {recovered}, cần {expected}")
    except (json.JSONDecodeError, KeyError, TypeError, ValueError) as exc:
        raise WalletError("Dual MCU trả dữ liệu chữ ký không hợp lệ") from exc
    signed = {
        "format": "anvil-cold-wallet-signed-v1",
        "from": recovered,
        "transactionHash": output["transactionHash"],
        "rawTransaction": raw_transaction,
        "unsigned": unsigned,
        "signature": {
            "implementation": "Sonix MCU model + Gowin ACG525 dual FPGA",
            "freezeId": output["freezeId"],
            "messageHash": output["messageHash"],
            "yParity": output["yParity"],
            "r": output["r"],
            "s": output["s"],
            "verifiedByMcu": True,
        },
    }
    app.config["FPGA_SIGNER_ADDRESS"] = recovered
    return signed


def transaction_balances(web3, signed: dict[str, Any]) -> dict[str, Any]:
    unsigned = signed.get("unsigned")
    if not isinstance(unsigned, dict):
        return {}
    transaction = unsigned.get("transaction")
    sender = signed.get("from")
    if not isinstance(transaction, dict) or not isinstance(sender, str):
        return {}
    recipient = transaction.get("to")
    if not isinstance(recipient, str):
        return {}
    sender = to_checksum_address(sender)
    recipient = to_checksum_address(recipient)
    sender_balance = web3.eth.get_balance(sender)
    recipient_balance = web3.eth.get_balance(recipient)
    return {
        "sender": {
            "address": sender,
            "balanceEth": str(web3.from_wei(sender_balance, "ether")),
        },
        "recipient": {
            "address": recipient,
            "balanceEth": str(web3.from_wei(recipient_balance, "ether")),
        },
    }


def main() -> int:
    parser = argparse.ArgumentParser(description="Local HTML cold-wallet interface")
    parser.add_argument("--host", default="127.0.0.1")
    parser.add_argument("--port", type=int, default=5000)
    args = parser.parse_args()
    if args.host not in {"127.0.0.1", "localhost", "::1"}:
        raise SystemExit("Refusing non-local host. Use 127.0.0.1 for wallet safety.")
    create_app().run(host=args.host, port=args.port, debug=False)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
