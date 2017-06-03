#ifndef N3RV_COMMON
#define N3RV_COMMON

#include <vector>
#include <zmq.hpp>


namespace n3rv {

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
        unsigned int port;
    } qserv;


    //WE WILL PROBABLY GEt RID OF SERVICE CLASSES AND BIDINGS, BECAUSE TOPOLOGY WILl BE HARD-CODED INTO SERVICES.

    /**
     * binding structure aims to store configuration for a zmq socket.
     * (socket_type is a direct allusion to zmq socket types.)
     */
    typedef struct binding_  {
        unsigned int port;
        unsigned int socket_type;
    } binding;


    /**
     * service_class structure aims to declare 
     * a new service class inside a topology.
     */
    typedef struct service_class_ {

        std::string name;
        std::string connects_to;
        std::vector<n3rv::binding> binds;

    } service_class;


    typedef void* (*fctptr)(void*, zmq::message_t*);

}

#endif