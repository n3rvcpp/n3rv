#include <n3rv/n3rvservicecontroller.hpp>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <array>
#include "testutils.hpp"

int test_servicecontroller_instanciate() {

    n3rv::servicecontroller sc1("0.0.0.0",10001);
    if (! test_listen(10001) ) return 1;
    if (! test_listen(10002)) return 2;
    sc1.terminate();
    return 0;
}

int test_servicecontroller_load_topology() {

    n3rv::servicecontroller sc1("0.0.0.0",10001);
    sc1.load_topology("topology.json");
    sc1.terminate();

    return 0;


}