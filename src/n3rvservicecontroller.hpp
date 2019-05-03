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
#include "n3rvtopology.hpp"
#include "n3rvzmq.hpp"


namespace n3rv {

  /**
   * servicecontroller is the directory service that:
   * - Maintains the full list of available endpoints.
   * - Propagates the available endpoints to every online nodes.*/
  class servicecontroller {

    public:
      /** service controller class constructor. 
       *  @param binding_addr Ip address the controller must listen to (0.0.0.0 for listen all)
       *  @param binding_port TCP port to bind controller on.
      */    
      servicecontroller(std::string binding_addr, unsigned int binding_port);

      ~servicecontroller();
      
      /** Loads a topology file for the service controller to distribute */
      int load_topology(std::string path);
      
      
      /** Runs the service controller once instanciated. */
      void run();
      
      /** Asynchronously runs the service controller, returns a handler for the running thread.*/
      std::thread* run_async();

      /** Stops the binding of CH1/CH2 sockets. */
      void terminate();

      n3rv::logger* ll;

      /* In case the service controller port was automatically allocated. */
      int get_port();


    protected:

      /** Retrieves the sender's ip in order to advertise endpoint 
       *  @param zmsg originating ZMQ message.
       *  @return send's ip string. */
      std::string peer_ip(zmq::message_t* zmsg);
      std::string binding_addr;
      unsigned int binding_port;
      zmq::context_t zctx;
      zmq::socket_t* zmsock;
      zmq::socket_t* zmsock_pub;
      std::vector<n3rv::qserv> directory;
      topology* topo_;

      int init_bindings(int binding_port);

  };


  /** Conveniency function that runs a service controller instance inside its own thread,
   *  so you can embed a service controller along with another service easilly. 
   *  @param binding_addr Ip address the controller must listen to (0.0.0.0 for listen all)
   *  @param binding_port TCP port to bind controller on.
   *  @param display_out tells if service controller must display logs on stdout or not.
   *  @param log_level Tells the log level of the service controller.
   */ 
  std::thread* start_controller(std::string binding_addr, unsigned int binding_port, bool display_out, int log_level) {

      
      std::thread* t = new std::thread([binding_addr, binding_port, display_out, log_level]() {
	       
        servicecontroller sc1(binding_addr, binding_port);
        if (display_out) {
          sc1.ll->add_dest("stdout");
          sc1.ll->set_loglevel(log_level);
        }
        sc1.run();

      } );

      t->detach();
      return t;

  }


}

#endif