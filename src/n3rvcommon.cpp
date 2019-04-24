#include "n3rvcommon.hpp"

namespace n3rv {

    qserv* nlookup(std::vector<qserv>& dir, std::string addr) {

        std::vector<qserv*> rrlist;


        std::string lookup_str = regex_replace(addr, std::regex("\\*"), "(.*)");
        for(auto n: dir) {         
            std::string absname = n.namespace_ + "." + n.service_class + "." + n.node_name;   
            if (regex_search(absname,std::regex(lookup_str))) {
                rrlist.emplace_back(&n);
            }
        }

        if (rrlist.size() == 1) return rrlist[0];
        else if (rrlist.size() > 1) {
            //Round Robin Node Lookup
            srand(time(NULL));
            int n_index = rand() % rrlist.size() + 0;
            return rrlist[n_index];
        }
        else return nullptr;
    }

    qserv* nlookup(std::vector<qserv>& dir, 
                std::string service_class, 
                std::string node_name, 
                std::string namespace_ ) {

        
        std::string lookup_str = namespace_ + "." + service_class + "." + node_name;
        return nlookup(dir, lookup_str);
        
    }

    binding* blookup(std::vector<qserv>& dir, 
                      std::string addr) {

          //extracts binding name
          int bpos = 0;
          for(int i=addr.length()-1;i>=0;i--) {
              if (addr[i] == '.') {
                bpos = i;
                break;
              }
          }

          if (bpos == 0) return nullptr;

          std::string naddr = addr.substr(0,bpos-1);
          std::string bname = addr.substr(bpos, addr.length() - bpos);
          qserv* s = nlookup(dir, naddr);

          if (s == nullptr) return nullptr;

          for (auto b: s->bindings) {
            b.parent  = s;
            if (b.name == bname) {
                    return &b;
            }
          }

          return nullptr;
    }

}