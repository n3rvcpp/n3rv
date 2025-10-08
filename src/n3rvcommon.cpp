#include "n3rvcommon.hpp"

namespace n3rv {

std::string randstr(const std::size_t length) {
  std::string result;
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_int_distribution<int> dist('a', 'z');
  std::generate_n(std::back_inserter(result), length, [&] { return dist(mt); });
  return result;
}

std::optional<std::reference_wrapper<qserv>> nlookup(std::vector<qserv> &dir,
                                                     const std::string &addr) {

  std::regex rgx("\\*");

  std::vector<std::reference_wrapper<qserv>> rrlist;
  std::string lookup_str =
      regex_replace(addr, rgx, "(.*?)", std::regex_constants::match_any);

  for (int i = 0; i < dir.size(); i++) {
    std::string absname =
        dir[i].namespace_ + "." + dir[i].service_class + "." + dir[i].node_name;

    if (regex_search(absname, std::regex(lookup_str))) {
      rrlist.push_back(dir[i]);
    }
  }

  if (rrlist.size() == 1)
    return rrlist[0].get();
  else if (rrlist.size() > 1) {
    // Rand Node Lookup
    srand(time(NULL));
    int n_index = rand() % rrlist.size() + 0;
    return rrlist[n_index].get();
  }
  return std::nullopt;
}

std::optional<std::reference_wrapper<qserv>> nlookup(std::vector<qserv> &dir,
                                                     std::string service_class,
                                                     std::string node_name,
                                                     std::string namespace_) {

  std::string lookup_str = namespace_ + "." + service_class + "." + node_name;
  return nlookup(dir, lookup_str);
}

std::optional<std::reference_wrapper<binding>>
blookup(const std::vector<qserv> &dir, const std::string &addr) {

  // extracts binding name
  std::size_t bpos = addr.find(".");

  if (std::string::npos == bpos) {
    return std::nullopt;
  }

  std::string naddr = addr.substr(0, bpos);
  std::string bname = addr.substr(bpos + 1, addr.length() - bpos);

  auto s = nlookup(dir, naddr);

  if (std::nullopt == s) {
    return std::nullopt;
  }

  /*TODO: use std::ranges for this */
  for (int i = 0; i < s->get().bindings.size(); i++) {

    if (s->get().bindings[i].name == bname) {
      s->get().bindings[i].parent = (void *)(&(s->get()));
      return s->get().bindings[i];
    }
  }

  return std::nullopt;
}

} // namespace n3rv