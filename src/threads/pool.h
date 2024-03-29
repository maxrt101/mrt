#ifndef _MRT_THREADS_THREAD_POOL_H_
#define _MRT_THREADS_THREAD_POOL_H_ 1

#include <queue>
#include <mutex>
#include <thread>
#include <atomic>
#include <vector>
#include <condition_variable>

namespace mrt {

/* ThreadPool class
 * T - task class. Required to have operator()
 * IMPORTANT: ThreadPool needs few ms to start up
 */
template <class T = std::function<void(void)>>
class ThreadPool {
 public:
  inline ThreadPool(int threadsNum = 0) {
    if (threadsNum) m_threadsNum = threadsNum;
    for (int i = 0; i < m_threadsNum; i++) {
      m_pool.push_back(std::thread(&ThreadPool::threadWorker, this));
    }
  }

  inline ~ThreadPool() {
    if (!m_stopped) {
      waitForAll();
    }
  }

  /* Adds task to the queue */
  inline void addTask(T task) {
    {
      std::unique_lock<std::mutex> lock(m_queueMutex);
      m_pendingTasks.push(task);
    }
    m_cv.notify_one();
  }

  /* Waits for all tasks that had started */
  inline void waitForAll() {
    m_finish.store(true);
    terminate();
  }

  /* Executes all tasks in queue, than waits for them to complete */
  inline void finishAll() {
    m_terminate.store(true);
    terminate();
  }

 private:
  /* Function that every thread runs */
  inline void threadWorker() {
    while (1) {
      T task;
      {
        std::unique_lock<std::mutex> lock(m_queueMutex);
        m_cv.wait(lock, [&]{ return !m_pendingTasks.empty() || m_terminate.load() || m_finish.load(); });
        if (m_terminate.load()) return;
        if (m_finish.load() && m_pendingTasks.empty()) return;
        task = m_pendingTasks.front();
        m_pendingTasks.pop();
      }
      task();
    }
  }

  /* Common code for waitForAll & finishAll */
  inline void terminate() {
    m_cv.notify_all();

    for (auto &thread : m_pool) {
      thread.join();
    }

    m_pool.clear();
    m_stopped = true;
  }

 private:
  std::vector<std::thread> m_pool;
  std::queue<T> m_pendingTasks;
  std::mutex m_queueMutex;
  std::condition_variable m_cv;
  std::atomic<bool> m_terminate = false; // true, when waitForAll is called
  std::atomic<bool> m_finish = false;    // true, when finishAll is called
  bool m_stopped = false;
  int m_threadsNum = std::thread::hardware_concurrency();
};

} // namespace mrt

#endif

