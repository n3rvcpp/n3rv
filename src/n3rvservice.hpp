#ifndef N3RV_SERVICE_HPP
#define N3RV_SERVICE_HPP

#include <map>
#include <iostream>
#include <sstream>

#include <zmq.hpp>
#include "n3rvcommon.hpp"
#include "n3rvproto.hpp"
#include "n3rvlogger.hpp"


#define CTLR_CH1 "controller_ch1"
#define CTLR_CH2 "controller_ch2"

namespace n3rv {


  /**
   * The main class of the n3rv framework. 
   * Each microservice inside a n3rv infrastructure inherits the service class,
   * and therefore benefits from its functionalities.   
   */
  class service {

   public:

    /** service class constructor.
     *  @param name The name of the service, acts as an identifier inside the cluster. 
     *  @param service_class basically describes the type of node for the instanciated service.
     *  @controller_host the ip/hostname of the controller.
     *  @controller_port the port number on which the controller is listening (ch1). 
     *  @log_level chooses the log level at which to start service.
     */
    service(std::string name, 
            std::string service_class, 
            std::string controller_host, 
            int controller_port, 
            int service_port
            );
    
    logger* ll;


    /** Service class initializer. Empty for base service class, but inheriting classes
     *  can use it to initialize new connections and make attachements.
     */
    virtual int initialize();


    /** service class destructor. */
    ~service();

    /** Registers the service on the controller, 
     *  for it to update its directory and advise others a new node is available.
     */
    int subscribe();

    /** Gracefuly Unregisters the service from the controller,
     *  in case the service must go down. Not very useful, because the ZMQ stack takes care of this.
     */
    int unsubscribe();

    /** Main service's loop. basically manages all the established connections, 
     * plus eventual extra behaviour coded inside hkloop(). 
     */
    int run();

    /**
     * hkloop is a "hook" function allowing to place some code inside a service's main loop.
     * Indeed, hkloop() is called inside the run() process, so if you need to run code inside the main loop,
     * you can override hkloop() with your own code.
     * 
     * Important Note: NEVER PERFORM BLOCKING OPERATIONS inside hkloop(), 
     * since it will affect the rest of the service's execution. If you really need to
     * perform blocking, synchronous operations, please do them in a separate thread/process.
     */
     virtual void hkloop();
    
    /**
     * starts a new connection (zmq_socket) to a remote service available inside the directory.
     * @param name identifier of the remote service inside the directory.
     * @param connection_type Type of connection to the remote host, directly related to ZMQ (ZMQ_REQ, ZMQ_SUB, etc..). 
     */
    int connect(std::string name, int connection_type);

    /**
     * binds a new ZMQ socket if the service needs a listening socket (ZMQ_REP, ZMQ_PUB, etc..)
     * @param bind_name name of the bound connection, to identify it.
     * @param endpoint string with form "ip:port", to tell on what parameters to bind the socket.
     * @param type kind of ZMQ socket to bind: ZMQ_REP, ZMQ_PUB, etc..
     */
    int add_bind(std::string bind_name, std::string endpoint, int bind_type );

    /** Attaches a service connection to its message handler callback !
     *  @param connection_name Name of the connection to attach callback to.
     *  @callback callback function. 
     */
    int attach(std::string connection_name, fctptr callback);


    /** Retrieves a service connection from the internal connections list.
     *  @param connection_name name of the connection to retrieve.
     *  @return the related connection object. */
    n3rv::qconn& get_connection(std::string connection_name);

    /** Conveniency function to send string data on a specified connection. 
     *  @param connection_name name of the connection to send data to.
     *  @param data string to send.
     *  @param flags ZMQ send flags.
     */
    int send(std::string connection_name, std::string& data, int flags);

    /** Conveniency function to send raw data on a specified connection. 
     *  @param connection_name name of the connection to send data to.
     *  @param data memory pointer of data to send.
     *  @param size size of the memory data block.
     *  @param flags ZMQ send flags.
     */
    int send(std::string connection_name, void* data, size_t size, int flags);

    /** Checks for deferred connections whose endpoint 
     * was not yet in directory, and tries to establish connection. */
    int check_deferred();

  protected:
   
    zmq::pollitem_t* refresh_pollitems();
    
    /** Directory updates message handling callback */
    static void* directory_update(void* objref, zmq::message_t* dirmsg);

   std::string service_class;
   std::string name;
   std::string controller_host;
   int controller_port;
   int service_port;
   std::map<std::string, n3rv::qserv> directory;
   std::map<std::string, n3rv::qconn> connections;

   zmq::context_t zctx;

   std::map<std::string, fctptr> chmap;

   int last_nconn;
   std::vector<std::string> last_connlist;
   std::vector<n3rv::qdef> deferred;

  };

}

#endif
