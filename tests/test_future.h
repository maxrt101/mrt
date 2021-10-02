#include <mrt/test/framework.h>
#include <mrt/threads/future.h>
#include <mrt/threads/executor.h>

#include <chrono>
#include <thread>

using namespace  std::literals::chrono_literals;

void test_future_add_tests(mrt::test::TestFramework& testFramework) {
  testFramework.addTest({"Test Future Get", []() -> bool {
    bool result = true;

    mrt::threads::Future<int> f;

    mrt::threads::Executor().run([&f](){
      std::this_thread::sleep_for(1s);
      f.set(100);
    });

    result &= (f.get() == 100);
    
    std::this_thread::sleep_for(2s);

    return result;
  }});
  
  testFramework.addTest({"Test Future Callback", []() -> bool {
    bool result = true;

    mrt::threads::Future<int> f;

    f.onReady([&result](auto value) { result &= (value == 100); });

    mrt::threads::Executor().run([&f](){
      std::this_thread::sleep_for(1s);
      f.set(100);
    });

    std::this_thread::sleep_for(2s);

    return result;
  }});
}

