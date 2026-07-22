`timescale 1ns/1ps

module uart_hash_signer_bridge #(
  parameter integer CLOCKS_PER_BIT = 434,
  parameter logic [255:0] PRIVATE_KEY = 256'h1
) (
  input  logic clk,
  input  logic reset_n,
  input  logic uart_rx,
  output logic uart_tx,
  output logic busy,
  output logic last_success
);
  localparam logic [7:0] PROTOCOL_VERSION = 8'h01;
  localparam logic [7:0] COMMAND_PING = 8'h00;
  localparam logic [7:0] COMMAND_SIGN_HASH = 8'h01;
  localparam logic [7:0] STATUS_OK = 8'h00;
  localparam logic [7:0] STATUS_CRC_ERROR = 8'h01;
  localparam logic [7:0] STATUS_LENGTH_ERROR = 8'h02;
  localparam logic [7:0] STATUS_COMMAND_ERROR = 8'h03;
  localparam logic [7:0] STATUS_BUSY = 8'h04;
  localparam logic [7:0] STATUS_PRIVATE_KEY_ERROR = 8'h05;
  localparam logic [7:0] STATUS_RECOVERY_ERROR = 8'h06;

  typedef enum logic [3:0] {
    PARSE_SYNC_A5,PARSE_SYNC_5A,PARSE_VERSION,PARSE_SEQUENCE,
    PARSE_COMMAND,PARSE_LENGTH_HIGH,PARSE_LENGTH_LOW,PARSE_PAYLOAD,
    PARSE_CRC_HIGH,PARSE_CRC_LOW
  } parser_state_t;

  parser_state_t parser_state;
  logic [7:0] rx_data;
  logic rx_valid,rx_framing_error;
  logic [15:0] request_crc;
  logic [7:0] request_crc_high;
  logic [7:0] request_version,request_sequence,request_command;
  logic [7:0] request_length_high;
  logic [15:0] request_length,payload_index;
  logic [255:0] message_hash_buffer;

  logic tx_start,tx_busy,tx_done;
  logic [7:0] tx_data;
  logic response_active;
  logic [8:0] response_index;
  logic [15:0] response_payload_length,response_crc;
  logic [7:0] response_sequence,response_command,response_status;
  logic [7:0] response_payload [0:64];
  logic [7:0] response_next_byte;

  logic signer_start,signer_busy,signer_done,signer_error;
  logic [3:0] signer_error_code;
  logic signer_y_parity;
  logic [255:0] signer_r,signer_s;
  logic sign_operation_active;
  logic [7:0] sign_sequence;

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

  uart_rx_8n1 #(.CLOCKS_PER_BIT(CLOCKS_PER_BIT)) receiver (
    .clk(clk),.reset_n(reset_n),.rx(uart_rx),.data(rx_data),
    .data_valid(rx_valid),.framing_error(rx_framing_error)
  );

  uart_tx_8n1 #(.CLOCKS_PER_BIT(CLOCKS_PER_BIT)) transmitter (
    .clk(clk),.reset_n(reset_n),.start(tx_start),.data(tx_data),
    .tx(uart_tx),.busy(tx_busy),.done(tx_done)
  );

  eth_hash_signer_core #(.PRIVATE_KEY(PRIVATE_KEY)) signer (
    .clk(clk),.reset_n(reset_n),.start(signer_start),
    .message_hash(message_hash_buffer),.busy(signer_busy),.done(signer_done),
    .error(signer_error),.error_code(signer_error_code),
    .y_parity(signer_y_parity),.signature_r(signer_r),.signature_s(signer_s)
  );

  assign busy=sign_operation_active|signer_busy;

  always_comb begin
    response_next_byte=8'h00;
    case(response_index)
      0: response_next_byte=8'h5a;
      1: response_next_byte=8'ha5;
      2: response_next_byte=PROTOCOL_VERSION;
      3: response_next_byte=response_sequence;
      4: response_next_byte=response_command;
      5: response_next_byte=response_status;
      6: response_next_byte=response_payload_length[15:8];
      7: response_next_byte=response_payload_length[7:0];
      default: begin
        if(response_index<8+response_payload_length)
          response_next_byte=response_payload[response_index-8];
        else if(response_index==8+response_payload_length)
          response_next_byte=response_crc[15:8];
        else response_next_byte=response_crc[7:0];
      end
    endcase
  end

  always_ff @(posedge clk or negedge reset_n) begin
    if(!reset_n) begin
      parser_state<=PARSE_SYNC_A5;
      request_crc<=16'hffff;
      request_crc_high<=0;
      request_version<=0;
      request_sequence<=0;
      request_command<=0;
      request_length_high<=0;
      request_length<=0;
      payload_index<=0;
      message_hash_buffer<=0;
      tx_start<=0;
      tx_data<=0;
      response_active<=0;
      response_index<=0;
      response_payload_length<=0;
      response_crc<=16'hffff;
      response_sequence<=0;
      response_command<=0;
      response_status<=0;
      signer_start<=0;
      sign_operation_active<=0;
      sign_sequence<=0;
      last_success<=0;
    end else begin
      tx_start<=1'b0;
      signer_start<=1'b0;

      if(response_active && !tx_busy && !tx_start) begin
        tx_data<=response_next_byte;
        tx_start<=1'b1;
        if(response_index>=2 && response_index<8+response_payload_length)
          response_crc<=crc16_next(response_crc,response_next_byte);
        if(response_index==9+response_payload_length) begin
          response_active<=1'b0;
          response_index<=0;
        end else response_index<=response_index+1'b1;
      end

      if(rx_framing_error) parser_state<=PARSE_SYNC_A5;
      else if(rx_valid) begin
        case(parser_state)
          PARSE_SYNC_A5: if(rx_data==8'ha5) parser_state<=PARSE_SYNC_5A;

          PARSE_SYNC_5A: begin
            if(rx_data==8'h5a) begin
              request_crc<=16'hffff;
              payload_index<=0;
              message_hash_buffer<=0;
              parser_state<=PARSE_VERSION;
            end else if(rx_data!=8'ha5) parser_state<=PARSE_SYNC_A5;
          end

          PARSE_VERSION: begin
            request_version<=rx_data;
            request_crc<=crc16_next(request_crc,rx_data);
            parser_state<=PARSE_SEQUENCE;
          end

          PARSE_SEQUENCE: begin
            request_sequence<=rx_data;
            request_crc<=crc16_next(request_crc,rx_data);
            parser_state<=PARSE_COMMAND;
          end

          PARSE_COMMAND: begin
            request_command<=rx_data;
            request_crc<=crc16_next(request_crc,rx_data);
            parser_state<=PARSE_LENGTH_HIGH;
          end

          PARSE_LENGTH_HIGH: begin
            request_length_high<=rx_data;
            request_crc<=crc16_next(request_crc,rx_data);
            parser_state<=PARSE_LENGTH_LOW;
          end

          PARSE_LENGTH_LOW: begin
            request_length<={request_length_high,rx_data};
            request_crc<=crc16_next(request_crc,rx_data);
            payload_index<=0;
            message_hash_buffer<=0;
            if({request_length_high,rx_data}>16'd32) begin
              if(!response_active) begin
                response_active<=1'b1;
                response_index<=0;
                response_crc<=16'hffff;
                response_sequence<=request_sequence;
                response_command<=request_command;
                response_status<=STATUS_LENGTH_ERROR;
                response_payload_length<=0;
              end
              parser_state<=PARSE_SYNC_A5;
            end else if({request_length_high,rx_data}==0)
              parser_state<=PARSE_CRC_HIGH;
            else parser_state<=PARSE_PAYLOAD;
          end

          PARSE_PAYLOAD: begin
            message_hash_buffer<={message_hash_buffer[247:0],rx_data};
            request_crc<=crc16_next(request_crc,rx_data);
            if(payload_index+1'b1==request_length) begin
              payload_index<=0;
              parser_state<=PARSE_CRC_HIGH;
            end else payload_index<=payload_index+1'b1;
          end

          PARSE_CRC_HIGH: begin
            request_crc_high<=rx_data;
            parser_state<=PARSE_CRC_LOW;
          end

          PARSE_CRC_LOW: begin
            parser_state<=PARSE_SYNC_A5;
            if({request_crc_high,rx_data}!=request_crc) begin
              if(!response_active) begin
                response_active<=1'b1;
                response_index<=0;
                response_crc<=16'hffff;
                response_sequence<=request_sequence;
                response_command<=request_command;
                response_status<=STATUS_CRC_ERROR;
                response_payload_length<=0;
              end
            end else if(request_version!=PROTOCOL_VERSION) begin
              if(!response_active) begin
                response_active<=1'b1;
                response_index<=0;
                response_crc<=16'hffff;
                response_sequence<=request_sequence;
                response_command<=request_command;
                response_status<=STATUS_COMMAND_ERROR;
                response_payload_length<=0;
              end
            end else if(request_command==COMMAND_PING) begin
              if(!response_active) begin
                response_payload[0]<=8'h50;
                response_payload[1]<=8'h4f;
                response_payload[2]<=8'h4e;
                response_payload[3]<=8'h47;
                response_active<=1'b1;
                response_index<=0;
                response_crc<=16'hffff;
                response_sequence<=request_sequence;
                response_command<=request_command;
                response_status<=(request_length==0)?STATUS_OK:STATUS_LENGTH_ERROR;
                response_payload_length<=(request_length==0)?16'd4:16'd0;
              end
            end else if(request_command==COMMAND_SIGN_HASH) begin
              if(request_length!=16'd32) begin
                if(!response_active) begin
                  response_active<=1'b1;
                  response_index<=0;
                  response_crc<=16'hffff;
                  response_sequence<=request_sequence;
                  response_command<=request_command;
                  response_status<=STATUS_LENGTH_ERROR;
                  response_payload_length<=0;
                end
              end else if(sign_operation_active || signer_busy) begin
                if(!response_active) begin
                  response_active<=1'b1;
                  response_index<=0;
                  response_crc<=16'hffff;
                  response_sequence<=request_sequence;
                  response_command<=request_command;
                  response_status<=STATUS_BUSY;
                  response_payload_length<=0;
                end
              end else begin
                sign_sequence<=request_sequence;
                sign_operation_active<=1'b1;
                signer_start<=1'b1;
              end
            end else if(!response_active) begin
              response_active<=1'b1;
              response_index<=0;
              response_crc<=16'hffff;
              response_sequence<=request_sequence;
              response_command<=request_command;
              response_status<=STATUS_COMMAND_ERROR;
              response_payload_length<=0;
            end
          end

          default: parser_state<=PARSE_SYNC_A5;
        endcase
      end

      if(signer_done && sign_operation_active && !response_active) begin
        response_active<=1'b1;
        response_index<=0;
        response_crc<=16'hffff;
        response_sequence<=sign_sequence;
        response_command<=COMMAND_SIGN_HASH;
        sign_operation_active<=1'b0;
        if(signer_error) begin
          response_status<=(signer_error_code==1)?
            STATUS_PRIVATE_KEY_ERROR:STATUS_RECOVERY_ERROR;
          response_payload_length<=0;
          last_success<=1'b0;
        end else begin
          response_status<=STATUS_OK;
          response_payload_length<=16'd65;
          response_payload[0]<={7'b0,signer_y_parity};
          for(integer signature_index=0;signature_index<32;signature_index=signature_index+1) begin
            response_payload[1+signature_index]<=signer_r[255-signature_index*8-:8];
            response_payload[33+signature_index]<=signer_s[255-signature_index*8-:8];
          end
          last_success<=1'b1;
        end
      end
    end
  end
endmodule
