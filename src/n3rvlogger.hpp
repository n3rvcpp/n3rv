#ifndef __N3RVLOGGER__
#define __N3RVLOGGER__

#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <syslog.h>

namespace n3rv {

    enum LOGLV {    
        LOGLV_PANIC,
        LOGLV_ALERT,
        LOGLV_CRIT,
        LOGLV_ERR,
        LOGLV_WARN, 
        LOGLV_NOTICE,
        LOGLV_INFO,
        LOGLV_DEBUG
    };


    /**
     * n3rv logging class
     * Allows to produce timestamped log inside files (so far, syslog udp/512 also planned). 
     */
    class logger {

        public:

            /** Adds a destination for the log stream.
             *  @param dest destination of the logs, eg "file:///var/log/n3rv.log", "stdout", or "syslog:name" */ 
            void add_dest(const char* dest);

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

            /** class destructor */
            ~logger();

            static std::map<int, std::string> ll_map;
            static std::map<std::string, int> fac_map;


            
        protected:

            std::vector<std::string> dests;
            bool has_syslog;
            std::vector<std::streambuf*> dest_buffers;
            std::string buffer;
            int log_level;

    };
}

#endif