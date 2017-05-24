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

    return query_;

  }

}

#endif