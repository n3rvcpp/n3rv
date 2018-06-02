cmake_minimum_required(VERSION 2.6)

project(n3rv)

SET (CMAKE_CXX_COMPILER     "/usr/bin/g++")
SET (CMAKE_C_COMPILER       "/usr/bib/gcc")
SET (CMAKE_CXX_FLAGS        "-g -std=c++1y -fPIC")
SET (CMAKE_LINKER_FLAGS    "-rdynamic")

add_library(
	n3rv SHARED
	src/n3rvservice.cpp
  src/n3rvservicecontroller.cpp
  src/n3rvlogger.cpp
  src/n3rvproto.cpp
  src/n3rvtopology.cpp
)

add_library(
  n3rvhttp
  src/n3rvhttpservice.cpp
)

target_link_libraries(
   n3rvhttp
   n3rv
   zmq
   pthread
)


target_link_libraries(
   n3rv
   zmq
   pthread
)

find_library (PTHREAD pthread)
find_library (ZMQ zmq)


install (TARGETS n3rv DESTINATION /usr/local/lib)
install (TARGETS n3rvhttp DESTINATION /usr/local/lib)
install (FILES src/n3rvcommon.hpp DESTINATION /usr/local/include/n3rv)
install (FILES src/n3rvservice.hpp DESTINATION /usr/local/include/n3rv)
install (FILES src/n3rvhttpservice.hpp DESTINATION /usr/local/include/n3rv)
install (FILES src/n3rvlogger.hpp DESTINATION /usr/local/include/n3rv)
install (FILES src/n3rvproto.hpp DESTINATION /usr/local/include/n3rv)
install (FILES src/n3rvzmq.hpp DESTINATION /usr/local/include/n3rv)
install (FILES src/n3rvservicecontroller.hpp DESTINATION /usr/local/include/n3rv)
install (FILES src/n3rvtopology.hpp DESTINATION /usr/local/include/n3rv)