#include <iostream>
#include <chrono>
#include <thread>

#include <mrt/threads/executor.h>

using namespace std::literals::chrono_literals;

int main() {
  std::cout << "IntervalExecutor test: (1s delays)" << std::endl;  

  mrt::threads::IntervalExecutor e;

  int count = 0;

  e.run(1, [&count](){
    std::cout << "IntervalExecutor Function: " << count++ << std::endl;
  });

  std::this_thread::sleep_for(5s);
}
