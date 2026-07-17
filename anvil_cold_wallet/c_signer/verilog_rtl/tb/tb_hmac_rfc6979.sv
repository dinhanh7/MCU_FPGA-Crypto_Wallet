`timescale 1ns/1ps
`include "private_key_sim.svh"

module tb_hmac_rfc6979;
  logic clk=0, reset_n=0;
  always #5 clk=~clk;

  logic h_start,h_done;
  logic [255:0] h_key;
  logic [775:0] h_message;
  logic [6:0] h_length;
  logic [255:0] h_digest;
  logic r_start,r_done;
  logic [255:0] r_nonce;

  hmac_sha256_32byte_key hmac (
    .clk(clk),.reset_n(reset_n),.start(h_start),.key(h_key),
    .message(h_message),.message_length(h_length),.busy(),.done(h_done),.digest(h_digest));
  rfc6979_nonce rfc (
    .clk(clk),.reset_n(reset_n),.start(r_start),.private_key(`ETH_PRIVATE_KEY),
    .message_hash(256'hb11a13b969e57b09787936eaac76c01e8989b68a17b2c18a93554c89d76912f7),
    .busy(),.done(r_done),.nonce(r_nonce));

  task automatic put_message(input integer index,input logic [7:0] value);
    h_message[775-index*8 -: 8]=value;
  endtask

  initial begin
    h_start=0; r_start=0; h_key=0; h_message=0; h_length=0;
    repeat(3) @(posedge clk); reset_n=1;

    h_key[255 -: 32] = 32'h4a656665; // "Jefe", zero-padded to a 32-byte key.
    put_message(0,"w"); put_message(1,"h"); put_message(2,"a"); put_message(3,"t");
    put_message(4," "); put_message(5,"d"); put_message(6,"o"); put_message(7," ");
    put_message(8,"y"); put_message(9,"a"); put_message(10," "); put_message(11,"w");
    put_message(12,"a"); put_message(13,"n"); put_message(14,"t"); put_message(15," ");
    put_message(16,"f"); put_message(17,"o"); put_message(18,"r"); put_message(19," ");
    put_message(20,"n"); put_message(21,"o"); put_message(22,"t"); put_message(23,"h");
    put_message(24,"i"); put_message(25,"n"); put_message(26,"g"); put_message(27,"?");
    h_length=28;
    @(posedge clk); h_start<=1;
    @(posedge clk); h_start<=0;
    wait(h_done);
    if(h_digest!==256'h5bdcc146bf60754e6a042426089575c75a003f089d2739839dec58b964ec3843)
      $fatal(1,"HMAC failed: %x",h_digest);

    @(posedge clk); r_start<=1;
    @(posedge clk); r_start<=0;
    wait(r_done);
    if(r_nonce!==256'h833634192353b7f66dadec04f7bc71458eea9179e07a65cd0f587e711efd795d)
      $fatal(1,"RFC6979 failed: %x",r_nonce);

    $display("PASS: synthesizable HMAC-SHA256 and RFC6979");
    $finish;
  end
endmodule
