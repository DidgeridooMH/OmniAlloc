#pragma once

#include <cstdint>
#include <omnialloc/omnialloc.hpp>

namespace omni {
struct OMNIALLOC_EXPORT StaticComponentIterator {
  void* GetNext(void* current);

  void* components;
  uint64_t* freeComponents;
  size_t elementSize;
  size_t maxSize;
};
}  // namespace omni
