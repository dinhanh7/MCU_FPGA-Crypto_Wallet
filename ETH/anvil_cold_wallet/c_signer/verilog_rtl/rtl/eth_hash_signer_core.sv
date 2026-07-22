`timescale 1ns/1ps

module eth_hash_signer_core #(
  parameter logic [255:0] PRIVATE_KEY = 256'h1
) (
  input  logic         clk,
  input  logic         reset_n,
  input  logic         start,
  input  logic [255:0] message_hash,
  output logic         busy,
  output logic         done,
  output logic         error,
  output logic [3:0]   error_code,
  output logic         y_parity,
  output logic [255:0] signature_r,
  output logic [255:0] signature_s
);
`include "eth_secp256k1_common.svh"
  localparam logic [255:0] CURVE_N =
    256'hfffffffffffffffffffffffffffffffebaaedce6af48a03bbfd25e8cd0364141;

  typedef enum logic [3:0] {
    SIGN_IDLE,
    SIGN_NONCE_START, SIGN_NONCE_WAIT, SIGN_NONCE_RETRY,
    SIGN_POINT_START, SIGN_POINT_WAIT,
    SIGN_RKEY_MUL_START, SIGN_RKEY_MUL_WAIT,
    SIGN_NUM_ADD_START, SIGN_NUM_ADD_WAIT,
    SIGN_NONCE_INV_START, SIGN_NONCE_INV_WAIT,
    SIGN_S_MUL_START, SIGN_S_MUL_WAIT
  } sign_state_t;

  sign_state_t state;
  logic [255:0] message_hash_reg;
  logic [255:0] rkey_product_reg;
  logic [255:0] nonce_inverse_reg;
  logic [255:0] signature_numerator;
  logic recovery_high_reg;

  logic nonce_start,nonce_retry,nonce_done;
  logic [255:0] deterministic_nonce;

  logic point_start,point_done;
  logic [255:0] point_x,point_y;
  logic point_add_start,point_add_subtract;
  logic [255:0] point_add_a,point_add_b,point_add_modulus;
  logic point_mul_start;
  logic [255:0] point_mul_a,point_mul_b,point_mul_modulus;
  logic point_inv_start;
  logic [255:0] point_inv_value,point_inv_modulus;

  logic arithmetic_mul_start,arithmetic_mul_done;
  logic [255:0] arithmetic_mul_a,arithmetic_mul_b,arithmetic_mul_result;
  logic multiplier_add_start;
  logic [255:0] multiplier_add_a,multiplier_add_b,multiplier_add_modulus;
  logic shared_inv_mul_start;
  logic [255:0] shared_inv_mul_a,shared_inv_mul_b;
  logic shared_mul_start;
  logic [255:0] shared_mul_a,shared_mul_b,shared_mul_modulus;
  logic shared_inv_is_point;

  logic arithmetic_add_start;
  logic [255:0] arithmetic_add_a,arithmetic_add_b;
  logic shared_add_start,shared_add_subtract,shared_add_done;
  logic [255:0] shared_add_a,shared_add_b,shared_add_modulus,shared_add_result;

  logic arithmetic_inv_start,arithmetic_inv_done;
  logic [255:0] arithmetic_inv_result;
  logic shared_inv_start;
  logic [255:0] shared_inv_value,shared_inv_modulus;

  rfc6979_nonce nonce_inst (
    .clk(clk),.reset_n(reset_n),.start(nonce_start),.retry(nonce_retry),
    .private_key(PRIVATE_KEY),.message_hash(message_hash_reg),
    .busy(),.done(nonce_done),.nonce(deterministic_nonce)
  );

  secp256k1_point_mul_controller point_inst (
    .clk(clk),.reset_n(reset_n),.start(point_start),.scalar(deterministic_nonce),
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

  assign shared_mul_start=point_mul_start|shared_inv_mul_start|arithmetic_mul_start;
  always_comb begin
    shared_mul_a=arithmetic_mul_a;
    shared_mul_b=arithmetic_mul_b;
    shared_mul_modulus=CURVE_N;
    if(shared_inv_mul_start) begin
      shared_mul_a=shared_inv_mul_a;
      shared_mul_b=shared_inv_mul_b;
      shared_mul_modulus=shared_inv_is_point ? SECP256K1_P : CURVE_N;
    end
    if(point_mul_start) begin
      shared_mul_a=point_mul_a;
      shared_mul_b=point_mul_b;
      shared_mul_modulus=point_mul_modulus;
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

  always_comb begin
    shared_add_start=point_add_start|multiplier_add_start|arithmetic_add_start;
    shared_add_subtract=1'b0;
    shared_add_a=multiplier_add_a;
    shared_add_b=multiplier_add_b;
    shared_add_modulus=multiplier_add_modulus;
    if(arithmetic_add_start) begin
      shared_add_a=arithmetic_add_a;
      shared_add_b=arithmetic_add_b;
      shared_add_modulus=CURVE_N;
    end
    if(point_add_start) begin
      shared_add_subtract=point_add_subtract;
      shared_add_a=point_add_a;
      shared_add_b=point_add_b;
      shared_add_modulus=point_add_modulus;
    end
  end

  modaddsub256_seq shared_modular_adder (
    .clk(clk),.reset_n(reset_n),.start(shared_add_start),
    .subtract(shared_add_subtract),.operand_a(shared_add_a),
    .operand_b(shared_add_b),.modulus(shared_add_modulus),
    .busy(),.done(shared_add_done),.result(shared_add_result)
  );

  assign shared_inv_start=point_inv_start|arithmetic_inv_start;
  assign shared_inv_value=point_inv_start ? point_inv_value : deterministic_nonce;
  assign shared_inv_modulus=point_inv_start ? point_inv_modulus : CURVE_N;

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

  always_comb begin
    nonce_start=(state==SIGN_NONCE_START);
    nonce_retry=(state==SIGN_NONCE_RETRY);
    point_start=(state==SIGN_POINT_START);
    arithmetic_mul_start=(state==SIGN_RKEY_MUL_START)||(state==SIGN_S_MUL_START);
    arithmetic_mul_a=signature_r;
    arithmetic_mul_b=PRIVATE_KEY;
    if(state==SIGN_S_MUL_START) begin
      arithmetic_mul_a=nonce_inverse_reg;
      arithmetic_mul_b=signature_numerator;
    end
    arithmetic_add_start=(state==SIGN_NUM_ADD_START);
    arithmetic_add_a=(message_hash_reg>=CURVE_N)?message_hash_reg-CURVE_N:message_hash_reg;
    arithmetic_add_b=rkey_product_reg;
    arithmetic_inv_start=(state==SIGN_NONCE_INV_START);
  end

  always_ff @(posedge clk or negedge reset_n) begin
    if(!reset_n) begin
      state<=SIGN_IDLE;
      message_hash_reg<=0;
      rkey_product_reg<=0;
      nonce_inverse_reg<=0;
      signature_numerator<=0;
      recovery_high_reg<=0;
      busy<=0;
      done<=0;
      error<=0;
      error_code<=0;
      y_parity<=0;
      signature_r<=0;
      signature_s<=0;
    end else begin
      done<=1'b0;
      case(state)
        SIGN_IDLE: if(start) begin
          error<=0;
          error_code<=0;
          if(PRIVATE_KEY==0 || PRIVATE_KEY>=CURVE_N) begin
            error<=1;
            error_code<=1;
            done<=1;
          end else begin
            message_hash_reg<=message_hash;
            signature_r<=0;
            signature_s<=0;
            y_parity<=0;
            recovery_high_reg<=0;
            busy<=1;
            state<=SIGN_NONCE_START;
          end
        end

        SIGN_NONCE_START: state<=SIGN_NONCE_WAIT;
        SIGN_NONCE_RETRY: state<=SIGN_NONCE_WAIT;
        SIGN_NONCE_WAIT: if(nonce_done) state<=SIGN_POINT_START;
        SIGN_POINT_START: state<=SIGN_POINT_WAIT;
        SIGN_POINT_WAIT: if(point_done) begin
          if(point_x==0 || point_x==CURVE_N) begin
            state<=SIGN_NONCE_RETRY;
          end else begin
            signature_r<=(point_x>CURVE_N)?point_x-CURVE_N:point_x;
            y_parity<=point_y[0];
            recovery_high_reg<=(point_x>CURVE_N);
            state<=SIGN_RKEY_MUL_START;
          end
        end

        SIGN_RKEY_MUL_START: state<=SIGN_RKEY_MUL_WAIT;
        SIGN_RKEY_MUL_WAIT: if(arithmetic_mul_done) begin
          rkey_product_reg<=arithmetic_mul_result;
          state<=SIGN_NUM_ADD_START;
        end

        SIGN_NUM_ADD_START: state<=SIGN_NUM_ADD_WAIT;
        SIGN_NUM_ADD_WAIT: if(shared_add_done) begin
          signature_numerator<=shared_add_result;
          state<=SIGN_NONCE_INV_START;
        end

        SIGN_NONCE_INV_START: state<=SIGN_NONCE_INV_WAIT;
        SIGN_NONCE_INV_WAIT: if(arithmetic_inv_done) begin
          nonce_inverse_reg<=arithmetic_inv_result;
          state<=SIGN_S_MUL_START;
        end

        SIGN_S_MUL_START: state<=SIGN_S_MUL_WAIT;
        SIGN_S_MUL_WAIT: if(arithmetic_mul_done) begin
          if(arithmetic_mul_result==0 || signature_r==0) begin
            state<=SIGN_NONCE_RETRY;
          end else if(recovery_high_reg) begin
            error<=1;
            error_code<=2;
            busy<=0;
            done<=1;
            state<=SIGN_IDLE;
          end else begin
            if(arithmetic_mul_result>SECP256K1_N_HALF) begin
              signature_s<=CURVE_N-arithmetic_mul_result;
              y_parity<=~y_parity;
            end else begin
              signature_s<=arithmetic_mul_result;
            end
            busy<=0;
            done<=1;
            state<=SIGN_IDLE;
          end
        end

        default: state<=SIGN_IDLE;
      endcase
    end
  end
endmodule
