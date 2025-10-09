#pragma once

#include <zmq.hpp>

#include <iostream>
#include <sstream>
#include <thread>
#include <unistd.h>

#include "n3rvcommon.hpp"
#include "n3rvlogger.hpp"
#include "n3rvproto.hpp"
#include "n3rvtopology.hpp"
#include "n3rvzmq.hpp"
#include <map>

namespace n3rv {

/**
 * servicecontroller is the directory service that:
 * - Maintains the full list of available endpoints.
 * - Propagates the available endpoints to every online nodes.*/
class servicecontroller {

public:
  /** service controller class constructor.
   *  @param binding_addr Ip address the controller must listen to (0.0.0.0 for
   * listen all)
   *  @param binding_port TCP port to bind controller on.
   */
  servicecontroller(const std::string &binding_addr, unsigned int binding_port,
                    nullable_ref<logger> = std::nullopt);

  ~servicecontroller();

  /** Loads a topology file for the service controller to distribute */
  int load_topology(std::string path);

  /** Runs the service controller once instanciated. */
  void run();

  /** Stops the service controller main loop. */
  void stop();

  /** Asynchronously runs the service controller, returns a handler for the
   * running thread.*/
  std::thread *run_async();

  /** Stops the binding of CH1/CH2 sockets. */
  void terminate();

  nullable_ref<logger> ll;

  /* In case the service controller port was automatically allocated. */
  int get_port();

protected:
  /** Retrieves the sender's ip in order to advertise endpoint
   *  @param zmsg originating ZMQ message.
   *  @return send's ip string. */
  std::string peer_ip(zmq::message_t *zmsg);
  bool running;
  std::string binding_addr;
  unsigned int binding_port;
  zmq::context_t zctx;
  std::unique_ptr<zmq::socket_t> zmsock;
  std::unique_ptr<zmq::socket_t> zmsock_pub;
  std::vector<n3rv::qserv> directory;
  std::optional<topology> topo{std::nullopt};

  int init_bindings(int binding_port);
};

struct scioctl {
  std::thread *th;
  servicecontroller *ctl;
};

/** Conveniency function that runs a service controller instance inside its own
 * thread, so you can embed a service controller along with another service
 * easilly.
 *  @param binding_addr Ip address the controller must listen to (0.0.0.0 for
 * listen all)
 *  @param binding_port TCP port to bind controller on.
 *  @param display_out tells if service controller must display logs on stdout
 * or not.
 *  @param log_level Tells the log level of the service controller.
 *  @return scioctl struct ptr containing service controller ref along with
 * running thread ref.
 */
scioctl *start_controller(const char *binding_addr, unsigned int binding_port,
                          nullable_ref<logger> ll = std::nullopt) {

  scioctl *res = new scioctl;

  res->th = new std::thread([binding_addr, binding_port, ll, res]() {
    res->ctl = new servicecontroller(binding_addr, binding_port, ll);
    res->ctl->run();
  });

  res->th->detach();
  return res;
}

} // namespace n3rv