#ifndef _MRT_DYNAMIC_LIBRARY_H_
#define _MRT_DYNAMIC_LIBRARY_H_ 1

#include <string>
#include <dlfcn.h>
#include <mrt/log.h>

namespace mrt {

class DynamicLibrary {
 private:
  void* m_handle = nullptr;

 public:
  inline DynamicLibrary() = default;
  inline DynamicLibrary(const DynamicLibrary& rhs) = delete;

  inline DynamicLibrary(const std::string& filename) {
    open(filename);
  }

  inline DynamicLibrary(DynamicLibrary&& rhs) {
    m_handle = rhs.m_handle;
    rhs.m_handle = nullptr;
  }

  inline ~DynamicLibrary() {
    close();
  }

  inline DynamicLibrary& operator=(DynamicLibrary&& rhs) {
    m_handle = rhs.m_handle;
    rhs.m_handle = nullptr;
    return *this;
  }

  inline void* getSymbol(const std::string& symbolName) const {
    return dlsym(m_handle, symbolName.c_str());
  }

  template <typename T>
  inline T getSymbolAs(const std::string& symbolName) const {
    return (T)getSymbol(symbolName);
  }

 private:
  inline int open(const std::string& filename) {
    m_handle = dlopen(filename.c_str(), RTLD_NOW);
    if (!m_handle) {
      #ifndef MRT_NO_ERROR_LOG
        error("Failed to open DynamicLibrary (file '%s'): %s", filename.c_str(), dlerror());
      #endif
      return -1;
    }
    return 0;
  }

  inline void close() {
    if (m_handle) {
      dlclose(m_handle);
      m_handle = nullptr;
    }
  }
};

} /* namespace mrt */

#endif
