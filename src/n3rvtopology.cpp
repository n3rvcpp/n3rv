#include "n3rvtopology.hpp"
#include <iostream>
#include <fstream>
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"


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
            topo_ct += line + "\n";
         }
         f1.close();

        return topology::parse(topo_ct);

    }


    topology* topology::parse(std::string& topology_def) {

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

                tbinding b;

                b.name = d[topo_][i][binds_][j]["name"].GetString();
                b.port = d[topo_][i][binds_][j]["port"].GetInt();
                b.type = d[topo_][i][binds_][j]["type"].GetString();
                n.bindings.emplace_back(b);

            }

            assert(d[topo_][i][conn_].IsArray());
            for (int j=0;j< d[topo_][i][conn_].Size();j++ ) {

                connection cx;
                cx.name = d[topo_][i][conn_][j]["name"].GetString();
                cx.type = d[topo_][i][conn_][j]["type"].GetString();
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

    std::string topology::serialize() {

        rapidjson::StringBuffer sb; 
        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);

        writer.StartObject();
        writer.String(topo_);

        writer.StartArray();


        for (auto node: this->nodes) {

            std::string sname = node.first;
            auto n = node.second;

            writer.StartObject();
            writer.String("service_class");
            writer.String(sname.c_str());

            writer.String(binds_);

            writer.StartArray();
            for ( auto b_: n.bindings) {
                writer.StartObject();
                writer.String("name");
                writer.String(b_.name.c_str());
                writer.String("port");
                writer.Int(b_.port);
                writer.String("type");
                writer.String(b_.type.c_str());
                writer.EndObject();
            }
            writer.EndArray();

            writer.String(conn_);

            writer.StartArray();
            for ( auto c_: n.connections) {
                writer.StartObject();
                writer.String("name");
                writer.String(c_.name.c_str());
                writer.String("type");
                writer.String(c_.type.c_str());
                writer.EndObject();
            }
            writer.EndArray();

            writer.String(cb_);

            writer.StartArray();
            for ( auto call_: n.callbacks) {
                writer.StartArray();            
                writer.String(call_.connection_name.c_str());       
                writer.String(call_.callback_name.c_str());
                writer.EndArray();
            }
            writer.EndArray();
        
            writer.EndObject();

        }

        writer.EndArray();    
        writer.EndObject();

        return sb.GetString();
        
    }

}

