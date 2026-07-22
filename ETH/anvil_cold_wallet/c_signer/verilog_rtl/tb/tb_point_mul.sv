`timescale 1ns/1ps

module tb_point_mul;
  import eth_secp256k1_pkg::*;
  logic clk=0,reset_n=0,start,done;
  logic [255:0] scalar,x,y;
  always #5 clk=~clk;
  secp256k1_point_mul dut(
    .clk(clk),.reset_n(reset_n),.start(start),.scalar(scalar),
    .busy(),.done(done),.infinity(),.affine_x(x),.affine_y(y));

  task automatic run_scalar(input logic [255:0] value);
    begin
      scalar=value;
      @(posedge clk); start<=1;
      @(posedge clk); start<=0;
      wait(done);
    end
  endtask

  initial begin
    start=0; scalar=0;
    repeat(3) @(posedge clk); reset_n=1;
    run_scalar(1);
    if(x!==SECP256K1_GX || y!==SECP256K1_GY)
      $fatal(1,"scalar 1 failed: x=%x y=%x",x,y);

    run_scalar(2);
    if(x!==256'hc6047f9441ed7d6d3045406e95c07cd85c778e4b8cef3ca7abac09b95c709ee5 ||
       y!==256'h1ae168fea63dc339a3c58419466ceaeef7f632653266d0e1236431a950cfe52a)
      $fatal(1,"scalar 2 failed: x=%x y=%x",x,y);

    run_scalar(256'h833634192353b7f66dadec04f7bc71458eea9179e07a65cd0f587e711efd795d);
    if(x!==256'h3c403f14f95cc9624023f2dbd5dce5df2d502104e3ee4c766ffcdad17444ec3a || y[0]!==1'b0)
      $fatal(1,"RFC6979 nonce point failed: x=%x parity=%0d",x,y[0]);
    $display("PASS: synthesizable sequential secp256k1 point multiplication");
    $finish;
  end
endmodule
