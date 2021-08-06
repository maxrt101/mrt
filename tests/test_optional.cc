#include <mrt/optional.h>

#include <iostream>

mrt::Optional<int> GetPositiveInt(int i) {
  if (i < 0) {
    return mrt::Optional<int>();
  }
  return mrt::Optional<int>(i);
}

int main(int argc, char ** argv) {

  auto opt_i = GetPositiveInt(10);

  std::cout << "opt_i = GetPositiveInt(10):" << std::endl
            << "  opt_i.exists(): " << opt_i.exists() << std::endl
            << "  opt_i.get(): " << opt_i.get() << std::endl
            << "  opt_i::operator bool(): " << (bool)opt_i << std::endl
            << "  opt_i::operator *(): " << *opt_i << std::endl;

  std::cout << "ifExists: ";
  GetPositiveInt(1).ifExists([](auto value) {
    std::cout << "ifExists lambda called: " << value << std::endl;
  });
  
  std::cout << "ifNotExists: ";
  GetPositiveInt(-1).ifNotExists([]() {
    std::cout << "ifNotExists lambda called" << std::endl;
  });

  std::cout << "returnOrElse: " << mrt::Optional<int>(5).returnOrElse(10) << std::endl;
  std::cout << "returnOrElse: " << mrt::Optional<int>().returnOrElse(10) << std::endl;

  return 0;
} 

