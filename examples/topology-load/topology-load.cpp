#include <n3rv/n3rvservice.hpp>
#include <n3rv/n3rvtopology.hpp>

class genclass: public n3rv::service {
    using n3rv::service::service;

};



int main() {

    n3rv::topology* t = n3rv::topology::load("topology.json");

}