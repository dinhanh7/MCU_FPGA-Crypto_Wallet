`timescale 1ns/1ps
`include "../rtl/btc_private_key_rtl.svh"

// One ACG525 image for both supported signing paths:
//   command 0x01: Ethereum 32-byte EIP-1559 signing hash -> yParity/r/s
//   command 0x10: Bitcoin frozen BIP143 fields -> freeze_id/digest/r/s
// The same provisioned secp256k1 key is intentionally shared by both modes.
module acg525_dual_uart_top (
  input  logic clk_in_50m,
  input  logic uart_rx,
  output logic uart_tx,
  output logic led_busy,
  output logic led_pass
);
  logic [7:0] reset_counter = 8'b0;
  logic reset_n;

  assign reset_n=&reset_counter;
  always_ff @(posedge clk_in_50m) begin
    if(!reset_n) reset_counter<=reset_counter+1'b1;
  end

  dual_coin_uart_signer_bridge #(
    .CLOCKS_PER_BIT(434),
    .PRIVATE_KEY(`BTC_PRIVATE_KEY),
    .MAX_OUTPUT_BYTES(512)
  ) bridge (
    .clk(clk_in_50m),.reset_n(reset_n),.uart_rx(uart_rx),.uart_tx(uart_tx),
    .busy(led_busy),.last_success(led_pass)
  );
endmodule
