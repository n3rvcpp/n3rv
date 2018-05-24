#include "n3rvtopology.hpp"
#include <iostream>
#include <fstream>

#define topo_ "topology"
#define binds_ "binds" 
#define conn_ "connects"
#define cb_ "callbacks"

namespace n3rv {

    topology* topology::load(std::string topology_file) {

        std::ifstream f1(topology_file.c_str());
        std::string topo_ct = "";

         std::string line;
         while(f1.good()){
            getline(f1,line);
            topo_ct += line;
         }
         f1.close();

        return topology::parse(topo_ct);

    }


    topology* topology::parse(std::string topology_def) {

        topology* t1  = new topology();

        rapidjson::Document d;
        d.Parse<0>(topology_def.c_str()); 

        assert(d.IsObject());
        assert(d[topo_].IsArray());

        for (int i=0;i< d[topo_].Size();i++) {

            node n;

            assert(d[topo_][i].IsObject());
            assert(d[topo_][i][binds_].IsArray());

            for (int j=0;j< d[topo_][i][binds_].Size();j++ ) {

                binding b;

                b.name = d[topo_][i][binds_][j]["name"].GetString();
                b.port = d[topo_][i][binds_][j]["port"].GetInt();
                b.type = d[topo_][i][binds_][j]["type"].GetString();
                n.bindings.emplace_back(b);

            }

            assert(d[topo_][i][conn_].IsArray());
            for (int j=0;j< d[topo_][i][conn_].Size();j++ ) {

                connection cx;
                cx.name = d[topo_][i][binds_][j]["name"].GetString();
                cx.type = d[topo_][i][binds_][j]["type"].GetString();
                n.connections.emplace_back(cx);

            }

            assert(d[topo_][i][cb_].IsArray());
            for (int j=0;j< d[topo_][i][cb_].Size();j++ ) {

                callback c;
                c.connection_name = d[topo_][i][cb_][j][0].GetString();
                c.callback_name = d[topo_][i][cb_][j][1].GetString();
                n.callbacks.emplace_back(c);

            }

            t1->nodes[d[topo_][i]["service_class"].GetString()] = n;

        }
        return t1;
    }
}

