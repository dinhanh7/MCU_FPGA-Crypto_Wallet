`timescale 1ns/1ps

module rfc6979_nonce (
  input  logic         clk,
  input  logic         reset_n,
  input  logic         start,
  input  logic [255:0] private_key,
  input  logic [255:0] message_hash,
  output logic         busy,
  output logic         done,
  output logic [255:0] nonce
);
`include "eth_secp256k1_common.svh"

  typedef enum logic [4:0] {
    RFC_IDLE,
    RFC_CALL_K0, RFC_WAIT_K0,
    RFC_CALL_V0, RFC_WAIT_V0,
    RFC_CALL_K1, RFC_WAIT_K1,
    RFC_CALL_V1, RFC_WAIT_V1,
    RFC_CALL_CANDIDATE, RFC_WAIT_CANDIDATE,
    RFC_CALL_RETRY_K, RFC_WAIT_RETRY_K,
    RFC_CALL_RETRY_V, RFC_WAIT_RETRY_V
  } rfc_state_t;

  rfc_state_t state;
  logic [255:0] private_key_reg;
  logic [255:0] reduced_hash;
  logic [255:0] k_value;
  logic [255:0] v_value;
  logic [1:0] hmac_message_kind;
  logic [7:0] hmac_marker;
  logic hmac_start;
  logic hmac_done;
  logic [255:0] hmac_digest;

  rfc6979_hmac_sha256 hmac_inst (
    .clk(clk), .reset_n(reset_n), .start(hmac_start),
    .key(k_value),.v_value(v_value),.marker(hmac_marker),
    .private_key(private_key_reg),.reduced_hash(reduced_hash),
    .message_kind(hmac_message_kind),
    .busy(), .done(hmac_done), .digest(hmac_digest)
  );

  always_comb begin
    hmac_start = (state == RFC_CALL_K0) || (state == RFC_CALL_V0) ||
                 (state == RFC_CALL_K1) || (state == RFC_CALL_V1) ||
                 (state == RFC_CALL_CANDIDATE) ||
                 (state == RFC_CALL_RETRY_K) || (state == RFC_CALL_RETRY_V);
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
    if (!reset_n) begin
      state <= RFC_IDLE;
      private_key_reg <= 0;
      reduced_hash <= 0;
      k_value <= 0;
      v_value <= 0;
      busy <= 0;
      done <= 0;
      nonce <= 0;
    end else begin
      done <= 1'b0;
      case (state)
        RFC_IDLE: begin
          if (start) begin
            private_key_reg <= private_key;
            reduced_hash <= (message_hash >= SECP256K1_N) ?
                            message_hash - SECP256K1_N : message_hash;
            k_value <= 256'b0;
            v_value <= 256'h0101010101010101010101010101010101010101010101010101010101010101;
            busy <= 1'b1;
            state <= RFC_CALL_K0;
          end
        end

        RFC_CALL_K0: state <= RFC_WAIT_K0;
        RFC_WAIT_K0: if (hmac_done) begin
          k_value <= hmac_digest;
          state <= RFC_CALL_V0;
        end

        RFC_CALL_V0: state <= RFC_WAIT_V0;
        RFC_WAIT_V0: if (hmac_done) begin
          v_value <= hmac_digest;
          state <= RFC_CALL_K1;
        end

        RFC_CALL_K1: state <= RFC_WAIT_K1;
        RFC_WAIT_K1: if (hmac_done) begin
          k_value <= hmac_digest;
          state <= RFC_CALL_V1;
        end

        RFC_CALL_V1: state <= RFC_WAIT_V1;
        RFC_WAIT_V1: if (hmac_done) begin
          v_value <= hmac_digest;
          state <= RFC_CALL_CANDIDATE;
        end

        RFC_CALL_CANDIDATE: state <= RFC_WAIT_CANDIDATE;
        RFC_WAIT_CANDIDATE: if (hmac_done) begin
          v_value <= hmac_digest;
          if (hmac_digest != 0 && hmac_digest < SECP256K1_N) begin
            nonce <= hmac_digest;
            busy <= 1'b0;
            done <= 1'b1;
            state <= RFC_IDLE;
          end else begin
            state <= RFC_CALL_RETRY_K;
          end
        end

        RFC_CALL_RETRY_K: state <= RFC_WAIT_RETRY_K;
        RFC_WAIT_RETRY_K: if (hmac_done) begin
          k_value <= hmac_digest;
          state <= RFC_CALL_RETRY_V;
        end

        RFC_CALL_RETRY_V: state <= RFC_WAIT_RETRY_V;
        RFC_WAIT_RETRY_V: if (hmac_done) begin
          v_value <= hmac_digest;
          state <= RFC_CALL_CANDIDATE;
        end

        default: state <= RFC_IDLE;
      endcase
    end
  end
endmodule
