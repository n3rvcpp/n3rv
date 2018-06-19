cmake_minimum_required(VERSION 2.6)

project(n3rv)

SET (CMAKE_CXX_COMPILER     "/usr/bin/g++")
SET (CMAKE_C_COMPILER       "/usr/bib/gcc")
SET (CMAKE_CXX_FLAGS        "-g -std=c++1y -fPIC")
SET (CMAKE_LINKER_FLAGS    "-rdynamic")

add_definitions(-DQATE_PREFIX=\"${CMAKE_INSTALL_PREFIX}\")


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

add_executable(
	topology-fetch/topology-fetch
	topology-fetch/topology-fetch.cpp
)


add_executable(
	http-gateway/http-gateway
	http-gateway/http-gateway.cpp
)


target_link_libraries(
  hello/hello
  n3rv
  zmq
  pthread
)

target_link_libraries(
  algotrading/broker
  n3rv
  zmq
  pthread
)

target_link_libraries(
  algotrading/eval
  n3rv
  zmq
  pthread
)


target_link_libraries(
  algotrading/exec
  n3rv
  zmq
  pthread
)

target_link_libraries(
  algotrading/moneyman
  n3rv
  zmq
  pthread
)


target_link_libraries(
  workers-single-factorial/workers-single-factorial
  n3rv
  zmq
  pthread
)

target_link_libraries(
  workers-single-factorial-hctl/workers-single-factorial-hctl
  n3rv
  zmq
  pthread
)


target_link_libraries(
  topology-load/topology-load
  n3rv
  zmq
  pthread
)

target_link_libraries(
  topology-fetch/topology-fetch
  n3rv
  zmq
  pthread
)

target_link_libraries(
  http-gateway/http-gateway
  n3rv
  n3rvhttp
  zmq
  pthread
)



find_library (N3RV n3rv)
find_library (N3RVHTTP n3rvhttp)
find_library (ZMQ zmq)
find_library (PTHREAD pthread)
