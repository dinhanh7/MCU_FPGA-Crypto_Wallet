`timescale 1ns/1ps

module tb_acg525_selftest_top;
  logic clk=0;
  logic led_busy,led_pass;
  integer cycles=0;

  always #5 clk=~clk;

  acg525_btc_selftest_top dut (
    .clk_in_50m(clk),.led_busy(led_busy),.led_pass(led_pass)
  );

  initial begin
    while(!led_pass && cycles<40_000_000) begin
      @(posedge clk);cycles=cycles+1;
    end
    if(!led_pass || led_busy) $fatal(1,"ACG525 self-test top failed");
    $display("PASS: ACG525 ROM vector and 768-bit known answer in %0d cycles",cycles);
    $finish;
  end
endmodule
