#pragma once

#include <cstddef>
#include <cstdlib>
#include <iterator>
#include <omnialloc/BaseComponentList.hpp>

namespace omni {
template <typename T>
class StaticComponentList : public BaseComponentList {
 public:
  StaticComponentList() {
    components = (T*)malloc(sizeof(T) * MaxListSize);
    memset(freeComponents, 0xFF, sizeof(freeComponents));
  }

  ~StaticComponentList() {
    for (size_t i = 0; i < MaxListSize / 64; i++) {
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

  ssize_t FindFreeComponent() {
    for (size_t i = 0; i < MaxListSize / 64; i++) {
      if ((freeComponents[i] & 0xFFFFFFFFFFFF) > 0) {
        for (size_t j = 0; j < 64; j++) {
          if (((freeComponents[i] >> j) & 1) > 0) {
            return i * 64 + j;
          }
        }
      }
    }
    return MaxListSize;
  }

  void* Allocate() override {
    auto freeComponent = FindFreeComponent();
    if (freeComponent == MaxListSize) {
      return nullptr;
    }

    freeComponents[freeComponent / 64] &= ~(1 << (freeComponent % 64));

    return new (components + freeComponent) T();
  }

  void Deallocate(void* component) override {
    size_t componentIndex =
        ((size_t)component - (size_t)components) / sizeof(T);
    freeComponents[componentIndex / 64] |= 1 << (componentIndex % 64);

    ((T*)component)->~T();
  }

  static constexpr size_t MaxListSize = 256;

 private:
  T* components;
  uint64_t freeComponents[MaxListSize / (sizeof(uint64_t) * 8)];
};
}  // namespace omni
