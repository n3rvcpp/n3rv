#ifndef N3RV_PROTO_HPP
#define N3RV_PROTO_HPP

#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include <vector>
#include <map>

#include "n3rvcommon.hpp"

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
   * Serializes query for later sending over the net as a JSON string.
   */
  std::string serialize_query(n3rv::n3rvquery& query);

  /**
   * Parses a JSON query comming from a service and 
   * puts it inside a n3rvquery structure.
   */
  n3rv::n3rvquery parse_query(std::string query);


  /**
   * Performs the JSON-serialization of a services directory. 
   */
  std::string serialize_directory(std::map<std::string, n3rv::qserv>& directory);

}

#endif