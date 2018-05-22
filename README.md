n3rv, C++ Microservices boosted by ZeroMQ
=========================================


![N3RV LOGO](/resources/images/n3rv.png?raw=true "n3rv logo")


About
-----

n3rv is a project that aims to provide an easy-to use microservice infrastructure in C++. It differs a lot from CppMicroServices in that it only focus on inter services communication channels, and does it good with the use of ZeroMQ as core substrate.

![Working Example](/resources/images/screens/sc1.png "working example")


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
