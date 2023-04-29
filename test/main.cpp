/**
 * Requirements:
 * - [x] One allocations list per type.
 *   [x] Interface for allocating
 *   [x] Interface for deallocating
 *   [x] Interface for retrieving one
 *   [x] Interface for retrieving all
 */

#include <chrono>
#include <omnialloc/ComponentPool.hpp>
#include <omnialloc/StaticComponentList.hpp>

struct SomeComponent {
  float position[3];
  float velocity[3];
  float acceleration[3];

  SomeComponent() {
    memset(position, 0, sizeof(position));
    memset(velocity, 0, sizeof(velocity));
    memset(acceleration, 0, sizeof(acceleration));
  }

  static void OnUpdate(SomeComponent* component) {
    for (int i = 0; i < 1024; i++) {
      int j = 0;
    }
  }
};

using UpdateCallback = void (*)(void*);

int main() {
  std::unordered_map<uint64_t, void (*)(void*)> onUpdateCallback;
  onUpdateCallback.insert(
      {COMPONENT_ID(SomeComponent), (UpdateCallback)&SomeComponent::OnUpdate});

  omni::ComponentPool pool;
  pool.Register(
      COMPONENT_ID(SomeComponent), "SomeComponent",
      std::make_shared<omni::StaticComponentList<SomeComponent>>(64 * 1000));

  for (int i = 0; i < 10000; i++) {
    pool.Allocate(COMPONENT_ID(SomeComponent));
  }

  auto start = std::chrono::steady_clock::now();
  for (int i = 0; i < 100; i++) {
    for (auto& [componentId, callback] : onUpdateCallback) {
      auto list = pool.GetAll(componentId);
      list->Apply(callback);
    }
  }
  auto end = std::chrono::steady_clock::now();
  auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
                  .count();
  printf("%lld ms\n", time / 100);

  return 0;
}
