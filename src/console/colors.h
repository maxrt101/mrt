#ifndef _MRT_CONSOLE_COLORS_H
#define _MRT_CONSOLE_COLORS_H

#include <cstdint>

namespace mrt {
namespace console {
namespace colors {

#ifdef MRT_NO_COLORS
constexpr char RESET[]   = "";
constexpr char BLACK[]   = "";
constexpr char RED[]     = "";
constexpr char RED_RED[] = "";
constexpr char GREEN[]   = "";
constexpr char YELLOW[]  = "";
constexpr char BLUE[]    = "";
constexpr char MAGENTA[] = "";
constexpr char CYAN[]    = "";
constexpr char WHITE[]   = "";
#else
constexpr char RESET[]   = "\u001b[0m";
constexpr char BLACK[]   = "\u001b[30m";
constexpr char RED[]     = "\u001b[31m";
constexpr char RED_RED[] = "\u001b[41m";
constexpr char GREEN[]   = "\u001b[32m";
constexpr char YELLOW[]  = "\u001b[33m";
constexpr char BLUE[]    = "\u001b[34m";
constexpr char MAGENTA[] = "\u001b[35m";
constexpr char CYAN[]    = "\u001b[36m";
constexpr char WHITE[]   = "\u001b[37m";
#endif

} /* namespace color */

#ifndef MRT_NO_USING_COLORS
using namespace colors;
#endif

} /* namespace console */
} /* namespace mrt */

#endif
