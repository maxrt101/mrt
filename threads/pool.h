#ifndef THREAD_POOL_THREAD_POOL_H_
#define THREAD_POOL_THREAD_POOL_H_

#include <queue>
#include <mutex>
#include <thread>
#include <atomic>
#include <vector>
#include <condition_variable>

#include "job.h"

namespace mrt {
namespace threads {

/* ThreadPool class */
class ThreadPool {
 public:
  ThreadPool(int threads_num = 0);
  ~ThreadPool();

  /* Adds job to the queue */
  void AddJob(Job job);

  /* Waits for all jobs that had started */
  void WaitForAll();

  /* Executes all jobs in queue, than waits for them to complete */
  void FinishAll();

 private:
  /* Function that every thread runs */
  void ThreadWorkerFunction();

  /* Common code for WaitForAll & FinishAll */
  void Terminate();

 private:
  std::vector<std::thread> pool_;
  std::queue<Job> pending_jobs_;
  std::mutex queue_mutex_;
  std::condition_variable cv_;
  std::atomic<bool> terminate_ = false; // true, when WaitForAll is called
  std::atomic<bool> finish_ = false;    // true, when FinishAll is called
  bool stopped = false;
  int threads_num_ = std::thread::hardware_concurrency();
};

} // namespace threads
} // namespace mrt

#endif

