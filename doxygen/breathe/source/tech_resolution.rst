Resolution Mechanism
====================

A n3rv cluster can regroup dozens of nodes of different service classes, and hundreds 
of different bindings at the same time. Therefore, a convenient objects resolution mechanism was
added to the framework in order to always find the element you want to connect to.

General Principle
-----------------

Each time a node calls its own connect(const char* uid, int ZMQ_TYPE) method, it will parse the uid
string passed as argument. connect() expects uids in one of the following formats:

- namespace.service_class.node_name.binding_name
- service_class.node_name.binding_name
- node_name.binding_name
- binding_name (basically useless, means that the node wants to connect to itself)

.. code-block:: c++

  n3rv::service s1("127.0.0.1",10001);
  s1.set_uid("com.sc1.node1");
  
  /*Node will try to connect to the binding "binding1" of node "node2", 
    which is of service class sc2, on namespace "com" */
  n3rv::qhandler* h1 = s1.connect("com.sc2.node2.binding1", ZMQ_CLIENT);

Note: When parts of the uid are missing, the node will use its own namespace/service class/node_name 
values to perform the lookup, thus restraining the scope.


The node will then look at its internal nodes/bindings directory and try to find 
the peer ip and TCP port associated with the provided uid. If it finds a match, it automatically
tries to connect to it. If it doesn't, the connection is added to a "deferred" queue for later processing,
in case the resolution becomes ok later.


Wildcard (*) Resolutions
------------------------

Since a n3rv cluster can have hundreds of referenced bindings, a basic lookup mechanism 
would not be enough. This is why connect() also supports wildcards resolutions in its uid parameters.

.. code-block:: c++

  n3rv::service s1("127.0.0.1",10001);
  s1.set_uid("com.sc1.node1");

  /*Node will try to connect to the binding "binding1" 
  of any node having a service class sc2, on any namespace. */
  n3rv::qhandler* h1 = s1.connect("*.sc2.*.binding1", ZMQ_CLIENT);

Note: As you would expect, wildcard lookups can return multiple results. 
When it is the case, connect() picks one of the list randomly. For batch connections,
see below.

Wildcard resulutions + batch connections: connect_all() 
-------------------------------------------------------

If you want to use the wildcard bindings resolution mechanism in order to connect to all the 
matching bindings at once, i suggest that you use the service::connect_all() method.

.. code-block:: c++

  n3rv::service s1("127.0.0.1",10001);
  s1.set_uid("com.sc1.node1");

  /* will try to connect to binding "binding1" of all the nodes of service class sc2
  on namespace com */
  std::vector<n3rv::qhandler*> hlist = s1.connect_all("com.sc2.*.binding1", ZMQ_CLIENT);

WARNING: Yet being very powerfull, i strongly advise you to be very careful with connect_all().
ZeroMQ not being able to connect any socket type to any other, therefore you must be well 
aware which kind of socket types the lookup will return. Also please note that like connect(), 
connect_all() defers lookups for later try only once, therefore all the bindings you want to connect 
to MUST be subscribed at the same. 










