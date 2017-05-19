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
)


add_executable(
	examples/workers-single-binary/workers-single
	examples/workers-single-binary/src/workers-single.cpp
)


target_link_libraries(
   n3rv
   zmq
)

target_link_libraries(
  examples/workers-single-binary/workers-single
  n3rv
)



find_library (PTHREAD pthread)
find_library (ZMQ zmq)
