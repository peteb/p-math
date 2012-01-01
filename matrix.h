/* -- matrix.h -------------------------------------------------------*- c++ -*-
 *
 * -------------------------------------------------------------------------- */

#ifndef P_UTILS_MATRIX_H
#define P_UTILS_MATRIX_H

#include <algorithm>

namespace p {

  template<typename T, std::size_t N>
  class vec;

  /**
   * The general case.
   */
  template<typename T, std::size_t M, std::size_t N>
  struct mat {
    typedef T value_type;
    enum {width = M};
    enum {height = N};

    explicit mat() {}
    explicit mat(T val) {
      std::uninitialized_fill(components, components + M*N, val);
    }

    vec<T, M> &row(std::size_t j) {
      return reinterpret_cast<vec<T, M> &>(components[M*j]);
    }

    const vec<T, M> &row(std::size_t j) const {
      return reinterpret_cast<vec<T, M> &>(components[M*j]);
    }
  
    T components[M*N];
  };


  template<typename T, std::size_t M> struct mat<T, M, 0> {private: mat(); };
  template<typename T, std::size_t N> struct mat<T, 0, N> {private: mat(); };

  typedef mat<float, 3, 3> mat3;
  typedef mat<float, 4, 4> mat4;
}

#endif // !P_UTILS_MATRIX_H

