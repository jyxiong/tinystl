#pragma once

#include "tinystl/memory/allocator/allocator.h"
#include "tinystl/memory/allocator/allocator_traits.h"

namespace tinystl {
template <class T, class Allocator = allocator<T>>
class vector {
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
    : m_alloc(alloc), m_size(0), m_capacity(0), m_data(nullptr) {}

  explicit vector(size_type n, const Allocator &alloc = Allocator())
    : m_alloc(alloc), m_size(n), m_capacity(n), m_data(nullptr) {
    m_data = allocator_traits<Allocator>::allocate(m_alloc, m_capacity);
    for (size_type i = 0; i < m_size; ++i) {
      allocator_traits<Allocator>::construct(m_alloc, m_data + i);
    }
  }

  vector(size_type n, const T &value, const Allocator &alloc = Allocator())
    : m_alloc(alloc), m_size(n), m_capacity(n), m_data(nullptr) {
    m_data = allocator_traits<Allocator>::allocate(m_alloc, m_capacity);
    for (size_type i = 0; i < m_size; ++i) {
      allocator_traits<Allocator>::construct(m_alloc, m_data + i, value);
    }
  }

  template <class InputIter>
  vector(InputIter first, InputIter last, const Allocator &alloc = Allocator())
    : m_alloc(alloc) {
    auto n = last - first;
    m_size = m_capacity = n;
    m_data = allocator_traits<Allocator>::allocate(m_alloc, m_capacity);
    for (size_type i = 0; i < m_size; ++i) {
      allocator_traits<Allocator>::construct(m_alloc, m_data + i, *first++);
    }
  }

  vector(const vector &other)
    : m_alloc(
        allocator_traits<Allocator>::select_on_container_copy_construction(
          other.m_alloc
        )
      ),
      m_size(other.m_size), m_capacity(other.m_size), m_data(nullptr) {
    m_data = allocator_traits<Allocator>::allocate(m_alloc, m_capacity);
    for (size_type i = 0; i < m_size; ++i) {
      allocator_traits<Allocator>::construct(
        m_alloc, m_data + i, other.m_data[i]
      );
    }
  }

  vector(vector &&other) noexcept
    : m_alloc(std::move(other.m_alloc)), m_size(other.m_size),
      m_capacity(other.m_size), m_data(other.m_data) {
    other.m_data = nullptr;
    other.m_size = 0;
    other.m_capacity = 0;
  }

  vector(const vector &other, const Allocator &alloc)
    : m_alloc(alloc), m_size(other.m_size), m_capacity(other.m_size),
      m_data(nullptr) {
    m_data = allocator_traits<Allocator>::allocate(m_alloc, m_capacity);
    for (size_type i = 0; i < m_size; ++i) {
      allocator_traits<Allocator>::construct(
        m_alloc, m_data + i, other.m_data[i]
      );
    }
  }

  vector(vector &&other, const Allocator &alloc)
    : m_alloc(
        allocator_traits<Allocator>::select_on_container_copy_construction(alloc
        )
      ),
      m_size(other.m_size), m_capacity(other.m_size), m_data(other.m_data) {
    other.m_data = nullptr;
    other.m_size = 0;
    other.m_capacity = 0;
  }

  vector(std::initializer_list<T> init, const Allocator &alloc = Allocator())
    : vector(init.begin(), init.end(), alloc) {}

  ~vector() {
    for (size_t i = 0; i < m_size; ++i) {
      allocator_traits<Allocator>(m_alloc).destroy(m_data + i);
    }

    allocator_traits<Allocator>::deallocate(m_alloc, m_data, m_capacity);
  }

  vector &operator=(const vector &other);

  vector &operator=(vector &&other);

  vector &operator=(std::initializer_list<value_type> init);

private:
  Allocator m_alloc;
  size_type m_size;
  size_type m_capacity;
  pointer m_data;
};

// TODO: specialization for bool
// template <class Allocator> class vector<bool, Allocator> {
// };
} // namespace tinystl
