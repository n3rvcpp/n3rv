#ifndef __N3RV_TOPOLOGY__
#define __N3RV_TOPOLOGY__

#include "rapidjson/document.h"
#include <map>
#include <vector>

namespace n3rv {

    typedef struct binding_ {

        public:
            std::string name;
            int port;
            std::string type;

    } binding;


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

    typedef struct node_ {

        public:
            std::vector<binding> bindings;
            std::vector<connection> connections;
            std::vector<callback> callbacks;
    } node;




    class topology {

    public:
        std::map<std::string, node> nodes;

        static topology* load(std::string topology_file);
        static topology* parse(std::string topology_def);

        topology() {}

    };


}

#endif
