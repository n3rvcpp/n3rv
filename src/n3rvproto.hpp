#ifndef N3RV_PROTO_HPP
#define N3RV_PROTO_HPP

#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include <vector>
#include <map>

#include "n3rvcommon.hpp"

namespace n3rv {

  typedef struct message_ {

    std::string sender;
    std::string action;
    std::vector<std::string> args;
    std::string payload;

  } message;

  
  /**
   * Serializes message for later sending over the net as a JSON string.
   */
  std::string serialize_msg(n3rv::message& msg);

  /**
   * Parses a JSON message comming from a service and 
   * puts it inside a n3rvquery structure.
   */
  n3rv::message parse_msg(std::string query);

  /**
   * Parses a JSON query directly from zmq message container.
   */ 
  n3rv::message parse_msg(zmq::message_t* msg);


  /**
   * Performs the JSON-serialization of a services directory. 
   */
  std::string serialize_directory(std::map<std::string, n3rv::qserv>& directory);

  /**
   * Performs parsing of JSON dirstring.
   */
  std::map<std::string, n3rv::qserv> parse_directory(std::string dirstr);
  
}

#endif