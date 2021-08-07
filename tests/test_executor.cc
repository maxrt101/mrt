#include <iostream>
#include <chrono>
#include <thread>

#include <mrt/threads/executor.h>

using namespace std::literals::chrono_literals;

int main() {
  std::cout << "Executor test: (1s delay)" << std::endl;

  mrt::threads::Executor().run([](){
    std::this_thread::sleep_for(1s);
    std::cout << "Executor Function" << std::endl;
  });

  std::this_thread::sleep_for(2s);
}

