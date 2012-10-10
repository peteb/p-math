/* -- vector.h -------------------------------------------------------*- c++ -*-
 *
 * Accessing components
 *   through []-operator
 *   casting to pointer
 *   .x/.r/.s
 *   .y/.g/.t
 *   .z/.b/.p
 *   .w/.a/.q
 *
 * Operations that can be done on vectors:
 *   - + * / += -= *= /= min max transform dot_product cross_product normalize
 *   magnitude
 * -------------------------------------------------------------------------- */

#ifndef P_UTILS_VECTOR_H
#define P_UTILS_VECTOR_H

#include <algorithm>
#include <functional>
#include <cassert>
#include <cmath>
#include <numeric>
#include <memory>
#include <cstring>
#include <type_traits>
#include <string>

namespace p {
  // TODO: make sure clamp, lerp, etc. work with vectors
  // TODO: a cmake config for c++11
  
  /**
   * The general case.
   */
  template<typename T, std::size_t N>
  struct vec {
    typedef T value_type;
    static const std::size_t size = N;

    vec() = default;
    vec(const vec &) = default;
    vec(T s) {
      std::fill(std::begin(components), std::begin(components) + N, s);
    }
    
    inline T &operator [](std::size_t pos) {return components[pos];}
    inline T operator [](std::size_t pos) const {return components[pos];}
  
    T components[size];
  };

  /**
   * Undefined deranged case of a vector.
   */
  template<typename T>
  struct vec<T, 0> {private: vec();};
  
  /**
   * Specialization of vector for 2 components, which must be POD type.
   */
  template<typename T>
  struct vec<T, 2> {
    typedef T value_type;
    static const std::size_t size = 2;

    vec() = default;
    vec(const vec &) = default;
    vec(T x, T y) : x(x), y(y) {}
    vec(T s) : x(s), y(s) {}
    
    inline T &operator [](std::size_t pos) {return components[pos];}
    inline T operator [](std::size_t pos) const {return components[pos];}
    
    union {
      struct {T x, y; };
      struct {T s, t; };
      struct {T components[size]; };
    };
  };

  
  /**
   * Specialization of vector for 3 components, which must be POD type.
   */
  template<typename T>
  struct vec<T, 3> {
    typedef T value_type;
    static const std::size_t size = 3;

    vec() = default;
    vec(const vec &) = default;
    vec(T x, T y, T z) : x(x), y(y), z(z) {}
    vec(T s) : x(s), y(s), z(s) {}  // TODO: what? why :(
    
    inline T &operator [](std::size_t pos) {return components[pos];}
    inline T operator [](std::size_t pos) const {return components[pos];}

    union {
      struct {T x, y, z; };
      struct {T s, t, p; };
      struct {T r, g, b; };
      struct {T components[size]; };
    };
  };
  
  /**
   * Specialization of vector for 4 components, which must be POD type.
   */
  template<typename T>
  struct vec<T, 4> {
    typedef T value_type;
    static const int size = 4;

    vec() = default;
    vec(const vec &) = default;
    vec(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}
    vec(T s) : vec(s, s, s) {}
    
    inline T &operator [](std::size_t pos) {return components[pos];}
    inline T operator [](std::size_t pos) const {return components[pos];}
    
    union {
      struct {T x, y, z, w; };
      struct {T s, t, p, q; };
      struct {T r, g, b, a; };
      struct {T components[size]; };
    };    
  };


  template<typename T, std::size_t size, typename opT>
  inline vec<T, size> transform(const vec<T, size> &lhs,
                                const vec<T, size> &rhs, opT op) {
    vec<T, size> ret;
    std::transform(lhs.components, lhs.components + size,
                   rhs.components, ret.components, op);
    return ret;
  }
  
  template<typename T>
  struct min_fun {
    T operator()(T lhs, T rhs) const {using std::min; return min(lhs, rhs); }
  };
  
  template<typename T>
  struct max_fun {
    T operator()(T lhs, T rhs) const {using std::max; return max(lhs, rhs); }
  };
  
  template<typename T, std::size_t size, typename OpT>
  inline T foldl(const vec<T, size> &v, OpT op) {
    T val = v[0];
    for (std::size_t i = 1; i < size; ++i)
      val = op(val, v[i]);

    return val;
  };
  
  
  /**
   * Unary minus; component-wise negation.
   */
  template<typename T, std::size_t size>
  inline vec<T, size> operator -(const vec<T, size> &rhs) {
    vec<T, size> ret;
    for (std::size_t i = 0; i < size; ++i)
      ret[i] = -rhs[i];
    return ret;
  }

  template<typename T, std::size_t size> 
  inline vec<T, size> operator +(const vec<T, size> &lhs,
                                 const vec<T, size> &rhs) {
    return transform(lhs, rhs, std::plus<T>());
  }

  template<typename T, std::size_t size> 
  inline vec<T, size> operator -(const vec<T, size>& lhs,
                                 const vec<T, size>& rhs) {
    return transform(lhs, rhs, std::minus<T>());
  }

  template<typename T, std::size_t size, typename ScalarT> 
  inline vec<T, size> operator *(const vec<T, size> &lhs, ScalarT rhs) {
    return transform(lhs, vec<T, size>(rhs), std::multiplies<T>());
  }
  
  template<typename T, std::size_t size, typename scalarT> 
  inline vec<T, size> operator /(const vec<T, size>& lhs, scalarT rhs) {
    return transform(lhs, vec<T, size>(rhs), std::divides<T>());
  }
  
  template<typename T, std::size_t size> 
  inline vec<T, size>& operator +=(vec<T, size>& lhs, const vec<T, size>& rhs) {
    lhs = lhs + rhs; return lhs;
  }
  
  template<typename T, std::size_t size> 
  inline vec<T, size>& operator -=(vec<T, size>& lhs, const vec<T, size>& rhs) {
    lhs = lhs - rhs; return lhs;
  }

  template<typename T, std::size_t size, typename scalarT> 
  inline vec<T, size>& operator *=(vec<T, size>& lhs, scalarT rhs) {
    lhs = lhs * rhs; return lhs;
  }

  template<typename T, std::size_t size, typename scalarT> 
  inline vec<T, size>& operator /=(vec<T, size>& lhs, scalarT rhs) {
    lhs = lhs / rhs; return lhs;
  }

  
  /**
   * Component-wise minimum.
   */
  template<typename T, std::size_t size>
  inline vec<T, size> min(const vec<T, size> &v1, const vec<T, size> &v2) {
    return transform(v1, v2, min_fun<T>());
  }

  /**
   * Component-wise maximum.
   */
  template<typename T, std::size_t size>
  inline vec<T, size> max(const vec<T, size> &v1, const vec<T, size> &v2) {
    return transform(v1, v2, max_fun<T>());
  }

  template<typename T, std::size_t size>
  inline T max(const vec<T, size> &v) {
    using std::max;
    return foldl(v, max_fun<T>());
  }

  template<typename T, std::size_t size>
  inline vec<T, size> abs(const vec<T, size> &v) {
    using std::abs;
    vec<T, size> r;
    for (std::size_t i = 0; i < size; ++i)
      r[i] = abs(v[i]);
    return r;
  }
  
  template<typename T, std::size_t size>
  inline T dot_product(const vec<T, size> &v1, const vec<T, size> &v2) {
    return std::inner_product(v1.components, v1.components + size,
                              v2.components, T());
  }
  
  template<typename T>
  inline vec<T, 3> cross_product(const vec<T, 3> &v1, const vec<T, 3> &v2) {
    return vec<T, 3>(v1.y * v2.z - v2.y * v1.z,
                     v1.z * v2.x - v2.z * v1.x,
                     v1.x * v2.y - v2.x * v1.y);
  }
  
  template<typename T, std::size_t size>
  inline T magnitude(const vec<T, size> &v) {
    T sumSquared = dot_product(v, v);
    return sqrt(sumSquared);
  }
  
  template<typename T>
  inline T normalize_fast(const T &v) {
    T ret = v / magnitude(v);
    return ret;
  }

  template<typename T>
  inline T normalize(const T &v) {
    return normalize_fast(v);
  }
  
  typedef vec<float, 2> vec2;
  typedef vec<float, 3> vec3;
  typedef vec<float, 4> vec4;
  typedef vec<int, 2> ivec2;
  typedef vec<int, 3> ivec3;
  typedef vec<int, 4> ivec4;
  typedef vec<unsigned char, 3> ubvec3;
  typedef vec<unsigned char, 4> ubvec4;

  
  static_assert(std::is_pod<vec2>::value, "should be POD");
  static_assert(std::is_pod<vec3>::value, "should be POD");
  static_assert(std::is_pod<vec4>::value, "should be POD");
  static_assert(std::is_pod<ivec2>::value, "should be POD");
  static_assert(std::is_pod<ivec3>::value, "should be POD");
  static_assert(std::is_pod<ivec4>::value, "should be POD");  
} // !p

#endif // !P_UTILS_VECTOR_H
