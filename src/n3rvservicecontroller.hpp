#ifndef N3RV_SERVICE_CTRL_HPP
#define N3RV_SERVICE_CTRL_HPP

#include <zmq.hpp>

#include <sstream>
#include <iostream>
#include <unistd.h>
#include <thread>

#include <map>
#include "n3rvcommon.hpp"
#include "n3rvproto.hpp"
#include "n3rvlogger.hpp"
#include "n3rvzmq.hpp"

namespace n3rv {

  class servicecontroller {

    public:

      servicecontroller(std::string binding_addr, unsigned int binding_port);
      void recv();
      n3rv::logger* ll;

    protected:

      std::string peer_ip(zmq::message_t* zmsg);
      std::string binding_addr;
      unsigned int binding_port;
      zmq::context_t zctx;
      zmq::socket_t* zmsock;
      zmq::socket_t* zmsock_pub;
      std::map<std::string, n3rv::qserv> directory;
  };


  int start_controller(std::string binding_addr, unsigned int binding_port, bool display_out, int log_level) {

      
      std::thread t([binding_addr, binding_port, display_out, log_level]() {
	       
        servicecontroller sc1(binding_addr, binding_port);
        if (display_out) {
          sc1.ll->add_dest("stdout");
          sc1.ll->set_loglevel(log_level);
        }
        sc1.recv();

      } );

      t.detach();

  }


}

#endif