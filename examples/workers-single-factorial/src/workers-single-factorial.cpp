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
    n3rv::qhandler* ventiler;
    int initialize() {

      this->working = false;
      this->ventiler = this->connect("com.vent.ventiler1.ventiler",ZMQ_REQ);
      this->attach(this->ventiler,runwork);
     
    }

    void hkloop() {
      
     n3rv::binding* bind = n3rv::blookup(this->directory,"com.vent.*.ventiler");
     
     if (! this->working && bind != nullptr ) {

        std::cout << "ENTRY EXISTS IN DIR" << std::endl;

        this->ll->log(n3rv::LOGLV_DEBUG,"asking for workload..");

        n3rv::message m;
        m.action = "WL_REQ";
        this->send(this->ventiler, m,0);        
        working = true;
      }
      
    }

    /** Effectively computes factorial when a workload is received. */
    static void* runwork(void* objref, zmq::message_t* zmsg) {
        

        n3rv::message msg = n3rv::parse_msg(zmsg);

        worker* self = (worker*) objref;
        self->ll->log(n3rv::LOGLV_DEBUG,"starting work..");
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
        self->ll->log(n3rv::LOGLV_NOTICE, "work result: " +  ss.str());

        //self->working = false;

    }

};

class vent: public n3rv::service {
  using n3rv::service::service;

  public:

    n3rv::qhandler* ventiler;

    int initialize() {

      this->map_callbacks();

      srand(time(NULL));

      this->ll->log(n3rv::LOGLV_NOTICE, "binding service..");
      this->ventiler = this->bind("ventiler","127.0.0.1",ZMQ_REP);
      this->attach(this->ventiler, vent::wl_dist);
    } 

    /** Once a workload query has been sent, this callback generates 
     * a new workload (factorial computation) to the asking worker. */
    static void* wl_dist(void* objref, zmq::message_t* zmsg) {

      n3rv::message m_ = n3rv::parse_msg(zmsg);

      if (m_.action == "WL_REQ") {

        vent* self = (vent*) objref;
      self->ll->log(n3rv::LOGLV_NOTICE,"distributing new workload..");

      int factor_val = rand() % 30 + 2;
      std::stringstream ss;
      ss << factor_val;

      n3rv::message msg;
      msg.action = "WL_DIST";
      msg.args.emplace_back(ss.str());

      self->send(self->ventiler,msg,0);

      }

    }

};


int main(int argc, char** argv) {

  std::cout << "N3rv Workers Example -- Copyright 2019 Quotek" << std::endl;
  std::cout << "=============================================" << std::endl;

  if ( strcmp(argv[2],"ctl") == 0 ) {

    n3rv::servicecontroller sc("0.0.0.0", atoi(argv[4]));

    sc.ll->add_dest("stdout");
    sc.ll->set_loglevel(n3rv::LOGLV_DEBUG);
    sc.ll->log(n3rv::LOGLV_NOTICE, "Running Service Controller..");
    sc.run();

  }

  else if (strcmp(argv[4], "worker") == 0) {

    worker w1(argv[6], atoi(argv[8]));
    w1.set_uid("com","worker",argv[2]);
    w1.ll->add_dest("stdout");
    w1.ll->set_loglevel(n3rv::LOGLV_DEBUG);
    w1.ll->log(n3rv::LOGLV_NOTICE,"Running Worker..");

    w1.initialize();
    w1.run();

  }

  else if (strcmp(argv[4],"vent") == 0) {

    vent v1(argv[6], atoi(argv[8]));
    v1.set_uid("com.vent.ventiler1");
    v1.ll->add_dest("stdout");
    v1.ll->set_loglevel(n3rv::LOGLV_DEBUG);
    v1.ll->log(n3rv::LOGLV_NOTICE,"Running Ventiler..");

    v1.initialize();
    v1.run();

  }

}