#ifndef _MRT_THREADS_TIMER_H_
#define _MRT_THREADS_TIMER_H_ 1

#include <atomic>
#include <chrono>
#include <thread>
#include <functional>

namespace mrt {
namespace threads {

template <typename DurationType = std::chrono::milliseconds>
class Timer {
 public:
  inline Timer() : m_running(false) {}

  inline ~Timer() {
    if (m_running.load()) {
      stop();
    }
  }

  template <typename F, typename ... Args>
  void run(int interval, F cb, Args&&... args) {
    if (m_running.load()) {
      stop();
    }
    m_running.store(true);
    m_thread = std::thread([this, interval, cb, args...](){
      while (m_running.load()) {
        cb(args...);
        std::this_thread::sleep_for(DurationType(interval));
      }
    });
  }
  
  void stop() {
    m_running.store(false);
    if (m_thread.joinable()) {
      m_thread.join();
    }
  }

  bool isRunning() const {}

 private:
  std::thread m_thread;
  std::atomic<bool> m_running;
};

} /* threads */
} /* mrt */

#endif
