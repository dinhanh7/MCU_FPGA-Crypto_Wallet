`timescale 1ns/1ps

module tb_sha256;
  logic clk = 0;
  logic reset_n = 0;
  always #5 clk = ~clk;
  logic start, in_valid, in_ready, in_last, done;
  logic [7:0] in_byte;
  logic [255:0] digest;

  sha256_stream dut (
    .clk(clk), .reset_n(reset_n), .start(start),
    .in_valid(in_valid), .in_ready(in_ready), .in_byte(in_byte), .in_last(in_last),
    .busy(), .done(done), .digest(digest)
  );

  task automatic begin_hash;
    begin
      @(posedge clk); start <= 1;
      @(posedge clk); start <= 0;
    end
  endtask

  task automatic send_byte(input logic [7:0] value, input logic last);
    begin
      while (!in_ready) @(posedge clk);
      in_byte <= value; in_last <= last; in_valid <= 1;
      @(posedge clk);
      in_valid <= 0; in_last <= 0;
    end
  endtask

  initial begin
    start=0; in_valid=0; in_last=0; in_byte=0;
    repeat (3) @(posedge clk); reset_n=1;

    begin_hash();
    send_byte("a",0); send_byte("b",0); send_byte("c",1);
    wait(done);
    if (digest !== 256'hba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad)
      $fatal(1,"SHA-256 abc failed: %x",digest);

    begin_hash();
    for (integer i=0; i<64; i=i+1) send_byte("a",i==63);
    wait(done);
    if (digest !== 256'hffe054fe7ae0cb6dc65c3af9b61d5209f439851db43d0ba5997337df154668eb)
      $fatal(1,"SHA-256 64-byte path failed: %x",digest);

    $display("PASS: synthesizable streaming SHA-256");
    $finish;
  end
endmodule
