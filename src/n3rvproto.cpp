#include "n3rvproto.hpp"

namespace n3rv {

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

    return sb.GetString();

  }

  n3rv::n3rvquery parse_query(std::string query) {

    n3rv::n3rvquery query_;

    rapidjson::Document d;
    d.Parse<0>(query.c_str()); 

    assert(d.IsObject());
    assert(d["action"].IsString());
    assert(d["args"].IsArray());
    assert(d["payload"].IsString());

    query_.action = d["action"].GetString();
    query_.payload = d["payload"].GetString();

    for (int i=0;i< d["args"].Size();i++) {
      query_.args.emplace_back( d["args"][i].GetString() );
    }

    return query_;
  }

}