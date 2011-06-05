#ifndef UTILS_ALGORITHM_H
#define UTILS_ALGORITHM_H

#include <algorithm>

template<typename T, typename Scalar>
T lerp(const T &begin, const T &end, Scalar amount) {
  return begin + (end - begin) * amount;
}

template<typename ValueT>
ValueT clamp(const ValueT &val, const ValueT &min, const ValueT &max) {
  return std::max(std::min(val, max), min);
}

inline double wrap(double value, double lower, double upper) {
  double distance = upper - lower;
  double times = floor((value - lower) / distance);
  return value - (times * distance);
}

#endif //!UTILS_ALGORITHM_H

