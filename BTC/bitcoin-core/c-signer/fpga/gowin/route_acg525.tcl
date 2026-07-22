set project_dir [file normalize [file dirname [info script]]]
cd $project_dir
open_project [file join $project_dir btc_signer_acg525.gprj]
set_option -bit_security 0
set_option -timing_driven 1
set_option -place_option 1
set_option -route_option 0
set_option -correct_hold_violation 1
run pnr
run close
