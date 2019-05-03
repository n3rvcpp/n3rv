#include "fixtures.hpp"
#include <map>

int test_nlookup_direct() {

    auto dir = directory();
    n3rv::qserv* s = n3rv::nlookup(dir, "com.class.node1");

    //error, lookup should be ok
    if ( s == nullptr ) return 1;

    s = n3rv::nlookup(dir, "com.class.ZjxsS" );

    //lookup should return null
    if (s != nullptr ) return 2;

    std::vector<n3rv::qserv_> zerodir;

    //tests nlookup with zero-sized directory
    s = n3rv::nlookup(zerodir, "com.class.node1");

    //lookup should return null
    if (s != nullptr ) return 3;

    return 0;
}

int test_nlookup_glob() {

    auto dir = directory();
    n3rv::qserv* s = n3rv::nlookup(dir, "com.class.*");

    //error, lookup should be ok
    if ( s == nullptr ) return 1;

    s = n3rv::nlookup(dir, "com.dkQlks.*");

    //lookup should return null
    if (s != nullptr) return 1;

    return 0;
}


int test_blookup_direct() {
    auto dir = directory();
    n3rv::binding* b = n3rv::blookup(dir, "com.class.node1.binding1");

    //error, lookup should be ok
    if ( b == nullptr ) return 1;

    b = n3rv::blookup(dir, "com.class.node1.dkldslkds");

    //lookup should return null
    if (b != nullptr) return 1;

    return 0;
}

int test_blookup_glob() {

    auto dir = directory();
    n3rv::binding* b = n3rv::blookup(dir, "com.*.*.binding1");

    //error, lookup should be ok
    if ( b == nullptr ) return 1;

    b = n3rv::blookup(dir, "com.fddfsf.*.binding1");

    //lookup should return null
    if (b != nullptr) return 2;

    return 0;


}

