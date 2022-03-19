#include <functional>
#include <mrt/test/framework.h>
#include <mrt/dynamic_library.h>
#include <mrt/args.h>
#include <mrt/log.h>

constexpr char TEST_SETUP_FUNCTION[] = "setupTests";
using TestSetupFunction = void(*)(mrt::test::TestFramework&);

static bool runTests(std::vector<mrt::DynamicLibrary>& tests, std::vector<std::string> testFiles) {
  mrt::test::TestFramework testFramework;
  for (auto& test : testFiles) {
    tests.push_back(mrt::DynamicLibrary(test));
    auto fn = tests.back().getSymbolAs<TestSetupFunction>(TEST_SETUP_FUNCTION);
    if (!fn) {
      mrt::error("Test setup function '%s' is absent in '%s'", TEST_SETUP_FUNCTION, test.c_str());
      return 1;
    }
    fn(testFramework);
  }
  return testFramework.run(true);
}

int main(int argc, const char ** argv) {
  auto parsed = mrt::args::Parser("Flags Test", {
    {"help", mrt::args::FLAG, {"-h", "--help"}, "Shows Help"},
    {"tests", mrt::args::POSITIONAL, {}, "Test .so files", true},
  }).parse(argc, argv);

  if (parsed.exists("help")) {
    printf("mrt tests\nUsage: %s [-h|--help] TEST...\nTEST are one or more test files (.so)\n", argv[0]);
    return 0;
  }

  if (!parsed.exists("tests")) {
    mrt::error("At least one test is required. Use '%s -h' to get help", argv[0]);
    return 1;
  }

  /*
    It is important that TestFramework is destroyed before
    DynamicLibrary that has test lambdas loaded.
    Because if those lambdas are unloaded before TestFramework
    that uses them, segfault will hapen in std::funtction::~function
  */
  std::vector<mrt::DynamicLibrary> tests;
  bool result = runTests(tests, parsed.get("tests"));
  return !result;
}

