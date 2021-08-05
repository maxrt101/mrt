#include <mrt/args/args.h>

#include <iostream>
#include <string>

int main(int argc, const char ** argv) {
  mrt::args::Parser argparser("mrt::args test", {
    {"version", 'F', {"-v", "--version"}, "Shows version"},
    {"int", 'V', {"-i", "--int"}, "Passes int"}
  });

  auto parsed = argparser.parse(argc, argv);

  parsed.ifExists("version", [](const auto& values) {
    std::cout << "Version 1.0\n";
    exit(0);
  });

  parsed.ifExists("int", [](const auto& values) {
    int i = std::stoi(values[0]);
    std::cout << "i: " << i << "\n";
  });

  return 0;
}

