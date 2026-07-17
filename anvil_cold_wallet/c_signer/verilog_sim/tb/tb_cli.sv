`timescale 1ns/1ps
`include "private_key_sim.svh"

module tb_cli;
  eth_signer_model #(.PRIVATE_KEY(`ETH_PRIVATE_KEY)) dut();

  string command;
  string chain_id;
  string nonce;
  string max_priority_fee_per_gas;
  string max_fee_per_gas;
  string gas_limit;
  string to;
  string value;
  string data;
  string result;

  initial begin
    command = "sign";
    if ($value$plusargs("COMMAND=%s", command)) begin end

    if (command == "address") begin
      dut.address(result);
      $display("%s", result);
      $finish;
    end

    if (!$value$plusargs("CHAIN_ID=%s", chain_id)) $fatal(1, "Missing +CHAIN_ID=<decimal>");
    if (!$value$plusargs("NONCE=%s", nonce)) $fatal(1, "Missing +NONCE=<decimal>");
    if (!$value$plusargs("MAX_PRIORITY_FEE_PER_GAS=%s", max_priority_fee_per_gas))
      $fatal(1, "Missing +MAX_PRIORITY_FEE_PER_GAS=<decimal>");
    if (!$value$plusargs("MAX_FEE_PER_GAS=%s", max_fee_per_gas))
      $fatal(1, "Missing +MAX_FEE_PER_GAS=<decimal>");
    if (!$value$plusargs("GAS_LIMIT=%s", gas_limit)) $fatal(1, "Missing +GAS_LIMIT=<decimal>");
    if (!$value$plusargs("TO=%s", to)) $fatal(1, "Missing +TO=0x<40 hex>");
    if (!$value$plusargs("VALUE=%s", value)) $fatal(1, "Missing +VALUE=<decimal wei>");
    if (!$value$plusargs("DATA=%s", data)) data = "0x";

    dut.sign(chain_id, nonce, max_priority_fee_per_gas, max_fee_per_gas,
      gas_limit, to, value, data, result);
    $display("%s", result);
    $finish;
  end
endmodule
