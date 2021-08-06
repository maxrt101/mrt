#ifndef _MRT_OPTIONAL_H_
#define _MRT_OPTIONAL_H_ 1

#include <functional>

namespace mrt {

template <typename T>
class Optional {
 public:
  using value_type = T;
  using function_type = std::function<void(value_type&)>;

 private:
  bool m_exists;
  value_type m_value;

 public:
  inline Optional() : m_exists(false) {}
  inline Optional(value_type value) : m_exists(true), m_value(value) {}

  inline bool exists() { return m_exists; }
  inline value_type& get() { return m_value; }

  inline value_type* operator->() { return &m_value; }
  inline value_type& operator*() { return m_value; }
  inline operator bool() { return m_exists; }

  inline Optional<value_type>& ifExists(function_type function) {
    if (exists()) {
      function(m_value);
    }
    return *this;
  }

  inline Optional<value_type>& ifNotExists(std::function<void()> function) {
    if (!exists()) {
      function();
    }
    return *this;
  }

  inline T returnOrElse(T else_value) {
    if (exists()) {
      return m_value;
    }
    return else_value;
  }
};

} /* namespace mrt */

#endif
