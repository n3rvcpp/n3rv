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
  


  std::map<std::string, n3rv::qserv> parse_directory(std::string dirstr) {


    std::map<std::string, n3rv::qserv> result;
    n3rvdirectory dir;
    dir.ParseFromString(dirstr);

    for (int i=0;i< dir.nodes_size();i++) {

      auto name = dir.nodes(i).name();
      result[name].service_class = dir.nodes(i).service_class();
      result[name].ip = dir.nodes(i).ip();
      result[name].port = dir.nodes(i).port();
    }

    return result;
  }


  std::string serialize_directory(std::map<std::string, n3rv::qserv>& directory){

     std::string result;
     n3rvdirectory dir;

    for(std::map<std::string, n3rv::qserv>::iterator iter = directory.begin(); 
        iter != directory.end(); 
        ++iter) {

          std::string k =  iter->first;
          n3rvnode* node_ = dir.add_nodes(); 

          node_->set_name(k.c_str());
          node_->set_service_class(directory[k].service_class.c_str());
          node_->set_ip(directory[k].ip.c_str());
          node_->set_port(directory[k].port);
      }

    result = dir.SerializeAsString();
    t128bug(result);
    return result;

  }

  std::string build_directory_update(std::string update_operation,n3rv::qserv& nserv) {

  }

  





}