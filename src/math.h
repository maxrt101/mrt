#ifndef _MRT_MATH_H
#define _MRT_MATH_H

namespace mrt {

template <typename T>
inline int countDigits(T number) {
  int digits = (number < 0) ? 1 : 0;
  while (number) {
    number /= 10;
    digits++;
  }
  return digits;
}

namespace math {
} /* namespace math */
} /* namespace mrt */

#endif