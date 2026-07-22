`timescale 1ns/1ps

// Deterministic secp256k1 ECDSA signer with an in-core verification pass.
// The in-core check evaluates s*k == z+r*d (mod n).  This is the ECDSA
// signing equation, including the low-S negation case, and reuses the same
// modular multiplier without a second point multiplication datapath pass.
module btc_ecdsa_sign_verify_core #(
  parameter logic [255:0] PRIVATE_KEY = 256'h1
) (
  input  logic         clk,
  input  logic         reset_n,
  input  logic         start,
  input  logic [255:0] message_hash,
  input  logic         message_preloaded,
  input  logic         message_load_valid,
  input  logic [7:0]   message_load_byte,
  output logic         busy,
  output logic         done,
  output logic         error,
  output logic [3:0]   error_code,
  output logic         verified,
  output logic         y_parity,
  output logic         recovery_high,
  output logic [255:0] signature_r,
  output logic [255:0] signature_s,
  output logic         sha_start,
  output logic         sha_in_valid,
  output logic [7:0]   sha_in_byte,
  output logic         sha_in_last,
  input  logic         sha_in_ready,
  input  logic         sha_done,
  input  logic [255:0] sha_digest
);
`include "eth_secp256k1_common.svh"
  typedef enum logic [4:0] {
    S_IDLE,S_NONCE_START,S_NONCE_WAIT,S_NONCE_RETRY,
    S_POINT_START,S_POINT_WAIT,S_RKEY_MUL_START,S_RKEY_MUL_WAIT,
    S_NUM_ADD_START,S_NUM_ADD_WAIT,S_NONCE_INV_START,S_NONCE_INV_WAIT,
    S_SIG_MUL_START,S_SIG_MUL_WAIT,S_VERIFY_NEG_START,S_VERIFY_NEG_WAIT,
    S_VERIFY_MUL_START,S_VERIFY_MUL_WAIT
  } state_t;

  state_t state;
  logic [255:0] signature_numerator;

  logic nonce_start,nonce_retry,nonce_done;
  logic [255:0] deterministic_nonce;
  btc_rfc6979_nonce nonce_inst (
    .clk(clk),.reset_n(reset_n),.start(nonce_start),.retry(nonce_retry),
    // signature_numerator holds the message until the z+r*d numerator is
    // available.  ETH can therefore stream its hash into an existing bank.
    .private_key(PRIVATE_KEY),.message_hash(signature_numerator),
    .busy(),.done(nonce_done),.nonce(deterministic_nonce),
    .sha_start(sha_start),.sha_in_valid(sha_in_valid),.sha_in_byte(sha_in_byte),
    .sha_in_last(sha_in_last),.sha_in_ready(sha_in_ready),
    .sha_done(sha_done),.sha_digest(sha_digest)
  );

  logic point_start,point_done,point_infinity;
  logic [255:0] point_scalar,point_x,point_y;
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
  logic [255:0] arithmetic_inv_value,arithmetic_inv_result;
  logic shared_inv_start;
  logic [255:0] shared_inv_value,shared_inv_modulus;

  assign point_scalar=deterministic_nonce;
  secp256k1_point_mul_controller point_inst (
    .clk(clk),.reset_n(reset_n),.start(point_start),.scalar(point_scalar),
    .busy(),.done(point_done),.infinity(point_infinity),
    .affine_x(point_x),.affine_y(point_y),
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
    shared_mul_modulus=SECP256K1_N;
    if(shared_inv_mul_start) begin
      shared_mul_a=shared_inv_mul_a;
      shared_mul_b=shared_inv_mul_b;
      shared_mul_modulus=shared_inv_is_point ? SECP256K1_P : SECP256K1_N;
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
      shared_add_subtract=(state==S_VERIFY_NEG_START);
      shared_add_a=arithmetic_add_a;
      shared_add_b=arithmetic_add_b;
      shared_add_modulus=SECP256K1_N;
    end
    if(point_add_start) begin
      shared_add_subtract=point_add_subtract;
      shared_add_a=point_add_a;
      shared_add_b=point_add_b;
      shared_add_modulus=point_add_modulus;
    end
  end

  modaddsub256_seq shared_adder (
    .clk(clk),.reset_n(reset_n),.start(shared_add_start),
    .subtract(shared_add_subtract),.operand_a(shared_add_a),
    .operand_b(shared_add_b),.modulus(shared_add_modulus),
    .busy(),.done(shared_add_done),.result(shared_add_result)
  );

  assign shared_inv_start=point_inv_start|arithmetic_inv_start;
  assign shared_inv_value=point_inv_start ? point_inv_value : arithmetic_inv_value;
  assign shared_inv_modulus=point_inv_start ? point_inv_modulus : SECP256K1_N;
  modinv256_controller shared_inverse (
    .clk(clk),.reset_n(reset_n),.start(shared_inv_start),
    .value(shared_inv_value),.modulus(shared_inv_modulus),
    .busy(),.done(arithmetic_inv_done),.result(arithmetic_inv_result),
    .mul_start(shared_inv_mul_start),.mul_a(shared_inv_mul_a),
    .mul_b(shared_inv_mul_b),.mul_done(arithmetic_mul_done),
    .mul_result(arithmetic_mul_result)
  );

  always_ff @(posedge clk or negedge reset_n) begin
    if(!reset_n) shared_inv_is_point<=1'b0;
    else if(point_inv_start) shared_inv_is_point<=1'b1;
    else if(arithmetic_inv_start) shared_inv_is_point<=1'b0;
  end

  always_comb begin
    nonce_start=(state==S_NONCE_START);
    nonce_retry=(state==S_NONCE_RETRY);
    point_start=(state==S_POINT_START);
    arithmetic_mul_start=(state==S_RKEY_MUL_START)||
                         (state==S_SIG_MUL_START)||
                         (state==S_VERIFY_MUL_START);
    arithmetic_mul_a=signature_r;
    arithmetic_mul_b=PRIVATE_KEY;
    if(state==S_SIG_MUL_START) begin
      // The inverse controller holds its result until the next inverse.  The
      // multiplier captures it on start, so a second 256-bit holding register
      // would only waste tightly packed CLS resources.
      arithmetic_mul_a=arithmetic_inv_result;
      arithmetic_mul_b=signature_numerator;
    end else if(state==S_VERIFY_MUL_START) begin
      arithmetic_mul_a=signature_s;
      arithmetic_mul_b=deterministic_nonce;
    end
    arithmetic_add_start=(state==S_NUM_ADD_START)||(state==S_VERIFY_NEG_START);
    arithmetic_add_a=(state==S_VERIFY_NEG_START) ? 256'b0 :
                     ((signature_numerator>=SECP256K1_N) ?
                       signature_numerator-SECP256K1_N : signature_numerator);
    arithmetic_add_b=(state==S_VERIFY_NEG_START) ?
                     signature_numerator : arithmetic_mul_result;
    arithmetic_inv_start=(state==S_NONCE_INV_START);
    arithmetic_inv_value=deterministic_nonce;
  end

  always_ff @(posedge clk or negedge reset_n) begin
    if(!reset_n) begin
      state<=S_IDLE;signature_numerator<=0;
      busy<=0;done<=0;error<=0;error_code<=0;
      verified<=0;y_parity<=0;recovery_high<=0;
      signature_r<=0;signature_s<=0;
    end else begin
      done<=1'b0;
      case(state)
        S_IDLE:begin
          if(message_load_valid)
            signature_numerator<={signature_numerator[247:0],message_load_byte};
          if(start) begin
            // BTC copies the completed BIP143 digest here.  ETH has already
            // shifted exactly 32 UART bytes into the same register.
            if(!message_preloaded) signature_numerator<=message_hash;
            error<=0;error_code<=0;verified<=0;y_parity<=0;recovery_high<=0;
            signature_r<=0;signature_s<=0;
            if(PRIVATE_KEY==0 || PRIVATE_KEY>=SECP256K1_N) begin
              error<=1;error_code<=1;done<=1;
            end else begin
              busy<=1;state<=S_NONCE_START;
            end
          end
        end
        S_NONCE_START:state<=S_NONCE_WAIT;
        S_NONCE_RETRY:state<=S_NONCE_WAIT;
        S_NONCE_WAIT:if(nonce_done) state<=S_POINT_START;
        S_POINT_START:state<=S_POINT_WAIT;
        S_POINT_WAIT:if(point_done) begin
          if(point_infinity || point_x==0 || point_x==SECP256K1_N)
            state<=S_NONCE_RETRY;
          else begin
            signature_r<=(point_x>SECP256K1_N)?point_x-SECP256K1_N:point_x;
            y_parity<=point_y[0];
            recovery_high<=(point_x>SECP256K1_N);
            state<=S_RKEY_MUL_START;
          end
        end
        S_RKEY_MUL_START:state<=S_RKEY_MUL_WAIT;
        S_RKEY_MUL_WAIT:if(arithmetic_mul_done) begin
          // arithmetic_mul_result remains stable while the shared adder uses
          // it in the following states.
          state<=S_NUM_ADD_START;
        end
        S_NUM_ADD_START:state<=S_NUM_ADD_WAIT;
        S_NUM_ADD_WAIT:if(shared_add_done) begin
          signature_numerator<=shared_add_result;state<=S_NONCE_INV_START;
        end
        S_NONCE_INV_START:state<=S_NONCE_INV_WAIT;
        S_NONCE_INV_WAIT:if(arithmetic_inv_done) begin
          state<=S_SIG_MUL_START;
        end
        S_SIG_MUL_START:state<=S_SIG_MUL_WAIT;
        S_SIG_MUL_WAIT:if(arithmetic_mul_done) begin
          if(arithmetic_mul_result==0 || signature_r==0) state<=S_NONCE_RETRY;
          else begin
            signature_s<=(arithmetic_mul_result>SECP256K1_N_HALF) ?
                        SECP256K1_N-arithmetic_mul_result : arithmetic_mul_result;
            if(arithmetic_mul_result>SECP256K1_N_HALF) y_parity<=~y_parity;
            state<=(arithmetic_mul_result>SECP256K1_N_HALF) ?
                   S_VERIFY_NEG_START : S_VERIFY_MUL_START;
          end
        end
        S_VERIFY_NEG_START:state<=S_VERIFY_NEG_WAIT;
        S_VERIFY_NEG_WAIT:if(shared_add_done) begin
          signature_numerator<=shared_add_result;state<=S_VERIFY_MUL_START;
        end
        S_VERIFY_MUL_START:state<=S_VERIFY_MUL_WAIT;
        S_VERIFY_MUL_WAIT:if(arithmetic_mul_done) begin
          busy<=0;done<=1;
          if(arithmetic_mul_result!=signature_numerator) begin
            error<=1;error_code<=3;verified<=0;
          end else verified<=1;
          state<=S_IDLE;
        end
        default:begin busy<=0;error<=1;error_code<=3;done<=1;state<=S_IDLE;end
      endcase
    end
  end
endmodule
