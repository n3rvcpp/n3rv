#include "../../n3rvproto.hpp"
#include <iostream>
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"

namespace n3rv {

  /** Fixes a nasty bug with zmq/debian */
  void t128bug(std::string& q) {
    if (q.size() % 128 == 0) q += "\n";
  }

  std::string serialize_msg(n3rv::message& msg) {
    
    rapidjson::StringBuffer sb; 
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);

    writer.StartObject();

    writer.String("sender");
    writer.String(msg.sender.c_str());

    writer.String("action");
    writer.String(msg.action.c_str());

    writer.String("args");
    writer.StartArray();;
 
    for (std::string arg: msg.args ) {
      writer.String(arg.c_str());
    }
    writer.EndArray();

    writer.String("payload");
    writer.String(msg.payload.c_str());
    writer.EndObject();

    std::string result = sb.GetString();

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

    rapidjson::Document d;
    d.Parse<0>(msg.c_str()); 

    assert(d.IsObject());
    assert(d["sender"].IsString());
    assert(d["action"].IsString());
    assert(d["args"].IsArray());
    assert(d["payload"].IsString());

    msg_.sender = d["sender"].GetString();
    msg_.action = d["action"].GetString();
    msg_.payload = d["payload"].GetString();

    for (int i=0;i< d["args"].Size();i++) {
      msg_.args.emplace_back( d["args"][i].GetString() );
    }

    return msg_;
  }
  


  std::vector<n3rv::qserv> parse_directory(std::string dirstr) {

    std::vector<n3rv::qserv> result;

    rapidjson::Document d;
    d.Parse<0>(dirstr.c_str());
    assert(d.IsArray());

    for (int i =0; i < d.Size(); i++ ) {

      assert(d[i]["namespace"].IsString());
      assert(d[i]["service_class"].IsString());
      assert(d[i]["name"].IsString());
      assert(d[i]["ip"].IsString());
      assert(d[i]["bindings"].IsArray()); 

  
      n3rv::qserv qs;
      qs.namespace_ = d[i]["namespace"].GetString();
      qs.service_class = d[i]["service_class"].GetString();
      qs.node_name = d[i]["name"].GetString();
      qs.ip = d[i]["ip"].GetString();

      for(int j=0;j< d[i]["bindings"].GetArray().Size();j++) {

          n3rv::binding b;
          b.name = d[i]["bindings"][j]["name"].GetString();
          b.port = d[i]["bindings"][j]["port"].GetInt();
          b.socket_type = d[i]["bindings"][j]["socket_type"].GetInt();
          qs.bindings.emplace_back(b);
      }

      result.emplace_back(qs);
    }

    return result;
  }


  std::string serialize_directory(std::vector<n3rv::qserv>& directory){

    std::string result;
    rapidjson::StringBuffer sb; 
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);

    writer.StartArray();
    
    for (int k=0;k<directory.size();k++) {

       writer.StartObject();
       writer.String("namespace");
       writer.String(directory[k].namespace_.c_str());
       writer.String("service_class");
       writer.String(directory[k].service_class.c_str());
       writer.String("name");
       writer.String(directory[k].node_name.c_str());
       writer.String("ip");
       writer.String(directory[k].ip.c_str());
       writer.String("bindings");

       writer.StartArray();
       
       for (int l=0;l< directory[k].bindings.size();l++) {
         writer.StartObject();
         writer.String("name");
         writer.String(directory[k].bindings[l].name.c_str());
         writer.String("port");
         writer.Int(directory[k].bindings[l].port);
         writer.String("socket_type");
         writer.Int(directory[k].bindings[l].socket_type);
         writer.EndObject();         
       }

       writer.EndArray();
       writer.EndObject();
    }

    writer.EndArray();
    result = sb.GetString();
    t128bug(result);

    return result;

  }

  std::string build_directory_update(std::string update_operation,n3rv::qserv& nserv) {

  }

  





}