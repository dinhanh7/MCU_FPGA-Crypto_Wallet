`timescale 1ns/1ps

// Area-oriented 256-bit modular add/subtract unit.
//
// Only one 32-bit carry chain is implemented.  Eight little-endian words are
// processed in sequence, followed by a second pass for the modular correction.
// Inputs must be reduced (operand_a < modulus and operand_b < modulus).
module modaddsub256_seq (
  input  logic         clk,
  input  logic         reset_n,
  input  logic         start,
  input  logic         subtract,
  input  logic [255:0] operand_a,
  input  logic [255:0] operand_b,
  input  logic [255:0] modulus,
  output logic         busy,
  output logic         done,
  output logic [255:0] result
);
  typedef enum logic [2:0] {
    A_IDLE,A_FIRST,A_DECIDE,A_SECOND,A_RESTORE,A_FINISH
  } state_t;

  state_t state;
  logic subtract_reg;
  logic [255:0] operand_a_shift,operand_b_shift,modulus_shift;
  logic [255:0] work_shift;
  logic [3:0] word_index;
  logic carry_reg,borrow_reg,high_reg;

  logic [31:0] word_a,word_b,word_b_adjusted;
  logic word_subtract,word_carry_in;
  logic [32:0] word_value;

  always_comb begin
    word_a=((state==A_SECOND)||(state==A_RESTORE)) ?
           work_shift[31:0] : operand_a_shift[31:0];
    word_b=((state==A_SECOND)||(state==A_RESTORE)) ?
           modulus_shift[31:0] : operand_b_shift[31:0];
    word_subtract=(state==A_SECOND) ? !subtract_reg :
                  (state==A_RESTORE) ? 1'b0 : subtract_reg;
    word_b_adjusted=word_subtract ? ~word_b : word_b;
    word_carry_in=word_subtract ? !borrow_reg : carry_reg;
    word_value={1'b0,word_a}+{1'b0,word_b_adjusted}+word_carry_in;
  end

  always_ff @(posedge clk or negedge reset_n) begin
    if(!reset_n) begin
      state<=A_IDLE;
      subtract_reg<=1'b0;
      operand_a_shift<=0;operand_b_shift<=0;modulus_shift<=0;
      work_shift<=0;
      word_index<=0;carry_reg<=0;borrow_reg<=0;high_reg<=0;
      busy<=0;done<=0;
    end else begin
      done<=1'b0;
      case(state)
        A_IDLE:if(start) begin
          subtract_reg<=subtract;
          operand_a_shift<=operand_a;
          operand_b_shift<=operand_b;
          modulus_shift<=modulus;
          work_shift<=0;
          word_index<=0;carry_reg<=0;borrow_reg<=0;high_reg<=0;
          busy<=1'b1;
          state<=A_FIRST;
        end

        A_FIRST:begin
          operand_a_shift<={32'b0,operand_a_shift[255:32]};
          operand_b_shift<={32'b0,operand_b_shift[255:32]};
          work_shift<={word_value[31:0],work_shift[255:32]};
          if(subtract_reg) borrow_reg<=!word_value[32];
          else carry_reg<=word_value[32];
          if(word_index==7) begin
            high_reg<=subtract_reg ? 1'b0 : word_value[32];
            word_index<=0;
            state<=A_DECIDE;
          end else word_index<=word_index+1'b1;
        end

        A_DECIDE:begin
          word_index<=0;
          carry_reg<=0;
          if(subtract_reg && !borrow_reg) begin
            busy<=1'b0;done<=1'b1;state<=A_IDLE;
          end else begin
            borrow_reg<=0;
            state<=A_SECOND;
          end
        end

        A_SECOND:begin
          work_shift<={word_value[31:0],work_shift[255:32]};
          // Rotate instead of clearing: if subtraction underflows, the
          // original modulus is immediately available for A_RESTORE.
          modulus_shift<={modulus_shift[31:0],modulus_shift[255:32]};
          if(subtract_reg) carry_reg<=word_value[32];
          else borrow_reg<=!word_value[32];
          if(word_index==7) begin
            word_index<=0;
            state<=A_FINISH;
          end else word_index<=word_index+1'b1;
        end

        A_FINISH:begin
          if(!subtract_reg && !high_reg && borrow_reg) begin
            // raw < modulus: A_SECOND produced raw-modulus modulo 2^256.
            // Add the modulus back in place rather than retaining a second
            // 256-bit copy of raw.
            word_index<=0;carry_reg<=0;state<=A_RESTORE;
          end else begin
            busy<=1'b0;done<=1'b1;state<=A_IDLE;
          end
        end

        A_RESTORE:begin
          work_shift<={word_value[31:0],work_shift[255:32]};
          modulus_shift<={modulus_shift[31:0],modulus_shift[255:32]};
          carry_reg<=word_value[32];
          if(word_index==7) begin
            word_index<=0;busy<=1'b0;done<=1'b1;state<=A_IDLE;
          end else word_index<=word_index+1'b1;
        end

        default:state<=A_IDLE;
      endcase
    end
  end

  assign result=work_shift;
endmodule
