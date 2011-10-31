// -*- c++ -*-

#ifndef P_VECTOR_STREAM_H 
#define P_VECTOR_STREAM_H

#include <ostream>
#include <istream>
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
#include <iostream>

namespace p {
  namespace detail {
    // this is needed because chars should be cast to ints when outputted
    template<typename T>
    struct string_rep {typedef T type;};
    template<> struct string_rep<unsigned char> {typedef int type;};
    template<> struct string_rep<signed char> {typedef int type;};
    
    bool stricmp(const std::string &s1, const std::string &s2) {
      const std::string::size_type smallest = std::min(s1.size(), s2.size());
      for (std::string::size_type i = 0; i < smallest; ++i) {
        if (std::tolower(s1[i]) != std::tolower(s2[i]))
          return false;
      }

      return s1.size() == s2.size();
    }
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
  
  template<typename T, std::size_t size, typename InStream>
  InStream &operator >>(InStream &s, vec<T, size> &v) {
    const std::streampos startPos = s.tellg();
    const std::ios::iostate startState = s.rdstate();
    
    if (s >> v.components[0]) {
      for (std::size_t i = 1; i < size && s.good(); ++i) {
        s >> v.components[i];
      }
    }
    else {
      // it seems we can't parse it as a native type of the vector,
      // so we parse it as a string.
      s.seekg(startPos);
      s.clear(startState);
      std::string meaning;
      
      if (s >> meaning) {
        using detail::stricmp;
        if (stricmp(meaning, "null") || stricmp(meaning, "zero")) {
          for (std::size_t i = 0; i < size; ++i)
            v.components[i] = T();
        }
        else {
          s.clear(std::ios::failbit);
        }
      }
    }
    
    return s;
  }
}

#endif // !P_VECTOR_STREAM_H
