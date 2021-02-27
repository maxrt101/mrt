#ifndef THREAD_POOL_THREADS_JOB_H_
#define THREAD_POOL_THREADS_JOB_H_

#include <functional>

namespace mrt {
namespace threads {

typedef std::function<void(void*)> JobFunction;

struct Job {
  JobFunction function;
  void* arg;

  Job();
  Job(JobFunction function, void* arg);
  void Run();
};

} // namespace threads
} // namespace mrt

#endif

