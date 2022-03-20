#include <mrt/test/framework.h>
#include <mrt/threads/task.h>
#include <mrt/threads/pool.h>

#include <thread>
#include <utility>

using namespace  std::literals::chrono_literals;

extern "C" void setupTests(mrt::test::TestFramework& testFramework) {
  testFramework.addTest({"Test Thread Pool", "Tests if all tasks ran succesfully", []() -> mrt::test::Result {
    mrt::ThreadPool<mrt::Task<int>> pool;

    int sum = 0;

    std::mutex mutex;

    for (int i = 0; i < 5; i++) {
      pool.addTask({[&mutex, &sum](int x) {
        std::unique_lock<std::mutex> lock(mutex);
        sum += x;
      }, i});
    }

    std::this_thread::sleep_for(200ms);
    pool.finishAll();
    return {sum == 10, "Not all tasks executed"};
  }});
}

