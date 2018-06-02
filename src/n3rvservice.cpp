#include "n3rvservice.hpp"
#include <thread>

namespace n3rv {

  service::service(std::string name, 
                   std::string service_class, 
                   std::string controller_host, 
                   int controller_port) {


        this->ll = new logger(LOGLV_NORM);
        this->zctx = zmq::context_t(1);
        this->name = name;
        this->service_class = service_class;
        this->controller_host = controller_host;
        this->controller_port = controller_port;

        // Controller has 2 I/O channels: 1 for req/resp and one for multicast 
        // (the later mostly for directory updates). Therefore, each service must 
        // connect to the 2 of them.
        this->connections[CTLR_CH1].socket = new zmq::socket_t(this->zctx,ZMQ_REQ);
        this->connections[CTLR_CH2].socket = new zmq::socket_t(this->zctx,ZMQ_SUB);

        // Connects to controller on both channels.
        std::stringstream ss;
        ss << "tcp://" << controller_host << ":" << controller_port;

        this->ll->log(LOGLV_NORM, "Connecting to " + controller_host + " controller.." );
        this->connections[CTLR_CH1].socket->connect(ss.str().c_str());

        ss.str(std::string());
        ss.clear();
        
        ss << "tcp://" << controller_host << ":" << (controller_port + 1);
        this->connections[CTLR_CH2].socket->connect(ss.str().c_str());
        this->connections[CTLR_CH2].socket->setsockopt(ZMQ_SUBSCRIBE,"",0);

      
        // Attaches controller's channel 2 to directory updater callback. 
        this->attach(CTLR_CH2,this->directory_update);

  }

  int service::connect(std::string name, int connection_type) {

    this->ll->log(LOGLV_NORM,"connecting to " + name);

    if (this->directory.find(name) != this->directory.end()) {

      n3rv::qserv s = this->directory[name];  
      this->connections[name].socket = new zmq::socket_t(this->zctx, connection_type );
      std::stringstream ep;
      ep << "tcp://" << s.ip << ":" << s.port;
      this->connections[name].socket->connect(ep.str().c_str());

      //Adds sockopt if zmq socket type is ZMQ_SUB
      if (connection_type == ZMQ_SUB) {
        this->connections[name].socket->setsockopt(ZMQ_SUBSCRIBE,"",0);
      }

    }

    else {
      this->ll->log(LOGLV_WARN,"peer not found in directory, deferring connection..");
      n3rv::qdef cqd;
      cqd.name = name;
      cqd.socket_type = connection_type;
      this->deferred.emplace_back(cqd);
    }

  }

  int service::bind(std::string bind_name, std::string endpoint, int bind_type ) { 
    this->connections[bind_name].socket = new zmq::socket_t(this->zctx, bind_type);
    this->connections[bind_name].socket->bind(endpoint.c_str());
  }

  int service::bind(std::string bind_name, std::string ip, int port , int bind_type ) {

    this->connections[bind_name].socket = new zmq::socket_t(this->zctx, bind_type);

    //Port Autobinding (if 0) 
    if (port == 0) {

      srand(time(nullptr));
      port = rand() % 50000 + 10000;

      try {
        std::stringstream ep;
        ep << "tcp://" << ip << ":" << port;
        this->connections[bind_name].socket->bind(ep.str().c_str());
      }

      catch(std::exception e) {
        return this->bind(bind_name,ip,0,bind_type);
      }


    }

    else {
      std::stringstream ep;
      ep << "tcp://" << ip << ":" << port;
      this->connections[bind_name].socket->bind(ep.str().c_str());
    }
     
    this->subscribe(bind_name, this->service_class, port);

  }


  service::~service() {

  }

  int service::initialize() {

  }

  void service::map_callbacks() {

  }


  zmq::pollitem_t* service::refresh_pollitems() {

    this->last_nconn = this->connections.size();
    this->last_connlist.clear();    

    zmq::pollitem_t* items  = (zmq::pollitem_t*) malloc(sizeof(zmq::pollitem_t) * this->last_nconn );

    int i = 0;
    for(std::map<std::string, n3rv::qconn>::iterator iter = this->connections.begin(); 
        iter != this->connections.end(); 
        ++iter) {

      if (i >= last_nconn - 1 ) break;

      std::string k = iter->first;
      zmq::socket_t* s = iter->second.socket;

      if (k == CTLR_CH1) continue;

      this->last_connlist.emplace_back(k);
      
      //JEEEZZZZZ, that sucks !
      items[i].socket = static_cast<void*> (*s);
      items[i].fd = 0;
      items[i].events = ZMQ_POLLIN;
      items[i].revents = 0;
      i++;

    }

    return items;

  }


  int service::run() {

    zmq::message_t message;

    /** Main service loop, listens to open connections and forwards 
     * the data to the correct handler.
     */
    while(1) {

      zmq::pollitem_t* items = this->refresh_pollitems(); 
      zmq::poll (items,this->last_connlist.size(), 1000);

       for (int j=0;j < this->last_connlist.size(); j++) {
         
         if (items[j].revents & ZMQ_POLLIN) {

            this->connections[this->last_connlist[j]].socket->recv(&message);

            if ( this->chmap.find(this->last_connlist[j]) != this->chmap.end() ) {
              (*this->chmap[this->last_connlist[j]])(this, &message);
            }
         }
       }

       this->hkloop();      
    }

  }

  int service::run_async() {

    std::thread t([this]() {  this->run(); });
    t.detach();

  }

  int service::terminate() {

      for (auto n3sock: this->connections) {

        try {
          n3sock.second.socket->close();
        }
        catch (...) {

        }        
      }

      //this->zctx.destroy();
  }


  void service::hkloop() {

  }

  int service::attach(std::string connection_name, fctptr callback) {
    this->chmap[connection_name]  = callback;
  }

  int service::attach(std::string connection_name, std::string callback_name) {
    this->chmap[connection_name] = this->cbmap[callback_name];
  }


  int service::fetch_topology() {

      n3rv::message m;  
      m.action = "topology";
      m.payload = "";
      this->send(CTLR_CH1,m,0);

      //fetches response
      zmq::message_t r1;
      this->connections[CTLR_CH1].socket->recv(&r1);

      char * rawmsg = (char*) calloc(r1.size()+1, sizeof(char)   );
      memcpy(rawmsg, r1.data(), r1.size() );
      std::string topo_resp = rawmsg;
      std::cout << topo_resp << std::endl;
      
      if (topo_resp != "ERR: NO TOPOLOGY") {
        topology* t = topology::parse(topo_resp);
        this->load_topology(t);
      }
      return 0;
  }



  int service::load_topology(std::string path) {
    topology* t = topology::load(path);
    this->load_topology(t);
  }

  int service::load_topology(topology* t) {

    std::map<std::string, int> zmq_sockmap;

    zmq_sockmap["ZMQ_PUB"] = ZMQ_PUB;
    zmq_sockmap["ZMQ_SUB"] = ZMQ_SUB;
    zmq_sockmap["ZMQ_REQ"] = ZMQ_REQ;
    zmq_sockmap["ZMQ_REP"] = ZMQ_REP;
    zmq_sockmap["ZMQ_XPUB"] = ZMQ_XPUB;
    zmq_sockmap["ZMQ_XSUB"] = ZMQ_XSUB;
    zmq_sockmap["ZMQ_XREQ"] = ZMQ_XREQ;
    zmq_sockmap["ZMQ_XREP"] = ZMQ_XREP;
    zmq_sockmap["ZMQ_PULL"] = ZMQ_PULL;
    zmq_sockmap["ZMQ_PUSH"] = ZMQ_PUSH;
    zmq_sockmap["ZMQ_DEALER"] = ZMQ_DEALER;
    zmq_sockmap["ZMQ_ROUTER"] = ZMQ_ROUTER;
    zmq_sockmap["ZMQ_PAIR"] = ZMQ_PAIR;

    for (auto& nmap : t->nodes) {

      auto& key = nmap.first;
      auto& n = nmap.second;

      if ( key == this->service_class  ) {

        for (auto& b: n.bindings) {
          this->bind(this->name + "." + b.name, "0.0.0.0", b.port, zmq_sockmap[b.type] );
        }

        for (auto& c: n.connections) {
          this->connect(c.name, zmq_sockmap[c.type]);
        }

        for (auto& cb: n.callbacks) {
          this->attach(cb.connection_name, cb.callback_name);
        }
        break;
      }
    }
  }



  int service::subscribe(std::string name, std::string sclass, int port) {

      n3rv::message m;
      
      m.sender = name;
      m.action = "subscribe";
      m.args.emplace_back(sclass);

      std::stringstream ss;
      ss << port;

      m.args.emplace_back(ss.str());
      m.payload = "";

      std::string to_send = serialize_msg(m);

      zmq::message_t req (to_send.size());
      memcpy (req.data(), to_send.data() , to_send.size());

      this->connections[CTLR_CH1].socket->send(req);

      //Waits for response
      zmq::message_t r1;
      this->connections[CTLR_CH1].socket->recv(&r1);
      return 0;

  }


  n3rv::qconn& service::get_connection(std::string connection_name) {
     return this->connections[connection_name];
  }


  int service::send(std::string connection_name, std::string& data, int flags = 0) {
    
    zmq::message_t msg(data.size());
    memcpy(msg.data(), data.data(), data.size());
    this->connections[connection_name].socket->send(msg);

    return 0;

  }

  int service::send(std::string connection_name, void* data, size_t size, int flags = 0) {

    zmq::message_t msg(size);
    memcpy(msg.data(), data, size);
    this->connections[connection_name].socket->send(msg);

    return 0;

  }

  int service::send(std::string connection_name, message& msg, int flags=0) {
    if (msg.sender == "") msg.sender = this->name;
    std::string msg_ser = serialize_msg(msg);
    return this->send(connection_name,msg_ser, flags);
  }

  int service::send(std::string connection_name, zmq::message_t* zmsg, int flags =0) {
    this->connections[connection_name].socket->send(*zmsg);
  }

  int service::check_deferred() {

    this->ll->log(n3rv::LOGLV_XDEBUG,"checking deferred list..");
    std::vector<n3rv::qdef> deferred_iter(this->deferred);

    int res = 0;
    for (auto def: deferred_iter) {
      if (this->directory.find(def.name) != this->directory.end()) {

        this->ll->log(n3rv::LOGLV_NORM,"reconnecting to " + def.name);
        this->connect(def.name, def.socket_type);
        this->deferred.erase(this->deferred.begin() + res);

        res++;
      }
    }
    return res;
  }


  void* service::directory_update(void* objref, zmq::message_t* dirmsg) {

    service* self = (service*) objref;

    self->ll->log(LOGLV_DEBUG,"updating Directory..");
    std::string dirstring((char*) dirmsg->data(), dirmsg->size());
    self->directory = parse_directory(dirstring);    
    self->check_deferred();


  }

}