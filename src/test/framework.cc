#include <mrt/test/framework.h>

void mrt::test::TestFramework::addTest(Test test) {
  m_tests.push_back(test);
}

bool mrt::test::TestFramework::run(bool print) {
  bool result = true;
  for (const Test& test : m_tests) {
    bool test_result = mrt::test::run(test, print);
    m_results[test.getName()] = test_result;
    result &= test_result;
  }
  return result;
}

const std::map<std::string, bool>& mrt::test::TestFramework::getResults() const {
  return m_results;
}

