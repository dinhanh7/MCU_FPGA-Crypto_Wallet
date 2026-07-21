`timescale 1ns/1ps

module spi_mcu_fpga_bridge (
    input  wire        clk,
    input  wire        rst_n,

    // SPI Slave Interface (từ MCU Sonix)
    input  wire        spi_cs_n,
    input  wire        spi_sclk,
    input  wire        spi_mosi,
    output reg         spi_miso,

    // Giao tiếp với lõi Ký (Signer Core)
    output reg         start_sign,
    output reg [255:0] msg_hash_m,
    input  wire [255:0] private_key_d, // Sẽ được cấp bởi AES
    input  wire [255:0] signature_r,
    input  wire [255:0] signature_s,
    input  wire         sign_done,
    input  wire         sign_error,

    // Tín hiệu Trigger AES
    output reg         trigger_aes,
    output reg [31:0]  pin_code,
    input  wire        aes_done
);

    // -----------------------------------------------------------
    // SPI Receiver / Transmitter Logic (Clock Domain Crossing)
    // -----------------------------------------------------------
    reg [2:0] sclk_sync;
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) sclk_sync <= 3'b000;
        else sclk_sync <= {sclk_sync[1:0], spi_sclk};
    end
    wire sclk_rising = (sclk_sync[2:1] == 2'b01);
    wire sclk_falling = (sclk_sync[2:1] == 2'b10);

    reg [7:0] rx_byte;
    reg [2:0] bit_cnt;
    reg       rx_done;

    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            rx_byte <= 8'h00;
            bit_cnt <= 3'd7;
            rx_done <= 1'b0;
        end else if (!spi_cs_n) begin
            rx_done <= 1'b0;
            if (sclk_rising) begin
                rx_byte[bit_cnt] <= spi_mosi;
                if (bit_cnt == 0) begin
                    bit_cnt <= 3'd7;
                    rx_done <= 1'b1;
                end else begin
                    bit_cnt <= bit_cnt - 1'b1;
                end
            end
        end else begin
            bit_cnt <= 3'd7;
            rx_done <= 1'b0;
        end
    end

    // -----------------------------------------------------------
    // Protocol FSM
    // Lệnh 0x01: Nhận 32 byte Hash + 4 byte PIN -> Kích hoạt AES -> Ký
    // Lệnh 0x02: Đẩy 64 byte Chữ ký (r, s) ra MISO
    // -----------------------------------------------------------
    typedef enum logic [3:0] {
        IDLE,
        RX_CMD,
        RX_HASH,
        RX_PIN,
        WAIT_AES,
        WAIT_SIGN,
        TX_SIG
    } state_t;

    state_t state;
    reg [5:0] byte_cnt;
    
    // TX Logic
    reg [7:0] tx_byte;
    reg [511:0] full_signature;

    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            state <= IDLE;
            msg_hash_m <= 0;
            pin_code <= 0;
            trigger_aes <= 0;
            start_sign <= 0;
            byte_cnt <= 0;
        end else begin
            trigger_aes <= 1'b0;
            start_sign <= 1'b0;

            if (spi_cs_n) begin
                state <= IDLE;
            end else begin
                case (state)
                    IDLE: begin
                        byte_cnt <= 0;
                        if (rx_done) begin
                            if (rx_byte == 8'h01) state <= RX_HASH;
                            else if (rx_byte == 8'h02) begin
                                state <= TX_SIG;
                            end
                        end
                    end
                    
                    RX_HASH: begin
                        if (rx_done) begin
                            msg_hash_m <= {msg_hash_m[247:0], rx_byte};
                            byte_cnt <= byte_cnt + 1'b1;
                            if (byte_cnt == 6'd31) begin
                                byte_cnt <= 0;
                                state <= RX_PIN;
                            end
                        end
                    end

                    RX_PIN: begin
                        if (rx_done) begin
                            pin_code <= {pin_code[23:0], rx_byte};
                            byte_cnt <= byte_cnt + 1'b1;
                            if (byte_cnt == 6'd3) begin
                                trigger_aes <= 1'b1;
                                state <= WAIT_AES;
                            end
                        end
                    end

                    WAIT_AES: begin
                        if (aes_done) begin
                            start_sign <= 1'b1;
                            state <= WAIT_SIGN;
                        end
                    end

                    WAIT_SIGN: begin
                        if (sign_done || sign_error) begin
                            state <= IDLE; // Chờ MCU gửi lệnh 0x02 để đọc
                        end
                    end
                    
                    TX_SIG: begin
                        // Truyền dữ liệu SPI ở TX_SIG sẽ được xử lý ở falling edge phía dưới
                    end
                endcase
            end
        end
    end

    // -----------------------------------------------------------
    // SPI TX Shift Register (cập nhật khi sclk_falling)
    // -----------------------------------------------------------
    reg [2:0] tx_bit_cnt;
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            spi_miso <= 1'b0;
            tx_bit_cnt <= 3'd7;
        end else if (!spi_cs_n) begin
            if (state == IDLE && rx_done && rx_byte == 8'h02) begin
                full_signature <= {signature_r, signature_s};
            end else if (state == TX_SIG && sclk_falling) begin
                spi_miso <= full_signature[511];
                full_signature <= {full_signature[510:0], 1'b0};
            end
        end else begin
            spi_miso <= 1'bz; // Nhả bus MISO khi không được Select
        end
    end

endmodule
