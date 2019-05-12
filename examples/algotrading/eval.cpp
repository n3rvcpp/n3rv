#include <n3rv/n3rvservice.hpp>
#include <n3rv/n3rvlogger.hpp>
#include <n3rv/n3rvservicecontroller.hpp>

class eval: public n3rv::service {
    using n3rv::service::service;

    public:

    n3rv::qhandler* stream;
    n3rv::qhandler* exec;

    float dow_price = 0;
    int initialize() { 

        this->stream = this->connect("quotek.broker.*.stream", ZMQ_SUB);
        this->attach(this->stream,process_data);
        this->exec = this->connect("quotek.exec.*.exec",ZMQ_PUSH);

    }

    int send_order(std::string asset, int size, float stop, float limit) {

        //if exec is not connected yet, we discard order.       
        if ( this->connections.find(this->exec->cid) == this->connections.end()  ) return 1;

        n3rv::message msg;
        msg.action = "market_order";
        msg.args.emplace_back(asset);
        std::stringstream ss;
        ss << size;
        msg.args.emplace_back(ss.str());
        ss.str("");
        ss << stop;
        msg.args.emplace_back(ss.str());
        ss.str("");
        ss << limit;
        msg.args.emplace_back(ss.str());
        this->send(this->exec,msg,0);

        return 0;


    }

    static void* process_data(void* objref, zmq::message_t* zmsg) {

        eval* self = (eval*) objref;

        //retrieves market datastream
        n3rv::message msg = n3rv::parse_msg(zmsg);

        if ( msg.action == "market_price" && msg.args[0] == "DOW_IA" ) {

            self->dow_price = atoi( msg.payload.c_str() );

            //processing of retrieved data
            self->ll->log(n3rv::LOGLV_NOTICE,"DOW_PRICE:" + msg.payload );

            if (self->dow_price != 0 && self->dow_price <= 20900 ) {
                self->ll->log(n3rv::LOGLV_NOTICE,"SELL SIGNAL!");
                self->send_order("DOW_IA",-1, -5000, 2000);
            }

            else if (self->dow_price >= 21200) {
                self->ll->log(n3rv::LOGLV_NOTICE,"BUY SIGNAL!");
                self->send_order("DOW_IA",1, -5000, 2000);
            }
        
            else if (self->dow_price != 0) {
                self->ll->log(n3rv::LOGLV_NOTICE,"NO SIGNAL!");
            }

        }

    }

};


int main() {


    eval e0("127.0.0.1", 10001);
    e0.set_uid("quotek.eval.eval1");

    e0.ll->set_loglevel(n3rv::LOGLV_DEBUG);
    e0.ll->add_dest("stdout");

    e0.initialize();
    e0.run();


}