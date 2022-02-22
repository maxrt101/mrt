#ifndef _MRT_TEST_FRAMEWORK_H
#define _MRT_TEST_FRAMEWORK_H

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>

#include <mrt/container_utils.h>
#include <mrt/console/colors.h>
#include <mrt/test/test.h>
#include <mrt/math.h>

namespace mrt {
namespace test {

class TestFramework {
 private:
  std::vector<Test> m_tests;
  std::map<std::string, Result> m_results;

 public:
  TestFramework() = default;

  inline void addTest(Test test) {
    m_tests.push_back(test);
  }

  inline bool run(bool print = false) {
    using namespace mrt::console::colors;
    bool result = true;
    for (const Test& test : m_tests) {
      Result testResult = mrt::test::run(test, print);
      m_results[test.getName()] = testResult;
      result &= testResult.result;
    }
    if (print) {
      int okCount = mrt::reduce<int>(m_results, [](int prev, const auto& r){ return prev + (r.second.result ? 1 : 0); }, 0);
      int failCount = m_results.size() - okCount;
      int digits = countDigits(m_results.size());
      std::cout << "Summary: " << std::endl;
      std::cout << "OK:     " << GREEN << std::setw(digits) << okCount << RESET << " - " << std::setw(3) << (int)(okCount  *100.0/m_results.size()) << "%" << std::endl;
      std::cout << "FAILED: " << RED << std::setw(digits) << failCount << RESET << " - " << std::setw(3) << (int)(failCount*100.0/m_results.size()) << "%" << std::endl;
    }
    return result;
  }

  inline const std::map<std::string, Result>& getResults() const {
    return m_results;
  }
};

} /* namespace test */
} /* namespace mrt */

#endif

