cmake_minimum_required(VERSION 2.6)

project(n3rv)
option(WITH_PROTOBUF "With Protobuf" OFF)

SET (CMAKE_CXX_COMPILER     "/usr/bin/g++")
SET (CMAKE_C_COMPILER       "/usr/bib/gcc")
SET (CMAKE_CXX_FLAGS        "-g -std=c++1y -fPIC")
SET (CMAKE_LINKER_FLAGS    "-rdynamic")

add_definitions(-DQATE_PREFIX=\"${CMAKE_INSTALL_PREFIX}\")

if(WITH_PROTOBUF)
  find_library (PROTOBUF protobuf)
  set(LIBS n3rv zmq pthread protobuf)
else()
  set(LIBS n3rv zmq pthread)
endif(WITH_PROTOBUF)


add_executable(
	hello/hello
  hello/hello.cpp
)

add_executable(
	workers-single-factorial/workers-single-factorial
	workers-single-factorial/src/workers-single-factorial.cpp
)

add_executable(
	workers-single-factorial-hctl/workers-single-factorial-hctl
	workers-single-factorial-hctl/src/workers-single-factorial-hctl.cpp
)

add_executable(
	algotrading/broker
	algotrading/broker.cpp
)

add_executable(
	algotrading/eval
	algotrading/eval.cpp
)

add_executable(
	algotrading/exec
	algotrading/exec.cpp
)

add_executable(
	algotrading/moneyman
	algotrading/moneyman.cpp
)

add_executable(
	topology-load/topology-load
	topology-load/topology-load.cpp
)
#
add_executable(
	topology-fetch/topology-fetch
	topology-fetch/topology-fetch.cpp
)

target_link_libraries(hello/hello ${LIBS})
target_link_libraries(algotrading/broker ${LIBS})
target_link_libraries(algotrading/eval ${LIBS})
target_link_libraries(algotrading/exec ${LIBS})
target_link_libraries(algotrading/moneyman ${LIBS})
target_link_libraries(workers-single-factorial/workers-single-factorial ${LIBS})
target_link_libraries(workers-single-factorial-hctl/workers-single-factorial-hctl ${LIBS})
target_link_libraries(topology-load/topology-load ${LIBS})
target_link_libraries(topology-fetch/topology-fetch ${LIBS})

find_library (N3RV n3rv)
find_library (ZMQ zmq)
find_library (PTHREAD pthread)

