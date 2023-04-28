#pragma once

#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <iterator>
#include <limits>
#include <omnialloc/BaseComponentList.hpp>
#include <stdexcept>
#include <vector>

namespace omni {
template <typename T>
class StaticComponentList : public BaseComponentList {
 public:
  StaticComponentList(size_t maxSize = 64)
      : maxSize(maxSize),
        freeComponents(maxSize / 64, std::numeric_limits<uint64_t>::max()) {
    if (maxSize % 64 > 0) {
      throw std::runtime_error(
          "Component list sizes must be a multiple of 64.");
    }

    components = (T*)malloc(sizeof(T) * maxSize);
  }

  ~StaticComponentList() {
    for (size_t i = 0; i < maxSize / 64; i++) {
      if (~freeComponents[i] > 0) {
        for (size_t j = 0; j < 64; j++) {
          if (((freeComponents[i] >> j) & 1) == 0) {
            components[i * 64 + j].~T();
          }
        }
      }
    }
    free(components);
  }

  size_t FindFreeComponent() {
    for (size_t i = 0; i < maxSize / 64; i++) {
      if (freeComponents[i] > 0) {
        for (size_t j = 0; j < 64; j++) {
          if (((freeComponents[i] >> j) & 1) > 0) {
            return i * 64 + j;
          }
        }
      }
    }
    return maxSize;
  }

  void* Allocate() override {
    auto freeComponent = FindFreeComponent();
    if (freeComponent == maxSize) {
      return nullptr;
    }

    freeComponents[freeComponent / 64] &= ~(1 << (freeComponent % 64));

    return new (components + freeComponent) T();
  }

  void Deallocate(void* component) override {
    size_t componentIndex =
        ((size_t)component - (size_t)components) / sizeof(T);
    freeComponents[componentIndex / 64] |= (uint64_t)1 << (componentIndex % 64);

    ((T*)component)->~T();
  }

 private:
  T* components;
  size_t maxSize;
  std::vector<uint64_t> freeComponents;
};
}  // namespace omni
