#ifndef UTILS_VECTOR_H
#define UTILS_VECTOR_H

#include <sstream>
#include <string>
#include <iostream>
#include <algorithm>
#include <functional>
#include <cassert>
#include <cmath>
#include <ostream>
#include <numeric>

#include <utils/algorithm.h>

// TODO: add header here for some simple use cases
// just do this, add some tests, and be done with it

namespace p {
  
  // The base classes
  #pragma mark - Basic vector types

  template<typename T, std::size_t size>
  struct vec {
    explicit vec(T val) {std::fill(components, components + size, val);}
    explicit vec(T *values) {std::copy(values, values + size, components);}
    
    typedef T value_type;
    
    T components[size];
  };

  template<typename T>
  struct vec<T, 0> {};
  
  template<typename T>
  struct vec<T, 2> {
    vec() {}
    explicit vec(T x, T y) : x(x), y(y) {}
    explicit vec(T val) {std::fill(components, components + 2, val);}
    explicit vec(T *values) {std::copy(values, values + 2, components);}
    
    typedef T value_type;

    union {
      struct {T x, y; };
      struct {T u, v; };
      struct {T components[2]; };
    };
  };
  
  
  template<typename T>
  struct vec<T, 3> {  // enable_if<is_pod<T> >
    vec() {}
    explicit vec(T x, T y, T z) : x(x), y(y), z(z) {}
    explicit vec(T val) {std::fill(components, components + 3, val);}
    explicit vec(T *values) {std::copy(values, values + 3, components);}
    
    typedef T value_type;

    union {
      struct {T x, y, z; };
      struct {T u, v, s; };
      struct {T components[3]; };
    };
  };

  template<typename T>
  struct vec<T, 4> {
    vec() {}
    explicit vec(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}
    explicit vec(T val) {std::fill(components, components + 4, val);}
    explicit vec(T *values) {std::copy(values, values + 4, components);}

    typedef T value_type;

    union {
      struct {T x, y, z, w; };
      struct {T u, v, s, t; };
      struct {T components[4]; };
    };
  };

  


  
  // helpers
  #pragma mark - Helpers
  
  // this is needed because chars should be cast to ints when outputted
  template<typename T>
  struct string_rep {typedef T type;};
  template<> struct string_rep<unsigned char> {typedef std::size_t type;};
  
  template<typename T, std::size_t size>
  std::ostream &operator <<(std::ostream &s, const vec<T, size> &v) {
    s << v.components[0];
    for (std::size_t i = 1; i < size; ++i)
      s << ", " << typename string_rep<T>::type(v.components[i]);
    
    return s;  
  }
  
  template<typename T, std::size_t size, typename OpT>
  inline vec<T, size> transform(const vec<T, size> &lhs, const vec<T, size> &rhs, OpT op) {
    vec<T, size> ret;
    std::transform(lhs.components, lhs.components + size, rhs.components, ret.components, op);
    return ret;
  }


  // operator overloads
  #pragma mark - Operators
  template<typename T, std::size_t size> 
  inline vec<T, size> operator +(const vec<T, size> &lhs, const vec<T, size> &rhs) {
    vec<T, size> ret = transform(lhs, rhs, std::plus<T>());
    return ret;
  }

  template<typename T, std::size_t size> 
  inline vec<T, size> operator -(const vec<T, size>& lhs, const vec<T, size>& rhs) {
    vec<T, size> ret = transform(lhs, rhs, std::minus<T>());
    return ret;
  }

  template<typename T, std::size_t size, typename Scalar> 
  inline vec<T, size>& operator *=(vec<T, size>& lhs, Scalar rhs) {
    lhs = lhs * vec<T, size>(rhs);
    return lhs;
  }

  template<typename T, std::size_t size>
  inline vec<T, size> operator -(const vec<T, size> &rhs) {
    vec<T, size> ret;
    for (std::size_t i = 0; i < size; ++i)
      ret.components[i] = -rhs.components[i];
    return ret;
  }

  template<typename T, std::size_t size, typename Scalar> 
  inline vec<T, size> operator *(const vec<T, size> &lhs, Scalar rhs) {
    vec<T, size> ret = transform(lhs, vec<T, size>(rhs), std::multiplies<T>());
    return ret;
  }

  template<typename T, std::size_t size, typename Scalar> 
  inline vec<T, size> operator /(const vec<T, size>& lhs, Scalar rhs) {
    vec<T, size> ret = transform(lhs, vec<T, size>(rhs), std::divides<T>());
    return ret;
  }
  
  template<typename T, std::size_t size, typename Scalar> 
  inline vec<T, size>& operator /=(vec<T, size>& lhs, Scalar rhs) {
    lhs = lhs / vec<T, size>(rhs);
    return lhs;
  }

  template<typename T, std::size_t size> 
  inline vec<T, size>& operator +=(vec<T, size>& lhs, const vec<T, size>& rhs) {
    lhs = lhs + rhs;
    return lhs;
  }
  
  template<typename T, std::size_t size> 
  inline vec<T, size>& operator -=(vec<T, size>& lhs, const vec<T, size>& rhs) {
    lhs = lhs - rhs;
    return lhs;
  }

  
  // some more helpers
  template<typename T>
  struct min_fun {
    T operator()(T lhs, T rhs) const {using std::min; return min(lhs, rhs); }
  };

  template<typename T>
  struct max_fun {
    T operator()(T lhs, T rhs) const {using std::max; return max(lhs, rhs); }
  };

  
  // algorithm overloads
  #pragma mark - Algorithms

  template<typename T, std::size_t size>
  inline vec<T, size> min(const vec<T, size> &v1, const vec<T, size> &v2) {
    vec<T, size> ret = transform(v1, v2, min_fun<T>());
    return ret;
  }

  template<typename T, std::size_t size>
  inline vec<T, size> max(const vec<T, size> &v1, const vec<T, size> &v2) {
    vec<T, size> ret = transform(v1, v2, max_fun<T>());
    return ret;
  }

  
  template<typename T, std::size_t size>
  inline T dot_product(const vec<T, size> &v1, const vec<T, size> &v2) {
    return std::inner_product(v1.components, v1.components + size, v2.components, T(0));
  }
  
  
  template<typename T, std::size_t size>
  inline T magnitude(const vec<T, size> &v) {
    T sumSquared = std::inner_product(v.components, v.components + size, v.components, T(0));
    return sqrt(sumSquared);
  }
  
  // very generic normalize function
  template<typename T>
  inline void normalize(T &v) {
    v /= magnitude(v);
  }

  template<typename T>
  inline T normalized(const T &v) {
    T ret = v / magnitude(v);
    return ret;
  }

  typedef vec<float, 2> vec2;
  typedef vec<float, 3> vec3;
  typedef vec<float, 4> vec4;
  typedef vec<int, 2> ivec2;
  typedef vec<int, 3> ivec3;
  typedef vec<int, 4> ivec4;
  typedef vec<unsigned char, 3> ubvec3;
  typedef vec<unsigned char, 4> ubvec4;

  // TODO: color
  
} // !p

#endif // !UTILS_VECTOR_H

