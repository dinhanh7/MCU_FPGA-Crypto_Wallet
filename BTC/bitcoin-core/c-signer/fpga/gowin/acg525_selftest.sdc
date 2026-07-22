create_clock -name clk_in_50m -period 20.000 -waveform {0.000 10.000} [get_ports {clk_in_50m}]
create_generated_clock -name core_clk_12m5 -source [get_ports {clk_in_50m}] -master_clock clk_in_50m -divide_by 4 [get_nets {core_clk}]
