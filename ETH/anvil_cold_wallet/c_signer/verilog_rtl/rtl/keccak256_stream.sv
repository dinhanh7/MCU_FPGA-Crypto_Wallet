`timescale 1ns/1ps

// Compact Keccak-256 using two 25x64 synchronous RAM banks.  The architecture
// is lane-serial: area is prioritized over throughput for an offline signer.
module keccak256_stream (
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
    K_IDLE, K_CLEAR, K_ABSORB, K_ABSORB_WRITE,
    K_PAD_DELIM_REQ, K_PAD_DELIM_WRITE, K_PAD_FINAL_WRITE,
    K_ROUND_INIT, K_THETA_REQ, K_THETA_ACCUM,
    K_TR_REQ, K_TR_CAPTURE, K_TR_ROTATE, K_TR_WRITE,
    K_CHI_REQ, K_CHI_CAPTURE, K_CHI_WRITE,
    K_EMPTY_PAD_REQ0, K_EMPTY_PAD_WRITE0, K_EMPTY_PAD_WRITE16,
    K_DIGEST_REQ, K_DIGEST_CAPTURE
  } state_t;
  typedef enum logic [1:0] {
    AFTER_CONTINUE, AFTER_OUTPUT, AFTER_PAD_EMPTY
  } after_action_t;

  state_t state;
  after_action_t after_action;

  (* ram_style="block", syn_ramstyle="block_ram", no_rw_check *)
  logic [63:0] state_a [0:31];
  (* ram_style="block", syn_ramstyle="block_ram", no_rw_check *)
  logic [63:0] state_b [0:31];
  logic [4:0] a_read_address, b_read_address;
  logic [63:0] a_read_data, b_read_data;
  logic a_write_enable, b_write_enable;
  logic [4:0] a_write_address, b_write_address;
  logic [63:0] a_write_data, b_write_data;

  logic [63:0] column_parity [0:4];
  logic [63:0] chi_row [0:4];
  logic [63:0] theta_delta;
  logic [5:0] lane_counter;
  logic [2:0] column_counter;
  logic [2:0] row_counter;
  logic [2:0] chi_write_counter;
  logic [4:0] round_index;
  logic [4:0] clear_counter;
  logic [2:0] digest_counter;
  logic [7:0] byte_position;
  logic [63:0] rho_value;
  logic [5:0] rho_remaining;
  logic [4:0] rho_destination;

  logic [7:0] accepted_byte;
  logic       accepted_last;
  logic [7:0] accepted_position;
  logic [7:0] pad_position;

  assign in_ready = (state == K_ABSORB);
  assign pad_position = accepted_position + 1'b1;

  function automatic logic [63:0] rotate_left64(
    input logic [63:0] value,
    input integer amount
  );
    begin
      if (amount == 0) rotate_left64=value;
      else rotate_left64=(value<<amount)|(value>>(64-amount));
    end
  endfunction

  function automatic logic [63:0] place_byte(
    input logic [7:0] value,
    input logic [2:0] offset
  );
    begin
      case (offset)
        0: place_byte={56'b0,value};
        1: place_byte={48'b0,value,8'b0};
        2: place_byte={40'b0,value,16'b0};
        3: place_byte={32'b0,value,24'b0};
        4: place_byte={24'b0,value,32'b0};
        5: place_byte={16'b0,value,40'b0};
        6: place_byte={8'b0,value,48'b0};
        default: place_byte={value,56'b0};
      endcase
    end
  endfunction

  function automatic integer rho_offset(input logic [4:0] index);
    begin
      case (index)
         0:rho_offset=0;   1:rho_offset=1;   2:rho_offset=62;
         3:rho_offset=28;  4:rho_offset=27;  5:rho_offset=36;
         6:rho_offset=44;  7:rho_offset=6;   8:rho_offset=55;
         9:rho_offset=20; 10:rho_offset=3;  11:rho_offset=10;
        12:rho_offset=43; 13:rho_offset=25; 14:rho_offset=39;
        15:rho_offset=41; 16:rho_offset=45; 17:rho_offset=15;
        18:rho_offset=21; 19:rho_offset=8;  20:rho_offset=18;
        21:rho_offset=2;  22:rho_offset=61; 23:rho_offset=56;
        24:rho_offset=14;
        default:rho_offset=0;
      endcase
    end
  endfunction

  function automatic logic [4:0] pi_destination(input logic [4:0] index);
    begin
      case (index)
         0:pi_destination=0;  1:pi_destination=10; 2:pi_destination=20;
         3:pi_destination=5;  4:pi_destination=15; 5:pi_destination=16;
         6:pi_destination=1;  7:pi_destination=11; 8:pi_destination=21;
         9:pi_destination=6; 10:pi_destination=7; 11:pi_destination=17;
        12:pi_destination=2; 13:pi_destination=12;14:pi_destination=22;
        15:pi_destination=23;16:pi_destination=8; 17:pi_destination=18;
        18:pi_destination=3; 19:pi_destination=13;20:pi_destination=14;
        21:pi_destination=24;22:pi_destination=9; 23:pi_destination=19;
        24:pi_destination=4;
        default:pi_destination=0;
      endcase
    end
  endfunction

  function automatic logic [63:0] round_constant(input logic [4:0] index);
    begin
      case (index)
         0:round_constant=64'h0000000000000001;
         1:round_constant=64'h0000000000008082;
         2:round_constant=64'h800000000000808a;
         3:round_constant=64'h8000000080008000;
         4:round_constant=64'h000000000000808b;
         5:round_constant=64'h0000000080000001;
         6:round_constant=64'h8000000080008081;
         7:round_constant=64'h8000000000008009;
         8:round_constant=64'h000000000000008a;
         9:round_constant=64'h0000000000000088;
        10:round_constant=64'h0000000080008009;
        11:round_constant=64'h000000008000000a;
        12:round_constant=64'h000000008000808b;
        13:round_constant=64'h800000000000008b;
        14:round_constant=64'h8000000000008089;
        15:round_constant=64'h8000000000008003;
        16:round_constant=64'h8000000000008002;
        17:round_constant=64'h8000000000000080;
        18:round_constant=64'h000000000000800a;
        19:round_constant=64'h800000008000000a;
        20:round_constant=64'h8000000080008081;
        21:round_constant=64'h8000000000008080;
        22:round_constant=64'h0000000080000001;
        23:round_constant=64'h8000000080008008;
        default:round_constant=0;
      endcase
    end
  endfunction

  function automatic logic [63:0] reverse_bytes64(input logic [63:0] value);
    reverse_bytes64={value[7:0],value[15:8],value[23:16],value[31:24],
      value[39:32],value[47:40],value[55:48],value[63:56]};
  endfunction

  function automatic logic [63:0] chi_lane(
    input logic [63:0] a,input logic [63:0] b,input logic [63:0] c
  );
    chi_lane=a^((~b)&c);
  endfunction

  function automatic logic [4:0] row_lane_address(
    input logic [2:0] row,input logic [2:0] column
  );
    begin
      case (row)
        0:row_lane_address={2'b0,column};
        1:row_lane_address=5+column;
        2:row_lane_address=10+column;
        3:row_lane_address=15+column;
        default:row_lane_address=20+column;
      endcase
    end
  endfunction

  always_ff @(posedge clk) begin
    if (a_write_enable) state_a[a_write_address] <= a_write_data;
    if (b_write_enable) state_b[b_write_address] <= b_write_data;
    a_read_data <= state_a[a_read_address];
    b_read_data <= state_b[b_read_address];
  end

  always_comb begin
    a_read_address=0; b_read_address=0;
    a_write_enable=0; b_write_enable=0;
    a_write_address=0; b_write_address=0;
    a_write_data=0; b_write_data=0;

    // During rho/pi, column_parity is rotated after every lane.  Therefore
    // these two fixed taps always represent C[x-1] and C[x+1].
    theta_delta=column_parity[4]^{column_parity[1][62:0],column_parity[1][63]};

    case (state)
      K_CLEAR: begin
        a_write_enable=1; a_write_address=clear_counter; a_write_data=0;
      end
      K_ABSORB: a_read_address=byte_position[7:3];
      K_ABSORB_WRITE: begin
        a_write_enable=1; a_write_address=accepted_position[7:3];
        a_write_data=a_read_data^place_byte(accepted_byte,accepted_position[2:0]);
      end
      K_PAD_DELIM_REQ: a_read_address=pad_position[7:3];
      K_PAD_DELIM_WRITE: begin
        a_write_enable=1; a_write_address=pad_position[7:3];
        a_write_data=a_read_data^place_byte(8'h01,pad_position[2:0]);
        if (pad_position[7:3]==5'd16)
          a_write_data=a_write_data^64'h8000000000000000;
        else
          a_read_address=5'd16;
      end
      K_PAD_FINAL_WRITE: begin
        a_write_enable=1; a_write_address=5'd16;
        a_write_data=a_read_data^64'h8000000000000000;
      end
      K_THETA_REQ: a_read_address=lane_counter[4:0];
      K_TR_REQ: a_read_address=lane_counter[4:0];
      K_TR_WRITE: begin
        b_write_enable=1;
        b_write_address=rho_destination;
        b_write_data=rho_value;
      end
      K_CHI_REQ: b_read_address=row_lane_address(row_counter,column_counter);
      K_CHI_WRITE: begin
        a_write_enable=1;
        a_write_address=row_lane_address(row_counter,chi_write_counter);
        // chi_row is rotated after each write, so one fixed three-lane tap
        // produces all five Chi outputs without a 64-bit 5:1 mux.
        a_write_data=chi_lane(chi_row[0],chi_row[1],chi_row[2]);
        if (row_counter==0 && chi_write_counter==0)
          a_write_data=a_write_data^round_constant(round_index);
      end
      K_EMPTY_PAD_REQ0: a_read_address=0;
      K_EMPTY_PAD_WRITE0: begin
        a_write_enable=1;a_write_address=0;
        a_write_data=a_read_data^64'h1;a_read_address=5'd16;
      end
      K_EMPTY_PAD_WRITE16: begin
        a_write_enable=1;a_write_address=5'd16;
        a_write_data=a_read_data^64'h8000000000000000;
      end
      K_DIGEST_REQ: a_read_address={2'b0,digest_counter};
      default: begin end
    endcase
  end

  integer reset_i;
  always_ff @(posedge clk or negedge reset_n) begin
    if (!reset_n) begin
      state<=K_IDLE;after_action<=AFTER_CONTINUE;
      for (reset_i=0;reset_i<5;reset_i=reset_i+1) begin
        column_parity[reset_i]<=0;chi_row[reset_i]<=0;
      end
      lane_counter<=0;column_counter<=0;row_counter<=0;chi_write_counter<=0;
      round_index<=0;clear_counter<=0;digest_counter<=0;byte_position<=0;
      rho_value<=0;rho_remaining<=0;rho_destination<=0;
      accepted_byte<=0;accepted_last<=0;accepted_position<=0;
      busy<=0;done<=0;digest<=0;
    end else begin
      done<=0;
      case (state)
        K_IDLE: if (start) begin
          clear_counter<=0;busy<=1;state<=K_CLEAR;
        end
        K_CLEAR: begin
          if (clear_counter==24) begin byte_position<=0;state<=K_ABSORB;end
          else clear_counter<=clear_counter+1'b1;
        end
        K_ABSORB: if (in_valid) begin
          accepted_byte<=in_byte;accepted_last<=in_last;
          accepted_position<=byte_position;state<=K_ABSORB_WRITE;
        end
        K_ABSORB_WRITE: begin
          if (accepted_last) begin
            if (accepted_position==135) begin
              after_action<=AFTER_PAD_EMPTY;state<=K_ROUND_INIT;
            end else state<=K_PAD_DELIM_REQ;
          end else if (accepted_position==135) begin
            after_action<=AFTER_CONTINUE;state<=K_ROUND_INIT;
          end else begin
            byte_position<=accepted_position+1'b1;state<=K_ABSORB;
          end
        end
        K_PAD_DELIM_REQ: state<=K_PAD_DELIM_WRITE;
        K_PAD_DELIM_WRITE: begin
          if (pad_position[7:3]==16) begin
            after_action<=AFTER_OUTPUT;state<=K_ROUND_INIT;
          end else state<=K_PAD_FINAL_WRITE;
        end
        K_PAD_FINAL_WRITE: begin after_action<=AFTER_OUTPUT;state<=K_ROUND_INIT;end

        K_ROUND_INIT: begin
          for (reset_i=0;reset_i<5;reset_i=reset_i+1) column_parity[reset_i]<=0;
          lane_counter<=0;column_counter<=0;state<=K_THETA_REQ;
        end
        K_THETA_REQ: state<=K_THETA_ACCUM;
        K_THETA_ACCUM: begin
          case (column_counter)
            0:column_parity[0]<=column_parity[0]^a_read_data;
            1:column_parity[1]<=column_parity[1]^a_read_data;
            2:column_parity[2]<=column_parity[2]^a_read_data;
            3:column_parity[3]<=column_parity[3]^a_read_data;
            default:column_parity[4]<=column_parity[4]^a_read_data;
          endcase
          if (lane_counter==24) begin
            lane_counter<=0;column_counter<=0;state<=K_TR_REQ;
          end else begin
            lane_counter<=lane_counter+1'b1;
            if (column_counter==4) column_counter<=0;
            else column_counter<=column_counter+1'b1;
            state<=K_THETA_REQ;
          end
        end
        K_TR_REQ: state<=K_TR_CAPTURE;
        K_TR_CAPTURE:begin
          rho_value<=a_read_data^theta_delta;
          rho_remaining<=rho_offset(lane_counter[4:0]);
          rho_destination<=pi_destination(lane_counter[4:0]);
          state<=K_TR_ROTATE;
        end
        K_TR_ROTATE:begin
          if(rho_remaining==0)
            state<=K_TR_WRITE;
          else begin
            rho_value<={rho_value[62:0],rho_value[63]};
            rho_remaining<=rho_remaining-1'b1;
          end
        end
        K_TR_WRITE: begin
          column_parity[0]<=column_parity[1];
          column_parity[1]<=column_parity[2];
          column_parity[2]<=column_parity[3];
          column_parity[3]<=column_parity[4];
          column_parity[4]<=column_parity[0];
          if (lane_counter==24) begin
            row_counter<=0;column_counter<=0;state<=K_CHI_REQ;
          end else begin
            lane_counter<=lane_counter+1'b1;
            state<=K_TR_REQ;
          end
        end
        K_CHI_REQ: state<=K_CHI_CAPTURE;
        K_CHI_CAPTURE: begin
          case (column_counter)
            0:chi_row[0]<=b_read_data;1:chi_row[1]<=b_read_data;
            2:chi_row[2]<=b_read_data;3:chi_row[3]<=b_read_data;
            default:chi_row[4]<=b_read_data;
          endcase
          if (column_counter==4) begin chi_write_counter<=0;state<=K_CHI_WRITE;end
          else begin column_counter<=column_counter+1'b1;state<=K_CHI_REQ;end
        end
        K_CHI_WRITE: begin
          chi_row[0]<=chi_row[1];
          chi_row[1]<=chi_row[2];
          chi_row[2]<=chi_row[3];
          chi_row[3]<=chi_row[4];
          chi_row[4]<=chi_row[0];
          if (chi_write_counter==4) begin
            if (row_counter==4) begin
              if (round_index==23) begin
                round_index<=0;
                case (after_action)
                  AFTER_CONTINUE: begin byte_position<=0;state<=K_ABSORB;end
                  AFTER_PAD_EMPTY: state<=K_EMPTY_PAD_REQ0;
                  default: begin digest_counter<=0;state<=K_DIGEST_REQ;end
                endcase
              end else begin
                round_index<=round_index+1'b1;state<=K_ROUND_INIT;
              end
            end else begin
              row_counter<=row_counter+1'b1;column_counter<=0;state<=K_CHI_REQ;
            end
          end else chi_write_counter<=chi_write_counter+1'b1;
        end

        K_EMPTY_PAD_REQ0: state<=K_EMPTY_PAD_WRITE0;
        K_EMPTY_PAD_WRITE0: state<=K_EMPTY_PAD_WRITE16;
        K_EMPTY_PAD_WRITE16: begin after_action<=AFTER_OUTPUT;state<=K_ROUND_INIT;end

        K_DIGEST_REQ: state<=K_DIGEST_CAPTURE;
        K_DIGEST_CAPTURE: begin
          case (digest_counter)
            0:digest[255:192]<=reverse_bytes64(a_read_data);
            1:digest[191:128]<=reverse_bytes64(a_read_data);
            2:digest[127:64]<=reverse_bytes64(a_read_data);
            default:digest[63:0]<=reverse_bytes64(a_read_data);
          endcase
          if (digest_counter==3) begin busy<=0;done<=1;state<=K_IDLE;end
          else begin digest_counter<=digest_counter+1'b1;state<=K_DIGEST_REQ;end
        end
        default:state<=K_IDLE;
      endcase
    end
  end
endmodule
