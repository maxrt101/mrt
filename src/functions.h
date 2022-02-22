#ifndef _MRT_FUNCTIONS_H_
#define _MRT_FUNCTIONS_H_ 1

#include <functional>
#include <typeinfo>
#include <cxxabi.h>
#include <memory>
#include <string>

namespace mrt {

/* Binds `this` to a member function */
template <typename F, typename... Args>
inline auto bindMember(F&& fn, Args&&... args) {
  return std::bind(fn, std::placeholders::_1, args...);
}

/* Demangles type name */
inline std::string demangleTypeName(const char* mangled) {
  int status = 0;
  std::unique_ptr<char> result(abi::__cxa_demangle(mangled, 0, 0, &status));
  if (status) {
    return "<error>";
  }
  return std::string(result.get());
}

/* Returns typename for T */
template <typename T>
inline std::string getTypeName() {
  return demangleTypeName(typeid(T).name());
}

} /* namespace mrt */

#endif
