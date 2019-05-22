#ifndef N3RV_HTTPSERVICE_HPP
#define N3RV_HTTPSERVICE_HPP

#include "n3rvservice.hpp"
#include <memory>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <evhttp.h>
#include <thread>


namespace n3rv {

  typedef void (*httpcb) (evhttp_request*, void *);

  typedef struct fserve_res_ {
    std::string data;
    bool found;
  } fserve_res;


  class httpservice : public service {
    using service::service;
    public:
    
      /** Opens file on host and retrieves its content.
       *  @param path: file's path.
       *  @return file's content and & bool indicating if file was found.
       */
      fserve_res serve_file(const char* path) {
        fserve_res result;

        char* buffer = new char[1024^2];
        std::fstream inp(path,std::ios::binary|std::ios::in);
        if (inp) {
           result.found = true;
           while(inp.read(buffer, 1024^2)) {
              result.data += buffer;
           }
           inp.close();
        }
        else result.found = false;
        return result;
      }

      /** 
       * Callback for serving directories *
       */
      static void http_dir_callback(evhttp_request *req, void * objref) {

        auto *out_buff = evhttp_request_get_output_buffer(req);
        if (!out_buff) return;

        httpservice* self = (httpservice*) objref;

        std::vector<std::string> uri_parts;
        std::string uristr = std::string(req->uri);

        std::regex slashsplit("\\/");
        std::sregex_token_iterator iter(uristr.begin(),
                                        uristr.end(),
                                        slashsplit,
                                        -1);
        std::sregex_token_iterator end;
        for ( ; iter != end; ++iter) uri_parts.emplace_back(*iter);

        std::string c_uri = "";
        std::string c_rest = uristr;
        int pos = 0;
        for (int i=0;i<uri_parts.size();i++) {
           
          pos += uri_parts[i].length() +1;
          c_uri += "/" + uri_parts[i];     

          std::string real_path = self->uri_dir_map[c_uri];
          if (real_path != "") {
             c_rest = c_rest.substr(pos, c_rest.length() - pos);        
             fserve_res sf = self->serve_file(std::string(real_path + c_rest).c_str());
             if ( sf.found ) {
                evbuffer_add(out_buff, sf.data.data(),sf.data.size());
                self->ll->log(LOGLV_DEBUG, "sending back http reply (200)");
                evhttp_send_reply(req, HTTP_OK, "", out_buff);
                return;
             }
             break;       
          }     
        }

        evbuffer_add_printf(out_buff,"<html><body><h1>404 Not Found</h1></body></html>");
        self->ll->log(LOGLV_DEBUG, "sending back http reply (404)");
        evhttp_send_reply(req, HTTP_NOTFOUND, "", out_buff); 

      }

       /** default HTTP callback (for testing and example)
       * @param req request object coming from libevent.
       * @param objref Calling object's back ref.
       */
      static void http_callback(evhttp_request *req, void * objref) {

        httpservice* self = (httpservice*) objref;
        auto *out_buff = evhttp_request_get_output_buffer(req);
        if (!out_buff) return;

        self->ll->log(LOGLV_DEBUG, "sending back http reply");
        evbuffer_add_printf(out_buff, "<html><body>Hello World!</body></html>");    
        evhttp_send_reply(req, HTTP_OK, "", out_buff);

      }

      /** initializes http service on listen_addr:port
       *  @param http_listen_addr IP to bind http service on.
       *  @param http_listen_port TCP port to bind http service on.
       */ 
      int init_http(std::string http_listen_addr, int http_listen_port) {

        this->http_listen_addr = http_listen_addr;
        this->http_listen_port = http_listen_port;
        this->evb = event_base_new();

        if (!this->evb) {
          this->ll->log(LOGLV_CRIT,"failed to load libevent");
          return -1;
        }
 
        this->ev_server = evhttp_new(this->evb);

        if (!ev_server) {
          this->ll->log(LOGLV_CRIT,"failed to init http server!");
          return -1;
        }

        evhttp_set_gencb(this->ev_server, http_dir_callback , this);
      }

      /** Starts the http service once initialized */          
      int run_http() {

        auto* bound_sock = evhttp_bind_socket_with_handle(this->ev_server,
                                                        this->http_listen_addr.c_str(), 
                                                        this->http_listen_port);

        this->ll->log(LOGLV_DEBUG,"running httpd message loop");
        
        int  res = event_base_dispatch(this->evb);
        if (res == -1) {
           this->ll->log(LOGLV_CRIT,"failed to run httpd message loop");
           return -1;
        }

        else if (res == 1) {
            this->ll->log(LOGLV_CRIT,"no events to listen to!");
            return -1;
        }
      }

      /** Starts the http service, in a dedicated thread */
      std::thread* run_http_async() {
        std::thread* t = new std::thread( [this] {
            this->run_http();
        }   );

        t->detach();
        return t;
      }

      /** HTTP URI Callback binding method
        * @param uri: URI address to attach callback to
        * @param cb callback pointer
        */
      int attach_http(std::string uri, httpcb cb) {
       
        evhttp_set_cb(this->ev_server, uri.c_str(), cb, this);
        return 0;
      }

      int register_dir(std::string uri, std::string path) {
        this->uri_dir_map[uri] = path;
        return 0;
      }

      std::map<std::string,std::string> uri_dir_map;

    protected:

      std::string http_listen_addr;
      int http_listen_port;
      struct event_base* evb;
      evhttp* ev_server;
  
  };


}

#endif