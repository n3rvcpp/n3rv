#include "n3rvservicecontroller.hpp"

namespace n3rv {

  servicecontroller::servicecontroller(std::string binding_addr, unsigned int binding_port) {

        this->zctx = zmq::context_t(2);
        this->zmsock = new zmq::socket_t(this->zctx, ZMQ_REP);
        this->zmsock_pub = new zmq::socket_t(this->zctx, ZMQ_PUB);

        this->binding_addr = binding_addr; 
        this->binding_port = binding_port;

        std::stringstream ss;
        ss << "tcp://" << this->binding_addr << ":" << this->binding_port;
        std::cout << "Binding service Controller on " << ss.str() << ".." << std::endl;

        zmsock->bind(ss.str().c_str());
        //zmsock->setsockopt(ZMQ_RCVTIMEO,1);

        ss.str(std::string());
        ss.clear();

        ss << "tcp://" << this->binding_addr << ":" << (this->binding_port + 1);
        std::cout << "Binding service Controller on " << ss.str() << ".." << std::endl;

        zmsock_pub->bind(ss.str().c_str());

      }


  void servicecontroller::send_directory_update() {

       

    

  }

  void servicecontroller::recv() {

        zmq::message_t query;
        while(true) {
            
            if (zmsock->recv(&query) != 0 ) {

              std::cout << query.data() << std::endl;

              n3rv::n3rvquery q1 = parse_query((char*) query.data());

              if ( q1.action == "subscribe"  ) {
                std::cout << "SUBSCRIPTION OK!" << std::endl;
              
                n3rv::qserv nserv;
                nserv.service_class = q1.args[0];
                nserv.port = atoi(q1.args[1].c_str());
                this->directory[q1.sender] = nserv;

                zmq::message_t reply(2);
                memcpy(reply.data(),"OK",2);
                zmsock->send(reply);

                sleep(1);
                std::string newdict =  serialize_directory(this->directory) ;

                 //Sends directory update to whole pool of connected nodes.
                zmq::message_t to_send(newdict.size());
                memcpy(to_send.data(),newdict.data(),newdict.size());
                std::cout << "Sending directory update..:" << (char*) to_send.data() << std::endl;
                zmsock_pub->send(to_send);
                
              }
            }
        }
    }

}
