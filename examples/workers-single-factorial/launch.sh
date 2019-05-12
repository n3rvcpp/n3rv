#!/bin/bash


TERM="xterm"

#Launches servicecontroller on port 10001
$TERM -hold -fg Aqua -title "Service Controller" -geometry "80x20+100+100" -e "./workers-single-factorial -t ctl -l 10001" &

#launches worker service 1
$TERM -hold -fg DeepPink2 -title "Worker 1" -geometry "80x20+600+100" -e "./workers-single-factorial -n worker1 -s worker -c 127.0.0.1 -p 10001" &

#launches worker service 2
$TERM -hold -fg Gold1 -title "Worker 2" -geometry "80x20+1100+100" -e "./workers-single-factorial -n worker2 -s worker -c 127.0.0.1 -p 10001" &

#launches ventiler service 1
$TERM -hold -fg GreenYellow -title "Ventiler 1" -geometry "80x20+100+400" -e "./workers-single-factorial -n ventiler1 -s vent -c 127.0.0.1 -p 10001" &