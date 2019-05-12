#!/bin/bash


TERM="xterm"

#Launches servicecontroller on port 10001
$TERM -hold -fg Aqua -title "Broker" -geometry "80x20+100+100" -e "./broker" &

#launches worker service 1
$TERM -hold -fg Gold1 -title "Eval" -geometry "80x20+600+100" -e "./eval" &

#launches worker service 2
$TERM -hold -fg DeepPink2 -title "Exec" -geometry "80x20+600+400" -e "./exec" &

#launches ventiler service 1
$TERM -hold -fg GreenYellow -title "Money Manager" -geometry "80x20+100+400" -e "./moneyman" &
