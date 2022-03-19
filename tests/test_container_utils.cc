
#include <mrt/test/framework.h>
#include <mrt/container_utils.h>

#include <string>
#include <vector>
#include <map>

extern "C" void setupTests(mrt::test::TestFramework& testFramework) {
  std::vector<int> vector {1, 10, 2, 9, 3, 8, 4, 7, 5, 6, 0};

  testFramework.addTest({"Test Container Utils - Filter 1", "Tests filtering of vector with x > 5", [vector]() -> mrt::test::Result {
    std::vector<int> filtered {10, 9, 8, 7, 6};
    return {mrt::filter(vector, [](const int& x){ return x > 5; }) == filtered, "Failed to filter the vector correctly"};
  }});
  
  testFramework.addTest({"Test Container Utils - Filter 2", "Tests filtering of vector with x <= 5", [vector]() -> mrt::test::Result {
    std::vector<int> filtered {1, 2, 3, 0};
    return {mrt::filter(vector, [](const int& x){ return x <= 3; }) == filtered, "Failed to filter the vector correctly"};
  }});

  std::map<std::string, int> map {{"test", 1}, {"map", 2}};
  std::map<std::string, int> map2 {{"test", 2}, {"map", 3}};

  testFramework.addTest({"Test Container Utils - hasKey 1", "Tests if hasKey correctly checks the presence of a key in map", [map]() -> mrt::test::Result {
    return {mrt::hasKey(map, std::string("test")), "hasKey is wrong"};
  }});
  
  testFramework.addTest({"Test Container Utils - hasKey 2", "Tests if hasKey correctly checks the presence of a key in map", [map]() -> mrt::test::Result {
    return {!mrt::hasKey(map, std::string("abcd")), "hasKey is wrong"};
  }});

  testFramework.addTest({"Test Container Utils - Equal 1", "Tests if 2 maps are not equal", [map, map2]() -> mrt::test::Result {
    return {!mrt::equal(map, map2), "mrt::equal is wrong"};
  }});
  
  testFramework.addTest({"Test Container Utils - Equal 2", "Tests if 2 maps are equal", [map]() -> mrt::test::Result {
    return {mrt::equal(map, map), "mrt::equal is wrong"};
  }});
  
  testFramework.addTest({"Test Container Utils - Equal Predicate", "Tests if maps are equal using a predicate", [map, map2]() -> mrt::test::Result {
    return {mrt::equal(map, map2, [](auto a, auto b) { return a.first == b.first; }), "mrt::equal is wrong"};
  }});
}

