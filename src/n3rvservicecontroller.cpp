#include "n3rvservicecontroller.hpp"

namespace n3rv {

  servicecontroller::servicecontroller(std::string binding_addr, unsigned int binding_port) {

        this->ll = new logger(LOGLV_NORM);

        this->zctx = zmq::context_t(2);
        this->zmsock = new zmq::socket_t(this->zctx, ZMQ_REP);
        this->zmsock_pub = new zmq::socket_t(this->zctx, ZMQ_PUB);

        this->binding_addr = binding_addr; 
        this->binding_port = binding_port;

        this->topo_ = nullptr;

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


  std::string servicecontroller::peer_ip(zmq::message_t* zmsg) {

      std::string ip;
      int fd = zmq_msg_get((zmq_msg_t*) zmsg, ZMQ_SRCFD);
      zmq::get_peer_ip_address(fd, ip);
      return ip;

  }

  int servicecontroller::load_topology(std::string path) {
      this->topo_  = topology::load(path);
  }

  std::thread* servicecontroller::run_async() {
      
        std::thread* res = new std::thread([this] {  this->run();  });
        res->detach();
        return res;
    }

  void servicecontroller::run() {

        zmq::message_t query;
        while(true) {
            
            if (zmsock->recv(&query) != 0 ) {

              n3rv::message m = parse_msg(&query);

              if ( m.action == "subscribe"  ) {

                if (m.args.size() < 5) {
                  zmq::message_t reply(3);
                  memcpy(reply.data(),"ERR",3);
                  zmsock->send(reply);
                  this->ll->log(LOGLV_DEBUG, "subscription error: missing arg");
                  continue;
                }

                n3rv::binding b;
                b.name = m.args[3];
                b.port = atoi(m.args[4].c_str());

                //tries to lookup qserv before creating a new one.    
                n3rv::qserv* nserv = nlookup(this->directory,m.args[1],m.args[2]);

                //we found node, we just add binding
                if (nserv != nullptr)  {
                    nserv->bindings.emplace_back(b);
                }

                //node didn't exist, we create it (with binding), and add it to the directory
                else {
                  nserv = new n3rv::qserv();
                  nserv->namespace_ = m.args[0];
                  nserv->service_class = m.args[1];
                  nserv->node_name = m.args[2];
                  nserv->ip = this->peer_ip(&query);
                  nserv->bindings.emplace_back(b);
                  this->directory.emplace_back(*nserv);
                }



                zmq::message_t reply(2);
                memcpy(reply.data(),"OK",2);
                zmsock->send(reply);

                this->ll->log(LOGLV_DEBUG, "subscription ok");


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

              else if ( m.action == "topology" ) {

                 std::string resp = "";
                 if (this->topo_ != nullptr) { 
                      resp = this->topo_->serialize();               
                 }
                 else {
                     resp = "ERR: NO TOPOLOGY";
                 }

                 zmq::message_t to_send(resp.size());
                 memcpy(to_send.data(), resp.data(), resp.size());
                 this->ll->log(LOGLV_DEBUG, "sending topology..");
                 zmsock->send(to_send);
           
              }

            }
        }
    }

    void servicecontroller::terminate() {

        this->zmsock->close();
        this->zmsock_pub->close();

    }


}
