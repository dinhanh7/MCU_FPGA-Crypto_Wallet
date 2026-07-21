`timescale 1ns/1ps

module trng_ring_oscillator (
    input  wire         clk,
    input  wire         rst_n,
    output reg  [255:0] random_out,
    output reg          valid
);

    // -------------------------------------------------------------
    // 1. Ring Oscillators (RO)
    // -------------------------------------------------------------
    localparam NUM_RO = 8;
    localparam NUM_INV = 3; // Số lượng Inverter mỗi vòng (phải là số lẻ)

    wire [NUM_RO-1:0] ro_out;

    genvar i, j;
    generate
        for (i = 0; i < NUM_RO; i = i + 1) begin : gen_ro
            // Thuộc tính keep để ngăn Synthesizer tối ưu hóa vòng lặp logic
            (* keep = "true" *) wire [NUM_INV-1:0] inv_chain;
            
            for (j = 0; j < NUM_INV; j = j + 1) begin : gen_inv
                if (j == 0) begin
                    assign inv_chain[0] = ~inv_chain[NUM_INV-1];
                end else begin
                    assign inv_chain[j] = ~inv_chain[j-1];
                end
            end
            assign ro_out[i] = inv_chain[NUM_INV-1];
        end
    endgenerate

    // -------------------------------------------------------------
    // 2. XOR các RO và chống Metastability (Synchronizer)
    // -------------------------------------------------------------
    wire raw_ro_xor;
    assign raw_ro_xor = ^ro_out;

    reg sync_1, sync_2;
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            sync_1 <= 1'b0;
            sync_2 <= 1'b0;
        end else begin
            sync_1 <= raw_ro_xor;
            sync_2 <= sync_1; // Bit nhiễu đã được đồng bộ với System Clock
        end
    end

    // -------------------------------------------------------------
    // 3. Von Neumann Extractor
    // -------------------------------------------------------------
    reg  prev_bit;
    reg  bit_wait;
    reg  vn_valid;
    reg  vn_bit;

    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            prev_bit <= 1'b0;
            bit_wait <= 1'b0;
            vn_valid <= 1'b0;
            vn_bit   <= 1'b0;
        end else begin
            vn_valid <= 1'b0;
            if (!bit_wait) begin
                // Lưu bit đầu tiên của cặp
                prev_bit <= sync_2;
                bit_wait <= 1'b1;
            end else begin
                // Đã có 2 bit, tiến hành so sánh
                if (prev_bit != sync_2) begin
                    // 01 -> 0, 10 -> 1 (Chấp nhận bit đầu tiên)
                    vn_bit   <= prev_bit;
                    vn_valid <= 1'b1;
                end
                // Reset trạng thái chờ bit mới
                bit_wait <= 1'b0;
            end
        end
    end

    // -------------------------------------------------------------
    // 4. Shift Register (Accumulate 256 bits)
    // -------------------------------------------------------------
    reg [8:0] bit_count; // Đếm đến 256

    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            random_out <= 256'b0;
            valid      <= 1'b0;
            bit_count  <= 9'b0;
        end else begin
            if (vn_valid && (bit_count < 9'd256)) begin
                random_out <= {random_out[254:0], vn_bit};
                bit_count  <= bit_count + 1'b1;
            end
            
            if (bit_count == 9'd256) begin
                valid <= 1'b1;
            end else begin
                valid <= 1'b0;
            end
        end
    end

endmodule
