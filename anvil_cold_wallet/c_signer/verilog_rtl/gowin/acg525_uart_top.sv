`timescale 1ns/1ps
`include "../rtl/private_key_rtl.svh"

module acg525_uart_top (
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

  uart_hash_signer_bridge #(
    .CLOCKS_PER_BIT(434),
    .PRIVATE_KEY(`ETH_PRIVATE_KEY)
  ) bridge (
    .clk(clk_in_50m),
    .reset_n(reset_n),
    .uart_rx(uart_rx),
    .uart_tx(uart_tx),
    .busy(led_busy),
    .last_success(led_pass)
  );
endmodule
