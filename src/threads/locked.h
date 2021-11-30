#ifndef _MRT_THREADS_LOCKED_H_
#define _MRT_THREADS_LOCKED_H_ 1

#include <mutex>
#include <vector>
#include <functional>

namespace mrt {
namespace threads {

template <typename T>
class Locked {
 public:
  using FunctionType = std::function<void(T&)>;

 private:
  T m_data;
  std::mutex m_mutex;
  std::vector<FunctionType> m_callbacks;

 public:
  inline Locked() {}
  inline Locked(const T& data) : m_data(data) {}

  inline T& get() {
    std::unique_lock<std::mutex> lock(m_mutex);
    return m_data;
  }

  inline void set(const T& data) {
    std::unique_lock<std::mutex> lock(m_mutex);
    m_data = data;
  }

  void onUpdate(FunctionType f) {
    m_callbacks.push_back(f);
  }

  void update(FunctionType f) {
    std::unique_lock<std::mutex> lock(m_mutex);
    f(m_data);
    for (auto& cb : m_callbacks) {
      cb(m_data);
    }
  }
};

} /* namespace threads */
} /* namespace mrt */

#endif
