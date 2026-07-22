`timescale 1ns/1ps

// Compact secp256k1 scalar multiplier controller. Point coordinates and
// temporaries live in BSRAM; arithmetic datapaths are supplied by the caller.
module secp256k1_point_mul_controller (
  input  logic         clk,
  input  logic         reset_n,
  input  logic         start,
  input  logic [255:0] scalar,
  output logic         busy,
  output logic         done,
  output logic         infinity,
  output logic [255:0] affine_x,
  output logic [255:0] affine_y,
  output logic         add_start,
  output logic         add_subtract,
  output logic [255:0] add_operand_a,
  output logic [255:0] add_operand_b,
  output logic [255:0] add_modulus,
  input  logic         add_done,
  input  logic [255:0] add_result,
  output logic         mul_start,
  output logic [255:0] mul_operand_a,
  output logic [255:0] mul_operand_b,
  output logic [255:0] mul_modulus,
  input  logic         mul_done,
  input  logic [255:0] mul_result,
  output logic         inv_start,
  output logic [255:0] inv_value,
  output logic [255:0] inv_modulus,
  input  logic         inv_done,
  input  logic [255:0] inv_result
);
`include "eth_secp256k1_common.svh"
  localparam logic [4:0]
    R_X=0,R_Y=1,R_Z=2,R_YY=3,R_YYYY=4,R_S=5,R_M=6,R_NEWX=7,
    R_NYP=8,R_Z2=9,R_U2=10,R_Z3=11,R_S2=12,R_H=13,R_HH=14,
    R_I=15,R_J=16,R_R=17,R_V=18,R_AYP=19,R_AYJ=20,R_ZINV=21,
    R_AZ2=22,R_AZ3=23,R_TEMP=24,R_GX=25,R_GY=26,R_ZERO=27,R_ONE=28;

  typedef enum logic [2:0] {P_DOUBLE,P_ADD_PRE,P_ADD_MAIN,P_AFFINE} program_t;
  typedef enum logic [1:0] {I_ADD,I_SUB,I_MUL} instruction_t;
  typedef enum logic [3:0] {
    O_IDLE,O_READ_A,O_READ_B,O_DISPATCH,O_ADD_START,O_ADD_WAIT,O_ADD_WRITE,
    O_MUL_START,O_MUL_WAIT,O_MUL_WRITE
  } op_state_t;
  typedef enum logic [5:0] {
    H_IDLE,H_INIT,H_BIT,H_SET_X,H_SET_Y,H_SET_Z,
    H_DOUBLE_START,H_DOUBLE_WAIT,H_AFTER_DOUBLE,
    H_ADD_PRE_START,H_ADD_PRE_WAIT,H_CMP_X_READ_A,H_CMP_X_READ_B,H_CMP_X_EVAL,
    H_CMP_Y_READ_A,H_CMP_Y_READ_B,H_CMP_Y_EVAL,H_ADD_MAIN_START,H_ADD_MAIN_WAIT,
    H_NEXT,H_INV_REQ,H_INV_START,H_INV_WAIT,H_INV_WRITE,
    H_AFFINE_START,H_AFFINE_WAIT,H_OUT_X_READ,H_OUT_Y_READ,H_OUT_CAPTURE
  } high_state_t;

  // A single synchronous read port is sequenced by the microcode.  This uses
  // half as many BSRAMs as a replicated two-read-port register file.
  (* ram_style="block", syn_ramstyle="block_ram", no_rw_check *)
  logic [255:0] register_file [0:31];
  logic [4:0] read_address,write_address;
  logic [255:0] read_data,operand_a_reg,operand_b_reg,compare_left,write_data;
  logic write_enable;

  high_state_t high_state;
  op_state_t op_state;
  program_t active_program;
  logic [4:0] program_counter;
  logic program_start,program_busy,program_done;
  program_t requested_program;
  instruction_t instruction;
  logic [4:0] instruction_dst,instruction_a,instruction_b;
  logic instruction_last;

  logic [255:0] scalar_reg;
  logic [7:0] bit_index;
  logic point_is_infinity;
  logic [2:0] init_counter;

  assign add_start=(op_state==O_ADD_START);
  assign add_subtract=(op_state==O_ADD_START)&&(instruction==I_SUB);
  assign add_operand_a=operand_a_reg;
  assign add_operand_b=operand_b_reg;
  assign add_modulus=SECP256K1_P;

  logic normal_mul_start;
  logic [255:0] mul_a,mul_b;
  assign mul_operand_a=mul_a;
  assign mul_operand_b=mul_b;
  assign mul_modulus=SECP256K1_P;
  assign inv_value=read_data;
  assign inv_modulus=SECP256K1_P;

  always_ff @(posedge clk) begin
    if (write_enable)
      register_file[write_address]<=write_data;
    else
      read_data<=register_file[read_address];
  end

  // Microcode ROM.  Each instruction writes exactly one register-file word.
  always_comb begin
    instruction=I_ADD;instruction_dst=R_ZERO;
    instruction_a=R_ZERO;instruction_b=R_ZERO;instruction_last=0;
    case (active_program)
      P_DOUBLE: case (program_counter)
         0:begin instruction=I_MUL;instruction_dst=R_YY;instruction_a=R_Y;instruction_b=R_Y;end
         1:begin instruction=I_MUL;instruction_dst=R_YYYY;instruction_a=R_YY;instruction_b=R_YY;end
         2:begin instruction=I_MUL;instruction_dst=R_S;instruction_a=R_X;instruction_b=R_YY;end
         3:begin instruction_dst=R_S;instruction_a=R_S;instruction_b=R_S;end
         4:begin instruction_dst=R_S;instruction_a=R_S;instruction_b=R_S;end
         5:begin instruction=I_MUL;instruction_dst=R_M;instruction_a=R_X;instruction_b=R_X;end
         6:begin instruction_dst=R_TEMP;instruction_a=R_M;instruction_b=R_M;end
         7:begin instruction_dst=R_M;instruction_a=R_TEMP;instruction_b=R_M;end
         8:begin instruction=I_MUL;instruction_dst=R_NEWX;instruction_a=R_M;instruction_b=R_M;end
         9:begin instruction_dst=R_TEMP;instruction_a=R_S;instruction_b=R_S;end
        10:begin instruction=I_SUB;instruction_dst=R_NEWX;instruction_a=R_NEWX;instruction_b=R_TEMP;end
        11:begin instruction=I_SUB;instruction_dst=R_TEMP;instruction_a=R_S;instruction_b=R_NEWX;end
        12:begin instruction=I_MUL;instruction_dst=R_NYP;instruction_a=R_M;instruction_b=R_TEMP;end
        13:begin instruction=I_MUL;instruction_dst=R_Z;instruction_a=R_Y;instruction_b=R_Z;end
        14:begin instruction_dst=R_TEMP;instruction_a=R_YYYY;instruction_b=R_YYYY;end
        15:begin instruction_dst=R_TEMP;instruction_a=R_TEMP;instruction_b=R_TEMP;end
        16:begin instruction_dst=R_TEMP;instruction_a=R_TEMP;instruction_b=R_TEMP;end
        17:begin instruction=I_SUB;instruction_dst=R_Y;instruction_a=R_NYP;instruction_b=R_TEMP;end
        18:begin instruction_dst=R_Z;instruction_a=R_Z;instruction_b=R_Z;end
        default:begin instruction_dst=R_X;instruction_a=R_NEWX;instruction_b=R_ZERO;instruction_last=1;end
      endcase
      P_ADD_PRE: case (program_counter)
        0:begin instruction=I_MUL;instruction_dst=R_Z2;instruction_a=R_Z;instruction_b=R_Z;end
        1:begin instruction=I_MUL;instruction_dst=R_U2;instruction_a=R_GX;instruction_b=R_Z2;end
        2:begin instruction=I_MUL;instruction_dst=R_Z3;instruction_a=R_Z;instruction_b=R_Z2;end
        default:begin instruction=I_MUL;instruction_dst=R_S2;instruction_a=R_GY;instruction_b=R_Z3;instruction_last=1;end
      endcase
      P_ADD_MAIN: case (program_counter)
         0:begin instruction=I_SUB;instruction_dst=R_H;instruction_a=R_U2;instruction_b=R_X;end
         1:begin instruction=I_SUB;instruction_dst=R_R;instruction_a=R_S2;instruction_b=R_Y;end
         2:begin instruction_dst=R_R;instruction_a=R_R;instruction_b=R_R;end
         3:begin instruction=I_MUL;instruction_dst=R_HH;instruction_a=R_H;instruction_b=R_H;end
         4:begin instruction_dst=R_I;instruction_a=R_HH;instruction_b=R_HH;end
         5:begin instruction_dst=R_I;instruction_a=R_I;instruction_b=R_I;end
         6:begin instruction=I_MUL;instruction_dst=R_J;instruction_a=R_H;instruction_b=R_I;end
         7:begin instruction=I_MUL;instruction_dst=R_V;instruction_a=R_X;instruction_b=R_I;end
         8:begin instruction=I_MUL;instruction_dst=R_NEWX;instruction_a=R_R;instruction_b=R_R;end
         9:begin instruction=I_SUB;instruction_dst=R_NEWX;instruction_a=R_NEWX;instruction_b=R_J;end
        10:begin instruction_dst=R_TEMP;instruction_a=R_V;instruction_b=R_V;end
        11:begin instruction=I_SUB;instruction_dst=R_NEWX;instruction_a=R_NEWX;instruction_b=R_TEMP;end
        12:begin instruction=I_SUB;instruction_dst=R_TEMP;instruction_a=R_V;instruction_b=R_NEWX;end
        13:begin instruction=I_MUL;instruction_dst=R_AYP;instruction_a=R_R;instruction_b=R_TEMP;end
        14:begin instruction=I_MUL;instruction_dst=R_AYJ;instruction_a=R_Y;instruction_b=R_J;end
        15:begin instruction_dst=R_TEMP;instruction_a=R_AYJ;instruction_b=R_AYJ;end
        16:begin instruction=I_SUB;instruction_dst=R_Y;instruction_a=R_AYP;instruction_b=R_TEMP;end
        17:begin instruction_dst=R_TEMP;instruction_a=R_Z;instruction_b=R_H;end
        18:begin instruction=I_MUL;instruction_dst=R_Z;instruction_a=R_TEMP;instruction_b=R_TEMP;end
        19:begin instruction=I_SUB;instruction_dst=R_Z;instruction_a=R_Z;instruction_b=R_Z2;end
        20:begin instruction=I_SUB;instruction_dst=R_Z;instruction_a=R_Z;instruction_b=R_HH;end
        default:begin instruction_dst=R_X;instruction_a=R_NEWX;instruction_b=R_ZERO;instruction_last=1;end
      endcase
      default: case (program_counter)
        0:begin instruction=I_MUL;instruction_dst=R_AZ2;instruction_a=R_ZINV;instruction_b=R_ZINV;end
        1:begin instruction=I_MUL;instruction_dst=R_X;instruction_a=R_X;instruction_b=R_AZ2;end
        2:begin instruction=I_MUL;instruction_dst=R_AZ3;instruction_a=R_AZ2;instruction_b=R_ZINV;end
        default:begin instruction=I_MUL;instruction_dst=R_Y;instruction_a=R_Y;instruction_b=R_AZ3;instruction_last=1;end
      endcase
    endcase
  end

  always_comb begin
    program_start=0;requested_program=P_DOUBLE;
    case (high_state)
      H_DOUBLE_START:begin program_start=1;requested_program=P_DOUBLE;end
      H_ADD_PRE_START:begin program_start=1;requested_program=P_ADD_PRE;end
      H_ADD_MAIN_START:begin program_start=1;requested_program=P_ADD_MAIN;end
      H_AFFINE_START:begin program_start=1;requested_program=P_AFFINE;end
      default:begin end
    endcase
  end

  always_comb begin
    read_address=0;write_enable=0;write_address=0;write_data=0;
    if (op_state==O_READ_A) read_address=instruction_a;
    else if (op_state==O_READ_B) read_address=instruction_b;
    if (op_state==O_ADD_WRITE) begin
      write_enable=1;write_address=instruction_dst;write_data=add_result;
    end else if (op_state==O_MUL_WRITE) begin
      write_enable=1;write_address=instruction_dst;write_data=mul_result;
    end
    case (high_state)
      H_INIT: begin
        write_enable=1;
        case (init_counter)
          0:begin write_address=R_X;write_data=0;end
          1:begin write_address=R_Y;write_data=0;end
          2:begin write_address=R_Z;write_data=0;end
          3:begin write_address=R_GX;write_data=SECP256K1_GX;end
          4:begin write_address=R_GY;write_data=SECP256K1_GY;end
          5:begin write_address=R_ZERO;write_data=0;end
          default:begin write_address=R_ONE;write_data=1;end
        endcase
      end
      H_SET_X:begin write_enable=1;write_address=R_X;write_data=SECP256K1_GX;end
      H_SET_Y:begin write_enable=1;write_address=R_Y;write_data=SECP256K1_GY;end
      H_SET_Z:begin write_enable=1;write_address=R_Z;write_data=1;end
      H_CMP_X_READ_A:read_address=R_U2;
      H_CMP_X_READ_B:read_address=R_X;
      H_CMP_Y_READ_A:read_address=R_S2;
      H_CMP_Y_READ_B:read_address=R_Y;
      H_INV_REQ:read_address=R_Z;
      H_INV_WRITE:begin write_enable=1;write_address=R_ZINV;write_data=inv_result;end
      H_OUT_X_READ:read_address=R_X;
      H_OUT_Y_READ:read_address=R_Y;
      default:begin end
    endcase
  end

  assign normal_mul_start=(op_state==O_MUL_START);
  assign inv_start=(high_state==H_INV_START);
  always_comb begin
    mul_start=normal_mul_start;mul_a=operand_a_reg;mul_b=operand_b_reg;
  end

  always_ff @(posedge clk or negedge reset_n) begin
    if (!reset_n) begin
      op_state<=O_IDLE;active_program<=P_DOUBLE;program_counter<=0;
      program_busy<=0;program_done<=0;operand_a_reg<=0;operand_b_reg<=0;
    end else begin
      program_done<=0;
      case (op_state)
        O_IDLE:if(program_start) begin
          active_program<=requested_program;program_counter<=0;program_busy<=1;op_state<=O_READ_A;
        end
        O_READ_A:op_state<=O_READ_B;
        O_READ_B:begin operand_a_reg<=read_data;op_state<=O_DISPATCH;end
        O_DISPATCH:begin
          operand_b_reg<=read_data;
          if(instruction==I_MUL) op_state<=O_MUL_START;
          else op_state<=O_ADD_START;
        end
        O_ADD_START:op_state<=O_ADD_WAIT;
        O_ADD_WAIT:if(add_done) op_state<=O_ADD_WRITE;
        O_ADD_WRITE:begin
          if(instruction_last) begin program_busy<=0;program_done<=1;op_state<=O_IDLE;end
          else begin program_counter<=program_counter+1'b1;op_state<=O_READ_A;end
        end
        O_MUL_START:op_state<=O_MUL_WAIT;
        O_MUL_WAIT:if(mul_done) op_state<=O_MUL_WRITE;
        O_MUL_WRITE:begin
          if(instruction_last) begin program_busy<=0;program_done<=1;op_state<=O_IDLE;end
          else begin program_counter<=program_counter+1'b1;op_state<=O_READ_A;end
        end
        default:op_state<=O_IDLE;
      endcase
    end
  end

  always_ff @(posedge clk or negedge reset_n) begin
    if (!reset_n) begin
      high_state<=H_IDLE;scalar_reg<=0;bit_index<=0;point_is_infinity<=1;init_counter<=0;
      busy<=0;done<=0;infinity<=1;affine_x<=0;affine_y<=0;compare_left<=0;
    end else begin
      done<=0;
      case (high_state)
        H_IDLE:if(start) begin
          if(scalar==0) begin infinity<=1;affine_x<=0;affine_y<=0;done<=1;end
          else begin scalar_reg<=scalar;bit_index<=255;init_counter<=0;
            point_is_infinity<=1;busy<=1;high_state<=H_INIT;end
        end
        H_INIT:if(init_counter==6) high_state<=H_BIT;
          else init_counter<=init_counter+1'b1;
        H_BIT:begin
          if(point_is_infinity) begin
            if(scalar_reg[bit_index]) high_state<=H_SET_X;else high_state<=H_NEXT;
          end else high_state<=H_DOUBLE_START;
        end
        H_SET_X:high_state<=H_SET_Y;
        H_SET_Y:high_state<=H_SET_Z;
        H_SET_Z:begin point_is_infinity<=0;high_state<=H_NEXT;end
        H_DOUBLE_START:high_state<=H_DOUBLE_WAIT;
        H_DOUBLE_WAIT:if(program_done) high_state<=H_AFTER_DOUBLE;
        H_AFTER_DOUBLE:begin
          if(scalar_reg[bit_index]) high_state<=H_ADD_PRE_START;else high_state<=H_NEXT;
        end
        H_ADD_PRE_START:high_state<=H_ADD_PRE_WAIT;
        H_ADD_PRE_WAIT:if(program_done) high_state<=H_CMP_X_READ_A;
        H_CMP_X_READ_A:high_state<=H_CMP_X_READ_B;
        H_CMP_X_READ_B:begin compare_left<=read_data;high_state<=H_CMP_X_EVAL;end
        H_CMP_X_EVAL:begin
          if(compare_left!=read_data) high_state<=H_ADD_MAIN_START;
          else high_state<=H_CMP_Y_READ_A;
        end
        H_CMP_Y_READ_A:high_state<=H_CMP_Y_READ_B;
        H_CMP_Y_READ_B:begin compare_left<=read_data;high_state<=H_CMP_Y_EVAL;end
        H_CMP_Y_EVAL:begin
          if(compare_left!=read_data) begin point_is_infinity<=1;high_state<=H_NEXT;end
          else high_state<=H_DOUBLE_START;
        end
        H_ADD_MAIN_START:high_state<=H_ADD_MAIN_WAIT;
        H_ADD_MAIN_WAIT:if(program_done) high_state<=H_NEXT;
        H_NEXT:begin
          if(bit_index==0) begin
            if(point_is_infinity) begin busy<=0;done<=1;infinity<=1;affine_x<=0;affine_y<=0;high_state<=H_IDLE;end
            else high_state<=H_INV_REQ;
          end else begin bit_index<=bit_index-1'b1;high_state<=H_BIT;end
        end
        H_INV_REQ:high_state<=H_INV_START;
        H_INV_START:high_state<=H_INV_WAIT;
        H_INV_WAIT:if(inv_done) high_state<=H_INV_WRITE;
        H_INV_WRITE:high_state<=H_AFFINE_START;
        H_AFFINE_START:high_state<=H_AFFINE_WAIT;
        H_AFFINE_WAIT:if(program_done) high_state<=H_OUT_X_READ;
        H_OUT_X_READ:high_state<=H_OUT_Y_READ;
        H_OUT_Y_READ:begin affine_x<=read_data;high_state<=H_OUT_CAPTURE;end
        H_OUT_CAPTURE:begin
          affine_y<=read_data;infinity<=0;busy<=0;done<=1;high_state<=H_IDLE;
        end
        default:high_state<=H_IDLE;
      endcase
    end
  end
endmodule

// Standalone wrapper retained for unit tests and reuse outside the complete
// signer.  eth_signer_core instantiates the controller directly so that this
// adder can be shared with the ECDSA arithmetic phase.
module secp256k1_point_mul (
  input  logic         clk,
  input  logic         reset_n,
  input  logic         start,
  input  logic [255:0] scalar,
  output logic         busy,
  output logic         done,
  output logic         infinity,
  output logic [255:0] affine_x,
  output logic [255:0] affine_y
);
  logic point_add_start,point_add_subtract;
  logic [255:0] point_add_a,point_add_b,point_add_modulus;
  logic point_mul_start,mul_done,mul_add_start,add_done;
  logic [255:0] point_mul_a,point_mul_b,point_mul_modulus,mul_result;
  logic point_inv_start,inv_done,inv_mul_start;
  logic [255:0] point_inv_value,point_inv_modulus,inv_result,inv_mul_a,inv_mul_b;
  logic shared_mul_start;
  logic [255:0] shared_mul_a,shared_mul_b;
  logic [255:0] mul_add_a,mul_add_b,mul_add_modulus;
  logic shared_add_start,shared_add_subtract;
  logic [255:0] shared_add_a,shared_add_b,shared_add_modulus,add_result;

  assign shared_add_start=point_add_start|mul_add_start;
  assign shared_add_subtract=point_add_start&&point_add_subtract;
  assign shared_add_a=point_add_start ? point_add_a : mul_add_a;
  assign shared_add_b=point_add_start ? point_add_b : mul_add_b;
  assign shared_add_modulus=point_add_start ? point_add_modulus : mul_add_modulus;
  assign shared_mul_start=point_mul_start|inv_mul_start;
  assign shared_mul_a=point_mul_start ? point_mul_a : inv_mul_a;
  assign shared_mul_b=point_mul_start ? point_mul_b : inv_mul_b;

  modaddsub256_seq shared_adder (
    .clk(clk),.reset_n(reset_n),.start(shared_add_start),
    .subtract(shared_add_subtract),.operand_a(shared_add_a),
    .operand_b(shared_add_b),.modulus(shared_add_modulus),
    .busy(),.done(add_done),.result(add_result)
  );

  modmul256_controller shared_multiplier (
    .clk(clk),.reset_n(reset_n),.start(shared_mul_start),
    .operand_a(shared_mul_a),.operand_b(shared_mul_b),.modulus(point_mul_modulus),
    .busy(),.done(mul_done),.result(mul_result),
    .add_start(mul_add_start),.add_operand_a(mul_add_a),
    .add_operand_b(mul_add_b),.add_modulus(mul_add_modulus),
    .add_done(add_done),.add_result(add_result)
  );

  modinv256_controller shared_inverse (
    .clk(clk),.reset_n(reset_n),.start(point_inv_start),
    .value(point_inv_value),.modulus(point_inv_modulus),
    .busy(),.done(inv_done),.result(inv_result),
    .mul_start(inv_mul_start),.mul_a(inv_mul_a),.mul_b(inv_mul_b),
    .mul_done(mul_done),.mul_result(mul_result)
  );

  secp256k1_point_mul_controller controller (
    .clk(clk),.reset_n(reset_n),.start(start),.scalar(scalar),
    .busy(busy),.done(done),.infinity(infinity),
    .affine_x(affine_x),.affine_y(affine_y),
    .add_start(point_add_start),.add_subtract(point_add_subtract),
    .add_operand_a(point_add_a),.add_operand_b(point_add_b),
    .add_modulus(point_add_modulus),.add_done(add_done),.add_result(add_result),
    .mul_start(point_mul_start),.mul_operand_a(point_mul_a),.mul_operand_b(point_mul_b),
    .mul_modulus(point_mul_modulus),.mul_done(mul_done),.mul_result(mul_result),
    .inv_start(point_inv_start),.inv_value(point_inv_value),
    .inv_modulus(point_inv_modulus),.inv_done(inv_done),.inv_result(inv_result)
  );
endmodule
