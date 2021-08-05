#include <mrt/container_utils.h>

#include <iostream>
#include <vector>
#include <map>

template <typename T>
static void printContainer(T container) {
  for (auto& element : container) {
    std::cout << element << " ";
  }
  std::cout << std::endl;
}

int main() {
  std::vector<int> base_vector {1, 10, 2, 9, 3, 8, 4, 7, 5, 6, 0};

  std::cout << "base vector: ";
  printContainer(base_vector);

  std::cout << "filtered by x > 5: ";
  printContainer(mrt::filter(base_vector, [](const int& x){ return x > 5; }));

  std::cout << "filtered by x <= 3: ";
  printContainer(mrt::filter(base_vector, [](const int& x){ return x <= 3; }));
  
  std::map<std::string, int> map { {"test", 1}, {"map", 2} };

  std::cout << "map: ";
  printContainer(map);

  std::cout << "has key 'test': " << mrt::hasKey(map, std::string("test")) << std::endl;
  std::cout << "has key 'abcd': " << mrt::hasKey(map, std::string("abcd")) << std::endl;

  return 0;
}

