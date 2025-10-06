#pragma once

#include <concepts>
#include <iterator>
#include <memory>

#include "tinystl/container/split_buffer.h"
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

template <class T, class Alloc = std::allocator<T>>
class deque {
  using alloc_traits = std::allocator_traits<Alloc>;

  using pointer_alloc =
    alloc_traits::template rebind_alloc<typename alloc_traits::pointer>;
  using const_pointer_allcoator =
    alloc_traits::template rebind_alloc<typename alloc_traits::pointer>;

  using map = split_buffer<typename alloc_traits::pointer, pointer_alloc>;
  using map_pointer = typename std::allocator_traits<pointer_alloc>::pointer;
  using map_const_pointer =
    typename std::allocator_traits<const_pointer_allcoator>::const_pointer;
  using map_const_iterator = typename map::const_iterator;

public:
  using value_type = T;
  using reference = value_type &;
  using const_reference = const value_type &;

  using allocator_type = Alloc;

  using size_type = typename alloc_traits::size_type;
  using difference_type = typename alloc_traits::difference_type;

  using pointer = typename alloc_traits::pointer;
  using const_pointer = typename alloc_traits::const_pointer;

  using iterator = deque_iterator<
    value_type, pointer, reference, map_pointer, difference_type>;
  using const_iterator = deque_iterator<
    value_type, const_pointer, const_reference, map_const_pointer,
    difference_type>;

  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  // construct/copy/destroy
  deque();
  explicit deque(const Alloc &alloc);
  explicit deque(size_type n, const Alloc &alloc = Alloc());
  deque(size_type n, const_reference val, const Alloc &alloc = Alloc());
  template <class InputIt>
  deque(InputIt first, InputIt last, const Alloc &alloc = Alloc());
  deque(const deque &other);
  deque(const deque &other, const Alloc &alloc);
  deque(deque &&other) noexcept;
  deque(deque &&other, const Alloc &alloc);
  deque(std::initializer_list<value_type> init, const Alloc &alloc = Alloc());

  ~deque();

  deque &operator=(const deque &other);
  deque &operator=(deque &&other);
  deque &operator=(std::initializer_list<value_type> init);

  void assign(size_type n, const_reference val);
  template <class InputIt>
  void assign(InputIt first, InputIt last);
  void assign(std::initializer_list<value_type> ilist);

  allocator_type get_allocator() const;

  // element access
  reference at(size_type pos);
  const_reference at(size_type pos) const;
  reference operator[](size_type pos);
  const_reference operator[](size_type pos) const;
  reference front();
  const_reference front() const;
  reference back();
  const_reference back() const;

  // iterators
  iterator begin() noexcept;
  const_iterator begin() const noexcept;
  const_iterator cbegin() const noexcept;
  iterator end() noexcept;
  const_iterator end() const noexcept;
  const_iterator cend() const noexcept;
  reverse_iterator rbegin() noexcept;
  const_reverse_iterator rbegin() const noexcept;
  const_reverse_iterator crbegin() const noexcept;
  reverse_iterator rend() noexcept;
  const_reverse_iterator rend() const noexcept;
  const_reverse_iterator crend() const noexcept;

  // capacity
  bool empty() const noexcept;
  size_type size() const noexcept;
  size_type max_size() const noexcept;
  void shrink_to_fit();

  // modifiers
  void clear() noexcept;
  iterator insert(const_iterator pos, const_reference val);
  iterator insert(const_iterator pos, value_type &&val);
  iterator insert(const_iterator pos, size_type n, const_reference val);
  template <std::forward_iterator ForwardIter>
    requires std::constructible_from<T, std::iter_reference_t<ForwardIter>>
  iterator insert(const_iterator pos, ForwardIter first, ForwardIter last);
  template <std::input_iterator InputIter>
    requires std::constructible_from<T, std::iter_reference_t<InputIter>> &&
             (!std::forward_iterator<InputIter>)
  iterator insert(const_iterator pos, InputIter first, InputIter last);
  iterator insert(const_iterator pos, std::initializer_list<T> init);
  template <class... Args>
  iterator emplace(const_iterator pos, Args &&...args);
  iterator erase(const_iterator pos);
  iterator erase(const_iterator first, const_iterator last);
  void push_back(const_reference val);
  void push_back(value_type &&val);
  template <class... Args>
  reference emplace_back(Args &&...args);
  void pop_back();
  void push_front(const_reference val);
  void push_front(value_type &&val);
  template <class... Args>
  reference emplace_front(Args &&...args);
  void pop_front();
  void resize(size_type n);
  void resize(size_type n, const_reference val);
  void swap(deque &other) noexcept(alloc_traits::is_always_equal::value);

private:
  size_type capacity() const;
  size_type front_spare() const;
  size_type back_spare() const;

  void append(size_type n);
  void append(size_type n, const value_type &val);

  void add_front_capacity();
  void add_front_capacity(size_type n);
  void add_back_capacity();
  void add_back_capacity(size_type n);

  void throw_length_error();
  void throw_out_of_range();

  size_type recommend(size_type new_size);

  void allocate(size_type n);

  void deallocate();

  void construct(size_type n);
  void construct(size_type n, const_reference val);
  template <class InputIter, class Sentinel>
  void construct(InputIter first, Sentinel last, size_type n);

  void destruct(pointer new_last);

  void move_to_insert(pointer first, pointer last, pointer dst_first);

private:
  static const difference_type m_block_size;

  Alloc m_alloc;
  map m_map;
  size_type m_start;
  size_type m_size;
};

/* -------------------------------------------------------------------------- */
/*                           static member variable                           */
/* -------------------------------------------------------------------------- */
template <class T, class Alloc>
const typename deque<T, Alloc>::difference_type deque<T, Alloc>::m_block_size =
  deque_block_size<value_type, difference_type>::value;

template <class T, class Alloc>
bool operator==(const deque<T, Alloc> &lhs, const deque<T, Alloc> &rhs);

template <class T, class Alloc>
auto operator<=>(const deque<T, Alloc> &lhs, const deque<T, Alloc> &rhs);

template <class T, class Alloc>
void swap(deque<T, Alloc> &lhs, deque<T, Alloc> &rhs) noexcept(
  noexcept(lhs.swap(rhs))
);

template <class T, class Alloc, class U>
typename deque<T, Alloc>::size_type erase(vector<T, Alloc> &c, const U &val);

template <class T, class Alloc, class Pred>
typename deque<T, Alloc>::size_type erase_if(deque<T, Alloc> &c, Pred pred);

template <class T, class Alloc>
deque<T, Alloc>::deque() : m_start(0), m_size(0) {}

template <class T, class Alloc>
deque<T, Alloc>::deque(const Alloc &alloc)
  : m_map(pointer_alloc(alloc)), m_start(0), m_size(0) {}

template <class T, class Alloc>
deque<T, Alloc>::deque(size_type n, const Alloc &alloc)
  : m_map(pointer_alloc(alloc)), m_start(0), m_size(0) {
  if (n > 0) {
    this->append(n);
  }
}

template <class T, class Alloc>
deque<T, Alloc>::deque(size_type n, const_reference val, const Alloc &alloc)
  : m_map(pointer_alloc(alloc)), m_start(0), m_size(0) {
  if (n > 0) {
    this->append(n, val);
  }
}

template <class T, class Alloc>
template <class InputIt>
deque<T, Alloc>::deque(InputIt first, InputIt last, const Alloc &alloc) {}

template <class T, class Alloc>
deque<T, Alloc>::deque(const deque &other) {}

template <class T, class Alloc>
deque<T, Alloc>::deque(const deque &other, const Alloc &alloc) {}

template <class T, class Alloc>
deque<T, Alloc>::deque(deque &&other) noexcept {}

template <class T, class Alloc>
deque<T, Alloc>::deque(deque &&other, const Alloc &alloc) {}

template <class T, class Alloc>
deque<T, Alloc>::deque(
  std::initializer_list<value_type> init, const Alloc &alloc
) {}

/* -------------------------------------------------------------------------- */
/*                          private member functions                          */
/* -------------------------------------------------------------------------- */
template <class T, class Alloc>
typename deque<T, Alloc>::size_type deque<T, Alloc>::capacity() const {
  return m_map.size() == 0 ? 0 : m_map.size() * m_block_size - 1;
}

template <class T, class Alloc>
typename deque<T, Alloc>::size_type deque<T, Alloc>::front_spare() const {
  return m_start;
}

template <class T, class Alloc>
typename deque<T, Alloc>::size_type deque<T, Alloc>::back_spare() const {
  return this->capacity() - (m_start + m_size); 
}

template <class T, class Alloc>
void deque<T, Alloc>::append(size_type n) {
  size_type back_cap = this->back_spare();
  if (n > back_cap) {
    this->add_back_capacity(n - back_cap);
  }
}

template <class T, class Alloc>
void deque<T, Alloc>::append(size_type n, const value_type &val) {
  size_type back_cap = this->back_spare();
  if (n > back_cap) {
    this->add_back_capacity(n - back_cap);
  }
}
} // namespace tinystl