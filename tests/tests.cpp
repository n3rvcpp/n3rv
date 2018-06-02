#include "test_servicecontroller.hpp"
#include "test_topology_parsing.hpp"
#include "test_logger.hpp"
#include "test_messages.hpp"
#include "test_httpservice.hpp"

#include <map>

typedef int (*tfctptr)();

int main() {

    std::map<std::string, tfctptr > tests_list;
    tests_list["test_servicecontroller_instanciate"] = test_servicecontroller_instanciate;
    tests_list["test_servicecontroller_load_topology"] = test_servicecontroller_load_topology;
    tests_list["test_topology_serialize"] = test_topology_serialize;
    tests_list["test_topology_parse"] = test_topology_parse;
    tests_list["test_logger_add_dest_stdout"] = test_logger_add_dest_stdout;
    tests_list["test_logger_add_dest_file"] = test_logger_add_dest_file; 
    tests_list["test_logger_set_loglevel"] = test_logger_set_loglevel;
    tests_list["test_message_serialize_parse"] = test_message_serialize_parse;
    tests_list["test_http_service"] = test_http_service;


    int test_num = 1;
    int num_ok = 0;
    int num_err = 0;
    int total = tests_list.size();

    for(auto t: tests_list) {

        std::cout << "Launching Test [" << test_num << "/" << total << "]: " << t.first << "..";
        int test_result = -1;

        //effectively launches test.
        try {
            test_result = (t.second)();
        }

        catch(std::exception e) {
            
        }

        

        if ( test_result == 0 )  {
            std::cout << "OK" << std::endl;
            num_ok++;
        }
        else {
            std::cout << "ERROR NUM " << test_result << std::endl;
            num_err++;
        }

        test_num ++;
    }

    std::cout << "Tests Results:" << std::endl << "===============" << std::endl;
    std::cout << "Total: " <<  total << std::endl;
    std::cout << "OK: " <<  num_ok << std::endl;
    std::cout << "Errors: " <<  num_err << std::endl;
    std::cout << "===============" << std::endl;

}