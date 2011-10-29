#ifndef UTILS_LOG_H
#define UTILS_LOG_H

#include <sstream>

#define INFO false, __FILE__, __FUNCTION__, __LINE__
#define ERROR true, __FILE__, __FUNCTION__, __LINE__

namespace p {
  class Log {
  public:
    Log(bool error, const char* file, const char* function, int line);
    ~Log();
    
    template<typename T>
    Log& operator <<(T value) {
      _stream << value;
      return *this;
    }
    
  private:
    std::stringstream _stream;
    const bool _error;
  };
  
}

#endif // !UTILS_LOG_H
