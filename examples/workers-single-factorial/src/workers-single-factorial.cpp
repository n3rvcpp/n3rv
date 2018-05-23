/**
 * This example shows a basic service architecture. 1 ventiler communicating with 2 workers.
 * workers ask for jobs to the ventiler, which give them.
 * workers perform the asked computation (factorial), then display the result on their stdout.
 * Note: for continuous work delivery, just uncomment the line "self->working = false;" in runwork() function."
 */

#include <n3rv/n3rvservicecontroller.hpp>
#include <n3rv/n3rvservice.hpp>
#include <n3rv/n3rvlogger.hpp>
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

        n3rv::message m;
        m.action = "WL_REQ";
        this->send("ventiler1", m,0);        
        working = true;
      }
      
    }

    /** Effectively computes factorial when a workload is received. */
    static void* runwork(void* objref, zmq::message_t* zmsg) {
        

        n3rv::message msg = n3rv::parse_msg(zmsg);

        worker* self = (worker*) objref;
        self->ll->log(n3rv::LOGLV_XDEBUG,"starting work..");
        self->working = true;
  
        int facto_pl = atoi(msg.args[0].c_str());

        long long res = 1;

        //factorial, iterrative method.
        for (int i=1;i<facto_pl;i++) {
          res = res * i;
        }

        //prints results (could be sent back to ventiler or to another serivce).
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


    /** Once a workload query has been sent, this callback generates 
     * a new workload (factorial computation) to the asking worker. */
    static void* wl_dist(void* objref, zmq::message_t* zmsg) {

      n3rv::message m_ = n3rv::parse_msg(zmsg);

      if (m_.action == "WL_REQ") {

        vent* self = (vent*) objref;
      self->ll->log(n3rv::LOGLV_NORM,"distributing new workload..");

      srand(time(0));
      int factor_val = rand() % 30 + 2;
      std::stringstream ss;
      ss << factor_val;

      n3rv::message msg;
      msg.action = "WL_DIST";
      msg.args.emplace_back(ss.str());

      self->send("workers_ch",msg,0);

      }

    }

};


int main(int argc, char** argv) {

  std::cout << "N3rv Workers Example -- Copyright 2018 Quotek" << std::endl;
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