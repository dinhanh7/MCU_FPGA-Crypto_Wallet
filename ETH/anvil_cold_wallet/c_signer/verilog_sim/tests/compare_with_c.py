#!/usr/bin/env python3
"""Differential tests: pure SystemVerilog model versus the C/libsecp256k1 signer."""

from __future__ import annotations

import json
import subprocess
import sys
from pathlib import Path


PROJECT = Path(__file__).resolve().parents[2]
C_SIGNER = PROJECT / "build" / "eth_signer"

CASES = [
    {
        "chain_id": "31338",
        "nonce": "3",
        "priority": "1000000000",
        "max_fee": "1021491012",
        "gas": "21000",
        "to": "0x1d6D332F0aB9C6CFd95FAc2ba2b8CeFD39F012De",
        "value": "2000000000000000000",
        "data": "0x",
    },
    {
        "chain_id": "1",
        "nonce": "0",
        "priority": "1000000000",
        "max_fee": "30000000000",
        "gas": "21000",
        "to": "0x1111111111111111111111111111111111111111",
        "value": "1",
        "data": "0x",
    },
    {
        "chain_id": "31338",
        "nonce": "15",
        "priority": "7",
        "max_fee": "1234567890123",
        "gas": "100000",
        "to": "0xabcdefabcdefabcdefabcdefabcdefabcdefabcd",
        "value": "0",
        "data": "0xdeadbeef",
    },
    {
        "chain_id": "31338",
        "nonce": "255",
        "priority": "1000000000",
        "max_fee": "2000000000",
        "gas": "250000",
        "to": "0x2222222222222222222222222222222222222222",
        "value": "123456789012345678901234567890",
        "data": "0x" + "a5" * 60,
    },
]


def c_command(case: dict[str, str]) -> list[str]:
    return [
        str(C_SIGNER),
        "sign",
        "--chain-id",
        case["chain_id"],
        "--nonce",
        case["nonce"],
        "--max-priority-fee-per-gas",
        case["priority"],
        "--max-fee-per-gas",
        case["max_fee"],
        "--gas-limit",
        case["gas"],
        "--to",
        case["to"],
        "--value",
        case["value"],
        "--data",
        case["data"],
        "--yes",
    ]


def sv_command(executable: Path, case: dict[str, str]) -> list[str]:
    return [
        str(executable),
        f"+CHAIN_ID={case['chain_id']}",
        f"+NONCE={case['nonce']}",
        f"+MAX_PRIORITY_FEE_PER_GAS={case['priority']}",
        f"+MAX_FEE_PER_GAS={case['max_fee']}",
        f"+GAS_LIMIT={case['gas']}",
        f"+TO={case['to']}",
        f"+VALUE={case['value']}",
        f"+DATA={case['data']}",
    ]


def extract_json(output: str) -> dict[str, object]:
    start = output.index("{")
    end = output.index("\n}\n", start) + 2
    return json.loads(output[start:end])


def run_json(command: list[str]) -> dict[str, object]:
    completed = subprocess.run(command, check=True, capture_output=True, text=True)
    return extract_json(completed.stdout)


def main() -> int:
    executable = Path(sys.argv[1]).resolve()
    if not C_SIGNER.is_file():
        raise SystemExit(f"Missing C signer: {C_SIGNER}. Run ../setup.sh first.")
    for index, case in enumerate(CASES, start=1):
        expected = run_json(c_command(case))
        actual = run_json(sv_command(executable, case))
        if actual != expected:
            print(f"FAIL case {index}")
            print("C:", json.dumps(expected, indent=2))
            print("SV:", json.dumps(actual, indent=2))
            return 1
        print(f"PASS case {index}: {actual['transactionHash']}")
    print(f"PASS: {len(CASES)} SystemVerilog transactions match C/libsecp256k1 exactly")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
