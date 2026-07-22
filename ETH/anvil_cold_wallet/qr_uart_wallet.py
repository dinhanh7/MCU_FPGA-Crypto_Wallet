from __future__ import annotations

import argparse
import sys
from pathlib import Path

from fpga_uart import FpgaUartError, FpgaUartSigner
from fpga_wallet import sign_payload_fpga
from qr_protocol import QrProtocolError, decode_request, encode_request, encode_response
from wallet import WalletError, read_json, show_unsigned


def read_text(path: str) -> str:
    if path == "-":
        return sys.stdin.read()
    return Path(path).read_text(encoding="utf-8")


def write_text(path: str, value: str) -> None:
    if path == "-":
        print(value)
        return
    Path(path).write_text(value + "\n", encoding="utf-8")


def command_encode(args: argparse.Namespace) -> None:
    unsigned = read_json(Path(args.unsigned))
    write_text(args.output, encode_request(unsigned, args.request_id))


def command_sign(args: argparse.Namespace) -> None:
    request_id, unsigned = decode_request(read_text(args.request))
    show_unsigned(unsigned)
    if not args.yes:
        confirmation = input("Type 'yes' to send this hash to FPGA: ").strip().lower()
        if confirmation != "yes":
            raise WalletError("Transaction cancelled")
    with FpgaUartSigner(args.port, timeout=args.timeout) as signer:
        signer.ping()
        signed = sign_payload_fpga(unsigned, signer)
    write_text(args.output, encode_response(request_id, signed))


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(
        description="QR-decoded transaction to Gowin ACG525 UART signing pipeline"
    )
    subparsers = parser.add_subparsers(dest="command", required=True)

    encode = subparsers.add_parser("encode", help="encode unsigned JSON as QR payload text")
    encode.add_argument("--unsigned", required=True)
    encode.add_argument("--request-id", type=int, required=True)
    encode.add_argument("--output", default="qr_request.txt")
    encode.set_defaults(handler=command_encode)

    sign = subparsers.add_parser("sign", help="sign a decoded QR payload through UART")
    sign.add_argument("--request", required=True, help="QR payload text file or - for stdin")
    sign.add_argument("--port", default="/dev/ttyACM0")
    sign.add_argument("--timeout", type=float, default=3.0)
    sign.add_argument("--output", default="qr_response.txt")
    sign.add_argument("--yes", action="store_true")
    sign.set_defaults(handler=command_sign)
    return parser


def main() -> int:
    try:
        args = build_parser().parse_args()
        args.handler(args)
        return 0
    except (FpgaUartError, QrProtocolError, WalletError, OSError, ValueError) as exc:
        print(f"Error: {exc}", file=sys.stderr)
        return 1
    except KeyboardInterrupt:
        print("\nCancelled", file=sys.stderr)
        return 130


if __name__ == "__main__":
    raise SystemExit(main())
