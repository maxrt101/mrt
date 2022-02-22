#ifndef _MRT_FORMAT_H_
#define _MRT_FORMAT_H_ 1

#include <map>
#include <set>
#include <string>
#include <string_view>
#include <unordered_map>
#include <mrt/functions.h>

#define _MRT_VEC_TO_STRING_FUNC(cls) \
  template <typename T> \
  inline std::string to_string(const cls<T>& vec) { \
    std::string tmp = "{"; \
    for (auto itr = vec.begin(); itr != vec.end(); ++itr) { \
      tmp += to_string(*itr) + ", "; \
    } \
    if (tmp.size() > 2) { \
      tmp.erase(tmp.end()-2, tmp.end()); \
    } \
    return tmp + "}"; \
  }

#define _MRT_MAP_TO_STRING_FUNC(cls) \
  template <typename K, typename V> \
  inline std::string to_string(const cls<K, V>& map) { \
    std::string tmp = "{"; \
    for (auto itr = map.begin(); itr != map.end(); ++itr) { \
      tmp += to_string(itr->first) + ": " + to_string(itr->second) + ", "; \
    } \
    if (tmp.size() > 2) { \
      tmp.erase(tmp.end()-2, tmp.end()); \
    } \
    return tmp + "}"; \
  }

namespace mrt {

/* to_string functions - return a string representation of an object */

template<typename T>
inline std::string to_string(const T& v) {
  return std::to_string(v);
}

inline std::string to_string(const char c) {
  return std::string() + c;
}

inline std::string to_string(const char* s) {
  return s;
}

inline std::string to_string(const std::string& s) {
  return s;
}

template <typename T1, typename T2>
inline std::string to_string(const std::pair<T1, T2>& p) {
  return std::string("(") + to_string(p.first) + ", " + to_string(p.second);
}

_MRT_VEC_TO_STRING_FUNC(std::vector)
_MRT_VEC_TO_STRING_FUNC(std::set)
_MRT_MAP_TO_STRING_FUNC(std::map)
_MRT_MAP_TO_STRING_FUNC(std::unordered_map)

namespace impl {

/* Implementation for mrt::format - base case */
template <typename... Args>
inline void format(const std::string_view& fmt, std::string& output) {
  output += fmt;
}

/* Implementation for mrt::format
 * Format String Specification:
 *  {} - value
 *  {T} - value type
 */
template <typename... Args, typename T>
inline void format(const std::string_view& fmt, std::string& output, T& val, Args&&... args) {
  for (size_t i = 0; i < fmt.size(); i++) {
    if (fmt[i] != '\\') {      // Escape format
      if (fmt[i] == '{') {     // Format begin
        if (fmt[++i] == '}') { // Format end
          output += to_string(val);
          impl::format(fmt.substr(i+1), output, args...);
          return;
        } else {
          switch (fmt[i]) {
            case 'T': { // Type of argument
              if (fmt[i+1] != '}') { // TODO: Handle error
                output += "{T";
                continue;
              }
              output += getTypeName<T>();
              impl::format(fmt.substr(i+1), output, args...);
              return;
            }
            default: {
              output += fmt[i];
              break;
            }
          }
        }
      } else {
        output += fmt[i];
      }
    } else {
      output += fmt[++i];
    }
  }
  output += to_string(val);
}

} /* namespace impl */

/* Returns a formatted string
 * Parameters:
 *  fmt - format string
 *  args
 */
template <typename... Args>
inline std::string format(std::string fmt, Args&&... args) {
  std::string output;
  impl::format(fmt, output, args...);
  return output;
}

} /* namespace mrt */

#undef _MRT_VEC_TO_STRING_FUNC
#undef _MRT_MAP_TO_STRING_FUNC

#endif