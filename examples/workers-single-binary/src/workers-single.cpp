#include "../../../src/n3rvservicecontroller.hpp"
#include "../../../src/n3rvservice.hpp"

#include <iostream>


typedef struct opts_ {

  std::string type;
  std::string ctl_addr;
  int ctl_port;

} opts;


int main(int argc, char** argv) {

  std::cout << "N3rv Workers Example -- Copyright 2017 Quotek" << std::endl;
  std::cout << "=============================================" << std::endl;

  if ( strcmp(argv[2],"ctl") == 0 ) {

    std::cout << "Running Service Controller.." << std::endl;

    n3rv::servicecontroller sc("0.0.0.0", atoi(argv[4]));
    sc.recv();

  }

  else {

    std::cout << "Running Worker.." << std::endl;

    n3rv::service w1(argv[2],argv[4], atoi(argv[6]), 11001);
    w1.subscribe();

  }




}