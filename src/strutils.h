#ifndef _MRT_STRUTILS_H_
#define _MRT_STRUTILS_H_ 1

#include <string>
#include <vector>

namespace mrt {
namespace str {

bool startsWith(const std::string& str, const std::string& beginning);
bool endsWith(const std::string& str, const std::string& ending);

std::vector<std::string> split(const std::string& str, const std::string& delimiter = " ");

} // namespace str
} // namespace mrt

#endif
