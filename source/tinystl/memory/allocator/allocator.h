#pragma once

#include <cstddef>
#include <limits>
#include <new>

#include "tinystl/type_traits/helper_classes/integral_constant.h"

namespace tinystl {
// https://en.cppreference.com/w/cpp/memory/allocator.html
template <class T>
struct allocator {
  // Member types
  using value_type = T;
  using pointer [[deprecated("deprecated")]] = T *;               // util c++20
  using const_pointer [[deprecated("deprecated")]] = const T *;   // util c++20
  using reference [[deprecated("deprecated")]] = T &;             // util c++20
  using const_reference [[deprecated("deprecated")]] = const T &; // util c++20
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using propagate_on_container_move_assignment = true_type;
  template <class U>
  struct [[deprecated("deprecated")]] rebind {
    typedef allocator<U> other;
  }; // util c++20
  using is_always_equal [[deprecated("deprecated")]] = true_type; // util c++26

  // Member functions
  allocator() noexcept {}

  allocator(const allocator &other) noexcept {}

  template <class U>
  allocator(const allocator<U> &) noexcept {}

  ~allocator() noexcept {}

  [[deprecated("deprecated")]]
  pointer address(reference x) const noexcept {
    return &x;
  } // util c++20

  [[deprecated("deprecated")]]
  const_pointer address(const_reference x) const noexcept {
    return &x;
  } // util c++20

  [[deprecated("deprecated")]]
  T *allocate(size_type n, const void *) {
    allocate(n);
  } // util c++20

  T *allocate(size_type n) {
    if (std::numeric_limits<std::size_t>::max() / sizeof(T) < n)
      throw std::bad_array_new_length();
    return static_cast<T *>(::operator new(n * sizeof(T)));
  }

  void deallocate(T *p, std::size_t n) {
    ::operator delete(p);
  }

  [[deprecated("deprecated")]]
  size_type max_size() const noexcept {
    return std::numeric_limits<std::size_t>::max() / sizeof(T);
  } // util c++20

  template <class U, class... Args>
  [[deprecated("deprecated")]]
  void construct(U *p, Args &&...args) {
    ::new ((void *)p) U(std::forward<Args>(args)...);
  } // util c++20

  template <class U>
  [[deprecated("deprecated")]]
  void destroy(U *p) {
    p->~U();
  } // util c++20
};

// Non-member functions
template <class T, class U>
bool operator==(const allocator<T> &, const allocator<U> &) noexcept {
  return true;
} // constexpr since c++20

template <class T, class U>
[[deprecated("deprecated")]]
bool operator!=(const allocator<T> &, const allocator<U> &) noexcept {
  return false;
} // util c++20

} // namespace tinystl