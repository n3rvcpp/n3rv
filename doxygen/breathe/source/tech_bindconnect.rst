bind & connect
==============

`service::bind()` and `service::connect()` are the base methods to establish connections
between your nodes. These methods are directly linked to `zmq::socket_t->bind()` and 
`zmq::socket_t->connect()` (with extra work).


service::bind()
---------------

As its name indicates, bind() allows to create ZMQ TCP bindings on the IP address, 
port and socket type given as arguments. 

Once the binding is created, the service will AUTOMATICALLY register it on the service 
controller to make it available to other nodes.  Finally the bind() method will return 
an `n3rv::qhandler*` object, to be later used with `service::attach()` and `service::send()`.

The method's signature is the following:

.. code-block:: c++

  qhandler* bind(const char* bind_name, const char* ip , int zmq_socket_type, int port = 0 );


The binding will be registred in the directory under the name "bind_name"

A kewl feature about `service::bind()` is that since the binding is directory-listed, you are not
required to provide a TCP port number ! If you don't provide one, the service will try to bind it on
a random, high-range port. 

.. code-block:: c++

  n3rv::service s1("127.0.01",10001);
  s1.set_uid("com.svc.node1");

  /* We create a first binding named "my_binding", of type REP on TCP port 12001 */
  qhandler* q = s1.bind("my_binding", "0.0.0.0",ZMQ_REP, 12001);

  /* we create a second one, of type PUB that will be bound on a random port */
  qhandler* q2= s1.bind("my_pub_binding", "0.0.0.0",ZMQ_PUB);

Important notes about `service::bind()`:
****************************************

 - A service cannot use bind() if its uid is not properly set with `service::set_uid()`, because if no uid is set, it cannot subscribe the binding.
 - In order to unbind TCP ports more quickly, ZMQ binds are set with the option ZMQ_LINGER at 0

service::connect()
------------------

`service::connect()` is the method used by your services to connect to other nodes.
Like bind(), connect will return a `n3rv::qhandler*` pointer to let you later interact 
with the connection.

Its signature is 

.. code-block:: c++

  qhandler* connect(const char* lookup, int zmq_socket_type, qhandler* hdlref = nullptr);

lookup
****** 

lookup declares the binding you want to connect to, usually 
in the `namespace.service_class.node.binding` format. To get more informations about 
lookup strings, i suggest that you have a look at `the resolution mechanism section <./tech_resolution.html#resolution-mechanism/>`_.

zmq_socket_type
***************

As its name suggests, it is the ZMQ socket type to create.

hdlref
******

Non mandatory parameter, it has 2 usages: 

- for deferred connections to pass back their initially created qhandler* when they try to reconnect.
- When you want to connect to multiple bindings with the same qhandler*/ZMQ socket.

.. code-block:: c++

  n3rv::service s1("127.0.01",10001);
  s1.set_uid("com.svc.node1");

  /* We try to connect to binding "com.svc.node2.binding1" with a REQ ZMQ socket */
  qhandler* q = s1.connect("com.svc.node2.binding1", ZMQ_REQ);


Important notes about `service::connect()`:
*******************************************

 - If the binding you want to connect to is not yet available in the directory, then the connection is placed inside a "deferred" queue, to retry to connect later.
 - when ZMQ socket type is ZMQ_SUB, connect() automatically sets the ZMQ_SUBSCRIBE socket option to 0


ZeroMQ Socket Types
-------------------

As you can see the `service::bind()` and `service::connect()` need a ZMQ socket type parameter
in order to work properly. You can find the full list of available ZMQ socket types at the 
follwing URL: http://api.zeromq.org/4-2:zmq-socket



