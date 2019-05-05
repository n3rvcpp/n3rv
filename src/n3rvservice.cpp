#include "n3rvservice.hpp"
#include <thread>
#include <regex>
#include <iostream>
#include <unistd.h>

namespace n3rv {


  service::service(                
                   const char* controller_host, 
                   int controller_port,
                   logger* ll) {

        this->ll = (ll == nullptr) ? new logger(LOGLV_NORM) :ll;
        
        srand(time(nullptr));

        this->namespace_ = "";
        this->service_class = "";
        this->name = "";

        this->running = false;

        this->poll_timeout = 1000;
        this->zctx = zmq::context_t(1);
        this->controller_host = std::string(controller_host);
        this->controller_port = controller_port;

        // Controller has 2 I/O channels: 1 for req/resp and one for multicast 
        // (the later mostly for directory updates). Therefore, each service must 
        // connect to the 2 of them.

        this->ctlr_ch1 = new qhandler();
        this->ctlr_ch2 = new qhandler();

        this->ctlr_ch1->cid = randstr(10);
        this->ctlr_ch2->cid = randstr(10);

        this->connections[this->ctlr_ch1->cid].socket = new zmq::socket_t(this->zctx,ZMQ_REQ);
        this->connections[this->ctlr_ch2->cid].socket = new zmq::socket_t(this->zctx,ZMQ_SUB);

        // Connects to controller on both channels.
        std::stringstream ss;
        ss << "tcp://" << controller_host << ":" << controller_port;

        this->ll->log(LOGLV_NORM, "Connecting to " + std::string(controller_host) + " controller.." );
        this->connections[this->ctlr_ch1->cid].socket->connect(ss.str().c_str());

        std::stringstream ss2;
        ss2 << "tcp://" << controller_host << ":" << (controller_port + 1);
        this->connections[this->ctlr_ch2->cid].socket->connect(ss2.str().c_str());
        this->connections[this->ctlr_ch2->cid].socket->setsockopt(ZMQ_SUBSCRIBE,"",0);
        
        // Attaches controller's channel 2 to directory updater callback. 
        this->attach(this->ctlr_ch2,this->directory_update);

  }

  service::~service() {
    this->stop();
    this->terminate();
  }

  void service::set_uid(const char* namespace_, const char* service_class, const char* name) {

    this->namespace_ = std::string(namespace_);
    this->service_class = std::string(service_class);
    this->name = std::string(name);

  }

  void service::set_uid(const char* uid) {

    std::string suid = std::string(uid);

    std::vector<std::string> uid_parts;
    std::regex dotsplit("\\.");
    std::sregex_token_iterator iter(suid.begin(),
    suid.end(),
    dotsplit,
    -1);
    std::sregex_token_iterator end;
    for ( ; iter != end; ++iter) uid_parts.emplace_back(*iter);

    if (uid_parts.size() != 3) {
      this->ll->log(LOGLV_CRIT, "cannot set UID: Invalid string");
      return;
    }

    this->namespace_ = uid_parts[0];
    this->service_class = uid_parts[1];
    this->name = uid_parts[2];

  }



  qhandler* service::connect(const char* name, int connection_type, qhandler* hdlref) {

    qhandler* hdl;
    
    if (hdlref == nullptr) {
      hdl = new qhandler();
      hdl->cid = randstr(9);
    }

    else {
      hdl = hdlref;
    }
    
    this->ll->log(LOGLV_NORM,"connecting to " + std::string(name));
    binding* b =  blookup(this->directory, name);

    if (b != nullptr) {
      
      qserv* s = (qserv*) b->parent;

      this->connections[hdl->cid].socket = new zmq::socket_t(this->zctx, connection_type );
      std::stringstream ep;
      ep << "tcp://" << s->ip << ":" << b->port;
      this->connections[hdl->cid].socket->connect(ep.str().c_str());

      //Adds sockopt if zmq socket type is ZMQ_SUB
      if (connection_type == ZMQ_SUB) {
        this->connections[hdl->cid].socket->setsockopt(ZMQ_SUBSCRIBE,"",0);
      }

      hdl->peer_uid = s->namespace_ + "." + 
                      s->service_class + "." + 
                      s->node_name + "." +
                      b->name;
                      
    }

    else {
      this->ll->log(LOGLV_WARN,"peer not found in directory, deferring connection..");
      n3rv::qdef cqd;
      cqd.name = name;
      cqd.socket_type = connection_type;
      cqd.hdl  = hdl;
      this->deferred.emplace_back(cqd);
    }

    return hdl;

  }

  qhandler* service::zbind(const char* bind_name, const char* endpoint, int bind_type ) { 
    
     qhandler* hdl = new qhandler();
     hdl->cid = randstr(8);

    this->connections[hdl->cid].socket = new zmq::socket_t(this->zctx, bind_type);
    this->connections[hdl->cid].socket->bind(endpoint);
    this->connections[hdl->cid].socket->setsockopt(ZMQ_LINGER,0);

    return hdl;

  }

  qhandler* service::bind(const char* bind_name, const char* ip , int bind_type, int port ) {

  
    if (this->namespace_ == "" || 
        this->service_class == "" || 
        this->name == "") {
      this->ll->log(LOGLV_CRIT, "Cannot create binding: Please ensure to call set_uid() beforehand.");
      return nullptr;
    }

    if (bind_name == "") {
       this->ll->log(LOGLV_CRIT, "Cannot bind with an empty name");
      return nullptr;
    }
    
    qhandler* hdl = new qhandler();
    hdl->cid = randstr(8);

    
    this->connections[hdl->cid].socket = new zmq::socket_t(this->zctx, bind_type);
    this->connections[hdl->cid].socket->setsockopt(ZMQ_LINGER,0);
    
    //Port Autobinding (if 0) 
    if (port == 0) {

      /*we use a binding range of 40000-50000 to mitigate collision 
        possibilities */
      port = (rand() % 10001 ) + 40000;

      try {
        std::stringstream ep;
        ep << "tcp://" << ip << ":" << port;
        this->connections[hdl->cid].socket->bind(ep.str().c_str());
      }

      catch(const zmq::error_t& e) {

        if (e.num() == 98) {
          std::stringstream ss;
          ss << "Port " << port << " is already bound, trying another one..";
          this->ll->log(LOGLV_WARN, ss.str());
          return this->bind(bind_name, ip, bind_type, 0);
        }
      }
    }

    else {
      std::stringstream ep;
      ep << "tcp://" << ip << ":" << port;
      this->connections[hdl->cid].socket->bind(ep.str().c_str());
    }
     
    this->subscribe(bind_name, port);
    return hdl;

  }

  int service::initialize() {

  }

  void service::map_callbacks() {

  }


  zmq::pollitem_t* service::refresh_pollitems() {

    if (this == nullptr) return nullptr;

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

      if (k == this->ctlr_ch1->cid) continue;

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
 
    this->running = true;
    /** Main service loop, listens to open connections and forwards 
     * the data to the correct handler.
     */
    while(this->running) {

      zmq::pollitem_t* items = this->refresh_pollitems(); 
      try {
        zmq::poll (items,this->last_connlist.size(), this->poll_timeout);
      }
      catch(const zmq::error_t& e) {}

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

  std::thread* service::run_async() {

    std::thread* t = new std::thread([this]() { this->run(); });
    t->detach();
    return t;

  }

  void service::stop() {
    this->running = false;
  }

  int service::terminate() {

      for (auto n3sock: this->connections) {

        try {
          n3sock.second.socket->close();
        }
        catch (const zmq::error_t& e) {
          std::cout << e.what() << std::endl;
        }        
      }

      /*
      try {
          this->zctx.close();
      }
      catch (const zmq::error_t& e) {
        std::cout << e.what() << std::endl;
      }*/

      
  }


  void service::hkloop() {

  }

  int service::attach(qhandler* hdl, fctptr callback) {
    this->chmap[hdl->cid]  = callback;
  }

  int service::attach(qhandler* hdl, std::string callback_name) {
    this->chmap[hdl->cid] = this->cbmap[callback_name];
  }


  std::map<std::string, qhandler*> service::fetch_topology() {

      std::map<std::string, qhandler*> res;

      n3rv::message m;  
      m.action = "topology";
      m.payload = "";
      this->send(this->ctlr_ch1,m,0);

      //fetches response
      zmq::message_t r1;
      this->connections[this->ctlr_ch1->cid].socket->recv(&r1);

      char * rawmsg = (char*) calloc(r1.size()+1, sizeof(char)   );
      memcpy(rawmsg, r1.data(), r1.size() );
      std::string topo_resp = rawmsg;
      std::cout << topo_resp << std::endl;
      
      if (topo_resp != "ERR: NO TOPOLOGY") {
        topology* t = topology::parse(topo_resp);
        res = this->load_topology(t);
      }
      return res;
  }



  std::map<std::string, qhandler*> service::load_topology(std::string path) {
    topology* t = topology::load(path);
    return this->load_topology(t);
  }

  std::map<std::string, qhandler*> service::load_topology(topology* t) {

    std::map<std::string, qhandler*> res;  
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

    for (auto& nmap : t->svclasses) {

      auto& key = nmap.first;
      auto& n = nmap.second;

      if ( n.namespace_ == this->namespace_ && key == this->service_class  ) {

        for (auto& b: n.bindings) {
          qhandler* h = this->bind(b.binding_name.c_str(), "0.0.0.0", zmq_sockmap[b.type], b.port );
          res[b.binding_name] = h;
        }

        for (auto c: n.connections) {
          qhandler* h = this->connect(c.lookup.c_str(), zmq_sockmap[c.type]);
          res[c.uid] = h;
        }

        for (auto& cb: n.callbacks) {
          this->attach(res[cb.uid], cb.callback_name);
        }
        break;
      }
    }

    return res;
  }



  int service::subscribe(const char* binding_name, int port) {

      if (this->connections[this->ctlr_ch1->cid].socket == nullptr) {
        std::cout << "CRIRICAL: Controller socket is null" << std::endl;
        return 1;
      }

      n3rv::message m;
      
      m.sender = this->name;
      m.action = "subscribe";
      m.args.emplace_back(this->namespace_);
      m.args.emplace_back(this->service_class);
      m.args.emplace_back(this->name);
      
      m.args.emplace_back(std::string(binding_name));

      std::stringstream ss;
      ss << port;

      m.args.emplace_back(ss.str());
      m.payload = "";

      std::string to_send = serialize_msg(m);

      zmq::message_t req (to_send.size());
      memcpy (req.data(), to_send.data() , to_send.size());

      this->connections[this->ctlr_ch1->cid].socket->send(req);

      //Waits for response
      zmq::message_t *r1 = new zmq::message_t;
      this->connections[this->ctlr_ch1->cid].socket->recv(r1);
      return 0;

  }


  n3rv::qconn& service::get_connection(qhandler& hdl) {
     return this->connections[hdl.cid];
  }


  int service::send(qhandler* hdl, std::string& data, int flags = 0) {

    zmq::message_t msg(data.size());
    memcpy(msg.data(), data.data(), data.size());
    this->connections[hdl->cid].socket->send(msg);

    return 0;

  }

  int service::send(qhandler* hdl, void* data, size_t size, int flags = 0) {

    zmq::message_t msg(size);
    memcpy(msg.data(), data, size);
    this->connections[hdl->cid].socket->send(msg);

    return 0;

  }

  int service::send(qhandler* hdl, message& msg, int flags=0) {
    if (msg.sender == "") msg.sender = this->name;
    std::string msg_ser = serialize_msg(msg);
    return this->send(hdl,msg_ser, flags);
  }

  int service::send(qhandler* hdl, zmq::message_t* zmsg, int flags =0) {
    this->connections[hdl->cid].socket->send(*zmsg);
  }

  int service::check_deferred() {

    this->ll->log(n3rv::LOGLV_XDEBUG,"checking deferred list..");
    std::vector<n3rv::qdef> deferred_iter(this->deferred);

    int res = 0;
    for (auto def: deferred_iter) {

      binding* b = blookup(this->directory,def.name);
      if (b != nullptr) {
        
        this->ll->log(n3rv::LOGLV_NORM,"reconnecting to " + def.name);
        this->connect(def.name.c_str(), def.socket_type,def.hdl);
        this->deferred.erase(this->deferred.begin() + res);

        res++;
      }

    }
    return res;
  }


  void* service::directory_update(void* objref, zmq::message_t* dirmsg) {

    service* self = (service*) objref;

    self->ll->log(LOGLV_DEBUG,"updating Directory..");
    std::string dirstring(static_cast<char*>(dirmsg->data()), dirmsg->size());
    self->directory = parse_directory(dirstring);   
    self->check_deferred();

  }

  void service::set_poll_timeout(int poll_timeout) {
    this->poll_timeout = poll_timeout;
  }

}