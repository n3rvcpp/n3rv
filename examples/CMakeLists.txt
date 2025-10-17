cmake_minimum_required(VERSION 3.5)

project(n3rv)
option(WITH_PROTOBUF "With Protobuf" OFF)

SET (CMAKE_CXX_EXTENSIONS OFF)
SET (CMAKE_CXX_STANDARD 23)

SET (CMAKE_CXX_COMPILER     "/usr/bin/g++")
SET (CMAKE_C_COMPILER       "/usr/bib/gcc")

add_definitions(-DQATE_PREFIX=\"${CMAKE_INSTALL_PREFIX}\")

if(WITH_PROTOBUF)
  find_library (PROTOBUF protobuf)
  set(LIBS n3rv zmq pthread protobuf)
else()
  set(LIBS n3rv zmq pthread)
endif(WITH_PROTOBUF)


add_executable(
	hello
  	hello/hello.cpp
)

add_executable(
	workers-single-factorial
	workers-single-factorial/src/workers-single-factorial.cpp
)

add_executable(
	workers-single-factorial-hctl
	workers-single-factorial-hctl/src/workers-single-factorial-hctl.cpp
)

add_executable(
	algotrading_broker
	algotrading/broker.cpp
)

add_executable(
	algotrading_eval
	algotrading/eval.cpp
)

add_executable(
	algotrading_exec
	algotrading/exec.cpp
)

add_executable(
	algotrading_moneyman
	algotrading/moneyman.cpp
)

add_executable(
	topology-load
	topology-load/topology-load.cpp
)

add_executable(
	topology-fetch
	topology-fetch/topology-fetch.cpp
)

add_executable(
	httpserv
	httpserv/httpserv.cpp
)


target_link_libraries(hello ${LIBS})
target_link_libraries(algotrading_broker ${LIBS})
target_link_libraries(algotrading_eval ${LIBS})
target_link_libraries(algotrading_exec ${LIBS})
target_link_libraries(algotrading_moneyman ${LIBS})
target_link_libraries(workers-single-factorial ${LIBS})
target_link_libraries(workers-single-factorial-hctl ${LIBS})
target_link_libraries(topology-load ${LIBS})
target_link_libraries(topology-fetch ${LIBS})
target_link_libraries(httpserv ${LIBS} event)

find_library (N3RV n3rv)
find_library (ZMQ zmq)
find_library (PTHREAD pthread)
find_library (EVENT event)



