#ifndef MRT_LOGIC_H_
#define MRT_LOGIC_H_ 1

#include <map>

namespace mrt {

template<typename Map, typename T>
inline bool containsKey(Map<T>& map, T& value) {
  return map.find(value) != map.end();
}

template<typename T, typename ... Args>
inline bool isIn(T&& value, Args&& ...args) {
  return ((value == args) || ...);
}

} // namespace mrt

#endif
