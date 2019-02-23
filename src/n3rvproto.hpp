#ifndef N3RV_PROTO_HPP
#define N3RV_PROTO_HPP

#include <vector>
#include <map>

#include "n3rvcommon.hpp"

namespace n3rv {

  /** Main n3rv message struct */
  typedef struct message_ {

    /** message sender (automatically filled at send-time) */
    std::string sender;
    /** which action is required to process payload. */
    std::string action;
    /** eventual arguments needed to complete action. */
    std::vector<std::string> args;
    /** message payload. */
    std::string payload;

  } message;

  
  /**
   * Serializes message for later sending over the net as a JSON string.
   * @param msg message to serialize.
   * @return serialized message as string.
   */
  std::string serialize_msg(n3rv::message& msg);

  /**
   * Parses a Protobuf message comming from a service and 
   * puts it inside a n3rvquery structure.
   * @param msgstr message to parse.
   * @return parsed message.
   */
  n3rv::message parse_msg(std::string msgstr);

  /**
   * Parses a Protobuf message directly from zmq message container.
   * @param msg ZMQ message to parse.
   * @return parsed message.
   */ 
  n3rv::message parse_msg(zmq::message_t* msg);


  /**
   * Performs the protobuf-serialization of a services directory. 
   * @param directory Directory to serialize.
   * @return String-serialized directory.
   */
  std::string serialize_directory(std::map<std::string, n3rv::qserv>& directory);

  /**
   * Performs parsing of protobuf dirstring.
   * @param String-serialized directory.
   * @return Unserialized Directory.
   */
  std::map<std::string, n3rv::qserv> parse_directory(std::string dirstr);
  
  void t128bug(std::string& q);

}

#endif