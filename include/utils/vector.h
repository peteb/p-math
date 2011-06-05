#ifndef UTILS_VECTOR_H
#define UTILS_VECTOR_H

#include <sstream>
#include <string>
#include <iostream>

namespace p {
  
template<typename T>
struct string_rep {typedef void type;};
template<> struct string_rep<float> {typedef float type;};
template<> struct string_rep<unsigned char> {typedef int type;};

/*
 * Base classes for vector types. They rely on the assumption that other fields
 * than what was written to in a union is defined behavior, which it isn't.
 */ 
template<typename T, int Size>
struct nvector {
  T& operator[](size_t idx) {return values[idx]; }
  const T& operator[](size_t idx) const {return values[idx]; }
  
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

typedef color4<float> fcolor4;
typedef color3<float> fcolor3;
typedef color4<unsigned char> ubcolor4;
typedef color3<unsigned char> ubcolor3;

} // !p

#endif // !UTILS_VECTOR_H

