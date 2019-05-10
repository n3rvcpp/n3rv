.. n3rv SDK documentation master file, created by
   sphinx-quickstart on Thu Aug  6 02:11:10 2015.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

API Reference
=============

This section covers the full n3rv API documention. You will find below everything you need in order 
to get the best of the framework, and produce fast and reliable multiservices architectures.

.. toctree::
   :hidden:
   :includehidden:
   :titlesonly:

   api_service
   api_logger
   api_servicecontroller
   api_topology

Classes List
-------------

You will find here the exaustive list of the usable 
classes in the n3rv library. 

Service Class
^^^^^^^^^^^^^^

The main superclass class of the project, every user defined service class inherit from this one.
:doc:`See Class Documentation <api_service>`

Logger Class
^^^^^^^^^^^^

Provides timestamped logging to your service, which output can be syslog, files and/or stdout.
:doc:`See Class Documentation <api_logger>`

Service Controller Class
^^^^^^^^^^^^^^^^^^^^^^^^

This class allows to instanciate a service controller, which plays 
the role of endpoints directory.
:doc:`See Class Documentation <api_servicecontroller>`

HTTP Service Class
^^^^^^^^^^^^^^^^^^

n3rv::httpservice is an extension of the n3rv::service superclass which provides additionnal http 
server capacities with libevent
:doc:`See Class Documentation <api_httpservice>`

Topology Class
^^^^^^^^^^^^^^

This class allows to load and parse topologies, which allows to automatically 
bind/connect to service endpoints.
:doc:`See Class Documentation <api_topology>`
