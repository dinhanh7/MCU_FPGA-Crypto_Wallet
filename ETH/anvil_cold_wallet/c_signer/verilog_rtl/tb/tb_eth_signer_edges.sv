`timescale 1ns/1ps
`include "private_key_sim.svh"

module tb_eth_signer_edges;
  logic clk=0,reset_n=0,start=0;
  always #5 clk=~clk;

  logic [255:0] chain_id,nonce,max_priority_fee_per_gas,max_fee_per_gas;
  logic [255:0] gas_limit,value;
  logic [159:0] recipient;
  logic [11:0] data_length;
  logic data_write_enable;
  logic [10:0] data_write_address;
  logic [7:0] data_write_byte;
  logic busy,done,error;
  logic [3:0] error_code;
  logic [255:0] message_hash,signature_r,signature_s,transaction_hash;
  logic y_parity;
  logic [12:0] raw_transaction_length;
  longint unsigned cycle_count;

  eth_signer_core #(
    .PRIVATE_KEY(`ETH_PRIVATE_KEY),
    .COMPUTE_SIGNER_ADDRESS(1'b0)
  ) dut (
    .clk(clk),.reset_n(reset_n),.start(start),
    .chain_id(chain_id),.nonce(nonce),
    .max_priority_fee_per_gas(max_priority_fee_per_gas),
    .max_fee_per_gas(max_fee_per_gas),.gas_limit(gas_limit),
    .recipient(recipient),.value(value),.data_length(data_length),
    .data_write_enable(data_write_enable),
    .data_write_address(data_write_address),.data_write_byte(data_write_byte),
    .busy(busy),.done(done),.error(error),.error_code(error_code),
    .signer_address(),.message_hash(message_hash),.y_parity(y_parity),
    .signature_r(signature_r),.signature_s(signature_s),
    .transaction_hash(transaction_hash),
    .raw_transaction_length(raw_transaction_length),
    .raw_read_enable(1'b0),.raw_read_address(12'b0),.raw_read_byte()
  );

  always @(posedge clk) begin
    if(!reset_n) cycle_count<=0;
    else begin
      cycle_count<=cycle_count+1'b1;
      if(cycle_count>150000000)
        $fatal(1,"edge regression timeout");
    end
  end

  task automatic load_incrementing_data(input integer length);
    begin
      data_write_enable=0;
      for(integer index=0;index<length;index=index+1) begin
        @(negedge clk);
        data_write_enable=1;
        data_write_address=index[10:0];
        data_write_byte=index[7:0];
      end
      @(negedge clk);
      data_write_enable=0;
    end
  endtask

  task automatic run_case(
    input integer case_index,
    input logic [255:0] expected_message_hash,
    input logic expected_y_parity,
    input logic [255:0] expected_r,
    input logic [255:0] expected_s,
    input logic [255:0] expected_transaction_hash,
    input logic [12:0] expected_raw_length
  );
    begin
      @(negedge clk);start=1;
      @(negedge clk);start=0;
      wait(done);
      @(negedge clk);
      if(error)$fatal(1,"case %0d error code %0d",case_index,error_code);
      if(message_hash!==expected_message_hash)
        $fatal(1,"case %0d message hash %x",case_index,message_hash);
      if(y_parity!==expected_y_parity)
        $fatal(1,"case %0d parity %0d",case_index,y_parity);
      if(signature_r!==expected_r)
        $fatal(1,"case %0d r %x",case_index,signature_r);
      if(signature_s!==expected_s)
        $fatal(1,"case %0d s %x",case_index,signature_s);
      if(transaction_hash!==expected_transaction_hash)
        $fatal(1,"case %0d transaction hash %x",case_index,transaction_hash);
      if(raw_transaction_length!==expected_raw_length)
        $fatal(1,"case %0d raw length %0d",case_index,raw_transaction_length);
      $display("PASS: edge transaction %0d",case_index);
    end
  endtask

  initial begin
    chain_id=0;nonce=0;max_priority_fee_per_gas=0;max_fee_per_gas=0;
    gas_limit=0;recipient=0;value=0;data_length=0;
    data_write_enable=0;data_write_address=0;data_write_byte=0;
    repeat(3)@(posedge clk);reset_n=1;

    data_length=1;
    load_incrementing_data(1);
    run_case(
      1,
      256'hbcec0918e6458443624163aaf18c7e558860bc97e64cb02c03780a47ba62ba21,
      1'b0,
      256'h10932b7b27eba70cebead7fb8139871a207a1413b310c470ccd7125ec89b3854,
      256'h4cd57c957b5a5b0ca6afebc1c0f394a4c9cb65229f4dafc54a2de6d23cd58c78,
      256'h55ed5b52897fb9e46da22794b843a596b0df8247fb16ad972b6ac83b84e71c74,
      13'd99
    );

    chain_id={256{1'b1}};nonce={256{1'b1}};
    max_priority_fee_per_gas={256{1'b1}};max_fee_per_gas={256{1'b1}};
    gas_limit={256{1'b1}};recipient={160{1'b1}};value={256{1'b1}};
    data_length=56;
    load_incrementing_data(56);
    run_case(
      2,
      256'h099ef841f0ebfb6a8336aaba954b0cbc1f90eb3a57a3771d8b68dc7a37563bb3,
      1'b1,
      256'h5de42ab0559ffa8b91cb7bf49aeaf89b8ec09d4b13ceef76ddb217f03378a792,
      256'h200465ab16c24877ae7c6886509f077b3330f6fc1d3ed1a60a224f88562a4ba2,
      256'h20bcb4d277631b9fda94e50037df3f3230d534525448bc1e7debbf1520a99bfd,
      13'd349
    );

    chain_id=31338;nonce=9;max_priority_fee_per_gas=1;max_fee_per_gas=2;
    gas_limit=5000000;
    recipient=160'h0123456789abcdef0123456789abcdef01234567;
    value=127;data_length=2048;
    load_incrementing_data(2048);
    run_case(
      3,
      256'h509a04626d96ac233d38c16c4ea5b966bfb4a4c9cdf9349abbccc2bad38189af,
      1'b1,
      256'h6b119d988b45a858dc63d8d12147c614d123b795746846f7d1cd17037d88e5ff,
      256'h772c08a3b7b02755ac90965e56324f08e2a272b82979a2698a2384aeff85284d,
      256'h77263c3816154939d30f06ae8e48243171d3ba1bdc6d515d61223b5d0f696a44,
      13'd2155
    );

    data_length=2049;
    @(negedge clk);start=1;
    @(negedge clk);start=0;
    wait(done);
    @(negedge clk);
    if(!error || error_code!==4'd2 || busy)
      $fatal(1,"oversize calldata check failed error=%0d code=%0d busy=%0d",error,error_code,busy);

    $display("PASS: RTL edge transactions match C and reject oversized calldata");
    $finish;
  end
endmodule
