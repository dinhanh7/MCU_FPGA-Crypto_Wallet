`timescale 1ns/1ps

module modmul256 (
  input  logic         clk,
  input  logic         reset_n,
  input  logic         start,
  input  logic [255:0] operand_a,
  input  logic [255:0] operand_b,
  input  logic [255:0] modulus,
  output logic         busy,
  output logic         done,
  output logic [255:0] result
);
  logic add_start,add_done;
  logic [255:0] add_operand_a,add_operand_b,add_modulus,add_result;

  modaddsub256_seq modular_adder (
    .clk(clk),.reset_n(reset_n),.start(add_start),.subtract(1'b0),
    .operand_a(add_operand_a),.operand_b(add_operand_b),.modulus(add_modulus),
    .busy(),.done(add_done),.result(add_result)
  );

  modmul256_controller controller (
    .clk(clk),.reset_n(reset_n),.start(start),
    .operand_a(operand_a),.operand_b(operand_b),.modulus(modulus),
    .busy(busy),.done(done),.result(result),
    .add_start(add_start),.add_operand_a(add_operand_a),
    .add_operand_b(add_operand_b),.add_modulus(add_modulus),
    .add_done(add_done),.add_result(add_result)
  );
endmodule
