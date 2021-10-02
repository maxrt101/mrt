
#include <mrt/test/framework.h>
#include <mrt/container_utils.h>

#include <string>
#include <vector>
#include <map>

void test_container_utils_add_tests(mrt::test::TestFramework& testFramework) {
  std::vector<int> vector {1, 10, 2, 9, 3, 8, 4, 7, 5, 6, 0};

  testFramework.addTest({"Test Container Utils - Filter 1", [vector]() -> bool {
    std::vector<int> filtered {10, 9, 8, 7, 6};
    return mrt::filter(vector, [](const int& x){ return x > 5; }) == filtered;
  }});
  
  testFramework.addTest({"Test Container Utils - Filter 2", [vector]() -> bool {
    std::vector<int> filtered {1, 2, 3, 0};
    return mrt::filter(vector, [](const int& x){ return x <= 3; }) == filtered;
  }});

  std::map<std::string, int> map {{"test", 1}, {"map", 2}};

  testFramework.addTest({"Test Container Utils - hasKey 1", [map]() -> bool {
    return mrt::hasKey(map, std::string("test"));
  }});
  
  testFramework.addTest({"Test Container Utils - hasKey 2", [map]() -> bool {
    return !mrt::hasKey(map, std::string("abcd"));
  }});
}

