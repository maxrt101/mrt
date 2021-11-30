
#include <mrt/test/framework.h>
#include <mrt/args/args.h>

#include <string>

void test_args_add_tests(mrt::test::TestFramework& testFramework) {
  testFramework.addTest({"Test Args - Flag", "Tests a flag presence", []() -> mrt::test::Result {
    const char* argv[] {"binary", "-v"};

    auto parsed = mrt::args::Parser("Flags Test", {
      {"version", 'F', {"-v", "--version"}, "Shows Version"}
    }).parse(sizeof(argv)/sizeof(argv[0]), argv);

    return {parsed.exists("version"), "The flag is not present"};
  }});

  testFramework.addTest({"Test Args - Value", "Tests if args with values are identified correctly", []() -> mrt::test::Result {
    const char* argv[] {"binary", "-i", "100"};

    auto parsed = mrt::args::Parser("Flags Test", {
      {"int", 'V', {"-i", "--int"}, "Pass int"}
    }).parse(sizeof(argv)/sizeof(argv[0]), argv);

    return {parsed.getFirstOr("int", "") == "100", "The value of an argument is not what it should be"};
  }});

  testFramework.addTest({"Test Args - Positional", "Tests if positional args are identified correctly", []() -> mrt::test::Result {
    const char* argv[] {"binary", "ABC"};

    auto parsed = mrt::args::Parser("Flags Test", {
      {"pos", 'P', {}, "Pass value"}
    }).parse(sizeof(argv)/sizeof(argv[0]), argv);

    return {parsed.getFirstOr("pos", "") == "ABC", "Positional args are not identified correctly"};
  }});
}

