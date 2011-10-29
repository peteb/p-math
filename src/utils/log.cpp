#include "log.h"
#include <iostream>
#include <string>

p::Log::Log(bool error, const char* file, const char* function, int line) 
  : _error(error)
{
  std::string file_name = file ? file : "";
  size_t pos = file_name.find_last_of("/"); 
  file_name = file_name.substr(pos + 1);
  
  _stream << "[" << (_error ? "ERROR: " : "") << file_name << ":" << line << " " << function << "] "; 
}

p::Log::~Log() {
  std::ostream& out = (_error ? std::cerr : std::cout);
  out << _stream.str() << std::endl;
}

