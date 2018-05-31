cmake_minimum_required(VERSION 2.6)

project(tests)

SET (CMAKE_CXX_COMPILER     "/usr/bin/g++")
SET (CMAKE_C_COMPILER       "/usr/bib/gcc")
SET (CMAKE_CXX_FLAGS        "-g -std=c++1y -fPIC")
SET (CMAKE_LINKER_FLAGS    "-rdynamic")

add_definitions(-DQATE_PREFIX=\"${CMAKE_INSTALL_PREFIX}\")


add_executable(
	tests
    tests.cpp
)

target_link_libraries(
  tests
  n3rv
  zmq
  pthread
)

find_library (N3RV n3rv)
find_library (ZMQ zmq)
find_library (PTHREAD pthread)
