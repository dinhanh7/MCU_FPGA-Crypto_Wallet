`timescale 1ns/1ps
`include "private_key_sim.svh"

module tb_eth_signer;
  import eth_signer_pkg::*;

  eth_signer_model #(.PRIVATE_KEY(`ETH_PRIVATE_KEY)) dut();

  byte_queue_t empty_message;
  u256_t digest;
  string actual_address;
  string actual_json;
  string expected_json;

  initial begin
    empty_message = {};
    sha256(empty_message, digest);
    if (digest !== 256'he3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855)
      $fatal(1, "SHA-256 empty-message vector failed: %064x", digest);

    empty_message = '{8'h61, 8'h62, 8'h63};
    sha256(empty_message, digest);
    if (digest !== 256'hba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad)
      $fatal(1, "SHA-256 abc vector failed: %064x", digest);

    empty_message = {};
    repeat (64) empty_message.push_back(8'h61);
    sha256(empty_message, digest);
    if (digest !== 256'hffe054fe7ae0cb6dc65c3af9b61d5209f439851db43d0ba5997337df154668eb)
      $fatal(1, "SHA-256 multi-block vector failed: %064x", digest);

    begin
      byte_queue_t hmac_key;
      byte_queue_t hmac_message;
      hmac_key = {};
      repeat (20) hmac_key.push_back(8'h0b);
      hmac_message = '{8'h48,8'h69,8'h20,8'h54,8'h68,8'h65,8'h72,8'h65};
      hmac_sha256(hmac_key, hmac_message, digest);
      if (digest !== 256'hb0344c61d8db38535ca8afceaf0bf12b881dc200c9833da726e9376c2e32cff7)
        $fatal(1, "HMAC-SHA256 RFC4231 vector failed: %064x", digest);
    end

    empty_message = {};
    keccak256(empty_message, digest);
    if (digest !== 256'hc5d2460186f7233c927e7db2dcc703c0e500b653ca82273b7bfad8045d85a470)
      $fatal(1, "Ethereum Keccak-256 empty-message vector failed: %064x", digest);

    rfc6979_nonce(`ETH_PRIVATE_KEY,
      256'hb11a13b969e57b09787936eaac76c01e8989b68a17b2c18a93554c89d76912f7,
      digest);
    if (digest !== 256'h833634192353b7f66dadec04f7bc71458eea9179e07a65cd0f587e711efd795d)
      $fatal(1, "RFC6979 vector failed: %064x", digest);

    dut.address(actual_address);
    if (actual_address != "0xc19558ded8d849a994a1d9a37a2d1575bba08b5b")
      $fatal(1, "Signer address mismatch: %s", actual_address);

    dut.sign(
      "31338",
      "3",
      "1000000000",
      "1021491012",
      "21000",
      "0x1d6D332F0aB9C6CFd95FAc2ba2b8CeFD39F012De",
      "2000000000000000000",
      "0x",
      actual_json
    );

    expected_json = {
      "{\n",
      "  \"format\": \"ethereum-c-signer-v1\",\n",
      "  \"from\": \"0xc19558ded8d849a994a1d9a37a2d1575bba08b5b\",\n",
      "  \"messageHash\": \"0xb11a13b969e57b09787936eaac76c01e8989b68a17b2c18a93554c89d76912f7\",\n",
      "  \"yParity\": 0,\n",
      "  \"r\": \"0x3c403f14f95cc9624023f2dbd5dce5df2d502104e3ee4c766ffcdad17444ec3a\",\n",
      "  \"s\": \"0x6a0829a2fae42ce7871a7ae448dd244b636dcfec370d6f3630a3d4b01404b090\",\n",
      "  \"rawTransaction\": \"0x02f874827a6a03843b9aca00843ce2b744825208941d6d332f0ab9c6cfd95fac2ba2b8cefd39f012de881bc16d674ec8000080c080a03c403f14f95cc9624023f2dbd5dce5df2d502104e3ee4c766ffcdad17444ec3aa06a0829a2fae42ce7871a7ae448dd244b636dcfec370d6f3630a3d4b01404b090\",\n",
      "  \"transactionHash\": \"0x10374c522b0b831dd4a6f6d800f759f8bcdff8be899bea1f767e545e8ead5580\"\n",
      "}\n"
    };

    if (actual_json != expected_json) begin
      $display("EXPECTED:\n%s", expected_json);
      $display("ACTUAL:\n%s", actual_json);
      $fatal(1, "SystemVerilog output differs from C signer");
    end

    $display("PASS: SHA-256, Keccak-256, address, RFC6979, secp256k1, RLP and JSON match C signer.");
    $display("%s", actual_json);
    $finish;
  end
endmodule
