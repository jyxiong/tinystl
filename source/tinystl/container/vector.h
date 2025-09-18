#pragma once

#include "tinystl/memory/allocator/allocator.h"
#include "tinystl/memory/allocator/allocator_traits.h"

namespace tinystl {

template <class T, class Allocator = allocator<T>> class vector {
public:
  using value_type = T;
  using allocator_type = Allocator;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using reference = value_type &;
  using const_reference = const value_type &;
  using pointer = allocator_traits<Allocator>::pointer;
  using const_pointer = allocator_traits<Allocator>::const_pointer;
  using iterator = pointer;
  using const_iterator = const_pointer;

  // construct/copy/destroy
  vector() noexcept(noexcept(Allocator())) : vector(Allocator()) {}

  explicit vector(const Allocator &alloc) noexcept
      : m_alloc(alloc), m_data(nullptr), m_size(0), m_capacity(0) {}

  explicit vector(size_type n, const Allocator &alloc = Allocator())
      : vector(alloc) {
    m_data = allocator_traits<Allocator>::allocate(m_alloc, n);
    for (size_type i = 0; i < n; ++i) {
      allocator_traits<Allocator>::construct(m_alloc, m_data + i);
    }
    m_size = n;
    m_capacity = n;
  }

  vector(size_type n, const T &value, const Allocator &alloc = Allocator())
      : m_alloc(alloc) {
    m_data = allocator_traits<Allocator>::allocate(m_alloc, n);
    for (size_type i = 0; i < n; ++i) {
      allocator_traits<Allocator>::construct(m_alloc, m_data + i, value);
    }
  }

  template <class InputIter>
  vector(InputIter first, InputIter last, const Allocator &alloc = Allocator());

  vector(const vector &other);

  vector(vector &&) noexcept;

  vector(const vector &other, const Allocator &alloc);

  vector(vector &&other, const Allocator &alloc);

  vector(std::initializer_list<T>, const Allocator &alloc = Allocator());

private:
  Allocator m_alloc;
  pointer m_data;
  size_type m_size;
  size_type m_capacity;
};

// TODO: specialization for bool
// template <class Allocator> class vector<bool, Allocator> {
// };

} // namespace tinystl