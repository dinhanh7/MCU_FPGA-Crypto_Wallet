from __future__ import annotations

import os
import json
import subprocess
import tempfile
import unittest
from decimal import Decimal
from pathlib import Path
from unittest.mock import patch

from eth_account import Account
from eth_keys import keys
from web3 import Web3

from app import create_app
from wallet import WalletError, validate_rpc_url


TEST_FPGA_KEY = bytes.fromhex(
    "cf441a9aa8fa75a2822ab42be53155f6c99a95dfba39bb864348fdaea7f4ce88"
)


class FakeFpgaUartSigner:
    def __init__(self, path: str, timeout: float = 3.0):
        self.path = path
        self.timeout = timeout
        self.private_key = keys.PrivateKey(TEST_FPGA_KEY)

    def __enter__(self):
        return self

    def __exit__(self, exc_type, exc_value, traceback):
        return None

    def ping(self) -> None:
        return None

    def sign_hash(self, message_hash: bytes) -> dict[str, int]:
        signature = self.private_key.sign_msg_hash(message_hash)
        return {
            "yParity": signature.v,
            "r": signature.r,
            "s": signature.s,
        }


class WebInterfaceTest(unittest.TestCase):
    def setUp(self) -> None:
        self.temporary = tempfile.TemporaryDirectory()
        self.app = create_app(Path(self.temporary.name))
        self.app.config["TESTING"] = True
        self.client = self.app.test_client()

    def tearDown(self) -> None:
        self.temporary.cleanup()

    def test_index_has_security_headers(self) -> None:
        response = self.client.get("/")
        self.assertEqual(response.status_code, 200)
        self.assertIn(b"Anvil Cold Wallet", response.data)
        self.assertIn(b"Gowin FPGA", response.data)
        self.assertEqual(response.headers["Cache-Control"], "no-store, max-age=0")
        self.assertEqual(response.headers["X-Frame-Options"], "DENY")
        self.assertIn("default-src 'self'", response.headers["Content-Security-Policy"])

    def test_accepts_local_and_external_anvil_rpc_urls(self) -> None:
        self.assertEqual(
            validate_rpc_url("http://127.0.0.1:8545"),
            "http://127.0.0.1:8545",
        )
        self.assertEqual(
            validate_rpc_url("http://192.168.1.100:8545"),
            "http://192.168.1.100:8545",
        )
        self.assertEqual(
            validate_rpc_url("https://anvil.example/rpc/project-key"),
            "https://anvil.example/rpc/project-key",
        )

    def test_rejects_unsafe_anvil_rpc_urls(self) -> None:
        for value in (
            "file:///etc/passwd",
            "ws://anvil.example:8545",
            "http://user:password@anvil.example:8545",
            "http://anvil.example:8545/#fragment",
            "not-a-url",
        ):
            with self.subTest(value=value), self.assertRaises(WalletError):
                validate_rpc_url(value)

    def test_fpga_status_and_sign_flow(self) -> None:
        self.app.config["FPGA_UART_PORT"] = "/dev/fake-fpga"
        sender = Account.from_key(TEST_FPGA_KEY).address
        unsigned = {
            "format": "anvil-cold-wallet-unsigned-v1",
            "from": sender,
            "transaction": {
                "type": 2,
                "chainId": 31338,
                "nonce": 0,
                "maxPriorityFeePerGas": 1_000_000_000,
                "maxFeePerGas": 2_000_000_000,
                "gas": 21000,
                "to": Account.create().address,
                "value": 123456789,
                "data": "0x",
            },
            "display": {},
        }

        expected = Account.sign_transaction(unsigned["transaction"], TEST_FPGA_KEY)
        raw_transaction = "0x" + bytes(expected.raw_transaction).hex()
        signer_output = {
            "format": "dual-mcu-eip1559-v1",
            "from": sender,
            "freezeId": "0x" + "11" * 32,
            "messageHash": "0x" + "22" * 32,
            "yParity": expected.v,
            "r": f"0x{expected.r:064x}",
            "s": f"0x{expected.s:064x}",
            "rawTransaction": raw_transaction,
            "transactionHash": "0x" + bytes(expected.hash).hex(),
            "signatureVerified": True,
        }
        process_results = [
            subprocess.CompletedProcess([], 0, stdout=sender + "\n", stderr=""),
            subprocess.CompletedProcess(
                [], 0, stdout=json.dumps(signer_output), stderr=""
            ),
        ]

        with patch("app.FpgaUartSigner", FakeFpgaUartSigner), patch(
            "app.subprocess.run", side_effect=process_results
        ) as run_process:
            signer_info = self.client.get("/api/fpga-signer")
            self.assertEqual(signer_info.status_code, 200)
            info = signer_info.get_json()
            self.assertTrue(info["available"])
            self.assertEqual(info["address"], sender)
            self.assertEqual(info["baudRate"], 115200)

            signed_response = self.client.post(
                "/api/sign-fpga",
                json={"unsigned": unsigned, "passkey": "123456"},
            )

        self.assertEqual(signed_response.status_code, 200)
        signed = signed_response.get_json()["signed"]
        self.assertEqual(signed["from"], sender)
        self.assertEqual(
            signed["rawTransaction"], "0x" + bytes(expected.raw_transaction).hex()
        )
        self.assertEqual(
            signed["signature"]["implementation"],
            "Sonix MCU model + Gowin ACG525 dual FPGA",
        )
        self.assertTrue(signed["signature"]["verifiedByMcu"])
        self.assertEqual(run_process.call_count, 2)
        self.assertEqual(run_process.call_args.kwargs["input"], "123456\n")

    def test_fpga_sign_requires_mcu_passkey(self) -> None:
        response = self.client.post(
            "/api/sign-fpga",
            json={
                "unsigned": {
                    "format": "anvil-cold-wallet-unsigned-v1",
                    "transaction": {"type": 2},
                }
            },
        )
        self.assertEqual(response.status_code, 400)
        self.assertIn("passkey", response.get_json()["error"])

    def test_rejects_unsafe_wallet_name(self) -> None:
        response = self.client.post(
            "/api/wallets/create",
            json={"name": "../escape", "password": "test-password"},
        )
        self.assertEqual(response.status_code, 400)
        self.assertFalse(response.get_json()["ok"])

    def test_optional_basic_auth(self) -> None:
        self.app.config["WEB_PASSWORD"] = "public-test-password"
        unauthorized = self.client.get("/")
        self.assertEqual(unauthorized.status_code, 401)
        self.assertIn("Basic", unauthorized.headers["WWW-Authenticate"])

        authorized = self.client.get(
            "/", auth=(self.app.config["WEB_USERNAME"], "public-test-password")
        )
        self.assertEqual(authorized.status_code, 200)

    @unittest.skipUnless(os.environ.get("ANVIL_RPC_URL"), "ANVIL_RPC_URL not set")
    def test_full_build_sign_broadcast_flow(self) -> None:
        rpc_url = os.environ["ANVIL_RPC_URL"]
        password = "integration-test-password"

        created = self.client.post(
            "/api/wallets/create",
            json={"name": "integration", "password": password},
        ).get_json()
        self.assertTrue(created["ok"])
        sender = created["address"]

        web3 = Web3(Web3.HTTPProvider(rpc_url, request_kwargs={"timeout": 15}))
        self.assertTrue(web3.is_connected())
        funding = web3.provider.make_request(
            "anvil_setBalance", [sender, hex(web3.to_wei(2, "ether"))]
        )
        self.assertIsNone(funding.get("error"))

        self_transfer = self.client.post(
            "/api/build",
            json={
                "wallet": "integration",
                "to": sender,
                "amount": "1",
                "rpcUrl": rpc_url,
            },
        )
        self.assertEqual(self_transfer.status_code, 400)
        self.assertIn("trùng với ví gửi", self_transfer.get_json()["error"])

        recipient = Account.create().address
        unsigned_response = self.client.post(
            "/api/build",
            json={
                "wallet": "integration",
                "to": recipient,
                "amount": "0.006789",
                "rpcUrl": rpc_url,
            },
        )
        self.assertEqual(unsigned_response.status_code, 200)
        unsigned = unsigned_response.get_json()["unsigned"]
        self.assertEqual(unsigned["transaction"]["chainId"], web3.eth.chain_id)
        self.assertEqual(unsigned["transaction"]["data"], "0x")

        signed_response = self.client.post(
            "/api/sign",
            json={
                "wallet": "integration",
                "password": password,
                "unsigned": unsigned,
            },
        )
        self.assertEqual(signed_response.status_code, 200)
        signed = signed_response.get_json()["signed"]
        self.assertEqual(signed["from"], sender)

        broadcast_response = self.client.post(
            "/api/broadcast", json={"signed": signed, "rpcUrl": rpc_url}
        )
        self.assertEqual(broadcast_response.status_code, 200)
        receipt = broadcast_response.get_json()["receipt"]
        self.assertEqual(receipt["status"], 1)
        self.assertEqual(web3.eth.get_balance(recipient), web3.to_wei(Decimal("0.006789"), "ether"))

        repeated_response = self.client.post(
            "/api/broadcast", json={"signed": signed, "rpcUrl": rpc_url}
        )
        self.assertEqual(repeated_response.status_code, 200)
        repeated_receipt = repeated_response.get_json()["receipt"]
        self.assertEqual(repeated_receipt["transactionHash"], receipt["transactionHash"])
        self.assertTrue(repeated_receipt["alreadyMined"])

    @unittest.skipUnless(os.environ.get("ANVIL_RPC_URL"), "ANVIL_RPC_URL not set")
    def test_c_signer_build_sign_broadcast_flow(self) -> None:
        if not self.app.config["C_SIGNER_BIN"].is_file():
            self.skipTest("C signer binary not built")
        rpc_url = os.environ["ANVIL_RPC_URL"]
        signer_info = self.client.get("/api/c-signer").get_json()
        self.assertTrue(signer_info["available"])
        sender = signer_info["address"]

        web3 = Web3(Web3.HTTPProvider(rpc_url, request_kwargs={"timeout": 15}))
        funding = web3.provider.make_request(
            "anvil_setBalance", [sender, hex(web3.to_wei(2, "ether"))]
        )
        self.assertIsNone(funding.get("error"))
        recipient = Account.create().address

        unsigned_response = self.client.post(
            "/api/build",
            json={
                "wallet": "__c_signer__",
                "to": recipient,
                "amount": "0.004321",
                "rpcUrl": rpc_url,
            },
        )
        self.assertEqual(unsigned_response.status_code, 200)
        unsigned = unsigned_response.get_json()["unsigned"]

        signed_response = self.client.post(
            "/api/sign-c", json={"unsigned": unsigned}
        )
        self.assertEqual(signed_response.status_code, 200)
        signed = signed_response.get_json()["signed"]
        self.assertEqual(signed["from"], sender)
        self.assertEqual(signed["signature"]["implementation"], "C/libsecp256k1")
        self.assertEqual(Account.recover_transaction(signed["rawTransaction"]), sender)

        broadcast_response = self.client.post(
            "/api/broadcast", json={"signed": signed, "rpcUrl": rpc_url}
        )
        self.assertEqual(broadcast_response.status_code, 200)
        receipt = broadcast_response.get_json()["receipt"]
        self.assertEqual(receipt["status"], 1)
        self.assertEqual(
            web3.eth.get_balance(recipient), web3.to_wei(Decimal("0.004321"), "ether")
        )


if __name__ == "__main__":
    unittest.main()
