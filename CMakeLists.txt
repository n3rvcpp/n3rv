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

find_library (PTHREAD pthread)
find_library (ZMQ zmq)
