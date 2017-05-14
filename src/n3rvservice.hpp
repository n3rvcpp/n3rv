#ifndef N3RV_SERVICE_HPP
#define N3RV_SERVICE_HPP

#include <map>
#include <zmq.hpp>

namespace n3rv {

  typedef struct qserv_ {
     std::string host;
     unsigned int port;
  } qserv;

  class service {

   public:

    service(std::string controller_host, int controller_port, int service_port) {


        this->zctx = zmq::context_t(1);

        this->controller_host = controller_host;
        this->controller_port = controller_port;
        this->service_port= service_port;

    }

    ~service() {

    }

    int subscribe() {

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

   std::string name;
   std::string controller_host;
   int controller_port;
   int service_port;
   std::map<std::string, qserv> services_map;
   zmq::context_t zctx;

  };

}

#endif
