#include <iostream>
#include <chrono>
#include <thread>

#include <mrt/threads/executor.h>

using namespace std::literals::chrono_literals;

int main() {
  std::cout << "DelayedExecutor test: (1s delay)" << std::endl;

  mrt::threads::DelayedExecutor().run(1, [](){
    std::cout << "DelayedExecutor Function" << std::endl;
  });

  std::this_thread::sleep_for(2s);
}

