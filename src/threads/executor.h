#ifndef _MRT_THREADS_EXECUTOR_H_
#define _MRT_THREADS_EXECUTOR_H_ 1

#include <chrono>
#include <atomic>
#include <thread>
#include <functional>

namespace mrt {
namespace threads {

/* Runs task in a thread */
class Executor {
 public:
  /* Used for releasing an executor, when it's done */
  using Deleter = std::function<void(Executor*)>;

  /* Default behavior is to stop the executior (calls join on the thread) */
  static inline void DefaultDeleter(Executor* object) {
    object->stop();
  }

  /* Detaches the thread */
  static inline void DetachDeleter(Executor* object) {
    object->detach();
  }

 public:
  inline Executor(Deleter deleter = DefaultDeleter)
    : m_deleter(deleter), m_isRunning(false) {}

  /* If the executior is running, calls a deleter */
  inline virtual ~Executor() {
    if (m_isRunning.load()) {
      m_deleter(this);
    }
  }
  
  /* Main executor function, runs threadFunction with args */
  template <class ThreadFunction, class ... Args>
  inline Executor& run(ThreadFunction&& threadFunction, Args&&... args) {
    if (m_isRunning.load()) {
      stop();
    }
    m_isRunning.store(true);
    m_thread = std::thread([threadFunction](Args&&... args) {
      threadFunction(args...);
    }, args...);
    return *this;
  }

  /* Calls join */
  inline void stop() {
    m_isRunning.store(false);
    join();
  }

  /* Joins the thread */
  inline void join() {
    if (m_thread.joinable()) {
      m_thread.join();
    }
  }

  /* Detaches the thread */
  inline void detach() {
    m_thread.detach();
  }

  /* Is executor running */
  inline bool isRunning() {
    return m_isRunning.load();
  }
 
 private:
  std::thread m_thread;
  std::atomic<bool> m_isRunning;
  Deleter m_deleter;
};


/* Starts the task with a delay */
class DelayedExecutor : public Executor {
 public:
  inline DelayedExecutor(Deleter deleter = DefaultDeleter)
    : Executor(deleter) {}

  template <class Duration = std::chrono::seconds,
            class ThreadFunction,
            class ... Args>
  inline DelayedExecutor& run(int delay, ThreadFunction&& threadFunction, Args&&... args) {
    Executor::run(
      [delay, threadFunction](Args&&... args) {
        std::this_thread::sleep_for(Duration(delay));
        threadFunction(args...);
      }, std::forward<Args>(args)...
    );
    return *this;
  }
};


/* Runs the task with intervals */
class IntervalExecutor : public Executor {
 public:
  inline IntervalExecutor(Deleter deleter = DefaultDeleter)
    : Executor(deleter) {}
  
  template <class Duration = std::chrono::seconds,
            class ThreadFunction,
            class ... Args>
  inline IntervalExecutor& run(int interval, ThreadFunction&& threadFunction, Args&&... args) {
    Executor::run(
      [this, interval, threadFunction](Args&&... args) {
        while (this->isRunning()) {
          std::this_thread::sleep_for(Duration(interval));
          threadFunction(args...);
        }
      }, std::forward<Args>(args)...
    );
    return *this;
  }
};

} /* namespace threads */
} /* namespace mrt */

#endif
