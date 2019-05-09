#include "n3rvlogger.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctime>

namespace n3rv {

    std::map<int, std::string> logger::ll_map;
    std::map<std::string,int> logger::fac_map;

    logger::logger(int log_level) {
        this->log_level = log_level;
        logger::ll_map[LOGLV_PANIC] = "PANIC";
        logger::ll_map[LOGLV_ALERT] = "ALERT";
        logger::ll_map[LOGLV_CRIT] = "CRITICAL";
        logger::ll_map[LOGLV_ERR] = "ERROR";
        logger::ll_map[LOGLV_WARN] = "WARNING";
        logger::ll_map[LOGLV_NOTICE] = "NORMAL";
        logger::ll_map[LOGLV_INFO] = "INFO";
        logger::ll_map[LOGLV_DEBUG] = "DEBUG";

        logger::fac_map["local0"] = LOG_LOCAL0;
        logger::fac_map["local1"] = LOG_LOCAL1;
        logger::fac_map["local2"] = LOG_LOCAL2;
        logger::fac_map["local3"] = LOG_LOCAL3;
        logger::fac_map["local4"] = LOG_LOCAL4;
        logger::fac_map["local5"] = LOG_LOCAL5;
        logger::fac_map["local6"] = LOG_LOCAL6;
        logger::fac_map["local7"] = LOG_LOCAL7;
        logger::fac_map["user"] = LOG_USER;

        this->has_syslog = false;

    }

    logger::~logger() {
        if (has_syslog) closelog();
    }

    void logger::add_dest(const char* dest) {

        std::string dst = std::string(dest);

        this->dests.emplace_back(dst);

        if (dst.substr(0,7) == "file://") {
            std::string dpath = std::regex_replace(dst, std::regex("file:\\/\\/"), "");        
            std::ofstream* out = new std::ofstream();
            out->open(dpath);
            this->dest_buffers.emplace_back(out->rdbuf());
        }

        else if ( dst == "stdout") {
            this->dest_buffers.emplace_back(std::cout.rdbuf());
        }

        else if (dst.substr(0,7) == "syslog:") {

            std::vector<std::string> syslog_args;    
            std::regex dotsplit(":");
            std::sregex_token_iterator iter(dst.begin(),
                                            dst.end(),
                                           dotsplit,
                                          -1);
            std::sregex_token_iterator end;
            for ( ; iter != end; ++iter) syslog_args.emplace_back(*iter);     
            //invalid syslog dest
            if (syslog_args.size() != 3) return;
            //invalid facility
            if (  logger::fac_map.find(syslog_args[2]) == logger::fac_map.end() ) return;

            openlog(syslog_args[1].c_str(),LOG_NDELAY,logger::fac_map[syslog_args[2]]);
            this->has_syslog = true;
        }
    }

    void logger::log(int log_level, std::string str) {

        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);

        if ( this->log_level >= log_level ) {

            //syslog
            if (this->has_syslog) {
                syslog(log_level, "%s", str.c_str());
            }

            //files or stdout
            for(auto& dest : this->dest_buffers ) {
                std::ostream out(dest);
                out  << "[" << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << "]" << "[" << this->ll_map[log_level] << "] " << str << std::endl;
                out.flush();
            }

        }

    }

    void logger::set_loglevel(int lvl) {
        this->log_level = lvl;
    }

}