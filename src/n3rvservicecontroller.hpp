#ifndef N3RV_SERVICE_CTRL_HPP
#define N3RV_SERVICE_CTRL_HPP

#include <zmq.hpp>
#include <sstream>
#include <iostream>

#include <map>
#include "n3rvcommon.hpp"

namespace n3rv {

  class servicecontroller {

    public:

      servicecontroller(std::string binding_addr, unsigned int binding_port) {

        this->zctx = zmq::context_t(1);
        this->zmsock = new zmq::socket_t(this->zctx, ZMQ_REP);

        this->binding_addr = binding_addr;
        this->binding_port = binding_port;

        std::stringstream ss;
        ss << "tcp://" << this->binding_addr << ":" << this->binding_port;

        zmsock->bind(ss.str().c_str());

      }


      void recv() {

        zmq::message_t reply;
        while(  zmsock->recv(&reply) ) {
            std::cout << (char*) reply.data() << std::endl;
        }

      }


    protected:

      std::string binding_addr;
      unsigned int binding_port;
      zmq::context_t zctx;
      zmq::socket_t* zmsock;
      std::map<std::string, n3rv::qserv> registered_services;

  };

}

#endif