/* -- vector_stream.h ------------------------------------------------*- c++ -*-
 * Functionality for writing and reading vector classes to/from streams.
 * The text format for vectors are the values of the components adjoined by
 * whitespace (' ') characters.
 * 
 * When reading from a stream to a vector, some special keyword values are
 * reserved:
 *   null, zero - all components are zeroed
 * When reading into a vector through a p::color_reader:
 *   red - first component is maximum, others are 0
 * p::color_reader can also parse hexadecimal colors in the form of
 * "0xRRGGBBAA".
 * 
 * If parsing fails, the fail bit will be set.
 * 
 * Vectors:
 * out() << vec3(13.2f, 32.0f, -1.0f) // => "13.2 32.0 -1.0"
 *
 * Colors:
 * vec3 color;
 * in() >> p::color_reader(color);
 *
 * -------------------------------------------------------------------------- */

#ifndef P_VECTOR_STREAM_H 
#define P_VECTOR_STREAM_H

#include <ostream>
#include <istream>
#include <cstddef>
#include <cstdlib>
#include <limits>
#include <sstream>

#include "vector.h"

namespace p {
  namespace detail {
    // this is needed because chars should be cast to ints when outputted
    template<typename T>
    struct int_type {typedef T type;};
    template<> struct int_type<unsigned char> {typedef int type;};
    template<> struct int_type<signed char> {typedef int type;};

    bool stricmp(const char *s1, const char *s2) {
      std::size_t i = 0;
      do {
        if (std::tolower(s1[i]) != std::tolower(s2[i]))
          return false;
        ++i;
      } while (s1[i] && s2[i]);
      
      return true;
    }
    
    template<typename T, std::size_t size>
    void set_all_but(vec<T, size> &vector, std::size_t ignore, T value = T()) {
      for (std::size_t i = 0; i < size; ++i) {
        if (i != ignore) {
          vector[i] = value;
        }
      }
    }
    
    // color limits; 1.0 = max for floats/doubles, 255 = max for uchar, etc.
    template<typename T>
    struct color_limits {
      static inline T max() {return std::numeric_limits<T>::max();}
    };
    template<> struct color_limits<float> {
      static inline float max() {return 1.0f;}
    };
    template<> struct color_limits<double> {
      static inline double max() {return 1.0;}
    };

    /**
     * A class that can store the state of an istream and revert to that state.
     * It also wraps istream::sentry so you don't have to instantiate two helper
     * classes.
     */
    template<typename InStream>
    class streamstate {
      const std::streampos startPos;
      const std::ios::iostate startState;
      const std::istream::sentry sentry;
      
    public:
      streamstate(InStream &s)
        : startPos(s.tellg()), startState(s.rdstate()), sentry(s) {}
      void reset(InStream &s) const {s.clear(startState); s.seekg(startPos); }
      operator const void *() const {return (sentry ? this : 0); }
    };

    template<class charT, class traits>
    std::basic_ostream<charT, traits> &fail(std::basic_ostream<charT, traits> &os) {
      os.clear(std::ios::failbit);
      return os;
    }
    
    /**
     * Parsing helping class for reading colors from stream.
     * Should be created with p::color_reader.
     */
    template<typename T, std::size_t size>
    class color_reader_impl {
    public:
      color_reader_impl(vec<T, size> &target)
        : target(target)
      {
      }
      
      template<typename Stream>
      Stream &read(Stream &is) const {
        const streamstate<Stream> start(is);
        if (!start)
          return is;
        
        if (!(is >> target)) {
          start.reset(is);
          
          std::string textual;
          if (is >> textual) {
            // TODO: test with whitespace
            // TODO: first try parsing as components
            
            if (stricmp(textual.c_str(), "red")) {
              set_all_but(target, 0);
              target[0] = color_limits<T>::max();
            }
            // ...
            else if (textual.size() > 2 &&
                     textual[0] == '0' && textual[1] == 'x') {
              // parse it as hex
	      // convert_from_bin_component to type_component
	      // convert(0x000000FF, 8, float)
	      // TODO: this involves some muls/divs; integer conversions
	      //       might turn out incorrect. Maybe there should be a
	      //       fast-path for those.
              unsigned long val = std::strtoul(textual.c_str(), NULL, 16);
              for (std::size_t i = size; i--; ) {
                target[i] = (val & 0xFF) * (color_limits<T>::max() / 255.0);
                val >>= 8;
              }
            }
            else {
              fail(is);
            }
          }
        }
        
        return is;
      }
      
    private:
      vec<T, size> &target;
    };

  } // !detail


  /**
   * Creates a reader object that can do some 'smart' parsing
   * of strings into colors. For example, reading hexadecimal
   * values (0xC00FFEEE).
   *
   * @code
   * vec3 color;
   * std::cin >> p::color_reader(color);
   */
  template<typename T, std::size_t size>
  detail::color_reader_impl<T, size> color_reader(vec<T, size> &target) {
    return detail::color_reader_impl<T, size>(target);
  }

  // TODO: color_writer
  
  /**
   * Output a vector in the format x[ y[ z[...]]] unless std::hex has
   * been set on the stream.
   * For example, "123, 43".
   */
  template<typename T, std::size_t size>
  std::ostream &operator <<(std::ostream &s, const vec<T, size> &v) {
    const std::ostream::sentry sentry(s);
    if (!sentry)
      return s;
    
    if (s.flags() & std::ios::hex) {
      // TODO: this shouldn't be here... make a color_writer
      std::stringstream ss;
      ss << "0x" << std::hex;
      for (std::size_t i = size; i--; ) {
        double fraction = v[i] / double(detail::color_limits<T>::max());
        ss << int(fraction * 255.0);
      }
      
      // this safety is probably unnecessary.
      s << (ss ? ss : detail::fail(s));
    }
    else {
      typedef typename detail::int_type<T>::type text_rep;
      
      s << text_rep(v[0]);
      for (std::size_t i = 1; i < size; ++i)
        s << " " << text_rep(v[i]);
    }
  
    return s;  
  }  
  
  /**
   * Read a generic vector from a stream. Also supports
   * special values 'null'/'zero' for a vector with 0 length.
   */
  template<typename T, std::size_t size, typename InStream>
  InStream &operator >>(InStream &s, vec<T, size> &v) {
    const detail::streamstate<InStream> start(s);
    if (!start)
      return s;
    
    if (s >> v.components[0]) {
      for (std::size_t i = 1; i < size && s.good(); ++i) {
        typename detail::int_type<T>::type tmp;
        s >> tmp;
        v[i] = tmp;
      }
    }
    else {
      // it seems we can't parse it as a native type of the vector,
      // so we parse it as a string.
      start.reset(s);
      std::string textual;
      
      if (s >> textual) {
        using detail::stricmp;
        if (stricmp(textual.c_str(), "null") ||
            stricmp(textual.c_str(), "zero")) {
          for (std::size_t i = 0; i < size; ++i)
            v[i] = T();
        }
        else {
          s.clear(std::ios::failbit);
        }
      }
    }
  
    return s;
  }
  
  template<typename T, std::size_t size, typename InStream>
  InStream &operator >>(InStream &s,
                        const detail::color_reader_impl<T, size> &reader) {
    return reader.read(s);
  }

} // !p

#endif // !P_VECTOR_STREAM_H
