#include "../src/n3rvcommon.hpp"

n3rv::servicecontroller* fix_svctl() {

    n3rv::servicecontroller sc1("0.0.0.0",10001);
    sc1.run_async();
}


std::vector<n3rv::qserv_> directory() {
    std::vector<n3rv::qserv_> res;
    n3rv::qserv_ q1;

    q1.service_class = "service1";
    res.emplace_back(q1);

    return res;
}