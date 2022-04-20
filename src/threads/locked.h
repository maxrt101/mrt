#ifndef _MRT_THREADS_LOCKED_H_
#define _MRT_THREADS_LOCKED_H_ 1

#include <mutex>
#include <vector>
#include <functional>

namespace mrt {

template <typename T>
class Locked {
 public:
  using Callback = std::function<void(const T&)>;
  using Updater = std::function<void(T&)>;

 private:
  T m_data;
  std::mutex m_mutex;
  std::vector<Callback> m_callbacks;

 public:
  inline Locked() {}
  inline Locked(const T& data) : m_data(data) {}

  /* Returns ref to underlying resource, blocks if the resource is in use */
  inline T& get() {
    std::unique_lock<std::mutex> lock(m_mutex);
    return m_data;
  }

  /* Sets an underlying resource, blocks if the resource is used */
  inline void set(const T& data) {
    std::unique_lock<std::mutex> lock(m_mutex);
    m_data = data;
  }

  /* Adds a callback for resource update */
  void onUpdate(Callback f) {
    m_callbacks.push_back(f);
  }

  /* Updates a resource value, using a function of type void(T&) */
  void update(Updater f) {
    std::unique_lock<std::mutex> lock(m_mutex);
    f(m_data);
    for (auto& cb : m_callbacks) {
      cb(m_data);
    }
  }

  /* Perform an operation with resource locked */
  void withLocked(Updater f) {
    std::unique_lock<std::mutex> lock(m_mutex);
    f(m_data);
  }

  /* Locks underlying mutex */
  void lock() {
    m_mutex.lock();
  }

  /* Unlocks underlying mutex */
  void unlock() {
    m_mutex.unlock();
  }

  /* Creates a unique lock */
  std::unique_lock<std::mutex> createLock() {
    return std::move(std::unique_lock<std::mutex>(m_mutex));
  }
};

} /* namespace mrt */

#endif
