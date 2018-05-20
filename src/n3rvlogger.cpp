#include "n3rvlogger.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctime>

namespace n3rv {

    std::map<int, std::string> logger::ll_map;


    logger::logger(int log_level) {

        this->log_level = log_level;
        logger::ll_map[LOGLV_CRIT] = "CRITICAL";
        logger::ll_map[LOGLV_WARN] = "WARNING";
        logger::ll_map[LOGLV_NORM] = "NORMAL";
        logger::ll_map[LOGLV_DEBUG] = "DEBUG";
        logger::ll_map[LOGLV_XDEBUG] = "XDEBUG";

    }

    void logger::add_dest(std::string dst) {

        this->dests.emplace_back(dst);

        if (dst.substr(0,7) == "file://") {
            std::string dpath = std::regex_replace(dst, std::regex("file:\\/\\/"), "");        
            std::ofstream out(dpath);
            this->dest_buffers.emplace_back(out.rdbuf());
        }

        else if ( dst == "stdout") {
            this->dest_buffers.emplace_back(std::cout.rdbuf());
        }

    }

    void logger::log(int log_level, std::string str) {

        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);

        if ( this->log_level >= log_level ) {

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