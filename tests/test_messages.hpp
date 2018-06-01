#include <n3rv/n3rvproto.hpp>
#include <iostream>

int test_message_serialize_parse() {

    n3rv::message m;
    m.action = "foobar";
    m.args.emplace_back("arg1");
    m.args.emplace_back("arg2");
    m.args.emplace_back("arg3");
    m.payload = "payload";

    std::string ser_msg = n3rv::serialize_msg(m);

    if ( ser_msg == "" ) return 1;

    n3rv::message reconst_m = n3rv::parse_msg(ser_msg);

    if ( reconst_m.action != "foobar"  ) return 1;
    if ( reconst_m.args.size() != 3  ) return 1;
    if ( reconst_m.args[0]!= "arg1"  ) return 1;
    if ( reconst_m.args[1]!= "arg2"  ) return 1;
    if ( reconst_m.payload != "payload"  ) return 1;

    return 0;
    
}


