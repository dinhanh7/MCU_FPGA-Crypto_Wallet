`timescale 1ns/1ps

module tb_dual_coin_uart_signer;
  localparam integer CPB=4;
  localparam logic [255:0] ETH_HASH=
    256'h4f0cc815c8e4f9c9854ce1dd70a47d8f189509f1ec5664b8f30f57d16ee2be77;
  localparam logic ETH_Y_PARITY=1'b1;
  localparam logic [255:0] ETH_R=
    256'h47e0afbf026aff9eb2554348650d75fd7392773d9d3f204b20a56dba9ab875a0;
  localparam logic [255:0] ETH_S=
    256'h30b709805ba569f69bdab48d98cac0eafd0784ecb6ac17b491d1bb67f069ac47;

  localparam logic [495:0] BTC_OUTPUTS=496'h40787d0100000000160014c5be098348757c63d5a3ffd69f9f46797c53d64bd8e84f1c0000000016001473aa96b8e4dcd3e0c0b34500aeb61bfffd557f9d;
  localparam logic [255:0] BTC_DIGEST=256'h81a90ab2c9368b6e5d143ad2edbbbe325e1b2a0470bcfae2e9f6215ddbb1314c;
  localparam logic [255:0] BTC_R=256'hf59a0b6afa5e29acbcdcd4212942028e2a13766ce5d7a5f01a6f07f575ce8079;
  localparam logic [255:0] BTC_S=256'h20f3158b759b6b51540c251e3fa13edf78ff1e35a3bc3b3eba950597135fe0e7;
  localparam logic [31:0] BTC_TX_VERSION=32'h02000000;
  localparam logic [287:0] BTC_OUTPOINT=288'habab683dbf0fe3b0a7b8748e4f39888386240fc1b9ae5b251437f16cf276098301000000;
  localparam logic [31:0] BTC_SEQUENCE=32'hfdffffff;
  localparam logic [159:0] BTC_PUBKEY_HASH=160'h73aa96b8e4dcd3e0c0b34500aeb61bfffd557f9d;
  localparam logic [63:0] BTC_AMOUNT=64'h0065cd1d00000000;

  logic clk=0,reset_n=0,uart_rx=1;
  logic uart_tx,busy,last_success;
  logic [7:0] monitor_data,response[0:140];
  logic monitor_valid,monitor_error;
  integer count,total;
  longint unsigned cycles;
  always #5 clk=~clk;

  dual_coin_uart_signer_bridge #(
    .CLOCKS_PER_BIT(CPB),.PRIVATE_KEY(256'h1)
  ) dut (
    .clk(clk),.reset_n(reset_n),.uart_rx(uart_rx),.uart_tx(uart_tx),
    .busy(busy),.last_success(last_success)
  );
  uart_rx_8n1 #(.CLOCKS_PER_BIT(CPB)) monitor (
    .clk(clk),.reset_n(reset_n),.rx(uart_tx),.data(monitor_data),
    .data_valid(monitor_valid),.framing_error(monitor_error)
  );

  function automatic logic [15:0] crc_next(
    input logic [15:0] current,input logic [7:0] byte_value
  );
    logic [15:0] value;
    integer bit_index;
    begin
      value=current;
      for(bit_index=0;bit_index<8;bit_index=bit_index+1)
        value=(value[15]^byte_value[7-bit_index])?
              (value<<1)^16'h1021:value<<1;
      crc_next=value;
    end
  endfunction

  task automatic send_byte(input logic [7:0] byte_value);
    begin
      uart_rx=0;repeat(CPB)@(posedge clk);
      for(integer bit_index=0;bit_index<8;bit_index=bit_index+1) begin
        uart_rx=byte_value[bit_index];repeat(CPB)@(posedge clk);
      end
      uart_rx=1;repeat(CPB)@(posedge clk);
    end
  endtask

  task automatic send_crc_byte(
    input logic [7:0] byte_value,inout logic [15:0] crc
  );
    begin send_byte(byte_value);crc=crc_next(crc,byte_value);end
  endtask

  task automatic begin_request(
    input logic [7:0] sequence_id,input logic [7:0] command,
    input logic [15:0] length,inout logic [15:0] crc
  );
    begin
      crc=16'hffff;send_byte(8'ha5);send_byte(8'h5a);
      send_crc_byte(1,crc);send_crc_byte(sequence_id,crc);
      send_crc_byte(command,crc);send_crc_byte(length[15:8],crc);
      send_crc_byte(length[7:0],crc);
    end
  endtask

  task automatic finish_request(input logic [15:0] crc);
    begin send_byte(crc[15:8]);send_byte(crc[7:0]);end
  endtask

  task automatic send_eth_request;
    logic [15:0] crc;
    begin
      begin_request(8'h31,8'h01,16'd32,crc);
      for(integer index=0;index<32;index=index+1)
        send_crc_byte(ETH_HASH[255-index*8-:8],crc);
      finish_request(crc);
    end
  endtask

  task automatic send_btc_request;
    logic [15:0] crc;
    begin
      begin_request(8'h32,8'h10,16'd176,crc);
      for(integer index=0;index<32;index=index+1)send_crc_byte(index[7:0],crc);
      for(integer index=0;index<4;index=index+1)send_crc_byte(BTC_TX_VERSION[31-index*8-:8],crc);
      for(integer index=0;index<36;index=index+1)send_crc_byte(BTC_OUTPOINT[287-index*8-:8],crc);
      for(integer index=0;index<4;index=index+1)send_crc_byte(BTC_SEQUENCE[31-index*8-:8],crc);
      for(integer index=0;index<20;index=index+1)send_crc_byte(BTC_PUBKEY_HASH[159-index*8-:8],crc);
      for(integer index=0;index<8;index=index+1)send_crc_byte(BTC_AMOUNT[63-index*8-:8],crc);
      send_crc_byte(0,crc);send_crc_byte(8'h3e,crc);
      for(integer index=0;index<62;index=index+1)send_crc_byte(BTC_OUTPUTS[495-index*8-:8],crc);
      repeat(4)send_crc_byte(0,crc);
      send_crc_byte(1,crc);repeat(3)send_crc_byte(0,crc);
      finish_request(crc);
    end
  endtask

  task automatic receive_response;
    logic [15:0] crc;
    begin
      count=0;total=0;
      while(total==0||count<total) begin
        @(posedge clk);#1;
        if(monitor_error)$fatal(1,"UART framing error");
        if(monitor_valid) begin
          response[count]=monitor_data;count=count+1;
          if(count==8)total=10+{response[6],response[7]};
        end
      end
      crc=16'hffff;
      for(integer index=2;index<total-2;index=index+1)
        crc=crc_next(crc,response[index]);
      if({response[total-2],response[total-1]}!==crc)
        $fatal(1,"response CRC mismatch");
    end
  endtask

  task automatic check_eth_response;
    begin
      if(response[0]!==8'h5a||response[1]!==8'ha5||response[3]!==8'h31||
         response[4]!==8'h01||response[5]!==0||{response[6],response[7]}!==65)
        $fatal(1,"ETH response header mismatch");
      if(response[8]!=={7'b0,ETH_Y_PARITY})$fatal(1,"ETH yParity mismatch");
      for(integer index=0;index<32;index=index+1) begin
        if(response[9+index]!==ETH_R[255-index*8-:8])$fatal(1,"ETH r mismatch");
        if(response[41+index]!==ETH_S[255-index*8-:8])$fatal(1,"ETH s mismatch");
      end
    end
  endtask

  task automatic check_btc_response;
    begin
      if(response[0]!==8'h5a||response[1]!==8'ha5||response[3]!==8'h32||
         response[4]!==8'h10||response[5]!==0||{response[6],response[7]}!==128)
        $fatal(1,"BTC response header mismatch");
      for(integer index=0;index<32;index=index+1) begin
        if(response[8+index]!==index[7:0])$fatal(1,"BTC freeze mismatch");
        if(response[40+index]!==BTC_DIGEST[255-index*8-:8])$fatal(1,"BTC digest mismatch");
        if(response[72+index]!==BTC_R[255-index*8-:8])$fatal(1,"BTC r mismatch");
        if(response[104+index]!==BTC_S[255-index*8-:8])$fatal(1,"BTC s mismatch");
      end
    end
  endtask

  always @(posedge clk)if(reset_n)begin
    cycles<=cycles+1;
    if(cycles>160000000)$fatal(1,"dual signer timeout");
  end

  initial begin
    cycles=0;repeat(5)@(posedge clk);reset_n=1;
    send_eth_request();receive_response();check_eth_response();
    send_btc_request();receive_response();check_btc_response();
    if(!last_success)$fatal(1,"success LED not set");
    $display("PASS: one UART image signs ETH hash and BTC BIP143 with shared ECDSA core");
    $finish;
  end
endmodule
