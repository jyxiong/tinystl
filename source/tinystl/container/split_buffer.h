#pragma once

#include <algorithm>
#include <iterator>
#include <memory>
#include <type_traits>

#include "tinystl/memory/swap_allocator.h"

namespace tinystl {
template <class T, class Alloc>
class split_buffer {
public:
  using allocator_type = Alloc;
  using value_type = T;
  using reference = value_type &;
  using const_reference = const value_type &;
  using alloc_rr = std::remove_reference_t<allocator_type>;
  using alloc_traits = std::allocator_traits<allocator_type>;
  using size_type = typename alloc_traits::size_type;
  using difference_type = typename alloc_traits::difference_type;
  using pointer = typename alloc_traits::pointer;
  using const_pointer = typename alloc_traits::const_pointer;
  using iterator = pointer;
  using const_iterator = const_pointer;
  using sentinel_type = pointer;

  split_buffer(const split_buffer &) = delete;
  split_buffer &operator=(const split_buffer &) = delete;

  split_buffer() = default;
  explicit split_buffer(alloc_rr &a);
  explicit split_buffer(const alloc_rr &a);
  split_buffer(size_type cap, size_type start, alloc_rr &a);
  split_buffer(
    split_buffer &&c
  ) noexcept(std::is_nothrow_move_constructible_v<allocator_type>);
  split_buffer(split_buffer &&c, const alloc_rr &a);

  ~split_buffer();

  split_buffer &operator=(split_buffer &&c) noexcept(
    (alloc_traits::propagate_on_container_move_assignment::value &&
     std::is_nothrow_move_assignable_v<allocator_type>) ||
    !alloc_traits::propagate_on_container_move_assignment::value
  );

  allocator_type &get_allocator() noexcept { return m_alloc; }
  allocator_type const &get_allocator() const noexcept { return m_alloc; }

  // element access
  reference front() { return *m_begin; };
  const_reference front() const { return *m_begin; };
  reference back() noexcept { return *(m_end - 1); }
  const_reference back() const noexcept { return *(m_end - 1); }

  // iterators
  pointer front_cap() noexcept { return m_front_cap; }
  const_pointer front_cap() const noexcept { return m_front_cap; }
  pointer begin() noexcept { return m_begin; }
  const_pointer begin() const noexcept { return m_begin; }
  pointer end() noexcept { return m_end; }
  const_pointer end() const noexcept { return m_end; }
  sentinel_type raw_sentinel() const noexcept { return m_end; }
  sentinel_type raw_capacity() const noexcept { return m_back_cap; }

  // capacity
  bool empty() const noexcept { return m_begin == m_end; }
  size_type size() const noexcept {
    return static_cast<size_type>(m_end - m_begin);
  }
  size_type capacity() const noexcept {
    return static_cast<size_type>(m_back_cap - m_front_cap);
  }
  void shrink_to_fit() noexcept;
  size_type front_spare() const noexcept {
    return static_cast<size_type>(m_begin - m_front_cap);
  }
  size_type back_spare() const noexcept {
    return static_cast<size_type>(m_back_cap - m_end);
  }

  // modifiers
  void clear() noexcept { this->destruct_at_end(m_begin); };

  template <class... Args>
  void emplace_front(Args &&...args);
  template <class... Args>
  void emplace_back(Args &&...args);

  void pop_front();
  void pop_back();

  void swap(split_buffer &other) noexcept(
    !alloc_traits::propagate_on_container_swap::value ||
    std::is_nothrow_swappable_v<alloc_rr>
  );

  void swap_without_alloc(split_buffer<value_type, alloc_rr &> &other) noexcept;

  void copy_without_alloc(const split_buffer &other) noexcept(
    std::is_nothrow_copy_assignable<pointer>::value
  );

  void move_assign_alloc(split_buffer &c);

  void construct_at_end(size_type n);
  void construct_at_end(size_type n, const_reference x);
  template <std::forward_iterator ForwardIter>
    requires std::constructible_from<T, std::iter_reference_t<ForwardIter>>
  void construct_at_end(ForwardIter first, ForwardIter last);

  template <class Iterator, class Sentinel>
  void construct_at_end_with_sentinel(Iterator first, Sentinel last);

  template <class Iterator>
  void construct_at_end_with_size(Iterator first, size_type n);

  void destruct_at_begin(pointer new_begin);

  void destruct_at_end(pointer new_end) noexcept;

  bool invariants() const;

  void reset() noexcept {
    m_front_cap = nullptr;
    m_begin = nullptr;
    m_end = nullptr;
    m_back_cap = nullptr;
  }

private:
  Alloc m_alloc;
  pointer m_front_cap = nullptr;
  pointer m_begin = nullptr;
  pointer m_end = nullptr;
  pointer m_back_cap = nullptr;
};

/* ------------------------------- constructor ------------------------------ */
template <class T, class Alloc>
split_buffer<T, Alloc>::split_buffer(alloc_rr &a) : m_alloc(a) {}

template <class T, class Alloc>
split_buffer<T, Alloc>::split_buffer(const alloc_rr &a) : m_alloc(a) {}

template <class T, class Alloc>
split_buffer<T, Alloc>::split_buffer(
  size_type cap, size_type start, alloc_rr &a
)
  : m_alloc(a) {
  assert(cap >= start && "can't have a start point outside the capacity");

  if (cap > 0) {
    m_front_cap = alloc_traits::allocate(m_alloc, cap);
  }

  m_begin = m_front_cap + start;
  m_end = m_begin;
  m_back_cap = m_front_cap + cap;
}

template <class T, class Alloc>
split_buffer<T, Alloc>::split_buffer(
  split_buffer &&c
) noexcept(std::is_nothrow_move_constructible_v<allocator_type>) {
  m_front_cap = c.m_front_cap;
  m_begin = c.m_begin;
  m_end = c.m_end;
  m_back_cap = c.m_back_cap;
  c.reset();
}

template <class T, class Alloc>
split_buffer<T, Alloc>::split_buffer(split_buffer &&c, const alloc_rr &a)
  : m_alloc(a) {
  if (m_alloc == c.m_alloc) {
    m_front_cap = c.m_front_cap;
    m_begin = c.m_begin;
    m_end = c.m_end;
    m_back_cap = c.m_back_cap;
    c.reset();
  } else {
    size_type sz = c.size();
    m_front_cap = alloc_traits::allocate(m_alloc, sz);
    m_begin = m_front_cap;
    m_end = m_front_cap;
    m_back_cap = m_front_cap + sz;
    this->construct_at_end(
      std::make_move_iterator(c.begin()), std::make_move_iterator(c.end())
    );
  }
}

/* ------------------------------- destructor ------------------------------- */
template <class T, class Alloc>
split_buffer<T, Alloc>::~split_buffer() {
  this->clear();
  if (m_front_cap != nullptr) {
    alloc_traits::deallocate(m_alloc, m_front_cap, this->capacity());
  }
}

/* -------------------------------- operator= ------------------------------- */
template <class T, class Alloc>
split_buffer<T, Alloc> &
split_buffer<T, Alloc>::operator=(split_buffer &&c) noexcept(
  (alloc_traits::propagate_on_container_move_assignment::value &&
   std::is_nothrow_move_assignable_v<allocator_type>) ||
  !alloc_traits::propagate_on_container_move_assignment::value
) {
  this->clear();
  this->shrink_to_fit();
  this->copy_without_alloc(c);
  this->move_assign_alloc(c);
  c.reset();
  return *this;
}

/* -------------------------------- capacity -------------------------------- */
template <class T, class Alloc>
void split_buffer<T, Alloc>::shrink_to_fit() noexcept {
  size_type cap = this->capacity();
  size_type sz = this->size();
  if (cap > sz) {
    split_buffer<value_type, alloc_rr> sb(sz, 0, m_alloc);
    if (sb.capacity() < cap) {
      sb.construct_at_end(
        std::make_move_iterator(m_begin), std::make_move_iterator(m_end)
      );
      this->swap_without_alloc(sb);
    }
  }
}

/* -------------------------------- modifiers ------------------------------- */
template <class T, class Alloc>
template <class... Args>
void split_buffer<T, Alloc>::emplace_front(Args &&...args) {
  if (this->front_spare() == 0) {
    difference_type d = this->back_spare();
    if (d > 0) {
      d = (d + 1) / 2;
      pointer new_end = m_end + d;
      m_begin = std::move_backward(m_begin, m_end, new_end);
      m_end = new_end;
    } else {
      size_type sz = std::max<size_type>(2 * this->capacity(), 1);
      split_buffer<value_type, alloc_rr> sb(sz, (sz + 3) / 4, m_alloc);
      sb.construct_at_end(
        std::make_move_iterator(m_begin), std::make_move_iterator(m_end)
      );
      this->swap_without_alloc(sb);
    }
  }

  alloc_traits::construct(
    m_alloc, std::to_address(m_begin - 1), std::forward<Args>(args)...
  );
  --m_begin;
}

template <class T, class Alloc>
template <class... Args>
void split_buffer<T, Alloc>::emplace_back(Args &&...args) {
  if (this->back_spare() == 0) {
    difference_type d = this->front_spare();
    if (this->front_spare() > 0) {
      d = (d + 1) / 2;
      pointer new_begin = m_begin - d;
      m_end = std::move(m_begin, m_end, new_begin);
      m_begin = new_begin;
    } else {
      size_type sz = std::max<size_type>(2 * this->capacity(), 1);
      split_buffer<value_type, alloc_rr> sb(sz, sz / 4, m_alloc);
      sb.construct_at_end(
        std::make_move_iterator(m_begin), std::make_move_iterator(m_end)
      );
      this->swap_without_alloc(sb);
    }
  }

  alloc_traits::construct(
    m_alloc, std::to_address(m_end), std::forward<Args>(args)...
  );
  ++m_end;
}

template <class T, class Alloc>
void split_buffer<T, Alloc>::pop_front() {
  this->destruct_at_begin(m_begin + 1);
}

template <class T, class Alloc>
void split_buffer<T, Alloc>::pop_back() {
  this->destruct_at_end(m_end - 1);
}

template <class T, class Alloc>
void split_buffer<T, Alloc>::swap(split_buffer &other) noexcept(
  !alloc_traits::propagate_on_container_swap::value ||
  std::is_nothrow_swappable_v<alloc_rr>
) {
  std::swap(m_front_cap, other.m_front_cap);
  std::swap(m_begin, other.m_begin);
  std::swap(m_end, other.m_end);
  std::swap(m_back_cap, other.m_back_cap);
  swap_allocator(m_alloc, other.m_alloc);
}

template <class T, class Alloc>
void split_buffer<T, Alloc>::swap_without_alloc(
  split_buffer<value_type, alloc_rr &> &other
) noexcept {
  std::swap(m_front_cap, other.m_front_cap);
  std::swap(m_begin, other.m_begin);
  std::swap(m_end, other.m_end);
  std::swap(m_back_cap, other.m_back_cap);
}

template <class T, class Alloc>
void split_buffer<T, Alloc>::copy_without_alloc(
  const split_buffer &other
) noexcept(std::is_nothrow_copy_assignable<pointer>::value) {
  m_front_cap = other.m_front_cap;
  m_begin = other.m_begin;
  m_end = other.m_end;
  m_back_cap = other.m_back_cap;
}

template <class T, class Alloc>
void split_buffer<T, Alloc>::move_assign_alloc(split_buffer &c) {
  if constexpr (std::bool_constant<
                  alloc_traits::propagate_on_container_move_assignment::
                    value>()) {
    m_alloc = std::move(c.m_alloc);
  }
}

template <class T, class Alloc>
void split_buffer<T, Alloc>::construct_at_end(size_type n) {
  for (size_type i = 0; i < n; ++i) {
    alloc_traits::construct(m_alloc, std::to_address(m_end));
    ++m_end;
  }
}

template <class T, class Alloc>
void split_buffer<T, Alloc>::construct_at_end(size_type n, const_reference x) {
  for (size_type i = 0; i < n; ++i) {
    alloc_traits::construct(m_alloc, std::to_address(m_end), x);
    ++m_end;
  }
}

template <class T, class Alloc>
template <std::forward_iterator ForwardIter>
  requires std::constructible_from<T, std::iter_reference_t<ForwardIter>>
void split_buffer<T, Alloc>::construct_at_end(
  ForwardIter first, ForwardIter last
) {
  this->construct_at_end_with_size(first, std::distance(first, last));
}

template <class T, class Alloc>
template <class Iterator, class Sentinel>
void split_buffer<T, Alloc>::construct_at_end_with_sentinel(
  Iterator first, Sentinel last
) {
  for (; first != last; ++first) {
    if (this->back_spare() == 0) {
      size_type old_cap = this->capacity();
      size_type new_cap = std::max<size_type>(2 * old_cap, 8);
      split_buffer buf(new_cap, 0, m_alloc);

      for (pointer p = m_begin; p != m_end; ++p) {
        alloc_traits::construct(
          m_alloc, std::to_address(buf.m_end), std::move(*p)
        );
        ++buf.m_end;
      }
      this->swap(buf);
    }

    alloc_traits::construct(m_alloc, std::to_address(m_end), *first);
    ++m_end;
  }
}

template <class T, class Alloc>
template <class Iterator>
void split_buffer<T, Alloc>::construct_at_end_with_size(
  Iterator first, size_type n
) {
  for (size_type i = 0; i < n; ++i) {
    alloc_traits::construct(m_alloc, std::to_address(m_end), *first);
    ++m_end;
    ++first;
  }
}

template <class T, class Alloc>
void split_buffer<T, Alloc>::destruct_at_begin(pointer new_begin) {
  if constexpr (std::is_trivially_destructible_v<value_type>) {
    m_begin = new_begin;
  } else {
    while (m_begin != new_begin) {
      alloc_traits::destroy(m_alloc, std::to_address(m_begin));
      ++m_begin;
    }
  }
}

template <class T, class Alloc>
void split_buffer<T, Alloc>::destruct_at_end(pointer new_end) noexcept {
  if constexpr (std::is_trivially_destructible_v<value_type>) {
    m_end = new_end;
  } else {
    while (m_end != new_end) {
      alloc_traits::destroy(m_alloc, std::to_address(--m_end));
    }
  }
}

template <class T, class Alloc>
bool split_buffer<T, Alloc>::invariants() const {
  // basic invariants: pointers must be ordered and either all null or all non-null
  bool all_null = (m_front_cap == nullptr && m_begin == nullptr &&
                   m_end == nullptr && m_back_cap == nullptr);
  if (all_null) return true;
  if (!(m_front_cap <= m_begin && m_begin <= m_end && m_end <= m_back_cap))
    return false;
  return true;
}

} // namespace tinystl