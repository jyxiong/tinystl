#pragma once

// https://en.cppreference.com/w/cpp/container/array.html

#include <compare>
#include <cstddef>
#include <stdexcept>

#include "tinystl/iterator/reverse_iterator.h"

namespace tinystl {
template <class T, std::size_t N>
struct array {
  // Member types
  using value_type = T;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using reference = value_type &;
  using const_reference = const value_type &;
  using pointer = value_type *;
  using const_pointer = const value_type *;
  using iterator = pointer;
  using const_iterator = const_pointer;
  using reverse_iterator = tinystl::reverse_iterator<iterator>;
  using const_reverse_iterator = tinystl::reverse_iterator<const_iterator>;

  // Member functions

  // Implicitly-declared member functions:
  //   constructor
  //   destructor
  //   operator=

  // Element accesses
  reference at(size_type pos) {
    if (pos >= N) {
      throw std::out_of_range("tinystl::array::at - index out of range");
    }
    return m_data[pos];
  }

  const_reference at(size_type pos) const {
    if (pos >= N) {
      throw std::out_of_range("tinystl::array::at - index out of range");
    }
    return m_data[pos];
  }

  reference operator[](size_type pos) { return m_data[pos]; }
  const_reference operator[](size_type pos) const { return m_data[pos]; }

  reference front() { return m_data[0]; }
  const_reference front() const { return m_data[0]; }

  reference back() { return m_data[N - 1]; }
  const_reference back() const { return m_data[N - 1]; }

  T *data() noexcept { return m_data; }
  const T *data() const noexcept { return m_data; }

  // Iterators
  iterator begin() noexcept { return m_data; }
  const_iterator begin() const noexcept { return m_data; }
  const_iterator cbegin() const noexcept { return m_data; }

  iterator end() noexcept { return m_data + N; }
  const_iterator end() const noexcept { return m_data + N; }
  const_iterator cend() const noexcept { return m_data + N; }

  reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
  const_reverse_iterator rbegin() const noexcept {
    return const_reverse_iterator(end());
  }
  const_reverse_iterator crbegin() const noexcept {
    return const_reverse_iterator(end());
  }

  reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
  const_reverse_iterator rend() const noexcept {
    return const_reverse_iterator(begin());
  }
  const_reverse_iterator crend() const noexcept {
    return const_reverse_iterator(begin());
  }

  // Capacity
  constexpr bool empty() const noexcept { return N == 0; }
  constexpr size_type size() const noexcept { return N; }
  constexpr size_type max_size() const noexcept { return N; }

  // Operations
  void fill(const T &value) {
    for (size_type i = 0; i < N; ++i) {
      m_data[i] = value;
    }
  }

  void swap(array &other) noexcept(std::is_nothrow_swappable_v<T>) {
    for (size_type i = 0; i < N; ++i) {
      std::swap(m_data[i], other.m_data[i]);
    }
  }

  // IMPORTANT: The underlying array must be public to allow aggregate
  // initialization
  T m_data[N];
};

// Non-member functions
template <class T, std::size_t N>
constexpr bool operator==(const array<T, N> &lhs, const array<T, N> &rhs) {
  return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <class T, std::size_t N>
constexpr auto operator<=>(const array<T, N> &lhs, const array<T, N> &rhs) {
  return std::lexicographical_compare_three_way(
    lhs.data(), lhs.data() + N, rhs.data(), rhs.data() + N,
    std::compare_three_way{}
  );
}

} // namespace tinystl
