`timescale 1ns/1ps

module tb_modmath;
  import eth_secp256k1_pkg::*;

  logic clk = 1'b0;
  logic reset_n = 1'b0;
  always #5 clk = ~clk;

  logic mul_start, mul_done;
  logic [255:0] mul_a, mul_b, mul_mod, mul_result;
  logic inv_start, inv_done;
  logic [255:0] inv_value, inv_mod, inv_result;
  logic add_start,add_subtract,add_done;
  logic [255:0] add_a,add_b,add_mod,add_result;

  modaddsub256_seq addsub (
    .clk(clk),.reset_n(reset_n),.start(add_start),.subtract(add_subtract),
    .operand_a(add_a),.operand_b(add_b),.modulus(add_mod),
    .busy(),.done(add_done),.result(add_result)
  );

  modmul256 mul (
    .clk(clk), .reset_n(reset_n), .start(mul_start),
    .operand_a(mul_a), .operand_b(mul_b), .modulus(mul_mod),
    .busy(), .done(mul_done), .result(mul_result)
  );

  modinv256 inv (
    .clk(clk), .reset_n(reset_n), .start(inv_start),
    .value(inv_value), .modulus(inv_mod),
    .busy(), .done(inv_done), .result(inv_result)
  );

  initial begin
    mul_start = 0;
    inv_start = 0;
    add_start = 0;
    add_subtract = 0;
    repeat (3) @(posedge clk);
    reset_n = 1;

    @(posedge clk);
    add_a=8;add_b=7;add_mod=13;add_subtract=0;add_start=1;
    @(posedge clk);add_start=0;
    wait(add_done);
    if(add_result!==2) $fatal(1,"modadd reduction failed: %x",add_result);

    @(posedge clk);
    add_a=2;add_b=7;add_mod=13;add_subtract=1;add_start=1;
    @(posedge clk);add_start=0;
    wait(add_done);
    if(add_result!==8) $fatal(1,"modsub correction failed: %x",add_result);

    @(posedge clk);
    mul_a = 256'd123456789;
    mul_b = 256'd987654321;
    mul_mod = SECP256K1_N;
    mul_start = 1;
    @(posedge clk);
    mul_start = 0;
    wait (mul_done);
    if (mul_result !== 256'd121932631112635269)
      $fatal(1, "modmul failed: %x", mul_result);

    @(posedge clk);
    inv_value = 256'd7;
    inv_mod = SECP256K1_N;
    inv_start = 1;
    @(posedge clk);
    inv_start = 0;
    wait (inv_done);

    @(posedge clk);
    mul_a = inv_result;
    mul_b = 256'd7;
    mul_mod = SECP256K1_N;
    mul_start = 1;
    @(posedge clk);
    mul_start = 0;
    wait (mul_done);
    if (mul_result !== 256'd1)
      $fatal(1, "modinv failed: %x", mul_result);

    $display("PASS: synthesizable modular multiplier and inverse");
    $finish;
  end
endmodule
