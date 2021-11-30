#ifndef _MRT_CONSOLE_COLORS_H
#define _MRT_CONSOLE_COLORS_H

#include <cstdint>

namespace mrt {
namespace console {
namespace colors {
constexpr auto RESET   = "\u001b[0m";
constexpr auto BLACK   = "\u001b[30m";
constexpr auto RED     = "\u001b[31m";
constexpr auto GREEN   = "\u001b[32m";
constexpr auto YELLOW  = "\u001b[33m";
constexpr auto BLUE    = "\u001b[34m";
constexpr auto MAGENTA = "\u001b[35m";
constexpr auto CYAN    = "\u001b[36m";
constexpr auto WHITE   = "\u001b[37m";
} /* namespace color */

#ifndef MRT_NO_USING_COLORS
using namespace colors;
#endif

} /* namespace console */
} /* namespace mrt */

#endif