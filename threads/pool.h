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

class ThreadPool {
 public:
  ThreadPool();
  ~ThreadPool();

  void AddJob(Job job);

  void WaitForAll();
  void FinishAll();

 private:
  void ThreadWorkerFunction();

 private:
  std::vector<std::thread> pool_;
  std::queue<Job> pending_jobs_;
  std::mutex queue_mutex_;
  std::condition_variable cv_;
  std::atomic<bool> terminate_ = false;
  std::atomic<bool> finish_ = false;
  bool stopped = false;
  const int threads_num_ = std::thread::hardware_concurrency();
};

} // namespace threads
} // namespace mrt

#endif

