#include "n3rvhttpservice.hpp"
#include <evhttp.h>
#include <thread>

namespace n3rv {

  
    int httpservice::init_http(std::string http_listen_addr, int http_listen_port) {

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

    }

    int httpservice::attach_http(std::string uri, httpcb cb) {
        if (uri == "*") {
            evhttp_set_gencb(this->ev_server, cb , this);
        }

        else {
            evhttp_set_cb(this->ev_server, uri.c_str(), cb, this);
        }

        return 0;

    }


    int httpservice::run_http() {

        auto* bound_sock = evhttp_bind_socket_with_handle(this->ev_server,
                                                        this->http_listen_addr.c_str(), 
                                                        this->http_listen_port);

        this->ll->log(LOGLV_XDEBUG,"running httpd message loop");
        
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

    std::thread* httpservice::run_http_async() {

        std::thread* t = new std::thread( [this] {
            this->run_http();
        }   );

        t->detach();
        return t;

    }

    /* sample callback */
    void httpservice::http_callback(evhttp_request *req, void * objref) {

        httpservice* self = (httpservice*) objref;
        auto *out_buff = evhttp_request_get_output_buffer(req);
        if (!out_buff) return;

        self->ll->log(LOGLV_DEBUG, "sending back http reply");

        evbuffer_add_printf(out_buff, "<html><body>Hello World!</body></html>");
        evhttp_send_reply(req, HTTP_OK, "", out_buff);
    }

}



