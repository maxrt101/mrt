#ifndef MRT_THREADS_THREAD_POOL_H_
#define MRT_THREADS_THREAD_POOL_H_ 1

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
  void addJob(Job job);

  /* Waits for all jobs that had started */
  void waitForAll();

  /* Executes all jobs in queue, than waits for them to complete */
  void finishAll();

 private:
  /* Function that every thread runs */
  void threadWorkerFunction();

  /* Common code for WaitForAll & FinishAll */
  void terminate();

 private:
  std::vector<std::thread> m_pool;
  std::queue<Job> m_pending_jobs;
  std::mutex m_queue_mutex;
  std::condition_variable m_cv;
  std::atomic<bool> m_terminate = false; // true, when WaitForAll is called
  std::atomic<bool> m_finish = false;    // true, when FinishAll is called
  bool m_stopped = false;
  int m_threads_num = std::thread::hardware_concurrency();
};

} // namespace threads
} // namespace mrt

#endif

