cmake_minimum_required(VERSION 3.5)

project(tests)

SET (CMAKE_CXX_EXTENSIONS OFF)
SET (CMAKE_CXX_STANDARD 23)

SET (CMAKE_CXX_COMPILER     "/usr/bin/g++")
SET (CMAKE_C_COMPILER       "/usr/bib/gcc")

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
  event
)

find_library (N3RV n3rv)
find_library (ZMQ zmq)
find_library (PTHREAD pthread)
find_library (EVENT event)
