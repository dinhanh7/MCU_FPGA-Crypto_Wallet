from __future__ import annotations

import os
import select
import termios
import time
from dataclasses import dataclass

REQUEST_SYNC = b"\xa5\x5a"
RESPONSE_SYNC = b"\x5a\xa5"
PROTOCOL_VERSION = 1

COMMAND_PING = 0x00
COMMAND_SIGN_HASH = 0x01

STATUS_OK = 0x00
STATUS_CRC_ERROR = 0x01
STATUS_LENGTH_ERROR = 0x02
STATUS_COMMAND_ERROR = 0x03
STATUS_BUSY = 0x04
STATUS_PRIVATE_KEY_ERROR = 0x05
STATUS_RECOVERY_ERROR = 0x06

STATUS_NAMES = {
    STATUS_OK: "ok",
    STATUS_CRC_ERROR: "crc-error",
    STATUS_LENGTH_ERROR: "length-error",
    STATUS_COMMAND_ERROR: "command-error",
    STATUS_BUSY: "busy",
    STATUS_PRIVATE_KEY_ERROR: "private-key-error",
    STATUS_RECOVERY_ERROR: "recovery-error",
}


class FpgaUartError(RuntimeError):
    pass


@dataclass(frozen=True)
class FpgaResponse:
    sequence: int
    command: int
    status: int
    payload: bytes


def crc16_ccitt(data: bytes, initial: int = 0xFFFF) -> int:
    crc_value = initial
    for byte_value in data:
        crc_value ^= byte_value << 8
        for _ in range(8):
            if crc_value & 0x8000:
                crc_value = ((crc_value << 1) ^ 0x1021) & 0xFFFF
            else:
                crc_value = (crc_value << 1) & 0xFFFF
    return crc_value


def build_request(sequence: int, command: int, payload: bytes = b"") -> bytes:
    if not 0 <= sequence <= 0xFF:
        raise ValueError("sequence must fit in one byte")
    if not 0 <= command <= 0xFF:
        raise ValueError("command must fit in one byte")
    if len(payload) > 0xFFFF:
        raise ValueError("UART payload is too large")
    body = bytes((PROTOCOL_VERSION, sequence, command)) + len(payload).to_bytes(2, "big") + payload
    return REQUEST_SYNC + body + crc16_ccitt(body).to_bytes(2, "big")


def parse_response(frame: bytes) -> FpgaResponse:
    if len(frame) < 10:
        raise FpgaUartError("UART response is too short")
    if frame[:2] != RESPONSE_SYNC:
        raise FpgaUartError("UART response sync is invalid")
    version, sequence, command, status = frame[2:6]
    if version != PROTOCOL_VERSION:
        raise FpgaUartError(f"unsupported FPGA protocol version: {version}")
    payload_length = int.from_bytes(frame[6:8], "big")
    expected_length = 10 + payload_length
    if len(frame) != expected_length:
        raise FpgaUartError(
            f"UART response length mismatch: expected {expected_length}, got {len(frame)}"
        )
    expected_crc = int.from_bytes(frame[-2:], "big")
    actual_crc = crc16_ccitt(frame[2:-2])
    if expected_crc != actual_crc:
        raise FpgaUartError("UART response CRC is invalid")
    return FpgaResponse(sequence, command, status, frame[8:-2])


class PosixUart:
    def __init__(self, path: str, baud_rate: int = 115200):
        if baud_rate != 115200:
            raise ValueError("the current FPGA build only supports 115200 baud")
        self.path = path
        self.file_descriptor = os.open(path, os.O_RDWR | os.O_NOCTTY | os.O_NONBLOCK)
        attributes = termios.tcgetattr(self.file_descriptor)
        attributes[0] = 0
        attributes[1] = 0
        attributes[2] = termios.CS8 | termios.CLOCAL | termios.CREAD
        attributes[3] = 0
        attributes[4] = termios.B115200
        attributes[5] = termios.B115200
        attributes[6][termios.VMIN] = 0
        attributes[6][termios.VTIME] = 0
        termios.tcsetattr(self.file_descriptor, termios.TCSANOW, attributes)
        termios.tcflush(self.file_descriptor, termios.TCIOFLUSH)

    def close(self) -> None:
        if self.file_descriptor >= 0:
            os.close(self.file_descriptor)
            self.file_descriptor = -1

    def __enter__(self) -> PosixUart:
        return self

    def __exit__(self, exc_type, exc_value, traceback) -> None:
        self.close()

    def write(self, data: bytes) -> None:
        offset = 0
        while offset < len(data):
            _, writable, _ = select.select([], [self.file_descriptor], [], 1.0)
            if not writable:
                raise FpgaUartError("UART write timeout")
            offset += os.write(self.file_descriptor, data[offset:])

    def read_exact(self, length: int, deadline: float) -> bytes:
        received = bytearray()
        while len(received) < length:
            remaining = deadline - time.monotonic()
            if remaining <= 0:
                raise FpgaUartError("UART read timeout")
            readable, _, _ = select.select([self.file_descriptor], [], [], remaining)
            if not readable:
                raise FpgaUartError("UART read timeout")
            chunk = os.read(self.file_descriptor, length - len(received))
            if chunk:
                received.extend(chunk)
        return bytes(received)

    def read_response(self, timeout: float) -> FpgaResponse:
        deadline = time.monotonic() + timeout
        sync_index = 0
        while sync_index < len(RESPONSE_SYNC):
            byte_value = self.read_exact(1, deadline)[0]
            if byte_value == RESPONSE_SYNC[sync_index]:
                sync_index += 1
            else:
                sync_index = 1 if byte_value == RESPONSE_SYNC[0] else 0
        header = self.read_exact(6, deadline)
        payload_length = int.from_bytes(header[4:6], "big")
        tail = self.read_exact(payload_length + 2, deadline)
        return parse_response(RESPONSE_SYNC + header + tail)


class FpgaUartSigner:
    def __init__(self, path: str, timeout: float = 3.0):
        self.transport = PosixUart(path)
        self.timeout = timeout
        self.sequence = 0

    def close(self) -> None:
        self.transport.close()

    def __enter__(self) -> FpgaUartSigner:
        return self

    def __exit__(self, exc_type, exc_value, traceback) -> None:
        self.close()

    def next_sequence(self) -> int:
        self.sequence = (self.sequence + 1) & 0xFF
        return self.sequence

    def exchange(self, command: int, payload: bytes, timeout: float | None = None) -> FpgaResponse:
        sequence = self.next_sequence()
        self.transport.write(build_request(sequence, command, payload))
        response = self.transport.read_response(timeout or self.timeout)
        if response.sequence != sequence or response.command != command:
            raise FpgaUartError("UART response does not match the request")
        if response.status != STATUS_OK:
            status_name = STATUS_NAMES.get(response.status, f"unknown-{response.status}")
            raise FpgaUartError(f"FPGA rejected request: {status_name}")
        return response

    def ping(self) -> None:
        response = self.exchange(COMMAND_PING, b"", timeout=1.0)
        if response.payload != b"PONG":
            raise FpgaUartError("FPGA PING response payload is invalid")

    def sign_hash(self, message_hash: bytes) -> dict[str, int]:
        if len(message_hash) != 32:
            raise ValueError("message hash must contain exactly 32 bytes")
        response = self.exchange(COMMAND_SIGN_HASH, message_hash)
        if len(response.payload) != 65:
            raise FpgaUartError("FPGA signature response must contain 65 bytes")
        y_parity = response.payload[0]
        if y_parity not in (0, 1):
            raise FpgaUartError("FPGA returned an invalid yParity")
        return {
            "yParity": y_parity,
            "r": int.from_bytes(response.payload[1:33], "big"),
            "s": int.from_bytes(response.payload[33:65], "big"),
        }
