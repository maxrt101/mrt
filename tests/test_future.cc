#include <iostream>
#include <chrono>

#include <mrt/threads/future.h>
#include <mrt/threads/executor.h>

using namespace  std::literals::chrono_literals;

int main() {
  mrt::threads::Future<int> f;

  f.onReady([](auto value) {std::cout << "Callback: " << value << std::endl; });

  mrt::threads::Executor().run([&f](){
    std::this_thread::sleep_for(1s);
    f.set(100);
  });

  std::cout << "Main thread: Future<int>::get: " << f.get() << std::endl;

  std::this_thread::sleep_for(2s);
}

