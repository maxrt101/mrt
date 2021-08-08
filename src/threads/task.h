#ifndef _MRT_THREADS_TASK_H_
#define _MRT_THREADS_TASK_H_ 1

#include <functional>

namespace mrt {
namespace threads {

/* Encapsulates function pointer and argument to pass to it */
template <class T = void*>
class Task {
 private:
  std::function<void(T)> m_function;
  T m_arg;

 public:
  inline Task() {}

  template <class F>
  inline Task(F function, T arg = nullptr) : m_function(function), m_arg(arg) {}

  inline void operator()() {
    m_function(m_arg);
  }
};

} // namespace threads
} // namespace mrt

#endif

