`timescale 1ns/1ps

module acg525_selftest_top (
  input  logic clk_in_50m,
  input  logic btn_start, // Nút bấm kích hoạt (Active Low)
  output logic led_busy,
  output logic led_pass
);
  logic [7:0] reset_counter = 8'b0;
  logic reset_n;
  logic start;
  logic launched;
  logic busy, done, error;
  logic y_parity;
  logic [255:0] signature_r, signature_s;

  assign reset_n = &reset_counter;
  assign led_busy = busy;

  always_ff @(posedge clk_in_50m) begin
    if(!reset_n)
      reset_counter <= reset_counter + 1'b1;
  end

  // =====================================================================
  // GOLDEN TEST VECTORS (Sinh bằng Script/Python hoặc OpenSSL)
  // Lưu ý: ECDSA yêu cầu K ngẫu nhiên. Để test tự động, ta cố định K.
  // =====================================================================
  localparam [255:0] TEST_MSG_HASH = 256'h9c22ff5f21f0b81b113e63f7db6da94fedad110597daeaac144171ea7e4b9864;
  localparam [255:0] TEST_PRIV_KEY = 256'h3b6329aee5582f3fb8d4e13589b91702fcc564c70d067edafbf0357d60920f69;
  localparam [255:0] TEST_NONCE_K  = 256'h1c2b53a0f7e4362a7cf73b5e40e4f24300a0fdfabcf7c9716e91f62306263595;
  
  // Kết quả Signature (R, S) mong đợi (Golden Output)
  localparam [255:0] GOLDEN_R      = 256'hc6bd327072ccb17ec1e9a3028d7a8d59301daea1166699a224976c61fcf1c5ab;
  localparam [255:0] GOLDEN_S      = 256'h03db9363065d648fdfd475ef8c8f5f134568b6fffe7ec7f42d2a4473ef258380;
  localparam TEST_Y_PARITY       = 1'b1;

  logic btn_start_last;

  always_ff @(posedge clk_in_50m) begin
    if(!reset_n) begin
      start <= 1'b0;
      launched <= 1'b0;
      led_pass <= 1'b0;
      btn_start_last <= 1'b1; // Trạng thái nhả nút (Active Low)
    end else begin
      start <= 1'b0;
      btn_start_last <= btn_start;
      
      // Bấm nút (Falling Edge) và hệ thống đang rảnh
      if(btn_start_last == 1'b1 && btn_start == 1'b0 && !launched && !busy) begin
        start <= 1'b1;
        launched <= 1'b1;
        led_pass <= 1'b0; // Tắt đèn cũ để chuẩn bị ký vòng mới
      end
      
      if(done) begin
        // Đối chiếu kết quả của Core với Golden Output
        led_pass <= !error &&
          y_parity == TEST_Y_PARITY &&
          signature_r == GOLDEN_R &&
          signature_s == GOLDEN_S;
          
        launched <= 1'b0; // Mở khóa cho lần test tiếp theo
      end
    end
  end

  // Instantiation Lõi chữ ký mới (Đã loại bỏ RFC6979 cũ)
  eth_signer_core signer (
    .clk(clk_in_50m),
    .reset_n(reset_n),
    .start(start),
    
    // Nạp các Test Vector tĩnh vào Core
    .msg_hash_m(TEST_MSG_HASH),
    .private_key_d(TEST_PRIV_KEY),
    .trng_nonce_k(TEST_NONCE_K),
    
    .busy(busy),
    .done(done),
    .error(error),
    .y_parity(y_parity),
    .signature_r(signature_r),
    .signature_s(signature_s)
  );
endmodule
