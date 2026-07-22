`timescale 1ns/1ps

module tb_btc_uart_signer;
  localparam integer CPB=4;
  localparam logic [495:0] OUTPUTS=496'h40787d0100000000160014c5be098348757c63d5a3ffd69f9f46797c53d64bd8e84f1c0000000016001473aa96b8e4dcd3e0c0b34500aeb61bfffd557f9d;
  localparam logic [255:0] DIGEST=256'h81a90ab2c9368b6e5d143ad2edbbbe325e1b2a0470bcfae2e9f6215ddbb1314c;
  localparam logic [255:0] EXPECTED_R=256'hf59a0b6afa5e29acbcdcd4212942028e2a13766ce5d7a5f01a6f07f575ce8079;
  localparam logic [255:0] EXPECTED_S=256'h20f3158b759b6b51540c251e3fa13edf78ff1e35a3bc3b3eba950597135fe0e7;
  localparam logic [31:0] TX_VERSION=32'h02000000;
  localparam logic [287:0] OUTPOINT=288'habab683dbf0fe3b0a7b8748e4f39888386240fc1b9ae5b251437f16cf276098301000000;
  localparam logic [31:0] INPUT_SEQUENCE=32'hfdffffff;
  localparam logic [159:0] PUBKEY_HASH=160'h73aa96b8e4dcd3e0c0b34500aeb61bfffd557f9d;
  localparam logic [63:0] AMOUNT=64'h0065cd1d00000000;
  logic clk=0,reset_n=0,uart_rx=1;
  logic uart_tx,busy,last_success;
  logic [7:0] monitor_data,response[0:140];
  logic monitor_valid,monitor_error;
  integer count,total;
  longint unsigned cycles;
  always #5 clk=~clk;

  btc_uart_signer_bridge #(.CLOCKS_PER_BIT(CPB),.PRIVATE_KEY(256'h1)) dut(
    .clk(clk),.reset_n(reset_n),.uart_rx(uart_rx),.uart_tx(uart_tx),
    .busy(busy),.last_success(last_success));
  uart_rx_8n1 #(.CLOCKS_PER_BIT(CPB)) monitor(
    .clk(clk),.reset_n(reset_n),.rx(uart_tx),.data(monitor_data),
    .data_valid(monitor_valid),.framing_error(monitor_error));

  function automatic logic [15:0] crc_next(input logic [15:0] c,input logic [7:0] b);
    logic [15:0] v;integer i;
    begin v=c;for(i=0;i<8;i=i+1)v=(v[15]^b[7-i])?(v<<1)^16'h1021:v<<1;crc_next=v;end
  endfunction
  task automatic send_byte(input logic [7:0] b);
    begin uart_rx=0;repeat(CPB)@(posedge clk);for(integer i=0;i<8;i=i+1)begin
      uart_rx=b[i];repeat(CPB)@(posedge clk);end uart_rx=1;repeat(CPB)@(posedge clk);end
  endtask
  task automatic send_crc_byte(input logic [7:0] b,inout logic [15:0] crc);
    begin send_byte(b);crc=crc_next(crc,b);end
  endtask
  task automatic send_request;
    logic [15:0] crc;logic [7:0] b;
    begin
      crc=16'hffff;send_byte(8'ha5);send_byte(8'h5a);
      send_crc_byte(1,crc);send_crc_byte(8'h42,crc);send_crc_byte(8'h10,crc);
      send_crc_byte(0,crc);send_crc_byte(8'd176,crc);
      for(integer i=0;i<32;i=i+1)send_crc_byte(i[7:0],crc);
      for(integer i=0;i<4;i=i+1)send_crc_byte(TX_VERSION[31-i*8-:8],crc);
      for(integer i=0;i<36;i=i+1)send_crc_byte(OUTPOINT[287-i*8-:8],crc);
      for(integer i=0;i<4;i=i+1)send_crc_byte(INPUT_SEQUENCE[31-i*8-:8],crc);
      for(integer i=0;i<20;i=i+1)send_crc_byte(PUBKEY_HASH[159-i*8-:8],crc);
      for(integer i=0;i<8;i=i+1)send_crc_byte(AMOUNT[63-i*8-:8],crc);
      send_crc_byte(0,crc);send_crc_byte(8'h3e,crc);
      for(integer i=0;i<62;i=i+1)send_crc_byte(OUTPUTS[495-i*8-:8],crc);
      repeat(4)send_crc_byte(0,crc);
      send_crc_byte(1,crc);repeat(3)send_crc_byte(0,crc);
      send_byte(crc[15:8]);send_byte(crc[7:0]);
    end
  endtask
  task automatic receive_response;
    logic [15:0] crc;
    begin count=0;total=0;while(total==0||count<total)begin @(posedge clk);#1;
      if(monitor_error)$fatal(1,"UART framing error");if(monitor_valid)begin
        response[count]=monitor_data;count=count+1;if(count==8)total=10+{response[6],response[7]};end end
      crc=16'hffff;for(integer i=2;i<total-2;i=i+1)crc=crc_next(crc,response[i]);
      if({response[total-2],response[total-1]}!==crc)$fatal(1,"response CRC mismatch");
    end
  endtask

  always @(posedge clk)if(reset_n)begin cycles<=cycles+1;if(cycles>100000000)$fatal(1,"timeout");end
  initial begin
    cycles=0;repeat(5)@(posedge clk);reset_n=1;send_request();receive_response();
    if(response[0]!==8'h5a||response[1]!==8'ha5||response[3]!==8'h42||
       response[4]!==8'h10||response[5]!==0||{response[6],response[7]}!==128)
      $fatal(1,"response header mismatch");
    for(integer i=0;i<32;i=i+1)begin
      if(response[8+i]!==i[7:0])$fatal(1,"freeze mismatch");
      if(response[40+i]!==DIGEST[255-i*8-:8])$fatal(1,"digest mismatch");
      if(response[72+i]!==EXPECTED_R[255-i*8-:8])$fatal(1,"r mismatch");
      if(response[104+i]!==EXPECTED_S[255-i*8-:8])$fatal(1,"s mismatch");
    end
    if(!last_success)$fatal(1,"success LED not set");
    $display("PASS: SN34F788F protocol -> BIP143 -> verified ECDSA -> UART response");
    $finish;
  end
endmodule
