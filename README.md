n3rv, C++ Microservices boosted by ZeroMQ
=========================================


![N3RV LOGO](/resources/images/n3rv.png?raw=true "n3rv logo")


About
-----

n3rv is a project that aims to provide a performant and easy-to-use microservice framework in C++. it only focus on inter services communication channels and messages transport, and does it good with the use of ZeroMQ as core substrate.

![Working Example](/resources/images/screens/sc1.png "working example")

How does it work ?
------------------

n3rv was built to run multiple services concurrently and let them communicate seamlessy between each other, no matter where they are in your network.
In order to achieve this goal, i needed a very robust and performant communication stack, so ZeroMQ imposed itself from the beginning.

![arch example](/resources/images/n3rv_arch_example.png?raw=true "arch example")

### In General

Each service developped with n3rv must first and foremost register/subscribe to a directory service, which i call "service controller". 
Each new register or update of the directory will trigger an update of the directories in each node (service) alive, via a ZMQ publish socket.
At this point every service has the list of its own peers, therefore it can connect to them..and the magic begins !

Note: Now you might ask yourself "What's the point of using ZMQ if it's for adding a big SPOF to my architecture", right ? Well, that's not entirely true..What would happen if the service controller goes down ? Not much actually..all the services keep working as usual, the only setback is that you can't add new nodes to your platform until the controller becomes back up.

### For the Devs

n3rv lets you define custom service classes, all inheriting from the service 'superclass'. In these newly defined classes, you can fully describe the behaviour of your service: which connections it must bind, which are the other services it must connect to, what it should answer in such and such situations, etc..
The framework also comes with a built-in, JSON-based messages formatting and transport mechanism, which means you don't have to care about that either. ( yet you can still send/receive raw binary data between your services if you want to ==> I also plan to add protobuf messages transport in the near futur. )

Project Dependancies
--------------------

* libzmq 3.2+ (zeromq/libzmq)
* rapidjson (miloyip/rapidjson)
* cmake 2.6+

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
#include <iostream>

class customservice: public n3rv::service {
  using n3rv::service::service;

    //here you can declare connections to other services 
    // to bind and/or establish, and other service initialization directices.
    int initialize() {

        std::stringstream ss;
        ss << "tcp://0.0.0.0:" << this->service_port;
        //creates a new ZMQ socket binding of type PUBLISH
        this->add_bind("pub0",ss.str(), ZMQ_PUB);

    }

    //Allows to pass instructions inside the main service's loop, can be left empty.
    //Note: NEVER perform blocking ops here.
    void hkloop() {

    }

}

int main() {

   
    //creates new instance of customservice class. 
    // this node will be identidied as "pub0", it will try to connect
    // to its service controller at address 127.0.0.1:10001.
    customservice c1("pub0","publisher","127.0.0.1",10001,11001);

    c1.initialize();

    //Runs service main loop.
    c1.run();

}
```

If you want to dive further inside the inner workings of n3rv, i invite you to check the "examples" section of this repository.
