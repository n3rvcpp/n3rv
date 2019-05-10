#ifndef N3RV_SERVICE_HPP
#define N3RV_SERVICE_HPP

#include <map>
#include <iostream>
#include <sstream>
#include <thread>

#include <zmq.hpp>
#include "n3rvcommon.hpp"
#include "n3rvproto.hpp"
#include "n3rvlogger.hpp"
#include "n3rvtopology.hpp"

namespace n3rv {

  
  /**
   * The main class of the n3rv framework. 
   * Each microservice inside a n3rv infrastructure inherits the service class,
   * and therefore benefits from its functionalities.   
   */
  class service {

   public:

    /** service class constructor.
     *  @controller_host the ip/hostname of the controller.
     *  @controller_port the port number on which the controller is listening (ch1). 
     */
    service(const char* controller_host, 
            int controller_port,
            logger* ll = nullptr);

    /** service class destructor, 
     *  closes all open zmq sockets and zmq context.
     */
    ~service();
      
    logger* ll;

    /** Service class initializer. Empty for base service class, but inheriting classes
     *  can use it to initialize new connections and make attachements.
     */
    virtual int initialize();

    /** Sets UID for node. This method MUST be imperatively called before any bind() operation. 
     *  @param namespace_ namespace choosen for the service.
     *  @param service_class service class (usually you can choose the service-inhereted class name)
     *  @param name Name of the node.
     */
    void set_uid(const char* namespace_, const char* service_class, const char* name);

    /** Sets UID for node. This method MUST be imperatively called before any connect()/binding action. 
     *  @param uid Node UID, at the following format "namespace_.service_class.node_name"
     */
    void set_uid(const char* uid);


    /** Registers the service on the controller, 
     *  for it to update its directory and advise others a new node is available.
     *  This method is called automatically by bind() on TCP sockets, except specific cases
     *  you don't need to call it yourself.
     *  @param binding_name Name of the binding to register.
     *  @param port port of the binding to register.
     */
    int subscribe(const char* binding_name, int port);


    /** Gracefuly Unregisters the service from the controller,
     *  in case the service must go down. Not very useful, because the ZMQ stack takes care of this.
     */
    int unsubscribe();

    /** Starts the service. run() Basically manages all the established connections, 
     * plus eventual extra behaviour coded inside hkloop(). 
     */
    int run();

    /** Stops the main running loop.*/
    void stop();

    /** Start the service asynchronously, by running it in its own dedicated thread. */
    std::thread* run_async();

    /** Closes all sockets and destroys ZMQ context */
    int terminate();

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
      * Another, more elaborate mechanism to add extra code in the running loop.
      * callbacks added with this function get executed for every loop iteration.
      * @param cbid callback identifier.
      * @param cb callback function pointer.
      */
     void register_main(const char* cbid, mlptr cb);
     
     /**
      *  used to unregister a previously regestered main loop callback
      *  @param cbid callback identifier.
      *  @return 0 if unregister was succesful, >0 otherwise.*/
     int unregister_main(const char* cbid);


    /** This method is used essentially to map callback function pointers to an identifier string.
     *  Can be very useful for text-defined topologies.
     */     
    virtual void map_callbacks();

    /**
     * starts a new connection (zmq_socket) to a remote service available inside the directory.
     * @param name identifier of the remote service inside the directory.
     * @param connection_type Type of connection to the remote host, directly related to ZMQ (ZMQ_REQ, ZMQ_SUB, etc..). 
     * @param hdlref optional, used for internal purpose when we have to reconnect using existing qhandler object.
     * @return n3rv connection handler.
     */
    qhandler* connect(const char* name, int connection_type, qhandler* hdlref = nullptr);

    /**
     * Binds A NEW ZMQ TCP Socket (main endpoint type supported by n3rv)
     * @param bind_name Name of the binding.
     * @param ip ip to listen on (0.0.0.0 to listen on all addr).
     * @param bind_type ZMQ socket type to create (ZMQ_REP, ZMQ_PUB,..)
     * @param port TCP port to listen on.
     * 
     * Note: If port is set to 0, n3rv will try to find a randomly choosen, available port on the machine.
     * (Not very firewall-friendly but can solve a few headaches).
     *
     * @return n3rv connection handler.
     */
    qhandler* bind(const char* bind_name, const char* ip , int bind_type, int port = 0 );

    /**
     * binds a new RAW ZMQ socket if the service needs a listening socket (ZMQ_REP, ZMQ_PUB, etc..)
     * @param bind_name name of the bound connection, to identify it.
     * @param endpoint string, to tell on what parameters to bind the socket.
     * @param type kind of ZMQ socket to bind: ZMQ_REP, ZMQ_PUB, etc..
     * @return n3rv connection handler.
     */
    qhandler* zbind(const char* bind_name, const char* endpoint, int bind_type );

    /**
     * Allows to deal with underlying ZeroMQ stack and set socket options for a given connection
     * @param hdl n3rv connection handler pointer
     * @param option_name ZMQ option to set.
     * @param option_value ZMQ value to give to option.
     * @param option_len size of option value.
     */
    void zsockopt(qhandler* hdl, int option_name, const void* option_value, size_t option_len = -1 );

    /** Attaches a service connection to its message handler callback !
     *  @param hdl n3rv connection handler, created with connect(), bind() or zbind().
     *  @param connection_name Name of the connection to attach callback to.
     *  @callback callback function.
     *  @return 0 if attach went correctly, >0 otherwise. 
     */
    int attach(qhandler* hdl, fctptr callback);

    /** Attaches a service connection to its message handler callback !
     *  Warning: this method only works if service::cbmap was filled at map_callbacks() time.
     *  @param connection_name Name of the connection to attach callback to.
     *  @param callback_name string name of the callback to atach to connection.
     *  @return 0 if attach went correctly, >0 otherwise.
     */ 
    int attach(qhandler* hdl, std::string callback_name);

    /* Tries to download the topology from the service controller, and if available 
     * automatically bind ports, connects to remote endpoints and attach callbacks. 
     * @return a dictionary of uid/qhandler* key/values*/
    std::map<std::string, qhandler*> fetch_topology();

    /** Loads a topology file and automatically bind ports, 
     * connects to remote endpoints and attach callbacks 
     * @return a dictionary of uid/qhandler* key/values*/
    std::map<std::string, qhandler*> load_topology(std::string topology_file);

    /** Uses a previously defined topology object to automatically bind ports, 
     * connects to remote endpoints and attach callbacks.
     * @return a dictionary of uid/qhandler* key/values */
    std::map<std::string, qhandler*> load_topology(topology* topo);

    /** Retrieves a RAW ZMQ socket from the internal connections list.
     *  @param hdl n3rv connection handler.
     *  @return the related connection object. */
    zmq::socket_t* get_zsocket(qhandler* hdl);

    /** Conveniency function to send string data on a specified connection. 
     *  @param hdl n3rv connection handler to send data to.
     *  @param data string to send.
     *  @param flags ZMQ send flags.
     */
    int send(qhandler* hdl, std::string& data, int flags);

    /** Conveniency function to send raw data on a specified connection. 
     *  @param hdl n3rv connection handler to send data to.
     *  @param data memory pointer of data to send.
     *  @param size size of the memory data block.
     *  @param flags ZMQ send flags.
     */
    int send(qhandler* hdl, void* data, size_t size, int flags);

    /** Conveniency function to send n3rv::mesage data on a specified connection. 
     *  @param hdl n3rv connection handler to send data to.
     *  @param msg n3rv::message to send.
     *  @param flags ZMQ send flags.
    */
    int send(qhandler* hdl, message& msg, int flags);

    /** Conveniency function to send direct zmq::message_t data on a specified connection. 
     *  @param hdl n3rv connection handler to send data to.
     *  @param msg ZMQ message_t to send.
     *  @param flags ZMQ send flags. 
     */
    int send(qhandler* hdl, zmq::message_t* zmsg, int flags);


    /** Checks for deferred connections whose endpoint 
     * was not yet in directory, and tries to establish connection. */
    int check_deferred();

    /** Sets the main ZMQ polling timeout, if required 
     *  @param poll_timeout time (in ms) the poller must wait before returning.
     */
    void set_poll_timeout(int poll_timeout);

  protected:
   
    bool running;

    qhandler* ctlr_ch1;
    qhandler* ctlr_ch2;

    zmq::pollitem_t* refresh_pollitems();
    int poll_timeout;
    
    /** Directory updates message handling callback */
    static void* directory_update(void* objref, zmq::message_t* dirmsg);

    /** Controller connection procedure */
    int connect_controller();

    /** prepends an uid with missing scope parts */
    std::string add_scope(const char* name);

    std::string namespace_;
    std::string service_class;
    std::string name;
    std::string controller_host;
    int controller_port;
    std::vector<n3rv::qserv> directory;
    std::map<std::string, n3rv::qconn> connections;
    zmq::context_t zctx;


   std::map<std::string, fctptr> cbmap;
   std::map<std::string, fctptr> chmap;

   /** Main loop callbacks dictionary */
   std::map<std::string, mlptr> ml_chmap;

   int last_nconn;
   std::vector<std::string> last_connlist;
   std::vector<n3rv::qdef> deferred;
  
  };

}

#endif
