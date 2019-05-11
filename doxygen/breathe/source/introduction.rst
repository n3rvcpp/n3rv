Introduction
============

What is n3rv ?
--------------

n3rv is a C++ library that leverages the power of ZeroMQ to create multiservices architectures.
Using ZMQ as core communication mechanism, n3rv basically provides all the components that 
modern services development requires, meaning:

 - A Services Directory & subscribing mechanism.
 - An events handler.
 - Messages encapsulation methods.
 - Logging.
 - An HTTP/ZeroMQ gateway.
 - Dynamic architecture reconfiguration.

Actually it is quite similar to `Facebook's fbzmq <https://github.com/facebook/fbzmq/>`_, 
but easier to use and with more features.

How does it Work ?
------------------

The n3rv library implements the `n3rv::service` superclass. 
