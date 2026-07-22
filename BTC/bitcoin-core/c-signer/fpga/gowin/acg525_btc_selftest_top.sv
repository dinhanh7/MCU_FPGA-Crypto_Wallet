`timescale 1ns/1ps
`include "../rtl/btc_private_key_rtl.svh"

// FPGA-only bring-up image. It executes the complete BIP143 -> RFC6979 ->
// ECDSA -> internal verification path using a fixed public test vector.
module acg525_btc_selftest_top (
  input  logic clk_in_50m,
  output logic led_busy,
  output logic led_pass
);
  localparam logic [495:0] OUTPUTS = 496'h40787d0100000000160014c5be098348757c63d5a3ffd69f9f46797c53d64bd8e84f1c0000000016001473aa96b8e4dcd3e0c0b34500aeb61bfffd557f9d;
  localparam logic [255:0] EXPECTED_DIGEST = 256'h81a90ab2c9368b6e5d143ad2edbbbe325e1b2a0470bcfae2e9f6215ddbb1314c;
  localparam logic [255:0] EXPECTED_R = 256'hf59a0b6afa5e29acbcdcd4212942028e2a13766ce5d7a5f01a6f07f575ce8079;
  localparam logic [255:0] EXPECTED_S = 256'h20f3158b759b6b51540c251e3fa13edf78ff1e35a3bc3b3eba950597135fe0e7;

  typedef enum logic [2:0] {T_RESET,T_START,T_WAIT,T_CHECK,T_FINISHED} state_t;
  state_t state=T_RESET;
  logic [7:0] reset_counter=0;
  logic reset_n;
  logic [4:0] check_index;
  logic core_busy,core_done,core_error,test_pass;
  logic [3:0] core_error_code;
  logic [255:0] digest,signature_r,signature_s;

  assign reset_n=&reset_counter;
  assign led_pass=(state==T_FINISHED)&&test_pass;
  // Busy while running; remains asserted after a failed self-test.
  assign led_busy=(state!=T_FINISHED)||!test_pass;

  always_ff @(posedge clk_in_50m) begin
    if(!reset_n) reset_counter<=reset_counter+1'b1;
  end

  always_ff @(posedge clk_in_50m) begin
    if(!reset_n) begin
      state<=T_RESET;check_index<=0;test_pass<=0;
    end else begin
      case(state)
        T_RESET:state<=T_START;
        T_START:state<=T_WAIT;
        T_WAIT:if(core_done) begin
          test_pass<=!core_error;check_index<=0;state<=T_CHECK;
        end
        // Compare the complete 768-bit known answer one 32-bit word at a
        // time.  A single reused comparator is much easier to route than
        // three parallel 256-bit equality trees on GW5A-25.
        T_CHECK:begin
          case(check_index)
             0:if(digest[255:224]    !=EXPECTED_DIGEST[255:224]) test_pass<=0;
             1:if(digest[223:192]    !=EXPECTED_DIGEST[223:192]) test_pass<=0;
             2:if(digest[191:160]    !=EXPECTED_DIGEST[191:160]) test_pass<=0;
             3:if(digest[159:128]    !=EXPECTED_DIGEST[159:128]) test_pass<=0;
             4:if(digest[127:96]     !=EXPECTED_DIGEST[127:96])  test_pass<=0;
             5:if(digest[95:64]      !=EXPECTED_DIGEST[95:64])   test_pass<=0;
             6:if(digest[63:32]      !=EXPECTED_DIGEST[63:32])   test_pass<=0;
             7:if(digest[31:0]       !=EXPECTED_DIGEST[31:0])    test_pass<=0;
             8:if(signature_r[255:224]!=EXPECTED_R[255:224]) test_pass<=0;
             9:if(signature_r[223:192]!=EXPECTED_R[223:192]) test_pass<=0;
            10:if(signature_r[191:160]!=EXPECTED_R[191:160]) test_pass<=0;
            11:if(signature_r[159:128]!=EXPECTED_R[159:128]) test_pass<=0;
            12:if(signature_r[127:96] !=EXPECTED_R[127:96])  test_pass<=0;
            13:if(signature_r[95:64]  !=EXPECTED_R[95:64])   test_pass<=0;
            14:if(signature_r[63:32]  !=EXPECTED_R[63:32])   test_pass<=0;
            15:if(signature_r[31:0]   !=EXPECTED_R[31:0])    test_pass<=0;
            16:if(signature_s[255:224]!=EXPECTED_S[255:224]) test_pass<=0;
            17:if(signature_s[223:192]!=EXPECTED_S[223:192]) test_pass<=0;
            18:if(signature_s[191:160]!=EXPECTED_S[191:160]) test_pass<=0;
            19:if(signature_s[159:128]!=EXPECTED_S[159:128]) test_pass<=0;
            20:if(signature_s[127:96] !=EXPECTED_S[127:96])  test_pass<=0;
            21:if(signature_s[95:64]  !=EXPECTED_S[95:64])   test_pass<=0;
            22:if(signature_s[63:32]  !=EXPECTED_S[63:32])   test_pass<=0;
            23:if(signature_s[31:0]   !=EXPECTED_S[31:0])    test_pass<=0;
            default:test_pass<=0;
          endcase
          if(check_index==5'd23) state<=T_FINISHED;
          else check_index<=check_index+1'b1;
        end
        default:state<=T_FINISHED;
      endcase
    end
  end

  btc_fpga_signer_core #(
    .PRIVATE_KEY(`BTC_PRIVATE_KEY),.MAX_OUTPUT_BYTES(64),
    .PRELOAD_OUTPUTS(1'b1),.PRELOAD_OUTPUT_DATA({OUTPUTS,16'h0000})
  ) core (
    .clk(clk_in_50m),.reset_n(reset_n),
    .outputs_load_valid(1'b0),.outputs_load_address(10'b0),
    .outputs_load_byte(8'b0),
    .request_length(10'b0),
    .buffer_read_enable(1'b0),.buffer_read_address(10'b0),
    .buffer_read_byte(),
    .start(state==T_START),.tx_version(32'h02000000),
    .outpoint(288'habab683dbf0fe3b0a7b8748e4f39888386240fc1b9ae5b251437f16cf276098301000000),
    .input_sequence(32'hfdffffff),
    .pubkey_hash(160'h73aa96b8e4dcd3e0c0b34500aeb61bfffd557f9d),
    .prevout_amount(64'h0065cd1d00000000),.outputs_length(10'd62),
    .locktime(32'h00000000),.sighash_type(32'h01000000),
    .busy(core_busy),.done(core_done),.error(core_error),
    .error_code(core_error_code),.bip143_digest(digest),
    .signature_r(signature_r),.signature_s(signature_s)
  );
endmodule
