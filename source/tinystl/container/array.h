#pragma once

// https://en.cppreference.com/w/cpp/container/array.html

#include <algorithm>
#include <cstddef>
#include <stdexcept>

#include "tinystl/iterator/reverse_iterator.h"
#include "tinystl/type_traits/const_volatility_specifiers/remove_cv.h"

namespace tinystl {

template <class T, size_t N> struct array_const_iterator {
  using iterator_category = random_access_iterator_tag;
  using value_type = T;
  using difference_type = std::ptrdiff_t;
  using pointer = const T *;
  using reference = const T &;

  constexpr array_const_iterator() : m_ptr(nullptr) {}

  constexpr explicit array_const_iterator(const T *ptr, std::size_t offset = 0)
      : m_ptr(ptr + offset) {}

  constexpr reference operator*() const { return *m_ptr; }

  constexpr pointer operator->() const { return m_ptr; }

  constexpr array_const_iterator &operator++() {
    ++m_ptr;
    return *this;
  }

  constexpr array_const_iterator operator++(int) {
    array_const_iterator tmp = *this;
    ++m_ptr;
    return tmp;
  }

  constexpr array_const_iterator &operator--() {
    --m_ptr;
    return *this;
  }

  constexpr array_const_iterator operator--(int) {
    array_const_iterator tmp = *this;
    --m_ptr;
    return tmp;
  }

  constexpr array_const_iterator &operator+=(difference_type n) {
    m_ptr += n;
    return *this;
  }

  constexpr array_const_iterator &operator-=(difference_type n) {
    m_ptr -= n;
    return *this;
  }

  constexpr std::ptrdiff_t operator-(const array_const_iterator &other) const {
    return m_ptr - other.m_ptr;
  }

  constexpr reference operator[](difference_type n) const {
    return *(m_ptr + n);
  }

  constexpr bool operator==(const array_const_iterator &other) const {
    return m_ptr == other.m_ptr;
  }

  constexpr auto operator<=>(const array_const_iterator &other) const {
    return m_ptr <=> other.m_ptr;
  }

private:
  pointer m_ptr;
};

template <class T, size_t N>
struct array_iteraotr : array_const_iterator<T, N> {
  using iterator_category = random_access_iterator_tag;
  using value_type = T;
  using difference_type = std::ptrdiff_t;
  using pointer = T *;
  using reference = T &;

  constexpr array_iteraotr() : array_const_iterator<T, N>() {}

  constexpr explicit array_iteraotr(T *ptr, std::size_t offset = 0)
      : array_const_iterator<T, N>(ptr, offset) {}

  constexpr reference operator*() const {
    return const_cast<reference>(array_const_iterator<T, N>::operator*());
  }

  constexpr pointer operator->() const {
    return const_cast<pointer>(array_const_iterator<T, N>::operator->());
  }

  constexpr array_iteraotr &operator++() {
    array_const_iterator<T, N>::operator++();
    return *this;
  }

  constexpr array_iteraotr operator++(int) {
    array_iteraotr tmp = *this;
    array_const_iterator<T, N>::operator++();
    return tmp;
  }

  constexpr array_iteraotr &operator--() {
    array_const_iterator<T, N>::operator--();
    return *this;
  }

  constexpr array_iteraotr operator--(int) {
    array_iteraotr tmp = *this;
    array_const_iterator<T, N>::operator--();
    return tmp;
  }

  constexpr array_iteraotr &operator+=(difference_type n) {
    array_const_iterator<T, N>::operator+=(n);
    return *this;
  }

  constexpr array_iteraotr &operator-=(difference_type n) {
    array_const_iterator<T, N>::operator-=(n);
    return *this;
  }

  constexpr std::ptrdiff_t operator-(const array_iteraotr &other) const {
    return array_const_iterator<T, N>::operator-(other);
  }

  constexpr reference operator[](difference_type n) const {
    return const_cast<reference>(array_const_iterator<T, N>::operator[](n));
  }

  constexpr bool operator==(const array_iteraotr &other) const {
    return array_const_iterator<T, N>::operator==(other);
  }

  constexpr auto operator<=>(const array_iteraotr &other) const {
    return array_const_iterator<T, N>::operator<=>(other);
  }
};

// class template array
template <class T, size_t N>
struct array // partially freestanding
{
  // types
  using value_type = T;
  using pointer = T *;
  using const_pointer = const T *;
  using reference = T &;
  using const_reference = const T &;
  using size_type = size_t;
  using difference_type = ptrdiff_t;
  using iterator = array_iteraotr<T, N>;
  using const_iterator = array_const_iterator<T, N>;
  using reverse_iterator = tinystl::reverse_iterator<iterator>;
  using const_reverse_iterator = tinystl::reverse_iterator<const_iterator>;

  // no explicit construct/copy/destroy for aggregate type

  constexpr void fill(const T &u) { std::fill_n(m_data, N, u); }

  constexpr void swap(array &other) noexcept(std::is_nothrow_swappable_v<T>) {
    std::swap_ranges(m_data, m_data + N, other.m_data);
  }

  // iterators
  constexpr iterator begin() noexcept { return iterator(m_data, 0); }
  constexpr const_iterator begin() const noexcept {
    return const_iterator(m_data, 0);
  }
  constexpr iterator end() noexcept { return iterator(m_data, N); }
  constexpr const_iterator end() const noexcept {
    return const_iterator(m_data, N);
  }

  constexpr reverse_iterator rbegin() noexcept {
    return reverse_iterator(end());
  }
  constexpr const_reverse_iterator rbegin() const noexcept {
    return const_reverse_iterator(end());
  }
  constexpr reverse_iterator rend() noexcept {
    return reverse_iterator(begin());
  }
  constexpr const_reverse_iterator rend() const noexcept {
    return const_reverse_iterator(begin());
  }

  constexpr const_iterator cbegin() const noexcept {
    return const_iterator(m_data, 0);
  }
  constexpr const_iterator cend() const noexcept {
    return const_iterator(m_data, N);
  }
  constexpr const_reverse_iterator crbegin() const noexcept {
    return const_reverse_iterator(cend());
  }
  constexpr const_reverse_iterator crend() const noexcept {
    return const_reverse_iterator(cbegin());
  }

  // capacity
  constexpr bool empty() const noexcept { return N == 0; }
  constexpr size_type size() const noexcept { return N; }
  constexpr size_type max_size() const noexcept { return N; }

  // element access
  constexpr reference operator[](size_type n) { return m_data[n]; }
  constexpr const_reference operator[](size_type n) const { return m_data[n]; }
  constexpr reference at(size_type n) // freestanding-deleted
  {
    if (n >= N) {
      throw std::out_of_range("array::at: index out of range");
    }
    return m_data[n];
  }
  constexpr const_reference at(size_type n) const // freestanding-deleted
  {
    if (n >= N) {
      throw std::out_of_range("array::at: index out of range");
    }
    return m_data[n];
  }
  constexpr reference front() { return m_data[0]; }
  constexpr const_reference front() const { return m_data[0]; }
  constexpr reference back() { return m_data[N - 1]; }
  constexpr const_reference back() const { return m_data[N - 1]; }

  constexpr T *data() noexcept { return m_data; }
  constexpr const T *data() const noexcept { return m_data; }

  T m_data[N];
};

template <class T> struct array<T, 0> {
  using value_type = T;
  using pointer = T *;
  using const_pointer = const T *;
  using reference = T &;
  using const_reference = const T &;
  using size_type = size_t;
  using difference_type = ptrdiff_t;
  using iterator = array_iteraotr<T, 0>;
  using const_iterator = array_const_iterator<T, 0>;
  using reverse_iterator = tinystl::reverse_iterator<iterator>;
  using const_reverse_iterator = tinystl::reverse_iterator<const_iterator>;

  // no explicit construct/copy/destroy for aggregate type

  constexpr void fill(const T &) {}

  constexpr void swap(array &) noexcept {}

  // iterators
  constexpr iterator begin() noexcept { return iterator(); }
  constexpr const_iterator begin() const noexcept { return const_iterator(); }
  constexpr iterator end() noexcept { return iterator(); }
  constexpr const_iterator end() const noexcept { return const_iterator(); }
  constexpr reverse_iterator rbegin() noexcept {
    return reverse_iterator(end());
  }
  constexpr const_reverse_iterator rbegin() const noexcept {
    return const_reverse_iterator(end());
  }
  constexpr reverse_iterator rend() noexcept {
    return reverse_iterator(begin());
  }
  constexpr const_reverse_iterator rend() const noexcept {
    return const_reverse_iterator(begin());
  }
  constexpr const_iterator cbegin() const noexcept { return const_iterator(); }
  constexpr const_iterator cend() const noexcept { return const_iterator(); }
  constexpr const_reverse_iterator crbegin() const noexcept {
    return const_reverse_iterator(cend());
  }
  constexpr const_reverse_iterator crend() const noexcept {
    return const_reverse_iterator(cbegin());
  }

  // capacity
  constexpr bool empty() const noexcept { return true; }
  constexpr size_type size() const noexcept { return 0; }
  constexpr size_type max_size() const noexcept { return 0; }

  // element access
  constexpr reference operator[](size_type) {
    throw std::out_of_range("array::operator[]: index out of range");
  }
  constexpr const_reference operator[](size_type) const {
    throw std::out_of_range("array::operator[]: index out of range");
  }
  constexpr reference at(size_type) // freestanding-deleted
  {
    throw std::out_of_range("array::at: index out of range");
  }
  constexpr const_reference at(size_type) const // freestanding-deleted
  {
    throw std::out_of_range("array::at: index out of range");
  }
  constexpr reference front() {
    throw std::out_of_range("array::front: array is empty");
  }
  constexpr const_reference front() const {
    throw std::out_of_range("array::front: array is empty");
  }
  constexpr reference back() {
    throw std::out_of_range("array::back: array is empty");
  }
  constexpr const_reference back() const {
    throw std::out_of_range("array::back: array is empty");
  }

  constexpr T *data() noexcept { return nullptr; }
  constexpr const T *data() const noexcept { return nullptr; }

  T m_data[1]; // to ensure sizeof(array<T,0>) != 0
};

template <class T, size_t N>
constexpr bool operator==(const array<T, N> &x, const array<T, N> &y)
{
  return std::equal(x.begin(), x.end(), y.begin());
}

template <class T, size_t N>
constexpr auto operator<=>(const array<T, N>& x, const array<T, N>& y) {
  return std::lexicographical_compare_three_way(x.begin(), x.end(), y.begin(), y.end(), std::compare_three_way());
}

// // specialized algorithms
// template <class T, size_t N>
// constexpr void swap(array<T, N> &x,
//                     array<T, N> &y) noexcept(noexcept(x.swap(y)));

// // array creation functions
// template <class T, size_t N>
// constexpr array<remove_cv_t<T>, N> to_array(T (&a)[N]);
// template <class T, size_t N>
// constexpr array<remove_cv_t<T>, N> to_array(T (&&a)[N]);

// // tuple interface
// template <class T> struct tuple_size;
// template <size_t I, class T> struct tuple_element;
// template <class T, size_t N> struct tuple_size<array<T, N>>;
// template <size_t I, class T, size_t N> struct tuple_element<I, array<T, N>>;
// template <size_t I, class T, size_t N> constexpr T &get(array<T, N> &)
// noexcept; template <size_t I, class T, size_t N> constexpr T &&get(array<T,
// N> &&) noexcept; template <size_t I, class T, size_t N> constexpr const T
// &get(const array<T, N> &) noexcept; template <size_t I, class T, size_t N>
// constexpr const T &&get(const array<T, N> &&) noexcept;

} // namespace tinystl