#ifndef UTILS_ALGORITHM_H
#define UTILS_ALGORITHM_H

#include <algorithm>

namespace p {

template<typename T, typename Scalar>
T lerp(T begin, T end, Scalar amount) {
  return begin + (end - begin) * amount;
}


template<typename T>
T clamp(T val, T min, T max) {
  return std::max(std::min(val, max), min);
}

template<typename T>
T saturate(T val) {
  return clamp(val, static_cast<T>(0.0), static_cast<T>(1.0));
}

template<typename T>
inline T wrap(T value, T lower, T upper) {
  T distance = upper - lower;
  T times = floor((value - lower) / distance);
  return value - (times * distance);
}

} // !p

#endif //!UTILS_ALGORITHM_H

