#include "common/DynamicLoad.hpp"

#include <dlfcn.h>
#include <iostream>

namespace ale {

bool DynamicLinkFunction(void** fn, const char* source, const char* library) {
  // Function already linked
  if (*fn != nullptr) {
    return true;
  }

  // Try linking to local symbol first
  *fn = dlsym(RTLD_NEXT, source);
  if (*fn != nullptr) {
    return true;
  } else {
    std::cout << "Failed to dlsym RTLD_NEXT " << source << std::endl;
  }

  // Try loading library
  if (library != nullptr) {
    // We can repetitvely call dlopen, it gets refcounted
    // and will return the sample handle once the library is mmaped
    void* handle = dlopen(library, RTLD_LAZY);
    if (handle != nullptr) {
      // Dynamically link function
      *fn = dlsym(handle, source);
      if (*fn != nullptr) {
        return true;
      } else {
        std::cout << "Failed to dlsym " << source << std::endl;
      }
    } else {
      std::cout << "Failed to dlopen " << library << std::endl;
    }
  }

  *fn = nullptr;
  return false;
}

} // namespace ale
