#pragma once

#include <utility>

#include "tinystl/iterator/iterator.h"

namespace tinystl {
// https://en.cppreference.com/w/cpp/iterator/insert_iterator.html
template <typename Container>
class insert_iterator {
public:
  // Member types
  using iterator_category = output_iterator_tag;
  using value_type = void;
  using difference_type = ptrdiff_t;
  using pointer = void;
  using reference = void;

  // Member functions
  insert_iterator(Container &c, typename Container::iterator it)
    : container(&c), iter(it) {}

  insert_iterator &operator=(const typename Container::value_type &value) {
    iter = container->insert(iter, value);
    ++iter;
    return *this;
  }

  insert_iterator &operator=(typename Container::value_type &&value) {
    iter = container->insert(iter, std::move(value));
    ++iter;
    return *this;
  }

  insert_iterator &operator*() { return *this; }

  insert_iterator &operator++() { return *this; }

  insert_iterator operator++(int) { return *this; }

protected:
  Container *container;
  typename Container::iterator iter;
};

template <typename Container>
insert_iterator<Container>
inserter(Container &c, typename Container::iterator it) {
  return insert_iterator<Container>(c, it);
}

} // namespace tinystl