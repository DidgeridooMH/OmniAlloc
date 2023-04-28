#pragma once

#include <cstdint>
#include <memory>
#include <omnialloc/BaseComponentList.hpp>
#include <string>
#include <unordered_map>

#define COMPONENT_ID(T) typeid(T).hash_code()

namespace omni {
class ComponentPool {
 public:
  void Register(size_t id, const std::string& name,
                std::shared_ptr<BaseComponentList> list);

  void Deregister(size_t id, const std::string& name);

  void* Allocate(const std::string& name);
  void* Allocate(size_t id);

  void Deallocate(const std::string& name, void* component);
  void Deallocate(size_t id, void* component);

 private:
  std::unordered_map<std::string, size_t> namesToIds;
  std::unordered_map<size_t, std::shared_ptr<BaseComponentList>> idsToLists;
};
}  // namespace omni
