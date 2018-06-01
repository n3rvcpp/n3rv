#ifndef N3RV_SERVICE_HPP
#define N3RV_SERVICE_HPP

#include <map>
#include <iostream>
#include <sstream>

#include <zmq.hpp>
#include "n3rvcommon.hpp"
#include "n3rvproto.hpp"
#include "n3rvlogger.hpp"
#include "n3rvtopology.hpp"


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
     */
    service(std::string name, 
            std::string service_class, 
            std::string controller_host, 
            int controller_port);
    
    logger* ll;


    /** Service class initializer. Empty for base service class, but inheriting classes
     *  can use it to initialize new connections and make attachements.
     */
    virtual int initialize();


    /** service class destructor. */
    ~service();

    /** Registers the service on the controller, 
     *  for it to update its directory and advise others a new node is available.
     *  This method is called automatically by bind() on TCP sockets, except specific cases
     *  you don't need to call it yourself.
     *  @param name Name of the binding to register.
     *  @param sclass service class of the binding to register.
     *  @param port port of the binding to register.
     */
    int subscribe(std::string name, std::string sclass, int port);


    /** Gracefuly Unregisters the service from the controller,
     *  in case the service must go down. Not very useful, because the ZMQ stack takes care of this.
     */
    int unsubscribe();

    /** Starts the service. run() Basically manages all the established connections, 
     * plus eventual extra behaviour coded inside hkloop(). 
     */
    int run();

    /** Start the service asynchronously, by running it in its own dedicated thread. */
    int run_async();

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
    
    /** This method is used essentially to map callback function pointers to an identifier string.
     *  Can be very useful for text-defined topologies.
     */     
    virtual void map_callbacks();

    /**
     * starts a new connection (zmq_socket) to a remote service available inside the directory.
     * @param name identifier of the remote service inside the directory.
     * @param connection_type Type of connection to the remote host, directly related to ZMQ (ZMQ_REQ, ZMQ_SUB, etc..). 
     */
    int connect(std::string name, int connection_type);

    /**
     * Binds A NEW ZMQ TCP Socket (main endpoint type supported by n3rv)
     * @param bind_name Name of the binding.
     * @param ip ip to listen on (0.0.0.0 to listen on all addr).
     * @param port TCP port to listen on.
     * @param bind_type ZMQ socket type to create (ZMQ_REP, ZMQ_PUB,..)
     * 
     * Note: If port is set to 0, n3rv will try to find a randomly choosen, available port on the machine.
     * (Not very firewall-friendly but can solve a few headaches).
     */
    int bind(std::string bind_name, std::string ip, int port , int bind_type );

    /**
     * binds a new RAW ZMQ socket if the service needs a listening socket (ZMQ_REP, ZMQ_PUB, etc..)
     * @param bind_name name of the bound connection, to identify it.
     * @param endpoint string, to tell on what parameters to bind the socket.
     * @param type kind of ZMQ socket to bind: ZMQ_REP, ZMQ_PUB, etc..
     */
    int bind(std::string bind_name, std::string endpoint, int bind_type );


    /** Attaches a service connection to its message handler callback !
     *  @param connection_name Name of the connection to attach callback to.
     *  @callback callback function. 
     */
    int attach(std::string connection_name, fctptr callback);

    /** Attaches a service connection to its message handler callback !
     *  Warning: this method only works if service::cbmap was filled at map_callbacks() time.
     *  @param connection_name Name of the connection to attach callback to.
     *  @param callback_name string name of the callback to atach to connection.
     */ 
    int attach(std::string connection_name, std::string callback_name);

    /* Tries to download the topology from the service controller, and if available 
     * automatically bind ports, connects to remote endpoints and attach callbacks. */
    int fetch_topology();

    /** Loads a topology file and automatically bind ports, 
     * connects to remote endpoints and attach callbacks */
    int load_topology(std::string topology_file);

    /** Uses a previously define topology object to automatically bind ports, 
     * connects to remote endpoints and attach callbacks */
    int load_topology(topology* topo);

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

    /** Conveniency function to send n3rv::mesage data on a specified connection. 
     *  @param connection_name name of the connection to send data to.
     *  @param msg n3rv::message to send.
     *  @param flags ZMQ send flags.
    */
    int send(std::string connection_name, message& msg, int flags);

    /** Conveniency function to send direct zmq::message_t data on a specified connection. 
     *  @param connection_name name of the connection to send data to.
     *  @param msg ZMQ message_t to send.
     *  @param flags ZMQ send flags. 
     */
    int send(std::string connection_name, zmq::message_t* zmsg, int flags);


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
   std::map<std::string, n3rv::qserv> directory;
   std::map<std::string, n3rv::qconn> connections;
   zmq::context_t zctx;


   std::map<std::string, fctptr> cbmap;
   std::map<std::string, fctptr> chmap;

   int last_nconn;
   std::vector<std::string> last_connlist;
   std::vector<n3rv::qdef> deferred;

  };

}

#endif
