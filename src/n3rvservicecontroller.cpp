#include "n3rvservicecontroller.hpp"

namespace n3rv {

  servicecontroller::servicecontroller(std::string binding_addr, unsigned int binding_port) {

        this->zctx = zmq::context_t(1);
        this->zmsock = new zmq::socket_t(this->zctx, ZMQ_REP);

        this->binding_addr = binding_addr;
        this->binding_port = binding_port;

        std::stringstream ss;
        ss << "tcp://" << this->binding_addr << ":" << this->binding_port;
        std::cout << "Binding service Controller on " << ss.str() << ".." << std::endl;

        zmsock->bind(ss.str().c_str());

      }

  void servicecontroller::recv() {

        zmq::message_t query;
        while(true) {

            zmsock->recv(&query);
            n3rv::n3rvquery q1 = parse_query((char*) query.data());

            if ( q1.action == "subscribe"  ) {
              std::cout << "SUBSCRIPTION OK!" << std::endl;
              
              zmq::message_t reply(2);
              memcpy(reply.data(),"OK",2);
              zmsock->send(reply);

            }

        }

  }

}
