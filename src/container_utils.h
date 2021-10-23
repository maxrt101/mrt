#ifndef _MRT_CONTAINER_UTILS_H_
#define _MRT_CONTAINER_UTILS_H_ 1

#include <iostream>

/* Allows for std::pair to be printed by cout */
#ifndef MRT_DISABLE_STDPAIR_PRINTER
template <typename T1, typename T2>
inline std::ostream& operator<<(std::ostream& os, const std::pair<T1, T2>& pair) {
  os << "(" << pair.first << ", " << pair.second << ")";
  return os;
}
#endif

namespace mrt {

/* Checks if map has a key
 * Template parameters:
 *  M - Map
 *  T - Key
 */
template <template <class, class...> class M, class T, class ... Args>
inline bool hasKey(const M<T, Args...>& map, const T& value) {
  return map.find(value) != map.end();
}

/* Compares containers (vectors, maps, etc)
 * Template parameters:
 *  C - Container
 */
template <typename C>
inline bool equal(const C& lhs, const C& rhs) {
  return lhs.size() == rhs.size() && std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

/* Compares containers (vectors, maps, etc) by predicate
 * Template parameters:
 *  C - Container
 *  P - Predicate (for example [](auto a, auto b) -> bool { return a == b; })
 */
template <typename C, typename P>
inline bool equal(const C& lhs, const C& rhs, P pred) {
  return lhs.size() == rhs.size() && std::equal(lhs.begin(), lhs.end(), rhs.begin(), pred);
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
