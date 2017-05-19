#!/bin/bash


TERM="xterm"

#Launches servicecontroller on port 10001
$TERM -title "Service Controller" -geometry "80x40+100+100" -e "./workers-single -t ctl -l 10001" &

#launches worker service 1
$TERM -title "Worker 1" -geometry "80x40+600+100" -e "./workers-single -t worker -c 127.0.0.1:10001 -l 10002" &

#launches worker service 2
$TERM -title "Worker 2" -geometry "80x40+1100+100" -e "./workers-single -t worker -c 127.0.0.1:10001 -l 10003" &
