#ifndef UTILS_VECTOR_H
#define UTILS_VECTOR_H

#include <sstream>
#include <string>
#include <iostream>
#include <algorithm>
#include <functional>
#include <cassert>

#include <utils/algorithm.h>

// TODO: add header here for some simple use cases
// just do this, add some tests, and be done with it

namespace p {
  // sorry about the dense code in here
  
  
  // this is needed because chars should be cast to ints when outputted
  template<typename T>
  struct string_rep {typedef T type;};
  template<> struct string_rep<unsigned char> {typedef int type;};

  

  #pragma mark - Basic vector types

  template<int Size, typename T>
  struct vec {};
  
  template<typename T>
  struct vec<2, T> {
    vec() {}
    vec(T x, T y) : x(x), y(y) {}
    vec(T val) : x(val), y(val) {}
    
    typedef typename string_rep<T>::type string_type;
    typedef T value_type;
    
    T x, y;
    
    vec<2, T> operator -() const {
      vec<2, T> ret(-x, -y);
      return ret;
    }
    
    operator std::string() const {
      std::stringstream ss;
      ss << string_type(x) << ", " 
         << string_type(y);
      return ss.str();
    }


  };


  template<typename T>
  struct vec<3, T> {
    vec() {}
    vec(T x, T y, T z) : x(x), y(y), z(z) {}
    vec(T val) : x(val), y(val), z(val) {}

    typedef typename string_rep<T>::type string_type;
    typedef T value_type;

    T x, y, z;

    vec<2, T>& truncated() {
      return *reinterpret_cast<vec<2, T>*>(this);
    }
    
    const vec<2, T>& truncated() const {
      return *reinterpret_cast<const vec<2, T>*>(this);
    }  

    vec<3, T> operator -() const {
      vec<3, T> ret(-x, -y, -z);
      return ret;
    }

    operator std::string() const {
      std::stringstream ss;
      ss << string_type(x) << ", " 
         << string_type(y) << ", " 
         << string_type(z);
      return ss.str();
    }

  };

  template<typename T>
  struct vec<4, T> {
    vec() {}
    vec(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}
    vec(T val) : x(val), y(val), z(val), w(val) {}

    typedef typename string_rep<T>::type string_type;
    typedef T value_type;

    T x, y, z, w;
    
    vec<3, T>& truncated() {
      return *reinterpret_cast<vec<3, T>*>(this);
    }

    const vec<3, T>& truncated() const {
      return *reinterpret_cast<const vec<3, T>*>(this);
    }

    vec<4, T> operator -() const {
      vec<4, T> ret(-x, -y, -z, -w);
      return ret;
    }

    operator std::string() const {
      std::stringstream ss;
      ss << string_type(x) << ", " 
         << string_type(y) << ", " 
         << string_type(z) << ", "
         << string_type(w) << ", ";
      return ss.str();
    }


  };
  
  // helpers
  #pragma mark - Helpers
  
  template<typename T, typename OpT>
  inline vec<2, T> doOp(const vec<2, T>& lhs, const vec<2, T>& rhs, const OpT& op) {
    vec<2, T> ret(op(lhs.x, rhs.x), op(lhs.y, rhs.y));
    return ret;
  }

  template<typename T, typename OpT>
  inline vec<3, T> doOp(const vec<3, T>& lhs, const vec<3, T>& rhs, const OpT& op) {
    vec<3, T> ret(op(lhs.x, rhs.x), op(lhs.y, rhs.y), op(lhs.z, rhs.z));
    return ret;
  }

  template<typename T, typename OpT>
  inline vec<4, T> doOp(const vec<4, T>& lhs, const vec<4, T>& rhs, const OpT& op) {
    vec<4, T> ret(op(lhs.x, rhs.x), op(lhs.y, rhs.y), op(lhs.z, rhs.z), op(lhs.w, rhs.w));
    return ret;
  }

  // operator overloads
  #pragma mark - Operators
  template<int Size, typename T, typename OtherT> 
  inline vec<Size, T> operator *(const vec<Size, T>& lhs, OtherT rhs) {
    vec<Size, T> ret = doOp(lhs, vec<Size, T>(rhs), std::multiplies<T>());
    return ret;
  }

  template<int Size, typename T, typename OtherT> 
  inline vec<Size, T> operator /(const vec<Size, T>& lhs, OtherT rhs) {
    vec<Size, T> ret = doOp(lhs, vec<Size, T>(rhs), std::divides<T>());
    return ret;
  }

  template<int Size, typename T> 
  inline vec<Size, T> operator +(const vec<Size, T>& lhs, const vec<Size, T>& rhs) {
    vec<Size, T> ret = doOp(lhs, rhs, std::plus<T>());
    return ret;
  }

  template<int Size, typename T> 
  inline vec<Size, T> operator -(const vec<Size, T>& lhs, const vec<Size, T>& rhs) {
    vec<Size, T> ret = doOp(lhs, rhs, std::minus<T>());
    return ret;
  }

  template<int Size, typename T, typename RhT> 
  inline vec<Size, T>& operator *=(vec<Size, T>& lhs, RhT rhs) {
    lhs = lhs * vec<Size, T>(rhs);
    return lhs;
  }

  template<int Size, typename T, typename RhT> 
  inline vec<Size, T>& operator /=(vec<Size, T>& lhs, RhT rhs) {
    lhs = lhs / vec<Size, T>(rhs);
    return lhs;
  }

  template<int Size, typename T> 
  inline vec<Size, T>& operator +=(vec<Size, T>& lhs, const vec<Size, T>& rhs) {
    lhs = lhs + rhs;
    return lhs;
  }
  
  template<int Size, typename T> 
  inline vec<Size, T>& operator -=(vec<Size, T>& lhs, const vec<Size, T>& rhs) {
    lhs = lhs - rhs;
    return lhs;
  }

  template<int Size, typename T>
  std::ostream& operator <<(std::ostream& out, const vec<Size, T>& v) {
    return (out << std::string(v));
  }
  
  template<int Size>
  std::ostream& operator <<(std::ostream& out, const vec<Size, unsigned char>& v) {
    return (out << std::string(v));
  }

  
  // some more helpers
  template<typename T>
  struct min_fun {
    const T& operator()(const T& lhs, const T& rhs) const {using std::min; return min(lhs, rhs); }
  };

  template<typename T>
  struct max_fun {
    const T& operator()(const T& lhs, const T& rhs) const {using std::max; return max(lhs, rhs); }
  };

  
  // algorithm overloads
  #pragma mark - Algorithms

  template<int Size, typename T>
  inline vec<Size, T> min(const vec<Size, T>& v1, const vec<Size, T>& v2) {
    vec<Size, T> ret = doOp(v1, v2, min_fun<T>());
    return ret;
  }

  template<int Size, typename T>
  inline vec<Size, T> max(const vec<Size, T>& v1, const vec<Size, T>& v2) {
    vec<Size, T> ret = doOp(v1, v2, max_fun<T>());
    return ret;
  }

  template<typename T>
  inline T dot_product(const vec<2, T>& v1, const vec<2, T>& v2) {
    return v1.x * v2.x + v1.y * v2.y;
  }

  template<typename T>
  inline T dot_product(const vec<3, T>& v1, const vec<3, T>& v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
  }

  template<typename T>
  inline T magnitude(const vec<2, T>& v) {
    return sqrt(v.x * v.x + v.y * v.y);
  }

  template<typename T>
  inline T magnitude(const vec<3, T>& v) {
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
  }
  
  template<typename T>
  inline void normalize(T& v) {
    v /= magnitude(v);
  }

  template<int Size, typename T>
  inline vec<Size, T> normalized(const vec<Size, T>& v) {
    vec<Size, T> ret = v;
    v /= magnitude(v);
    return ret;
  }

  typedef vec<2, float> vec2;
  typedef vec<3, float> vec3;
  typedef vec<4, float> vec4;
  typedef vec<2, int> ivec2;
  typedef vec<3, int> ivec3;
  typedef vec<4, int> ivec4;
  typedef vec<3, unsigned char> ubvec3;
  typedef vec<4, unsigned char> ubvec4;

  
  

// this file should define these types:
// vec2, vec3, vec4, ivec2, ivec3, ivec4
// color3, color4, ubcolor3, ubcolor4
// all types have common operators (*,-,+,/,prefix-), and their *= counterpart.
// equality & neq operators are only defined for integer types
// lerp, minmax, normalize, magnitude should work on all.
// saturate should work on colors (0..1) for floats, (0..255) for ub, etc.

// different color types can be converted between eachother



} // !p

#endif // !UTILS_VECTOR_H

