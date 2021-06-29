#include "strutils.h"

bool mrt::str::endsWith(const std::string& str, const std::string& ending) {
  if (ending.size() > str.size()) {
    return false;
  }
  return std::equal(ending.rbegin(), ending.rend(), str.rbegin());
}

