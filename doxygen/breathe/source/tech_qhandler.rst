Qhandlers
=========

`n3rv::qhandler` objects are producted whenever a service creates a new binding or `connect()` to one.
Concretely, it is an overlying layer which "hides" ZMQ sockets under the hood.

The qhandler object structure is really simple:

.. code-block:: c++

  /** Connction handler returned by connect() 
   * or bind(), used for send() and attach() operations. */

  typedef struct qhandler_ {

        /** randomly generated connection id */
        std::string cid;

        /** full peer node uid after lookup (filled only for connect() operations ) */
        std::string peer_uid;
    } qhandler;


qhandlers are the objects which make the link with ZMQ sockets in n3rv, and `service::send()` and 
`service::attach()` use them.

Getting ZMQ socket with a qhandler*
-----------------------------------

If for some obscur reason you need to get direct access to the raw ZMQ socket of a qhandler*, you can
use the `service::get_zsocket()` method:

.. code-block:: c++

   n3rv::service s1("127.0.0.1",10001);
   n3rv::qhandler* q = s1.connect("com.svc.node1.binding",ZMQ_REQ);

   /* We retrieve the raw ZMQ socket with of q */
   zmq::socket_t* sock = s1.get_zsocket(q);

Qhandlers and ZMQ Socket Options 
--------------------------------

Also If required, you can directly set ZMQ socket options with `service::zsockopt()` , 
which is the equivalent of `zmq::socket_t::setsockopt(ZMQ_OPTION,ZMQ_VALUE)` except
that it takes a qhandler* pointer instead:

.. code-block:: c++

   n3rv::service s1("127.0.0.1",10001);
   n3rv::qhandler* q = s1.connect("com.svc.node1.binding",ZMQ_REQ);

   /* We set a ZMQ option for socket linked to q */
   s1.zsockopt(q,ZMQ_LINGER,0);
