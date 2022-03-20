#ifndef _MRT_STRUTILS_H_
#define _MRT_STRUTILS_H_ 1

#include <string>
#include <vector>

namespace mrt {
namespace str {

inline bool startsWith(const std::string& str, const std::string& beginning) {
  return str.rfind(beginning, 0) == 0;
}

inline bool endsWith(const std::string& str, const std::string& ending) {
  if (ending.size() > str.size()) {
    return false;
  }
  return std::equal(ending.rbegin(), ending.rend(), str.rbegin());
}

inline std::vector<std::string> split(const std::string& str, const std::string& delimiter = " ") {
  std::vector<std::string> result;
  size_t last = 0, next = 0;
  while ((next = str.find(delimiter, last)) != std::string::npos) {
    result.push_back(str.substr(last, next-last));
    last = next + 1;
  }
  result.push_back(str.substr(last));
  return result;
}

inline std::vector<std::string> splitQuoted(const std::string& str, const std::string& delimiter = " ") {
  std::vector<std::string> result;
  size_t last = 0, next = 0;
  while ((next = str.find(delimiter, last)) != std::string::npos) {
    result.push_back(str.substr(last, next-last));
    last = next + 1;
  }
  result.push_back(str.substr(last));
  for (int i = 0; i < result.size(); i++) {
    if (result[i][0] == '\'' || result[i][0] == '\"') {
      char quote = result[i][0];
      std::string token = result[i];
      int j = i++;
      while (i < result.size() && result[i][result[i].size()-1] != quote) {
        token += " " + result[i++];
      }
      token += " " + result[i++];
      result.erase(result.begin()+j, result.begin()+i);
      i = j;
      result.insert(result.begin() + i, token.substr(1, token.size()-2));
    }
  }
  return result;
}

} // namespace str
} // namespace mrt

#endif
