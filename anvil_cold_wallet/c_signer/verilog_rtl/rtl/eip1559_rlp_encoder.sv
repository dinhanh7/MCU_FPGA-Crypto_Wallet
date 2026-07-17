`timescale 1ns/1ps

module eip1559_rlp_encoder (
  input  logic         clk,
  input  logic         reset_n,
  input  logic         start,
  input  logic         signed_mode,
  input  logic [255:0] chain_id,
  input  logic [255:0] nonce,
  input  logic [255:0] max_priority_fee_per_gas,
  input  logic [255:0] max_fee_per_gas,
  input  logic [255:0] gas_limit,
  input  logic [159:0] recipient,
  input  logic [255:0] value,
  input  logic [11:0]  data_length,
  output logic [10:0]  data_read_address,
  input  logic [7:0]   data_read_byte,
  input  logic         y_parity,
  input  logic [255:0] signature_r,
  input  logic [255:0] signature_s,
  output logic         out_valid,
  input  logic         out_ready,
  output logic [7:0]   out_byte,
  output logic         out_last,
  output logic         busy,
  output logic         done,
  output logic [12:0]  encoded_length
);
`include "eth_secp256k1_common.svh"

  typedef enum logic [3:0] {
    ENC_IDLE,
    ENC_TYPE,
    ENC_LIST_PREFIX,
    ENC_LIST_LENGTH,
    ENC_FIELD_SETUP,
    ENC_FIELD_PREFIX,
    ENC_FIELD_LENGTH,
    ENC_DATA_WAIT,
    ENC_FIELD_BYTES,
    ENC_LITERAL
  } encoder_state_t;

  typedef enum logic [1:0] {
    SOURCE_VALUE,
    SOURCE_ADDRESS,
    SOURCE_DATA,
    SOURCE_LITERAL
  } source_t;

  encoder_state_t state;
  source_t selected_source;
  source_t field_source;
  logic signed_mode_reg;
  logic [3:0] field_index;
  logic [3:0] final_field_index;
  logic [255:0] selected_value;
  logic [255:0] field_value;
  logic [12:0] selected_length;
  logic [12:0] field_length;
  logic [7:0] selected_first_byte;
  logic [7:0] selected_literal;
  logic [7:0] literal_byte;
  logic [12:0] payload_length;
  logic [2:0] list_length_bytes;
  logic [2:0] field_length_bytes;
  logic [2:0] length_position;
  logic [12:0] byte_position;

  function automatic logic [12:0] rlp_string_size(
    input logic [12:0] length,
    input logic [7:0] first_byte
  );
    begin
      if (length == 1 && first_byte < 8'h80)
        rlp_string_size = 1;
      else if (length <= 55)
        rlp_string_size = 1 + length;
      else
        rlp_string_size = 1 + length_of_length(length) + length;
    end
  endfunction

  function automatic logic [12:0] rlp_uint_size(input logic [255:0] number);
    logic [5:0] length;
    begin
      length = uint256_min_bytes(number);
      rlp_uint_size = rlp_string_size(length, number[7:0]);
    end
  endfunction

  function automatic logic [12:0] common_payload_size(
    input logic [255:0] f_chain,
    input logic [255:0] f_nonce,
    input logic [255:0] f_priority,
    input logic [255:0] f_max_fee,
    input logic [255:0] f_gas,
    input logic [255:0] f_value,
    input logic [11:0] f_data_length,
    input logic [7:0] f_data_first
  );
    begin
      common_payload_size = rlp_uint_size(f_chain) + rlp_uint_size(f_nonce) +
        rlp_uint_size(f_priority) + rlp_uint_size(f_max_fee) +
        rlp_uint_size(f_gas) + 13'd21 + rlp_uint_size(f_value) +
        rlp_string_size({1'b0,f_data_length},f_data_first) + 13'd1;
    end
  endfunction

  logic [12:0] common_length_comb;
  logic [12:0] payload_length_comb;
  logic [12:0] list_prefix_size_comb;
  always_comb begin
    common_length_comb = common_payload_size(chain_id,nonce,max_priority_fee_per_gas,
      max_fee_per_gas,gas_limit,value,data_length,data_read_byte);
    payload_length_comb = common_length_comb;
    if (signed_mode)
      payload_length_comb = common_length_comb + rlp_uint_size({255'b0,y_parity}) +
                            rlp_uint_size(signature_r) + rlp_uint_size(signature_s);
    if (payload_length_comb <= 55)
      list_prefix_size_comb = 1;
    else
      list_prefix_size_comb = 1 + length_of_length(payload_length_comb);
  end

  always_comb begin
    selected_source = SOURCE_VALUE;
    selected_value = 256'b0;
    selected_length = 0;
    selected_first_byte = 0;
    selected_literal = 0;
    case (field_index)
      0: selected_value=chain_id;
      1: selected_value=nonce;
      2: selected_value=max_priority_fee_per_gas;
      3: selected_value=max_fee_per_gas;
      4: selected_value=gas_limit;
      5: begin
        selected_source=SOURCE_ADDRESS;
        selected_value={96'b0,recipient};
        selected_length=20;
        selected_first_byte=recipient[159:152];
      end
      6: selected_value=value;
      7: begin
        selected_source=SOURCE_DATA;
        selected_length={1'b0,data_length};
        selected_first_byte=data_read_byte;
      end
      8: begin
        selected_source=SOURCE_LITERAL;
        selected_literal=8'hc0;
      end
      9: selected_value={255'b0,y_parity};
      10: selected_value=signature_r;
      default: selected_value=signature_s;
    endcase
    if (selected_source == SOURCE_VALUE) begin
      selected_length = uint256_min_bytes(selected_value);
      selected_first_byte = selected_value[7:0];
    end
  end

  logic field_is_final;
  assign field_is_final = (field_index == final_field_index);
  assign data_read_address = ((state == ENC_FIELD_BYTES || state == ENC_DATA_WAIT) &&
                              field_source == SOURCE_DATA) ?
                             byte_position[10:0] : 11'b0;

  always_comb begin
    out_valid = 1'b0;
    out_byte = 8'b0;
    out_last = 1'b0;
    case (state)
      ENC_TYPE: begin out_valid=1; out_byte=8'h02; end
      ENC_LIST_PREFIX: begin
        out_valid=1;
        if(payload_length<=55) out_byte=8'hc0+payload_length[7:0];
        else out_byte=8'hf7+list_length_bytes;
      end
      ENC_LIST_LENGTH: begin
        out_valid=1;
        out_byte=(payload_length >> ((list_length_bytes-1-length_position)*8));
      end
      ENC_FIELD_PREFIX: begin
        out_valid=1;
        if(field_length<=55) out_byte=8'h80+field_length[7:0];
        else out_byte=8'hb7+field_length_bytes;
      end
      ENC_FIELD_LENGTH: begin
        out_valid=1;
        out_byte=(field_length >> ((field_length_bytes-1-length_position)*8));
      end
      ENC_FIELD_BYTES: begin
        out_valid=1;
        if(field_source==SOURCE_DATA) out_byte=data_read_byte;
        else out_byte=field_value >> ((field_length-1-byte_position)*8);
        out_last=field_is_final && (byte_position==field_length-1);
      end
      ENC_LITERAL: begin
        out_valid=1; out_byte=literal_byte; out_last=field_is_final;
      end
      default: begin end
    endcase
  end

  always_ff @(posedge clk or negedge reset_n) begin
    if(!reset_n) begin
      state<=ENC_IDLE; signed_mode_reg<=0; field_index<=0; final_field_index<=8;
      field_source<=SOURCE_VALUE; field_value<=0; field_length<=0;
      literal_byte<=0; payload_length<=0; list_length_bytes<=0;
      field_length_bytes<=0; length_position<=0; byte_position<=0;
      busy<=0; done<=0; encoded_length<=0;
    end else begin
      done<=1'b0;
      case(state)
        ENC_IDLE: if(start) begin
          signed_mode_reg<=signed_mode;
          final_field_index<=signed_mode ? 4'd11 : 4'd8;
          payload_length<=payload_length_comb;
          list_length_bytes<=length_of_length(payload_length_comb);
          encoded_length<=1 + list_prefix_size_comb + payload_length_comb;
          field_index<=0; busy<=1; state<=ENC_TYPE;
        end
        ENC_TYPE: if(out_ready) state<=ENC_LIST_PREFIX;
        ENC_LIST_PREFIX: if(out_ready) begin
          if(payload_length<=55) state<=ENC_FIELD_SETUP;
          else begin length_position<=0; state<=ENC_LIST_LENGTH; end
        end
        ENC_LIST_LENGTH: if(out_ready) begin
          if(length_position==list_length_bytes-1) state<=ENC_FIELD_SETUP;
          else length_position<=length_position+1'b1;
        end
        ENC_FIELD_SETUP: begin
          field_source<=selected_source;
          field_value<=selected_value;
          field_length<=selected_length;
          literal_byte<=selected_literal;
          byte_position<=0;
          if(selected_source==SOURCE_LITERAL) state<=ENC_LITERAL;
          else if(selected_length==1 && selected_first_byte<8'h80)
            state <= (selected_source==SOURCE_DATA) ? ENC_DATA_WAIT : ENC_FIELD_BYTES;
          else begin
            field_length_bytes<=length_of_length(selected_length);
            state<=ENC_FIELD_PREFIX;
          end
        end
        ENC_FIELD_PREFIX: if(out_ready) begin
          if(field_length>55) begin length_position<=0; state<=ENC_FIELD_LENGTH; end
          else if(field_length==0) begin
            if(field_is_final) begin busy<=0;done<=1;state<=ENC_IDLE;end
            else begin field_index<=field_index+1'b1;state<=ENC_FIELD_SETUP;end
          end
          else state <= (field_source==SOURCE_DATA) ? ENC_DATA_WAIT : ENC_FIELD_BYTES;
        end
        ENC_FIELD_LENGTH: if(out_ready) begin
          if(length_position==field_length_bytes-1) begin
            byte_position<=0;
            state <= (field_source==SOURCE_DATA) ? ENC_DATA_WAIT : ENC_FIELD_BYTES;
          end
          else length_position<=length_position+1'b1;
        end
        ENC_DATA_WAIT: state<=ENC_FIELD_BYTES;
        ENC_FIELD_BYTES: if(out_ready) begin
          if(byte_position==field_length-1) begin
            if(field_is_final) begin busy<=0;done<=1;state<=ENC_IDLE;end
            else begin field_index<=field_index+1'b1;state<=ENC_FIELD_SETUP;end
          end
          else begin
            byte_position<=byte_position+1'b1;
            if(field_source==SOURCE_DATA) state<=ENC_DATA_WAIT;
          end
        end
        ENC_LITERAL: if(out_ready) begin
          if(field_is_final) begin busy<=0;done<=1;state<=ENC_IDLE;end
          else begin field_index<=field_index+1'b1;state<=ENC_FIELD_SETUP;end
        end
        default: state<=ENC_IDLE;
      endcase
    end
  end
endmodule
