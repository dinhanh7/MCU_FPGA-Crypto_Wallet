IVERILOG ?= iverilog
VVP ?= vvp
VERILATOR ?= conda run -n crypto verilator
YOSYS ?= conda run -n crypto yosys

RTL := rtl/modaddsub256_seq.sv rtl/modmul256_controller.sv rtl/modmul256.sv rtl/modinv256.sv rtl/modinv256_controller.sv rtl/secp256k1_point_mul.sv \
       rtl/keccak256_stream.sv rtl/sha256_stream.sv \
       rtl/hmac_sha256_32byte_key.sv rtl/rfc6979_hmac_sha256.sv rtl/rfc6979_nonce.sv \
       rtl/eip1559_rlp_encoder.sv rtl/eth_signer_core.sv
COMMON := rtl/eth_secp256k1_common.svh
INCLUDES := -Irtl -I../verilog_sim/rtl

.PHONY: test unit full edge acg525 lint synth-check clean

test: unit full edge

unit: build/tb_modmath build/tb_keccak build/tb_sha256 build/tb_hmac_rfc6979 \
      build/tb_point_mul build/tb_rlp_encoder
	$(VVP) build/tb_modmath
	$(VVP) build/tb_keccak
	$(VVP) build/tb_sha256
	$(VVP) build/tb_hmac_rfc6979
	$(VVP) build/tb_point_mul
	$(VVP) build/tb_rlp_encoder

full: build/verilator_full/Vtb_eth_signer_core
	./$<

edge: build/verilator_edge/Vtb_eth_signer_edges
	./$<

acg525: build/verilator_acg525/Vtb_acg525_selftest
	./$<

lint:
	$(VERILATOR) --lint-only --timing -Wno-fatal -Wno-TIMESCALEMOD -Wno-WIDTH \
		$(INCLUDES) --top-module eth_signer_core $(RTL)

synth-check:
	mkdir -p build
	$(YOSYS) -q -l build/yosys_check.log -p 'read_verilog -sv -Irtl $(RTL); hierarchy -check -top eth_signer_core; proc; check'

build/tb_modmath: $(COMMON) rtl/eth_secp256k1_pkg.sv rtl/modaddsub256_seq.sv rtl/modmul256_controller.sv rtl/modmul256.sv rtl/modinv256.sv tb/tb_modmath.sv
	mkdir -p build
	$(IVERILOG) -g2012 $(INCLUDES) -s tb_modmath -o $@ $^

build/tb_keccak: rtl/keccak256_stream.sv tb/tb_keccak.sv
	mkdir -p build
	$(IVERILOG) -g2012 $(INCLUDES) -s tb_keccak -o $@ $^

build/tb_sha256: rtl/sha256_stream.sv tb/tb_sha256.sv
	mkdir -p build
	$(IVERILOG) -g2012 $(INCLUDES) -s tb_sha256 -o $@ $^

build/tb_hmac_rfc6979: $(COMMON) rtl/eth_secp256k1_pkg.sv rtl/sha256_stream.sv \
                        rtl/hmac_sha256_32byte_key.sv rtl/rfc6979_hmac_sha256.sv rtl/rfc6979_nonce.sv \
                        tb/tb_hmac_rfc6979.sv
	mkdir -p build
	$(IVERILOG) -g2012 $(INCLUDES) -s tb_hmac_rfc6979 -o $@ $^

build/tb_point_mul: $(COMMON) rtl/eth_secp256k1_pkg.sv rtl/modaddsub256_seq.sv rtl/modmul256_controller.sv rtl/modmul256.sv rtl/modinv256.sv \
                    rtl/modinv256_controller.sv \
                    rtl/secp256k1_point_mul.sv tb/tb_point_mul.sv
	mkdir -p build
	$(IVERILOG) -g2012 $(INCLUDES) -s tb_point_mul -o $@ $^

build/tb_rlp_encoder: $(COMMON) rtl/eth_secp256k1_pkg.sv rtl/eip1559_rlp_encoder.sv tb/tb_rlp_encoder.sv
	mkdir -p build
	$(IVERILOG) -g2012 $(INCLUDES) -s tb_rlp_encoder -o $@ $^

build/verilator_full/Vtb_eth_signer_core: $(COMMON) $(RTL) tb/tb_eth_signer_core.sv
	$(VERILATOR) --binary --timing -O2 -Wno-fatal -Wno-TIMESCALEMOD -Wno-WIDTH \
		$(INCLUDES) --top-module tb_eth_signer_core --Mdir build/verilator_full \
		$(RTL) tb/tb_eth_signer_core.sv

build/verilator_edge/Vtb_eth_signer_edges: $(COMMON) $(RTL) tb/tb_eth_signer_edges.sv
	$(VERILATOR) --binary --timing -O2 -Wno-fatal -Wno-TIMESCALEMOD -Wno-WIDTH \
		$(INCLUDES) --top-module tb_eth_signer_edges --Mdir build/verilator_edge \
		$(RTL) tb/tb_eth_signer_edges.sv

build/verilator_acg525/Vtb_acg525_selftest: $(COMMON) $(RTL) gowin/acg525_selftest_top.sv tb/tb_acg525_selftest.sv
	$(VERILATOR) --binary --timing -O2 -Wno-fatal -Wno-TIMESCALEMOD -Wno-WIDTH \
		$(INCLUDES) --top-module tb_acg525_selftest --Mdir build/verilator_acg525 \
		$(RTL) gowin/acg525_selftest_top.sv tb/tb_acg525_selftest.sv

clean:
	rm -rf build
