from __future__ import annotations

import binascii
import json
import unittest

from eth_account import Account
from eth_account._utils.legacy_transactions import serializable_unsigned_transaction_from_dict

from fpga_uart import (
    COMMAND_PING,
    PROTOCOL_VERSION,
    FpgaUartError,
    build_request,
    crc16_ccitt,
    parse_response,
)
from fpga_wallet import sign_payload_fpga
from qr_protocol import QrProtocolError, decode_request, decode_response, encode_request, encode_response


TEST_KEY = bytes.fromhex(
    "cf441a9aa8fa75a2822ab42be53155f6c99a95dfba39bb864348fdaea7f4ce88"
)


def unsigned_payload() -> dict:
    transaction = {
        "type": 2,
        "chainId": 31338,
        "nonce": 0,
        "maxPriorityFeePerGas": 1_000_000_000,
        "maxFeePerGas": 2_000_000_000,
        "gas": 21000,
        "to": "0x1111111111111111111111111111111111111111",
        "value": 100_000_000_000_000_000,
        "data": "0x",
    }
    return {
        "format": "anvil-cold-wallet-unsigned-v1",
        "from": Account.from_key(TEST_KEY).address,
        "transaction": transaction,
        "display": {},
    }


class FakeHashSigner:
    def __init__(self, transaction: dict):
        self.expected = Account.sign_transaction(transaction, TEST_KEY)
        serializable = serializable_unsigned_transaction_from_dict(
            {**transaction, "accessList": []}
        )
        self.message_hash = bytes(serializable.hash())

    def sign_hash(self, message_hash: bytes) -> dict[str, int]:
        if message_hash != self.message_hash:
            raise AssertionError("message hash mismatch")
        return {
            "yParity": self.expected.v,
            "r": self.expected.r,
            "s": self.expected.s,
        }


class FpgaUartProtocolTest(unittest.TestCase):
    def test_crc_matches_standard_library(self) -> None:
        data = bytes.fromhex("0111000000")
        self.assertEqual(crc16_ccitt(data), binascii.crc_hqx(data, 0xFFFF))

    def test_request_and_response_frames(self) -> None:
        request = build_request(0x11, COMMAND_PING)
        self.assertEqual(request[:2], b"\xa5\x5a")
        self.assertEqual(request[2:7], bytes((PROTOCOL_VERSION, 0x11, 0, 0, 0)))
        body = bytes((PROTOCOL_VERSION, 0x11, COMMAND_PING, 0, 0, 4)) + b"PONG"
        frame = b"\x5a\xa5" + body + crc16_ccitt(body).to_bytes(2, "big")
        response = parse_response(frame)
        self.assertEqual(response.payload, b"PONG")
        with self.assertRaises(FpgaUartError):
            parse_response(frame[:-1] + bytes((frame[-1] ^ 1,)))

    def test_qr_fpga_pipeline_matches_eth_account(self) -> None:
        unsigned = unsigned_payload()
        request_text = encode_request(unsigned, 123)
        request_id, decoded_unsigned = decode_request(request_text)
        self.assertEqual(request_id, 123)
        signer = FakeHashSigner(decoded_unsigned["transaction"])
        signed = sign_payload_fpga(decoded_unsigned, signer)
        expected = Account.sign_transaction(decoded_unsigned["transaction"], TEST_KEY)
        self.assertEqual(signed["rawTransaction"], "0x" + bytes(expected.raw_transaction).hex())
        response_text = encode_response(request_id, signed)
        decoded_id, decoded_signed = decode_response(response_text)
        self.assertEqual(decoded_id, request_id)
        self.assertEqual(decoded_signed["transactionHash"], signed["transactionHash"])

    def test_qr_rejects_duplicate_fields(self) -> None:
        duplicate = json.dumps({"format": "ignored"})[:-1] + ',"format":"again"}'
        with self.assertRaises(QrProtocolError):
            decode_request(duplicate)


if __name__ == "__main__":
    unittest.main()
