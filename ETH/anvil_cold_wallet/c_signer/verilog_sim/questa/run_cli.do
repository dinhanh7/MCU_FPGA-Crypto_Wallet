onerror {quit -code 1}
if {[file exists work]} {vdel -all -lib work}
vlib work
vlog -sv +incdir+../rtl ../rtl/eth_signer_pkg.sv ../rtl/eth_signer_model.sv ../tb/tb_cli.sv
vsim -c work.tb_cli {*}$argv
run -all
quit -code 0
