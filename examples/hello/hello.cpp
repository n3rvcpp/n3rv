#include <n3rv/n3rvcommon.hpp>
#include <n3rv/n3rvservice.hpp>
#include <n3rv/n3rvservicecontroller.hpp>
#include <iostream>

    //This service continuously broadcasts "
    //Hello World" to a publish endpoint.
    class hellosend: public n3rv::service {
      using n3rv::service::service;
      public:

      n3rv::qhandler* hello;

      //here you can declare connections to other services 
      // to bind and/or establish, and other service initialization directices.
      int initialize() {

        //Sets a global identifier for service node.
        this->set_uid("com.hellosend.hello1");

        //creates a new ZMQ socket binding of type PUBLISH
        hello = this->bind("hello","0.0.0.0", ZMQ_PUB);

      }

      void hkloop() {
          n3rv::message msg;
          msg.payload = "Hello World";
          this->send(hello,msg,0);
      }

    };

    //This service subscribes to the "hello" 
    //service to receive its broadcasts.
    class helloreceive: public n3rv::service {
      using n3rv::service::service;
      public:

      n3rv::qhandler* hello;

      int initialize() {
          
          this->set_uid("com.hellorecv.recv1");
          hello = this->connect("com.hellosend.*.hello",ZMQ_SUB);
          this->attach(hello, hello_recv);
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

       hellosend hs("127.0.0.1",10001);
       hs.initialize();
       hs.run_async();

       helloreceive hr("127.0.0.1",10001);
       hr.ll->add_dest("stdout");
       
       hr.initialize();
       hr.run();

}
