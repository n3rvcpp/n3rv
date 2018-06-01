#include <n3rv/n3rvservicecontroller.hpp>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <array>

int test_servicecontroller_instanciate() {

    n3rv::servicecontroller sc1("0.0.0.0",10001);

    FILE* fh = popen("netstat -anp|egrep '(10001|10002)'","r");
    std::array<char, 255> buff;
    std::string net_out = "";

    while( ! feof(fh) ) {
        fgets(buff.data(),255,fh);
        net_out += buff.data();
    }

    sc1.terminate();

    if (  net_out.find("0.0.0.0:10001") == std::string::npos ||
          net_out.find("0.0.0.0:10002") == std::string::npos ) {
              return 1;
          }

    return 0;
}

int test_servicecontroller_load_topology() {

    n3rv::servicecontroller sc1("0.0.0.0",10001);
    sc1.load_topology("topology.json");
    sc1.terminate();

    return 0;


}