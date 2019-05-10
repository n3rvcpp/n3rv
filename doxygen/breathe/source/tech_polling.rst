Polling System
==============

As you may have noticed, the `n3rv::service` superclass does not have a `recv()` method, 
and this is perfectly normal. n3rv data receiving mechanism is event-driven: When running,
n3rv services use ZeroMQ's polling mechanism to detect if data is available on a given socket.
If this is effectively the case, the message is dequeued and then passed to a user-defined 
receive callback.

| Below is a simplified version of a service running loop:
.. code-block:: c++

  /** Main service loop, polls open sockets and forwards 
   * the data to the correct callback. */
   while(this->running) {
      
      zmq::poll (items, timeout);

      for (;;items_size) {
         
         if (item.revents & ZMQ_POLLIN) {
       
            /* If ZMQ polling found data ready, we call recv() on socket */
            socket->recv(&message);
            /* We call the receive callback for message */
            (*callback(this,&message));           
        }
      }
   }

Receive Callbacks and attach()
------------------------------

The mapping between a qhandler* n3rv connection/binding (& by extension ZMQ Socket)
and its receive callback is made with the help of the `n3rv::service::attach()` method.
With this mechanism, you tell your service class which funtion to call whenever a given socket
receives data.

The signature for a receive callback MUST be the following:

.. code-block:: c++

  static void* receive_callback (void* objref, zmq::message_t* msg);

Also once you have declared your callback, using `attach()` to bind it with a qhandler* object is 
easy:

.. code-block:: c++
 
  class example: public n3rv::service {
      using n3rv::service::service;

      public:

        /* We declare a new receive callback .*/
        static void* mycb(void* objref, zmq::message_t* msg) {}

        int initialize() {

          /*We bind a new socket (for the sake of example)*/
          qhandler *q = this->bind("0.0.0.0",ZMQ_REP);

          /* We use attach() to bind socket to "mycb" */
          this->attach(q,example::mycb);

        }
  }

More on Receive Callbacks
----------------------------

As you might have noticed, receive callbacks have a strange signature:

.. code-block:: c++

  static void* receive_callback (void* objref, zmq::message_t* msg);

Now i will explain why:

static ?
********

C++ is unable to produce pointers on non-static methods, this is why whenever you want to
use callbacks in C++, you have to declare them static. This has of course a large drawback:
you're losing the caller reference ! But there's a trick:  

objref
******

The trick consists in passing the caller reference as a void* pointer, then cast it back to
the caller type inside the callback. This is what `void* objref` is for.
In your callback, to get back acces to your object, you will want to make:

.. code-block:: c++

  static void* receive_callback (void* objref, zmq::message_t* msg) {

    /*we cast back objref to its real type. Now caller ref is available through self */
    example* self = static_cast<example*>(objref);

  }

msg
***

When data is received by a ZMQ socket, Receive callbacks have direct acces the raw zmq
data instead of just `n3rv::message` objects. This is a deliberate choice, allowing to
be more flexible. Nevertheless if you want to retrieve n3rv message inside your receive callbacks,
you just have to call `n3rv::parse_msg()`:

.. code-block:: c++

  static void* receive_callback (void* objref, zmq::message_t* msg) {

    /*we call parse_msg to get message from zmq raw data */
    n3rv::message m = n3rv::parse_msg(msg);

    /* Now you can process the message and act accordingly */
    if (m.action == "RPC_CALL") {
      /* ... */
    }

  }


Polling Timeout
---------------

When a node's main loop needs to poll multiple sockets at the same time, 
it will wait `n3rv::service::poll_timeout` milliseconds before continuing. 
poll_timeout is set to `1000ms` by default, which is quite large specially 
when you want to achieve high throughput for your services. But Of course you 
can lower this value by calling `service::set_timeout(int)` before running the service.






