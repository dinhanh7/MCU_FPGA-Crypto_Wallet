`timescale 1ns/1ps

// Shift-and-add modular multiplier using a caller-owned modular adder.
// The caller can share that adder with other sequential field operations.
module modmul256_controller (
  input  logic         clk,
  input  logic         reset_n,
  input  logic         start,
  input  logic [255:0] operand_a,
  input  logic [255:0] operand_b,
  input  logic [255:0] modulus,
  output logic         busy,
  output logic         done,
  output logic [255:0] result,
  output logic         add_start,
  output logic [255:0] add_operand_a,
  output logic [255:0] add_operand_b,
  output logic [255:0] add_modulus,
  input  logic         add_done,
  input  logic [255:0] add_result
);
  typedef enum logic [2:0] {
    M_IDLE,M_BIT,M_ACC_START,M_ACC_WAIT,M_DOUBLE_START,M_DOUBLE_WAIT
  } mul_state_t;

  mul_state_t state;
  logic [255:0] addend,multiplier,accumulator,modulus_reg;
  logic [8:0] bit_count;

  always_comb begin
    add_start=(state==M_ACC_START)||(state==M_DOUBLE_START);
    add_operand_a=(state==M_ACC_START||state==M_ACC_WAIT) ? accumulator : addend;
    add_operand_b=addend;
    add_modulus=modulus_reg;
  end

  always_ff @(posedge clk or negedge reset_n) begin
    if(!reset_n) begin
      state<=M_IDLE;
      addend<=0;multiplier<=0;accumulator<=0;modulus_reg<=0;bit_count<=0;
      busy<=0;done<=0;result<=0;
    end else begin
      done<=1'b0;
      case(state)
        M_IDLE:if(start) begin
          busy<=1'b1;
          addend<=operand_a;
          multiplier<=operand_b;
          accumulator<=0;
          modulus_reg<=modulus;
          bit_count<=0;
          state<=M_BIT;
        end
        M_BIT:state<=multiplier[0] ? M_ACC_START : M_DOUBLE_START;
        M_ACC_START:state<=M_ACC_WAIT;
        M_ACC_WAIT:if(add_done) begin
          accumulator<=add_result;
          state<=M_DOUBLE_START;
        end
        M_DOUBLE_START:state<=M_DOUBLE_WAIT;
        M_DOUBLE_WAIT:if(add_done) begin
          addend<=add_result;
          multiplier<={1'b0,multiplier[255:1]};
          if(bit_count==9'd255) begin
            result<=accumulator;
            busy<=1'b0;done<=1'b1;state<=M_IDLE;
          end else begin
            bit_count<=bit_count+1'b1;
            state<=M_BIT;
          end
        end
        default:state<=M_IDLE;
      endcase
    end
  end
endmodule
