`timescale 1ns/1ps

module acg525_selftest_top (
  input  logic clk_in_50m,
  output logic led_busy,
  output logic led_pass
);
  logic [7:0] reset_counter = 8'b0;
  logic reset_n;
  logic start;
  logic launched;
  logic busy,done,error;
  logic [3:0] error_code;
  logic y_parity;
  logic [255:0] signature_r,signature_s;

  assign reset_n=&reset_counter;
  assign led_busy=busy;

  always_ff @(posedge clk_in_50m) begin
    if(!reset_n)
      reset_counter<=reset_counter+1'b1;
  end

  always_ff @(posedge clk_in_50m) begin
    if(!reset_n) begin
      start<=1'b0;
      launched<=1'b0;
      led_pass<=1'b0;
    end else begin
      start<=1'b0;
      if(!launched) begin
        start<=1'b1;
        launched<=1'b1;
      end
      if(done) begin
        led_pass<=!error &&
          y_parity==1'b0 &&
          signature_r==256'h1b6ca156b695076113ffd52800324239add9650dca3fe132afcf4fa8a3b023c3 &&
          signature_s==256'h7613a92b63f79d019784b0a278689db01cf0d18fbab969c71f4dc8d19b5d2c78;
      end
    end
  end

  eth_hash_signer_core #(
    .PRIVATE_KEY(256'h0000000000000000000000000000000000000000000000000000000000000001)
  ) signer (
    .clk(clk_in_50m),
    .reset_n(reset_n),
    .start(start),
    .message_hash(256'hb11a13b969e57b09787936eaac76c01e8989b68a17b2c18a93554c89d76912f7),
    .busy(busy),
    .done(done),
    .error(error),
    .error_code(error_code),
    .y_parity(y_parity),
    .signature_r(signature_r),
    .signature_s(signature_s)
  );
endmodule
