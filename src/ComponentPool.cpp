#include "include/omnialloc/ComponentPool.hpp"

#include <omnialloc/ComponentPool.hpp>

namespace omni {
void ComponentPool::Register(size_t id, const std::string& name,
                             std::shared_ptr<BaseComponentList> list) {
  namesToIds.insert({name, id});
  idsToLists.insert({id, list});
}

void ComponentPool::Deregister(size_t id, const std::string& name) {
  namesToIds.erase(name);
  idsToLists.erase(id);
}

void* ComponentPool::Allocate(const std::string& name) {
  return Allocate(namesToIds[name]);
}

void* ComponentPool::Allocate(size_t id) { return idsToLists[id]->Allocate(); }

void ComponentPool::Deallocate(const std::string& name, void* component) {
  Deallocate(namesToIds[name], component);
}

void ComponentPool::Deallocate(size_t id, void* component) {
  idsToLists[id]->Deallocate(component);
}
}  // namespace omni
