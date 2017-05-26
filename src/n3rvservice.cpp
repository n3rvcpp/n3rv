#include "n3rvservice.hpp"

namespace n3rv {

  service::service(std::string name, 
                   std::string service_class, 
                   std::string controller_host, 
                   int controller_port, 
                   int service_port) {

        this->zctx = zmq::context_t(1);

        this->name = name;
        this->service_class = service_class;
        this->controller_host = controller_host;
        this->controller_port = controller_port;
        this->service_port= service_port;

        this->connections["controller_c1"] = new zmq::socket_t(this->zctx,ZMQ_REQ);
        this->connections["controller_c2"] = new zmq::socket_t(this->zctx,ZMQ_SUB);

        //connects to controller.
        std::stringstream ss;
        ss << "tcp://" << controller_host << ":" << controller_port;

        std::cout << "Connecting to " << controller_host << " controller.." << std::endl;

        this->connections["controller_c1"]->connect(ss.str().c_str());

        ss.str(std::string());
        ss.clear();

        ss << "tcp://" << controller_host << ":" << (controller_port + 1);
        this->connections["controller_c2"]->connect(ss.str().c_str());

  }

  service::~service() {

  }

  int service::subscribe() {

      n3rv::n3rvquery q1;

      q1.sender = this->name;
      q1.action = "subscribe";
      q1.args.emplace_back(this->service_class);

      std::stringstream ss;
      ss << this->service_port;

      q1.args.emplace_back(ss.str());
      q1.payload = "";

      std::string to_send = serialize_query(q1);

      //std::cout << "to_send:" << to_send << std::endl;
      
      zmq::message_t req (to_send.size());
      memcpy (req.data(), to_send.data() , to_send.size());

      this->connections["controller_c1"]->send(req);

      return 0;

  }


}