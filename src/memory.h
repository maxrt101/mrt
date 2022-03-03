#ifndef _MRT_MEMORY_H_
#define _MRT_MEMORY_H_ 1

namespace mrt {
namespace mem {

template <typename T>
inline void copy(T* dest, const T* src, size_t size) {
  for (size_t i = 0; i < size; i++) {
    dest[i] = src[i];
  }
}

inline T* copy(const T* src, size_t size) {
  T* buffer = new T[size];
  for (size_t i = 0; i < size; i++) {
    buffer[i] = src[i];
  }
  return buffer;
}

template <typename T>
inline void move(T* dest, T* src, size_t size) {
  for (size_t i = 0; i < size; i++) {
    dest[i] = std::move(src[i]);
  }
}

template <typename T>
inline T* realloc(T* ptr, size_t oldSize, size_t newSize) {
  T* buffer = new T[newSize];
  move(buffer, ptr, oldSize);
  delete [] ptr;
  return buffer;
}

} /* namespace mem */
} /* namespace mrt */

#endif