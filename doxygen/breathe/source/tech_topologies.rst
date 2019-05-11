Topologies
==========

The following section will talk about one of the most interesting aspects of the n3rv framework: 
topologies ! Long story short, topologies are JSON service class proto-definitions. 
Once loaded (or fetched) by your nodes, the topology tell them:

 - which bindings to create.
 - which bindings to connect() to.
 - which receive callbacks to attach().
 - which main loop callbacks to run.

In short, you can dynamically reconfigure an entire n3rv cluster behaviour, 
that without changing a single line of code nor recompiling your binaries. This
is extremely powerful: You can for instance create a design pattern where all your code
stands inside a single, "generic" service class, and then instanciate multiple nodes whose
behaviour will be controlled through a topology.


Topology definitions
--------------------

As explained above, topologies are JSON files which contain the descriptions of both 
the external and internal wiring of your nodes.

You will find below an example of JSON topology definition:

.. code-block:: json

  {
	"topology": [

		{
			"namespace": "com",
			"service_class": "pong",
			"binds": [{

					"name": "pong",
					"port": 11001,
					"type": "ZMQ_REP"
				}
			],

			"connects": [],
			"receive_callbacks": [
				["pong", "ping_received"]
			],
			"ml_callbacks": [["00_mainloop","mloop"]]

		},

		{
			"namespace": "com",
			"service_class": "ping",
			"binds": [],
			"connects": [{ "uid": "pong_conn", 
                           "lookup": "com.pong.*.pong", 
                           "type": "ZMQ_REQ"} ],

			"receive_callbacks": [["pong_conn","pong_received"]],
			"ml_callbacks": []

		}
	 ]
  }

Here we defined 2 service classes: `com.ping` and `com.pong` . 
Now If a node with an uid starting with `com.pong` loads this topology,
it will automatically create a new binding named "pong", and attach its ping_received callback
method to it. 

Similarly, if a node with uid starting with `com.ping` loads this topology, it will automatically try
to connect to `"com.pong.*.pong"` binding, and attach the resulting handler with its pong_received callback.

Loading Topologies
------------------

n3rv offers 2 different ways for nodes to load topologies.

Local JSON file
***************

`n3rv::service` has a dedicated method to load topology files: `service::load_topology()`

.. code-block:: c++

  /* We instanciate a new node */
  n3rv::service s1("127.0.0.1",10001);

  /* We set the node's uid */
  s1.set_uid("com.pong.p1");

  /** We load topology file and get back the associated qhandlers */
  std::map<std::string, qhandler*> qhlist = s1.load_topology("/etc/n3rv/topology.json");

The royal way: using the service controller
*******************************************

A smarter way is to use the service controller to distribute topologies:

.. code-block:: c++

  /* We instanciate a new service controller */
  n3rv::servicecontroller sc1("0.0.0.0",10001);

  /* We tell the sc to load the JSON topology file */
  sc1.load_topology("/etc/n3rv/topoplogy.json");

  sc1.run_async();

  /* We instanciate a new node */
  n3rv::service s1("127.0.0.1",10001);

  /* We set the node's uid */
  s1.set_uid("com.pong.p1");

  /* Finally we tell the node to fetch its topology from the service controller */
  std::map<std::string, qhandler*> qhlist = s1.fetch_topology();

  s1.run();

Note about `load_topology()` and `fetch_topologies()` return values: the key string inside the map
correspond respectively to the "uid" value of connects objects, and the "name" field of binds objects.

Callbacks Preregistering
------------------------

As you may have noticed, receive_callbacks and ml_callbacks contains arrays of string couples.
As there is no convenient mechanism in C++ to get a static method pointer given its string name, 
callbacks ( both receive and main loop ones) need to be "pre-registered" by your service classes, meaning
mapped to strings for the topology to find them.

In order to do that, `n3rv::service` has 2 methods: `register_rcb(char*, n3rv::fctptr)` 
and `register_mlcb(char*, n3rv::mlptr)`

Usually called at the `initialize()` phase of your service classes, these methods will allow
the topology parser to effectively find the callbacks referenced in the JSON.

.. code-block:: c++

  class example: public n3rv::service {
	  using n3rv::service::service;

      public:

	    /*let's say that we declare 1 receive callback */
	    static void* recv(void* objref, zmq::message_t* msg) {}

	    /* And then 1 Main loop Callback */
	    static void* ml(void* objref) {}

	    int initialize() {

		  /* To make the 2 previously defined callbacks available for topologies,
		   we need to "preregister" them both: */

		  this->register_rcb("recv",recv);
		  this->register_mlcb("ml",ml)
		
	    }
  };





