#include "../../../src/n3rvservicecontroller.hpp"
#include "../../../src/n3rvservice.hpp"
#include "../../../src/n3rvlogger.hpp"
#include <iostream>


/** We inherit n3rv service class to create our own service ! */
class worker: public n3rv::service {
  using n3rv::service::service;

  public:

    bool working;

    int initialize() {

      this->working = false;
      this->connect("ventiler1",ZMQ_REQ);
      this->attach("ventiler1",runwork);
     
    }

    void hkloop() {

      if (! this->working && this->directory.find("ventiler1") != this->directory.end() ) {
        this->ll->log(n3rv::LOGLV_DEBUG,"asking for workload..");
        std::string wl_req_str = "WL_REQ";
        zmq::message_t wl_req(wl_req_str.size());
        memcpy(wl_req.data(),wl_req_str.data(),wl_req_str.size());
        this->get_connection("ventiler1").socket->send(wl_req);
        working = true;
      }
      
    }

    static void* runwork(void* objref, zmq::message_t* msg) {
        
    
        worker* self = (worker*) objref;
        self->ll->log(n3rv::LOGLV_XDEBUG,"starting work..");
        self->working = true;

        char * data_ = (char*) calloc(msg->size(), sizeof(char) );
        memcpy(data_, msg->data(), msg->size());
        
        std::string data = data_;
        data = std::regex_replace(data,std::regex("WL_RESP:"),"");
        int facto_pl = atoi(data.c_str());

        long long res = 1;
        for (int i=1;i<facto_pl;i++) {
          res = res * i;
        }

        std::stringstream ss;
        ss << res;
        self->ll->log(n3rv::LOGLV_NORM, "work result: " +  ss.str());

        //self->working = false;

    }

};

class vent: public n3rv::service {
  using n3rv::service::service;

  public:

    int initialize() {

      std::stringstream ss;
      ss << "127.0.0.1:" << this->service_port;

      this->ll->log(n3rv::LOGLV_NORM, "binding service to " + ss.str());
      this->add_bind("workers_ch",ss.str().c_str(),ZMQ_REP);
      this->attach("workers_ch", wl_dist);
    } 

    static void* wl_dist(void* objref, zmq::message_t* dirmsg) {

      vent* self = (vent*) objref;

      self->ll->log(n3rv::LOGLV_NORM,"distributing new workload..");

      srand(time(0));
      int factor_val = rand() % 30 + 2;
      std::stringstream ss;
      ss << factor_val;
      std::string foo = "WL_RESP:" + ss.str();
      self->send("workers_ch", foo,0);

    }

};


int main(int argc, char** argv) {

  std::cout << "N3rv Workers Example -- Copyright 2017 Quotek" << std::endl;
  std::cout << "=============================================" << std::endl;

  if ( strcmp(argv[2],"ctl") == 0 ) {

    n3rv::servicecontroller sc("0.0.0.0", atoi(argv[4]));

    sc.ll->add_dest("stdout");
    sc.ll->set_loglevel(n3rv::LOGLV_XDEBUG);
    sc.ll->log(n3rv::LOGLV_NORM, "Running Service Controller..");
    sc.recv();

  }

  else if (strcmp(argv[4], "worker") == 0) {

    worker w1(argv[2],argv[4], argv[6], atoi(argv[8]), atoi(argv[10]) );
    w1.ll->add_dest("stdout");
    w1.ll->set_loglevel(n3rv::LOGLV_XDEBUG);
    w1.ll->log(n3rv::LOGLV_NORM,"Running Worker..");

    w1.initialize();
    w1.subscribe();    
    w1.run();

  }

  else if (strcmp(argv[4],"vent") == 0) {

    vent v1(argv[2],argv[4], argv[6], atoi(argv[8]), atoi(argv[10]) );
    v1.ll->add_dest("stdout");
    v1.ll->set_loglevel(n3rv::LOGLV_XDEBUG);
    v1.ll->log(n3rv::LOGLV_NORM,"Running Ventiler..");

    v1.initialize();
    v1.subscribe();
    v1.run();

  }

}