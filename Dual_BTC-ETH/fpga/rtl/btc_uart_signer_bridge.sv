`timescale 1ns/1ps

module btc_uart_signer_bridge #(
  parameter integer CLOCKS_PER_BIT = 434,
  parameter logic [255:0] PRIVATE_KEY = 256'h1,
  parameter integer MAX_OUTPUT_BYTES = 512
) (
  input  logic clk,
  input  logic reset_n,
  input  logic uart_rx,
  output logic uart_tx,
  output logic busy,
  output logic last_success
);
  localparam logic [7:0] VERSION=8'h01,COMMAND_PING=8'h00;
  localparam logic [7:0] COMMAND_SIGN_ETH=8'h01,COMMAND_SIGN_BTC=8'h10;
  localparam logic [7:0] STATUS_OK=8'h00,STATUS_CRC=8'h01,STATUS_LENGTH=8'h02;
  localparam logic [7:0] STATUS_COMMAND=8'h03,STATUS_BUSY=8'h04,STATUS_KEY=8'h05;
  localparam logic [7:0] STATUS_BIP143=8'h06,STATUS_VERIFY=8'h07;
  localparam logic [7:0] STATUS_ETH_RECOVERY=8'h06;
  localparam integer MAX_PAYLOAD=114+MAX_OUTPUT_BYTES;

  typedef enum logic [3:0] {
    P_SYNC_A5,P_SYNC_5A,P_VERSION,P_SEQUENCE,P_COMMAND,P_LEN_HI,P_LEN_LO,
    P_PAYLOAD,P_CRC_HI,P_CRC_LO
  } parser_state_t;
  parser_state_t parser_state;

  logic [7:0] rx_data;
  logic rx_valid,rx_framing_error;
  logic [15:0] request_crc;
  logic [7:0] request_crc_hi,request_version,request_sequence,request_command;
  logic [7:0] request_len_hi;
  logic [15:0] request_length,payload_index,declared_outputs_length;
  logic [7:0] declared_outputs_hi;

  logic [31:0] sighash_buffer;
  logic output_load_valid;
  logic [9:0] output_load_address;
  logic [7:0] output_load_byte;
  logic request_buffer_read_enable;
  logic [9:0] request_buffer_read_address;
  logic [7:0] request_buffer_read_byte;

  logic signer_start,signer_busy,signer_done,signer_error;
  logic [3:0] signer_error_code;
  logic [255:0] signer_digest,signer_r,signer_s;
  logic signer_y_parity,signer_recovery_high,sign_is_eth;
  logic sign_operation_active,sign_result_pending;
  logic [7:0] sign_sequence;

  logic tx_start,tx_busy,tx_done;
  logic [7:0] tx_data;
  logic response_active;
  logic [8:0] response_index;
  logic [15:0] response_payload_length,response_crc;
  logic [7:0] response_sequence,response_command,response_status,response_next_byte;
  // Serialize one 32-bit word at a time.  Loading a statically selected word
  // every four UART bytes is substantially smaller than a 256-bit shifter.
  logic [31:0] response_word_shift;
  logic [31:0] response_next_digest_word,response_next_signature_word;
  logic [4:0] response_next_word_index;
  logic [8:0] response_signature_start,response_signature_offset;

  function automatic logic [15:0] crc16_next(
    input logic [15:0] current_crc,input logic [7:0] byte_value
  );
    logic [15:0] value;
    integer bit_index;
    begin
      value=current_crc;
      for(bit_index=0;bit_index<8;bit_index=bit_index+1)
        value=(value[15]^byte_value[7-bit_index]) ?
              (value<<1)^16'h1021 : value<<1;
      crc16_next=value;
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
  btc_fpga_signer_core #(
    .PRIVATE_KEY(PRIVATE_KEY),.MAX_OUTPUT_BYTES(MAX_OUTPUT_BYTES),
    .SERIALIZED_REQUEST(1'b1)
  ) signer (
    .clk(clk),.reset_n(reset_n),
    .outputs_load_valid(output_load_valid),
    .outputs_load_address(output_load_address),
    .outputs_load_byte(output_load_byte),
    .request_length(request_length[9:0]),.start(signer_start),
    .buffer_read_enable(request_buffer_read_enable),
    .buffer_read_address(request_buffer_read_address),
    .buffer_read_byte(request_buffer_read_byte),
    .direct_hash_mode(sign_is_eth),
    .direct_hash_load_valid(rx_valid && parser_state==P_PAYLOAD &&
      request_command==COMMAND_SIGN_ETH && !sign_operation_active && !signer_busy),
    .direct_hash_load_byte(rx_data),
    .tx_version(32'b0),.outpoint(288'b0),
    .input_sequence(32'b0),.pubkey_hash(160'b0),
    .prevout_amount(64'b0),.outputs_length(declared_outputs_length[9:0]),
    .locktime(32'b0),.sighash_type(sighash_buffer),
    .busy(signer_busy),.done(signer_done),.error(signer_error),
    .error_code(signer_error_code),.bip143_digest(signer_digest),
    .y_parity(signer_y_parity),.recovery_high(signer_recovery_high),
    .signature_r(signer_r),.signature_s(signer_s)
  );

  assign busy=sign_operation_active|signer_busy;
  always_comb begin
    request_buffer_read_enable=response_active &&
      response_command==COMMAND_SIGN_BTC && response_status==STATUS_OK;
    request_buffer_read_address=10'd0;
    if(response_index>=9'd8 && response_index<9'd40)
      request_buffer_read_address=response_index-9'd8;

    // Both protocols share exactly the same r/s serializer.  Only the byte
    // at which the signature begins differs (ETH=9, BTC=72).
    response_signature_start=(response_command==COMMAND_SIGN_ETH)?9'd9:9'd72;
    response_signature_offset=response_index-response_signature_start;
    response_next_word_index=(response_signature_offset>>2)+1'b1;
    response_next_signature_word=32'b0;
    case(response_next_word_index)
       1:response_next_signature_word=signer_r[223:192];
       2:response_next_signature_word=signer_r[191:160];
       3:response_next_signature_word=signer_r[159:128];
       4:response_next_signature_word=signer_r[127:96];
       5:response_next_signature_word=signer_r[95:64];
       6:response_next_signature_word=signer_r[63:32];
       7:response_next_signature_word=signer_r[31:0];
       8:response_next_signature_word=signer_s[255:224];
       9:response_next_signature_word=signer_s[223:192];
      10:response_next_signature_word=signer_s[191:160];
      11:response_next_signature_word=signer_s[159:128];
      12:response_next_signature_word=signer_s[127:96];
      13:response_next_signature_word=signer_s[95:64];
      14:response_next_signature_word=signer_s[63:32];
      15:response_next_signature_word=signer_s[31:0];
      default:response_next_signature_word=32'b0;
    endcase
    response_next_digest_word=32'b0;
    case(((response_index-9'd40)>>2)+1'b1)
      1:response_next_digest_word=signer_digest[223:192];
      2:response_next_digest_word=signer_digest[191:160];
      3:response_next_digest_word=signer_digest[159:128];
      4:response_next_digest_word=signer_digest[127:96];
      5:response_next_digest_word=signer_digest[95:64];
      6:response_next_digest_word=signer_digest[63:32];
      7:response_next_digest_word=signer_digest[31:0];
      default:response_next_digest_word=32'b0;
    endcase

    response_next_byte=8'h00;
    case(response_index)
      0:response_next_byte=8'h5a;
      1:response_next_byte=8'ha5;
      2:response_next_byte=VERSION;
      3:response_next_byte=response_sequence;
      4:response_next_byte=response_command;
      5:response_next_byte=response_status;
      6:response_next_byte=response_payload_length[15:8];
      7:response_next_byte=response_payload_length[7:0];
      default:begin
        if(response_index<8+response_payload_length) begin
          if(response_command==COMMAND_PING)
            case(response_index-8) 0:response_next_byte="P";1:response_next_byte="O";
              2:response_next_byte="N";default:response_next_byte="G";endcase
          else if(response_command==COMMAND_SIGN_ETH && response_index==9'd8)
            response_next_byte={7'b0,signer_y_parity};
          else if(response_command==COMMAND_SIGN_BTC && response_index<9'd40)
            response_next_byte=request_buffer_read_byte;
          else
            response_next_byte=response_word_shift[31:24];
        end else if(response_index==8+response_payload_length)
          response_next_byte=response_crc[15:8];
        else response_next_byte=response_crc[7:0];
      end
    endcase
  end

  task automatic begin_response(
    input logic [7:0] seq,input logic [7:0] command,
    input logic [7:0] status,input logic [15:0] length
  );
    begin
      response_active<=1;response_index<=0;response_crc<=16'hffff;
      response_sequence<=seq;response_command<=command;
      response_status<=status;response_payload_length<=length;
    end
  endtask

  always_ff @(posedge clk or negedge reset_n) begin
    if(!reset_n) begin
      parser_state<=P_SYNC_A5;request_crc<=16'hffff;request_crc_hi<=0;
      request_version<=0;request_sequence<=0;request_command<=0;
      request_len_hi<=0;request_length<=0;payload_index<=0;
      declared_outputs_length<=0;declared_outputs_hi<=0;
      sighash_buffer<=0;sign_is_eth<=0;
      output_load_valid<=0;output_load_address<=0;output_load_byte<=0;
      signer_start<=0;sign_operation_active<=0;sign_result_pending<=0;
      sign_sequence<=0;
      tx_start<=0;tx_data<=0;response_active<=0;response_index<=0;
      response_payload_length<=0;response_crc<=16'hffff;
      response_sequence<=0;response_command<=0;response_status<=0;
      response_word_shift<=0;
      last_success<=0;
    end else begin
      tx_start<=0;signer_start<=0;output_load_valid<=0;

      if(response_active && !tx_busy && !tx_start) begin
        tx_data<=response_next_byte;tx_start<=1;
        if(response_index>=2 && response_index<8+response_payload_length)
          response_crc<=crc16_next(response_crc,response_next_byte);
        if(response_command==COMMAND_SIGN_BTC &&
           response_index>=40 && response_index<72) begin
          if(response_index[1:0]==2'b11) begin
            if(response_index==9'd71)
              response_word_shift<=signer_r[255:224];
            else response_word_shift<=response_next_digest_word;
          end
          else response_word_shift<={response_word_shift[23:0],8'h00};
        end else if((response_command==COMMAND_SIGN_BTC ||
                     response_command==COMMAND_SIGN_ETH) &&
                    response_index>=response_signature_start &&
                    response_index<response_signature_start+9'd64) begin
          if(response_signature_offset[1:0]==2'b11 &&
             response_index<response_signature_start+9'd63)
            response_word_shift<=response_next_signature_word;
          else response_word_shift<={response_word_shift[23:0],8'h00};
        end
        if(response_index==9+response_payload_length) begin
          response_active<=0;response_index<=0;
        end else response_index<=response_index+1'b1;
      end

      if(rx_framing_error) parser_state<=P_SYNC_A5;
      else if(rx_valid) begin
        case(parser_state)
          P_SYNC_A5:if(rx_data==8'ha5) parser_state<=P_SYNC_5A;
          P_SYNC_5A:begin
            if(rx_data==8'h5a) begin request_crc<=16'hffff;payload_index<=0;parser_state<=P_VERSION;end
            else if(rx_data!=8'ha5) parser_state<=P_SYNC_A5;
          end
          P_VERSION:begin request_version<=rx_data;request_crc<=crc16_next(request_crc,rx_data);parser_state<=P_SEQUENCE;end
          P_SEQUENCE:begin request_sequence<=rx_data;request_crc<=crc16_next(request_crc,rx_data);parser_state<=P_COMMAND;end
          P_COMMAND:begin request_command<=rx_data;request_crc<=crc16_next(request_crc,rx_data);parser_state<=P_LEN_HI;end
          P_LEN_HI:begin request_len_hi<=rx_data;request_crc<=crc16_next(request_crc,rx_data);parser_state<=P_LEN_LO;end
          P_LEN_LO:begin
            request_length<={request_len_hi,rx_data};request_crc<=crc16_next(request_crc,rx_data);
            payload_index<=0;
            // While a signature is active these registers are the frozen
            // request.  Parse an incoming frame far enough to return BUSY,
            // but never clear or overwrite the transaction being signed.
            if(!sign_operation_active && !signer_busy) begin
              sighash_buffer<=0;declared_outputs_length<=0;
            end
            if({request_len_hi,rx_data}>MAX_PAYLOAD ||
               (request_command==COMMAND_SIGN_BTC && {request_len_hi,rx_data}<114)) begin
              if(!response_active)begin_response(request_sequence,request_command,STATUS_LENGTH,0);
              parser_state<=P_SYNC_A5;
            end else if({request_len_hi,rx_data}==0) parser_state<=P_CRC_HI;
            else parser_state<=P_PAYLOAD;
          end
          P_PAYLOAD:begin
            request_crc<=crc16_next(request_crc,rx_data);
            if(!sign_operation_active && !signer_busy) begin
              // Store the complete frozen request once in BSRAM.  The BIP143
              // engine reads version/outpoint/sequence/script/amount/outputs
              // and trailer bytes directly by their protocol offsets.
              if(request_command==COMMAND_SIGN_BTC) begin
                output_load_valid<=1;output_load_address<=payload_index[9:0];
                output_load_byte<=rx_data;
              end
              if(payload_index==104) declared_outputs_hi<=rx_data;
              else if(payload_index==105) declared_outputs_length<={declared_outputs_hi,rx_data};
              if(payload_index>=request_length-4)
                sighash_buffer<={sighash_buffer[23:0],rx_data};
            end
            if(payload_index+1'b1==request_length) begin payload_index<=0;parser_state<=P_CRC_HI;end
            else payload_index<=payload_index+1'b1;
          end
          P_CRC_HI:begin request_crc_hi<=rx_data;parser_state<=P_CRC_LO;end
          P_CRC_LO:begin
            parser_state<=P_SYNC_A5;
            if({request_crc_hi,rx_data}!=request_crc) begin
              if(!response_active)begin_response(request_sequence,request_command,STATUS_CRC,0);
            end else if(request_version!=VERSION) begin
              if(!response_active)begin_response(request_sequence,request_command,STATUS_COMMAND,0);
            end else if(request_command==COMMAND_PING) begin
              if(!response_active)begin_response(request_sequence,request_command,
                request_length==0?STATUS_OK:STATUS_LENGTH,request_length==0?16'd4:16'd0);
            end else if(request_command!=COMMAND_SIGN_BTC &&
                        request_command!=COMMAND_SIGN_ETH) begin
              if(!response_active)begin_response(request_sequence,request_command,STATUS_COMMAND,0);
            end else if(sign_operation_active || signer_busy) begin
              if(!response_active)begin_response(request_sequence,request_command,STATUS_BUSY,0);
            end else if(request_command==COMMAND_SIGN_ETH) begin
              if(request_length!=16'd32) begin
                if(!response_active)begin_response(request_sequence,request_command,STATUS_LENGTH,0);
              end else begin
                sign_sequence<=request_sequence;sign_is_eth<=1;
                sign_operation_active<=1;signer_start<=1;
              end
            end else if(declared_outputs_length==0 || declared_outputs_length>MAX_OUTPUT_BYTES ||
                        request_length!=114+declared_outputs_length ||
                        sighash_buffer!=32'h01000000) begin
              if(!response_active)begin_response(request_sequence,request_command,STATUS_LENGTH,0);
            end else begin
              sign_sequence<=request_sequence;sign_is_eth<=0;
              sign_operation_active<=1;signer_start<=1;
            end
          end
          default:parser_state<=P_SYNC_A5;
        endcase
      end

      if(signer_done && sign_operation_active) sign_result_pending<=1;
      if(sign_result_pending && !response_active) begin
        sign_result_pending<=0;sign_operation_active<=0;
        if(signer_error) begin
          begin_response(sign_sequence,sign_is_eth?COMMAND_SIGN_ETH:COMMAND_SIGN_BTC,
            signer_error_code==1?STATUS_KEY:
            (sign_is_eth&&signer_error_code==4)?STATUS_ETH_RECOVERY:
            signer_error_code==2?STATUS_BIP143:STATUS_VERIFY,0);
          last_success<=0;
        end else begin
          response_word_shift<=sign_is_eth?signer_r[255:224]:signer_digest[255:224];
          begin_response(sign_sequence,sign_is_eth?COMMAND_SIGN_ETH:COMMAND_SIGN_BTC,
                         STATUS_OK,sign_is_eth?16'd65:16'd128);
          last_success<=1;
        end
      end
    end
  end
endmodule
