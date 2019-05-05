#include <n3rv/n3rvservice.hpp>
#include <n3rv/n3rvservicecontroller.hpp>
#include "fixtures.hpp"
#include "testutils.hpp"
#include <thread>

namespace n3rv {

    class service_test: public service {
    using service::service;
    public:

        int initialize() {

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

    int result = 0;

    n3rv::service_test st1("127.0.0.1",10001);

    if (st1.get_controller_host() != "127.0.0.1") result = 1;
    if (st1.get_controller_port() != 10001) result = 2;
    if (st1.get_connections().size() < 2) result =  3;

    st1.stop();
    st1.terminate();

    return result;

}

int test_service_bind() {

    n3rv::servicecontroller sc("0.0.0.0",10001);
    sc.run_async();

    n3rv::service_test* st1 = new n3rv::service_test("127.0.0.1",10001);
    st1->set_uid("com.class.node1");
    n3rv::qhandler* q1 = st1->bind("foo", "0.0.0.0", ZMQ_REP, 12004);

    st1->run_async();

    if (! test_listen(12004)) {
        delete st1;
        return 1;
    }

    delete st1;

    if (test_listen(12004)) return 2;
    
    return 0;
}

int test_service_connect() {

       n3rv::logger* ll = new n3rv::logger(n3rv::LOGLV_XDEBUG);
       ll->add_dest("stdout");

       //n3rv::servicecontroller sc("0.0.0.0",10001,ll);
       //sc.run_async();

       //n3rv::service_test st1("127.0.0.1",10003,ll);
       //st1.set_uid("com.class.node1");

       return 0;

}
