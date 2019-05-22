#include <n3rv/n3rvtopology.hpp>
#include <iostream>

int test_topology_parse() {

    return 0;

}

int test_topology_serialize() {

    n3rv::topology t;

    n3rv::jservice_class svc;

    n3rv::jbinding_ b;
    b.binding_name ="foo";
    b.type="ZMQ_PUB";
    b.port = 11001;
    svc.bindings.emplace_back(b);

    n3rv::jcallback cb;
    cb.uid="foo";
    cb.callback_name="fb_callback";
    svc.receive_callbacks.emplace_back(cb);

    t.svclasses["foo_class"] = svc;

    std::string serialized_topo = t.serialize();
    
    if ( serialized_topo.size() == 0 ) {
        return 1;
    }
    return 0;

}
