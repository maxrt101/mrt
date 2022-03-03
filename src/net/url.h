#ifndef _MRT_NET_URL_H_
#define _MRT_NET_URL_H_ 1

#include <string>
#include <map>

#include <mrt/container_utils.h>
#include <mrt/format.h>

namespace mrt {
namespace net {

struct Url {
  std::string url;
  
  std::string protocol;
  std::string host;
  std::string resource;
  std::map<std::string, std::string> params;

  inline std::string toString() const {
    std::string s = format("{}{}{}{}",
      protocol.empty() ? "" : protocol + "://",
      host,
      resource,
      params.empty() ? "" : reduce(params, [](auto result, auto element) {
        return format("{}{}={}&", result, element.first, element.second);
      }, std::string("?"))
    );
    return s.substr(0, s.size()-1);
  }

  static inline Url fromString(const std::string& str) {
    Url url;
    url.url = str;

    size_t idx = str.find("://");
    if (idx != std::string::npos) {
      url.protocol = str.substr(0, idx);
      idx += 3;
    }

    while (idx < str.size() && str[idx] != '/') {
      url.host += str[idx++];
    }

    while (idx < str.size() && str[idx] != '?') {
      url.resource += str[idx++];
    }

    size_t prev = ++idx;
    std::string key;
    while (idx < str.size()) {
      if (str[idx] == '=') {
        key = str.substr(prev, idx-prev);
        prev = idx;
      } else if (str[idx] == '&') {
        url.params[key] = str.substr(prev+1, idx-prev-1);
        prev = idx+1;
      }
      idx++;
    }
    url.params[key] = str.substr(prev+1, idx-prev-1);

    return url;
  }
};

} /* namespace net */
} /* namespace mrt */

#endif