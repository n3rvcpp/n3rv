#ifndef N3RV_COMMON
#define N3RV_COMMON

#include <vector>
#include <zmq.hpp>
#include <regex>


namespace n3rv {

     /** Connction handler returned by connect() 
      *  or bind(), used for send() operations. */
    typedef struct qhandler_ {
        /** randomly generated connection id */
        std::string cid;
        /** full peer node uid after lookup (filled only for connect() operations )*/
        std::string peer_uid;
    } qhandler;

    /** stores deffered connections for later use, 
     *  when service becomes available in directory.
     */
    typedef struct qdef_ {
        /** name/lookup of the binding to defer. */
        std::string name;
        /** socket type of connection to defer. */
        int socket_type;
        /** n3rv connection handler ref. */
        qhandler* hdl;
    } qdef;

    /** qconn aims to store information about 
     *  connections and manages zmq sockets. */
    typedef struct qconn_ {
      int type;
      zmq::socket_t* socket;
      int socket_type;
      std::vector<std::string> peers;
    } qconn;

     /**
     * binding structure aims to store configuration for a zmq socket.
     * (socket_type is a direct allusion to zmq socket types.)
     */
    typedef struct binding_  {
        /** n3rv::qserv* parent, stored as void* pointer (to avoid circular refs.) */
        void* parent;
        /** Name of the binding */
        std::string name;
        /** TCP Port of the binding */
        unsigned int port;
        /** ZMQ Socket type for the binding. */
        unsigned int socket_type;
    } binding;

    /**
     * This struct aims to store the subscribed nodes inside the main directory.
     */
    typedef struct qserv_ {
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
    } qserv;

    /** n3rv receive callbacks signature.
     *  Each callback used with attach() must have the following signature:
     *  static void* mycallback(void* objref, zmq::message_t* msg)
     *  @param objref: backward reference to calling object, a workarround to static/C++ mangling. 
     *                 this pointer is usually casted back in myclass* inside the callback.
     *  @param msg zmq message for which the callback was called. */
    typedef void* (*fctptr)(void*, zmq::message_t*);

    /** Main loop callbacks signature. */
    typedef void* (*mlptr)(void*);
 

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
     * Note about nodes lookup: Lookup terms supports glob expressions (*) in each parameters,
     * if multiple nodes are found then nlookup() picks one of them randomly.
     */
    qserv* nlookup(std::vector<qserv_>& dir, std::string service_class, std::string node_name, std::string namespace_ = "*" );
    
    /**
     * Tries to Resolve node from directory provided as argument, given full node addr.
     * @param dir Directory map object to look in.
     * @param addr Node address in format "namespace_.service_class.node_name".
     * @return Directory Node pointer if found, nullptr otherwise.
     * 
     * Note about nodes lookup: addr supports glob expressions (*) for each term,
     * if multiple nodes are found then nlookup() picks one of them randomly.
     */
    qserv* nlookup(std::vector<qserv_>& dir, std::string addr);

    /** 
     * Tries to resolve binding from full address.
     * @param dir Directory map object to look in.
     * @param addr Binding address in format "namespace_.service_class.node_name.binding_name"
     * @return Direcotry binding pointer if found, nullptr otherwise.
     * 
     * Note about bindings lookup: addr supports glob expressions (*) for each term,
     * if multiple bindings are found then blookup() picks one of them randomly.
     */
    binding* blookup(std::vector<qserv>& dir, std::string addr);


}

#endif