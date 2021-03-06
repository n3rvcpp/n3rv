#include <n3rv/n3rvservice.hpp>
#include <n3rv/n3rvlogger.hpp>
#include <n3rv/n3rvservicecontroller.hpp>

class exec: public n3rv::service {
    using n3rv::service::service;

    public:

    n3rv::qhandler* exech;
    n3rv::qhandler* moneyman;

    int initialize() { 

        this->exech = this->bind("exec","0.0.0.0", ZMQ_PULL);
        this->moneyman = this->connect("quotek.moneyman.*.moneyman", ZMQ_REQ);
        //this->attach("broker1.orders",broker_resp_process);
        this->attach(this->exech, process_orders);
        this->attach(this->moneyman, mm_answers);

    }


    static void* process_orders(void* objref, zmq::message_t* zmsg) {

        exec* self = (exec*) objref;

        //retrieves market datastream
        n3rv::message msg = n3rv::parse_msg(zmsg);

        if ( msg.action == "market_order") {

            //processing of retrieved data
            self->ll->log(n3rv::LOGLV_NOTICE,"Forwarding order to money manager..");
            self->send(self->moneyman, zmsg,0);

        }

    }

    static void* mm_answers(void* objref, zmq::message_t* zmsg) {

        exec* self = (exec*) objref;
         //retrieves market datastream
        n3rv::message msg = n3rv::parse_msg(zmsg);

        if (msg.action == "ack") {
            self->ll->log(n3rv::LOGLV_NOTICE,"Position Accepted by Money Manager");
        }

        else if (msg.action == "error") {
            self->ll->log(n3rv::LOGLV_NOTICE,"Position Refused by Money Manager:" + msg.args[0]);
        }

    }



};





int main() {

    exec e0("127.0.0.1", 10001);
    e0.set_uid("quotek.exec.exec1");
    e0.ll->set_loglevel(n3rv::LOGLV_DEBUG);
    e0.ll->add_dest("stdout");
    e0.initialize();
    e0.run();


}