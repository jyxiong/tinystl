#pragma once

#include <concepts>
#include <iterator>
#include <memory>

#include "tinystl/container/vector.h"
#include "tinystl/iterator/segmented_iterator.h"

namespace tinystl {

template <class ValueType, class DiffType>
struct deque_block_size {
  static const DiffType value =
    sizeof(ValueType) < 256 ? 4096 / sizeof(ValueType) : 16;
};

template <
  class ValueType, class Pointer, class Reference, class MapPointer,
  class DiffType,
  DiffType BlockSize = deque_block_size<ValueType, DiffType>::value>
class deque_iterator {
  using map_iterator = MapPointer;

public:
  using iterator_category = std::random_access_iterator_tag;
  using value_type = ValueType;
  using difference_type = DiffType;
  using reference = Reference;
  using pointer = Pointer;

  deque_iterator() : m_iter(nullptr), m_ptr(nullptr) {}
  template <class P, class R, class M>
    requires std::is_convertible_v<P, pointer>
  deque_iterator(const deque_iterator &it)
    : m_iter(it.m_iter), m_ptr(it.m_ptr) {}

  reference operator*() const { return *m_ptr; }
  pointer operator->() const { return m_ptr; }

  deque_iterator &operator++() {
    if (++m_ptr - *m_iter == m_block_size) {
      ++m_iter;
      m_ptr = *m_iter;
    }
    return *this;
  }

  deque_iterator operator++(int) {
    deque_iterator tmp = *this;
    ++(*this);
    return tmp;
  }

  deque_iterator &operator--() {
    if (m_ptr == *m_iter) {
      --m_iter;
      m_ptr = *m_iter + m_block_size;
    }
    --m_ptr;
    return *this;
  }

  deque_iterator operator--(int) {
    deque_iterator tmp = *this;
    --(*this);
    return tmp;
  }

  deque_iterator &operator+=(difference_type n) {
    if (n != 0) {
      n += m_ptr - *m_iter;
      if (n > 0) {
        m_iter += n / m_block_size;
        m_ptr = *m_iter + n % m_block_size;
      } else {
        difference_type z = m_block_size - 1 - n;
        m_iter -= z / m_block_size;
        m_ptr = *m_iter + (m_block_size - 1 - z % m_block_size);
      }
    }

    return *this;
  }

  deque_iterator &operator-=(difference_type n) { return *this += -n; }

  deque_iterator operator+(difference_type n) const {
    deque_iterator t(*this);
    t += n;
    return t;
  }

  deque_iterator operator-(difference_type n) const {
    deque_iterator t(*this);
    t -= n;
    return t;
  }

  reference operator[](difference_type n) const { return *(*this + n); }

  friend deque_iterator operator+(difference_type n, const deque_iterator &it) {
    return it + n;
  }

  friend difference_type
  operator-(const deque_iterator &x, const deque_iterator &y) {
    if (x != y)
      return (x.m_iter - y.m_iter) * m_block_size + (x.m_ptr - *x.m_iter) -
             (y.m_ptr - *y.m_iter);
    return 0;
  }

  friend bool operator==(const deque_iterator &x, const deque_iterator &y) {
    return x.m_ptr == y.m_ptr;
  }

  friend std::strong_ordering
  operator<=>(const deque_iterator &x, const deque_iterator &y) {
    if (x.m_iter < y.m_iter) {
      return std::strong_ordering::less;
    }

    if (x.m_iter == y.m_iter) {
      if constexpr (std::three_way_comparable<pointer, std::strong_ordering>) {
        x.m_ptr <=> y.m_ptr;
      } else {
        if (x.m_ptr < y.m_ptr) {
          return std::strong_ordering::less;
        }

        if (x.m_ptr == y.m_ptr) {
          return std::strong_ordering::equal;
        }

        return std::strong_ordering::greater;
      }
    }

    return std::strong_ordering::greater;
  }

private:
  deque_iterator(map_iterator m, pointer p) noexcept : m_iter(m), m_ptr(p) {}

  template <class T, class Alloc>
  friend class deque;

  template <class V, class P, class R, class M, class D, D>
  friend class deque_iterator;

  template <class>
  friend struct segmented_iterator_traits;

private:
  map_iterator m_iter;
  pointer m_ptr;

  static const difference_type m_block_size;
  ;
};

template <
  class ValueType, class Pointer, class Reference, class MapPointer,
  class DiffType, DiffType BlockSize>
const DiffType deque_iterator<
  ValueType, Pointer, Reference, MapPointer, DiffType,
  BlockSize>::m_block_size = deque_block_size<ValueType, DiffType>::value;

template <
  class ValueType, class Pointer, class Reference, class MapPointer,
  class DiffType, DiffType BlockSize>
struct segmented_iterator_traits<deque_iterator<
  ValueType, Pointer, Reference, MapPointer, DiffType, BlockSize>> {
private:
  using Iterator = deque_iterator<
    ValueType, Pointer, Reference, MapPointer, DiffType, BlockSize>;

public:
  using segment_iterator = MapPointer;
  using local_iterator = Pointer;

  static segment_iterator segment(Iterator iter) { return iter.m_iter; }
  static local_iterator local(Iterator iter) { return iter.m_ptr; }

  static local_iterator begin(segment_iterator iter) { return *iter; }
  static local_iterator end(segment_iterator iter) {
    return *iter + Iterator::m_block_size;
  }

  static Iterator compose(segment_iterator segment, local_iterator local) {
    if (segment && local == end(segment)) {
      ++segment;
      return _Iterator(segment, *segment);
    }
    return _Iterator(segment, local);
  }
};

} // namespace tinystl