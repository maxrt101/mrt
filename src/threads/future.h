#ifndef _MRT_THREADS_FUTURE_H_
#define _MRT_THREADS_FUTURE_H_ 1

#include <atomic>
#include <thread>
#include <mutex>
#include <condition_variable>

#include <mrt/threads/executor.h>

namespace mrt {
namespace threads {

template <class T>
class Future {
 public:
  using Callback = std::function<void(T&)>;

 public:
  inline Future() : m_is_ready(false) {}

  inline bool isReady() const {
    return m_is_ready.load();
  }

  inline void onReady(Callback callback) {
    m_callback = callback;
  } 

  inline T& get() {
    std::unique_lock<std::mutex> lock(m_value_mutex);
    m_cv.wait(lock, [&](){ return m_is_ready.load(); });
    return m_value;
  }

  inline void set(T&& value) {
    {
      std::unique_lock<std::mutex> lock(m_value_mutex);
      m_value = std::move(value);
      m_is_ready.store(true);
    }
    m_cv.notify_all();
    m_callback(m_value);
  }

 private:
  std::atomic<bool> m_is_ready;
  std::condition_variable m_cv;
  std::mutex m_value_mutex;
  T m_value;
  Callback m_callback;
};

} /* namespace threads */
} /* namespace mrt */

#endif
