#ifndef _MRT_PRINT_H_
#define _MRT_PRINT_H_ 1

#include <cstdio>
#include <string>
#include <mrt/format.h>

namespace mrt {

namespace impl {

template <typename... Args>
inline void print(std::string& output) {
  fwrite(output.c_str(), 1, output.size(), stdout);
}

template <typename... Args, typename T>
inline void print(std::string& output, T& val, Args&&... args) {
  output += to_string(val);
  impl::print(output, args...);
}

} /* namespace impl */

/* Prints a string, formatted with mrt::format */
template <typename... Args>
inline void printf(const std::string& fmt, Args&&... args) {
  std::string output = format(fmt, args...);
  fwrite(output.c_str(), 1, output.size(), stdout);
}

/* Prints a string, formatted with mrt::format into a file */
template <typename... Args>
inline void fprintf(const std::string& filename, const std::string& fmt, Args&&... args) {
  FILE* file = fopen(filename.c_str(), "w");
  std::string output = format(fmt, args...);
  fwrite(output.c_str(), 1, output.size(), file);
}

/* Prints a string, formatted with mrt::format into a file */
template <typename... Args>
inline void fprintf(FILE* file, const std::string& filename, const std::string& fmt, Args&&... args) {
  std::string output = format(fmt, args...);
  fwrite(output.c_str(), 1, output.size(), file);
}

/* Prints args, delimiting with a space */
template <typename... Args>
inline void print(Args&&... args) {
  std::string output;
  impl::print(output, args...);
}

/* Prints args, delimiting with a space, with a newline at the end */
template <typename... Args>
inline void println(Args&&... args) {
  std::string output;
  impl::print(output, args..., "\n");
}

} /* namespace mrt */

#endif