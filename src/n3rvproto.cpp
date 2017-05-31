#include "n3rvproto.hpp"
#include <iostream>

namespace n3rv {

  /** Fixes a nasty bug with zmq/debian */
  void t128bug(std::string& q) {
    if (q.size() % 128 == 0) q += "\n";
  }

  std::string serialize_query(n3rv::n3rvquery& query) {
    
    rapidjson::StringBuffer sb; 
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);

    writer.StartObject();

    writer.String("sender");
    writer.String(query.sender.c_str());

    writer.String("action");
    writer.String(query.action.c_str());

    writer.String("args");
    writer.StartArray();
 
    for (std::string arg: query.args ) {
      writer.String(arg.c_str());
    }
    writer.EndArray();

    writer.String("payload");
    writer.String(query.payload.c_str());
    writer.EndObject();

    std::string result = sb.GetString();

    t128bug(result);
    return result;

  }

  n3rv::n3rvquery parse_query(std::string query) {

    n3rv::n3rvquery query_;

    rapidjson::Document d;
    d.Parse<0>(query.c_str()); 

    assert(d.IsObject());
    assert(d["sender"].IsString());
    assert(d["action"].IsString());
    assert(d["args"].IsArray());
    assert(d["payload"].IsString());

    query_.sender = d["sender"].GetString();
    query_.action = d["action"].GetString();
    query_.payload = d["payload"].GetString();

    for (int i=0;i< d["args"].Size();i++) {
      query_.args.emplace_back( d["args"][i].GetString() );
    }

    return query_;
  }
  


  std::map<std::string, n3rv::qserv> parse_directory(std::string dirstr) {

    std::map<std::string, n3rv::qserv> result;

    rapidjson::Document d;
    d.Parse<0>(dirstr.c_str());
    assert(d.IsArray());

    for (int i =0; i < d.Size(); i++ ) {

      assert(d[i]["name"].IsString());
      assert(d[i]["service_class"].IsString());
      assert(d[i]["port"].IsInt());

      std::string name = d[i]["name"].GetString();

      result[name].service_class = d[i]["service_class"].GetString();
      result[name].port = d[i]["port"].GetInt();

    }

    return result;
  }


  std::string serialize_directory(std::map<std::string, n3rv::qserv>& directory){

    rapidjson::StringBuffer sb; 
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);

    writer.StartArray();
    
    for(std::map<std::string, n3rv::qserv>::iterator iter = directory.begin(); 
        iter != directory.end(); 
        ++iter) {

       std::string k =  iter->first;

       writer.StartObject();

       writer.String("name");
       writer.String(k.c_str());
       writer.String("service_class");
       writer.String(directory[k].service_class.c_str());
       writer.String("port");
       writer.Int(directory[k].port);

       writer.EndObject();
    }

    writer.EndArray();

    return sb.GetString();

  }

}