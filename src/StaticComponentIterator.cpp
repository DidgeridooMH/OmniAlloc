#include <omnialloc/StaticComponentIterator.hpp>

namespace omni {
void* StaticComponentIterator::GetNext(void* current) {
  size_t currentIndex = 0;
  if (current != nullptr) {
    currentIndex = (((size_t)current - (size_t)components) / elementSize) + 1;
  }

  size_t bitCompensation = currentIndex % 64;
  for (size_t i = currentIndex / 64; i < maxSize / 64; i++) {
    if (~freeComponents[i] > 0) {
      for (size_t j = bitCompensation; j < 64; j++) {
        if (((freeComponents[i] >> j) & 1) == 0) {
          return (char*)components + ((i * 64 + j) * elementSize);
        }
      }
    }
    bitCompensation = 0;
  }

  return nullptr;
}
}  // namespace omni
