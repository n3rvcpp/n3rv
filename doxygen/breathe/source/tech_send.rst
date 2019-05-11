Sending Messages
================

Of course the main purpose of n3rv is to make your nodes to exchange messages with each other,
and this is what we are going to see in the following section.

Once you have opened a new connection or created a new binding there are multiple ways to make messages
pass through it, although they all use the `service::send()` method or one of its overloadings.

service::send()
---------------

`service::send()` has many signatures, which are basically:

.. code-block:: c++

  int send(qhandler* hdl, message& msg, int flags); 
  int send(qhandler* hdl, void* data, size_t size, int flags);
  int send(qhandler* hdl, zmq::message_t* zmsg, int flags);


Send a n3rv::message
--------------------

The point here is to declare a new `n3rv::message object`, fill it with your data and send it over 
the wire. At the time of sending, the n3rv message struct will be serialized either to JSON/Protobuf 
(depending which serializing method you chose when building the lib), 
ready to be deserialized on the other end.

.. code-block:: c++
 
    n3rv::service s1("127.0.0.1",10001);
    
    /* We create a new connection to get a qhandler* */
    n3rv::qhandler* q = s1.connect("com.svc.node1.binding1",ZMQ_REQ);

    /* We create a new n3rv message and fill it with some data */
    n3rv::message msg;
    msg.action = "RPC_CALL";
    msg.args.emplace_back("MyRemoteFunction");
    msg.payload = "I am a n3rv message payload!";

    /* We send the message to the ZMQ socket attached to qhandler* q */
    s1.send(q,msg,0);


Send a Raw ZMQ message
----------------------

n3rv also lets the possibility to send raw ZMQ messages (`zmq::message_t`), which is not much more complicated
than n3rv messages:

.. code-block:: c++
 
    n3rv::service s1("127.0.0.1",10001);
    
    /* We create a new connection to get a qhandler* */
    n3rv::qhandler* q = s1.connect("com.svc.node1.binding1",ZMQ_REQ);

    /* We create a new zmq message_t and fill it with some data */
    
    std::string pload = "My ZMQ Payload !!";
    zmq::message_t* msg = new zmq::message_t(pload.size());
    memcpy(msg->data(), pload.data(), pload.size());

    /* We send the message to the ZMQ socket attached to qhandler* q */
    s1.send(q,msg,0);


ZMQ flags
---------

As you may have noticed, `service::send()` has a `flags` paramaters, which allows to 
pass extra flags to the underlying `zmq_send()`, including:

- ZMQ_DONTWAIT, for socket types 
- ZMQ_SNDMORE, for multipart messages (eg big payloads)

You can find more details here: http://api.zeromq.org/4-2:zmq-send

