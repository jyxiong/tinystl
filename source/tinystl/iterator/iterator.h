#pragma once

#include <cstddef>

// https://en.cppreference.com/w/cpp/iterator/iterator_tags.html
// https://en.cppreference.com/w/cpp/iterator/iterator.html
// https://en.cppreference.com/w/cpp/iterator/iterator_traits.html

namespace tinystl {
// iteratror category tags
struct input_iterator_tag {};

struct output_iterator_tag {};

struct forward_iterator_tag : public input_iterator_tag {};

struct bidirectional_iterator_tag : public forward_iterator_tag {};

struct random_access_iterator_tag : public bidirectional_iterator_tag {};

struct contiguous_iterator_tag : public random_access_iterator_tag {};

// iterator template
template <typename Category, typename T, typename Distance = ptrdiff_t,
          typename Pointer = T *, typename Reference = T &>
struct iterator {
  using iterator_category = Category;
  using value_type = T;
  using difference_type = Distance;
  using pointer = Pointer;
  using reference = Reference;
};

// iterator_traits template
template <typename Iterator> struct iterator_traits {
  using iterator_category = typename Iterator::iterator_category;
  using value_type = typename Iterator::value_type;
  using difference_type = typename Iterator::difference_type;
  using pointer = typename Iterator::pointer;
  using reference = typename Iterator::reference;
};

// Specialization for raw pointers
template <typename T> struct iterator_traits<T *> {
  using iterator_category = random_access_iterator_tag;
  using value_type = T;
  using difference_type = ptrdiff_t;
  using pointer = T *;
  using reference = T &;
};

// Specialization for const raw pointers
template <typename T> struct iterator_traits<const T *> {
  using iterator_category = random_access_iterator_tag;
  using value_type = T;
  using difference_type = ptrdiff_t;
  using pointer = const T *;
  using reference = const T &;
};

} // namespace tinystl