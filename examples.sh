#!/bin/bash

TERM="xterm"

CMD_DIR="build_examples/"

if [ $# -lt 2 ]; then
    echo "Usage: examples.sh (run <example_name>|stop)"
fi

if [ $1 = "run" ]; then

    if [ $2 = "hello" ]; then
         ${CMD_DIR}./hello
    fi

    if [ $2 = "httpserv" ]; then
         ${CMD_DIR}./httpserv
    fi

    if [ $2 = "algotrading" ]; then
        #Launches servicecontroller on port 10001
        $TERM -hold -fg Aqua -title "Broker" -geometry "80x20+100+100" -e "${CMD_DIR}./algotrading_broker" &

        #launches worker service 1
        $TERM -hold -fg Gold1 -title "Eval" -geometry "80x20+600+100" -e "${CMD_DIR}./algotrading_eval" &

        #launches worker service 2
        $TERM -hold -fg DeepPink2 -title "Exec" -geometry "80x20+600+400" -e ".${CMD_DIR}./algotrading_exec" &

        #launches ventiler service 1
        $TERM -hold -fg GreenYellow -title "Money Manager" -geometry "80x20+100+400" -e "${CMD_DIR}./algotrading_moneyman" &
    fi

    if [ $2 = "topology-fetch" ]; then
        #launches sc
        $TERM -hold -title "ctl" -fg Aqua -geometry "80x20+100+400" -e "${CMD_DIR}./topology-fetch ctl" &

        #Launches ping service
        $TERM -hold -title "Ping" -fg DeepPink2 -geometry "80x20+100+100" -e "${CMD_DIR}./topology-fetch ping" &

        #launches pong service
        $TERM -hold -title "Pong" -fg Gold1 -geometry "80x20+600+100" -e "${CMD_DIR}./topology-fetch pong" &
    fi

    if [ $2 = "topology-load" ]; then
        #launches sc
        $TERM -hold -fg Aqua -title "ctl" -geometry "80x20+100+400" -e "${CMD_DIR}./topology-load ctl" &

        #Launches ping service
        $TERM -hold -fg DeepPink2 -title "Ping" -geometry "80x20+100+100" -e "${CMD_DIR}./topology-load ping" &

        #launches pong service
        $TERM -hold -fg Gold1 -title "Pong" -geometry "80x20+600+100" -e "${CMD_DIR}./topology-load pong" &
    fi

    if [ $2 = "workers-single-factorial" ] ;then
        #Launches servicecontroller on port 10001
        $TERM -hold -fg Aqua -title "Service Controller" -geometry "80x20+100+100" -e "${CMD_DIR}./workers-single-factorial -t ctl -l 10001" &

        #launches worker service 1
        $TERM -hold -fg DeepPink2 -title "Worker 1" -geometry "80x20+600+100" -e "${CMD_DIR}./workers-single-factorial -n worker1 -s worker -c 127.0.0.1 -p 10001" &

        #launches worker service 2
        $TERM -hold -fg Gold1 -title "Worker 2" -geometry "80x20+1100+100" -e "${CMD_DIR}./workers-single-factorial -n worker2 -s worker -c 127.0.0.1 -p 10001" &

        #launches ventiler service 1
        $TERM -hold -fg GreenYellow -title "Ventiler 1" -geometry "80x20+100+400" -e "${CMD_DIR}./workers-single-factorial -n ventiler1 -s vent -c 127.0.0.1 -p 10001" &
    fi

    if [ $2 = "workers-single-factorial-hctl" ] ;then
        #launches ventiler service 1
        $TERM -hold -fg Aqua -title "Ventiler 1" -geometry "80x20+100+100" -e "${CMD_DIR}./workers-single-factorial-hctl -n ventiler1 -s vent -c 127.0.0.1 -p 10001 -l 11003" &

        #launches worker service 1
        $TERM -hold -fg DeepPink2 -title "Worker 1" -geometry "80x20+100+400" -e "${CMD_DIR}./workers-single-factorial-hctl -n worker1 -s worker -c 127.0.0.1 -p 10001 -l 11001" &

        #launches worker service 2
        $TERM -hold -fg Gold1 -title "Worker 2" -geometry "80x20+600+100" -e "${CMD_DIR}./workers-single-factorial-hctl -n worker2 -s worker -c 127.0.0.1 -p 10001 -l 11002" &
    fi

fi

if [ $1 = "stop" ] ;then
  pkill -9 xterm
fi