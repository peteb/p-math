#include <portal.h>
#include <stdexcept>
#include <utility>
#include <string>

class Interface *p::portal::getInterface(const std::string& id) const {
  InterfaceMap::const_iterator iter = interfaces.find(id);
  if (iter != interfaces.end()) {
    return iter->second;
  }
  
  throw std::runtime_error("interface '" + id + "' not registered");
}

void p::Portal::addInterface(const std::string &id, 
                             class Interface *interface) 
{
  interfaces.insert(std::make_pair(id, interface));  
}
