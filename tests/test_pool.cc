#include <iostream>
#include <thread>
#include <utility>

#include <mrt/threads/task.h>
#include <mrt/threads/pool.h>

using namespace std::literals::chrono_literals;

auto printer = [](int x) {
  std::cout << x << std::endl;
};

int main() {
  mrt::threads::ThreadPool<mrt::threads::Task<int>> pool;

  for (int i = 0; i < 5; i++) {
    pool.addTask({printer, i});
  }

  std::this_thread::sleep_for(100ms);
  pool.finishAll();
}

