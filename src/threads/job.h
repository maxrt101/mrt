#ifndef _MRT_THREADS_JOB_H_
#define _MRT_THREADS_JOB_H_ 1

#include <functional>

namespace mrt {
namespace threads {

/* Type of job worker function */
typedef std::function<void(void*)> JobFunction;

/* Encapsulates function pointer and argument to pass to it */
struct Job {
  JobFunction function;
  void* arg;

  Job();
  Job(JobFunction function, void* arg);

  void run();
};

} // namespace threads
} // namespace mrt

#endif

