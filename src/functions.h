#ifndef _MRT_FUNCTIONS_H
#define _MRT_FUNCTIONS_H 1

#include <functional>
#include <typeinfo>
// #include <type_traits>
#include <cxxabi.h>
#include <memory>
#include <string>

namespace mrt {

template <typename F, typename... Args>
inline auto bindMember(F&& fn, Args&&... args) {
  return std::bind(fn, std::placeholders::_1, args...);
}

inline std::string demangleTypeName(const char* mangled) {
  int status = 0;
  std::unique_ptr<char> result(abi::__cxa_demangle(mangled, 0, 0, &status));
  if (status) {
    return "<error>";
  }
  return std::string(result.get());
}

template <typename T>
inline std::string getTypeName() {
  return demangleTypeName(typeid(T).name());
}

} /* namespace mrt */

#endif
