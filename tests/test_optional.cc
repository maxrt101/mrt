#include <mrt/optional.h>

#include <iostream>

Optional<int> GetPositiveInt(int i) {
  if (i < 0) {
    return Optional<int>();
  }
  return Optional<int>(i);
}

int main(int argc, char ** argv) {

  Optional<int> opt_i = GetPositiveInt(10);

  std::cout << "GetPositiveInt(10): " << opt_i.exists() << " " << opt_i.get() << std::endl;

  std::cout << "GetPositiveInt(1).ifExists: ";
  GetPositiveInt(1).ifExists([](int& value) { std::cout << value << "\n"; });
  
  std::cout << "GetPositiveInt(-1).ifNotExists: ";
  GetPositiveInt(-1).ifNotExists([]() { std::cout << "none" << "\n";  });

  return 0;
} 

