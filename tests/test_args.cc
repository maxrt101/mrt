
#include <mrt/test/framework.h>
#include <mrt/args.h>

#include <string>

extern "C" void setupTests(mrt::test::TestFramework& testFramework) {
  testFramework.addTest({"Test Args - Flag", "Tests a flag presence", []() -> mrt::test::Result {
    const char* argv[] {"binary", "-v"};

    auto parsed = mrt::args::Parser("Flags Test", {
      {"version", mrt::args::FLAG, {"-v", "--version"}, "Shows Version"}
    }).parse(sizeof(argv)/sizeof(argv[0]), argv);

    MRT_ASSERT_TRUE(parsed.exists("version"), "The flag is not present");
    return {};
  }});

  testFramework.addTest({"Test Args - Value", "Tests if args with values are identified correctly", []() -> mrt::test::Result {
    const char* argv[] {"binary", "-i", "100"};

    auto parsed = mrt::args::Parser("Flags Test", {
      {"int", mrt::args::WITH_VALUE, {"-i", "--int"}, "Pass int"}
    }).parse(sizeof(argv)/sizeof(argv[0]), argv);

    MRT_ASSERT_EQUAL(parsed.getFirstOr("int", ""), "100", "The value of an argument is not what it should be");
    return {};
  }});

  testFramework.addTest({"Test Args - Positional", "Tests if positional args are identified correctly", []() -> mrt::test::Result {
    const char* argv[] {"binary", "ABC"};

    auto parsed = mrt::args::Parser("Flags Test", {
      {"pos", mrt::args::POSITIONAL, {}, "Pass value"}
    }).parse(sizeof(argv)/sizeof(argv[0]), argv);

    MRT_ASSERT_EQUAL(parsed.getFirstOr("pos", ""), "ABC", "Positional args are not identified correctly");
    return {};
  }});
}

