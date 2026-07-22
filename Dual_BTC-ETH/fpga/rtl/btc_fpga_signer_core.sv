`timescale 1ns/1ps

module btc_fpga_signer_core #(
  parameter logic [255:0] PRIVATE_KEY = 256'h1,
  parameter integer MAX_OUTPUT_BYTES = 512,
  parameter bit SERIALIZED_REQUEST = 1'b0,
  parameter bit PRELOAD_OUTPUTS = 1'b0,
  parameter logic [MAX_OUTPUT_BYTES*8-1:0] PRELOAD_OUTPUT_DATA = '0
) (
  input  logic         clk,
  input  logic         reset_n,
  input  logic         outputs_load_valid,
  input  logic [9:0]   outputs_load_address,
  input  logic [7:0]   outputs_load_byte,
  input  logic [9:0]   request_length,
  input  logic         buffer_read_enable,
  input  logic [9:0]   buffer_read_address,
  output logic [7:0]   buffer_read_byte,
  input  logic         start,
  input  logic         direct_hash_mode,
  input  logic         direct_hash_load_valid,
  input  logic [7:0]   direct_hash_load_byte,
  input  logic [31:0]  tx_version,
  input  logic [287:0] outpoint,
  input  logic [31:0]  input_sequence,
  input  logic [159:0] pubkey_hash,
  input  logic [63:0]  prevout_amount,
  input  logic [9:0]   outputs_length,
  input  logic [31:0]  locktime,
  input  logic [31:0]  sighash_type,
  output logic         busy,
  output logic         done,
  output logic         error,
  output logic [3:0]   error_code,
  output logic [255:0] bip143_digest,
  output logic         y_parity,
  output logic         recovery_high,
  output logic [255:0] signature_r,
  output logic [255:0] signature_s
);
  typedef enum logic [2:0] {C_IDLE,C_HASH_START,C_HASH_WAIT,C_SIGN_START,C_SIGN_WAIT} state_t;
  state_t state;
  logic hash_start,hash_busy,hash_done,hash_error;
  logic [255:0] hash_digest;
  logic sign_start,sign_busy,sign_done,sign_error,sign_verified;
  logic sign_y_parity,sign_recovery_high;
  logic [3:0] sign_error_code;
  logic operation_direct;
  logic hash_sha_start,hash_sha_valid,hash_sha_last,hash_sha_ready,hash_sha_done;
  logic [7:0] hash_sha_byte;
  logic sign_sha_start,sign_sha_valid,sign_sha_last,sign_sha_ready,sign_sha_done;
  logic [7:0] sign_sha_byte;
  logic shared_sha_start,shared_sha_valid,shared_sha_last,shared_sha_ready,shared_sha_done;
  logic [7:0] shared_sha_byte;
  logic [255:0] shared_sha_digest;
  logic use_sign_sha;

  // The hasher keeps its completed digest until the next hash transaction.
  // Expose that register directly instead of copying all 256 bits into a
  // second bank before starting the signer.
  assign bip143_digest=hash_digest;
  assign y_parity=sign_y_parity;
  assign recovery_high=sign_recovery_high;

  assign use_sign_sha=(state==C_SIGN_START)||(state==C_SIGN_WAIT);
  assign shared_sha_start=use_sign_sha ? sign_sha_start : hash_sha_start;
  assign shared_sha_valid=use_sign_sha ? sign_sha_valid : hash_sha_valid;
  assign shared_sha_byte=use_sign_sha ? sign_sha_byte : hash_sha_byte;
  assign shared_sha_last=use_sign_sha ? sign_sha_last : hash_sha_last;
  assign hash_sha_ready=!use_sign_sha && shared_sha_ready;
  assign hash_sha_done=!use_sign_sha && shared_sha_done;
  assign sign_sha_ready=use_sign_sha && shared_sha_ready;
  assign sign_sha_done=use_sign_sha && shared_sha_done;

  sha256_stream shared_sha (
    .clk(clk),.reset_n(reset_n),.start(shared_sha_start),
    .in_valid(shared_sha_valid),.in_ready(shared_sha_ready),
    .in_byte(shared_sha_byte),.in_last(shared_sha_last),
    .busy(),.done(shared_sha_done),.digest(shared_sha_digest)
  );

  btc_bip143_hash #(
    .MAX_OUTPUT_BYTES(MAX_OUTPUT_BYTES),.SERIALIZED_REQUEST(SERIALIZED_REQUEST),
    .PRELOAD_OUTPUTS(PRELOAD_OUTPUTS),
    .PRELOAD_OUTPUT_DATA(PRELOAD_OUTPUT_DATA)
  ) hasher (
    .clk(clk),.reset_n(reset_n),
    .outputs_load_valid(outputs_load_valid),
    .outputs_load_address(outputs_load_address),
    .outputs_load_byte(outputs_load_byte),
    .request_length(request_length),
    .buffer_read_enable(buffer_read_enable),
    .buffer_read_address(buffer_read_address),
    .buffer_read_byte(buffer_read_byte),
    .start(hash_start),.tx_version(tx_version),.outpoint(outpoint),
    .input_sequence(input_sequence),.pubkey_hash(pubkey_hash),
    .prevout_amount(prevout_amount),.outputs_length(outputs_length),
    .locktime(locktime),.sighash_type(sighash_type),
    .busy(hash_busy),.done(hash_done),.error(hash_error),.digest(hash_digest),
    .sha_start(hash_sha_start),.sha_in_valid(hash_sha_valid),
    .sha_in_byte(hash_sha_byte),.sha_in_last(hash_sha_last),
    .sha_in_ready(hash_sha_ready),.sha_done(hash_sha_done),
    .sha_digest(shared_sha_digest)
  );

  btc_ecdsa_sign_verify_core #(.PRIVATE_KEY(PRIVATE_KEY)) signer (
    .clk(clk),.reset_n(reset_n),.start(sign_start),.message_hash(hash_digest),
    .message_preloaded(operation_direct),
    .message_load_valid(direct_hash_load_valid),
    .message_load_byte(direct_hash_load_byte),
    .busy(sign_busy),.done(sign_done),.error(sign_error),
    .error_code(sign_error_code),.verified(sign_verified),
    .y_parity(sign_y_parity),.recovery_high(sign_recovery_high),
    .signature_r(signature_r),.signature_s(signature_s),
    .sha_start(sign_sha_start),.sha_in_valid(sign_sha_valid),
    .sha_in_byte(sign_sha_byte),.sha_in_last(sign_sha_last),
    .sha_in_ready(sign_sha_ready),.sha_done(sign_sha_done),
    .sha_digest(shared_sha_digest)
  );

  assign hash_start=(state==C_HASH_START);
  assign sign_start=(state==C_SIGN_START);
  assign busy=(state!=C_IDLE);

  always_ff @(posedge clk or negedge reset_n) begin
    if(!reset_n) begin
      state<=C_IDLE;done<=0;error<=0;error_code<=0;
      operation_direct<=0;
    end else begin
      done<=1'b0;
      case(state)
        C_IDLE:if(start) begin
          error<=0;error_code<=0;operation_direct<=direct_hash_mode;
          if(direct_hash_mode) state<=C_SIGN_START;
          else state<=C_HASH_START;
        end
        C_HASH_START:state<=C_HASH_WAIT;
        C_HASH_WAIT:if(hash_done) begin
          if(hash_error) begin error<=1;error_code<=2;done<=1;state<=C_IDLE;end
          else state<=C_SIGN_START;
        end
        C_SIGN_START:state<=C_SIGN_WAIT;
        C_SIGN_WAIT:if(sign_done) begin
          if(sign_error || !sign_verified || (operation_direct && sign_recovery_high)) begin
            error<=1;error_code<=(sign_error_code==1)?4'd1:4'd3;
            if(operation_direct && sign_recovery_high) error_code<=4'd4;
          end
          done<=1;state<=C_IDLE;
        end
        default:begin error<=1;error_code<=3;done<=1;state<=C_IDLE;end
      endcase
    end
  end
endmodule
