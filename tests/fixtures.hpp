#ifndef __TST_FIXTURES__
#define __TST_FIXTURES__

#include <n3rv/n3rvcommon.hpp>
#include <n3rv/n3rvservicecontroller.hpp>

n3rv::servicecontroller* fix_svctl() {

    n3rv::servicecontroller* sc1 = new n3rv::servicecontroller("0.0.0.0",10001);
    return sc1;
}

std::vector<n3rv::qserv_> directory() {
    std::vector<n3rv::qserv_> res;
    n3rv::qserv_ q1;

    q1.namespace_ = "com";
    q1.service_class = "class";
    q1.node_name = "node1";

    n3rv::binding b1;
    b1.name= "binding1";
    b1.port = 0,
    b1.socket_type = ZMQ_REP;

    q1.bindings.emplace_back(b1);
    res.emplace_back(q1);

    return res;
}

#endif