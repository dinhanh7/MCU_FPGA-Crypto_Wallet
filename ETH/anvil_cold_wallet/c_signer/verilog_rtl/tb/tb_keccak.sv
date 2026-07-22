`timescale 1ns/1ps

module tb_keccak;
  logic clk = 0;
  logic reset_n = 0;
  always #5 clk = ~clk;

  logic start, in_valid, in_ready, in_last, done;
  logic [7:0] in_byte;
  logic [255:0] digest;
  integer cycle_count = 0;

  keccak256_stream dut (
    .clk(clk), .reset_n(reset_n), .start(start),
    .in_valid(in_valid), .in_ready(in_ready), .in_byte(in_byte), .in_last(in_last),
    .busy(), .done(done), .digest(digest)
  );

  always @(posedge clk) begin
    cycle_count <= cycle_count + 1;
    if (cycle_count > 100000)
      $fatal(1,"Keccak timeout: state=%0d round=%0d lane=%0d row=%0d col=%0d byte=%0d accepted_last=%0d busy=%0d done=%0d",
        dut.state,dut.round_index,dut.lane_counter,dut.row_counter,dut.column_counter,
        dut.byte_position,dut.accepted_last,dut.busy,dut.done);
  end

  task automatic begin_hash;
    begin
      @(posedge clk); start <= 1;
      @(posedge clk); start <= 0;
    end
  endtask

  task automatic send_byte(input logic [7:0] value, input logic last);
    begin
      while (!in_ready) @(negedge clk);
      @(negedge clk);
      while (!in_ready) @(negedge clk);
      in_byte = value;
      in_last = last;
      in_valid = 1;
      @(posedge clk);
      @(negedge clk);
      in_valid = 0;
      in_last = 0;
    end
  endtask

  initial begin
    start = 0; in_valid = 0; in_last = 0; in_byte = 0;
    repeat (3) @(posedge clk);
    reset_n = 1;

    // The stream interface represents a non-empty message. Hash one byte here;
    // empty-message coverage is supplied by the full SHA/Keccak regression.
    begin_hash();
    send_byte(8'h61, 1'b1);
    wait (done);
    if (digest !== 256'h3ac225168df54212a25c1c01fd35bebfea408fdac2e31ddd6f80a4bbf9a5f1cb)
      $fatal(1, "Keccak-256('a') failed: %x", digest);

    // Exercise the exact-rate padding path with 136 zero bytes.
    begin_hash();
    for (integer i = 0; i < 136; i = i + 1)
      send_byte(8'h00, i == 135);
    wait (done);
    if (digest !== 256'h3a5912a7c5faa06ee4fe906253e339467a9ce87d533c65be3c15cb231cdb25f9)
      $fatal(1, "Keccak exact-rate vector failed: %x", digest);

    $display("PASS: synthesizable streaming Keccak-256");
    $finish;
  end
endmodule
