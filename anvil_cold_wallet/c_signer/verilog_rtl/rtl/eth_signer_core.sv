`timescale 1ns/1ps

module eth_signer_core (
  input  logic         clk,
  input  logic         reset_n,
  input  logic         start,
  
  // Data Inputs từ SPI / AES / TRNG
  input  logic [255:0] msg_hash_m,
  input  logic [255:0] private_key_d,
  input  logic [255:0] trng_nonce_k,
  
  // Data Outputs gửi về MCU
  output logic [255:0] signature_r,
  output logic [255:0] signature_s,
  output logic         y_parity,
  
  // Trạng thái module
  output logic         busy,
  output logic         done,
  output logic         error
);

`include "eth_secp256k1_common.svh"
  localparam logic [255:0] CURVE_N =
    256'hfffffffffffffffffffffffffffffffebaaedce6af48a03bbfd25e8cd0364141;

  typedef enum logic [3:0] {
    ST_IDLE,
    ST_NONCE_POINT_START, ST_NONCE_POINT_WAIT,
    ST_RKEY_MUL_START, ST_RKEY_MUL_WAIT,
    ST_NONCE_INV_START, ST_NONCE_INV_WAIT,
    ST_S_MUL_START, ST_S_MUL_WAIT
  } state_t;

  state_t state;
  logic [255:0] signature_numerator;
  logic recovery_high_reg;
  logic [255:0] nonce_inverse_reg;

  // -------------------------------------------------------------
  // SECP256K1 Point Multiplication (R = k * G)
  // -------------------------------------------------------------
  logic point_start, point_done;
  logic [255:0] point_scalar, point_x, point_y;
  logic point_add_start,point_add_subtract;
  logic [255:0] point_add_a,point_add_b,point_add_modulus;
  logic point_mul_start;
  logic [255:0] point_mul_a,point_mul_b,point_mul_modulus;
  logic point_inv_start;
  logic [255:0] point_inv_value,point_inv_modulus;

  secp256k1_point_mul_controller point_inst (
    .clk(clk),.reset_n(reset_n),.start(point_start),.scalar(point_scalar),
    .busy(),.done(point_done),.infinity(),.affine_x(point_x),.affine_y(point_y),
    .add_start(point_add_start),.add_subtract(point_add_subtract),
    .add_operand_a(point_add_a),.add_operand_b(point_add_b),
    .add_modulus(point_add_modulus),
    .add_done(shared_add_done),.add_result(shared_add_result),
    .mul_start(point_mul_start),.mul_operand_a(point_mul_a),
    .mul_operand_b(point_mul_b),.mul_modulus(point_mul_modulus),
    .mul_done(arithmetic_mul_done),.mul_result(arithmetic_mul_result),
    .inv_start(point_inv_start),.inv_value(point_inv_value),
    .inv_modulus(point_inv_modulus),
    .inv_done(arithmetic_inv_done),.inv_result(arithmetic_inv_result)
  );

  // -------------------------------------------------------------
  // Shared Modular Multiplier
  // -------------------------------------------------------------
  logic arithmetic_mul_start, arithmetic_mul_done;
  logic [255:0] arithmetic_mul_a, arithmetic_mul_b, arithmetic_mul_result;
  logic multiplier_add_start;
  logic [255:0] multiplier_add_a,multiplier_add_b,multiplier_add_modulus;
  logic shared_inv_mul_start;
  logic [255:0] shared_inv_mul_a,shared_inv_mul_b;
  logic shared_mul_start;
  logic [255:0] shared_mul_a,shared_mul_b,shared_mul_modulus;
  logic shared_inv_is_point;
  
  assign shared_mul_start=point_mul_start|shared_inv_mul_start|arithmetic_mul_start;
  
  always_comb begin
    shared_mul_a = arithmetic_mul_a;
    shared_mul_b = arithmetic_mul_b;
    shared_mul_modulus = CURVE_N;
    if(shared_inv_mul_start) begin
      shared_mul_a = shared_inv_mul_a;
      shared_mul_b = shared_inv_mul_b;
      shared_mul_modulus = shared_inv_is_point ? SECP256K1_P : CURVE_N;
    end
    if(point_mul_start) begin
      shared_mul_a = point_mul_a;
      shared_mul_b = point_mul_b;
      shared_mul_modulus = point_mul_modulus;
    end
  end

  modmul256_controller shared_multiplier (
    .clk(clk),.reset_n(reset_n),.start(shared_mul_start),
    .operand_a(shared_mul_a),.operand_b(shared_mul_b),.modulus(shared_mul_modulus),
    .busy(),.done(arithmetic_mul_done),.result(arithmetic_mul_result),
    .add_start(multiplier_add_start),.add_operand_a(multiplier_add_a),
    .add_operand_b(multiplier_add_b),.add_modulus(multiplier_add_modulus),
    .add_done(shared_add_done),.add_result(shared_add_result)
  );

  // -------------------------------------------------------------
  // Shared Modular Adder
  // -------------------------------------------------------------
  logic shared_add_start,shared_add_subtract,shared_add_done;
  logic [255:0] shared_add_a,shared_add_b,shared_add_modulus,shared_add_result;
  always_comb begin
    shared_add_start = point_add_start | multiplier_add_start;
    shared_add_subtract = 1'b0;
    shared_add_a = multiplier_add_a;
    shared_add_b = multiplier_add_b;
    shared_add_modulus = multiplier_add_modulus;
    if(point_add_start) begin
      shared_add_subtract = point_add_subtract;
      shared_add_a = point_add_a;
      shared_add_b = point_add_b;
      shared_add_modulus = point_add_modulus;
    end
  end
  modaddsub256_seq shared_modular_adder (
    .clk(clk),.reset_n(reset_n),.start(shared_add_start),
    .subtract(shared_add_subtract),.operand_a(shared_add_a),
    .operand_b(shared_add_b),.modulus(shared_add_modulus),
    .busy(),.done(shared_add_done),.result(shared_add_result)
  );

  // -------------------------------------------------------------
  // Shared Modular Inverter
  // -------------------------------------------------------------
  logic arithmetic_inv_start, arithmetic_inv_done;
  logic [255:0] arithmetic_inv_result;
  logic shared_inv_start;
  logic [255:0] shared_inv_value,shared_inv_modulus;
  assign shared_inv_start = point_inv_start | arithmetic_inv_start;
  assign shared_inv_value = point_inv_start ? point_inv_value : trng_nonce_k;
  assign shared_inv_modulus = point_inv_start ? point_inv_modulus : CURVE_N;
  
  modinv256_controller shared_inverse (
    .clk(clk),.reset_n(reset_n),.start(shared_inv_start),
    .value(shared_inv_value),.modulus(shared_inv_modulus),
    .busy(),.done(arithmetic_inv_done),.result(arithmetic_inv_result),
    .mul_start(shared_inv_mul_start),
    .mul_a(shared_inv_mul_a),.mul_b(shared_inv_mul_b),
    .mul_done(arithmetic_mul_done),.mul_result(arithmetic_mul_result)
  );

  always_ff @(posedge clk or negedge reset_n) begin
    if(!reset_n) shared_inv_is_point<=1'b0;
    else if(point_inv_start) shared_inv_is_point<=1'b1;
    else if(arithmetic_inv_start) shared_inv_is_point<=1'b0;
  end

  // -------------------------------------------------------------
  // Core FSM (Control Path)
  // -------------------------------------------------------------
  always_comb begin
    point_start = (state == ST_NONCE_POINT_START);
    point_scalar = trng_nonce_k;
    
    arithmetic_mul_start = (state == ST_RKEY_MUL_START) || (state == ST_S_MUL_START);
    arithmetic_mul_a = signature_r;
    arithmetic_mul_b = private_key_d;
    if (state == ST_S_MUL_START) begin
      arithmetic_mul_a = nonce_inverse_reg;
      arithmetic_mul_b = signature_numerator;
    end
    arithmetic_inv_start = (state == ST_NONCE_INV_START);
  end



  always_ff @(posedge clk or negedge reset_n) begin
    if (!reset_n) begin
      state <= ST_IDLE;
      signature_r <= 0;
      signature_s <= 0;
      y_parity <= 0;
      busy <= 0;
      done <= 0;
      error <= 0;
      signature_numerator <= 0;
      nonce_inverse_reg <= 0;
      recovery_high_reg <= 0;
    end else begin
      done <= 1'b0;
      case (state)
        ST_IDLE: begin
          if (start) begin
            busy <= 1'b1;
            error <= 1'b0;
            if (private_key_d == 0 || private_key_d >= CURVE_N) begin
              error <= 1'b1;
              done <= 1'b1;
              busy <= 1'b0;
            end else begin
              state <= ST_NONCE_POINT_START;
            end
          end
        end

        ST_NONCE_POINT_START: state <= ST_NONCE_POINT_WAIT;
        ST_NONCE_POINT_WAIT: begin
          if (point_done) begin
            if (point_x == 0 || point_x == CURVE_N) begin
              error <= 1'b1;
              done <= 1'b1;
              busy <= 1'b0;
              state <= ST_IDLE;
            end else begin
              signature_r <= (point_x > CURVE_N) ? point_x - CURVE_N : point_x;
              y_parity <= point_y[0];
              recovery_high_reg <= (point_x > CURVE_N);
              state <= ST_RKEY_MUL_START;
            end
          end
        end

        ST_RKEY_MUL_START: state <= ST_RKEY_MUL_WAIT;
        ST_RKEY_MUL_WAIT: begin
          if (arithmetic_mul_done) begin
            logic [255:0] msg_mod_n;
            msg_mod_n = (msg_hash_m >= CURVE_N) ? msg_hash_m - CURVE_N : msg_hash_m;
            signature_numerator <= mod_add256(msg_mod_n, arithmetic_mul_result, CURVE_N);
            state <= ST_NONCE_INV_START;
          end
        end

        ST_NONCE_INV_START: state <= ST_NONCE_INV_WAIT;
        ST_NONCE_INV_WAIT: begin
          if (arithmetic_inv_done) begin
            nonce_inverse_reg <= arithmetic_inv_result;
            state <= ST_S_MUL_START;
          end
        end

        ST_S_MUL_START: state <= ST_S_MUL_WAIT;
        ST_S_MUL_WAIT: begin
          if (arithmetic_mul_done) begin
            if (arithmetic_mul_result == 0 || signature_r == 0 || recovery_high_reg) begin
              error <= 1'b1;
              done <= 1'b1;
              busy <= 1'b0;
              state <= ST_IDLE;
            end else begin
              if (arithmetic_mul_result > SECP256K1_N_HALF) begin
                signature_s <= CURVE_N - arithmetic_mul_result;
                y_parity <= ~y_parity;
              end else begin
                signature_s <= arithmetic_mul_result;
              end
              done <= 1'b1;
              busy <= 1'b0;
              state <= ST_IDLE;
            end
          end
        end
        default: state <= ST_IDLE;
      endcase
    end
  end
endmodule
