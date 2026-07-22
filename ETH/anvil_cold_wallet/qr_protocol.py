from __future__ import annotations

import json
from typing import Any

QR_REQUEST_FORMAT = "anvil-cold-wallet-qr-request-v1"
QR_RESPONSE_FORMAT = "anvil-cold-wallet-qr-response-v1"


class QrProtocolError(ValueError):
    pass


def _reject_duplicate_keys(pairs: list[tuple[str, Any]]) -> dict[str, Any]:
    result: dict[str, Any] = {}
    for key, value in pairs:
        if key in result:
            raise QrProtocolError(f"duplicate QR field: {key}")
        result[key] = value
    return result


def _parse_json(text: str) -> dict[str, Any]:
    try:
        value = json.loads(text, object_pairs_hook=_reject_duplicate_keys)
    except json.JSONDecodeError as exc:
        raise QrProtocolError(f"invalid QR JSON: {exc}") from exc
    if not isinstance(value, dict):
        raise QrProtocolError("QR payload must be a JSON object")
    return value


def encode_request(unsigned: dict[str, Any], request_id: int) -> str:
    if not 0 <= request_id <= 0xFFFFFFFF:
        raise QrProtocolError("requestId must fit in 32 bits")
    if unsigned.get("format") != "anvil-cold-wallet-unsigned-v1":
        raise QrProtocolError("unsupported unsigned transaction format")
    envelope = {
        "format": QR_REQUEST_FORMAT,
        "requestId": request_id,
        "unsigned": unsigned,
    }
    return json.dumps(envelope, sort_keys=True, separators=(",", ":"))


def decode_request(text: str) -> tuple[int, dict[str, Any]]:
    envelope = _parse_json(text)
    if envelope.get("format") != QR_REQUEST_FORMAT:
        raise QrProtocolError("unsupported QR request format")
    request_id = envelope.get("requestId")
    unsigned = envelope.get("unsigned")
    if not isinstance(request_id, int) or not 0 <= request_id <= 0xFFFFFFFF:
        raise QrProtocolError("invalid QR requestId")
    if not isinstance(unsigned, dict):
        raise QrProtocolError("QR request has no unsigned transaction")
    if unsigned.get("format") != "anvil-cold-wallet-unsigned-v1":
        raise QrProtocolError("unsupported unsigned transaction format")
    return request_id, unsigned


def encode_response(request_id: int, signed: dict[str, Any]) -> str:
    envelope = {
        "format": QR_RESPONSE_FORMAT,
        "requestId": request_id,
        "signed": signed,
    }
    return json.dumps(envelope, sort_keys=True, separators=(",", ":"))


def decode_response(text: str) -> tuple[int, dict[str, Any]]:
    envelope = _parse_json(text)
    if envelope.get("format") != QR_RESPONSE_FORMAT:
        raise QrProtocolError("unsupported QR response format")
    request_id = envelope.get("requestId")
    signed = envelope.get("signed")
    if not isinstance(request_id, int) or not 0 <= request_id <= 0xFFFFFFFF:
        raise QrProtocolError("invalid QR requestId")
    if not isinstance(signed, dict):
        raise QrProtocolError("QR response has no signed transaction")
    return request_id, signed
