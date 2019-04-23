cmake_minimum_required(VERSION 2.6)

project(n3rv)

option(WITH_PROTOBUF "With Protobuf" OFF)

SET (CMAKE_CXX_COMPILER     "/usr/bin/g++")
SET (CMAKE_C_COMPILER       "/usr/bib/gcc")
SET (CMAKE_CXX_FLAGS        "-g -std=c++1y -fPIC")
SET (CMAKE_LINKER_FLAGS    "-rdynamic")

### PB BUILD ###
if(WITH_PROTOBUF)

add_library(
	n3rv SHARED
  src/n3rvcommon.cpp
	src/n3rvservice.cpp
  src/n3rvservicecontroller.cpp
  src/n3rvlogger.cpp
  src/protocol/protobuf/protocol.cpp
  src/protocol/protobuf/n3rvproto.pb.cc
  src/n3rvtopology.cpp
)
find_library (PROTOBUF protobuf)
set(LIBS zmq pthread protobuf)

### JSON BUILD ###
else()

add_library(
	n3rv SHARED
	src/n3rvservice.cpp
  src/n3rvservicecontroller.cpp
  src/n3rvlogger.cpp
  src/protocol/json/protocol.cpp
  src/n3rvtopology.cpp
)
set(LIBS zmq pthread)
endif(WITH_PROTOBUF)


add_library(
  n3rvhttp SHARED
  src/n3rvhttpservice.cpp
)


target_link_libraries(
  n3rv
  ${LIBS}
)

target_link_libraries(
   n3rvhttp
   n3rv
   ${LIBS}
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