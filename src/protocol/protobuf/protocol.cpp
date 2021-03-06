#include "../../n3rvproto.hpp"
#include "n3rvproto.pb.h"
#include <iostream>

namespace n3rv {

  /** Fixes a nasty bug with zmq/debian */
  void t128bug(std::string& q) {
    if (q.size() % 128 == 0) q += "\n";
  }

  std::string serialize_msg(n3rv::message& msg) {
    
    std::string result;
    n3rvmessage msg__;

    msg__.set_sender(msg.sender);
    msg__.set_action(msg.action);
    msg__.set_payload(msg.payload);

    for (auto a: msg.args) {
      msg__.add_args(a);
    }

    result = msg__.SerializeAsString();
    t128bug(result);
    return result;

  }


   n3rv::message parse_msg(zmq::message_t* msg) {
    std::string data_s(static_cast<char*>(msg->data()), msg->size());
    n3rv::message q = parse_msg(data_s);
    return q;
  }


   n3rv::message parse_msg(std::string msg) {

    n3rv::message msg_;
    n3rvmessage msg__;
    msg__.ParseFromString(msg);
    msg_.sender = msg__.sender();
    msg_.action = msg__.action();
    msg_.payload = msg__.payload();

    for (int i=0;i< msg__.args_size();i++) {
      msg_.args.emplace_back( msg__.args(i));
    }

    return msg_;
  }
  


  std::vector<n3rv::qserv> parse_directory(std::string dirstr) {


    std::vector<n3rv::qserv> result;
    n3rvdirectory dir;
    dir.ParseFromString(dirstr);

    for (int i=0;i< dir.nodes_size();i++) {

      n3rv::qserv qs;
      qs.namespace_ = dir.nodes(i).namespace_();
      qs.service_class = dir.nodes(i).service_class();
      qs.node_name = dir.nodes(i).name();
      qs.ip = dir.nodes(i).ip();

      for(int j=0;j<dir.nodes(i).bindings_size();j++) {

        n3rv::binding b;
        b.name = dir.nodes(i).bindings(j).name();
        b.port = dir.nodes(i).bindings(j).port();
        b.socket_type = dir.nodes(i).bindings(j).socket_type();

        qs.bindings.emplace_back(b);

      }

      result.emplace_back(qs);

    }

    return result;
  }


  std::string serialize_directory(std::vector<n3rv::qserv>& directory){

     std::string result;
     n3rvdirectory dir;
    
    for(int i=0;i<directory.size();i++) {

          n3rvnode* node_ = dir.add_nodes(); 
          qserv* s = &(directory[i]);

          node_->set_namespace_(s->namespace_);
          node_->set_name(s->node_name);
          node_->set_service_class(s->service_class);
          node_->set_ip(s->ip);

          for (int j=0;j<s->bindings.size();j++) {
            binding* b = &(s->bindings[j]);
            n3rvbinding* bser = node_->add_bindings();
            bser->set_name(b->name);
            bser->set_port(b->port);
            bser->set_socket_type(b->socket_type);
          }
    }
    result = dir.SerializeAsString();
    t128bug(result);
    return result;

  }

  std::string build_directory_update(std::string update_operation,n3rv::qserv& nserv) {

  }

  





}