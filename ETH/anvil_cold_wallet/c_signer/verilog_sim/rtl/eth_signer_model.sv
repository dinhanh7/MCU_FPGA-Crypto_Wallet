`timescale 1ns/1ps

module eth_signer_model #(
  parameter logic [255:0] PRIVATE_KEY = 256'h1
);
  import eth_signer_pkg::*;

  task automatic address(output string address_text);
    logic [159:0] signer_address;
    if (PRIVATE_KEY == 0 || PRIVATE_KEY >= SECP_N)
      $fatal(1, "PRIVATE_KEY is not a valid secp256k1 scalar");
    derive_address(PRIVATE_KEY, signer_address);
    address_text = $sformatf("0x%040x", signer_address);
  endtask

  task automatic sign(
    input string chain_id,
    input string nonce,
    input string max_priority_fee_per_gas,
    input string max_fee_per_gas,
    input string gas_limit,
    input string to,
    input string value,
    input string data,
    output string json
  );
    byte_queue_t unsigned_payload;
    byte_queue_t unsigned_transaction;
    byte_queue_t signed_payload;
    byte_queue_t raw_transaction;
    byte_queue_t scalar_bytes;
    logic [159:0] signer_address;
    u256_t signing_hash;
    u256_t transaction_hash;
    u256_t r;
    u256_t s;
    int recovery_id;
    string raw_transaction_hex;

    if (PRIVATE_KEY == 0 || PRIVATE_KEY >= SECP_N)
      $fatal(1, "PRIVATE_KEY is not a valid secp256k1 scalar");

    unsigned_payload = {};
    append_transaction_fields(unsigned_payload, chain_id, nonce,
      max_priority_fee_per_gas, max_fee_per_gas, gas_limit, to, value, data);
    make_typed_transaction(unsigned_payload, unsigned_transaction);
    keccak256(unsigned_transaction, signing_hash);
    derive_address(PRIVATE_KEY, signer_address);
    ecdsa_sign(PRIVATE_KEY, signing_hash, r, s, recovery_id);

    signed_payload = {};
    append_transaction_fields(signed_payload, chain_id, nonce,
      max_priority_fee_per_gas, max_fee_per_gas, gas_limit, to, value, data);
    if (recovery_id == 0) scalar_bytes = {};
    else scalar_bytes = '{byte'(recovery_id)};
    rlp_append_bytes(signed_payload,scalar_bytes);
    u256_to_minimal_bytes(r,scalar_bytes); rlp_append_bytes(signed_payload,scalar_bytes);
    u256_to_minimal_bytes(s,scalar_bytes); rlp_append_bytes(signed_payload,scalar_bytes);
    make_typed_transaction(signed_payload,raw_transaction);
    keccak256(raw_transaction,transaction_hash);
    bytes_to_hex(raw_transaction,raw_transaction_hex);

    json = $sformatf(
      "{\n  \"format\": \"ethereum-c-signer-v1\",\n  \"from\": \"0x%040x\",\n  \"messageHash\": \"%s\",\n  \"yParity\": %0d,\n  \"r\": \"%s\",\n  \"s\": \"%s\",\n  \"rawTransaction\": \"%s\",\n  \"transactionHash\": \"%s\"\n}\n",
      signer_address, u256_hex(signing_hash), recovery_id, u256_hex(r), u256_hex(s),
      raw_transaction_hex, u256_hex(transaction_hash));
  endtask
endmodule
