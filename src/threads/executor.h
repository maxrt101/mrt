#ifndef _MRT_THREADS_EXECUTOR_H_
#define _MRT_THREADS_EXECUTOR_H_ 1

#include <chrono>
#include <atomic>
#include <thread>
#include <functional>

namespace mrt {
namespace threads {

class Executor {
 public:
  using Deleter = std::function<void(Executor*)>;

  static inline void DefaultDeleter(Executor* object) {
    object->stop();
  }

  static inline void DetachDeleter(Executor* object) {
    object->detach();
  }

 public:
  inline Executor(Deleter deleter = DefaultDeleter)
    : m_deleter(deleter), m_is_running(false) {}

  inline virtual ~Executor() {
    if (m_is_running.load()) {
      m_deleter(this);
    }
  }
  
  template <class ThreadFunction, class ... Args>
  inline Executor& run(ThreadFunction&& thread_function, Args&&... args) {
    if (m_is_running.load()) {
      stop();
    }
    m_is_running.store(true);
    m_thread = std::thread([thread_function](Args&&... args) {
      thread_function(args...);
    }, args...);
    return *this;
  }

  inline void stop() {
    m_is_running.store(false);
    join();
  }

  inline void join() {
    if (m_thread.joinable()) {
      m_thread.join();
    }
  }

  inline void detach() {
    m_thread.detach();
  }

  inline bool isRunning() {
    return m_is_running.load();
  }
 
 private:
  std::thread m_thread;
  std::atomic<bool> m_is_running;
  Deleter m_deleter;
};


class DelayedExecutor : public Executor {
 public:
  inline DelayedExecutor(Deleter deleter = DefaultDeleter)
    : Executor(deleter) {}

  template <class Duration = std::chrono::seconds,
            class ThreadFunction,
            class ... Args>
  inline DelayedExecutor& run(int delay, ThreadFunction&& thread_function, Args&&... args) {
    Executor::run(
      [delay, thread_function](Args&&... args) {
        std::this_thread::sleep_for(Duration(delay));
        thread_function(args...);
      }, std::forward<Args>(args)...
    );
    return *this;
  }
};

class IntervalExecutor : public Executor {
 public:
  inline IntervalExecutor(Deleter deleter = DefaultDeleter)
    : Executor(deleter) {}
  
  template <class Duration = std::chrono::seconds,
            class ThreadFunction,
            class ... Args>
  inline IntervalExecutor& run(int interval, ThreadFunction&& thread_function, Args&&... args) {
    Executor::run(
      [this, interval, thread_function](Args&&... args) {
        while (this->isRunning()) {
          std::this_thread::sleep_for(Duration(interval));
          thread_function(args...);
        }
      }, std::forward<Args>(args)...
    );
    return *this;
  }
};

} /* namespace threads */
} /* namespace mrt */

#endif
