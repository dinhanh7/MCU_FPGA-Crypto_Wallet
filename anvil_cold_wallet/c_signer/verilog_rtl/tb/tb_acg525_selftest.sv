`timescale 1ns/1ps

module tb_acg525_selftest;
  logic clk=0;
  logic led_busy,led_pass;
  integer cycles=0;
  always #10 clk=~clk; // 50 MHz
  acg525_selftest_top dut(.clk_in_50m(clk),.led_busy(led_busy),.led_pass(led_pass));

  always @(posedge clk) begin
    cycles<=cycles+1;
    if(led_pass) begin
      $display("PASS: ACG525 power-on self-test completed in %0d cycles",cycles);
      $finish;
    end
    if(cycles>100000000)$fatal(1,"ACG525 self-test timeout");
  end
endmodule
