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

  int service::connect(std::string name, int connection_type) {

    n3rv::qserv s = this->directory[name];  
    this->connections[name] = new zmq::socket_t(this->zctx, connection_type );
    std::stringstream ep;
    ep << "tcp://" << name << ":" << s.port;
    this->connections[name]->connect(ep.str().c_str());

  }

  int service::add_bind(std::string bind_name, std::string endpoint, int bind_type ) { 
    std::stringstream ep;
    ep << "tcp://" << endpoint;
    this->connections[bind_name] = new zmq::socket_t(this->zctx, bind_type);
    this->connections[bind_name]->bind(ep.str().c_str());
  }

  service::~service() {

  }


  int service::run() {

    this->last_nconn = this->connections.size();
    this->last_connlist.clear();

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

      //JEEEZZZZZ, that sucks !
      items[i].socket = static_cast<void*> (*s);
      items[i].fd = 0;
      items[i].events = ZMQ_POLLIN;
      items[i].revents = 0;
      i++;

    }

    /** Main service loop, listens to open connections and forwards 
     * the data to the correct handler.
     */
    while(1) {
       
       zmq::poll (items,this->last_nconn, 500); 

       for (int j=0;j < this->last_nconn; j++) {
         
         if (items[j].revents & ZMQ_POLLIN) {
            this->connections[this->last_connlist[j]]->recv(&message);

            if ( this->chmap.find(this->last_connlist[j]) != this->chmap.end() ) {
              (*this->chmap[this->last_connlist[j]])(&message);
            }
         }
       } 

       this->hkloop();      
    }

  }

  void service::hkloop() {

  }

  int service::attach(std::string connection_name, fctptr callback) {
    this->chmap[connection_name]  = callback;
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