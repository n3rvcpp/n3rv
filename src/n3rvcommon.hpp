#pragma once

#include <algorithm>
#include <functional>
#include <iostream>
#include <optional>
#include <random>
#include <regex>
#include <vector>

#include <zmq.hpp>

namespace n3rv {

template <typename T>
using nullable_ref = std::optional<std::reference_wrapper<T>>;

/** stores deffered connections for later use,
 *  when service becomes available in directory.
 */
struct qdef {
  /** name/lookup of the binding to defer. */
  std::string name;
  /** socket type of connection to defer. */
  int socket_type;
  /** n3rv connection handler ref. */
  /* TODO: Replace this by unique_ptr (at least)*/
  void *hdl;
};

/** qconn aims to store information about
 *  connections and manages zmq sockets. */
struct qconn {
  int type;
  std::unique_ptr<zmq::socket_t> socket;
  int socket_type;
  std::vector<std::string> peers;
};

/**
 * binding structure aims to store configuration for a zmq socket.
 * (socket_type is a direct allusion to zmq socket types.)
 */
struct binding {
  /** n3rv::qserv* parent,
   * stored as void* pointer (to avoid circular refs.)
   TODO: Remove this naked PTR */
  void *parent;
  /** Name of the binding */
  std::string name;
  /** TCP Port of the binding */
  unsigned int port;
  /** ZMQ Socket type for the binding. */
  unsigned int socket_type;
};

/**
 * This struct aims to store the subscribed nodes inside the main directory.
 */
struct qserv {
  /** Namespace of the node. */
  std::string namespace_;
  /** Service Class of the node. */
  std::string service_class;
  /** Node's name. */
  std::string node_name;
  /** Node's IP Address. */
  std::string ip;
  /** List of subscribed bindings so far for the node. */
  std::vector<binding> bindings;
};

/** n3rv receive callbacks signature.
 *  Each callback used with attach() must have the following signature:
 *  static void* mycallback(void* objref, zmq::message_t* msg)
 *  @param objref: backward reference to calling object, a workarround to
 * static/C++ mangling. this pointer is usually casted back in myclass* inside
 * the callback.
 *  @param msg zmq message for which the callback was called. */
typedef void *(*fctptr)(void *, zmq::message_t *);

/** Main loop callbacks signature. */
typedef void *(*mlptr)(void *);

/**
 * Generates a random string of length "length"
 * @param length wished for the random string.
 * @return generated random string.
 */
std::string randstr(size_t length);

/**
 * Tries to Resolve node from directory provided as argument.
 * @param dir Directory map object to look in.
 * @param service_class Class of service to look for.
 * @param node_name Name of the node to look for.
 * @param namespace_ Namespace of the node to look for.
 * @return Directory Node pointer if found, nullptr otherwise.
 *
 * Note about nodes lookup: Lookup terms supports glob expressions (*) in each
 * parameters, if multiple nodes are found then nlookup() picks one of them
 * randomly.
 */
nullable_ref<qserv> node_lookup(std::vector<qserv> &dir,
                                std::string service_class,
                                std::string node_name,
                                std::string namespace_ = "*");

/**
 * Tries to Resolve node from directory provided as argument, given full node
 * addr.
 * @param dir Directory map object to look in.
 * @param addr Node address in format "namespace_.service_class.node_name".
 * @return Directory Node pointer if found, std::nullopt otherwise.
 *
 * Note about nodes lookup: addr supports glob expressions (*) for each term,
 * if multiple nodes are found then nlookup() picks one of them randomly.
 */
nullable_ref<qserv> node_lookup(const std::vector<qserv> &dir,
                                const std::string &addr);

/**
 * Tries to resolve binding from full address.
 * @param dir Directory map object to look in.
 * @param addr Binding address in format
 * "namespace_.service_class.node_name.binding_name"
 * @return Direcotry binding pointer if found, nullptr otherwise.
 *
 * Note about bindings lookup: addr supports glob expressions (*) for each term,
 * if multiple bindings are found then blookup() picks one of them randomly.
 */
nullable_ref<binding> binding_lookup(const std::vector<qserv> &dir,
                                     const std::string &addr);

} // namespace n3rv