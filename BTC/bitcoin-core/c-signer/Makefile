CC ?= cc
CFLAGS ?= -O2 -std=c11 -Wall -Wextra -Wpedantic
LDLIBS ?= -lcrypto -lz

BIN := bin/coldsign
SRC := src/coldsign.c src/fpga_protocol.c src/qrcodegen.c

.PHONY: all clean test fpga-protocol-test

all: $(BIN)

$(BIN): $(SRC) src/qrcodegen.h src/fpga_protocol.h
	mkdir -p bin
	$(CC) $(CFLAGS) -Wno-deprecated-declarations $(SRC) -o $(BIN) $(LDLIBS)

test: $(BIN) fpga-protocol-test
	mkdir -p tests
	$(BIN) selftest | tee tests/selftest.txt

fpga-protocol-test:
	$(CC) $(CFLAGS) -Isrc tests/test_fpga_protocol.c src/fpga_protocol.c \
		-o /tmp/btc-fpga-protocol-test
	/tmp/btc-fpga-protocol-test

clean:
	rm -f $(BIN)
