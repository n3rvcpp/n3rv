#include <n3rv/n3rvlogger.hpp>


class logger_test: public n3rv::logger {
    using n3rv::logger::logger;

    public: 

        std::vector<std::string> get_dests() {
            return this->dests;
        }

        std::vector<std::streambuf*> get_buffers() {
            return this->dest_buffers;
        }

        int get_loglevel() {
            return this->log_level;
        }
};


int test_logger_add_dest_stdout() {

    logger_test ll(n3rv::LOGLV_DEBUG);
    ll.add_dest("stdout");

    std::vector<std::string> dests = ll.get_dests();
    std::vector<std::streambuf*> buffs = ll.get_buffers();

    if (dests[0] != "stdout") {
        return 1;
    }

    if (buffs[0] != std::cout.rdbuf()) {
        return 1;
    }

    return 0;
}


int test_logger_add_dest_file() {

    logger_test ll(n3rv::LOGLV_DEBUG);
    ll.add_dest("file://n3rvtest.log");

    std::vector<std::string> dests = ll.get_dests();
    std::vector<std::streambuf*> buffs = ll.get_buffers();

    if (dests[0] != "file://n3rvtest.log") {
        return 1;
    }

    if (buffs.size() < 1) {
        return 1;
    }

    return 0;
}



int test_logger_set_loglevel() {

        logger_test ll(n3rv::LOGLV_DEBUG);
        if ( ll.get_loglevel() !=  n3rv::LOGLV_DEBUG ) return 1;
        ll.set_loglevel(n3rv::LOGLV_NOTICE);
        if ( ll.get_loglevel() !=  n3rv::LOGLV_NOTICE ) return 1;
        return 0;

}