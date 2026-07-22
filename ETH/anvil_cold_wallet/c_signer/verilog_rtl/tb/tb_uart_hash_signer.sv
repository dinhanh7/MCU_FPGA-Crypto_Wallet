`timescale 1ns/1ps
`include "private_key_sim.svh"

module tb_uart_hash_signer;
  localparam integer CLOCKS_PER_BIT = 8;
  localparam logic [255:0] EXPECTED_R =
    256'h3c403f14f95cc9624023f2dbd5dce5df2d502104e3ee4c766ffcdad17444ec3a;
  localparam logic [255:0] EXPECTED_S =
    256'h6a0829a2fae42ce7871a7ae448dd244b636dcfec370d6f3630a3d4b01404b090;
  logic clk=0,reset_n=0,uart_rx=1;
  always #5 clk=~clk;

  logic uart_tx,busy,last_success;
  logic [7:0] monitor_data;
  logic monitor_valid,monitor_error;
  logic [7:0] response_bytes [0:80];
  integer response_count;
  integer response_total;
  longint unsigned cycle_count;

  uart_hash_signer_bridge #(
    .CLOCKS_PER_BIT(CLOCKS_PER_BIT),
    .PRIVATE_KEY(`ETH_PRIVATE_KEY)
  ) dut (
    .clk(clk),.reset_n(reset_n),.uart_rx(uart_rx),.uart_tx(uart_tx),
    .busy(busy),.last_success(last_success)
  );

  uart_rx_8n1 #(.CLOCKS_PER_BIT(CLOCKS_PER_BIT)) monitor (
    .clk(clk),.reset_n(reset_n),.rx(uart_tx),.data(monitor_data),
    .data_valid(monitor_valid),.framing_error(monitor_error)
  );

  function automatic logic [15:0] crc16_next(
    input logic [15:0] current_crc,
    input logic [7:0] byte_value
  );
    logic [15:0] crc_value;
    integer bit_index;
    begin
      crc_value=current_crc;
      for(bit_index=0;bit_index<8;bit_index=bit_index+1) begin
        if(crc_value[15]^byte_value[7-bit_index])
          crc_value=(crc_value<<1)^16'h1021;
        else crc_value=crc_value<<1;
      end
      crc16_next=crc_value;
    end
  endfunction

  task automatic send_uart_byte(input logic [7:0] byte_value);
    begin
      uart_rx=1'b0;
      repeat(CLOCKS_PER_BIT) @(posedge clk);
      for(integer bit_index=0;bit_index<8;bit_index=bit_index+1) begin
        uart_rx=byte_value[bit_index];
        repeat(CLOCKS_PER_BIT) @(posedge clk);
      end
      uart_rx=1'b1;
      repeat(CLOCKS_PER_BIT) @(posedge clk);
    end
  endtask

  task automatic send_request(
    input logic [7:0] sequence_value,
    input logic [7:0] command_value,
    input logic [15:0] payload_length,
    input logic [255:0] hash_value,
    input logic corrupt_crc
  );
    logic [15:0] crc_value;
    logic [7:0] payload_byte;
    begin
      crc_value=16'hffff;
      send_uart_byte(8'ha5);
      send_uart_byte(8'h5a);
      send_uart_byte(8'h01);crc_value=crc16_next(crc_value,8'h01);
      send_uart_byte(sequence_value);crc_value=crc16_next(crc_value,sequence_value);
      send_uart_byte(command_value);crc_value=crc16_next(crc_value,command_value);
      send_uart_byte(payload_length[15:8]);crc_value=crc16_next(crc_value,payload_length[15:8]);
      send_uart_byte(payload_length[7:0]);crc_value=crc16_next(crc_value,payload_length[7:0]);
      for(integer payload_byte_index=0;payload_byte_index<payload_length;payload_byte_index=payload_byte_index+1) begin
        payload_byte=hash_value[255-payload_byte_index*8-:8];
        send_uart_byte(payload_byte);
        crc_value=crc16_next(crc_value,payload_byte);
      end
      send_uart_byte(crc_value[15:8]^{7'b0,corrupt_crc});
      send_uart_byte(crc_value[7:0]);
    end
  endtask

  task automatic receive_response;
    logic [15:0] crc_value;
    begin
      response_count=0;
      response_total=0;
      while(response_total==0 || response_count<response_total) begin
        @(posedge clk);#1;
        if(monitor_error)$fatal(1,"UART monitor framing error");
        if(monitor_valid) begin
          response_bytes[response_count]=monitor_data;
          response_count=response_count+1;
          if(response_count==8)
            response_total=10+{response_bytes[6],response_bytes[7]};
        end
      end
      if(response_bytes[0]!==8'h5a || response_bytes[1]!==8'ha5)
        $fatal(1,"response sync mismatch");
      crc_value=16'hffff;
      for(integer response_index=2;response_index<response_total-2;response_index=response_index+1)
        crc_value=crc16_next(crc_value,response_bytes[response_index]);
      if({response_bytes[response_total-2],response_bytes[response_total-1]}!==crc_value)
        $fatal(1,"response CRC mismatch");
    end
  endtask

  always @(posedge clk) begin
    if(!reset_n) cycle_count<=0;
    else begin
      cycle_count<=cycle_count+1'b1;
      if(cycle_count>180000000)$fatal(1,"UART signer timeout");
    end
  end

  initial begin
    repeat(5)@(posedge clk);reset_n=1;

    send_request(8'h11,8'h00,16'd0,256'h0,1'b0);
    receive_response();
    if(response_bytes[3]!==8'h11 || response_bytes[4]!==8'h00 ||
       response_bytes[5]!==8'h00 || {response_bytes[6],response_bytes[7]}!==16'd4 ||
       response_bytes[8]!==8'h50 || response_bytes[9]!==8'h4f ||
       response_bytes[10]!==8'h4e || response_bytes[11]!==8'h47)
      $fatal(1,"PING response mismatch");
    $display("PASS: UART PING");

    send_request(8'h12,8'h00,16'd0,256'h0,1'b1);
    receive_response();
    if(response_bytes[3]!==8'h12 || response_bytes[5]!==8'h01)
      $fatal(1,"CRC error response mismatch");
    $display("PASS: UART CRC rejection");

    send_request(
      8'h13,8'h01,16'd32,
      256'hb11a13b969e57b09787936eaac76c01e8989b68a17b2c18a93554c89d76912f7,
      1'b0
    );
    wait(busy);
    send_request(
      8'h14,8'h01,16'd32,
      256'hb11a13b969e57b09787936eaac76c01e8989b68a17b2c18a93554c89d76912f7,
      1'b0
    );
    receive_response();
    if(response_bytes[3]!==8'h14 || response_bytes[5]!==8'h04)
      $fatal(1,"busy response mismatch");
    $display("PASS: UART busy response");

    receive_response();
    if(response_bytes[3]!==8'h13 || response_bytes[4]!==8'h01 ||
       response_bytes[5]!==8'h00 || {response_bytes[6],response_bytes[7]}!==16'd65)
      $fatal(1,"signature response header mismatch");
    if(response_bytes[8]!==8'h00)$fatal(1,"signature parity mismatch");
    for(integer signature_index=0;signature_index<32;signature_index=signature_index+1) begin
      if(response_bytes[9+signature_index]!==EXPECTED_R[255-signature_index*8-:8])
        $fatal(1,"signature r mismatch at byte %0d",signature_index);
      if(response_bytes[41+signature_index]!==EXPECTED_S[255-signature_index*8-:8])
        $fatal(1,"signature s mismatch at byte %0d",signature_index);
    end
    if(!last_success)$fatal(1,"last_success was not set");
    $display("PASS: UART hash signer matches C/libsecp256k1");
    $finish;
  end
endmodule
