#ifndef MRT_OPTIONAL_H_
#define MRT_OPTIONAL_H_ 1

#include <functional>

template <typename T>
class Optional {
 public:
  using value_type = T;
  using function_type = std::function<void(value_type&)>;

 private:
  bool exists_;
  value_type value_;

 public:
  inline Optional() : exists_(false) {}
  inline Optional(value_type value) : exists_(true), value_(value) {}

  bool Exists() { return exists_; }
  value_type& Get() { return value_; }

  void IfExists(function_type function) {
    if (Exists()) {
      function(Get());
    }
  }

  void IfNotExists(std::function<void()> function) {
    if (!Exists()) {
      function();
    }
  }
};

#endif
