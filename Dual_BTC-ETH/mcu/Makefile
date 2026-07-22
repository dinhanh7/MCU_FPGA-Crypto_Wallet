CC ?= cc
CMAKE ?= cmake
CFLAGS ?= -O2 -std=c11 -Wall -Wextra -Wpedantic

BTC := btc_signer
SECP_SRC := vendor/secp256k1
SECP_BUILD := build/secp256k1
SECP_INC := $(SECP_SRC)/include
SECP_LIB := $(SECP_BUILD)/lib/libsecp256k1.a
OPENSSL_ROOT ?= /home/abuntu/miniconda3/envs/crypto
BUILD := build/obj
BIN := bin/dual-mcu

OBJS := $(BUILD)/dual_mcu.o $(BUILD)/eth_mcu_fpga.o \
	$(BUILD)/btc_coldsign.o $(BUILD)/fpga_protocol.o $(BUILD)/qrcodegen.o

.PHONY: all clean test protocol-test

all: $(BIN)

$(BIN): $(OBJS) $(SECP_LIB)
	mkdir -p bin
	$(CC) $(OBJS) $(SECP_LIB) -L$(OPENSSL_ROOT)/lib \
		-Wl,-rpath,$(OPENSSL_ROOT)/lib -lcrypto -lz -o $@

$(SECP_LIB): $(SECP_SRC)/CMakeLists.txt
	$(CMAKE) -S $(SECP_SRC) -B $(SECP_BUILD) \
		-DBUILD_SHARED_LIBS=OFF -DSECP256K1_ENABLE_MODULE_RECOVERY=ON \
		-DSECP256K1_BUILD_TESTS=OFF -DSECP256K1_BUILD_EXHAUSTIVE_TESTS=OFF \
		-DSECP256K1_BUILD_BENCHMARK=OFF -DSECP256K1_BUILD_EXAMPLES=OFF
	$(CMAKE) --build $(SECP_BUILD) --target secp256k1 -j2

$(BUILD):
	mkdir -p $@

$(BUILD)/dual_mcu.o: src/dual_mcu.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/eth_mcu_fpga.o: src/eth_mcu_fpga.c $(BTC)/src/fpga_protocol.h | $(BUILD)
	$(CC) $(CFLAGS) -I$(BTC)/src -I$(SECP_INC) \
		-I$(OPENSSL_ROOT)/include -c $< -o $@

$(BUILD)/btc_coldsign.o: $(BTC)/src/coldsign.c | $(BUILD)
	$(CC) $(CFLAGS) -Wno-deprecated-declarations -Dmain=btc_mcu_main \
		-I$(BTC)/src -I$(OPENSSL_ROOT)/include -c $< -o $@

$(BUILD)/fpga_protocol.o: $(BTC)/src/fpga_protocol.c $(BTC)/src/fpga_protocol.h | $(BUILD)
	$(CC) $(CFLAGS) -I$(BTC)/src -c $< -o $@

$(BUILD)/qrcodegen.o: $(BTC)/src/qrcodegen.c $(BTC)/src/qrcodegen.h | $(BUILD)
	$(CC) $(CFLAGS) -I$(BTC)/src -c $< -o $@

protocol-test:
	$(CC) $(CFLAGS) -I$(BTC)/src $(BTC)/tests/test_fpga_protocol.c \
		$(BTC)/src/fpga_protocol.c -o /tmp/dual-btc-eth-protocol-test
	/tmp/dual-btc-eth-protocol-test

test: $(BIN) protocol-test
	$(BIN) btc selftest
	$(BIN) eth address test-data/fpga_test_d1.pub

clean:
	rm -rf build bin
