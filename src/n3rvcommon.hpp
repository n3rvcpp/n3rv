#ifndef N3RV_COMMON
#define N3RV_COMMON

#include <vector>
#include <zmq.hpp>


namespace n3rv {

    /** stores deffered connections for later use, 
     *  when service becomes available in directory.
     */
    typedef struct qdef_ {
        std::string name;
        int socket_type;
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
     * qserv aims to store each services directory entries.
     */
    typedef struct qserv_ {
        std::string service_class;
        std::string ip;
        unsigned int port;
    } qserv;


    /**
     * binding structure aims to store configuration for a zmq socket.
     * (socket_type is a direct allusion to zmq socket types.)
     */
    typedef struct binding_  {
        unsigned int port;
        unsigned int socket_type;
    } binding;


    // Main n3rv callbacks signature.
    typedef void* (*fctptr)(void*, zmq::message_t*);

}

#endif