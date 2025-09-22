#pragma once

#include <utility>

#include "tinystl/iterator/iterator_tag.h"

namespace tinystl {

// https://en.cppreference.com/w/cpp/iterator/front_insert_iterator.html
template <class Container>
class front_insert_iterator {
public:
  // Member types
  using iterator_category = output_iterator_tag;
  using value_type = void;
  using difference_type = ptrdiff_t;
  using pointer = void;
  using reference = void;

  // Member functions
  explicit front_insert_iterator(Container &c) : container(&c) {}

  front_insert_iterator &
  operator=(const typename Container::value_type &value) {
    container->push_front(value);
    return *this;
  }

  front_insert_iterator &operator=(typename Container::value_type &&value) {
    container->push_front(std::move(value));
    return *this;
  }

  front_insert_iterator &operator*() { return *this; }

  front_insert_iterator &operator++() { return *this; }

  front_insert_iterator operator++(int) { return *this; }

protected:
  // Member objects
  Container *container;
};

// https://en.cppreference.com/w/cpp/iterator/front_inserter.html
template <class Container>
front_insert_iterator<Container> front_inserter(Container &c) {
  return front_insert_iterator<Container>(c);
}
} // namespace tinystl
