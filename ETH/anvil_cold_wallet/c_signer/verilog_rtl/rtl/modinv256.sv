`timescale 1ns/1ps

module modinv256 (
  input  logic         clk,
  input  logic         reset_n,
  input  logic         start,
  input  logic [255:0] value,
  input  logic [255:0] modulus,
  output logic         busy,
  output logic         done,
  output logic [255:0] result
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
  logic [255:0] modulus_reg;
  logic [8:0] bit_index;

  logic mul_start;
  logic mul_busy;
  logic mul_done;
  logic [255:0] mul_a;
  logic [255:0] mul_b;
  logic [255:0] mul_result;

  modmul256 multiplier_inst (
    .clk(clk),
    .reset_n(reset_n),
    .start(mul_start),
    .operand_a(mul_a),
    .operand_b(mul_b),
    .modulus(modulus_reg),
    .busy(mul_busy),
    .done(mul_done),
    .result(mul_result)
  );

  always_comb begin
    mul_start = 1'b0;
    mul_a = result_reg;
    mul_b = result_reg;
    if (state == INV_SQUARE_START) begin
      mul_start = 1'b1;
    end else if (state == INV_MULTIPLY_START) begin
      mul_start = 1'b1;
      mul_a = result_reg;
      mul_b = base_reg;
    end
  end

  always_ff @(posedge clk or negedge reset_n) begin
    if (!reset_n) begin
      state       <= INV_IDLE;
      busy        <= 1'b0;
      done        <= 1'b0;
      result      <= 256'b0;
      exponent    <= 256'b0;
      base_reg    <= 256'b0;
      result_reg  <= 256'b0;
      modulus_reg <= 256'b0;
      bit_index   <= 9'b0;
    end else begin
      done <= 1'b0;
      case (state)
        INV_IDLE: begin
          if (start) begin
            if (value == 0 || modulus < 3) begin
              result <= 256'b0;
              done   <= 1'b1;
            end else begin
              busy        <= 1'b1;
              exponent    <= modulus - 2'b10;
              base_reg    <= (value >= modulus) ? value - modulus : value;
              result_reg  <= 256'd1;
              modulus_reg <= modulus;
              bit_index   <= 9'd255;
              state       <= INV_SQUARE_START;
            end
          end
        end

        INV_SQUARE_START: state <= INV_SQUARE_WAIT;

        INV_SQUARE_WAIT: begin
          if (mul_done) begin
            result_reg <= mul_result;
            if (exponent[bit_index])
              state <= INV_MULTIPLY_START;
            else if (bit_index == 0) begin
              result <= mul_result;
              busy   <= 1'b0;
              done   <= 1'b1;
              state  <= INV_IDLE;
            end else begin
              bit_index <= bit_index - 1'b1;
              state <= INV_SQUARE_START;
            end
          end
        end

        INV_MULTIPLY_START: state <= INV_MULTIPLY_WAIT;

        INV_MULTIPLY_WAIT: begin
          if (mul_done) begin
            result_reg <= mul_result;
            if (bit_index == 0) begin
              result <= mul_result;
              busy   <= 1'b0;
              done   <= 1'b1;
              state  <= INV_IDLE;
            end else begin
              bit_index <= bit_index - 1'b1;
              state <= INV_SQUARE_START;
            end
          end
        end

        default: state <= INV_IDLE;
      endcase
    end
  end
endmodule
