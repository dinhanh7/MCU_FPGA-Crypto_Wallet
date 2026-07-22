VERILATOR ?= conda run -n crypto verilator
YOSYS ?= conda run -n crypto yosys

COMMON_RTL := rtl/modaddsub256_seq.sv rtl/modmul256_controller.sv \
	rtl/modinv256_controller.sv rtl/secp256k1_point_mul.sv \
	rtl/rfc6979_hmac_sha256.sv rtl/rfc6979_nonce.sv
BTC_RTL := rtl/btc_sha256_stream.sv rtl/btc_bip143_hash.sv \
	rtl/btc_rfc6979_hmac_sha256.sv rtl/btc_rfc6979_nonce.sv \
	rtl/btc_ecdsa_sign_verify_core.sv rtl/btc_fpga_signer_core.sv
UART_RTL := rtl/uart_tx_8n1.sv rtl/uart_rx_8n1.sv \
	rtl/btc_uart_signer_bridge.sv rtl/dual_coin_uart_signer_bridge.sv

.PHONY: test core uart dual lint synth-check clean

test: core uart dual

core: build/core/Vtb_btc_fpga_signer_core
	./$<

uart: build/uart/Vtb_btc_uart_signer
	./$<

dual: build/dual/Vtb_dual_coin_uart_signer
	./$<

build/core/Vtb_btc_fpga_signer_core: $(COMMON_RTL) $(BTC_RTL) tb/tb_btc_fpga_signer_core.sv
	mkdir -p build/core
	$(VERILATOR) --binary --timing -O2 -Wno-fatal -Wno-TIMESCALEMOD -Wno-WIDTH \
		-Irtl --top-module tb_btc_fpga_signer_core --Mdir build/core \
		$(COMMON_RTL) $(BTC_RTL) tb/tb_btc_fpga_signer_core.sv

build/uart/Vtb_btc_uart_signer: $(COMMON_RTL) $(BTC_RTL) $(UART_RTL) tb/tb_btc_uart_signer.sv
	mkdir -p build/uart
	$(VERILATOR) --binary --timing -O2 -Wno-fatal -Wno-TIMESCALEMOD -Wno-WIDTH \
		-Irtl --top-module tb_btc_uart_signer --Mdir build/uart \
		$(COMMON_RTL) $(BTC_RTL) $(UART_RTL) tb/tb_btc_uart_signer.sv

build/dual/Vtb_dual_coin_uart_signer: $(COMMON_RTL) $(BTC_RTL) $(UART_RTL) tb/tb_dual_coin_uart_signer.sv
	mkdir -p build/dual
	$(VERILATOR) --binary --timing -O2 -Wno-fatal -Wno-TIMESCALEMOD -Wno-WIDTH \
		-Irtl --top-module tb_dual_coin_uart_signer --Mdir build/dual \
		$(COMMON_RTL) $(BTC_RTL) $(UART_RTL) tb/tb_dual_coin_uart_signer.sv

lint:
	$(VERILATOR) --lint-only --timing -Wno-fatal -Wno-TIMESCALEMOD -Wno-WIDTH \
		-Irtl --top-module dual_coin_uart_signer_bridge \
		$(COMMON_RTL) $(BTC_RTL) $(UART_RTL)

synth-check:
	mkdir -p build
	$(YOSYS) -q -l build/yosys.log -p 'read_verilog -sv -Irtl $(COMMON_RTL) $(BTC_RTL) $(UART_RTL); hierarchy -check -top dual_coin_uart_signer_bridge; proc; check'

clean:
	rm -rf build
