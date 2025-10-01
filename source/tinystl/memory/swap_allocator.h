#pragma once

#include <memory>
#include <type_traits>

namespace tinystl {

template <typename Alloc>
void swap_allocator(Alloc &a1, Alloc &a2, std::true_type) noexcept {
  std::swap(a1, a2);
}

template <typename Alloc>
void swap_allocator(Alloc &, Alloc &, std::false_type) noexcept {}

template <typename Alloc>
void swap_allocator(Alloc &a1, Alloc &a2) noexcept {
  swap_allocator(
    a1, a2,
    std::bool_constant<
      std::allocator_traits<Alloc>::propagate_on_container_swap::value>()
  );
}
} // namespace tinystl