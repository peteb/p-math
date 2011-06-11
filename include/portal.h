#ifndef SERVICES_PORTAL_H
#define SERVICES_PORTAL_H

#include <map>
#include <string>

namespace p {
  
// Definition of the main service broker service
class Portal {
public:
  template<typename InterfaceT>
  void registerInterface() {
    InterfaceT& instance = InterfaceT::instance();
    addInterface(InterfaceT::id(), &instance);
  }

  template<typename InterfaceT>
  InterfaceT* get() const {
    return reinterpret_cast<InterfaceT*>(getInterface(InterfaceT::id()));
  }

private:
  typedef std::map<std::string, class Interface*> InterfaceMap;
  InterfaceMap interfaces;

  class Interface* getInterface(const std::string& id) const;
  void addInterface(const std::string& id, class Interface* interface);
};


extern Portal& services; //< main access point
} // !p

#endif // !SERVICES_PORTAL_H
