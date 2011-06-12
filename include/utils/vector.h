#ifndef UTILS_VECTOR_H
#define UTILS_VECTOR_H

#include <sstream>
#include <string>
#include <iostream>
#include <algorithm>

#include <utils/algorithm.h>

namespace p {
  
  
// this file should define these types:
// vec2, vec3, vec4, ivec2, ivec3, ivec4
// color3, color4, ubcolor3, ubcolor4
// all types have common operators (*,-,+,/,prefix-), and their *= counterpart.
// equality & neq operators are only defined for integer types
// lerp, minmax, normalize, magnitude should work on all.
// saturate should work on colors (0..1) for floats, (0..255) for ub, etc.

// different color types can be converted between eachother

#pragma mark - Base types
template<typename T>
// this is needed because chars should be cast to ints when outputted
struct string_rep {typedef void type;};
template<> struct string_rep<float> {typedef float type;};
template<> struct string_rep<unsigned char> {typedef int type;};

// TODO: check if xcode and llvm can autocomplete this.

/*
 * Base classes for vector types. They rely on the assumption that other fields
 * than what was written to in a union is defined behavior, which it isn't.
 */ 
template<typename T, int Size>
struct nvector {
  T& operator[](size_t idx) {return values[idx]; }
  const T& operator[](size_t idx) const {return values[idx]; }

  // TODO: pull this into src
  operator std::string() const {
    typedef typename string_rep<T>::type string_type;
    
    std::stringstream ss;
    ss << "(";
    for (size_t i = 0; i < Size; ++i)
      ss << string_type(values[i]) << (i < Size-1 ? ", " : "");
      
    ss << ")";
    return ss.str();
  }

  nvector& operator *=(T scalar) {
    for (size_t i = 0; i < Size; ++i)
      values[i] *= scalar;
      
    return *this;
  }
  
  T values[Size];
};

#pragma mark - Coordinates/directional types
// TODO: friend operators maybe. also good if we could move operator *=, *, etc.
//       into parent, using CRTP.

// TODO: ärv från vector_base<3, float, vector3> kanske. och ha inte
//       nvector som en del av unionen, testa om 
//       T& operator[] {if idx==0 ret x; } skulle funka

template<typename T>
struct vector3 {
  vector3() {}
  vector3(T e1, T e2, T e3)
    : x(e1), y(e2), z(e3) {}
  
  T& operator[](size_t idx) {return values[idx]; }
  const T& operator[](size_t idx) const {return values[idx]; }
  
  operator std::string() const {return values.operator std::string();}
  vector3& operator *=(T scalar) {values *= scalar; return *this; }  
  vector3 operator *(T scalar) const {
    vector3 ret(*this); ret *= scalar; 
    return ret;
  }
  
  union {
    struct {T x, y, z; };
    struct {T r, g, b; };
    struct {T s, t, p; };
    nvector<T, 3> values;
  };
};

template<typename T>
struct vector4 {
  vector4() {}
  vector4(T e1, T e2, T e3, T e4)
    : x(e1), y(e2), z(e3), w(e4) {}
  
  T& operator[](size_t idx) {return values[idx]; }
  const T& operator[](size_t idx) const {return values[idx]; }
  
  operator std::string() const {return values.operator std::string();}
  vector4& operator *=(T scalar) {values *= scalar; return *this; }  
  vector4 operator *(T scalar) const {
    vector4 ret(*this); ret *= scalar; 
    return ret;
  }
  
  union {
    struct {T x, y, z, w; };
    struct {T r, g, b, a; };
    struct {T s, t, p, q; };
    nvector<T, 4> values;
  };
};

#pragma mark - Color types
template<typename T>
struct color_range {
/*  static T min = std::numeric_limits<T>::min();
  static T max = std::numeric_limits<T>::max();*/
};


template<>
struct color_range<float> {
  static float min;
  static float max;
};

template<>
struct color_range<unsigned char> {
  static unsigned char min;
  static unsigned char max;
};

float color_range<float>::min(0.0f);
float color_range<float>::max(1.0f);
unsigned char color_range<unsigned char>::min(0);
unsigned char color_range<unsigned char>::max(255);


template<typename T>
struct color3 : public vector3<T> {
  color3(T e1, T e2, T e3) : vector3<T>(e1, e2, e3) {}
};


template<typename T>
struct color4 : public vector4<T> {
  color4(T e1, T e2, T e3, T e4) : vector4<T>(e1, e2, e3, e4) {}
  
  // Constructor for casting between different color types. ubcolor (255, 128, 0) will convert to
  // color4<float>/fcolor4 (1.0, 0.5, 0).
  template<typename OtherT>
  color4(const vector4<OtherT>& other) 
    : vector4<T>(
      (other[0] / color_range<OtherT>::max) * color_range<T>::max,
      (other[1] / color_range<OtherT>::max) * color_range<T>::max,
      (other[2] / color_range<OtherT>::max) * color_range<T>::max,
      (other[3] / color_range<OtherT>::max) * color_range<T>::max)    
  {}
  
  // Ugly hacks, but it enables some pretty nice usage. The reinterpret_cast
  // can be avoided if we employ inheritance for components.
  color3<T>& truncated() {
    return reinterpret_cast<color3<T>&>(*this);
  }

  const color3<T>& truncated() const {
    return reinterpret_cast<const color3<T>&>(*this);
  }
};

#pragma mark - Default short-types
typedef color4<float> fcolor4;
typedef color3<float> fcolor3;
typedef color4<unsigned char> ubcolor4;
typedef color3<unsigned char> ubcolor3;

#pragma mark - Overloaded algorithms
template<typename T>
inline vector3<T> min(const vector3<T>& v1, const vector3<T>& v2) {
  using std::min;
  
  return vector3<T>(min(v1.x, v2.x), 
                    min(v1.y, v2.y), 
                    min(v1.z, v2.z));
}

template<typename T>
inline vector3<T> max(const vector3<T>& v1, const vector3<T>& v2) {
  using std::max;
    
  return vector3<T>(max(v1.x, v2.x), 
                    max(v1.y, v2.y), 
                    max(v1.z, v2.z));
}

// TODO: make lerp more generic, for all vector types.
template<typename T>
vector3<T> lerp(const vector3<T>& begin, const vector3<T>& end, T amount) {
  vector3<T> ret;
  ret.x = lerp(begin.x, end.x, amount);
  ret.y = lerp(begin.y, end.y, amount);
  ret.z = lerp(begin.z, end.z, amount);
  
  return ret;
}
} // !p

#endif // !UTILS_VECTOR_H

