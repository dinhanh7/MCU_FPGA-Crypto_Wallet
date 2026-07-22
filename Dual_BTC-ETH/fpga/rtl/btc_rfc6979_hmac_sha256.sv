`timescale 1ns/1ps

// RFC6979 HMAC byte source using the BTC signer's shared SHA-256 engine.
module btc_rfc6979_hmac_sha256 (
  input  logic         clk,
  input  logic         reset_n,
  input  logic         start,
  input  logic [255:0] key,
  input  logic [255:0] v_value,
  input  logic [7:0]   marker,
  input  logic [255:0] private_key,
  input  logic [255:0] reduced_hash,
  input  logic [1:0]   message_kind,
  output logic         busy,
  output logic         done,
  output logic [255:0] digest,
  output logic         sha_start,
  output logic         sha_in_valid,
  output logic [7:0]   sha_in_byte,
  output logic         sha_in_last,
  input  logic         sha_in_ready,
  input  logic         sha_done,
  input  logic [255:0] sha_digest
);
  typedef enum logic [2:0] {
    H_IDLE,H_INNER_START,H_INNER_FEED,H_INNER_WAIT,
    H_OUTER_START,H_OUTER_FEED,H_OUTER_WAIT
  } state_t;

  state_t state;
  logic [255:0] inner_digest;
  logic [7:0] feed_index;
  logic [7:0] source_index,key_byte,message_byte;

  always_comb begin
    sha_start=(state==H_INNER_START)||(state==H_OUTER_START);
    sha_in_valid=(state==H_INNER_FEED)||(state==H_OUTER_FEED);
    sha_in_byte=0;sha_in_last=0;key_byte=0;message_byte=0;
    source_index=feed_index-8'd64;

    if(feed_index<32)
      key_byte=key[255-feed_index*8 -: 8];

    if(state==H_INNER_FEED) begin
      if(feed_index<64) sha_in_byte=key_byte^8'h36;
      else begin
        if(source_index<32)
          message_byte=v_value[255-source_index*8 -: 8];
        else if(source_index==32)
          message_byte=marker;
        else if(source_index<65)
          message_byte=private_key[255-(source_index-33)*8 -: 8];
        else
          message_byte=reduced_hash[255-(source_index-65)*8 -: 8];
        sha_in_byte=message_byte;
      end
      case(message_kind)
        0:sha_in_last=(feed_index==8'd95);
        1:sha_in_last=(feed_index==8'd96);
        default:sha_in_last=(feed_index==8'd160);
      endcase
    end else if(state==H_OUTER_FEED) begin
      if(feed_index<64) sha_in_byte=key_byte^8'h5c;
      else sha_in_byte=inner_digest[255-(feed_index-64)*8 -: 8];
      sha_in_last=(feed_index==8'd95);
    end
  end

  always_ff @(posedge clk or negedge reset_n) begin
    if(!reset_n) begin
      state<=H_IDLE;inner_digest<=0;feed_index<=0;
      busy<=0;done<=0;digest<=0;
    end else begin
      done<=0;
      case(state)
        H_IDLE:if(start) begin busy<=1;state<=H_INNER_START;end
        H_INNER_START:begin feed_index<=0;state<=H_INNER_FEED;end
        H_INNER_FEED:if(sha_in_ready) begin
          if(sha_in_last) state<=H_INNER_WAIT;
          else feed_index<=feed_index+1'b1;
        end
        H_INNER_WAIT:if(sha_done) begin
          inner_digest<=sha_digest;state<=H_OUTER_START;
        end
        H_OUTER_START:begin feed_index<=0;state<=H_OUTER_FEED;end
        H_OUTER_FEED:if(sha_in_ready) begin
          if(sha_in_last) state<=H_OUTER_WAIT;
          else feed_index<=feed_index+1'b1;
        end
        H_OUTER_WAIT:if(sha_done) begin
          digest<=sha_digest;busy<=0;done<=1;state<=H_IDLE;
        end
        default:state<=H_IDLE;
      endcase
    end
  end
endmodule
