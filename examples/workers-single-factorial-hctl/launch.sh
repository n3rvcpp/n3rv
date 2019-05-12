#!/bin/bash


TERM="xterm"

#launches ventiler service 1
$TERM -hold -fg Aqua -title "Ventiler 1" -geometry "80x20+100+100" -e "./workers-single-factorial-hctl -n ventiler1 -s vent -c 127.0.0.1 -p 10001 -l 11003" &

#launches worker service 1
$TERM -hold -fg DeepPink2 -title "Worker 1" -geometry "80x20+100+400" -e "./workers-single-factorial-hctl -n worker1 -s worker -c 127.0.0.1 -p 10001 -l 11001" &

#launches worker service 2
$TERM -hold -fg Gold1 -title "Worker 2" -geometry "80x20+600+100" -e "./workers-single-factorial-hctl -n worker2 -s worker -c 127.0.0.1 -p 10001 -l 11002" &

