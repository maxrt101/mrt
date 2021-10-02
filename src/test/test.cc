#include <mrt/test/test.h>

#include <iostream>

constexpr auto k_reset = "\u001b[0m";
constexpr auto k_green = "\u001b[32m";
constexpr auto k_red   = "\u001b[31m";

mrt::test::Test::Test(const std::string& name, FunctionType function)
  : m_name(name), m_test_function(function) {}

bool mrt::test::Test::run() const {
  return m_test_function();
}

std::string mrt::test::Test::getName() const {
  return m_name;
}

mrt::test::Test::FunctionType mrt::test::Test::getFunction() const {
  return m_test_function;
}

static void printTestResult(const std::string& name, bool result) {
  if (result) {
    std::cout << "[ " << k_green << " OK " << k_reset << " ] " << name << std::endl;
  } else {
    std::cout << "[ " << k_red << "FAIL" << k_reset << " ] " << name << std::endl;
  }
}

bool mrt::test::run(const Test& test, bool print) {
  bool result = test.run();
  if (print) printTestResult(test.getName(), result);
  return result;
}

bool mrt::test::run(const std::vector<Test>& tests, bool print) {
  bool result = true;
  for (const Test& test : tests) result &= run(test, print);
  return result;
}


