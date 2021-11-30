#ifndef _MRT_CONTAINER_UTILS_H_
#define _MRT_CONTAINER_UTILS_H_ 1

#include <algorithm>
#include <iostream>

#ifndef MRT_NO_MACROS

/* Allows for std::pair to be printed by cout */
#ifndef MRT_DISABLE_STDPAIR_PRINTER
template <typename T1, typename T2>
inline std::ostream& operator<<(std::ostream& os, const std::pair<T1, T2>& pair) {
  os << "(" << pair.first << ", " << pair.second << ")";
  return os;
}
#endif // MRT_DISABLE_STDPAIR_PRINTER

/* Returns range for container
 * c - any container
 */
#define MRT_ALL(c) c.begin(), c.end()

#endif // MRT_NO_MACROS

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
 * Template parameters:
 *  C - Container
 *  F - Predicate
 *  I - Inserter
 */
template <typename C, typename F, typename I = std::back_insert_iterator<C>>
inline C filter(const C& container, F&& predicate) {
  C result;
  std::copy_if(container.begin(), container.end(), I(result), predicate);
  return result;
}

/* Reduces container into single value
 * Template parameters:
 *  T - Return type
 *  C - Container
 *  F - Reducer
 */
template <typename T, typename C, typename F>
inline T reduce(const C& container, F&& reducer, T start = T{}) {
  T result = start;
  for (auto& element : container) {
    result = reducer(result, element);
  }
  return result;
}

/* Reduces container to the left into single value
 * Template parameters:
 *  T - Return type
 *  C - Container
 *  F - Reducer
 */
template <typename T, typename C, typename F>
inline T foldLeft(const C& container, F&& reducer, T start = T{}) {
  T result = start;
  for (auto itr = container.rbegin(); itr != container.rend(); itr++) {
    result = reducer(result, *itr);
  }
  return result;
}

/* Reduces container to the right into single value
 * Template parameters:
 *  T - Return type
 *  C - Container
 *  F - Reducer
 */
template <typename T, typename C, typename F>
inline T foldRight(const C& container, F&& reducer, T start = T{}) {
  T result = start;
  for (auto itr = container.begin(); itr != container.end(); itr++) {
    result = reducer(result, *itr);
  }
  return result;
}

} // namespace mrt

#endif
