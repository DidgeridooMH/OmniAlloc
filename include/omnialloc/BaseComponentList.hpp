#pragma once

#include <omnialloc/omnialloc.hpp>

namespace omni {
struct OMNIALLOC_EXPORT BaseComponentList {
  virtual ~BaseComponentList() {}

  virtual void* Allocate() = 0;
  virtual void Deallocate(void* component) = 0;
};
}  // namespace omni
