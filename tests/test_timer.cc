#include <mrt/threads/timer.h>

#include <iostream>

int main() {
  mrt::threads::Timer<std::chrono::seconds> timer;

  timer.run(2, [](int x){
    std::cout << "Timer triggered! (x: " << x << ")" << std::endl;
  }, 100);

  std::this_thread::sleep_for(std::chrono::seconds(10));
}

