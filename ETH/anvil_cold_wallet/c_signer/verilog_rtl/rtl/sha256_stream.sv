`timescale 1ns/1ps

// Compact streaming SHA-256.  The 64-byte input block and 64-word message
// schedule use synchronous BSRAM; schedule words are assembled/read serially.
module sha256_stream (
  input  logic         clk,
  input  logic         reset_n,
  input  logic         start,
  input  logic         in_valid,
  output logic         in_ready,
  input  logic [7:0]   in_byte,
  input  logic         in_last,
  output logic         busy,
  output logic         done,
  output logic [255:0] digest
);
  typedef enum logic [4:0] {
    S_IDLE,S_ABSORB,S_PAD,S_COMPRESS_INIT,
    S_BYTE_REQ,S_BYTE_CAPTURE,S_SCHED_REQ,S_SCHED_CAPTURE,
    S_SCHEDULE_PREP,S_W_ADD0,S_W_ADD1,S_W_ADD2,
    S_T1_ADD0,S_T1_ADD1,S_T1_ADD2,S_T1_ADD3,S_T2_ADD,
    S_NEXT_A,S_NEXT_E,S_COMMIT,S_FINAL_ADD
  } state_t;
  typedef enum logic [1:0] {
    AFTER_CONTINUE,AFTER_OUTPUT,AFTER_SECOND_PAD,AFTER_SECOND_LENGTH
  } after_action_t;

  state_t state;
  after_action_t compress_action,pad_after_action;

  (* ram_style="block", syn_ramstyle="block_ram", no_rw_check *)
  logic [7:0] block_memory [0:63];
  (* ram_style="block", syn_ramstyle="block_ram", no_rw_check *)
  logic [31:0] schedule_memory [0:63];
  logic [5:0] block_read_address,block_write_address;
  logic [7:0] block_read_data,block_write_data;
  logic block_write_enable;
  logic [5:0] schedule_read_address,schedule_write_address;
  logic [6:0] schedule_read_index;
  logic [31:0] schedule_read_data,schedule_write_data;
  logic schedule_write_enable;

  logic [5:0] byte_position;
  logic [63:0] total_bytes,final_bit_length;
  logic [5:0] pad_position,pad_marker_position;
  logic pad_insert_marker,pad_include_length;

  logic [6:0] round_index;
  logic [1:0] byte_subindex,schedule_subindex;
  logic [31:0] word_assembly;
  logic [31:0] word_m15,word_m2,word_m16,word_m7;
  logic [31:0] schedule_word_reg;
  logic [31:0] add_acc,temp1_reg,temp2_reg,next_a_reg,next_e_reg;
  logic [31:0] adder_a,adder_b,adder_sum;
  logic [2:0] final_index;

  logic [31:0] h0,h1,h2,h3,h4,h5,h6,h7;
  logic [31:0] a,b,c,d,e,f,g,h;

  assign in_ready=(state==S_ABSORB);

  function automatic logic [31:0] rotate_right32(
    input logic [31:0] value,input integer amount
  );
    rotate_right32=(value>>amount)|(value<<(32-amount));
  endfunction

  function automatic logic [31:0] sha_constant(input logic [5:0] index);
    begin
      case(index)
         0:sha_constant=32'h428a2f98;  1:sha_constant=32'h71374491;
         2:sha_constant=32'hb5c0fbcf;  3:sha_constant=32'he9b5dba5;
         4:sha_constant=32'h3956c25b;  5:sha_constant=32'h59f111f1;
         6:sha_constant=32'h923f82a4;  7:sha_constant=32'hab1c5ed5;
         8:sha_constant=32'hd807aa98;  9:sha_constant=32'h12835b01;
        10:sha_constant=32'h243185be; 11:sha_constant=32'h550c7dc3;
        12:sha_constant=32'h72be5d74; 13:sha_constant=32'h80deb1fe;
        14:sha_constant=32'h9bdc06a7; 15:sha_constant=32'hc19bf174;
        16:sha_constant=32'he49b69c1; 17:sha_constant=32'hefbe4786;
        18:sha_constant=32'h0fc19dc6; 19:sha_constant=32'h240ca1cc;
        20:sha_constant=32'h2de92c6f; 21:sha_constant=32'h4a7484aa;
        22:sha_constant=32'h5cb0a9dc; 23:sha_constant=32'h76f988da;
        24:sha_constant=32'h983e5152; 25:sha_constant=32'ha831c66d;
        26:sha_constant=32'hb00327c8; 27:sha_constant=32'hbf597fc7;
        28:sha_constant=32'hc6e00bf3; 29:sha_constant=32'hd5a79147;
        30:sha_constant=32'h06ca6351; 31:sha_constant=32'h14292967;
        32:sha_constant=32'h27b70a85; 33:sha_constant=32'h2e1b2138;
        34:sha_constant=32'h4d2c6dfc; 35:sha_constant=32'h53380d13;
        36:sha_constant=32'h650a7354; 37:sha_constant=32'h766a0abb;
        38:sha_constant=32'h81c2c92e; 39:sha_constant=32'h92722c85;
        40:sha_constant=32'ha2bfe8a1; 41:sha_constant=32'ha81a664b;
        42:sha_constant=32'hc24b8b70; 43:sha_constant=32'hc76c51a3;
        44:sha_constant=32'hd192e819; 45:sha_constant=32'hd6990624;
        46:sha_constant=32'hf40e3585; 47:sha_constant=32'h106aa070;
        48:sha_constant=32'h19a4c116; 49:sha_constant=32'h1e376c08;
        50:sha_constant=32'h2748774c; 51:sha_constant=32'h34b0bcb5;
        52:sha_constant=32'h391c0cb3; 53:sha_constant=32'h4ed8aa4a;
        54:sha_constant=32'h5b9cca4f; 55:sha_constant=32'h682e6ff3;
        56:sha_constant=32'h748f82ee; 57:sha_constant=32'h78a5636f;
        58:sha_constant=32'h84c87814; 59:sha_constant=32'h8cc70208;
        60:sha_constant=32'h90befffa; 61:sha_constant=32'ha4506ceb;
        62:sha_constant=32'hbef9a3f7;63:sha_constant=32'hc67178f2;
        default:sha_constant=0;
      endcase
    end
  endfunction

  always_ff @(posedge clk) begin
    if(block_write_enable) block_memory[block_write_address]<=block_write_data;
    if(schedule_write_enable) schedule_memory[schedule_write_address]<=schedule_write_data;
    block_read_data<=block_memory[block_read_address];
    schedule_read_data<=schedule_memory[schedule_read_address];
  end

  always_comb begin
    block_read_address=0;block_write_enable=0;block_write_address=0;block_write_data=0;
    schedule_read_address=0;schedule_write_enable=0;schedule_write_address=0;schedule_write_data=0;
    schedule_read_index=0;

    if(state==S_ABSORB && in_valid) begin
      block_write_enable=1;block_write_address=byte_position;block_write_data=in_byte;
    end else if(state==S_PAD) begin
      block_write_enable=1;block_write_address=pad_position;block_write_data=0;
      if(pad_insert_marker && pad_position==pad_marker_position)
        block_write_data=8'h80;
      else if(pad_include_length && pad_position>=56) begin
        case(pad_position[2:0])
          0:block_write_data=final_bit_length[63:56];
          1:block_write_data=final_bit_length[55:48];
          2:block_write_data=final_bit_length[47:40];
          3:block_write_data=final_bit_length[39:32];
          4:block_write_data=final_bit_length[31:24];
          5:block_write_data=final_bit_length[23:16];
          6:block_write_data=final_bit_length[15:8];
          default:block_write_data=final_bit_length[7:0];
        endcase
      end
    end

    if(state==S_BYTE_REQ)
      block_read_address={round_index[3:0],byte_subindex};
    if(state==S_SCHED_REQ) begin
      case(schedule_subindex)
        0:schedule_read_index=round_index-7'd15;
        1:schedule_read_index=round_index-7'd2;
        2:schedule_read_index=round_index-7'd16;
        default:schedule_read_index=round_index-7'd7;
      endcase
      schedule_read_address=schedule_read_index[5:0];
    end

    if(state==S_COMMIT) begin
      schedule_write_enable=1;schedule_write_address=round_index[5:0];
      schedule_write_data=schedule_word_reg;
    end

    // One physical 32-bit adder is selected by the round micro-operations.
    adder_a=0;adder_b=0;
    case(state)
      S_W_ADD0:begin
        adder_a=word_m16;
        adder_b=rotate_right32(word_m15,7)^rotate_right32(word_m15,18)^(word_m15>>3);
      end
      S_W_ADD1:begin adder_a=add_acc;adder_b=word_m7;end
      S_W_ADD2:begin
        adder_a=add_acc;
        adder_b=rotate_right32(word_m2,17)^rotate_right32(word_m2,19)^(word_m2>>10);
      end
      S_T1_ADD0:begin
        adder_a=h;
        adder_b=rotate_right32(e,6)^rotate_right32(e,11)^rotate_right32(e,25);
      end
      S_T1_ADD1:begin adder_a=add_acc;adder_b=(e&f)^((~e)&g);end
      S_T1_ADD2:begin adder_a=add_acc;adder_b=sha_constant(round_index[5:0]);end
      S_T1_ADD3:begin adder_a=add_acc;adder_b=schedule_word_reg;end
      S_T2_ADD:begin
        adder_a=rotate_right32(a,2)^rotate_right32(a,13)^rotate_right32(a,22);
        adder_b=(a&b)^(a&c)^(b&c);
      end
      S_NEXT_A:begin adder_a=temp1_reg;adder_b=temp2_reg;end
      S_NEXT_E:begin adder_a=d;adder_b=temp1_reg;end
      S_FINAL_ADD:begin
        case(final_index)
          0:begin adder_a=h0;adder_b=a;end
          1:begin adder_a=h1;adder_b=b;end
          2:begin adder_a=h2;adder_b=c;end
          3:begin adder_a=h3;adder_b=d;end
          4:begin adder_a=h4;adder_b=e;end
          5:begin adder_a=h5;adder_b=f;end
          6:begin adder_a=h6;adder_b=g;end
          default:begin adder_a=h7;adder_b=h;end
        endcase
      end
      default:begin end
    endcase
    adder_sum=adder_a+adder_b;
  end

  always_ff @(posedge clk or negedge reset_n) begin
    if(!reset_n) begin
      state<=S_IDLE;compress_action<=AFTER_CONTINUE;pad_after_action<=AFTER_OUTPUT;
      byte_position<=0;total_bytes<=0;final_bit_length<=0;
      pad_position<=0;pad_marker_position<=0;pad_insert_marker<=0;pad_include_length<=0;
      round_index<=0;byte_subindex<=0;schedule_subindex<=0;word_assembly<=0;
      word_m15<=0;word_m2<=0;word_m16<=0;word_m7<=0;
      schedule_word_reg<=0;add_acc<=0;temp1_reg<=0;temp2_reg<=0;
      next_a_reg<=0;next_e_reg<=0;final_index<=0;
      h0<=0;h1<=0;h2<=0;h3<=0;h4<=0;h5<=0;h6<=0;h7<=0;
      a<=0;b<=0;c<=0;d<=0;e<=0;f<=0;g<=0;h<=0;
      busy<=0;done<=0;digest<=0;
    end else begin
      done<=0;
      case(state)
        S_IDLE:if(start) begin
          h0<=32'h6a09e667;h1<=32'hbb67ae85;h2<=32'h3c6ef372;h3<=32'ha54ff53a;
          h4<=32'h510e527f;h5<=32'h9b05688c;h6<=32'h1f83d9ab;h7<=32'h5be0cd19;
          byte_position<=0;total_bytes<=0;busy<=1;state<=S_ABSORB;
        end
        S_ABSORB:if(in_valid) begin
          total_bytes<=total_bytes+1'b1;
          if(in_last) begin
            final_bit_length<=(total_bytes+1'b1)<<3;
            if(byte_position<=54) begin
              pad_position<=byte_position+1'b1;pad_marker_position<=byte_position+1'b1;
              pad_insert_marker<=1;pad_include_length<=1;pad_after_action<=AFTER_OUTPUT;
              state<=S_PAD;
            end else if(byte_position<63) begin
              pad_position<=byte_position+1'b1;pad_marker_position<=byte_position+1'b1;
              pad_insert_marker<=1;pad_include_length<=0;pad_after_action<=AFTER_SECOND_LENGTH;
              state<=S_PAD;
            end else begin compress_action<=AFTER_SECOND_PAD;state<=S_COMPRESS_INIT;end
          end else if(byte_position==63) begin
            compress_action<=AFTER_CONTINUE;state<=S_COMPRESS_INIT;
          end else byte_position<=byte_position+1'b1;
        end
        S_PAD:begin
          if(pad_position==63) begin compress_action<=pad_after_action;state<=S_COMPRESS_INIT;end
          else pad_position<=pad_position+1'b1;
        end
        S_COMPRESS_INIT:begin
          a<=h0;b<=h1;c<=h2;d<=h3;e<=h4;f<=h5;g<=h6;h<=h7;
          round_index<=0;byte_subindex<=0;word_assembly<=0;state<=S_BYTE_REQ;
        end
        S_BYTE_REQ:state<=S_BYTE_CAPTURE;
        S_BYTE_CAPTURE:begin
          case(byte_subindex)
            0:word_assembly[31:24]<=block_read_data;
            1:word_assembly[23:16]<=block_read_data;
            2:word_assembly[15:8]<=block_read_data;
            default:word_assembly[7:0]<=block_read_data;
          endcase
          if(byte_subindex==3) state<=S_SCHEDULE_PREP;
          else begin byte_subindex<=byte_subindex+1'b1;state<=S_BYTE_REQ;end
        end
        S_SCHED_REQ:state<=S_SCHED_CAPTURE;
        S_SCHED_CAPTURE:begin
          case(schedule_subindex)
            0:word_m15<=schedule_read_data;1:word_m2<=schedule_read_data;
            2:word_m16<=schedule_read_data;default:word_m7<=schedule_read_data;
          endcase
          if(schedule_subindex==3) state<=S_SCHEDULE_PREP;
          else begin schedule_subindex<=schedule_subindex+1'b1;state<=S_SCHED_REQ;end
        end
        S_SCHEDULE_PREP:begin
          if(round_index<16) begin schedule_word_reg<=word_assembly;state<=S_T1_ADD0;end
          else state<=S_W_ADD0;
        end
        S_W_ADD0:begin add_acc<=adder_sum;state<=S_W_ADD1;end
        S_W_ADD1:begin add_acc<=adder_sum;state<=S_W_ADD2;end
        S_W_ADD2:begin schedule_word_reg<=adder_sum;state<=S_T1_ADD0;end
        S_T1_ADD0:begin add_acc<=adder_sum;state<=S_T1_ADD1;end
        S_T1_ADD1:begin add_acc<=adder_sum;state<=S_T1_ADD2;end
        S_T1_ADD2:begin add_acc<=adder_sum;state<=S_T1_ADD3;end
        S_T1_ADD3:begin temp1_reg<=adder_sum;state<=S_T2_ADD;end
        S_T2_ADD:begin temp2_reg<=adder_sum;state<=S_NEXT_A;end
        S_NEXT_A:begin next_a_reg<=adder_sum;state<=S_NEXT_E;end
        S_NEXT_E:begin next_e_reg<=adder_sum;state<=S_COMMIT;end
        S_COMMIT:begin
          a<=next_a_reg;b<=a;c<=b;d<=c;e<=next_e_reg;f<=e;g<=f;h<=g;
          if(round_index==63) begin final_index<=0;state<=S_FINAL_ADD;end
          else begin
            round_index<=round_index+1'b1;
            if(round_index<15) begin byte_subindex<=0;word_assembly<=0;state<=S_BYTE_REQ;end
            else begin schedule_subindex<=0;state<=S_SCHED_REQ;end
          end
        end
        S_FINAL_ADD:begin
          case(final_index)
            0:begin h0<=adder_sum;digest[255:224]<=adder_sum;end
            1:begin h1<=adder_sum;digest[223:192]<=adder_sum;end
            2:begin h2<=adder_sum;digest[191:160]<=adder_sum;end
            3:begin h3<=adder_sum;digest[159:128]<=adder_sum;end
            4:begin h4<=adder_sum;digest[127:96]<=adder_sum;end
            5:begin h5<=adder_sum;digest[95:64]<=adder_sum;end
            6:begin h6<=adder_sum;digest[63:32]<=adder_sum;end
            default:begin h7<=adder_sum;digest[31:0]<=adder_sum;end
          endcase
          if(final_index==7) begin
            case(compress_action)
              AFTER_CONTINUE:begin byte_position<=0;state<=S_ABSORB;end
              AFTER_SECOND_PAD:begin
                pad_position<=0;pad_marker_position<=0;pad_insert_marker<=1;pad_include_length<=1;
                pad_after_action<=AFTER_OUTPUT;state<=S_PAD;
              end
              AFTER_SECOND_LENGTH:begin
                pad_position<=0;pad_marker_position<=0;pad_insert_marker<=0;pad_include_length<=1;
                pad_after_action<=AFTER_OUTPUT;state<=S_PAD;
              end
              default:begin busy<=0;done<=1;state<=S_IDLE;end
            endcase
          end else final_index<=final_index+1'b1;
        end
        default:state<=S_IDLE;
      endcase
    end
  end
endmodule
