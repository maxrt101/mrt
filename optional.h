#ifndef MRT_OPTIONAL_H_
#define MRT_OPTIONAL_H_ 1

#include <functional>

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

  bool exists() { return m_exists; }
  value_type& get() { return m_value; }

  void ifExists(function_type function) {
    if (exists()) {
      function(get());
    }
  }

  void ifNotExists(std::function<void()> function) {
    if (!exists()) {
      function();
    }
  }
};

#endif
