set project_dir [file normalize [file dirname [info script]]]
cd $project_dir
open_project [file join $project_dir btc_signer_acg525.gprj]
set_option -verilog_std sysv2017
set_option -top_module acg525_btc_uart_top
set_option -output_base_name btc_signer_acg525
set_option -opt_goal area
set_option -netlist_hierarchy 0
set_option -map_option 2
set_option -max_fanout 2000
set_option -bit_security 0
set_option -timing_driven 1
set_option -place_option 1
set_option -route_option 0
set_option -correct_hold_violation 1
run all
run close
