#include <n3rv/n3rvservice.hpp>
#include <n3rv/n3rvservicecontroller.hpp>
#include "fixtures.hpp"

namespace n3rv {

    class service_test: public service {
    using service::service;
    public:

        int initialize() {

        }

        std::string get_name() {
            return this->name;
        }

        std::string get_service_class() {
            return this->service_class;
        }

        std::string get_controller_host() {
            return this->controller_host;
        }

        int get_controller_port() {
            return this->controller_port;
        }

        std::map<std::string, n3rv::qconn_> get_connections() {
            return this->connections;   
        }

    };

}

int test_service_instanciate() {

    n3rv::service_test st1("foo","bar","127.0.0.1",10001);

    if (st1.get_name() != "foo") return 1;
    if (st1.get_service_class() != "bar") return 1;
    if (st1.get_controller_host() != "127.0.0.1") return 1;
    if (st1.get_controller_port() != 10001) return 1;

    st1.terminate();

    return 0;

}

int test_service_bind() {
  
    
    n3rv::start_controller("127.0.0.1",10001,4,true);
    n3rv::service_test st1("foo","bar","127.0.0.1",10001);

    st1.bind("foo.bind", "127.0.0.1", 12004, ZMQ_REP);

    st1.run_async();
    FILE* fh = popen("netstat -anp 2>/dev/null|grep LISTEN|grep 12004","r");

    std::array<char, 255> buff;
    std::string net_out = "";

    while( ! feof(fh) ) {
        fgets(buff.data(),255,fh);
        net_out += buff.data();
    }

    st1.terminate();

     if ( net_out.find("127.0.0.1:12004") == std::string::npos ) {
        return 1;
    }

    return 0;
}

int test_service_connect() {


       n3rv::servicecontroller* sc1 = fix_svctl();

       n3rv::service_test st1("foo","bar","127.0.0.1",10001);
       st1.connect("foo.listen", ZMQ_REQ);
       std::map<std::string, n3rv::qconn_> connections = st1.get_connections();
       n3rv::qconn_ qc;

       try {
           qc = connections["fdjsfkjfksjf"];
       }
       catch(std::exception e) {
           return 1;
       }
       if (qc.socket == nullptr) return 1;
       if (connections.size() != 2) return 1; ;

    
       st1.terminate();
       sc1->terminate();

       return 0;

}
