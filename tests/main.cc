/**/

#include <mrt/test/framework.h>

#include "test_args.h"
#include "test_container_utils.h"
#include "test_optional.h"
#include "test_future.h"
#include "test_executor.h"
#include "test_pool.h"
#include "test_locked.h"
#include "test_config.h"


int main(int argc, char ** argv) {
  mrt::test::TestFramework testFramework;
  
  test_args_add_tests(testFramework);
  test_container_utils_add_tests(testFramework);
  test_optional_add_tests(testFramework);
  test_future_add_tests(testFramework);
  test_executor_add_tests(testFramework);
  test_pool_add_tests(testFramework);
  test_locked_add_tests(testFramework);
  test_config_add_tests(testFramework);

  bool result = testFramework.run(true);

  return !result;
}

