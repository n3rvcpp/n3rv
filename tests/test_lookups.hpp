#include "fixtures.hpp"
#include <map>

inline int test_nlookup_direct() {

  auto dir = directory();
  n3rv::nullable_ref<n3rv::qserv> s = n3rv::node_lookup(dir, "com.class.node1");

  // error, lookup should be ok
  if (std::nullopt == s)
    return 1;

  s = n3rv::node_lookup(dir, "com.class.ZjxsS");

  // lookup should return null
  if (std::nullopt != s)
    return 2;

  std::vector<n3rv::qserv> zerodir;

  // tests nlookup with zero-sized directory
  s = n3rv::node_lookup(zerodir, "com.class.node1");

  // lookup should return null
  if (std::nullopt != s)
    return 3;

  return 0;
}

inline int test_nlookup_glob() {

  auto dir = directory();
  n3rv::nullable_ref<n3rv::qserv> s = n3rv::node_lookup(dir, "com.class.*");

  // error, lookup should be ok
  if (std::nullopt == s)
    return 1;

  s = n3rv::node_lookup(dir, "com.dkQlks.*");

  // lookup should return null
  if (std::nullopt != s)
    return 1;

  return 0;
}

inline int test_blookup_direct() {
  auto dir = directory();
  n3rv::nullable_ref<n3rv::binding> b =
      n3rv::binding_lookup(dir, "com.class.node1.binding1");

  // error, lookup should be ok
  if (std::nullopt == b)
    return 1;

  b = n3rv::binding_lookup(dir, "com.class.node1.dkldslkds");

  // lookup should return null
  if (std::nullopt == b)
    return 1;

  return 0;
}

inline int test_blookup_glob() {

  auto dir = directory();
  n3rv::nullable_ref<n3rv::binding> b =
      n3rv::binding_lookup(dir, "com.*.*.binding1");

  // error, lookup should be ok
  if (std::nullopt == b)
    return 1;

  b = n3rv::binding_lookup(dir, "com.fddfsf.*.binding1");

  // lookup should return null
  if (std::nullopt != b)
    return 2;

  return 0;
}
