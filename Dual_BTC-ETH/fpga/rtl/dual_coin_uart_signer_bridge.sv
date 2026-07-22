`timescale 1ns/1ps

// Semantic top-level name for the dual Bitcoin/Ethereum UART protocol.
// btc_uart_signer_bridge retains its historical module name so existing BTC
// testbenches and MCU firmware remain source-compatible.
module dual_coin_uart_signer_bridge #(
  parameter integer CLOCKS_PER_BIT = 434,
  parameter logic [255:0] PRIVATE_KEY = 256'h1,
  parameter integer MAX_OUTPUT_BYTES = 512
) (
  input  logic clk,
  input  logic reset_n,
  input  logic uart_rx,
  output logic uart_tx,
  output logic busy,
  output logic last_success
);
  btc_uart_signer_bridge #(
    .CLOCKS_PER_BIT(CLOCKS_PER_BIT),
    .PRIVATE_KEY(PRIVATE_KEY),
    .MAX_OUTPUT_BYTES(MAX_OUTPUT_BYTES)
  ) implementation (
    .clk(clk),.reset_n(reset_n),.uart_rx(uart_rx),.uart_tx(uart_tx),
    .busy(busy),.last_success(last_success)
  );
endmodule
