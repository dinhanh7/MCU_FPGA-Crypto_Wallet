`timescale 1ns/1ps

module tb_btc_fpga_signer_core;
  localparam logic [495:0] OUTPUTS = 496'h40787d0100000000160014c5be098348757c63d5a3ffd69f9f46797c53d64bd8e84f1c0000000016001473aa96b8e4dcd3e0c0b34500aeb61bfffd557f9d;
  localparam logic [255:0] EXPECTED_DIGEST = 256'h81a90ab2c9368b6e5d143ad2edbbbe325e1b2a0470bcfae2e9f6215ddbb1314c;
  localparam logic [255:0] EXPECTED_R = 256'hf59a0b6afa5e29acbcdcd4212942028e2a13766ce5d7a5f01a6f07f575ce8079;
  localparam logic [255:0] EXPECTED_S = 256'h20f3158b759b6b51540c251e3fa13edf78ff1e35a3bc3b3eba950597135fe0e7;

  logic clk=0,reset_n=0,start=0,load_valid=0;
  logic [8:0] load_address;
  logic [7:0] load_byte;
  logic busy,done,error;
  logic [3:0] error_code;
  logic [255:0] digest,r,s;
  longint unsigned cycles;
  always #5 clk=~clk;

  btc_fpga_signer_core #(.PRIVATE_KEY(256'h1)) dut (
    .clk(clk),.reset_n(reset_n),.outputs_load_valid(load_valid),
    .outputs_load_address({1'b0,load_address}),.outputs_load_byte(load_byte),
    .request_length(10'b0),
    .buffer_read_enable(1'b0),.buffer_read_address(10'b0),
    .buffer_read_byte(),
    .start(start),.tx_version(32'h02000000),
    .outpoint(288'habab683dbf0fe3b0a7b8748e4f39888386240fc1b9ae5b251437f16cf276098301000000),
    .input_sequence(32'hfdffffff),
    .pubkey_hash(160'h73aa96b8e4dcd3e0c0b34500aeb61bfffd557f9d),
    .prevout_amount(64'h0065cd1d00000000),.outputs_length(10'd62),
    .locktime(32'h00000000),.sighash_type(32'h01000000),
    .busy(busy),.done(done),.error(error),.error_code(error_code),
    .bip143_digest(digest),.signature_r(r),.signature_s(s)
  );

  always @(posedge clk) if(reset_n) begin
    cycles<=cycles+1'b1;
    if(cycles>100000000)$fatal(1,"BTC FPGA signer timeout");
  end

  initial begin
    cycles=0;load_address=0;load_byte=0;
    repeat(5)@(posedge clk);reset_n=1;
    for(integer i=0;i<62;i=i+1) begin
      @(negedge clk);load_valid=1;load_address=i;load_byte=OUTPUTS[495-i*8-:8];
    end
    @(negedge clk);load_valid=0;start=1;
    @(negedge clk);start=0;
    wait(done);#1;
    if(error)$fatal(1,"signer error code %0d",error_code);
    if(digest!==EXPECTED_DIGEST)$fatal(1,"BIP143 digest mismatch: %064x",digest);
    if(r!==EXPECTED_R)$fatal(1,"signature r mismatch: %064x",r);
    if(s!==EXPECTED_S)$fatal(1,"signature s mismatch: %064x",s);
    $display("PASS: FPGA computes BIP143, signs and self-verifies in %0d cycles",cycles);
    $finish;
  end
endmodule
