#ifndef _MRT_TEST_FRAMEWORK_H
#define _MRT_TEST_FRAMEWORK_H

#include <string>
#include <vector>
#include <map>

#include <mrt/test/test.h>

namespace mrt {
namespace test {

class TestFramework {
 private:
  std::vector<Test> m_tests;
  std::map<std::string, bool> m_results;

 public:
  TestFramework() = default;

  void addTest(Test test);

  bool run(bool print = false);

  const std::map<std::string, bool>& getResults() const;
};

} /* namespace test */
} /* namespace mrt */

#endif

