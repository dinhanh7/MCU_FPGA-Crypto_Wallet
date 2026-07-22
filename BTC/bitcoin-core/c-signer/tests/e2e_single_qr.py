#!/usr/bin/env python3
import base64
import json
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
PREFIX = "/tmp/mcu-single-qr-e2e-20260721"


def post(path, data):
    req = urllib.request.Request(URL + path, data=json.dumps(data).encode(),
        headers={"Content-Type":"application/json"}, method="POST")
    with urllib.request.urlopen(req) as response: return json.load(response)


def decode_bbqr_z(frame):
    assert re.fullmatch(r"B\$ZP0100[A-Z2-7]+", frame)
    text = frame[8:]
    packed = base64.b32decode(text + "=" * ((-len(text)) % 8))
    return zlib.decompress(packed, wbits=-10)


for suffix in ("-wrong.html", "-cancel.html", "-signed.html"):
    try: os.unlink(PREFIX + suffix)
    except FileNotFoundError: pass

created = post("/api/psbt/create", {
    "source":"coldwatch",
    "address":"bcrt1qhnqzulqqna2du5rag4lqtteldqn48t7fa68ua8",
    "amount":"0.00010000",
})
unsigned_qr = created["qr"]
unsigned = decode_bbqr_z(unsigned_qr)
scanner_input = unsigned_qr + "\n"

wrong = subprocess.run([SIGNER,"mcu-sign-qr",KEY,PASSFILE,PREFIX+"-wrong.html"],
    input=scanner_input+"000000\n111111\n222222\n", text=True, capture_output=True)
cancelled = subprocess.run([SIGNER,"mcu-sign-qr",KEY,PASSFILE,PREFIX+"-cancel.html"],
    input=scanner_input+PIN+"\nHUY\n", text=True, capture_output=True)
signed = subprocess.run([SIGNER,"mcu-sign-qr",KEY,PASSFILE,PREFIX+"-signed.html"],
    input=scanner_input+PIN+"\nDONG Y\n", text=True, capture_output=True, check=True)
out_frames = [m.group(1) for m in re.finditer(r"BBQR_OUT_FRAME_\d+=(B\$ZP0100[A-Z2-7]+)", signed.stdout)]
assert len(out_frames) == 1
signed_raw = decode_bbqr_z(out_frames[0])
broadcast = post("/api/psbt/broadcast", {
    "source":"coldwatch", "qr":out_frames[0]})
post("/api/cold/mine", {"source":"coldwatch","count":1})
with urllib.request.urlopen(URL+"/api/status") as response: status=json.load(response)
with open(PREFIX+"-signed.html") as f: html=f.read()

print(json.dumps({
    "transport":"single BBQr Z/P",
    "unsigned_psbt_bytes":len(unsigned),
    "unsigned_qr_chars":len(unsigned_qr),
    "unsigned_qr_frames":1,
    "wrong_passkey_no_qr":wrong.returncode == 3 and not os.path.exists(PREFIX+"-wrong.html"),
    "cancel_no_qr":cancelled.returncode == 4 and not os.path.exists(PREFIX+"-cancel.html"),
    "freeze_verified":"freeze_id=" in signed.stdout,
    "signature_verified":"signature_verified=true" in signed.stdout,
    "signed_psbt_bytes":len(signed_raw),
    "signed_qr_chars":len(out_frames[0]),
    "signed_qr_frames":len(out_frames),
    "mcu_qr_html_has_svg":"<svg" in html,
    "bitcoin_core_complete":broadcast["complete"],
    "mempool_allowed":broadcast["mempool_allowed"],
    "txid":broadcast["txid"],
    "confirmations":status["cold"]["confirmations"],
    "mempool_count":status["mempool_count"],
}, indent=2))
