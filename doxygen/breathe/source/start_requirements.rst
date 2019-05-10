Requirements
============

What do i need to use n3rv ?
----------------------------

Here you will find all the required elements that you need to get started with the n3rv framework in good conditions.

Prior knowledge
***************

n3rv was primarly designed to make the programmer's life easy, but there is still a minimum
skillset to have in order to use the library. Concretely you will need:

- C++ coding skills.
- Basic knowledge of a C++ toolchain in order to compile your code.
- Prior knowledge of ZeroMQ is not absolutely necessary but still really helpful.
- Notions of programming in multithreaded environments are also of great help.

| You can find the full ZeroMQ documentation here: http://zguide.zeromq.org/page:all
| A complete video tutorial on C++ concurency programming is also available here: https://www.youtube.com/playlist?list=PL1835A90FC78FF8BE


OS and tools
************

n3rv is mainly designed to work on Unix systems, although the windows-native porting effort shouldn't be that hard. 
Nevertheless, if you plan to use n3rv on Windows environments, i highly recommand that you use 
a cygwin environment to build the n3rv library.
 
| Note that you can setup a full cygwin environment on windows using https://cygwin.com/install.html

So in definitive and in order to build and use n3rv, you will need:

- A Linux/*BSD System or Cygwin env
- git 
- A C++ compilation toolchain (most likely g++)
- The cmake build tool
- The protocol buffer compiler v2 (optional)


library dependancies
********************

The n3rv framework mainly depends on libzmq-dev to work, though it may have extra dependancies if
you plan to use them. The list is the following:

- libzmq 4.2+
- libevent 2.1+ (Optional, if using n3rv::httpservice)
- protobuf v2+ (Optional, if using protobuf serialization)

Important Note: n3rv also needs cppzmq (/usr/include/zmq.hpp). On Debian-like systems, 
libzmq-dev directly embedds cppzmq but that may not be the case for your system. In this situation, 
run the following command as sudo/root:

.. code-block:: console

  sudo wget -qO/usr/include/zmq.hpp https://raw.githubusercontent.com/zeromq/cppzmq/master/zmq.hpp

