#!/usr/bin/env python3
import base64
import json
import hashlib
import os
import re
import subprocess
import urllib.request
import zlib

URL = "http://127.0.0.1:8787"
CORE = "/home/edabk_llm_manba/workspace/dinhanh_k68/bitcoin-core"
SIGNER = CORE + "/c-signer/bin/coldsign"
KEY = CORE + "/c-signer/keys/cold.key"
PASSFILE = CORE + "/c-signer/keys/mcu.passkey"
PIN = "246810"
PREFIX = "/tmp/mcu-qr100-e2e-20260721"
ALPH = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"


def post(path, data):
    req = urllib.request.Request(URL + path, data=json.dumps(data).encode(),
        headers={"Content-Type":"application/json"}, method="POST")
    with urllib.request.urlopen(req) as response: return json.load(response)


def decode_bbqr_z(frames):
    parts = {}
    total = None
    for frame in frames:
        assert re.fullmatch(r"B\$ZP[0-9A-Z]{4}[A-Z2-7]+", frame)
        frame_total, index = int(frame[4:6], 36), int(frame[6:8], 36)
        total = frame_total if total is None else total
        assert frame_total == total
        parts[index] = frame[8:]
    assert len(parts) == total
    text = "".join(parts[i] for i in range(total))
    packed = base64.b32decode(text + "=" * ((-len(text)) % 8))
    return zlib.decompress(packed, wbits=-10)


def encode_bbqr_z(raw):
    c = zlib.compressobj(9, zlib.DEFLATED, -10)
    text = base64.b32encode(c.compress(raw) + c.flush()).decode().rstrip("=")
    chunk = len(text) if len(text) + 8 <= 100 else 88
    total = (len(text) + chunk - 1) // chunk
    b36 = lambda n: ALPH[n // 36] + ALPH[n % 36]
    return ["B$ZP" + b36(total) + b36(i) + text[i*chunk:(i+1)*chunk]
            for i in range(total)]


def compact(n):
    if n < 253: return bytes([n])
    if n <= 65535: return b"\xfd" + n.to_bytes(2, "little")
    return b"\xfe" + n.to_bytes(4, "little")


def read_compact(raw, off):
    n = raw[off]
    if n < 253: return n, off + 1
    size = 2 if n == 253 else 4 if n == 254 else 8
    return int.from_bytes(raw[off+1:off+1+size], "little"), off + 1 + size


def add_global_padding(raw, count=900):
    off = 5
    while True:
        key_len, after = read_compact(raw, off)
        if key_len == 0: break
        off = after + key_len
        value_len, off = read_compact(raw, off)
        off += value_len
    noise = b"".join(hashlib.sha256(b"adaptive-qr" + i.to_bytes(4,"big")).digest()
                     for i in range((count + 31)//32))[:count]
    key = b"\xfc\x01Q\x00"
    field = compact(len(key)) + key + compact(len(noise)) + noise
    return raw[:off] + field + raw[off:]


for suffix in ("-wrong.html", "-cancel.html", "-signed.html"):
    try: os.unlink(PREFIX + suffix)
    except FileNotFoundError: pass

created = post("/api/psbt/create", {
    "source":"coldwatch",
    "address":"bcrt1qhnqzulqqna2du5rag4lqtteldqn48t7fa68ua8",
    "amount":"0.00010000",
})
unsigned_frames = created["qr_frames"]
assert max(len(x.encode("ascii")) for x in unsigned_frames) <= 100
unsigned = decode_bbqr_z(unsigned_frames)
scanner_input = "\n".join(reversed(unsigned_frames)) + "\n"

wrong = subprocess.run([SIGNER,"mcu-sign-qr",KEY,PASSFILE,PREFIX+"-wrong.html"],
    input=scanner_input+"000000\n111111\n222222\n", text=True, capture_output=True)
cancelled = subprocess.run([SIGNER,"mcu-sign-qr",KEY,PASSFILE,PREFIX+"-cancel.html"],
    input=scanner_input+PIN+"\nHUY\n", text=True, capture_output=True)
signed = subprocess.run([SIGNER,"mcu-sign-qr",KEY,PASSFILE,PREFIX+"-signed.html"],
    input=scanner_input+PIN+"\nDONG Y\n", text=True, capture_output=True, check=True)
out_frames = [m.group(1) for m in re.finditer(r"BBQR_OUT_FRAME_\d+=(B\$ZP[0-9A-Z]+)", signed.stdout)]
assert out_frames and max(len(x.encode("ascii")) for x in out_frames) <= 100
signed_raw = decode_bbqr_z(out_frames)
broadcast = post("/api/psbt/broadcast", {
    "source":"coldwatch", "qr_frames":out_frames})
post("/api/cold/mine", {"source":"coldwatch","count":1})

# Add a standards-compliant proprietary global PSBT field with incompressible
# test bytes so the exact same valid transaction must use animated BBQr.
large_created = post("/api/psbt/create", {
    "source":"coldwatch",
    "address":"bcrt1qhnqzulqqna2du5rag4lqtteldqn48t7fa68ua8",
    "amount":"0.00010000",
})
large_raw = add_global_padding(decode_bbqr_z(large_created["qr_frames"]))
large_frames = encode_bbqr_z(large_raw)
assert len(large_frames) > 1
assert max(len(x.encode("ascii")) for x in large_frames) <= 100
large_html = PREFIX + "-large-signed.html"
try: os.unlink(large_html)
except FileNotFoundError: pass
large_signed = subprocess.run([SIGNER,"mcu-sign-qr",KEY,PASSFILE,large_html],
    input="\n".join(reversed(large_frames))+"\n"+PIN+"\nDONG Y\n",
    text=True, capture_output=True, check=True)
large_out = [m.group(1) for m in re.finditer(r"BBQR_OUT_FRAME_\d+=(B\$ZP[0-9A-Z]+)", large_signed.stdout)]
assert len(large_out) > 1
assert max(len(x.encode("ascii")) for x in large_out) <= 100
large_broadcast = post("/api/psbt/broadcast", {
    "source":"coldwatch", "qr_frames":list(reversed(large_out))})
post("/api/cold/mine", {"source":"coldwatch","count":1})
with urllib.request.urlopen(URL+"/api/status") as response: status=json.load(response)
with open(PREFIX+"-signed.html") as f: html=f.read()
with open(large_html) as f: large_html_text=f.read()

print(json.dumps({
    "transport":"adaptive BBQr Z/P, max 100 bytes per QR",
    "unsigned_psbt_bytes":len(unsigned),
    "small_input_qr_frames":len(unsigned_frames),
    "small_input_max_frame_bytes":max(len(x.encode("ascii")) for x in unsigned_frames),
    "small_output_qr_frames":len(out_frames),
    "small_output_max_frame_bytes":max(len(x.encode("ascii")) for x in out_frames),
    "wrong_passkey_no_qr":wrong.returncode == 3 and not os.path.exists(PREFIX+"-wrong.html"),
    "cancel_no_qr":cancelled.returncode == 4 and not os.path.exists(PREFIX+"-cancel.html"),
    "freeze_verified":"freeze_id=" in signed.stdout,
    "signature_verified":"signature_verified=true" in signed.stdout,
    "small_signed_psbt_bytes":len(signed_raw),
    "mcu_qr_html_has_svg":"<svg" in html,
    "large_psbt_bytes":len(large_raw),
    "large_input_qr_frames":len(large_frames),
    "large_input_max_frame_bytes":max(len(x.encode("ascii")) for x in large_frames),
    "large_input_out_of_order":large_signed.stdout.count("QR_PROGRESS=") == len(large_frames),
    "large_output_qr_frames":len(large_out),
    "large_output_max_frame_bytes":max(len(x.encode("ascii")) for x in large_out),
    "animated_html": "setInterval" in large_html_text,
    "bitcoin_core_complete":broadcast["complete"],
    "mempool_allowed":broadcast["mempool_allowed"],
    "small_txid":broadcast["txid"],
    "large_bitcoin_core_complete":large_broadcast["complete"],
    "large_mempool_allowed":large_broadcast["mempool_allowed"],
    "large_txid":large_broadcast["txid"],
    "confirmations":status["cold"]["confirmations"],
    "mempool_count":status["mempool_count"],
}, indent=2))
