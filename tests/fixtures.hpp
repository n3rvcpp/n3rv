#include <n3rv/n3rvservicecontroller.hpp>

n3rv::servicecontroller* fix_svctl() {

    n3rv::servicecontroller sc1("0.0.0.0",10001);
    sc1.run_async();

}