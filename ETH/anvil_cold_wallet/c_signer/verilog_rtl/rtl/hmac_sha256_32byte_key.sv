`timescale 1ns/1ps

module hmac_sha256_32byte_key (
  input  logic         clk,
  input  logic         reset_n,
  input  logic         start,
  input  logic [255:0] key,
  input  logic [775:0] message,
  input  logic [6:0]   message_length,
  output logic         busy,
  output logic         done,
  output logic [255:0] digest
);
  typedef enum logic [2:0] {
    HMAC_IDLE,
    HMAC_INNER_START,
    HMAC_INNER_FEED,
    HMAC_INNER_WAIT,
    HMAC_OUTER_START,
    HMAC_OUTER_FEED,
    HMAC_OUTER_WAIT
  } hmac_state_t;

  hmac_state_t state;
  logic [255:0] key_reg;
  logic [775:0] message_reg;
  logic [6:0] message_length_reg;
  logic [255:0] inner_digest;
  logic [7:0] feed_index;

  logic sha_start;
  logic sha_in_valid;
  logic sha_in_ready;
  logic [7:0] sha_in_byte;
  logic sha_in_last;
  logic sha_done;
  logic [255:0] sha_digest;
  logic [7:0] key_byte;
  logic [7:0] message_byte;

  sha256_stream sha_inst (
    .clk(clk), .reset_n(reset_n), .start(sha_start),
    .in_valid(sha_in_valid), .in_ready(sha_in_ready),
    .in_byte(sha_in_byte), .in_last(sha_in_last),
    .busy(), .done(sha_done), .digest(sha_digest)
  );

  always_comb begin
    sha_start = (state == HMAC_INNER_START) || (state == HMAC_OUTER_START);
    sha_in_valid = (state == HMAC_INNER_FEED) || (state == HMAC_OUTER_FEED);
    sha_in_byte = 8'b0;
    sha_in_last = 1'b0;
    key_byte = 8'b0;
    message_byte = 8'b0;

    if (feed_index < 32)
      key_byte = key_reg[255-feed_index*8 -: 8];

    if (state == HMAC_INNER_FEED) begin
      if (feed_index < 64) begin
        sha_in_byte = key_byte ^ 8'h36;
      end else begin
        message_byte = message_reg[775-(feed_index-64)*8 -: 8];
        sha_in_byte = message_byte;
      end
      sha_in_last = (feed_index == (8'd63 + message_length_reg));
    end else if (state == HMAC_OUTER_FEED) begin
      if (feed_index < 64) begin
        sha_in_byte = key_byte ^ 8'h5c;
      end else begin
        sha_in_byte = inner_digest[255-(feed_index-64)*8 -: 8];
      end
      sha_in_last = (feed_index == 8'd95);
    end
  end

  always_ff @(posedge clk or negedge reset_n) begin
    if (!reset_n) begin
      state <= HMAC_IDLE;
      key_reg <= 0;
      message_reg <= 0;
      message_length_reg <= 0;
      inner_digest <= 0;
      feed_index <= 0;
      busy <= 0;
      done <= 0;
      digest <= 0;
    end else begin
      done <= 1'b0;
      case (state)
        HMAC_IDLE: begin
          if (start) begin
            key_reg <= key;
            message_reg <= message;
            message_length_reg <= message_length;
            busy <= 1'b1;
            state <= HMAC_INNER_START;
          end
        end
        HMAC_INNER_START: begin
          feed_index <= 0;
          state <= HMAC_INNER_FEED;
        end
        HMAC_INNER_FEED: begin
          if (sha_in_ready) begin
            if (sha_in_last)
              state <= HMAC_INNER_WAIT;
            else
              feed_index <= feed_index + 1'b1;
          end
        end
        HMAC_INNER_WAIT: begin
          if (sha_done) begin
            inner_digest <= sha_digest;
            state <= HMAC_OUTER_START;
          end
        end
        HMAC_OUTER_START: begin
          feed_index <= 0;
          state <= HMAC_OUTER_FEED;
        end
        HMAC_OUTER_FEED: begin
          if (sha_in_ready) begin
            if (sha_in_last)
              state <= HMAC_OUTER_WAIT;
            else
              feed_index <= feed_index + 1'b1;
          end
        end
        HMAC_OUTER_WAIT: begin
          if (sha_done) begin
            digest <= sha_digest;
            busy <= 1'b0;
            done <= 1'b1;
            state <= HMAC_IDLE;
          end
        end
        default: state <= HMAC_IDLE;
      endcase
    end
  end
endmodule
