#include "../../../src/n3rvservicecontroller.hpp"
#include "../../../src/n3rvservice.hpp"

#include <iostream>


/** We inherit n3rv service class to create our own service ! */
class worker: public n3rv::service {
  using n3rv::service::service;

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

    std::cout << "Running Worker.." << std::endl;

    worker w1(argv[2],argv[4], argv[6], atoi(argv[8]), atoi(argv[10]) );
    w1.subscribe();
    w1.run();

  }

  else if (strcmp(argv[4],"vent") == 0) {

    std::cout << "Running Ventiler.." << std::endl;

    vent v1(argv[2],argv[4], argv[6], atoi(argv[8]), atoi(argv[10]) );
    v1.subscribe();
    v1.run();

  }


}