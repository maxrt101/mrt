#include <mrt/test/framework.h>
#include <mrt/test/check.h>
#include <mrt/threads/locked.h>

#include <thread>

void test_locked_add_tests(mrt::test::TestFramework& testFramework) {
  testFramework.addTest({"Test Locked", "Tests if mrt::Locked works correctly", []() -> mrt::test::Result {

    mrt::threads::Locked<int> resource(1);

    MRT_ASSERT_TRUE(resource.get() == 1, "mrt::threads::Locked::get returns invalid value");
    
    resource.set(20);
    MRT_ASSERT_TRUE(resource.get() == 20, "mrt::threads::Locked::set doesn't work");

    resource.onUpdate([](const int& value) {
      MRT_ASSERT_TRUE(value == 100, "mrt::threads::Locked::onUpdate doesn't work");
    });

    resource.update([](int& value) {value = 100;});
    MRT_ASSERT_TRUE(resource.get() == 100, "mrt::threads::Locked::update doesn't work");

    return {};
  }});
}

