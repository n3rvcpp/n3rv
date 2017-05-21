#ifndef N3RV_SERVICE_HPP
#define N3RV_SERVICE_HPP

#include <map>
#include <iostream>
#include <sstream>

#include <zmq.hpp>
#include "n3rvcommon.hpp"

namespace n3rv {

  class service {

   public:

    service(std::string service_class, std::string controller_host, int controller_port, int service_port) {


        this->zctx = zmq::context_t(1);

        this->service_class = service_class;
        this->controller_host = controller_host;
        this->controller_port = controller_port;
        this->service_port= service_port;

        this->connections["controller"] = new zmq::socket_t(this->zctx,ZMQ_REQ);

        //connects to controller.
        std::stringstream ss;
        ss << "tcp://" << controller_host << ":" << controller_port;
        this->connections["controller"]->connect(ss.str().c_str());

    }

    ~service() {

    }

    int subscribe() {

      std::stringstream ss;
      ss << "SUBSCRIBE " << this->service_class << " " << this->service_port << "\n";

      const char* to_send = ss.str().c_str();
      
      zmq::message_t req (strlen(to_send));
      memcpy (req.data(), to_send , strlen(to_send));

      this->connections["controller"]->send(req);

    }

    int unsubscribe() {

    }

    int init() {

    }

    //Will run both runloop and protoloop.
    int start() {

    }

    void* runloop() {

    }

    void* protoloop() {

    }

    int send(std::string recipient, void* payload) {

    }

    int broadcast(void * payload) {

    }

  protected:

   std::string service_class;
   std::string name;
   std::string controller_host;
   int controller_port;
   int service_port;
   std::map<std::string, qserv> directory;
   std::map<std::string, zmq::socket_t*> connections;

   zmq::context_t zctx;

  };

}

#endif
