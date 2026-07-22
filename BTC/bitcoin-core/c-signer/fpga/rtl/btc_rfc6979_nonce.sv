`timescale 1ns/1ps

// RFC6979 nonce generator connected to an externally shared SHA-256 engine.
module btc_rfc6979_nonce (
  input  logic         clk,
  input  logic         reset_n,
  input  logic         start,
  input  logic         retry,
  input  logic [255:0] private_key,
  input  logic [255:0] message_hash,
  output logic         busy,
  output logic         done,
  output logic [255:0] nonce,
  output logic         sha_start,
  output logic         sha_in_valid,
  output logic [7:0]   sha_in_byte,
  output logic         sha_in_last,
  input  logic         sha_in_ready,
  input  logic         sha_done,
  input  logic [255:0] sha_digest
);
`include "eth_secp256k1_common.svh"
  typedef enum logic [4:0] {
    RFC_IDLE,
    RFC_CALL_K0,RFC_WAIT_K0,RFC_CALL_V0,RFC_WAIT_V0,
    RFC_CALL_K1,RFC_WAIT_K1,RFC_CALL_V1,RFC_WAIT_V1,
    RFC_CALL_CANDIDATE,RFC_WAIT_CANDIDATE,
    RFC_CALL_RETRY_K,RFC_WAIT_RETRY_K,RFC_CALL_RETRY_V,RFC_WAIT_RETRY_V
  } rfc_state_t;

  rfc_state_t state;
  logic [255:0] reduced_hash_value,k_value,v_value;
  logic [1:0] hmac_message_kind;
  logic [7:0] hmac_marker;
  logic hmac_start,hmac_done;
  logic [255:0] hmac_digest;

  btc_rfc6979_hmac_sha256 hmac_inst (
    .clk(clk),.reset_n(reset_n),.start(hmac_start),
    .key(k_value),.v_value(v_value),.marker(hmac_marker),
    // The signing core holds both inputs stable until RFC6979 completes, so
    // do not duplicate them in two additional 256-bit register banks.
    .private_key(private_key),.reduced_hash(reduced_hash_value),
    .message_kind(hmac_message_kind),.busy(),.done(hmac_done),.digest(hmac_digest),
    .sha_start(sha_start),.sha_in_valid(sha_in_valid),.sha_in_byte(sha_in_byte),
    .sha_in_last(sha_in_last),.sha_in_ready(sha_in_ready),
    .sha_done(sha_done),.sha_digest(sha_digest)
  );

  always_comb begin
    reduced_hash_value=(message_hash>=SECP256K1_N) ?
                       message_hash-SECP256K1_N : message_hash;
    hmac_start=(state==RFC_CALL_K0)||(state==RFC_CALL_V0)||
               (state==RFC_CALL_K1)||(state==RFC_CALL_V1)||
               (state==RFC_CALL_CANDIDATE)||(state==RFC_CALL_RETRY_K)||
               (state==RFC_CALL_RETRY_V);
    hmac_message_kind=0;hmac_marker=0;
    if(state==RFC_CALL_K0 || state==RFC_WAIT_K0) begin
      hmac_message_kind=2;hmac_marker=8'h00;
    end else if(state==RFC_CALL_K1 || state==RFC_WAIT_K1) begin
      hmac_message_kind=2;hmac_marker=8'h01;
    end else if(state==RFC_CALL_RETRY_K || state==RFC_WAIT_RETRY_K) begin
      hmac_message_kind=1;hmac_marker=8'h00;
    end
  end

  always_ff @(posedge clk or negedge reset_n) begin
    if(!reset_n) begin
      state<=RFC_IDLE;k_value<=0;v_value<=0;busy<=0;done<=0;nonce<=0;
    end else begin
      done<=1'b0;
      case(state)
        RFC_IDLE:begin
          if(start) begin
            k_value<=0;
            v_value<=256'h0101010101010101010101010101010101010101010101010101010101010101;
            busy<=1;state<=RFC_CALL_K0;
          end else if(retry) begin busy<=1;state<=RFC_CALL_RETRY_K;end
        end
        RFC_CALL_K0:state<=RFC_WAIT_K0;
        RFC_WAIT_K0:if(hmac_done) begin k_value<=hmac_digest;state<=RFC_CALL_V0;end
        RFC_CALL_V0:state<=RFC_WAIT_V0;
        RFC_WAIT_V0:if(hmac_done) begin v_value<=hmac_digest;state<=RFC_CALL_K1;end
        RFC_CALL_K1:state<=RFC_WAIT_K1;
        RFC_WAIT_K1:if(hmac_done) begin k_value<=hmac_digest;state<=RFC_CALL_V1;end
        RFC_CALL_V1:state<=RFC_WAIT_V1;
        RFC_WAIT_V1:if(hmac_done) begin v_value<=hmac_digest;state<=RFC_CALL_CANDIDATE;end
        RFC_CALL_CANDIDATE:state<=RFC_WAIT_CANDIDATE;
        RFC_WAIT_CANDIDATE:if(hmac_done) begin
          v_value<=hmac_digest;
          if(hmac_digest!=0 && hmac_digest<SECP256K1_N) begin
            nonce<=hmac_digest;busy<=0;done<=1;state<=RFC_IDLE;
          end else state<=RFC_CALL_RETRY_K;
        end
        RFC_CALL_RETRY_K:state<=RFC_WAIT_RETRY_K;
        RFC_WAIT_RETRY_K:if(hmac_done) begin k_value<=hmac_digest;state<=RFC_CALL_RETRY_V;end
        RFC_CALL_RETRY_V:state<=RFC_WAIT_RETRY_V;
        RFC_WAIT_RETRY_V:if(hmac_done) begin v_value<=hmac_digest;state<=RFC_CALL_CANDIDATE;end
        default:state<=RFC_IDLE;
      endcase
    end
  end
endmodule
