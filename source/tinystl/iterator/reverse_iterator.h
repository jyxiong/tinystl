#pragma once

#include "tinystl/iterator/iterator_traits.h"

#include <compare>

namespace tinystl {

// https://en.cppreference.com/w/cpp/iterator/reverse_iterator.html
template <class Iter>
class reverse_iterator {
public:
  // Member types
  using iterator_type = Iter;
  using iterator_category = typename iterator_traits<Iter>::iterator_category;
  using value_type = typename iterator_traits<Iter>::value_type;
  using difference_type = typename iterator_traits<Iter>::difference_type;
  using pointer = typename iterator_traits<Iter>::pointer;
  using reference = typename iterator_traits<Iter>::reference;

  // Member functions
  reverse_iterator() = default;

  explicit reverse_iterator(Iter itr) : current(itr) {}

  template <class U>
  reverse_iterator(const reverse_iterator<U> &other) : current(other.current) {}

  template <class U>
  reverse_iterator &operator=(const reverse_iterator<U> &other) {
    current = other.current;
    return *this;
  }

  iterator_type base() const { return current; }

  reference operator*() const {
    Iter tmp = current;
    return *--tmp;
  }

  pointer operator->() const { return std::addressof(operator*()); }

  reference operator[](difference_type n) const { return *(*this + n); }

  reverse_iterator &operator++() {
    --current;
    return *this;
  }

  reverse_iterator &operator--() {
    ++current;
    return *this;
  }

  reverse_iterator operator++(int) {
    reverse_iterator tmp = *this;
    --current;
    return tmp;
  }

  reverse_iterator operator--(int) {
    reverse_iterator tmp = *this;
    ++current;
    return tmp;
  }

  reverse_iterator operator+(difference_type n) const {
    return reverse_iterator(current - n);
  }

  reverse_iterator operator-(difference_type n) const {
    return reverse_iterator(current + n);
  }

  reverse_iterator &operator+=(difference_type n) {
    current -= n;
    return *this;
  }

  reverse_iterator &operator-=(difference_type n) {
    current += n;
    return *this;
  }

  // TODO
protected:
  Iter current = Iter();
};

// Non-member functions
template <class Iter1, class Iter2>
bool operator==(
  const reverse_iterator<Iter1> &lhs, const reverse_iterator<Iter2> &rhs
) {
  return lhs.base() == rhs.base();
}

template <class Iter1, class Iter2>
bool operator!=(
  const reverse_iterator<Iter1> &lhs, const reverse_iterator<Iter2> &rhs
) {
  return lhs.base() != rhs.base();
}

template <class Iter1, class Iter2>
bool operator<(
  const reverse_iterator<Iter1> &lhs, const reverse_iterator<Iter2> &rhs
) {
  return lhs.base() > rhs.base();
}

template <class Iter1, class Iter2>
bool operator<=(
  const reverse_iterator<Iter1> &lhs, const reverse_iterator<Iter2> &rhs
) {
  return lhs.base() >= rhs.base();
}

template <class Iter1, class Iter2>
bool operator>(
  const reverse_iterator<Iter1> &lhs, const reverse_iterator<Iter2> &rhs
) {
  return lhs.base() < rhs.base();
}

template <class Iter1, class Iter2>
bool operator>=(
  const reverse_iterator<Iter1> &lhs, const reverse_iterator<Iter2> &rhs
) {
  return lhs.base() <= rhs.base();
}

template <class Iter1, class Iter2>
constexpr std::compare_three_way_result_t<Iter1, Iter2> operator<=>(
  const reverse_iterator<Iter1> &lhs, const reverse_iterator<Iter2> &rhs
) {
  return rhs.base() <=> lhs.base();
}

template <class Iter>
reverse_iterator<Iter> operator+(
  typename reverse_iterator<Iter>::difference_type n,
  const reverse_iterator<Iter> &it
) {
  return it + n;
}

template <class Iter1, class Iter2>
auto operator-(
  const reverse_iterator<Iter1> &lhs, const reverse_iterator<Iter2> &rhs
) -> decltype(rhs.base() - lhs.base()) {
  return rhs.base() - lhs.base();
}

// https://en.cppreference.com/w/cpp/iterator/make_reverse_iterator.html
template <class Iter>
reverse_iterator<Iter> make_reverse_iterator(Iter i) {
  return reverse_iterator<Iter>(i);
}

} // namespace tinystl
