cmake_minimum_required(VERSION 2.6)

project(n3rv)

SET (CMAKE_CXX_COMPILER     "/usr/bin/g++")
SET (CMAKE_C_COMPILER       "/usr/bib/gcc")
SET (CMAKE_CXX_FLAGS        "-g -std=c++1y -fPIC")
SET (CMAKE_LINKER_FLAGS    "-rdynamic")

add_definitions(-DQATE_PREFIX=\"${CMAKE_INSTALL_PREFIX}\")

add_library(
	n3rv SHARED
	src/n3rvservice.cpp
  src/n3rvservicecontroller.cpp
  src/n3rvlogger.cpp
  src/n3rvproto.cpp
)


add_executable(
	examples/workers-single-factorial/workers-single-factorial
	examples/workers-single-factorial/src/workers-single-factorial.cpp
)


target_link_libraries(
   n3rv
   zmq
)

target_link_libraries(
  examples/workers-single-factorial/workers-single-factorial
  n3rv
)



find_library (PTHREAD pthread)
find_library (ZMQ zmq)
