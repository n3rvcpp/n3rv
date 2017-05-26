#ifndef N3RV_SERVICE_HPP
#define N3RV_SERVICE_HPP

#include <map>
#include <iostream>
#include <sstream>

#include <zmq.hpp>
#include "n3rvcommon.hpp"
#include "n3rvproto.hpp"

namespace n3rv {

  class service {

   public:

    service(std::string name, 
            std::string service_class, 
            std::string controller_host, 
            int controller_port, 
            int service_port);
            
    ~service();
    int subscribe();
    int unsubscribe();

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
