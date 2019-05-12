#!/bin/bash


TERM="xterm"

#launches sc
$TERM -hold -fg Aqua -title "ctl" -geometry "80x20+100+400" -e "./topology-load ctl" &


#Launches ping service
$TERM -hold -fg DeepPink2 -title "Ping" -geometry "80x20+100+100" -e "./topology-load ping" &

#launches pong service
$TERM -hold -fg Gold1 -title "Pong" -geometry "80x20+600+100" -e "./topology-load pong" &
