#ifndef N3RV_SERVICE_CTRL_HPP
#define N3RV_SERVICE_CTRL_HPP

#include <zmq.hpp>
#include <sstream>
#include <iostream>

#include <map>
#include "n3rvcommon.hpp"
#include "n3rvproto.hpp"

namespace n3rv {

  class servicecontroller {

    public:
    
      servicecontroller(std::string binding_addr, unsigned int binding_port);
      void recv();

    protected:

      std::string binding_addr;
      unsigned int binding_port;
      zmq::context_t zctx;
      zmq::socket_t* zmsock;
      std::map<std::string, n3rv::qserv> registered_services;

  };

}

#endif