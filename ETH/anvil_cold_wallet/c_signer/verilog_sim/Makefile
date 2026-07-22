VERILATOR ?= conda run -n crypto verilator
IVERILOG ?= iverilog

RTL := rtl/eth_signer_pkg.sv rtl/eth_signer_model.sv

.PHONY: test compare cli iverilog-lint clean

test: build/verilator_test/Vtb_eth_signer
	./$<

compare: build/verilator_cli/Vtb_cli
	python3 tests/compare_with_c.py $<

cli: build/verilator_cli/Vtb_cli
	./$< $(ARGS)

build/verilator_test/Vtb_eth_signer: $(RTL) tb/tb_eth_signer.sv rtl/private_key_sim.svh
	mkdir -p build
	$(VERILATOR) --binary --timing -O0 -Wno-fatal -Wno-TIMESCALEMOD \
		-Irtl --top-module tb_eth_signer --Mdir build/verilator_test \
		$(RTL) tb/tb_eth_signer.sv

build/verilator_cli/Vtb_cli: $(RTL) tb/tb_cli.sv rtl/private_key_sim.svh
	mkdir -p build
	$(VERILATOR) --binary --timing -O0 -Wno-fatal -Wno-TIMESCALEMOD \
		-Irtl --top-module tb_cli --Mdir build/verilator_cli \
		$(RTL) tb/tb_cli.sv

iverilog-lint:
	$(IVERILOG) -g2012 -Irtl -tnull $(RTL) tb/tb_eth_signer.sv

clean:
	rm -rf build questa/work questa/transcript questa/vsim.wlf
