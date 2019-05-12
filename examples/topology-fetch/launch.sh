#!/bin/bash


TERM="xterm"

#launches sc
$TERM -hold -title "ctl" -fg Aqua -geometry "80x20+100+400" -e "./topology-fetch ctl" &


#Launches ping service
$TERM -hold -title "Ping" -fg DeepPink2 -geometry "80x20+100+100" -e "./topology-fetch ping" &

#launches pong service
$TERM -hold -title "Pong" -fg Gold1 -geometry "80x20+600+100" -e "./topology-fetch pong" &
