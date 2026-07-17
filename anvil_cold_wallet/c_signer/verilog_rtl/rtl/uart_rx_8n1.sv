`timescale 1ns/1ps

module uart_rx_8n1 #(
  parameter integer CLOCKS_PER_BIT = 434
) (
  input  logic       clk,
  input  logic       reset_n,
  input  logic       rx,
  output logic [7:0] data,
  output logic       data_valid,
  output logic       framing_error
);
  localparam integer COUNTER_WIDTH =
    (CLOCKS_PER_BIT <= 2) ? 1 : $clog2(CLOCKS_PER_BIT);
  localparam integer HALF_BIT = CLOCKS_PER_BIT/2;

  typedef enum logic [1:0] {RX_IDLE,RX_START,RX_DATA,RX_STOP} rx_state_t;
  rx_state_t state;
  logic rx_meta,rx_sync;
  logic [COUNTER_WIDTH-1:0] clock_count;
  logic [2:0] bit_index;
  logic [7:0] data_reg;

  always_ff @(posedge clk or negedge reset_n) begin
    if(!reset_n) begin
      rx_meta<=1'b1;
      rx_sync<=1'b1;
    end else begin
      rx_meta<=rx;
      rx_sync<=rx_meta;
    end
  end

  always_ff @(posedge clk or negedge reset_n) begin
    if(!reset_n) begin
      state<=RX_IDLE;
      clock_count<=0;
      bit_index<=0;
      data_reg<=0;
      data<=0;
      data_valid<=1'b0;
      framing_error<=1'b0;
    end else begin
      data_valid<=1'b0;
      framing_error<=1'b0;
      case(state)
        RX_IDLE: begin
          clock_count<=0;
          bit_index<=0;
          if(!rx_sync) state<=RX_START;
        end

        RX_START: begin
          if(clock_count==HALF_BIT-1) begin
            clock_count<=0;
            if(!rx_sync) state<=RX_DATA;
            else state<=RX_IDLE;
          end else clock_count<=clock_count+1'b1;
        end

        RX_DATA: begin
          if(clock_count==CLOCKS_PER_BIT-1) begin
            clock_count<=0;
            data_reg[bit_index]<=rx_sync;
            if(bit_index==3'd7) begin
              bit_index<=0;
              state<=RX_STOP;
            end else bit_index<=bit_index+1'b1;
          end else clock_count<=clock_count+1'b1;
        end

        RX_STOP: begin
          if(clock_count==CLOCKS_PER_BIT-1) begin
            clock_count<=0;
            if(rx_sync) begin
              data<=data_reg;
              data_valid<=1'b1;
            end else framing_error<=1'b1;
            state<=RX_IDLE;
          end else clock_count<=clock_count+1'b1;
        end

        default: state<=RX_IDLE;
      endcase
    end
  end
endmodule
