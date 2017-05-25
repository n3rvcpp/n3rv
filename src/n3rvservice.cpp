#include "n3rvservice.hpp"

namespace n3rv {

  service::service(std::string service_class, std::string controller_host, int controller_port, int service_port) {

        this->zctx = zmq::context_t(1);

        this->service_class = service_class;
        this->controller_host = controller_host;
        this->controller_port = controller_port;
        this->service_port= service_port;

        this->connections["controller"] = new zmq::socket_t(this->zctx,ZMQ_REQ);

        //connects to controller.
        std::stringstream ss;
        ss << "tcp://" << controller_host << ":" << controller_port;

        std::cout << "Connecting to " << ss.str() << " controller.." << std::endl;

        this->connections["controller"]->connect(ss.str().c_str());

  }

  service::~service() {

  }

  int service::subscribe() {

      n3rv::n3rvquery q1;
      q1.action = "subscribe";
      q1.args.emplace_back(this->service_class);

      std::stringstream ss;
      ss << this->service_port;

      q1.args.emplace_back(ss.str());
      q1.payload = "";

      std::string to_send = serialize_query(q1);

      zmq::message_t req (to_send.size());
      memcpy (req.data(), to_send.data() , to_send.size());

      this->connections["controller"]->send(req);

      return 0;

  }


}