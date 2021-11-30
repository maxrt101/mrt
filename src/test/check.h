#ifndef _MRT_TEST_CHECK_H
#define _MRT_TEST_CHECK_H

#include <string>
#include <iostream>

#include <mrt/console/colors.h>

#ifndef MRT_NO_MACROS
#define MRT_ASSERT_TRUE(x, msg) mrt::test::assertTrue((x), #x, (msg), __FILE__, __LINE__)
#define MRT_ASSERT_EQUAL(x, y, msg) mrt::test::assertTrue((x) == (y), #x " == " #y, (msg), __FILE__, __LINE__)
#endif

namespace mrt {
namespace test {

class AssertionError : std::exception {
  std::string m_msg;
 public:
  inline AssertionError(const std::string& msg) : m_msg(msg) {}
  inline std::string getMsg() { return m_msg; }
};

inline void assertTrue(bool value, const std::string& assertion, const std::string& msg, const std::string& file, int line) {
  using namespace mrt::console::colors;
  if (!value) {
    std::cout << RED << "ERROR: " << RESET << file << ":" << line << ": Assertion failed: " << assertion << std::endl;
    throw AssertionError(msg);
  }
}

} /* namespace test */
} /* namespace mrt */

#endif
