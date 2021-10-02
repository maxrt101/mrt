#ifndef _MRT_TEST_H
#define _MRT_TEST_H

#include <string>
#include <vector>
#include <functional>

namespace mrt {
namespace test {

class Test {
 public:
  using FunctionType = std::function<bool()>;

 private:
  std::string m_name;
  FunctionType m_test_function;

 public:
  Test(const std::string& name, FunctionType function);

  bool run() const;
  std::string getName() const;
  FunctionType getFunction() const;
};

bool run(const Test& test, bool print = false);
bool run(const std::vector<Test>& tests, bool print = false);

} /* namespace test */
} /* namespace mrt */

#endif

