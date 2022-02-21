#ifndef _MRT_FUNCTIONS_H
#define _MRT_FUNCTIONS_H 1

#include <functional>

namespace mrt {

template <typename F, typename... Args>
inline auto bindMember(F&& fn, Args&&... args) {
  return std::bind(fn, std::placeholders::_1, args...);
}

} /* namespace mrt */

#endif
