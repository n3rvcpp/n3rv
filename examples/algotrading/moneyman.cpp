#include <n3rv/n3rvservice.hpp>
#include <n3rv/n3rvlogger.hpp>
#include <n3rv/n3rvservicecontroller.hpp>

typedef struct position {
    std::string asset;
    int size;
    float open;
    float pnl;
    float leverage; 
    float stop;
    float limit;
} position;



class moneyman: public n3rv::service {
    using n3rv::service::service;

    public:

    std::vector<position> portfolio;
    float RUN_PNL;
    float GLOB_PNL;

    int initialize() { 

        this->GLOB_PNL = 0;

        this->connect("quotek.broker.*.stream", ZMQ_SUB);
        this->bind("moneyman","0.0.0.0",11005, ZMQ_REP);
        //this->connect("broker1.orders", ZMQ_REQ);
        //this->attach("broker1.orders",broker_resp_process);
        this->attach("moneyman", process_mmqueries);
        this->attach("quotek.broker.*.stream", update_portfolio);
    }


    static void* process_mmqueries(void* objref, zmq::message_t* zmsg) {

        moneyman* self = (moneyman*) objref;

        //retrieves market datastream
        n3rv::message msg = n3rv::parse_msg(zmsg);

        if ( msg.action == "market_order") {
         
            //processing of retrieved data
            self->ll->log(n3rv::LOGLV_NORM,"Processing Market Order..");

            n3rv::message ord;

            //pnl is ok, we accept order
            if (self->RUN_PNL >= - 200 && self->portfolio.size() < 5) {
                ord.action = "ack";

                position p;
                p.asset = msg.args[0];
                p.size = atoi(msg.args[1].c_str());
                p.open = 0;
                p.stop = atof(msg.args[2].c_str());
                p.limit = atof(msg.args[3].c_str());
                p.leverage = 10;
                p.pnl  =0;

                self->portfolio.emplace_back(p);

            }
            else {
                ord.action = "error";
                ord.args.emplace_back("too risky");
            }

            self->send("moneyman",ord,0);


        }

    }

    static void* update_portfolio(void* objref, zmq::message_t* zmsg) {

        moneyman* self = (moneyman*) objref;

        //retrieves market datastream
        n3rv::message msg = n3rv::parse_msg(zmsg);

        if ( msg.action == "market_price") {
         
            self->ll->log(n3rv::LOGLV_NORM,"Recalculating PNL..");

            self->RUN_PNL = 0;

            std::vector<position> pfolio_copy(self->portfolio);

            int pnum = 0;
            for (auto pos: pfolio_copy) {


                if (pos.asset == msg.args[0]) {

                    float cval = atoi(msg.payload.c_str());

                    if (pos.open == 0) {
                        pos.open = cval;
                        self->portfolio[pnum].open = cval;
                    }

                    pos.pnl =  pos.size * (cval - pos.open ) * pos.leverage;
                    self->RUN_PNL += pos.pnl;

                    if (pos.pnl <= pos.stop) {
                        self->ll->log(n3rv::LOGLV_NORM,"Closing Position (Stop)");
                        self->GLOB_PNL += pos.pnl;
                        self->portfolio.erase(self->portfolio.begin() + pnum);
                    }

                    else if (pos.pnl >= pos.limit) {
                        self->ll->log(n3rv::LOGLV_NORM,"Closing Position (Limit)");
                        self->GLOB_PNL += pos.pnl;
                        self->portfolio.erase(self->portfolio.begin() + pnum);
                    }


                }

                pnum++;
            }

            std::stringstream ss;
            ss << "PNL:" << self->GLOB_PNL + self->RUN_PNL << " (RUNNING:" << self->RUN_PNL << " )";
            self->ll->log(n3rv::LOGLV_NORM,ss.str());
        }

    }



};





int main() {


    moneyman mm0("127.0.0.1", 10001);
    mm0.set_uid("quotek.moneyman.moneyman1");

    mm0.ll->set_loglevel(4);
    mm0.ll->add_dest("stdout");
    mm0.initialize();
    mm0.run();


}