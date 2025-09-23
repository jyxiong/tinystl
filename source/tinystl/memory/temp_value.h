#pragma once

#include "tinystl/memory/addressof.h"
#include "tinystl/memory/allocator_traits.h"

namespace tinystl {
template <class T, class Alloc>
class temp_value {
public:
  template <class... Args>
  temp_value(Alloc &alloc, Args &&...args) : m_alloc(alloc) {
    tinystl::allocator_traits<Alloc>::construct(
      m_alloc, addr(), std::forward<Args>(args)...
    );
  }

  ~temp_value() { tinystl::allocator_traits<Alloc>::destroy(m_alloc, addr()); }

  T &get() noexcept { return *addr(); }

private:
  T *addr() { return addressof(m_value); }

private:
  union {
    T m_value;
  };

  Alloc m_alloc;
};
} // namespace tinystl