#pragma once

#include <cstddef>

// https://en.cppreference.com/w/cpp/iterator/iterator.html
namespace tinystl {

template <
  typename Category, typename T, typename Distance = std::ptrdiff_t,
  typename Pointer = T *, typename Reference = T &>
struct [[deprecated("deprecated")]] iterator {
  using iterator_category = Category;
  using value_type = T;
  using difference_type = Distance;
  using pointer = Pointer;
  using reference = Reference;
};
} // namespace tinystl