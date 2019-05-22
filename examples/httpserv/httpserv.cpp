#include <n3rv/n3rvcommon.hpp>
#include <n3rv/n3rvservice.hpp>
#include <n3rv/n3rvhttpservice.hpp>


class httpgw: public n3rv::httpservice {
    using n3rv::httpservice::httpservice;

};



int main() {

    n3rv::logger* ll = new n3rv::logger(n3rv::LOGLV_DEBUG);
    ll->add_dest("stdout");

    httpgw mygw("127.0.0.1",10001,ll);
    mygw.init_http("0.0.0.0",8080);
    mygw.attach_http("/hello", n3rv::httpservice::http_callback);
    mygw.register_dir("/foo/bar","/var/www/html/");
    mygw.run_http();






}