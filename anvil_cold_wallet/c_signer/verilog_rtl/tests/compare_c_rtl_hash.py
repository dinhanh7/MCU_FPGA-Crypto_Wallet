#!/usr/bin/env python3
import json
import subprocess
import sys


DEFAULT_HASHES = (
    "b11a13b969e57b09787936eaac76c01e8989b68a17b2c18a93554c89d76912f7",
    "928f3d3a4e7f6c9b0d112233445566778899aabbccddeeff0011223344556677",
)


def run(command):
    return subprocess.run(command, check=True, capture_output=True, text=True).stdout


def main():
    if len(sys.argv) < 3:
        raise SystemExit("usage: compare_c_rtl_hash.py C_SIGNER RTL_SIM [HASH ...]")

    c_signer = sys.argv[1]
    rtl_sim = sys.argv[2]
    hashes = sys.argv[3:] or DEFAULT_HASHES

    for hash_text in hashes:
        normalized_hash = hash_text.removeprefix("0x").lower()
        if len(normalized_hash) != 64 or any(character not in "0123456789abcdef" for character in normalized_hash):
            raise SystemExit(f"invalid 32-byte hash: {hash_text}")

        c_result = json.loads(run([c_signer, "sign-hash", "--hash", normalized_hash, "--yes"]))
        rtl_output = run([rtl_sim, f"+MESSAGE_HASH={normalized_hash}"])
        result_line = next(
            (line for line in rtl_output.splitlines() if line.startswith("RTL_RESULT ")),
            None,
        )
        if result_line is None:
            raise RuntimeError(f"RTL result missing for {normalized_hash}\n{rtl_output}")
        rtl_result = json.loads(result_line.removeprefix("RTL_RESULT "))

        for field in ("messageHash", "yParity", "r", "s"):
            if c_result[field] != rtl_result[field]:
                raise RuntimeError(
                    f"mismatch for {normalized_hash}, field {field}: "
                    f"C={c_result[field]} RTL={rtl_result[field]}"
                )
        print(f"PASS: C and RTL match for 0x{normalized_hash}")


if __name__ == "__main__":
    main()
