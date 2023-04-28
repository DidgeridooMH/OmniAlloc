#pragma once

namespace omni {
struct BaseComponentList {
  virtual ~BaseComponentList() {}

  virtual void* Allocate() = 0;
  virtual void Deallocate(void* component) = 0;
};
}  // namespace omni
