#!/bin/bash


TERM="xterm"

#launches sc
$TERM -hold -title "ctl" -geometry "80x20+100+400" -e "./topology-fetch ctl" &


#Launches ping service
$TERM -hold -title "Ping" -geometry "80x20+100+100" -e "./topology-fetch ping" &

#launches pong service
$TERM -hold -title "Pong" -geometry "80x20+600+100" -e "./topology-fetch pong" &
