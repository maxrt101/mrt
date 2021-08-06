#include "strutils.h"

bool startsWith(const std::string& str, const std::string& beginning) {
  return str.rfind(beginning, 0) == 0;
}

bool mrt::str::endsWith(const std::string& str, const std::string& ending) {
  if (ending.size() > str.size()) {
    return false;
  }
  return std::equal(ending.rbegin(), ending.rend(), str.rbegin());
}

std::vector<std::string> mrt::str::split(const std::string& str, const std::string& delimiter) {
  std::vector<std::string> result;
  size_t last = 0, next = 0;
  while ((next = str.find(delimiter, last)) != std::string::npos) {
    result.push_back(str.substr(last, next-last));
    last = next + 1;
  }
  result.push_back(str.substr(last));
  return result;
}

