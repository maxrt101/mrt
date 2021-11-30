#ifndef _MRT_TEST_H
#define _MRT_TEST_H

#include <string>
#include <vector>
#include <functional>
#include <iostream>

#include <mrt/console/colors.h>

namespace mrt {
namespace test {

struct Result {
  bool result;
  std::string fail_msg;

  // inline Result(bool result) : result(result) {}
  // inline Result(bool result, const std::string& fail_msg) : result(result), fail_msg(fail_msg) {}
};

class Test {
 public:
  using FunctionType = std::function<Result()>;

 private:
  std::string m_name;
  std::string m_description;
  FunctionType m_test_function;

 public:
  inline Test(const std::string& name, FunctionType function)
    : m_name(name), m_test_function(function) {}
 
  inline Test(const std::string& name, const std::string& description, FunctionType function)
    : m_name(name), m_description(description), m_test_function(function) {}

  inline Result run() const {
    return m_test_function();
  }

  inline std::string getName() const {
    return m_name;
  }

  inline std::string getDescription() const {
    return m_description;
  }

  inline FunctionType getFunction() const {
    return m_test_function;
  }
};

inline Result run(const Test& test, bool print = false) {
  using namespace mrt::console::colors;
  Result result = test.run();
  if (result.result) {
    std::cout << "[ " << GREEN << " OK " << RESET << " ] " << test.getName();
    if (test.getDescription().size()) {
      std::cout << " - " << test.getDescription();
    }
    std::cout << std::endl;
  } else {
    std::cout << "[ " << RED << "FAIL" << RESET << " ] " << test.getName();
    if (result.fail_msg.size()) {
      std::cout << " - " << result.fail_msg;
    }
    std::cout << std::endl;
  }
  return result;
}

inline bool run(const std::vector<Test>& tests, bool print = false) {
  bool result = true;
  for (const Test& test : tests) result &= run(test, print).result;
  return result;
}

} /* namespace test */
} /* namespace mrt */

#endif
