n3rv Terminology
================

The n3rv framework uses a specific vocabulary to describe all the objects it 
actually deals with, but don't worry: if you are already familiar with 
multicomponents network architectures, nothing hard here.

Namespace
---------

A namespace is a named logical space within which your nodes can communicate. 

Note: 2 nodes belonging to different namesapces can communicate without any issue, the namespace
property of a node just adds an extra level of hierarchy so that you can organize your n3rv 
networks more easilly.

Service Class
-------------

In n3rv, a service class is a service implementation that tells how an instanciated node 
should behave: 

- which communication channels it should open/connect to
- How it should process the exchanged data (business rules). 

Concretely, a service class is a C++ class definition that inherits from the n3rv::service 
superclass. 

Node
----

The main building block of a n3rv architecture, a node is the running version 
of a service class, once it has been instanciated. You can of course run multiple 
nodes on seprarated physical hosts provided that they can communicate with each other 
over TCP/IP, or on a single machine. Each node on a n3rv cluster is identified with a 
unique uid, which is a dot-separated concatenation of the node's namespace, its service class
name and its short name.

Binding
-------

To communicate with each others and establish connections, your nodes will need 
to create bindings. Bindings are created by your nodes each time they call bind() on a ZMQ socket.
Once created, the node sends a subcribe request to the service controller 
for the binding to be reachable by your other network's nodes. Inside a n3rv network, each binding 
is identified by a unique id, which is composed of the Node's uid plus the binding short name.

Service Controller
------------------

A service controller is an essential component of a n3rv network, it allows to 
keep an uptodate directory of all your nodes and bindings in the network, and regularly 
sends updates of said directory to your nodes so they don't have to ask whenever they want to
reach a specific location.

Topology
--------

In the n3rv vocab, a topology is a description of how your different service classes interract
with each other and which communication channels (bindings/connections) they must open.
Topology files allow to reconfigure the architecture of your network and (partially) change 
the behaviour of your nodes without even needing to recompile your code.

cluster
-------

A cluster is a group of n3rv nodes communicating with each other (being in the same namespace or not) 
along with their service controllers.