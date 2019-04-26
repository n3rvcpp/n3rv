#include "n3rvcommon.hpp"
#include <iostream>

namespace n3rv {

    qserv* nlookup(std::vector<qserv>& dir, std::string addr) {

        std::vector<qserv*> rrlist;
        std::string lookup_str = regex_replace(addr, 
                                               std::regex("\\*"), 
                                               "(.*?)", 
                                                std::regex_constants::match_any);

        for(int i=0;i< dir.size(); i++) {         
            std::string absname = dir[i].namespace_ + "." + dir[i].service_class + "." + dir[i].node_name;   

            if (regex_search(absname,std::regex(lookup_str))) {
                rrlist.emplace_back(&(dir[i]));
            }
        }

        if (rrlist.size() == 1) return rrlist[0];
        else if (rrlist.size() > 1) {
            //Rand Node Lookup
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

    blookup_res blookup(std::vector<qserv>& dir, 
                      std::string addr) {

          blookup_res result;
          result.ip = "";
          result.bind = nullptr;

          //extracts binding name
          int bpos = 0;
          for(int i=addr.length()-1;i>=0;i--) {
              if (addr[i] == '.') {
                bpos = i;
                break;
              }
          }

          if (bpos == 0) {
              return result;
          }

          std::string naddr = addr.substr(0,bpos);
          std::string bname = addr.substr(bpos+1, addr.length() - bpos);

          qserv* s = nlookup(dir, naddr);

          if (s == nullptr) {
              return result;
          }

          for (int i =0;i < s->bindings.size(); i++ ) {
            
            if (s->bindings[i].name == bname) {             
                    result.ip = s->ip;
                    result.bind = &(s->bindings[i]);
                    break;
            }
          }

          return result;
    }

}