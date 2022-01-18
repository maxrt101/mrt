
#include <mrt/test/framework.h>
#include <mrt/test/check.h>
#include <mrt/config.h>

void test_config_add_tests(mrt::test::TestFramework& testFramework) {
  testFramework.addTest({"Test Config", "Test Config Parser", []() -> mrt::test::Result {
    std::string configSource = "[main]\na = 3\n[test]\nb = 6\nc = 10\n#d = 8\n";
    auto config = mrt::Config::fromString(configSource);

    MRT_ASSERT_TRUE(config.getValue("main", "a") == "3", "Config parsing failed");
    MRT_ASSERT_TRUE(config.getValue("test", "b") == "6", "Config parsing failed");
    MRT_ASSERT_TRUE(config.getValueOr("test", "c", "0") == "10", "Config parsing failed");
    MRT_ASSERT_TRUE(config.getValueOr("test", "d", "0") == "0", "Config parsing failed");

    return {true, ""};
  }});
}
