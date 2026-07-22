.PHONY: all mcu mcu-test fpga-check fpga-sim fpga-gowin web-test test clean

all: mcu

mcu:
	$(MAKE) -C mcu all

mcu-test:
	$(MAKE) -C mcu test

fpga-check:
	$(MAKE) -C fpga lint synth-check

fpga-sim:
	$(MAKE) -C fpga test

fpga-gowin:
	cd fpga/gowin && QT_IM_MODULE=compose gw_sh build_dual_acg525.tcl

web-test:
	cd web/ethereum && /home/abuntu/miniconda3/envs/crypto/bin/python \
		-m unittest discover -s tests -v

test: mcu-test fpga-check web-test

clean:
	$(MAKE) -C mcu clean
	$(MAKE) -C fpga clean
