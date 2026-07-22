`timescale 1ns/1ps
`include "private_key_sim.svh"

module tb_eth_signer_core;
  logic clk=0,reset_n=0,start,done,busy,error;
  always #5 clk=~clk;
  logic [3:0] error_code;
  logic [159:0] address;
  logic [255:0] message_hash,r,s,transaction_hash;
  logic parity;
  logic [12:0] raw_length;
  logic raw_read_enable;
  logic [11:0] raw_read_address;
  logic [7:0] raw_read_byte;
  integer cycle_count;
  logic [5:0] previous_state;

  eth_signer_core #(.PRIVATE_KEY(`ETH_PRIVATE_KEY)) dut(
    .clk(clk),.reset_n(reset_n),.start(start),
    .chain_id(256'd31338),.nonce(256'd3),
    .max_priority_fee_per_gas(256'd1000000000),
    .max_fee_per_gas(256'd1021491012),.gas_limit(256'd21000),
    .recipient(160'h1d6d332f0ab9c6cfd95fac2ba2b8cefd39f012de),
    .value(256'd2000000000000000000),.data_length(12'd0),
    .data_write_enable(1'b0),.data_write_address(11'b0),.data_write_byte(8'b0),
    .busy(busy),.done(done),.error(error),.error_code(error_code),
    .signer_address(address),.message_hash(message_hash),.y_parity(parity),
    .signature_r(r),.signature_s(s),.transaction_hash(transaction_hash),
    .raw_transaction_length(raw_length),.raw_read_enable(raw_read_enable),
    .raw_read_address(raw_read_address),.raw_read_byte(raw_read_byte));

  always @(posedge clk) begin
    if(!reset_n) begin cycle_count<=0; previous_state<=0; end
    else begin
      cycle_count<=cycle_count+1;
      if(dut.state!==previous_state) begin
        $display("cycle=%0d top_state=%0d",cycle_count,dut.state);
        previous_state<=dut.state;
      end
      if((cycle_count%5000000)==0)
        $display("progress cycle=%0d top=%0d point_state=%0d bit=%0d",cycle_count,dut.state,dut.point_inst.high_state,dut.point_inst.bit_index);
      if(cycle_count>100000000)$fatal(1,"top-level watchdog, state=%0d",dut.state);
    end
  end

  function automatic [3:0] hex_nibble(input [7:0] c);
    if(c>="0"&&c<="9") hex_nibble=c-"0";
    else if(c>="a"&&c<="f") hex_nibble=c-"a"+10;
    else hex_nibble=c-"A"+10;
  endfunction

  task automatic check_raw(input string expected);
    integer byte_count;
    reg [7:0] expected_byte;
    begin
      byte_count=(expected.len()-2)/2;
      if(raw_length!=byte_count)$fatal(1,"raw length got=%0d expected=%0d",raw_length,byte_count);
      for(integer i=0;i<byte_count;i=i+1) begin
        raw_read_address=i;raw_read_enable=1;
        @(posedge clk); #1;
        expected_byte={hex_nibble(expected[2+i*2]),hex_nibble(expected[3+i*2])};
        if(raw_read_byte!==expected_byte)$fatal(1,"raw byte %0d got=%02x expected=%02x",i,raw_read_byte,expected_byte);
      end
      raw_read_enable=0;
    end
  endtask

  initial begin
    start=0;raw_read_enable=0;raw_read_address=0;
    repeat(3)@(posedge clk);reset_n=1;
    @(posedge clk);start<=1;
    @(posedge clk);start<=0;
    wait(done);@(posedge clk);
    if(error)$fatal(1,"signer error code %0d",error_code);
    if(address!==160'hc19558ded8d849a994a1d9a37a2d1575bba08b5b)$fatal(1,"address %x",address);
    if(message_hash!==256'hb11a13b969e57b09787936eaac76c01e8989b68a17b2c18a93554c89d76912f7)$fatal(1,"hash %x",message_hash);
    if(parity!==0)$fatal(1,"parity %0d",parity);
    if(r!==256'h3c403f14f95cc9624023f2dbd5dce5df2d502104e3ee4c766ffcdad17444ec3a)$fatal(1,"r %x",r);
    if(s!==256'h6a0829a2fae42ce7871a7ae448dd244b636dcfec370d6f3630a3d4b01404b090)$fatal(1,"s %x",s);
    if(transaction_hash!==256'h10374c522b0b831dd4a6f6d800f759f8bcdff8be899bea1f767e545e8ead5580)$fatal(1,"tx hash %x",transaction_hash);
    check_raw("0x02f874827a6a03843b9aca00843ce2b744825208941d6d332f0ab9c6cfd95fac2ba2b8cefd39f012de881bc16d674ec8000080c080a03c403f14f95cc9624023f2dbd5dce5df2d502104e3ee4c766ffcdad17444ec3aa06a0829a2fae42ce7871a7ae448dd244b636dcfec370d6f3630a3d4b01404b090");
    $display("PASS: complete synthesizable Ethereum signer matches C exactly");
    $finish;
  end
endmodule
