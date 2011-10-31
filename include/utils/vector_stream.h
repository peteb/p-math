// -*- c++ -*-

#ifndef P_VECTOR_STREAM_H 
#define P_VECTOR_STREAM_H

#include <ostream>
#include <cstddef>

#include "vector.h"


/*
 om man skriver till en vec<T, N> så kan den hantera identity och null, samt v1 :: T[v2 :: T .. vN]
 negativa och positiva tal, samt 0.
 
 en color skulle kunna ta in data som hex också.
 s >> color_reader(vec)
 
 istream &s;
 vec ret;
 if (s >> vec_reader(ret, COLOR)) {
 ...
 }
 
 if (optional<vec3> vec = parse_color("#FF00FFAA")) {
 }
 */

namespace p {
  namespace detail {
    // this is needed because chars should be cast to ints when outputted
    template<typename T>
    struct string_rep {typedef T type;};
    template<> struct string_rep<unsigned char> {typedef int type;};
    template<> struct string_rep<signed char> {typedef int type;};
  }
  
  /**
   * Output a vector in the format x[, y[, z[...]]]
   * For example, "123, 43".
   */
  template<typename T, std::size_t size>
  std::ostream &operator <<(std::ostream &s, const vec<T, size> &v) {
    s << v.components[0];
    for (std::size_t i = 1; i < size; ++i)
      s << ", " << typename detail::string_rep<T>::type(v.components[i]);
    
    return s;  
  }  
}

#endif // !P_VECTOR_STREAM_H
