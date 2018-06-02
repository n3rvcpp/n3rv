n3rv, C++ Microservices boosted by ZeroMQ
=========================================


![N3RV LOGO](/resources/images/n3rv.png?raw=true "n3rv logo")


About
-----

n3rv is a project that aims to provide a performant and easy-to-use microservice framework in C++. it only focus on inter services communication channels and messages transport, and does it good with the use of ZeroMQ as core substrate.

![Working Example](/resources/images/screens/sc1.png "working example")

### Key Features

 - Deploy Microservice architectures without needing a Message Queue (Bye-Bye Kafka !).
 - Blazing fast messages exchange between services.
 - HTTP <=> ZMQ Gateway mechanism (httpservice).
 - Easy to code with.
 - Integrated hosts/endpoints resolving (no need for DNS).
 - Takes care of the messages exchange format via JSON serialization. (protobuf coming soon).
 - Dynamic architecture definition.
 
 
How does it work ?
------------------

n3rv was built to run multiple services concurrently and let them communicate seamlessy between each other, no matter where they are in your network.
In order to achieve this goal, i needed a very robust and performant communication stack, so ZeroMQ imposed itself from the beginning.

![arch example](/resources/images/n3rv_arch_example.png?raw=true "arch example")

### In General

Each service developped with n3rv must first and foremost register/subscribe to a directory service, which i call "service controller". Each or update of the directory will trigger an update of the directories in each node (service) alive, via a ZMQ publish socket.
At this point every service has the list of its own peers, therefore it can connect to them..and the magic begins !

Note: Now you might ask yourself "What's the point of using ZMQ if it's for adding a big SPOF to my architecture", right ? Well, that's not entirely true..What would happen if the service controller goes down ? Not much actually..all the services keep working as usual, the only setback is that you can't add new nodes to your platform until the controller becomes back up.

### For the Devs

n3rv lets you define custom service classes, all inheriting from the service 'superclass'. In these newly defined classes, you can fully describe the behaviour of your service: which connections it must bind, which are the other services it must connect to, what it should answer in such and such situations, etc..
The framework also comes with a built-in, JSON-based messages formatting and transport mechanism, which means you don't have to care about that either. ( yet you can still send/receive raw binary data between your services if you want to ==> I also plan to add protobuf messages transport in the near futur. )

API Documentation
-----------------

You can find the full API doc of the n3rv framework at the following address: https://n3rvcpp.github.io/api


Project Dependancies
--------------------

* libzmq 4+ (zeromq/libzmq)
* rapidjson (miloyip/rapidjson)
* cmake 2.6+
* libevent (Optional, if using httpservice)

Build
-----

Once the required dependancies are satisfacted, building n3rv is quite straightforward.

```Shell
git clone https://github.com/quotekio/n3rv
cd n3rv
cmake .
make
make install
```

That's all, you are now ready to build your first µservice architecture with n3rv !


Use
---

Usage for a c++ developer who wants to use n3rv as underlying µservice infrastructure is also quite easy.
Basically all he/she has to do is to inherit the n3rv::service class and define its behaviors ( data received/processed/sent, network IOs, etc.. ).

You'll find below a basic example of how to declare a new publishing service with n3rv.

```C++
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
```

If you want to dive further inside the inner workings of n3rv, i invite you to check the "examples" section of this repository and also to have a look at the API documentation at https://n3rvcpp.github.io/api .

Building Examples
-----------------

One the main library is correctly compiled and installed, you can build all the examples

```Shell
cd examples
cmake .
make
```

TOTO
----

- undiscriminated Service Class/name node resoution mechanics (with random node selection mech)?
- Support for protobuf in addition to JSON for inter-services messages exchange.
- Implementation of a WYSIWYG edtior allowing to generate topologies and produce class templates (separate project).