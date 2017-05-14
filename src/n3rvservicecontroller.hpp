#ifndef N3RV_SERVICE_CTRL_HPP
#define N3RV_SERVICE_CTRL_HPP

#include <zmq.hpp>
#include <sstream>

namespace n3rv {

  class servicecontroller {

    public:

      servicecontroller(std::string binding_addr, unsigned int binding_port) {

        this->zctx = zmq::context_t(1);
        this->zmsock = new zmq::socket_t(this->zctx, ZMQ_DEALER);

        this->binding_addr = binding_addr;
        this->binding_port = binding_port;

        std::stringstream ss;
        ss << "tcp://" << this->binding_addr << ":" << this->binding_port;

        zmsock->bind(ss.str().c_str());

      }

    protected:

      std::string binding_addr;
      unsigned int binding_port;
      zmq::context_t zctx;
      zmq::socket_t* zmsock;
   
  };

}

#endif