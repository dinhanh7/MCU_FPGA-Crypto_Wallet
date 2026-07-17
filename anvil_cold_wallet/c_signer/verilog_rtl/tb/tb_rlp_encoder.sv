`timescale 1ns/1ps

module tb_rlp_encoder;
  logic clk=0,reset_n=0,start,signed_mode,y_parity;
  always #5 clk=~clk;
  logic valid,ready,last,done;
  logic [7:0] out_byte,data_byte;
  logic [10:0] data_addr;
  logic [12:0] encoded_length;
  logic [7:0] captured[0:511];
  integer count;

  eip1559_rlp_encoder dut(
    .clk(clk),.reset_n(reset_n),.start(start),.signed_mode(signed_mode),
    .chain_id(31338),.nonce(3),.max_priority_fee_per_gas(1000000000),
    .max_fee_per_gas(1021491012),.gas_limit(21000),
    .recipient(160'h1d6d332f0ab9c6cfd95fac2ba2b8cefd39f012de),
    .value(2000000000000000000),.data_length(0),
    .data_read_address(data_addr),.data_read_byte(data_byte),
    .y_parity(y_parity),
    .signature_r(256'h3c403f14f95cc9624023f2dbd5dce5df2d502104e3ee4c766ffcdad17444ec3a),
    .signature_s(256'h6a0829a2fae42ce7871a7ae448dd244b636dcfec370d6f3630a3d4b01404b090),
    .out_valid(valid),.out_ready(ready),.out_byte(out_byte),.out_last(last),
    .busy(),.done(done),.encoded_length(encoded_length));
  assign data_byte=0;

  always @(posedge clk) if(valid&&ready) begin captured[count]<=out_byte; count<=count+1; end

  function automatic [3:0] hex_nibble(input [7:0] c);
    if(c>="0"&&c<="9") hex_nibble=c-"0";
    else if(c>="a"&&c<="f") hex_nibble=c-"a"+10;
    else hex_nibble=c-"A"+10;
  endfunction

  task automatic check_hex(input string expected);
    integer byte_count;
    reg [7:0] expected_byte;
    begin
      byte_count=(expected.len()-2)/2;
      if(count!=byte_count) $fatal(1,"length mismatch got=%0d expected=%0d",count,byte_count);
      for(integer i=0;i<byte_count;i=i+1) begin
        expected_byte={hex_nibble(expected[2+i*2]),hex_nibble(expected[3+i*2])};
        if(captured[i]!==expected_byte)
          $fatal(1,"byte %0d mismatch got=%02x expected=%02x",i,captured[i],expected_byte);
      end
    end
  endtask

  task automatic run_encoder(input logic mode);
    begin
      signed_mode=mode; count=0;
      @(posedge clk); start<=1;
      @(posedge clk); start<=0;
      wait(done); @(posedge clk);
    end
  endtask

  initial begin
    start=0;signed_mode=0;y_parity=0;ready=1;count=0;
    repeat(3) @(posedge clk);reset_n=1;
    run_encoder(0);
    check_hex("0x02f1827a6a03843b9aca00843ce2b744825208941d6d332f0ab9c6cfd95fac2ba2b8cefd39f012de881bc16d674ec8000080c0");
    run_encoder(1);
    check_hex("0x02f874827a6a03843b9aca00843ce2b744825208941d6d332f0ab9c6cfd95fac2ba2b8cefd39f012de881bc16d674ec8000080c080a03c403f14f95cc9624023f2dbd5dce5df2d502104e3ee4c766ffcdad17444ec3aa06a0829a2fae42ce7871a7ae448dd244b636dcfec370d6f3630a3d4b01404b090");
    $display("PASS: synthesizable EIP-1559 RLP stream encoder");
    $finish;
  end
endmodule
