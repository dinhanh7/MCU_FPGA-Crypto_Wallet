`timescale 1ns/1ps

// Fermat modular-inverse controller using a caller-owned modular multiplier.
// Sharing the multiplier is important on the small GW5A-25: inversion and the
// surrounding point/signature operations are sequential and never need two
// modular multiplications at once.
module modinv256_controller (
  input  logic         clk,
  input  logic         reset_n,
  input  logic         start,
  input  logic [255:0] value,
  input  logic [255:0] modulus,
  output logic         busy,
  output logic         done,
  output logic [255:0] result,
  output logic         mul_start,
  output logic [255:0] mul_a,
  output logic [255:0] mul_b,
  input  logic         mul_done,
  input  logic [255:0] mul_result
);
  typedef enum logic [2:0] {
    INV_IDLE,
    INV_SQUARE_START,
    INV_SQUARE_WAIT,
    INV_MULTIPLY_START,
    INV_MULTIPLY_WAIT
  } inv_state_t;

  inv_state_t state;
  logic [255:0] exponent;
  logic [255:0] base_reg;
  logic [255:0] result_reg;
  logic [8:0] bit_index;

  always_comb begin
    mul_start = 1'b0;
    mul_a = result_reg;
    mul_b = result_reg;
    if (state == INV_SQUARE_START) begin
      mul_start = 1'b1;
    end else if (state == INV_MULTIPLY_START) begin
      mul_start = 1'b1;
      mul_b = base_reg;
    end
  end

  always_ff @(posedge clk or negedge reset_n) begin
    if (!reset_n) begin
      state <= INV_IDLE;
      busy <= 1'b0;
      done <= 1'b0;
      result <= 256'b0;
      exponent <= 256'b0;
      base_reg <= 256'b0;
      result_reg <= 256'b0;
      bit_index <= 9'b0;
    end else begin
      done <= 1'b0;
      case (state)
        INV_IDLE: begin
          if (start) begin
            if (value == 0 || modulus < 3) begin
              result <= 256'b0;
              done <= 1'b1;
            end else begin
              busy <= 1'b1;
              exponent <= modulus - 2'b10;
              base_reg <= (value >= modulus) ? value - modulus : value;
              result_reg <= 256'd1;
              bit_index <= 9'd255;
              state <= INV_SQUARE_START;
            end
          end
        end

        INV_SQUARE_START: state <= INV_SQUARE_WAIT;
        INV_SQUARE_WAIT: if (mul_done) begin
          result_reg <= mul_result;
          if (exponent[bit_index]) begin
            state <= INV_MULTIPLY_START;
          end else if (bit_index == 0) begin
            result <= mul_result;
            busy <= 1'b0;
            done <= 1'b1;
            state <= INV_IDLE;
          end else begin
            bit_index <= bit_index - 1'b1;
            state <= INV_SQUARE_START;
          end
        end

        INV_MULTIPLY_START: state <= INV_MULTIPLY_WAIT;
        INV_MULTIPLY_WAIT: if (mul_done) begin
          result_reg <= mul_result;
          if (bit_index == 0) begin
            result <= mul_result;
            busy <= 1'b0;
            done <= 1'b1;
            state <= INV_IDLE;
          end else begin
            bit_index <= bit_index - 1'b1;
            state <= INV_SQUARE_START;
          end
        end

        default: state <= INV_IDLE;
      endcase
    end
  end
endmodule
