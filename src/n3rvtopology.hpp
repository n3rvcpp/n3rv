#ifndef __N3RV_TOPOLOGY__
#define __N3RV_TOPOLOGY__

#include <map>
#include <vector>

namespace n3rv {

    typedef struct tbinding_ {

        public:
            std::string name;
            int port;
            std::string type;

    } tbinding;


    typedef struct connection_ {
        public:
            std::string name;
            std::string type;

    } connection;

    typedef struct callback_ {
        public:
            std::string connection_name;
            std::string callback_name;

    } callback;

    /** Node structure allows to describe a full service class,
     *  with all its binding, connections and callback fucntions.
     */
    typedef struct node_ {

        public:
            /** List of ports a service class must bind. */
            std::vector<tbinding> bindings;
            /** List of connections to remote endpoints 
             *  a service class must establish. */
            std::vector<connection> connections;
            /** List of [connection, callback] attacchement, 
             *  which declare how to process messages. */
            std::vector<callback> callbacks;
    } node;



    /** This class is meant to store and manimulate topology representations, 
     *  in order to help services dynamically know which endpoints to bind or contact. */
    class topology {

    public:

        /** Dictionary of Nodes, mapped with their service class. */
        std::map<std::string, node> nodes;

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
