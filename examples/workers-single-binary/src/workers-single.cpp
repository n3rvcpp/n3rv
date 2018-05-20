#include "../../../src/n3rvservicecontroller.hpp"
#include "../../../src/n3rvservice.hpp"
#include "../../../src/n3rvlogger.hpp"
#include <iostream>


/** We inherit n3rv service class to create our own service ! */
class worker: public n3rv::service {
  using n3rv::service::service;

  public:
    int initialize() {
      this->connect("ventiler1",ZMQ_REQ);
    }

};

class vent: public n3rv::service {
  using n3rv::service::service;
};


int main(int argc, char** argv) {

  std::cout << "N3rv Workers Example -- Copyright 2017 Quotek" << std::endl;
  std::cout << "=============================================" << std::endl;

  if ( strcmp(argv[2],"ctl") == 0 ) {

    std::cout << "Running Service Controller.." << std::endl;

    n3rv::servicecontroller sc("0.0.0.0", atoi(argv[4]));
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

    v1.subscribe();
    v1.run();

  }


}