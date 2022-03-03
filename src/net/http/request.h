#ifndef _MRT_NET_HTTP_REQUEST_H_
#define _MRT_NET_HTTP_REQUEST_H_ 1

#include <string>
#include <map>

#include <mrt/net/http/parse_error.h>
#include <mrt/container_utils.h>
#include <mrt/strutils.h>
#include <mrt/format.h>

namespace mrt {
namespace http {

struct Method {
  static constexpr int NONE    = 0;
  static constexpr int GET     = 1;
  static constexpr int HEAD    = 2;
  static constexpr int POST    = 3;
  static constexpr int PUT     = 4;
  static constexpr int DELETE  = 5;
  static constexpr int CONNECT = 6;
  static constexpr int OPTIONS = 7;
  static constexpr int TRACE   = 8;
  static constexpr int PATCH   = 9;

 public:
  int value;

 public:
  inline Method(int value) : value(value) {}
  inline Method(const Method& method) : value(method.value) {}

  static inline Method fromString(const std::string& str) {
    if (str == "GET") {
      return Method(GET);
    } else if (str == "POST") {
      return Method(POST);
    } else if (str == "HEAD") {
      return Method(HEAD);
    } else if (str == "PUT") {
      return Method(PUT);
    } else if (str == "DELETE") {
      return Method(DELETE);
    } else if (str == "CONNECT") {
      return Method(CONNECT);
    } else if (str == "TRACE") {
      return Method(TRACE);
    } else if (str == "PATCH") {
      return Method(PATCH);
    } else {
      return Method(NONE);
    }
  }

  inline std::string toString() const {
    switch (value) {
      case GET:     return "GET";
      case HEAD:    return "HEAD";
      case POST:    return "POST";
      case PUT:     return "PUT";
      case DELETE:  return "DELETE";
      case CONNECT: return "CONNECT";
      case OPTIONS: return "OPTIONS";
      case TRACE:   return "TRACE";
      case PATCH:   return "PATCH";
      default:      return "";
    }
  }
};

struct RequestHeader {
  Method method = Method::GET;
  std::string url = "/";
  std::map<std::string, std::string> params = {};
  std::string httpVersion = "1.1";

  inline std::string toString() const {
    if (params.size()) {
      return format("{} {} {}",
        method.toString(),
        reduce(params, [](auto result, auto element) {
          return format("{}&{}={}", result, element.first, element.second);
        }, url + "?"),
        std::string("HTTP/") + httpVersion
      );
    } else {
      return format("{} {} {}", method.toString(), url, "HTTP/" + httpVersion);
    }
  }

  static inline RequestHeader fromString(const std::string& str) {
    auto parts = str::split(str);
    if (parts.size() != 3) {
      throw ParseError(ParseError::Type::REQUEST);
    }
    RequestHeader header;
    header.method = Method::fromString(parts[0]);
    return header; 
  }
};

struct Request {
  RequestHeader header;
  std::map<std::string, std::string> headers;
  std::string body;

  inline std::string toString() const {
    return format("{}\r\n{}\r\n{}",
      header.toString(),
      reduce(headers, [](auto result, auto element) {
        return format("{}{}: {}\r\n", result, element.first, element.second);
      }, std::string()),
      body
    );
  }
};

} /* namespace http */
} /* namespace mrt */

#endif