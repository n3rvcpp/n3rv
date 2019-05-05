#include <n3rv/n3rvservicecontroller.hpp>
#include <n3rv/n3rvhttpservice.hpp>
#include "testutils.hpp"

int test_http_service() {

    //n3rv::scioctl* sctl = n3rv::start_controller("0.0.0.0",10001);

    n3rv::httpservice ht1("127.0.0.1",10001);   
    ht1.set_uid("com.http.node1");
    ht1.init_http("0.0.0.0",8081);
    ht1.attach_http("/foo",n3rv::httpservice::http_callback);

    ht1.run_async();
    ht1.run_http_async();

    if (! test_listen(8081) ) {
        //sctl->ctl->terminate();
        return 1;
    }

    if (! test_http("http://127.0.0.1:8081/foo", "<html><body>Hello World!</body></html>" )) {
        //sctl->ctl->terminate();
        return 2;
    }

    //sctl->ctl->terminate(); 
    return 0;
}