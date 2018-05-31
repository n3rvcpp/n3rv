#include <n3rv/n3rvtopology.hpp>
#include <iostream>

int test_topology_parse() {

    return 0;

}

int test_topology_serialize() {

    n3rv::topology t;

    n3rv::node_ n;

    n3rv::tbinding_ b;
    b.name ="foo.bar";
    b.type="ZMQ_PUB";
    b.port = 11001;
    n.bindings.emplace_back(b);

    n3rv::callback cb;
    cb.connection_name="foo.bar";
    cb.callback_name="fb_callback";
    n.callbacks.emplace_back(cb);

    t.nodes["foo_class"] = n;

    std::string serialized_topo = t.serialize();
    
    if ( serialized_topo.size() == 0 ) {
        return 1;
    }
    return 0;

}
