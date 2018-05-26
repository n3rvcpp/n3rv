/** This example show the building of a generic service class, 
 * then the loading of a topology file in order to performs bindings/connections automatically. */

#include <n3rv/n3rvservicecontroller.hpp>
#include <n3rv/n3rvservice.hpp>
#include <n3rv/n3rvtopology.hpp>

class genericservice: public n3rv::service {
    using n3rv::service::service;

    public:

    bool plock;

    int initialize() {      
       this->map_callbacks();
       this->plock = false;
    }

    void map_callbacks() {
        this->cbmap["pr"] = ping_received;
        this->cbmap["por"] = pong_received;
    }

    void hkloop() {
        if (this->service_class == "ping") {

            if (this->directory.find("pong.pong") != this->directory.end() && ! this->plock) {
                 n3rv::message ping_msg;
                 ping_msg.action = "PING";
                 this->ll->log(n3rv::LOGLV_NORM, "Sending Ping..");
                 this->send("pong.pong",ping_msg,0);
                 this->plock = true;

            }         
        }
    }


    static void* pong_received(void* objref, zmq::message_t* zmsg) {
        genericservice* self = (genericservice*) objref;
        self->plock = false;
    }


    static void* ping_received(void* objref, zmq::message_t* zmsg) {

        genericservice* self = (genericservice*) objref;

        self->ll->log(n3rv::LOGLV_NORM, "Received ping, sending back\"pong\"");
        n3rv::message pong_msg;
        pong_msg.action = "PONG";
        self->send("pong.pong", pong_msg,0);
    }
};



int main(int argc, char* argv[]) {

    std::string sclass = argv[1];

    if (sclass == "ping" || sclass == "pong") {
         genericservice sc(sclass,sclass, "127.0.0.1", 10001);
         sc.ll->add_dest("stdout");
         sc.ll->set_loglevel(4);
         sc.initialize();

         //We load a topology file to determine how the services will behave.
         sc.load_topology("topology.json");
         sc.run();
    }

    else if (sclass == "ctl") {
          std::cout << "STARTING CTRL!!" << std::endl;
          n3rv::start_controller("0.0.0.0",10001,true,4);
          while(true) {}
    }

}