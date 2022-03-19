
#include <mrt/test/framework.h>
#include <mrt/threads/executor.h>

#include <chrono>
#include <thread>

using namespace std::literals::chrono_literals;

extern "C" void setupTests(mrt::test::TestFramework& testFramework) {
  testFramework.addTest({"Test Executor", "Tests executor", []() -> mrt::test::Result {
    int i = 0;

    mrt::threads::Executor().run([&i](){
      std::this_thread::sleep_for(1s);
      i = 1;
    });

    std::this_thread::sleep_for(2s);
    return {i == 1, "Executor failed to execute the task"};
  }});

  testFramework.addTest({"Test Delayed Executor", "Tests delayed execution", []() -> mrt::test::Result {
    int i = 0;

    mrt::threads::DelayedExecutor().run(1, [&i](){
      i = 1;
    });

    std::this_thread::sleep_for(2s);
    return {i == 1, "Executor failed to execute the task"};
  }});

  testFramework.addTest({"Test Interval Executor", "Tests interval execution", []() -> mrt::test::Result {
    int i = 0;

    mrt::threads::IntervalExecutor e;

    e.run(1, [&i](){
      i++;
    });

    std::this_thread::sleep_for(3s);
    e.stop();
    std::this_thread::sleep_for(1s);
    return {i > 0, "Executor failed to execute the task"};
  }});
}

