#include <n3rv/n3rvservicecontroller.hpp>
#include <n3rv/n3rvhttpservice.hpp>

int test_http_service() {

    n3rv::httpservice ht1("foobar","foobar","127.0.0.1",10001);   

    ht1.init_http("0.0.0.0",8081);
    ht1.run_http_async();

    FILE* fh = popen("netstat -anp 2>/dev/null|grep LISTEN|grep 8081","r");
    std::array<char, 255> buff;
    std::string net_out = "";

    while( ! feof(fh) ) {
        fgets(buff.data(),255,fh);
        net_out += buff.data();
    }

    ht1.terminate();

    if ( net_out.find("0.0.0.0:8081") == std::string::npos ) {
        return 1;
    }

    return 0;
}