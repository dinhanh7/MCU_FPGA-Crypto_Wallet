#!/usr/bin/env python3
"""One localhost web process for the BTC/ETH dual MCU + FPGA prototype."""

from __future__ import annotations

import argparse
import importlib.util
import json
import os
import sys
from pathlib import Path

from flask import Flask, Response, jsonify, redirect, request, send_file
from werkzeug.middleware.dispatcher import DispatcherMiddleware
from werkzeug.serving import run_simple


PROJECT_ROOT = Path(__file__).resolve().parents[1]
WEB_ROOT = PROJECT_ROOT / "web"
BTC_CORE_ROOT = Path(os.environ.get(
    "BITCOIN_CORE_ROOT", str(PROJECT_ROOT.parent / "BTC/bitcoin-core")
)).resolve()
BTC_WEB = WEB_ROOT / "bitcoin"
ETH_ROOT = WEB_ROOT / "ethereum"


def load_module(name: str, path: Path):
    spec = importlib.util.spec_from_file_location(name, path)
    if spec is None or spec.loader is None:
        raise RuntimeError(f"Cannot import {path}")
    module = importlib.util.module_from_spec(spec)
    sys.modules[name] = module
    spec.loader.exec_module(module)
    return module


sys.path.insert(0, str(ETH_ROOT))
btc = load_module("dual_btc_web", BTC_WEB / "server.py")
eth_module = load_module("dual_eth_web", ETH_ROOT / "app.py")

# The old BTC page was once tied to a developer home directory.  The dual
# launcher pins every runtime path to this checked-out workspace instead.
btc.BASE = str(BTC_CORE_ROOT)
btc.CLI = os.environ.get("BITCOIN_CLI", str(BTC_CORE_ROOT / "bin/bitcoin-cli"))
btc.DATA = os.environ.get("BITCOIN_DATA_DIR", str(BTC_CORE_ROOT / "data"))
btc.COLD_REGISTRY = str(BTC_WEB / "wallets.json")
btc.COLD_LAST_FILE = str(WEB_ROOT / "runtime/last-web-tx.json")


shell = Flask("dual_wallet_shell", static_folder=None)
btc_app = Flask("dual_btc_wallet", static_folder=None)
eth_app = eth_module.create_app()


LANDING_PAGE = """<!doctype html>
<html lang="vi"><head><meta charset="utf-8"><meta name="viewport" content="width=device-width,initial-scale=1">
<title>Dual BTC / ETH Hardware Wallet</title><style>
:root{color-scheme:dark;--bg:#07111f;--card:#111f32;--line:#29415e;--btc:#f7931a;--eth:#8ca6ff}
*{box-sizing:border-box}body{margin:0;background:radial-gradient(circle at top,#173152,var(--bg) 58%);font:16px system-ui;color:#edf4ff}
main{max-width:980px;margin:auto;padding:64px 18px}h1{font-size:clamp(34px,6vw,66px);margin:0 0 12px}.sub{color:#a9bdd7;max-width:760px;line-height:1.6}
.flow{margin:30px 0;padding:15px;border:1px solid var(--line);border-radius:12px;color:#bdd0e7;background:#0a1728}
.grid{display:grid;grid-template-columns:repeat(auto-fit,minmax(280px,1fr));gap:18px}.card{display:block;text-decoration:none;color:inherit;background:var(--card);border:1px solid var(--line);border-radius:18px;padding:27px;transition:.18s}.card:hover{transform:translateY(-3px);border-color:#6c89aa}.coin{font-size:40px}.btc{color:var(--btc)}.eth{color:var(--eth)}code{color:#ffd29b}.warn{color:#ffcc80;margin-top:28px}
</style></head><body><main><h1>Dual BTC / ETH Wallet</h1>
<p class="sub">Một web localhost, một chương trình C đại diện MCU Sonix SN34F788F và một bitstream dual trên Gowin ACG525. MCU review/freeze/PIN; FPGA giữ khóa và ký secp256k1.</p>
<div class="flow">Web/Core hoặc Anvil → <b>dual-mcu</b> → UART 115200 → <b>dual FPGA</b> → chữ ký được MCU tự xác minh → hoàn thiện và broadcast</div>
<div class="grid"><a class="card" href="/btc/"><div class="coin btc">₿</div><h2>Bitcoin</h2><p>PSBT · BIP143 trong FPGA · ghép chữ ký vào PSBT đã freeze</p></a>
<a class="card" href="/eth/"><div class="coin eth">Ξ</div><h2>Ethereum</h2><p>EIP-1559 · Keccak/RLP trong MCU · recoverable ECDSA trong FPGA</p></a></div>
<p class="warn">Prototype regtest/Anvil và khóa FPGA test d=1. Không dùng với mainnet hoặc tài sản thật.</p></main></body></html>"""


@shell.get("/")
def landing():
    return Response(LANDING_PAGE, content_type="text/html; charset=utf-8")


@shell.get("/btc")
def btc_redirect():
    return redirect("/btc/")


@shell.get("/eth")
def eth_redirect():
    return redirect("/eth/")


@btc_app.after_request
def btc_security_headers(response):
    response.headers["Cache-Control"] = "no-store"
    response.headers["X-Content-Type-Options"] = "nosniff"
    response.headers["X-Frame-Options"] = "DENY"
    response.headers["Referrer-Policy"] = "no-referrer"
    return response


@btc_app.get("/")
def btc_index():
    page = btc.PAGE.replace("/api/", "/btc/api/").replace(
        "/static/", "/btc/static/"
    )
    page = page.replace(
        "cd ~/workspace/dinhanh_k68/bitcoin-core/c-signer\n"
        "bin/coldsign mcu-sign-file-fpga",
        f"cd {PROJECT_ROOT / 'mcu'}\n"
        "bin/dual-mcu btc mcu-sign-file-fpga",
    )
    page = page.replace(
        "keys/KEY.pub keys/mcu.passkey /dev/ttyUSB0",
        f"{PROJECT_ROOT / 'mcu/test-data/fpga_test_d1.pub'} "
        f"{PROJECT_ROOT / 'mcu/test-data/test.passkey'} /dev/ttyACM0",
    )
    return Response(page, content_type="text/html; charset=utf-8",
                    headers={"Permissions-Policy": "camera=(self)"})


@btc_app.get("/static/<path:name>")
def btc_static(name: str):
    allowed = {"qrcode.min.js", "jsQR.js"}
    if name not in allowed:
        return jsonify(error="not found"), 404
    return send_file(BTC_WEB / name, max_age=86400)


@btc_app.get("/api/status")
def btc_status():
    try:
        info = btc.cli("getblockchaininfo")
        return jsonify(chain=info["chain"], blocks=info["blocks"],
                       mempool_count=len(btc.cli("getrawmempool")),
                       wallets=btc.wallet_status(), cold=btc.cold_status())
    except Exception as exc:
        return jsonify(error=str(exc)), 503


def btc_json():
    value = request.get_json(silent=True)
    if not isinstance(value, dict):
        raise ValueError("Request must be a JSON object")
    return value


@btc_app.post("/api/wallet")
def btc_wallet():
    try:
        data = btc_json()
        name = str(data.get("name", "")).strip()
        if not btc.re.fullmatch(r"[A-Za-z0-9_.-]{1,64}", name):
            raise ValueError("Invalid wallet name")
        btc.ensure_wallet(name)
        return jsonify(ok=True, name=name)
    except Exception as exc:
        return jsonify(error=str(exc)), 400


@btc_app.post("/api/address")
def btc_address():
    try:
        name = str(btc_json().get("wallet", "")).strip()
        info = btc.cli("-rpcwallet=" + name, "getwalletinfo")
        if not info.get("private_keys_enabled"):
            raise ValueError("Watch-only wallet cannot create address")
        return jsonify(address=btc.fixed_receive_address(name, True), fixed=True)
    except Exception as exc:
        return jsonify(error=str(exc)), 400


@btc_app.post("/api/mcu/register")
def btc_register():
    try:
        data = btc_json()
        return jsonify(btc.register_source(str(data.get("name", "")).strip(),
                                           str(data.get("pubkey", "")).strip()))
    except Exception as exc:
        return jsonify(error=str(exc)), 400


@btc_app.post("/api/psbt/create")
def btc_create_psbt():
    try:
        data = btc_json()
        return jsonify(btc.create_unsigned_psbt(
            str(data.get("source", "")).strip(),
            str(data.get("address", "")).strip(), data.get("amount", "0")))
    except Exception as exc:
        return jsonify(error=str(exc)), 400


@btc_app.post("/api/psbt/broadcast")
def btc_broadcast():
    try:
        data = btc_json()
        psbt = btc.signed_psbt_from_request(data)
        return jsonify(btc.broadcast_signed_psbt(
            str(data.get("source", "")).strip(), psbt))
    except Exception as exc:
        return jsonify(error=str(exc)), 400


@btc_app.post("/api/cold/mine")
def btc_mine():
    try:
        data = btc_json()
        return jsonify(btc.cold_mine(str(data.get("source", "")).strip(),
                                     data.get("count", 1)))
    except Exception as exc:
        return jsonify(error=str(exc)), 400


application = DispatcherMiddleware(shell, {"/btc": btc_app, "/eth": eth_app})


def main() -> int:
    parser = argparse.ArgumentParser(description="Dual BTC/ETH localhost web")
    parser.add_argument("--host", default="127.0.0.1")
    parser.add_argument("--port", default=8787, type=int)
    args = parser.parse_args()
    if args.host not in {"127.0.0.1", "localhost", "::1"}:
        raise SystemExit("Refusing non-local host for wallet safety")
    print(f"Dual wallet web: http://{args.host}:{args.port}", flush=True)
    run_simple(args.host, args.port, application, threaded=True,
               use_debugger=False, use_reloader=False)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
