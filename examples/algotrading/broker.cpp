#include <n3rv/n3rvservice.hpp>
#include <n3rv/n3rvlogger.hpp>
#include <n3rv/n3rvservicecontroller.hpp>

class broker: public n3rv::service {
    using n3rv::service::service;

    public:

    float cval = 21000;
    int initialize() { 

        this->set_poll_timeout(500);
        this->bind("broker1.stream","0.0.0.0",11001,ZMQ_PUB);
        this->bind("broker1.orders","0.0.0.0",11002,ZMQ_REP);
        this->attach("broker1.orders",process_orders);
    }
    void hkloop() {

        n3rv::message msg;
        //fake price, random walk
        srand(time(NULL));
        int sign = (rand() % 2 == 0 ) ? 1 : -1;
        this->cval += sign * (rand() % 50 + 2 );

        

        //we publish fake stream
        msg.action = "market_price";
        msg.args.emplace_back("DOW_IA");
        std::stringstream ss;
        ss << cval;
        msg.payload = ss.str();

        this->ll->log(n3rv::LOGLV_NORM,"GENERATED MARKET PRICE:" + msg.payload);
        this->send("broker1.stream",msg,0);

    }

    static void* process_orders(void* objref, zmq::message_t* zmsg) {

    }




};

int main() {

    //we start an hidden svc controller with broker.
    n3rv::start_controller("0.0.0.0",10001,true,4);

    broker b0("broker1", "broker", "127.0.0.1", 10001);
    b0.ll->add_dest("stdout");
    b0.initialize();
    b0.run();


}