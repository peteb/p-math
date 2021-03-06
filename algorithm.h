#ifndef UTILS_ALGORITHM_H
#define UTILS_ALGORITHM_H

#include <algorithm>
#include <utility>

namespace p {
  
  /*
   * Linear interpolation between two values.
   */
  template<typename T, typename Scalar>
  inline T lerp(const T& begin, const T& end, Scalar amount) {
    //T ret = begin + (end - begin) * amount;
    T ret = begin * (Scalar(1.0) - amount) + end * amount; // doesn't require T::operator -
    return ret;
  }
  
  /*
   * Restricts a value to a range
   */
  template<typename T>
  inline T clamp(T val, T vmin, T vmax) {
    using std::max;
    using std::min;
    return max(min(val, vmax), vmin);
  }
  
  template<typename T>
  inline const T& clamp(const T& val, const std::pair<T, T>& range) {
    return clamp(val, range.first, range.second);
  }
  
  /*
   * Forces a value to be between 0.0 and 1.0
   */
  template<typename T>
  inline T saturate(const T& val) {
    return clamp(val, T(0.0), T(1.0));
  }
  
  /*
   * Wraps a value in a range. Useful when dealing with rotations.
   */
  template<typename T>
  inline T wrap(const T& value, const T& lower, const T& upper) {
    T distance = upper - lower;
    T times = floor((value - lower) / distance);
    T ret = value - (times * distance);
    return ret;
  }
  
  template<typename T>
  inline T wrap(const T& value, const std::pair<T, T>& range) {
    return wrap(value, range.first, range.second);
  }
} // !p

#endif //!UTILS_ALGORITHM_H

