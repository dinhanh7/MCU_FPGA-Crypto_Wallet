"use strict";

const state = {
  wallets: [],
  unsigned: null,
  signed: null,
  broadcasted: false,
  cSigner: null,
};

const $ = (id) => document.getElementById(id);

function rpcUrl() {
  return $("rpcUrl").value.trim();
}

async function api(path, body = null, method = "POST") {
  const options = { method, headers: {} };
  if (body !== null) {
    options.headers["Content-Type"] = "application/json";
    options.body = JSON.stringify(body);
  }
  const response = await fetch(path, options);
  const payload = await response.json().catch(() => ({ ok: false, error: "Phản hồi không phải JSON" }));
  if (!response.ok || !payload.ok) {
    throw new Error(payload.error || `HTTP ${response.status}`);
  }
  return payload;
}

function log(message, data = null) {
  const timestamp = new Date().toLocaleTimeString("vi-VN");
  $("activityLog").textContent = data
    ? `[${timestamp}] ${message}\n${JSON.stringify(data, null, 2)}`
    : `[${timestamp}] ${message}`;
}

let toastTimer;
function toast(message, isError = false) {
  const element = $("toast");
  element.textContent = message;
  element.classList.toggle("error", isError);
  element.classList.add("visible");
  clearTimeout(toastTimer);
  toastTimer = setTimeout(() => element.classList.remove("visible"), 3500);
}

function setBusy(button, busy, text = "Đang xử lý…") {
  if (busy) {
    button.dataset.originalText = button.textContent;
    button.textContent = text;
    button.disabled = true;
  } else {
    button.textContent = button.dataset.originalText || button.textContent;
    button.disabled = false;
  }
}

function selectedWallet() {
  return $("walletSelect").value;
}

async function checkNetwork() {
  const button = $("connectButton");
  setBusy(button, true);
  try {
    const result = await api("/api/status", { rpcUrl: rpcUrl() });
    $("chainId").textContent = result.chainId;
    $("blockNumber").textContent = result.blockNumber;
    $("clientVersion").textContent = result.clientVersion;
    $("networkBadge").classList.remove("offline");
    $("networkBadge").classList.add("online");
    $("networkBadgeText").textContent = `Chain ${result.chainId}`;
    localStorage.setItem("anvilRpcUrl", rpcUrl());
    log("Kết nối Anvil thành công", result);
    toast(`Đã kết nối Chain ID ${result.chainId}`);
  } catch (error) {
    $("networkBadge").classList.remove("online");
    $("networkBadge").classList.add("offline");
    $("networkBadgeText").textContent = "Mất kết nối";
    toast(error.message, true);
    log("Lỗi kết nối", { error: error.message });
  } finally {
    setBusy(button, false);
  }
}

async function refreshWallets(selectName = null) {
  try {
    const [result, cSignerResult] = await Promise.all([
      api("/api/wallets", null, "GET"),
      api("/api/c-signer", null, "GET"),
    ]);
    state.wallets = result.wallets;
    state.cSigner = cSignerResult.available ? cSignerResult : null;
    if (state.cSigner) {
      state.wallets.push({
        name: "__c_signer__",
        address: state.cSigner.address,
        type: "c",
      });
    }
    const select = $("walletSelect");
    const current = selectName || select.value;
    select.innerHTML = '<option value="">Chọn encrypted keystore</option>';
    for (const wallet of state.wallets) {
      const option = document.createElement("option");
      option.value = wallet.name;
      option.textContent = wallet.type === "c"
        ? `C offline signer · ${shortAddress(wallet.address)}`
        : `${wallet.name} · ${shortAddress(wallet.address)}`;
      select.appendChild(option);
    }
    if (state.wallets.some((wallet) => wallet.name === current)) {
      select.value = current;
    }
    updateWalletSummary();
  } catch (error) {
    toast(error.message, true);
  }
}

function shortAddress(address) {
  return `${address.slice(0, 8)}…${address.slice(-6)}`;
}

function updateWalletSummary() {
  const wallet = state.wallets.find((item) => item.name === selectedWallet());
  $("walletAddress").textContent = wallet ? wallet.address : "Chưa chọn ví";
  $("walletBalance").textContent = "— ETH";
  $("balanceMeta").textContent = wallet
    ? "Đang chờ cập nhật số dư"
    : "Chọn ví để tự động cập nhật số dư";
  $("walletSummary").classList.toggle("empty", !wallet);
  const cSelected = wallet?.type === "c";
  $("passwordSigningFields").classList.toggle("hidden", cSelected);
  $("signerMode").classList.toggle("c-active", cSelected);
  $("signerMode").textContent = cSelected
    ? "C signer · private key được compile trong binary riêng"
    : wallet
      ? "Python signer · encrypted keystore"
      : "Chọn signer để ký giao dịch";
  $("signButton").textContent = cSelected
    ? "2. Ký bằng C offline signer"
    : "2. Kiểm tra và ký offline";
}

async function createWallet() {
  const name = $("createName").value.trim();
  const password = $("createPassword").value;
  const confirmation = $("createPasswordConfirm").value;
  if (password !== confirmation) {
    toast("Mật khẩu nhập lại không khớp", true);
    return;
  }
  const button = $("createWalletButton");
  setBusy(button, true);
  try {
    const result = await api("/api/wallets/create", { name, password });
    clearValues("createName", "createPassword", "createPasswordConfirm");
    await refreshWallets(result.name);
    log("Đã tạo encrypted keystore", { name: result.name, address: result.address });
    toast("Tạo ví thành công");
  } catch (error) {
    toast(error.message, true);
  } finally {
    setBusy(button, false);
  }
}

async function importWallet() {
  const button = $("importWalletButton");
  setBusy(button, true);
  try {
    const result = await api("/api/wallets/import", {
      name: $("importName").value.trim(),
      privateKey: $("privateKey").value.trim(),
      password: $("importPassword").value,
    });
    clearValues("importName", "privateKey", "importPassword");
    await refreshWallets(result.name);
    log("Đã import private key vào encrypted keystore", { name: result.name, address: result.address });
    toast("Import ví thành công");
  } catch (error) {
    toast(error.message, true);
  } finally {
    setBusy(button, false);
  }
}

async function updateBalance(silent = false) {
  if (!selectedWallet()) {
    toast("Hãy chọn ví", true);
    return;
  }
  const button = $("balanceButton");
  setBusy(button, true);
  try {
    const result = await api("/api/balance", {
      wallet: selectedWallet(),
      rpcUrl: rpcUrl(),
    });
    $("walletBalance").textContent = `${result.balanceEth} ETH`;
    $("balanceMeta").textContent = `Đã cập nhật lúc ${new Date().toLocaleTimeString("vi-VN")} · Chain ${result.chainId}`;
    if (!silent) log("Đã cập nhật số dư", result);
  } catch (error) {
    $("balanceMeta").textContent = "Không cập nhật được số dư";
    if (!silent) toast(error.message, true);
  } finally {
    setBusy(button, false);
  }
}

async function buildUnsigned() {
  if (!selectedWallet()) {
    toast("Hãy chọn ví gửi", true);
    return;
  }
  const button = $("buildButton");
  setBusy(button, true);
  try {
    const result = await api("/api/build", {
      wallet: selectedWallet(),
      to: $("recipient").value.trim(),
      amount: $("amount").value.trim(),
      rpcUrl: rpcUrl(),
    });
    state.unsigned = result.unsigned;
    state.signed = null;
    state.broadcasted = false;
    renderUnsigned();
    updateActionButtons();
    log("Đã build unsigned transaction", state.unsigned);
    toast("Unsigned transaction đã sẵn sàng");
  } catch (error) {
    toast(error.message, true);
    log("Build thất bại", { error: error.message });
  } finally {
    setBusy(button, false);
  }
}

function renderUnsigned() {
  const preview = $("transactionPreview");
  if (!state.unsigned) {
    preview.className = "transaction-preview empty";
    preview.innerHTML = "<p>Chưa có giao dịch</p>";
    return;
  }
  const tx = state.unsigned.transaction;
  preview.className = "transaction-preview";
  preview.innerHTML = "";
  const grid = document.createElement("div");
  grid.className = "transaction-grid";
  const rows = [
    ["From", state.unsigned.from],
    ["To", tx.to],
    ["Amount", `${state.unsigned.display.amountEth} ETH`],
    ["Chain ID", String(tx.chainId)],
    ["Nonce", String(tx.nonce)],
    ["Gas", String(tx.gas)],
    ["Max fee", `${state.unsigned.display.maximumFeeEth} ETH`],
    ["Data", tx.data === "0x" ? "0x · plain ETH transfer" : tx.data],
  ];
  for (const [label, value] of rows) {
    const labelNode = document.createElement("span");
    labelNode.textContent = label;
    const valueNode = document.createElement("code");
    valueNode.textContent = value;
    grid.append(labelNode, valueNode);
  }
  preview.appendChild(grid);
}

async function signUnsigned() {
  if (!state.unsigned || !selectedWallet()) return;
  const cSelected = selectedWallet() === "__c_signer__";
  const password = $("signPassword").value;
  if (!cSelected && !password) {
    toast("Nhập mật khẩu keystore", true);
    return;
  }
  if (!window.confirm("Bạn đã kiểm tra địa chỉ nhận, số ETH, Chain ID và phí tối đa?")) return;
  const button = $("signButton");
  setBusy(button, true);
  try {
    const result = cSelected
      ? await api("/api/sign-c", { unsigned: state.unsigned })
      : await api("/api/sign", {
          wallet: selectedWallet(),
          password,
          unsigned: state.unsigned,
        });
    state.signed = result.signed;
    state.broadcasted = false;
    $("signPassword").value = "";
    updateActionButtons();
    log(cSelected ? "C signer đã ký và recover kiểm tra chữ ký" : "Đã ký và tự kiểm tra chữ ký", {
      from: state.signed.from,
      transactionHash: state.signed.transactionHash,
      signature: state.signed.signature || "Python/eth-account",
    });
    toast("Ký giao dịch thành công");
  } catch (error) {
    $("signPassword").value = "";
    toast(error.message, true);
    log("Ký thất bại", { error: error.message });
  } finally {
    setBusy(button, false);
  }
}

async function broadcastSigned() {
  if (!state.signed) return;
  const button = $("broadcastButton");
  setBusy(button, true, "Đang chờ receipt…");
  try {
    const result = await api("/api/broadcast", {
      rpcUrl: rpcUrl(),
      signed: state.signed,
    });
    state.broadcasted = true;
    updateActionButtons();
    const message = result.receipt.alreadyMined
      ? "Giao dịch này đã được ghi trước đó"
      : "Broadcast thành công";
    log(message, { receipt: result.receipt, balances: result.balances });
    toast(`${message} · block ${result.receipt.blockNumber}`);
    if (result.balances?.sender?.address === $("walletAddress").textContent) {
      $("walletBalance").textContent = `${result.balances.sender.balanceEth} ETH`;
      $("balanceMeta").textContent = `Đã cập nhật sau block ${result.receipt.blockNumber}`;
    }
    await checkNetwork();
    await updateBalance(true);
  } catch (error) {
    toast(error.message, true);
    log("Broadcast thất bại", { error: error.message });
  } finally {
    setBusy(button, false);
  }
}

function updateActionButtons() {
  $("downloadUnsignedButton").disabled = !state.unsigned;
  $("signButton").disabled = !state.unsigned || !selectedWallet();
  $("downloadSignedButton").disabled = !state.signed;
  $("broadcastButton").disabled = !state.signed || state.broadcasted;
  $("broadcastButton").textContent = state.broadcasted
    ? "Đã broadcast · hãy build giao dịch mới"
    : "3. Broadcast lên Anvil";
}

function downloadJson(value, filename) {
  const blob = new Blob([JSON.stringify(value, null, 2) + "\n"], { type: "application/json" });
  const url = URL.createObjectURL(blob);
  const anchor = document.createElement("a");
  anchor.href = url;
  anchor.download = filename;
  anchor.click();
  URL.revokeObjectURL(url);
}

async function loadJsonFile(input, kind) {
  const file = input.files[0];
  if (!file) return;
  try {
    const value = JSON.parse(await file.text());
    if (kind === "unsigned") {
      if (value.format !== "anvil-cold-wallet-unsigned-v1") throw new Error("Sai định dạng unsigned transaction");
      state.unsigned = value;
      state.signed = null;
      state.broadcasted = false;
      renderUnsigned();
    } else {
      if (value.format !== "anvil-cold-wallet-signed-v1") throw new Error("Sai định dạng signed transaction");
      state.signed = value;
      state.broadcasted = false;
      if (value.unsigned) {
        state.unsigned = value.unsigned;
        renderUnsigned();
      }
    }
    updateActionButtons();
    log(`Đã mở ${kind} JSON`, value);
  } catch (error) {
    toast(error.message, true);
  } finally {
    input.value = "";
  }
}

function clearValues(...ids) {
  for (const id of ids) $(id).value = "";
}

function bindEvents() {
  $("connectButton").addEventListener("click", checkNetwork);
  $("refreshWalletsButton").addEventListener("click", () => refreshWallets());
  $("walletSelect").addEventListener("change", () => {
    updateWalletSummary();
    updateActionButtons();
    if (selectedWallet()) updateBalance(true);
  });
  $("balanceButton").addEventListener("click", () => updateBalance(false));
  $("createWalletButton").addEventListener("click", createWallet);
  $("importWalletButton").addEventListener("click", importWallet);
  $("buildButton").addEventListener("click", buildUnsigned);
  $("signButton").addEventListener("click", signUnsigned);
  $("broadcastButton").addEventListener("click", broadcastSigned);
  $("downloadUnsignedButton").addEventListener("click", () => downloadJson(state.unsigned, "unsigned_transaction.json"));
  $("downloadSignedButton").addEventListener("click", () => downloadJson(state.signed, "signed_transaction.json"));
  $("unsignedFile").addEventListener("change", (event) => loadJsonFile(event.target, "unsigned"));
  $("signedFile").addEventListener("change", (event) => loadJsonFile(event.target, "signed"));
  $("clearActivityButton").addEventListener("click", () => { $("activityLog").textContent = "Sẵn sàng."; });
}

async function initialize() {
  const savedRpc = localStorage.getItem("anvilRpcUrl");
  $("rpcUrl").value = savedRpc || document.body.dataset.defaultRpc;
  bindEvents();
  await refreshWallets();
  await checkNetwork();
  window.setInterval(() => {
    if (selectedWallet()) updateBalance(true);
  }, 10000);
}

window.addEventListener("DOMContentLoaded", initialize);
