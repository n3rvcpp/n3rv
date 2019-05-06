.. n3rv SDK documentation master file, created by
   sphinx-quickstart on Thu Aug  6 02:11:10 2015.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

API Documentation
=================

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

Provides timestamped logging to your service, which output can be files and/or stdout.
:doc:`See Class Documentation <api_logger>`

Service Controller Class
^^^^^^^^^^^^^^^^^^^^^^^^

This class allows to instanciate a service controller, which plays 
the role of endpoints directory.
:doc:`See Class Documentation <api_servicecontroller>`

Topology Class
^^^^^^^^^^^^^^

This class allows to load and parse topologies, which allows to automatically 
bind/connect to service endpoints.
:doc:`See Class Documentation <api_topology>`

Hello World
------------

  .. code-block:: c++

    #include <n3rv/n3rvservice.hpp>
    #include <n3rv/n3rvservicecontroller.hpp>
    #include <iostream>

        //This service continuously broadcasts "
        //Hello World" to a publish endpoint.
        class hellosend: public n3rv::service {
        using n3rv::service::service;
        public:

        //here you can declare connections to other services 
        // to bind and/or establish, and other service initialization directices.
        int initialize() {
            //creates a new ZMQ socket binding of type PUBLISH
            this->bind("hello","0.0.0.0", 11001, ZMQ_PUB);
        }

        void hkloop() {
            n3rv::message msg;
            msg.payload = "Hello World";
            this->send("hello",msg,0);
        }

        };

        //This service subscribes to the "hello" 
        //service to receive its broadcasts.
        class helloreceive: public n3rv::service {
        using n3rv::service::service;
        public:
        int initialize() {
            this->connect("hello",ZMQ_SUB);
            this->attach("hello", hello_recv);
        }

        //data receive callback.
        static void* hello_recv(void* objref, zmq::message_t* zmsg) {
            helloreceive* self = (helloreceive*) objref;      
            n3rv::message msg = n3rv::parse_msg(zmsg);
            self->ll->log(n3rv::LOGLV_NORM, msg.payload);
        }


        };

        int main() {

        n3rv::start_controller("0.0.0.0",10001,true,4);

        hellosend hs("hello","hello","127.0.0.1",10001);
        hs.initialize();
        hs.run_async();

        helloreceive hr("hellorecv","hellorecv","127.0.0.1",10001);
        hr.initialize();
        hr.ll->add_dest("stdout");
        hr.run();

    }
|
|
|
|
|
|
|
|