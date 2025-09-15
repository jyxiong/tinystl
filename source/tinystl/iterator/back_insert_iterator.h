#pragma once

#include <utility>

#include "tinystl/iterator/iterator.h"

namespace tinystl {

  
// https://en.cppreference.com/w/cpp/iterator/back_insert_iterator.html
template <typename Container> class back_insert_iterator {
public:
  // Member types
  using iterator_category = output_iterator_tag;
  using value_type = void;
  using difference_type = ptrdiff_t;
  using pointer = void;
  using reference = void;

  // Member functions
  explicit back_insert_iterator(Container &c) : container(&c) {}

  back_insert_iterator &operator=(const typename Container::value_type &value) {
    container->push_back(value);
    return *this;
  }

  back_insert_iterator &operator=(typename Container::value_type &&value) {
    container->push_back(std::move(value));
    return *this;
  }

  back_insert_iterator &operator*() { return *this; }

  back_insert_iterator &operator++() { return *this; }

  back_insert_iterator operator++(int) { return *this; }

protected:
  // Member objects
  Container *container;
};

// https://en.cppreference.com/w/cpp/iterator/back_inserter.html
template <typename Container>
back_insert_iterator<Container> back_inserter(Container &c) {
  return back_insert_iterator<Container>(c);
}

} // namespace tinystl