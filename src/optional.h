#ifndef _MRT_OPTIONAL_H_
#define _MRT_OPTIONAL_H_ 1

#include <functional>

namespace mrt {

template <typename T>
class Optional {
 public:
  using ValueType = T;
  using FunctionType = std::function<void(ValueType&)>;

 private:
  bool m_exists;
  ValueType m_value;

 public:
  inline Optional() : m_exists(false) {}
  inline Optional(ValueType value) : m_exists(true), m_value(value) {}

  inline bool exists() { return m_exists; }
  inline ValueType& get() { return m_value; }

  inline ValueType* operator->() { return &m_value; }
  inline ValueType& operator*() { return m_value; }
  inline operator bool() { return m_exists; }

  inline Optional<ValueType>& ifExists(FunctionType function) {
    if (exists()) {
      function(m_value);
    }
    return *this;
  }

  inline Optional<ValueType>& ifNotExists(std::function<void()> function) {
    if (!exists()) {
      function();
    }
    return *this;
  }

  inline T getOrElse(T elseValue) {
    if (exists()) {
      return m_value;
    }
    return elseValue;
  }
};

} /* namespace mrt */

#endif
