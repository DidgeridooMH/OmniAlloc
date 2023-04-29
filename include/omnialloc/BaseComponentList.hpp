#pragma once

#include <omnialloc/StaticComponentIterator.hpp>
#include <omnialloc/omnialloc.hpp>

namespace omni {
struct OMNIALLOC_EXPORT BaseComponentList {
  virtual ~BaseComponentList() {}

  virtual void* Allocate() = 0;
  virtual void Deallocate(void* component) = 0;

  virtual void Apply(void (*)(void*)) = 0;

  virtual StaticComponentIterator GetIterator() = 0;
};
}  // namespace omni
