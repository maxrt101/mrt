#ifndef _MRT_NET_HTTP_PARSE_ERROR_H_
#define _MRT_NET_HTTP_PARSE_ERROR_H_ 1

#include <exception>

namespace mrt {
namespace http {

class ParseError : public std::exception {
  constexpr static size_t kBufferSize = 256;

 public:
  enum class Type {
    REQUEST, RESPONSE
  };

 private:
  Type m_type;

 public:
  inline ParseError(Type type) : m_type(type) {}

  inline const char* what() const throw() {
    char* buffer = new char[kBufferSize];
    snprintf(buffer, kBufferSize, "HTTP %s parsing failed", (m_type == Type::REQUEST) ? "Request" : "Response");
    return buffer;
  }
};

} /* namespace http */
} /* namespace mrt */

#endif