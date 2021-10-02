
#include <mrt/test/framework.h>
#include <mrt/args/args.h>

#include <string>

void test_args_add_tests(mrt::test::TestFramework& testFramework) {
  testFramework.addTest({"Test Args - Flag", []() -> bool {
    const char* argv[] {"binary", "-v"};

    auto parsed = mrt::args::Parser("Flags Test", {
      {"version", 'F', {"-v", "--version"}, "Shows Version"}
    }).parse(sizeof(argv)/sizeof(argv[0]), argv);

    return parsed.exists("version");
  }});

  testFramework.addTest({"Test Args - Value", []() -> bool {
    const char* argv[] {"binary", "-i", "100"};

    auto parsed = mrt::args::Parser("Flags Test", {
      {"int", 'V', {"-i", "--int"}, "Pass int"}
    }).parse(sizeof(argv)/sizeof(argv[0]), argv);

    return parsed.getFirstOr("int", "") == "100";
  }});

  testFramework.addTest({"Test Args - Positional", []() -> bool {
    const char* argv[] {"binary", "ABC"};

    auto parsed = mrt::args::Parser("Flags Test", {
      {"pos", 'P', {}, "Pass value"}
    }).parse(sizeof(argv)/sizeof(argv[0]), argv);

    return parsed.getFirstOr("pos", "") == "ABC";
  }});
}

