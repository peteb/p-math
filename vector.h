/* -- vector.h -------------------------------------------------------*- c++ -*-
 *
 * Accessing components
 *   through []-operator
 *   copying vector memory to buffer
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

namespace p {
  // TODO: make sure clamp, lerp, etc. work with vectors
  /* ------------------------------------------------------------------------ */
#pragma mark Basic vector types
  
  /**
   * The general case.
   */
  template<typename T, std::size_t N>
  struct vec {
    typedef T value_type;
    enum {size = N};
  
    explicit vec() {}
    explicit vec(T val) {
      std::uninitialized_fill(components, components + size, val);
    }

    inline T &operator [](std::size_t pos) {return components[pos];}
    inline T operator [](std::size_t pos) const {return components[pos];}
  
    T components[size];
  };

  /**
   * Bottom type.
   */
  template<typename T>
  struct vec<T, 0> {private: vec();};
  
  /**
   * Specialization of vector for 2 components, which must be POD type.
   */
  template<typename T>
  struct vec<T, 2> {
    typedef T value_type;
    enum {size = 2};
    
    explicit vec() {}
    explicit vec(T x, T y) : x(x), y(y) {}
    explicit vec(T val) {
      std::uninitialized_fill(components, components + size, val);
    }
  
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
    enum {size = 3};

    explicit vec() {}
    explicit vec(T x, T y, T z) : x(x), y(y), z(z) {}
    explicit vec(T val) {
      std::uninitialized_fill(components, components + size, val);
    }

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
    enum {size = 4};
    
    explicit vec() {}
    explicit vec(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}
    explicit vec(T val) {
      std::uninitialized_fill(components, components + size, val);
    }
    
    inline T &operator [](std::size_t pos) {return components[pos];}
    inline T operator [](std::size_t pos) const {return components[pos];}
    
    union {
      struct {T x, y, z, w; };
      struct {T s, t, p, q; };
      struct {T r, g, b, a; };
      struct {T components[size]; };
    };    
  };
  
  
#pragma mark Helpers
    
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
  
  
  
  /* ------------------------------------------------------------------------ */
#pragma mark Operators
  
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

  template<typename T, std::size_t size, typename scalarT> 
  inline vec<T, size> operator *(const vec<T, size> &lhs, scalarT rhs) {
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
    lhs = lhs * vec<T, size>(rhs); return lhs;
  }

  template<typename T, std::size_t size, typename scalarT> 
  inline vec<T, size>& operator /=(vec<T, size>& lhs, scalarT rhs) {
    lhs = lhs / vec<T, size>(rhs); return lhs;
  }

  
  /* ------------------------------------------------------------------------ */
#pragma mark Algorithms

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
  
  // very generic normalize function
  template<typename T>
  inline T normalize(const T &v) {
    T ret = v / magnitude(v);
    return ret;
  }

  /* ------------------------------------------------------------------------ */
#pragma mark Types for usage
  typedef vec<float, 2> vec2;
  typedef vec<float, 3> vec3;
  typedef vec<float, 4> vec4;
  typedef vec<int, 2> ivec2;
  typedef vec<int, 3> ivec3;
  typedef vec<int, 4> ivec4;
  typedef vec<unsigned char, 3> ubvec3;
  typedef vec<unsigned char, 4> ubvec4;
} // !p

#endif // !P_UTILS_VECTOR_H
