#include <mrt/test/framework.h>
#include <mrt/test/check.h>
#include <mrt/threads/locked.h>

#include <thread>

extern "C" void setupTests(mrt::test::TestFramework& testFramework) {
  testFramework.addTest({"Test Locked", "Tests if mrt::Locked works correctly", []() -> mrt::test::Result {

    mrt::Locked<int> resource(1);

    MRT_ASSERT_TRUE(resource.get() == 1, "mrt::Locked::get returns invalid value");
    
    resource.set(20);
    MRT_ASSERT_TRUE(resource.get() == 20, "mrt::Locked::set doesn't work");

    resource.onUpdate([](const int& value) {
      MRT_ASSERT_TRUE(value == 100, "mrt::Locked::onUpdate doesn't work");
    });

    resource.update([](int& value) {
      value = 100;
    });
    MRT_ASSERT_TRUE(resource.get() == 100, "mrt::Locked::update doesn't work");

    resource.withLocked([](int& value) {
      value = 120;
    });
    MRT_ASSERT_TRUE(resource.get() == 120, "mrt::Locked::withLocked doesn't work");

    /*
      TODO create 2 threads that will try to change the value at the same time
      and test withLocked, lock, unlock, createLock
    */

    return {};
  }});
}

