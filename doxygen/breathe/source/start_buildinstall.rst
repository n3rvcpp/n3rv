Build & Install n3rv
====================

Now that we know exactly what kind of OS, tools and libs we need need to build n3rv, 
let's get to the real thing. Assuming that all the previously listed requirements are met,
building n3rv is quite straightforward.


How to build n3rv
-----------------

First we clone the n3rv repository on github:

.. code-block:: console

 git clone https://github.com/n3rvcpp/n3rv


Then we construct the MakeFile:

.. code-block:: console

    cd n3rv
    cmake .

And to finish we build & install the library:

.. code-block:: console

 make && sudo make install


Protobuf Support
----------------

Assuming that you correctly installed the protobuf lib on your system 
and that you want n3rv to use it as main serialization method, here is what to do:

First, run cmake with -DWITH_PROTOBUF=yes

.. code-block:: console

    cd n3rv
    cmake -DWITH_PROTOBUF=yes .

Next, you will need to compile the n3rvproto.proto file located in src/protocol/protobuf:

.. code-block:: console

    pushd .
    cd src/protocol/protobuf
    protoc n3rvproto.proto --cpp_out=.
    poopd

Finally, build & install as usual:

.. code-block:: console

 make && sudo make install


Building Examples
-----------------

As soon as you've succesfully built and installed the n3rv library, you can build all the examples
that are located in n3rv/examples:

.. code-block:: console

 cd examples
 cmake . && make

Building Tests
--------------

In order to check that the library was correctly compiled for your system, you can also
build and run the unit tests:

.. code-block:: console

 cd tests
 cmake . && make && ./tests

Troubleshooting Build errors
----------------------------

If you encounter troubles building the n3rv library, i suggest that you have a look at the 
following section:

Missing Header zmq.hpp
**********************

As said in the requirements, n3rv requires the cppzmq header. Some distributions provide this heaver
along with the libzmq-devel package but some don't. If you don't have this header on your system,
you can add it by running the following command:

.. code-block:: console

  sudo wget -qO/usr/include/zmq.hpp https://raw.githubusercontent.com/zeromq/cppzmq/v<ZMQ_VERSION>/zmq.hpp


Undefined reference to getnameinfo / NI_MAXHOST / NI_NUMERICSERV
****************************************************************

On some systems, the g++ std flag present in the Makefile can do some mess. If you have this error,
here is what you can do:

- Edit n3rv/CMakeList.txt and replace the line `SET (CMAKE_CXX_FLAGS  "-g -std=c++1y -fPIC")` by `SET (CMAKE_CXX_FLAGS  "-g -std=gnu++1y -fPIC")`
- Run cmake again (cmake .)


Undefined reference to ev_http_*
********************************

This error may appear because your version of libevent is too old, 
you must use at least libevent v2.1. Either update your package if possible, 
or build libevent from sources.



