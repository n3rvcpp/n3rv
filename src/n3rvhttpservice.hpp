#ifndef N3RV_HTTPSERVICE_HPP
#define N3RV_HTTPSERVICE_HPP

#include "n3rvservice.hpp"
#include <memory>
#include <cstdint>
#include <iostream>
#include <evhttp.h>
#include <thread>


namespace n3rv {

  typedef void (*httpcb) (evhttp_request*, void *);

  class httpservice : public service {
    using service::service;
    public:
    
      /** default HTTP callback (for testing and example)
       * @param req request object coming from libevent.
       * @param objref Calling object's back ref.
       */
      static void http_callback(evhttp_request *req, void * objref);

      /** initializes http service on listen_addr:port
       *  @param http_listen_addr IP to bind http service on.
       *  @param http_listen_port TCP port to bind http service on.
       */ 
      int init_http(std::string http_listen_addr, int http_listen_port);
      /* Starts the http service once initialized */      
      
      int run_http();
      /** Starts the http service, in a dedicated thread */
      std::thread* run_http_async();

      /** HTTP URI Callback binding method
        * @param uri: URI address to attach callback to
        * @param httpcb callback pointer
        */
      int attach_http(std::string uri, httpcb);

    protected:

      std::string http_listen_addr;
      int http_listen_port;
      struct event_base* evb;
      evhttp* ev_server;
  
  };


}

#endif