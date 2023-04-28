#pragma once

#include <cstddef>
#include <cstdlib>
#include <iterator>
#include <omnialloc/BaseComponentList.hpp>

namespace omni {
template <typename T>
struct StaticComponentList : public BaseComponentList {
  StaticComponentList() {
    components = (T*)malloc(sizeof(T) * MaxListSize);
    memset(freeComponents, 0xFF, sizeof(freeComponents));
  }

  ~StaticComponentList() {
    for (size_t i = 0; i < MaxListSize; i++) {
      if (((freeComponents[i / 64] >> (i % 64)) & 1) == 0) {
        components[i].~T();
      }
    }
    free(components);
  }

  ssize_t FindFreeComponent() {
    for (size_t i = 0; i < MaxListSize; i++) {
      if (((freeComponents[i / 64] >> (i % 64)) & 1) > 0) {
        return i;
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
