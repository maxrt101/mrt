#ifndef _MRT_CONTAINER_UTILS_H_
#define _MRT_CONTAINER_UTILS_H_ 1

#include <iostream>

/* Allows for std::pair to be printed be cout */
template <typename T1, typename T2>
inline std::ostream& operator<<(std::ostream& os, const std::pair<T1, T2>& pair) {
  os << "(" << pair.first << ", " << pair.second << ")";
  return os;
}

namespace mrt {

/* Checks if map (std::map, std::multimap) has a key
 * Template parameters:
 *  M - Map
 *  K - Key
 *  V - Value
 *  C - Compare
 */
template <template <typename, typename, typename, typename> typename M,
          typename K, typename V, typename C, typename A>
inline bool hasKey(const M<K, V, C, A>& map, const K& value) {
  return map.find(value) != map.end();
}

/* Checks of value is in any of args */
template <typename T, typename ... Args>
inline bool isIn(T&& value, Args&& ...args) {
  return ((value == args) || ...);
}

/* Filters the container by predicate
 * if predicate(e) for e in container - e ends up the resulting container
 */
template <typename T, typename F>
inline T filter(const T& container, F&& predicate) {
  T result;
  for (auto& element : container) {
    if (predicate(element)) {
      result.push_back(element);
    }
  }
  return result;
}

} // namespace mrt

#endif
