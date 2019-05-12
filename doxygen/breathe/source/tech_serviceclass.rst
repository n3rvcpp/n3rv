The service Class
=================

The `n3rv::service` class is the backbone of the n3rv framework. The service class mainly handles:

 - The polling of you open sockets.
 - Your socket events.
 - Communications with the service controller, for bindings subsciptions and directory updates.
 - The lookup of the other nodes it must connect to.


How to define a custom service class
------------------------------------

All your defined service classes must inherit from `n3rv::service` (or `n3rv::httpservice`) to 
work properly. Note that you imperatively must import the `n3rv::service` constructor with 
`using n3rv::service::service;` directive.

.. code-block:: c++

  #include <n3rv/n3rvcommon.hpp>
  #include <n3rv/n3rvservice.hpp>

  class example_service: public n3rv::service {
      using n3rv::service::service;
      public:

        int initalize() {
            /* Your service class init code here */
        }

  };


Instanciate a service class
---------------------------

The `n3rv::service` constructor signature is pretty simple:

.. code-block:: c++

  service(const char* controller_host, int controller_port, logger* ll = nullptr);

As you can see the creation of a service instance requires the service controller's IP and port
as parameters. The logger is not mandatory (see `Logging <./tech_logging.html>`_ for more details)

For our previously defined custom service class, that may give:

.. code-block:: c++

  /* Our service controller listens on 127.0.0.1:10001 */
  example_service* es = new example_service("127.0.0.1",10001);


UIDs
----

A very important aspect about service instances is that they must have a unique identifier
on the cluster. For this purpose, you must use the `service::set_uid()` method. Node UIDs
must be composed of 3 elements: the node's namespace, the node's service class, and the node's name.

.. code-block:: c++

  /* We tell our service instance that it is on namespace "com", 
     it has "example_service" service class and a name "node1" */
  es->set_uid("com.example_service.node1");


Running your service classes
----------------------------

In order to poll its sockets and receive directory updates, a service instance must be in 
running state. To pass a service instance in running state, one must use the `service::run()` (blocking) or 
`service::run_async` (non-blocking) methods

.. code-block:: c++

  /* We start our service instance */
  es->run_async();

Additionnal informations
------------------------

Feel free to explore the rest of this documentation to get more clues about how the service class
works. You can also go to the `n3rv::service class API reference <./api_service.html#n3rv-service>`_ for full infos.













