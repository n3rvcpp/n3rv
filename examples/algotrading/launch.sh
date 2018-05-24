#!/bin/bash


TERM="xterm"

#Launches servicecontroller on port 10001
$TERM -hold -title "Broker" -geometry "80x20+100+100" -e "./broker" &

#launches worker service 1
$TERM -hold -title "Eval" -geometry "80x20+600+100" -e "./eval" &

#launches worker service 2
$TERM -hold -title "Exec" -geometry "80x20+1100+100" -e "./exec" &

#launches ventiler service 1
$TERM -hold -title "Money Manager" -geometry "80x20+100+400" -e "./moneyman" &
