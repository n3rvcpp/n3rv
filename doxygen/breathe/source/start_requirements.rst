Requirements
============

What do i need to use n3rv ?
----------------------------

Here you will find all the required elements that you need to get started with the n3rv framework in good conditions.

Prior knowledge
***************

n3rv was primarly designed to make the programmer's life easy, but there is still a minimum
skillset to have in order to use the library. Concretely you will need:

- Basic C++ coding skills.
- Basic knowledge of a C++ toolchain in order to compile your code.
- Prior knowledge of ZeroMQ is not absolutely necessary but still really helpful.
- Notions of programming in multithreaded environments are also of great help.

| You can find the full ZeroMQ documentation here: http://zguide.zeromq.org/page:all
| A complete video tutorial on C++ concurency programming is also available here: https://www.youtube.com/playlist?list=PL1835A90FC78FF8BE


OS and tooling
**************

n3rv is mainly designed to work on Unix systems, although the windows-native porting effort shouldn't be that hard. 
Nevertheless, if you plan to use n3rv on Windows environments, i highly recommand that you use 
a cygwin environment to build the n3rv library.
|
|Note that you can setup a full cygwin environment on windows using the resources located here: https://cygwin.com/install.html
|
|So in definitive and in order to build and use n3rv, you will need:

- A Linux/*BSD System or Cygwin env
- git 
- A C++ toolchain (most likely g++ or llvm-clang)
- The cmake build tool
- The protocol buffer compiler v2 (optional)


library dependancies
********************

- libzmq 4.2+
- pthread
- libevent (Optional, if using n3rv::httpservice)
- protobuf v2+ (Optional, if using protobuf serialization)