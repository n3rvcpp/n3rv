#include <n3rv/n3rvservice.hpp>
#include <n3rv/n3rvlogger.hpp>
#include <n3rv/n3rvservicecontroller.hpp>

class exec: public n3rv::service {
    using n3rv::service::service;

    public:

    int initialize() { 

        this->bind("exec1","0.0.0.0",11004, ZMQ_PULL);
        this->connect("moneyman1", ZMQ_REQ);
        //this->attach("broker1.orders",broker_resp_process);
        this->attach("exec1", process_orders);
        this->attach("moneyman1", mm_answers);

    }


    static void* process_orders(void* objref, zmq::message_t* zmsg) {

        exec* self = (exec*) objref;

        //retrieves market datastream
        n3rv::message msg = n3rv::parse_msg(zmsg);

        if ( msg.action == "market_order") {

            //processing of retrieved data
            self->ll->log(n3rv::LOGLV_NORM,"Forwarding order to money manager..");
            self->send("moneyman1", zmsg,0);

        }

    }

    static void* mm_answers(void* objref, zmq::message_t* zmsg) {

        exec* self = (exec*) objref;
         //retrieves market datastream
        n3rv::message msg = n3rv::parse_msg(zmsg);

        if (msg.action == "ack") {
            self->ll->log(n3rv::LOGLV_NORM,"Position Accepted by Money Manager");
        }

        else if (msg.action == "error") {
            self->ll->log(n3rv::LOGLV_NORM,"Position Refused by Money Manager:" + msg.args[0]);
        }

    }



};





int main() {

    exec e0("exec1", "executor", "127.0.0.1", 10001);
    e0.ll->set_loglevel(4);
    e0.ll->add_dest("stdout");
    e0.initialize();
    e0.run();


}