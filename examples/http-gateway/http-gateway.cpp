#include <n3rv/n3rvservice.hpp>
#include <n3rv/n3rvhttpservice.hpp>
#include <n3rv/n3rvservicecontroller.hpp>


class w3service: n3rv::httpservice {

    int initialize() {

        this->attach_http("/foo",n3rv::httpservice::http_callback);

    }




};


int main() {
    //we start an hidden svc controller with broker.
    n3rv::start_controller("0.0.0.0",10001,true,4);

    




}

