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


Building Examples
-----------------


Building Tests
--------------


