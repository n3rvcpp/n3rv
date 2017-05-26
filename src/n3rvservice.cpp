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


  int service::recv_loop() {

    this->last_nconn = this->connections.size();
    this->last_connlist.clear();

    //std::vector<zmq::pollitem_t> items;
    zmq::pollitem_t* items  = (zmq::pollitem_t*) malloc(sizeof(zmq::pollitem_t) * this->last_nconn );
    zmq::message_t message;

    int i = 0;
    for(std::map<std::string, zmq::socket_t*>::iterator iter = this->connections.begin(); 
        iter != this->connections.end(); 
        ++iter) {

      if (i >= last_nconn - 1 ) break;

      std::string k = iter->first;
      zmq::socket_t* s = iter->second;

      this->last_connlist.emplace_back(k);

      items[i].socket  = s;
      items[i].fd = 0;
      items[i].events = ZMQ_POLLIN;
      items[i].revents = 0;
      i++;

    }

    while(1) {

       zmq::poll (&items[0], 2, -1); 

       for (int j=0;j < this->last_nconn; j++) {
         
         if (items[j].revents & ZMQ_POLLIN) {
            this->connections[this->last_connlist[j]]->recv(&message);

         }
       }       
    }
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