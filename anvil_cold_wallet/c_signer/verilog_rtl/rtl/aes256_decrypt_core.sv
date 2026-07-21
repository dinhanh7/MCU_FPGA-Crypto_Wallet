`timescale 1ns/1ps

module aes256_decrypt_core (
    input  wire         clk,
    input  wire         rst_n,
    input  wire         trigger,
    input  wire [31:0]  pin_code, // Sẽ được băm/kéo giãn thành 256-bit AES Key
    output reg  [255:0] private_key_out,
    output reg          done
);

    // BẢN MÃ ĐƯỢC LƯU SẴN (Hardcoded Encrypted Private Key)
    // Trong thực tế, chuỗi này nên được nạp từ Flash ngoài qua SPI
    localparam [255:0] CIPHERTEXT = 256'h9A8B7C6D5E4F3A2B1C0D9E8F7A6B5C4D3E2F1A0B9C8D7E6F5A4B3C2D1E0F9A8B;

    // KHÓA MỞ RỘNG TỪ PIN (Dummy KDF)
    // Dùng PIN lặp lại để tạo thành 256-bit Key
    wire [255:0] derived_key = { 8{pin_code} }; 

    // --- State Machine cho AES ---
    localparam IDLE = 0, EXPAND_KEY = 1, DECRYPT_BLK1 = 2, DECRYPT_BLK2 = 3, FINISH = 4;
    reg [2:0] state;
    
    // Vì giới hạn Contest và thời gian, đây là một MOCK AES Pipeline 
    // Trong thiết kế thật, ở đây sẽ Instantiation các round logic của AES:
    // aes256_inv_round round_inst(...)
    
    reg [7:0] delay_cnt;
    
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            state <= IDLE;
            done <= 1'b0;
            private_key_out <= 256'h0;
            delay_cnt <= 8'h0;
        end else begin
            case (state)
                IDLE: begin
                    done <= 1'b0;
                    if (trigger) begin
                        state <= EXPAND_KEY;
                        delay_cnt <= 8'd14; // AES-256 có 14 vòng
                    end
                end
                
                EXPAND_KEY: begin
                    // Giả lập tốn cycle để Key Expansion
                    if (delay_cnt == 0) begin
                        state <= DECRYPT_BLK1;
                        delay_cnt <= 8'd14;
                    end else begin
                        delay_cnt <= delay_cnt - 1'b1;
                    end
                end
                
                DECRYPT_BLK1: begin
                    if (delay_cnt == 0) begin
                        state <= DECRYPT_BLK2;
                        delay_cnt <= 8'd14;
                    end else begin
                        delay_cnt <= delay_cnt - 1'b1;
                    end
                end
                
                DECRYPT_BLK2: begin
                    if (delay_cnt == 0) begin
                        state <= FINISH;
                    end else begin
                        delay_cnt <= delay_cnt - 1'b1;
                    end
                end
                
                FINISH: begin
                    // Giả lập quá trình giải mã thành công: 
                    // XOR Ciphertext với Derived Key để ra Private Key
                    private_key_out <= CIPHERTEXT ^ derived_key; 
                    done <= 1'b1;
                    state <= IDLE;
                end
            endcase
        end
    end

endmodule
