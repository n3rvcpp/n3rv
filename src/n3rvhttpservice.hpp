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
    
      static void http_callback(evhttp_request *req, void *);
      int init_http(std::string http_listen_addr, int http_listen_port);      
      int run_http();
      std::thread* run_http_async();
      int attach_http(std::string uri, httpcb);

    protected:

      std::string http_listen_addr;
      int http_listen_port;
      struct event_base* evb;
      evhttp* ev_server;
  
  };


}

#endif