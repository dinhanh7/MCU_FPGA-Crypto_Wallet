`timescale 1ns/1ps

module eth_signer_core #(
  parameter logic [255:0] PRIVATE_KEY = 256'h1,
  parameter logic         COMPUTE_SIGNER_ADDRESS = 1'b1
) (
  input  logic         clk,
  input  logic         reset_n,
  input  logic         start,
  input  logic [255:0] chain_id,
  input  logic [255:0] nonce,
  input  logic [255:0] max_priority_fee_per_gas,
  input  logic [255:0] max_fee_per_gas,
  input  logic [255:0] gas_limit,
  input  logic [159:0] recipient,
  input  logic [255:0] value,
  input  logic [11:0]  data_length,
  input  logic         data_write_enable,
  input  logic [10:0]  data_write_address,
  input  logic [7:0]   data_write_byte,
  output logic         busy,
  output logic         done,
  output logic         error,
  output logic [3:0]   error_code,
  output logic [159:0] signer_address,
  output logic [255:0] message_hash,
  output logic         y_parity,
  output logic [255:0] signature_r,
  output logic [255:0] signature_s,
  output logic [255:0] transaction_hash,
  output logic [12:0]  raw_transaction_length,
  input  logic         raw_read_enable,
  input  logic [11:0]  raw_read_address,
  output logic [7:0]   raw_read_byte
);
`include "eth_secp256k1_common.svh"
  localparam logic [255:0] CURVE_N =
    256'hfffffffffffffffffffffffffffffffebaaedce6af48a03bbfd25e8cd0364141;

  typedef enum logic [5:0] {
    TOP_IDLE,
    TOP_PUBLIC_POINT_START, TOP_PUBLIC_POINT_WAIT,
    TOP_ADDRESS_HASH_START, TOP_ADDRESS_HASH_FEED, TOP_ADDRESS_HASH_WAIT,
    TOP_UNSIGNED_HASH_START, TOP_UNSIGNED_ENCODER_START, TOP_UNSIGNED_WAIT,
    TOP_NONCE_START, TOP_NONCE_WAIT, TOP_NONCE_RETRY,
    TOP_NONCE_POINT_START, TOP_NONCE_POINT_WAIT,
    TOP_RKEY_MUL_START, TOP_RKEY_MUL_WAIT,
    TOP_NONCE_INV_START, TOP_NONCE_INV_WAIT,
    TOP_S_MUL_START, TOP_S_MUL_WAIT,
    TOP_SIGNED_HASH_START, TOP_SIGNED_ENCODER_START, TOP_SIGNED_WAIT
  } top_state_t;

  top_state_t state;
  logic [255:0] chain_id_reg, nonce_field_reg;
  logic [255:0] priority_reg, max_fee_reg, gas_reg, value_reg;
  logic [159:0] recipient_reg;
  logic [11:0] data_length_reg;
  // Keep transaction buffers in the GW5A block RAMs.  In particular, do not
  // put their read ports in an asynchronously-reset process: that prevents
  // both Yosys and Gowin from recognizing synchronous BSRAM templates.
  (* ram_style = "block", syn_ramstyle = "block_ram" *)
  logic [7:0] calldata_memory [0:2047];
  (* ram_style = "block", syn_ramstyle = "block_ram" *)
  logic [7:0] raw_memory [0:4095];

  logic encoder_start, encoder_signed_mode;
  logic encoder_valid, encoder_ready, encoder_last, encoder_done;
  logic [7:0] encoder_byte;
  logic [10:0] encoder_data_address;
  logic [7:0] encoder_data_byte;
  logic [12:0] encoder_length;

  eip1559_rlp_encoder encoder_inst (
    .clk(clk),.reset_n(reset_n),.start(encoder_start),
    .signed_mode(encoder_signed_mode),
    .chain_id(chain_id_reg),.nonce(nonce_field_reg),
    .max_priority_fee_per_gas(priority_reg),.max_fee_per_gas(max_fee_reg),
    .gas_limit(gas_reg),.recipient(recipient_reg),.value(value_reg),
    .data_length(data_length_reg),.data_read_address(encoder_data_address),
    .data_read_byte(encoder_data_byte),.y_parity(y_parity),
    .signature_r(signature_r),.signature_s(signature_s),
    .out_valid(encoder_valid),.out_ready(encoder_ready),
    .out_byte(encoder_byte),.out_last(encoder_last),
    .busy(),.done(encoder_done),.encoded_length(encoder_length)
  );

  logic keccak_start, keccak_in_valid, keccak_in_ready, keccak_in_last, keccak_done;
  logic [7:0] keccak_in_byte;
  logic [255:0] keccak_digest;
  keccak256_stream keccak_inst (
    .clk(clk),.reset_n(reset_n),.start(keccak_start),
    .in_valid(keccak_in_valid),.in_ready(keccak_in_ready),
    .in_byte(keccak_in_byte),.in_last(keccak_in_last),
    .busy(),.done(keccak_done),.digest(keccak_digest)
  );

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

  logic nonce_start, nonce_retry, nonce_done;
  logic [255:0] deterministic_nonce;
  rfc6979_nonce nonce_inst (
    .clk(clk),.reset_n(reset_n),.start(nonce_start),.retry(nonce_retry),
    .private_key(PRIVATE_KEY),.message_hash(message_hash),
    .busy(),.done(nonce_done),.nonce(deterministic_nonce)
  );

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

  // Point multiplication and the final ECDSA arithmetic occupy disjoint top
  // states, so one 32-bit word-serial modular adder safely serves both.
  logic shared_add_start,shared_add_subtract,shared_add_done;
  logic [255:0] shared_add_a,shared_add_b,shared_add_modulus,shared_add_result;
  always_comb begin
    shared_add_start=point_add_start|multiplier_add_start;
    shared_add_subtract=1'b0;
    shared_add_a=multiplier_add_a;
    shared_add_b=multiplier_add_b;
    shared_add_modulus=multiplier_add_modulus;
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

  logic arithmetic_inv_start, arithmetic_inv_done;
  logic [255:0] arithmetic_inv_result;
  logic shared_inv_start;
  logic [255:0] shared_inv_value,shared_inv_modulus;
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

  logic [6:0] public_byte_index;
  logic [255:0] public_x_reg, public_y_reg;
  logic [255:0] nonce_inverse_reg;
  logic [255:0] signature_numerator;
  logic recovery_high_reg;
  logic raw_capture_active;
  logic [12:0] raw_write_index;

  always_comb begin
    encoder_start = (state==TOP_UNSIGNED_ENCODER_START) ||
                    (state==TOP_SIGNED_ENCODER_START);
    encoder_signed_mode = (state==TOP_SIGNED_HASH_START) ||
                          (state==TOP_SIGNED_ENCODER_START) ||
                          (state==TOP_SIGNED_WAIT);

    keccak_start = (state==TOP_ADDRESS_HASH_START) ||
                   (state==TOP_UNSIGNED_HASH_START) ||
                   (state==TOP_SIGNED_HASH_START);
    keccak_in_valid = 1'b0;
    keccak_in_byte = 8'b0;
    keccak_in_last = 1'b0;
    encoder_ready = 1'b0;
    if(state==TOP_ADDRESS_HASH_FEED) begin
      keccak_in_valid=1'b1;
      if(public_byte_index<32)
        keccak_in_byte=public_x_reg[255-public_byte_index*8 -: 8];
      else
        keccak_in_byte=public_y_reg[255-(public_byte_index-32)*8 -: 8];
      keccak_in_last=(public_byte_index==63);
    end else if(state==TOP_UNSIGNED_WAIT || state==TOP_SIGNED_WAIT) begin
      keccak_in_valid=encoder_valid;
      keccak_in_byte=encoder_byte;
      keccak_in_last=encoder_last;
      encoder_ready=keccak_in_ready;
    end

    point_start=(state==TOP_PUBLIC_POINT_START)||(state==TOP_NONCE_POINT_START);
    point_scalar=(state==TOP_PUBLIC_POINT_START||state==TOP_PUBLIC_POINT_WAIT) ?
                 PRIVATE_KEY : deterministic_nonce;
    nonce_start=(state==TOP_NONCE_START);
    nonce_retry=(state==TOP_NONCE_RETRY);

    arithmetic_mul_start=(state==TOP_RKEY_MUL_START)||(state==TOP_S_MUL_START);
    arithmetic_mul_a=signature_r;
    arithmetic_mul_b=PRIVATE_KEY;
    if(state==TOP_S_MUL_START) begin
      arithmetic_mul_a=nonce_inverse_reg;
      arithmetic_mul_b=signature_numerator;
    end
    arithmetic_inv_start=(state==TOP_NONCE_INV_START);

  end

  always_ff @(posedge clk) begin
    if(data_write_enable && !busy)
      calldata_memory[data_write_address] <= data_write_byte;
    encoder_data_byte <= calldata_memory[encoder_data_address];
  end

  always_ff @(posedge clk) begin
    if(raw_read_enable)
      raw_read_byte <= raw_memory[raw_read_address];
    if(raw_capture_active && encoder_valid && encoder_ready && raw_write_index<4096)
      raw_memory[raw_write_index] <= encoder_byte;
  end

  always_ff @(posedge clk or negedge reset_n) begin
    if(!reset_n) begin
      state<=TOP_IDLE;
      chain_id_reg<=0;nonce_field_reg<=0;priority_reg<=0;max_fee_reg<=0;
      gas_reg<=0;value_reg<=0;recipient_reg<=0;data_length_reg<=0;
      busy<=0;done<=0;error<=0;error_code<=0;
      signer_address<=0;message_hash<=0;y_parity<=0;
      signature_r<=0;signature_s<=0;transaction_hash<=0;
      raw_transaction_length<=0;
      public_byte_index<=0;public_x_reg<=0;public_y_reg<=0;
      nonce_inverse_reg<=0;signature_numerator<=0;
      recovery_high_reg<=0;
      raw_capture_active<=0;raw_write_index<=0;
    end else begin
      done<=1'b0;
      case(state)
        TOP_IDLE: if(start) begin
          error<=0;error_code<=0;
          if(PRIVATE_KEY==0 || PRIVATE_KEY>=CURVE_N) begin
            error<=1;error_code<=1;done<=1;
          end else if(data_length>2048) begin
            error<=1;error_code<=2;done<=1;
          end else begin
            chain_id_reg<=chain_id;nonce_field_reg<=nonce;
            priority_reg<=max_priority_fee_per_gas;max_fee_reg<=max_fee_per_gas;
            gas_reg<=gas_limit;recipient_reg<=recipient;value_reg<=value;
            data_length_reg<=data_length;
            signature_r<=0;signature_s<=0;y_parity<=0;
            raw_transaction_length<=0;transaction_hash<=0;
            busy<=1;
            state<=COMPUTE_SIGNER_ADDRESS ? TOP_PUBLIC_POINT_START : TOP_UNSIGNED_HASH_START;
          end
        end

        TOP_PUBLIC_POINT_START: state<=TOP_PUBLIC_POINT_WAIT;
        TOP_PUBLIC_POINT_WAIT: if(point_done) begin
          public_x_reg<=point_x;public_y_reg<=point_y;state<=TOP_ADDRESS_HASH_START;
        end
        TOP_ADDRESS_HASH_START: begin public_byte_index<=0;state<=TOP_ADDRESS_HASH_FEED;end
        TOP_ADDRESS_HASH_FEED: if(keccak_in_ready) begin
          if(public_byte_index==63) state<=TOP_ADDRESS_HASH_WAIT;
          else public_byte_index<=public_byte_index+1'b1;
        end
        TOP_ADDRESS_HASH_WAIT: if(keccak_done) begin
          signer_address<=keccak_digest[159:0];state<=TOP_UNSIGNED_HASH_START;
        end

        TOP_UNSIGNED_HASH_START: state<=TOP_UNSIGNED_ENCODER_START;
        TOP_UNSIGNED_ENCODER_START: state<=TOP_UNSIGNED_WAIT;
        TOP_UNSIGNED_WAIT: if(keccak_done) begin
          message_hash<=keccak_digest;state<=TOP_NONCE_START;
        end

        TOP_NONCE_START: state<=TOP_NONCE_WAIT;
        TOP_NONCE_RETRY: state<=TOP_NONCE_WAIT;
        TOP_NONCE_WAIT: if(nonce_done) state<=TOP_NONCE_POINT_START;
        TOP_NONCE_POINT_START: state<=TOP_NONCE_POINT_WAIT;
        TOP_NONCE_POINT_WAIT: if(point_done) begin
          if(point_x==0 || point_x==CURVE_N) begin
            state<=TOP_NONCE_RETRY;
          end else begin
            signature_r<=(point_x>CURVE_N)?point_x-CURVE_N:point_x;
            y_parity<=point_y[0];
            recovery_high_reg<=(point_x>CURVE_N);
            state<=TOP_RKEY_MUL_START;
          end
        end

        TOP_RKEY_MUL_START: state<=TOP_RKEY_MUL_WAIT;
        TOP_RKEY_MUL_WAIT: if(arithmetic_mul_done) begin
          signature_numerator<=mod_add256(
            (message_hash>=CURVE_N)?message_hash-CURVE_N:message_hash,
            arithmetic_mul_result,CURVE_N);
          state<=TOP_NONCE_INV_START;
        end
        TOP_NONCE_INV_START: state<=TOP_NONCE_INV_WAIT;
        TOP_NONCE_INV_WAIT: if(arithmetic_inv_done) begin
          nonce_inverse_reg<=arithmetic_inv_result;state<=TOP_S_MUL_START;
        end
        TOP_S_MUL_START: state<=TOP_S_MUL_WAIT;
        TOP_S_MUL_WAIT: if(arithmetic_mul_done) begin
          if(arithmetic_mul_result==0 || signature_r==0) begin
            state<=TOP_NONCE_RETRY;
          end else if(recovery_high_reg) begin
            error<=1;error_code<=3;busy<=0;done<=1;state<=TOP_IDLE;
          end else begin
            if(arithmetic_mul_result>SECP256K1_N_HALF) begin
              signature_s<=CURVE_N-arithmetic_mul_result;
              y_parity<=~y_parity;
            end else signature_s<=arithmetic_mul_result;
            state<=TOP_SIGNED_HASH_START;
          end
        end

        TOP_SIGNED_HASH_START: begin
          raw_write_index<=0;raw_capture_active<=1;state<=TOP_SIGNED_ENCODER_START;
        end
        TOP_SIGNED_ENCODER_START: state<=TOP_SIGNED_WAIT;
        TOP_SIGNED_WAIT: begin
          if(encoder_valid&&encoder_ready) raw_write_index<=raw_write_index+1'b1;
          if(encoder_done) begin
            raw_transaction_length<=encoder_length;
            raw_capture_active<=0;
          end
          if(keccak_done) begin
            transaction_hash<=keccak_digest;busy<=0;done<=1;state<=TOP_IDLE;
          end
        end
        default: state<=TOP_IDLE;
      endcase
    end
  end
endmodule
