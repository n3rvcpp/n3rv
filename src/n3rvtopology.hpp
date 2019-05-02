#ifndef __N3RV_TOPOLOGY__
#define __N3RV_TOPOLOGY__

#include <map>
#include <vector>

namespace n3rv {

    typedef struct jbinding_ {

        public:
            std::string binding_name;
            int port;
            std::string type;

    } jbinding;


    typedef struct jconnection_ {
        public:
            std::string uid;
            std::string lookup;
            std::string type;

    } jconnection;

    typedef struct jcallback_ {
        public:
            std::string uid;
            std::string callback_name;

    } jcallback;

    /** service_class struc allows to describe a full service class,
     *  with all its binding, connections and callback fucntions.
     */
    typedef struct jservice_class_ {

        public:

            /** Namespace of the service class. */
            std::string namespace_;

            /** List of bindings the service class must create. */
            std::vector<jbinding> bindings;
            /** List of connections to remote endpoints 
             *  a service class must establish. */
            std::vector<jconnection> connections;
            /** List of [connection, callback] attacchement, 
             *  which declare how to process messages. */
            std::vector<jcallback> callbacks;
    } jservice_class ;

    /** This class is meant to store and manimulate topology representations, 
     *  in order to help services dynamically know which endpoints to bind or contact. */
    class topology {

    public:

        /** Map of available service classes inside topology. */
        std::map<std::string, jservice_class> svclasses;

        /** Loads a topology from a JSON-formatted topology file. 
         *  @param topology_file Path of the topology file to load.
         *  @returns a topology object.
        */
        static topology* load(std::string topology_file);
        /** Parses a JSON-formatted string into a topology object. 
         *  @param topology_def JSON-serialized Topology String.
         *  @returns a topology object. 
         */
        static topology* parse(std::string& topology_def);

        /** Serializes current topology object to a JSON-string 
         *  @returns JSON-serialized topology object.
         */
        std::string serialize();


        /** class constructor */
        topology() {}

        /** class destructor */
        ~topology() {}

    };


}

#endif
