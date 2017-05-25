#ifndef N3RV_PROTO_HPP
#define N3RV_PROTO_HPP

#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include <vector>

namespace n3rv {

  typedef struct n3rvquery_ {

    std::string sender;
    std::string action;
    std::vector<std::string> args;
    std::string payload;

  } n3rvquery;

  typedef struct n3rvresponse_ {

  } n3rvresponse;

  /**
   * Serializes query for later sending over the net.
   */
  std::string serialize_query(n3rv::n3rvquery& query);

  /**
   * Parses a raw query comming from a service and 
   * puts it inside a n3rvquery structure.
   */
  n3rv::n3rvquery parse_query(std::string query);
}

#endif