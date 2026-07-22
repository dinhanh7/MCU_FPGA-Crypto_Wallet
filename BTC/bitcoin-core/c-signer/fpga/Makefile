VERILATOR ?= conda run -n crypto verilator
YOSYS ?= conda run -n crypto yosys

SHARED := ../../../../ETH/anvil_cold_wallet/c_signer/verilog_rtl/rtl
COMMON_RTL := $(SHARED)/modaddsub256_seq.sv \
              $(SHARED)/modmul256_controller.sv \
              $(SHARED)/modinv256_controller.sv \
              $(SHARED)/secp256k1_point_mul.sv \
              $(SHARED)/rfc6979_hmac_sha256.sv \
              $(SHARED)/rfc6979_nonce.sv
BTC_RTL := rtl/btc_sha256_stream.sv rtl/btc_bip143_hash.sv rtl/btc_rfc6979_hmac_sha256.sv \
           rtl/btc_rfc6979_nonce.sv rtl/btc_ecdsa_sign_verify_core.sv \
           rtl/btc_fpga_signer_core.sv
UART_RTL := $(SHARED)/uart_tx_8n1.sv $(SHARED)/uart_rx_8n1.sv \
            rtl/btc_uart_signer_bridge.sv

.PHONY: test core uart selftest lint synth-check clean

test: core uart selftest

core: build/core/Vtb_btc_fpga_signer_core
	./$<

uart: build/uart/Vtb_btc_uart_signer
	./$<

selftest: build/selftest/Vtb_acg525_selftest_top
	./$<

build/core/Vtb_btc_fpga_signer_core: $(COMMON_RTL) $(BTC_RTL) tb/tb_btc_fpga_signer_core.sv
	mkdir -p build/core
	$(VERILATOR) --binary --timing -O2 -Wno-fatal -Wno-TIMESCALEMOD -Wno-WIDTH \
		-I$(SHARED) --top-module tb_btc_fpga_signer_core --Mdir build/core \
		$(COMMON_RTL) $(BTC_RTL) tb/tb_btc_fpga_signer_core.sv

build/uart/Vtb_btc_uart_signer: $(COMMON_RTL) $(BTC_RTL) $(UART_RTL) tb/tb_btc_uart_signer.sv
	mkdir -p build/uart
	$(VERILATOR) --binary --timing -O2 -Wno-fatal -Wno-TIMESCALEMOD -Wno-WIDTH \
		-I$(SHARED) --top-module tb_btc_uart_signer --Mdir build/uart \
		$(COMMON_RTL) $(BTC_RTL) $(UART_RTL) tb/tb_btc_uart_signer.sv

build/selftest/Vtb_acg525_selftest_top: $(COMMON_RTL) $(BTC_RTL) gowin/acg525_btc_selftest_top.sv tb/tb_acg525_selftest_top.sv
	mkdir -p build/selftest
	$(VERILATOR) --binary --timing -O2 -Wno-fatal -Wno-TIMESCALEMOD -Wno-WIDTH \
		--relative-includes -I$(SHARED) --top-module tb_acg525_selftest_top \
		--Mdir build/selftest $(COMMON_RTL) $(BTC_RTL) \
		gowin/acg525_btc_selftest_top.sv tb/tb_acg525_selftest_top.sv

lint:
	$(VERILATOR) --lint-only --timing -Wno-fatal -Wno-TIMESCALEMOD -Wno-WIDTH \
		-I$(SHARED) --top-module btc_uart_signer_bridge \
		$(COMMON_RTL) $(BTC_RTL) $(UART_RTL)

synth-check:
	mkdir -p build
	$(YOSYS) -q -l build/yosys_btc.log -p 'read_verilog -sv -I$(SHARED) $(COMMON_RTL) $(BTC_RTL) $(UART_RTL); hierarchy -check -top btc_uart_signer_bridge; proc; check'

clean:
	rm -rf build
