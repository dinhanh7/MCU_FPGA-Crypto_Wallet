# SN34F788F wallet integration

This Keil project now contains the physical approval gateway used between the
web/C reference implementation and the ACG525 FPGA.

## Wiring (3.3 V TTL, common ground)

| Link | SN34F788F RX | SN34F788F TX | Peer |
|---|---|---|---|
| ESP32/UI | P0.11 (UART0) | P0.10 (UART0) | existing ESP32 |
| FPGA | P1.8 (UART1) | P1.9 (UART1) | FPGA TX/RX |
| Web USB-UART | P2.15 (UART2) | P2.14 (UART2) | adapter TX/RX |

All three links use 115200 baud, 8 data bits, no parity and one stop bit.
TX and RX must be crossed. Do not connect a 5 V UART adapter.

## Approval flow

1. Point `MCU_FPGA_UART_PORT` at the USB-UART connected to UART2. Keep
   `FPGA_UART_PORT` only for diagnostics during development.
2. The C signer sends the same versioned A5/5A frame it already sends to the
   FPGA. The MCU validates sync, version, command, bounded length and CRC16.
3. PING is forwarded automatically. ETH and BTC signing frames are frozen in
   MCU RAM and shown as pending on the trusted display.
4. `A` forwards the exact frozen frame to FPGA over UART1. `B` clears it and
   returns FPGA status `VERIFY_ERROR`; no signature is produced.
5. The MCU validates response framing/CRC/sequence before relaying it to the C
   signer. The C signer still verifies the returned signature independently.

The old QR demo signature was deliberately removed. QR transaction parsing did
not independently decode amounts/addresses and therefore was unsafe to approve.

## Build and test

Build `polling.uvprojx` with Keil ARM Compiler 5. The generated image is
`Objects/SN34F780.HEX`. Host-side protocol corner cases are in
`tests/test_wallet_fpga_protocol.c`.

The repository still uses the FPGA test key `d=1`; never use this image with
mainnet funds. PIN storage, attempt throttling and protected key provisioning
remain board/product security work, not test-wallet functionality.
