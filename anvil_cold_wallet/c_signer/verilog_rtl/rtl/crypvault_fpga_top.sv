`timescale 1ns/1ps

module crypvault_fpga_top (
    input  wire clk,
    input  wire rst_n,

    // Giao tiếp SPI với MCU Sonix SN34F788
    input  wire spi_cs_n,
    input  wire spi_sclk,
    input  wire spi_mosi,
    output wire spi_miso
);

    // Dây tín hiệu (Wires) nối các module
    wire         start_sign;
    wire         sign_done;
    wire         sign_error;
    wire [255:0] msg_hash_m;
    wire [255:0] signature_r;
    wire [255:0] signature_s;
    wire         y_parity;

    wire         trigger_aes;
    wire         aes_done;
    wire [31:0]  pin_code;
    wire [255:0] private_key_d;
    
    wire [255:0] trng_nonce_k;

    // -------------------------------------------------------------
    // 1. Cầu nối SPI (SPI Slave Bridge)
    // -------------------------------------------------------------
    spi_mcu_fpga_bridge spi_bridge_inst (
        .clk(clk),
        .rst_n(rst_n),
        .spi_cs_n(spi_cs_n),
        .spi_sclk(spi_sclk),
        .spi_mosi(spi_mosi),
        .spi_miso(spi_miso),

        .start_sign(start_sign),
        .msg_hash_m(msg_hash_m),
        .private_key_d(private_key_d),
        .signature_r(signature_r),
        .signature_s(signature_s),
        .sign_done(sign_done),
        .sign_error(sign_error),

        .trigger_aes(trigger_aes),
        .pin_code(pin_code),
        .aes_done(aes_done)
    );

    // -------------------------------------------------------------
    // 2. Lõi Ký Chữ Ký Điện Tử (ECDSA SECP256K1 Core)
    // -------------------------------------------------------------
    eth_signer_core ecdsa_core_inst (
        .clk(clk),
        .reset_n(rst_n),
        .start(start_sign),
        
        .msg_hash_m(msg_hash_m),
        .private_key_d(private_key_d),
        .trng_nonce_k(trng_nonce_k),
        
        .signature_r(signature_r),
        .signature_s(signature_s),
        .y_parity(y_parity),
        
        .busy(),
        .done(sign_done),
        .error(sign_error)
    );

    // -------------------------------------------------------------
    // 3. Khối giải mã AES-256-GCM (Chờ Team phần cứng code)
    // -------------------------------------------------------------
    aes256_decrypt_core aes_inst (
        .clk(clk),
        .rst_n(rst_n),
        .trigger(trigger_aes),
        .pin_code(pin_code),
        .private_key_out(private_key_d),
        .done(aes_done)
    );
    


    // -------------------------------------------------------------
    // 4. Mạch tạo số ngẫu nhiên vật lý (TRNG) (Chờ Team phần cứng code)
    // -------------------------------------------------------------
    trng_ring_oscillator trng_inst (
        .clk(clk),
        .rst_n(rst_n),
        .random_out(trng_nonce_k),
        .valid() // Bỏ qua chân valid trong thiết kế này (luôn lấy sample mới nhất)
    );
    


endmodule
