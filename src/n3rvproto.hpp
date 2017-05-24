#ifndef N3RV_PROTO
#define N3RV_PROTO

#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"

namespace n3rv {

  typedef struct n3rvquery_ {

    std::string action;
    std::vector<std::string> args;
    std::string payload;

  } n3rvquery;

  typedef struct n3rvresponse_ {

  } n3rvresponse;

  /**
   * Serializes query for later sending over the net.
   */
  std::string serialize_query(n3rv::n3rvquery& query) {
    
    rapidjson::PrettyWriter writer;

    writer.String("action");
    writer.String(query.action);

    writer.String("payload");
    writer.String(query.payload);

    return writer.str();

  }

  /**
   * Parses a raw query comming from a service and 
   * puts it inside a n3rvquery structure.
   */
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

#endif