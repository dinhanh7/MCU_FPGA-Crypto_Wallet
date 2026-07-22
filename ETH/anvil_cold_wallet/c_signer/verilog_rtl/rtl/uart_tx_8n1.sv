`timescale 1ns/1ps

module uart_tx_8n1 #(
  parameter integer CLOCKS_PER_BIT = 434
) (
  input  logic       clk,
  input  logic       reset_n,
  input  logic       start,
  input  logic [7:0] data,
  output logic       tx,
  output logic       busy,
  output logic       done
);
  localparam integer COUNTER_WIDTH =
    (CLOCKS_PER_BIT <= 2) ? 1 : $clog2(CLOCKS_PER_BIT);

  typedef enum logic [1:0] {TX_IDLE,TX_START,TX_DATA,TX_STOP} tx_state_t;
  tx_state_t state;
  logic [COUNTER_WIDTH-1:0] clock_count;
  logic [2:0] bit_index;
  logic [7:0] data_reg;

  always_ff @(posedge clk or negedge reset_n) begin
    if(!reset_n) begin
      state<=TX_IDLE;
      clock_count<=0;
      bit_index<=0;
      data_reg<=0;
      tx<=1'b1;
      busy<=1'b0;
      done<=1'b0;
    end else begin
      done<=1'b0;
      case(state)
        TX_IDLE: begin
          tx<=1'b1;
          busy<=1'b0;
          if(start) begin
            data_reg<=data;
            clock_count<=0;
            bit_index<=0;
            tx<=1'b0;
            busy<=1'b1;
            state<=TX_START;
          end
        end

        TX_START: begin
          if(clock_count==CLOCKS_PER_BIT-1) begin
            clock_count<=0;
            tx<=data_reg[0];
            state<=TX_DATA;
          end else clock_count<=clock_count+1'b1;
        end

        TX_DATA: begin
          if(clock_count==CLOCKS_PER_BIT-1) begin
            clock_count<=0;
            if(bit_index==3'd7) begin
              tx<=1'b1;
              state<=TX_STOP;
            end else begin
              bit_index<=bit_index+1'b1;
              tx<=data_reg[bit_index+1'b1];
            end
          end else clock_count<=clock_count+1'b1;
        end

        TX_STOP: begin
          if(clock_count==CLOCKS_PER_BIT-1) begin
            clock_count<=0;
            busy<=1'b0;
            done<=1'b1;
            state<=TX_IDLE;
          end else clock_count<=clock_count+1'b1;
        end

        default: state<=TX_IDLE;
      endcase
    end
  end
endmodule
