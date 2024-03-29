#ifndef _MRT_THREADS_FUTURE_H_
#define _MRT_THREADS_FUTURE_H_ 1

#include <condition_variable>
#include <atomic>
#include <thread>
#include <vector>
#include <mutex>

#include <mrt/threads/executor.h>

namespace mrt {

template <class T>
class Future {
 public:
  using Callback = std::function<void(T&)>;

 public:
  inline Future() : m_is_ready(false) {}

  /* Is future ready? */
  inline bool isReady() const {
    return m_is_ready.load();
  }

  /* Adds a callback, that will be called, when the resource is ready */
  inline void onReady(Callback callback) {
    std::unique_lock<std::mutex> lock(m_callback_mutex);
    m_callbacks.push_back(callback);
  } 

  /* Returns the future's value, blocks if it's not ready */
  inline T& get() {
    std::unique_lock<std::mutex> lock(m_value_mutex);
    m_cv.wait(lock, [&](){ return m_is_ready.load(); });
    return m_value;
  }

  /* Sets the future's value, calls all onReady callbacks */
  inline void set(const T& value) {
    {
      std::unique_lock<std::mutex> lock(m_value_mutex);
      m_value = value;
      m_is_ready.store(true);
    }
    m_cv.notify_all();
    std::unique_lock<std::mutex> lock(m_callback_mutex);
    for (auto& cb : m_callbacks) {
      cb(m_value);
    }
  }

  inline void set(T&& value) {
    {
      std::unique_lock<std::mutex> lock(m_value_mutex);
      m_value = std::move(value);
      m_is_ready.store(true);
    }
    m_cv.notify_all();
    std::unique_lock<std::mutex> lock(m_callback_mutex);
    for (auto& cb : m_callbacks) {
      cb(m_value);
    }
  }

 private:
  std::atomic<bool> m_is_ready;
  std::condition_variable m_cv;
  std::mutex m_value_mutex;
  std::mutex m_callback_mutex;
  T m_value;
  std::vector<Callback> m_callbacks;
};

} /* namespace mrt */

#endif
