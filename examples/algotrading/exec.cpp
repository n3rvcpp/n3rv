#include <n3rv/n3rvservice.hpp>
#include <n3rv/n3rvlogger.hpp>
#include <n3rv/n3rvservicecontroller.hpp>

class exec: public n3rv::service {
    using n3rv::service::service;

    public:

    int initialize() { 

        this->add_bind("exec","0.0.0.0:11004", ZMQ_REP);
        this->connect("broker1.orders", ZMQ_REQ);
        //this->attach("broker1.orders",broker_resp_process);

        this->attach("exec", process_orders);

    }


    static void* process_orders(void* objref, zmq::message_t* zmsg) {

        exec* self = (exec*) objref;

        //retrieves market datastream
        n3rv::message msg = n3rv::parse_msg(zmsg);

        if ( msg.action == "market_order") {
         
            //processing of retrieved data
            self->ll->log(n3rv::LOGLV_NORM,"Processing Market Order..");

        }

    }

};





int main() {


    exec e0("exec1", "executor", "127.0.0.1", 10001, 11004);

    e0.ll->set_loglevel(4);
    e0.ll->add_dest("stdout");

    e0.initialize();
    e0.subscribe();    
    e0.run();


}