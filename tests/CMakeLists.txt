cmake_minimum_required(VERSION 2.6)

project(tests)

SET (CMAKE_CXX_COMPILER     "/usr/bin/g++")
SET (CMAKE_C_COMPILER       "/usr/bib/gcc")
SET (CMAKE_CXX_FLAGS        "-g -std=c++1y -fPIC -fprofile-arcs -ftest-coverage")
SET (CMAKE_LINKER_FLAGS    "-rdynamic")

add_definitions(-DQATE_PREFIX=\"${CMAKE_INSTALL_PREFIX}\")


add_executable(
	tests
    tests.cpp
)

target_link_libraries(
  tests
  n3rv
  n3rvhttp
  zmq
  pthread
  event
)

find_library (N3RV n3rv)
find_library (N3RVHTTP n3rvhttp)
find_library (ZMQ zmq)
find_library (PTHREAD pthread)
find_library (EVENT event)
