#ifndef _MRT_LOG_H_
#define _MRT_LOG_H_ 1

#include <algorithm>
#include <cstdarg>
#include <cstdio>
#include <cctype>
#include <string>

#include <mrt/console/colors.h>

#define _VLOGF_INTERNAL(level) \
  va_list args; \
  va_start(args, format); \
  vlogf(level, format, args); \
  va_end(args);

namespace mrt {
namespace logger {

enum class LogLevel {
  DEBUG,
  INFO,
  WARNING,
  ERROR,
  FATAL
};

inline LogLevel stringToLogLevel(const std::string& s) {
  std::string str = s;
  std::transform(str.begin(), str.end(), str.begin(), tolower);
  if (str == "debug") {
    return LogLevel::DEBUG;
  } else if (str == "info") {
    return LogLevel::INFO;
  } else if (str == "warning") {
    return LogLevel::WARNING;
  } else if (str == "error") {
    return LogLevel::ERROR;
  } else if (str == "fatal") {
    return LogLevel::FATAL;
  }
  return LogLevel::DEBUG;
}

inline void vflogf(FILE* dest, LogLevel level, const std::string& format, va_list args) {
  switch (level) {
    case LogLevel::FATAL:   fprintf(dest, "%sFATAL",   mrt::console::RED_RED); break;
    case LogLevel::ERROR:   fprintf(dest, "%sERROR",   mrt::console::RED);     break;
    case LogLevel::WARNING: fprintf(dest, "%sWARNING", mrt::console::YELLOW);  break;
    case LogLevel::INFO:    fprintf(dest, "%sINFO",    mrt::console::CYAN);    break;
    case LogLevel::DEBUG:   fprintf(dest, "%sDEBUG",   mrt::console::BLUE);    break;
    default:                fprintf(dest, "INVALID_LOG_LEVEL");                break;
  }

  fprintf(dest, "\033[0m: ");
  vfprintf(dest, format.c_str(), args);
  fprintf(dest, "\n");
}

inline void vlogf(LogLevel level, const std::string& format, va_list args) {
  // if (level < g_logLevel) return; // global state cannot be used, because everyting is header-only

  FILE* dest = stdout;
  if (level > LogLevel::INFO) {
    dest = stderr;
  }

  vflogf(dest, level, format, args);
}

inline void logf(LogLevel level, const std::string format, ...) {
  va_list args;
  va_start(args, format);
  vlogf(level, format, args);
  va_end(args);
}

inline void debug(const std::string format, ...) {
  _VLOGF_INTERNAL(LogLevel::DEBUG);
}

inline void info(const std::string format, ...) {
  _VLOGF_INTERNAL(LogLevel::INFO);
}

inline void warning(const std::string format, ...) {
  _VLOGF_INTERNAL(LogLevel::WARNING);
}

inline void error(const std::string format, ...) {
  _VLOGF_INTERNAL(LogLevel::ERROR);
}

inline void fatal(const std::string format, ...) {
  _VLOGF_INTERNAL(LogLevel::FATAL);
}

// inline LogLevel getLogLevel();
// inline void setLogLevel(LogLevel level);

} /* namespace logger */

#ifndef MRT_NO_USING_LOGGER
using namespace logger;
#endif

} /* namespace mrt */

#undef _VLOGF_INTERNAL

#endif