`timescale 1ns/1ps

// BIP143 SIGHASH_ALL engine for the MCU policy used by coldsign:
// one native P2WPKH input and serialized transaction outputs supplied by MCU.
// The MCU never supplies a digest; all four double-SHA256 operations happen here.
module btc_bip143_hash #(
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
  output logic [255:0] digest,
  output logic         sha_start,
  output logic         sha_in_valid,
  output logic [7:0]   sha_in_byte,
  output logic         sha_in_last,
  input  logic         sha_in_ready,
  input  logic         sha_done,
  input  logic [255:0] sha_digest
);
  typedef enum logic [3:0] {
    H_IDLE,H_START,H_WAIT_READY,H_FEED,H_WAIT_DONE,H_COMPLETE,H_ERROR
  } hash_state_t;
  typedef enum logic [2:0] {
    P_PREVOUT_1,P_PREVOUT_2,P_SEQUENCE_1,P_SEQUENCE_2,
    P_OUTPUTS_1,P_OUTPUTS_2,P_PREIMAGE_1,P_PREIMAGE_2
  } pass_t;

  // The transaction-output buffer is deliberately synchronous.  An
  // asynchronous read makes GowinSynthesis implement 512 bytes as FF/LUT
  // fabric, which fills every CLS on GW5A-25 and leaves the design
  // unroutable.  This template maps to one BSRAM and the read address below
  // prefetches the next byte while SHA consumes the current byte.
  localparam integer BUFFER_BYTES = MAX_OUTPUT_BYTES +
                                    (SERIALIZED_REQUEST ? 114 : 0);
  (* ram_style="block", syn_ramstyle="block_ram", no_rw_check *)
  logic [7:0] outputs_memory [0:BUFFER_BYTES-1];
  logic [9:0] outputs_read_address;
  logic [7:0] outputs_read_byte;
  integer preload_index;

  initial begin
    if(PRELOAD_OUTPUTS)
      for(preload_index=0;preload_index<MAX_OUTPUT_BYTES;preload_index=preload_index+1)
        outputs_memory[preload_index]=
          PRELOAD_OUTPUT_DATA[MAX_OUTPUT_BYTES*8-1-preload_index*8-:8];
  end

  hash_state_t state;
  pass_t active_pass;
  logic [9:0] byte_index,pass_length;
  logic [255:0] first_digest;
  logic [255:0] hash_prevouts,hash_sequence,hash_outputs;

  always_ff @(posedge clk) begin
    if(outputs_load_valid && outputs_load_address<BUFFER_BYTES)
      outputs_memory[outputs_load_address]<=outputs_load_byte;
    outputs_read_byte<=outputs_memory[outputs_read_address];
  end

  assign buffer_read_byte=outputs_read_byte;

  function automatic logic [7:0] vector256_byte(
    input logic [255:0] value,input logic [5:0] index
  );
    vector256_byte=value[255-index*8-:8];
  endfunction

  // In UART mode the whole frozen payload is stored once in BSRAM:
  // freeze[0:31], version[32:35], outpoint[36:71], sequence[72:75],
  // pubkey hash[76:95], amount[96:103], output length[104:105], outputs,
  // locktime and sighash.  Return the BSRAM byte needed by a hash pass.
  function automatic logic [9:0] request_byte_address(
    input pass_t selected_pass,input logic [9:0] index
  );
    begin
      request_byte_address=10'd0;
      case(selected_pass)
        P_PREVOUT_1:request_byte_address=10'd36+index;
        P_SEQUENCE_1:request_byte_address=10'd72+index;
        P_OUTPUTS_1:request_byte_address=10'd106+index;
        P_PREIMAGE_1:begin
          if(index<4) request_byte_address=10'd32+index;
          else if(index>=68 && index<104)
            request_byte_address=10'd36+(index-68);
          else if(index>=108 && index<128)
            request_byte_address=10'd76+(index-108);
          else if(index>=130 && index<138)
            request_byte_address=10'd96+(index-130);
          else if(index>=138 && index<142)
            request_byte_address=10'd72+(index-138);
          else if(index>=174 && index<178)
            request_byte_address=request_length-10'd8+(index-174);
          else if(index>=178)
            request_byte_address=request_length-10'd4+(index-178);
        end
        default:request_byte_address=10'd0;
      endcase
    end
  endfunction

  function automatic logic request_byte_selected(
    input pass_t selected_pass,input logic [9:0] index
  );
    begin
      request_byte_selected=(selected_pass==P_PREVOUT_1)||
                            (selected_pass==P_SEQUENCE_1)||
                            (selected_pass==P_OUTPUTS_1);
      if(selected_pass==P_PREIMAGE_1)
        request_byte_selected=(index<4)||
          (index>=68 && index<104)||(index>=108 && index<128)||
          (index>=130 && index<142)||(index>=174);
    end
  endfunction

  always_comb begin
    sha_start=(state==H_START);
    sha_in_valid=(state==H_FEED);
    sha_in_last=(byte_index+1'b1==pass_length);
    sha_in_byte=8'h00;
    // Address zero is fetched during H_START/H_WAIT_READY.  Once feeding the
    // outputs pass, request N+1 as byte N is accepted by the SHA stream.
    outputs_read_address=10'd0;
    if(SERIALIZED_REQUEST) begin
      if(state==H_FEED && sha_in_ready)
        outputs_read_address=request_byte_address(active_pass,byte_index+1'b1);
      else
        outputs_read_address=request_byte_address(active_pass,byte_index);
    end else if(active_pass==P_OUTPUTS_1) begin
      // Hold the current address during SHA back-pressure; advance only when
      // the current byte is accepted.
      if(state==H_FEED && sha_in_ready)
        outputs_read_address=byte_index+1'b1;
      else
        outputs_read_address=byte_index;
    end
    // The UART bridge may read the frozen request only while the hashing
    // engine is idle.  Reusing this BSRAM port avoids a second 256-bit
    // freeze_id register bank in the bridge.
    if(buffer_read_enable && state==H_IDLE)
      outputs_read_address=buffer_read_address;
    case(active_pass)
      P_PREVOUT_1:sha_in_byte=SERIALIZED_REQUEST ? outputs_read_byte :
                             outpoint[287-byte_index*8-:8];
      P_PREVOUT_2,P_SEQUENCE_2,P_OUTPUTS_2,P_PREIMAGE_2:
        sha_in_byte=vector256_byte(first_digest,byte_index[5:0]);
      P_SEQUENCE_1:sha_in_byte=SERIALIZED_REQUEST ? outputs_read_byte :
                             input_sequence[31-byte_index*8-:8];
      P_OUTPUTS_1:sha_in_byte=outputs_read_byte;
      default:begin
        if(SERIALIZED_REQUEST && request_byte_selected(active_pass,byte_index))
          sha_in_byte=outputs_read_byte;
        else if(byte_index<4)
          sha_in_byte=tx_version[31-byte_index*8-:8];
        else if(byte_index<36)
          sha_in_byte=vector256_byte(hash_prevouts,byte_index-4);
        else if(byte_index<68)
          sha_in_byte=vector256_byte(hash_sequence,byte_index-36);
        else if(byte_index<104)
          sha_in_byte=outpoint[287-(byte_index-68)*8-:8];
        else if(byte_index==104) sha_in_byte=8'h19;
        else if(byte_index==105) sha_in_byte=8'h76;
        else if(byte_index==106) sha_in_byte=8'ha9;
        else if(byte_index==107) sha_in_byte=8'h14;
        else if(byte_index<128)
          sha_in_byte=pubkey_hash[159-(byte_index-108)*8-:8];
        else if(byte_index==128) sha_in_byte=8'h88;
        else if(byte_index==129) sha_in_byte=8'hac;
        else if(byte_index<138)
          sha_in_byte=prevout_amount[63-(byte_index-130)*8-:8];
        else if(byte_index<142)
          sha_in_byte=input_sequence[31-(byte_index-138)*8-:8];
        else if(byte_index<174)
          sha_in_byte=vector256_byte(hash_outputs,byte_index-142);
        else if(byte_index<178)
          sha_in_byte=locktime[31-(byte_index-174)*8-:8];
        else
          sha_in_byte=sighash_type[31-(byte_index-178)*8-:8];
      end
    endcase
  end

  always_ff @(posedge clk or negedge reset_n) begin
    if(!reset_n) begin
      state<=H_IDLE;active_pass<=P_PREVOUT_1;byte_index<=0;pass_length<=0;
      first_digest<=0;hash_prevouts<=0;hash_sequence<=0;hash_outputs<=0;
      busy<=0;done<=0;error<=0;digest<=0;
    end else begin
      done<=1'b0;
      case(state)
        H_IDLE:if(start) begin
          error<=1'b0;digest<=0;
          if(outputs_length==0 || outputs_length>MAX_OUTPUT_BYTES ||
             sighash_type!=32'h01000000) begin
            error<=1'b1;done<=1'b1;
          end else begin
            busy<=1'b1;active_pass<=P_PREVOUT_1;pass_length<=10'd36;
            byte_index<=0;state<=H_START;
          end
        end
        H_START:state<=H_WAIT_READY;
        H_WAIT_READY:if(sha_in_ready) begin byte_index<=0;state<=H_FEED;end
        H_FEED:if(sha_in_ready) begin
          if(byte_index+1'b1==pass_length) state<=H_WAIT_DONE;
          else byte_index<=byte_index+1'b1;
        end
        H_WAIT_DONE:if(sha_done) begin
          byte_index<=0;
          case(active_pass)
            P_PREVOUT_1:begin first_digest<=sha_digest;active_pass<=P_PREVOUT_2;pass_length<=32;state<=H_START;end
            P_PREVOUT_2:begin hash_prevouts<=sha_digest;active_pass<=P_SEQUENCE_1;pass_length<=4;state<=H_START;end
            P_SEQUENCE_1:begin first_digest<=sha_digest;active_pass<=P_SEQUENCE_2;pass_length<=32;state<=H_START;end
            P_SEQUENCE_2:begin hash_sequence<=sha_digest;active_pass<=P_OUTPUTS_1;pass_length<=outputs_length;state<=H_START;end
            P_OUTPUTS_1:begin first_digest<=sha_digest;active_pass<=P_OUTPUTS_2;pass_length<=32;state<=H_START;end
            P_OUTPUTS_2:begin hash_outputs<=sha_digest;active_pass<=P_PREIMAGE_1;pass_length<=10'd182;state<=H_START;end
            P_PREIMAGE_1:begin first_digest<=sha_digest;active_pass<=P_PREIMAGE_2;pass_length<=32;state<=H_START;end
            default:begin digest<=sha_digest;busy<=1'b0;done<=1'b1;state<=H_IDLE;end
          endcase
        end
        default:begin busy<=1'b0;error<=1'b1;done<=1'b1;state<=H_IDLE;end
      endcase
    end
  end
endmodule
