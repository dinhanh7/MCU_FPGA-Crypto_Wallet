`timescale 1ns/1ps
`include "private_key_sim.svh"

module tb_eth_hash_signer_core;
  logic clk=0,reset_n=0,start=0;
  always #5 clk=~clk;

  logic [255:0] message_hash;
  logic busy,done,error,y_parity;
  logic [3:0] error_code;
  logic [255:0] signature_r,signature_s;
  longint unsigned cycle_count;

  eth_hash_signer_core #(.PRIVATE_KEY(`ETH_PRIVATE_KEY)) dut (
    .clk(clk),.reset_n(reset_n),.start(start),.message_hash(message_hash),
    .busy(busy),.done(done),.error(error),.error_code(error_code),
    .y_parity(y_parity),.signature_r(signature_r),.signature_s(signature_s)
  );

  always @(posedge clk) begin
    if(!reset_n) cycle_count<=0;
    else begin
      cycle_count<=cycle_count+1'b1;
      if(cycle_count>160000000)
        $fatal(1,"hash signer regression timeout");
    end
  end

  task automatic run_case(
    input integer case_index,
    input logic [255:0] hash_value,
    input logic expected_parity,
    input logic [255:0] expected_r,
    input logic [255:0] expected_s
  );
    begin
      message_hash=hash_value;
      @(negedge clk);start=1;
      @(negedge clk);start=0;
      wait(done);
      @(negedge clk);
      if(error)$fatal(1,"case %0d error code %0d",case_index,error_code);
      if(y_parity!==expected_parity)$fatal(1,"case %0d parity %0d",case_index,y_parity);
      if(signature_r!==expected_r)$fatal(1,"case %0d r %x",case_index,signature_r);
      if(signature_s!==expected_s)$fatal(1,"case %0d s %x",case_index,signature_s);
      $display("PASS: hash signer case %0d",case_index);
    end
  endtask

  initial begin
    message_hash=0;
    repeat(3)@(posedge clk);reset_n=1;

    if($value$plusargs("MESSAGE_HASH=%h",message_hash))begin
      @(negedge clk);start=1;
      @(negedge clk);start=0;
      wait(done);
      @(negedge clk);
      if(error)$fatal(1,"hash signer CLI error code %0d",error_code);
      $display(
        "RTL_RESULT {\"messageHash\":\"0x%064x\",\"yParity\":%0d,\"r\":\"0x%064x\",\"s\":\"0x%064x\"}",
        message_hash,y_parity,signature_r,signature_s
      );
      $finish;
    end

    run_case(
      1,
      256'hb11a13b969e57b09787936eaac76c01e8989b68a17b2c18a93554c89d76912f7,
      1'b0,
      256'h3c403f14f95cc9624023f2dbd5dce5df2d502104e3ee4c766ffcdad17444ec3a,
      256'h6a0829a2fae42ce7871a7ae448dd244b636dcfec370d6f3630a3d4b01404b090
    );
    run_case(
      2,
      256'h0,
      1'b0,
      256'he6fecfd3ff72185dba6e0528ea228b77e30e346a923ad44fa928217babd9beab,
      256'h166689179545e0d9be84a3be39b111b874a7de2834989e20c9e02c2540433a48
    );
    run_case(
      3,
      256'hfffffffffffffffffffffffffffffffebaaedce6af48a03bbfd25e8cd0364141,
      1'b0,
      256'he6fecfd3ff72185dba6e0528ea228b77e30e346a923ad44fa928217babd9beab,
      256'h166689179545e0d9be84a3be39b111b874a7de2834989e20c9e02c2540433a48
    );
    run_case(
      4,
      256'hffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff,
      1'b0,
      256'h6f178a7aed847eaf42559659b99b62b8b6b1d23308ef9f35b1235b21782d69a4,
      256'h7552b856a3b4acb2db6b8fe74932a52d7a5181d46e3ee741799246a269d94406
    );

    $display("PASS: hash signer matches C/libsecp256k1 for all vectors");
    $finish;
  end
endmodule
