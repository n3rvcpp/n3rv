#include "n3rvservicecontroller.hpp"

namespace n3rv {

  servicecontroller::servicecontroller(std::string binding_addr, unsigned int binding_port) {

        this->ll = new logger(LOGLV_NORM);

        this->zctx = zmq::context_t(2);
        this->zmsock = new zmq::socket_t(this->zctx, ZMQ_REP);
        this->zmsock_pub = new zmq::socket_t(this->zctx, ZMQ_PUB);

        this->binding_addr = binding_addr; 
        this->binding_port = binding_port;

        std::stringstream ss;
        ss << "tcp://" << this->binding_addr << ":" << this->binding_port;     
        this->ll->log(LOGLV_NORM, "binding service Controller on " + ss.str() + "..");

        zmsock->bind(ss.str().c_str());
        //zmsock->setsockopt(ZMQ_RCVTIMEO,1);

        ss.str(std::string());
        ss.clear();

        ss << "tcp://" << this->binding_addr << ":" << (this->binding_port + 1);
        this->ll->log(LOGLV_NORM, "binding service Controller on " + ss.str() + ".." );
        zmsock_pub->bind(ss.str().c_str());

      }


  void servicecontroller::recv() {

        zmq::message_t query;
        while(true) {
            
            if (zmsock->recv(&query) != 0 ) {

              std::cout << query.data() << std::endl;

              n3rv::n3rvquery q1 = parse_query((char*) query.data());

              if ( q1.action == "subscribe"  ) {

                this->ll->log(LOGLV_DEBUG, "subscription ok");              
                n3rv::qserv nserv;
                nserv.service_class = q1.args[0];
                nserv.port = atoi(q1.args[1].c_str());
                this->directory[q1.sender] = nserv;

                zmq::message_t reply(2);
                memcpy(reply.data(),"OK",2);
                zmsock->send(reply);

                sleep(1);
                std::string newdict =  serialize_directory(this->directory) ;

                 //Sends new directory to whole pool of connected nodes.
                zmq::message_t to_send(newdict.size());
                memcpy(to_send.data(),newdict.data(),newdict.size());
                std::stringstream ss;
                ss << (char*) to_send.data();
                this->ll->log(LOGLV_DEBUG, "sending directory update:" + ss.str());
                zmsock_pub->send(to_send);
                
              }
            }
        }
    }

}
