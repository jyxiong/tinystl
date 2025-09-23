#pragma once

#include "tinystl/memory/allocator_traits.h"

namespace tinystl {

template <class Pointer>
struct allocation_result {
  Pointer ptr;
  size_t count;
};

template <class Alloc>
constexpr allocation_result<typename allocator_traits<Alloc>::pointer>
allocate_at_least(Alloc &alloc, size_t n) {
  return {alloc.allocate(n), n};
}

} // namespace tinystl