`timescale 1ns/1ps

// ACG525 hardware smoke-test wrapper.
// It signs one fixed Anvil transaction after power-up using the public test key 1.
// LED2 (N16): signer busy. LED3 (C17): signature/hash matched the expected vector.
module acg525_selftest_top (
  input  logic clk_in_50m,
  output logic led_busy,
  output logic led_pass
);
  logic [7:0] reset_counter = 8'b0;
  logic reset_n;
  logic start;
  logic launched;
  logic busy, done, error;
  logic [3:0] error_code;
  logic [255:0] transaction_hash;

  assign reset_n = &reset_counter;
  assign led_busy = busy;

  always_ff @(posedge clk_in_50m) begin
    if (!reset_n)
      reset_counter <= reset_counter + 1'b1;
  end

  always_ff @(posedge clk_in_50m) begin
    if (!reset_n) begin
      start <= 1'b0;
      launched <= 1'b0;
      led_pass <= 1'b0;
    end else begin
      start <= 1'b0;
      if (!launched) begin
        start <= 1'b1;
        launched <= 1'b1;
      end
      if (done)
        led_pass <= !error &&
          transaction_hash == 256'h2c50c41ecef402f9703d593f0f9556328cd204c02862a870e8063479c0edac00;
    end
  end

  eth_signer_core #(
    // Deliberately public test key. Never fund this address on a real network.
    .PRIVATE_KEY(256'h0000000000000000000000000000000000000000000000000000000000000001),
    // The smoke test only checks the signed transaction hash, so omit the
    // otherwise unused public-key/address derivation to improve device fit.
    .COMPUTE_SIGNER_ADDRESS(1'b0)
  ) signer (
    .clk(clk_in_50m),
    .reset_n(reset_n),
    .start(start),
    .chain_id(256'd31338),
    .nonce(256'd3),
    .max_priority_fee_per_gas(256'd1000000000),
    .max_fee_per_gas(256'd1021491012),
    .gas_limit(256'd21000),
    .recipient(160'h1d6d332f0ab9c6cfd95fac2ba2b8cefd39f012de),
    .value(256'd2000000000000000000),
    .data_length(12'd0),
    .data_write_enable(1'b0),
    .data_write_address(11'b0),
    .data_write_byte(8'b0),
    .busy(busy),
    .done(done),
    .error(error),
    .error_code(error_code),
    .signer_address(),
    .message_hash(),
    .y_parity(),
    .signature_r(),
    .signature_s(),
    .transaction_hash(transaction_hash),
    .raw_transaction_length(),
    .raw_read_enable(1'b0),
    .raw_read_address(12'b0),
    .raw_read_byte()
  );
endmodule
