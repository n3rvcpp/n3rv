Ubuntu Install
==============

How to setup n3rv on a fresh Ubuntu/Debian Box
----------------------------------------------

The follwing tutorial will explain how to get started with n3rv, having a freshly installed
Debian or ubuntu system.

1. Install the toolchain and library dependancies
-------------------------------------------------

Our first step will be to install the required tools and dependancies to build n3rv

.. code-block:: console

  sudo apt-get install build-essential cmake git libzmq5-dev libevent-dev


2. Clone the repository
-----------------------

Then we will fetch the n3rv source code from Github:

.. code-block:: console

  git clone https://github.com/n3rvcpp/n3rv


3. Build the library
--------------------

.. code-block:: console

  cd n3rv
  cmake .
  make 
  
4. Install
----------

If the build phase was succesfull, you can now install n3rv:

.. code-block:: console
  
  sudo make install

The libs are installed by default in `/usr/local/lib` (libn3rv.so and libn3rvhttp.so)
The include files are in /usr/local/include/n3rv

That is all, you're ready to go with n3rv !


