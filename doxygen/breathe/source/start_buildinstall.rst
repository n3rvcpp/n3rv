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


