/**
 * Requirements:
 * - [x] One allocations list per type.
 *   [x] Interface for allocating
 *   [x] Interface for deallocating
 *   [x] Interface for retrieving one
 *   [ ] Interface for retrieving all
 */

#include <omnialloc/ComponentPool.hpp>
#include <omnialloc/StaticComponentList.hpp>

struct SomeComponent {
  float position[3];
  float velocity[3];
  float acceleration[3];

  SomeComponent() {
    printf("Some component created %p\n", (void*)this);
    memset(position, 0, sizeof(position));
    memset(velocity, 0, sizeof(velocity));
    memset(acceleration, 0, sizeof(acceleration));
  }

  ~SomeComponent() { printf("Some compoent deleted %p\n", (void*)this); }

  void OnUpdate() { position[0] = 32.0f; }
};

int main() {
  omni::ComponentPool pool;
  pool.Register(COMPONENT_ID(SomeComponent), "SomeComponent",
                std::make_shared<omni::StaticComponentList<SomeComponent>>());

  auto component = (SomeComponent*)pool.Allocate(COMPONENT_ID(SomeComponent));
  component->OnUpdate();

  auto otherComponent = (SomeComponent*)pool.Allocate("SomeComponent");

  (void)(SomeComponent*)pool.Allocate(COMPONENT_ID(SomeComponent));

  pool.Deallocate(COMPONENT_ID(SomeComponent), otherComponent);

  (void)(SomeComponent*)pool.Allocate(COMPONENT_ID(SomeComponent));

  pool.Deregister(COMPONENT_ID(SomeComponent), "SomeComponent");

  return 0;
}

