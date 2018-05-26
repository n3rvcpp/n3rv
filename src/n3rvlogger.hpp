#ifndef __N3RVLOGGER__
#define __N3RVLOGGER__

#include <iostream>
#include <string>
#include <vector>
#include <regex>

namespace n3rv {

    enum LOGLV {    
        LOGLV_CRIT,
        LOGLV_WARN, 
        LOGLV_NORM,
        LOGLV_DEBUG,
        LOGLV_XDEBUG
    };


    /**
     * n3rv logging class
     * Allows to produce timestamped log inside files (so far, syslog udp/512 also planned). 
     */
    class logger {

        public:

            /** Adds a destination for the log stream.
             *  @param dest destination of the logs, eg "file:///var/log/n3rv.log" or "stdout" */ 
            void add_dest(std::string dst);

            /** >> Operator overloading, allows to write content directly to buffer.
             *  Note: If buffer encounters std::endl then if is flushed to destinations. */ 
            std::istream& operator>>(std::istream& is) ;

            /** Adds str to log buffer and flushes it. 
             *  @param log_level log level of the string to log.
             *  @param str string to log.
            */
            void log(int log_level, std::string str);

            /** Changes current logger's log level. 
             *  @param lvl log level to set, from 0 to 4.
            */
            void set_loglevel(int lvl);

            /** class constructor */
            logger(int log_level);

            static std::map<int, std::string> ll_map;

            
        protected:

            std::vector<std::string> dests;
            std::vector<std::streambuf*> dest_buffers;
            std::string buffer;
            int log_level;
    };
}

#endif