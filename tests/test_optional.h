#include <mrt/test/framework.h>
#include <mrt/optional.h>

mrt::Optional<int> get_positive_int(int i) {
  if (i < 0) {
    return mrt::Optional<int>();
  }
  return mrt::Optional<int>(i);
}

void test_optional_add_tests(mrt::test::TestFramework& testFramework) {
  testFramework.addTest({"Test Optional", "Tests various optional methods", []() -> mrt::test::Result {
    auto opt_i = get_positive_int(10);
    
    if (!opt_i.exists()) return {false, "mrt::Optional::exists failed"};
    if (!opt_i) return {false, "mrt::Optional::bool failed"};
    if (opt_i.get() != 10) return {false, "mrt::Optional::get failed"};
    if (*opt_i != 10) return {false, "mrt::Optional::get failed"};

    bool result = true;

    get_positive_int(1).ifExists([&result](auto value) { result &= (value == 1); });
    get_positive_int(-1).ifNotExists([&result]() { result &= true; });

    if (mrt::Optional<int>(5).returnOrElse(10) != 5) return {false, "mrt::Optional::returnOrElse failed"};
    if (mrt::Optional<int>().returnOrElse(10) != 10) return {false, "mrt::Optional::returnOrElse failed"};

    return {result, "Optional tests failed"};
  }});
}

