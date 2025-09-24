#pragma once

#include "tinystl/iterator/reverse_iterator.h"
#include "tinystl/memory/addressof.h"
#include "tinystl/memory/allocate_at_least.h"
#include "tinystl/memory/allocator.h"
#include "tinystl/memory/allocator_traits.h"
#include "tinystl/memory/to_address.h"
#include "tinystl/type_traits/enable_if.h"
#include "tinystl/type_traits/integral_constant.h"
#include "tinystl/type_traits/is_same.h"

#include <cstddef>
#include <stdexcept>

namespace tinystl {
template <class T, class Allocator = allocator<T>>
class vector {
  using alloc_traits = allocator_traits<Allocator>;

public:
  using value_type = T;
  using allocator_type = Allocator;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using reference = value_type &;
  using const_reference = const value_type &;
  using pointer = alloc_traits::pointer;
  using const_pointer = alloc_traits::const_pointer;
  using iterator = pointer;
  using const_iterator = const_pointer;
  using reverse_iterator = tinystl::reverse_iterator<iterator>;
  using const_reverse_iterator = tinystl::reverse_iterator<const_iterator>;

  // construct/copy/destroy
  vector() noexcept(noexcept(Allocator()));
  explicit vector(const Allocator &alloc) noexcept;
  explicit vector(size_type n, const Allocator &alloc = Allocator());
  vector(size_type n, const_reference value, const Allocator &alloc = Allocator());
  template <class InputIter>
  vector(InputIter first, InputIter last, const Allocator &alloc = Allocator());
  vector(const vector &other);
  vector(vector &&other) noexcept;
  vector(const vector &other, const Allocator &alloc);
  vector(vector &&other, const Allocator &alloc);
  vector(std::initializer_list<T> init, const Allocator &alloc = Allocator());

  ~vector();

  vector &operator=(const vector &other);
  vector &operator=(vector &&other);
  vector &operator=(std::initializer_list<value_type> init);

  void assign(size_type n, const_reference value);
  template <class InputIt>
  void assign(InputIt first, InputIt last);
  void assign(std::initializer_list<T> ilist);

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
  T *data() noexcept;
  const T *data() const noexcept;

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
  void reserve(size_type new_cap);
  size_type capacity() const noexcept;
  void shrink_to_fit();

  // modifiers
  void clear() noexcept;
  iterator insert(const_iterator pos, const_reference value);
  iterator insert(const_iterator pos, T &&value);
  iterator insert(const_iterator pos, size_type n, const_reference value);
  template <class InputIter>
  iterator insert(const_iterator position, InputIter first, InputIter last);
  template <class... Args>
  iterator emplace(const_iterator pos, Args &&...args);
  iterator erase(const_iterator pos);
  iterator erase(const_iterator first, const_iterator last);
  void push_back(const_reference value);
  void push_back(T &&value);
  template <class... Args>
  reference emplace_back(Args &&...args);
  void pop_back();
  void resize(size_type sz);
  void resize(size_type sz, const value_type &value);
  void swap(vector &other) noexcept(
    alloc_traits::propagate_on_container_swap::value ||
    alloc_traits::is_always_equal::value
  );

private:
  struct ConstructTransaction {
    vector &v;
    pointer pos;
    const_pointer const new_end;

    ConstructTransaction(vector &v, size_type n);

    ~ConstructTransaction();

    ConstructTransaction(const ConstructTransaction &) = delete;
    ConstructTransaction &operator=(ConstructTransaction &) = delete;
  };

  class destroy_vector {
  public:
    destroy_vector(vector &vec);

    void operator()();

  private:
    vector &m_vec;
  };

  void allocate(size_type n);
  void deallocate();
  size_type recommend(size_type new_size) const;

  void construct_at_end(size_type n);
  void construct_at_end(size_type n, const_reference value);
  template <class InputIterator, class Sential>
  void construct_at_end(InputIterator first, Sential last, size_type n);

  void destroy_at_end(pointer new_last) noexcept;
  void base_destroy_at_end(pointer new_last) noexcept;

  template <class InputIterator, class Sential>
  void init_with_size(InputIterator first, Sential last, size_type n);
  template <class InputIterator, class Sential>
  void init_with_sential(InputIterator first, Sential last);

  template <class Iterator, class Sential>
  void assign_with_sential(Iterator first, Sential last);
  template <class Iterator, class Sential>
  void assign_with_size(Iterator first, Sential last, size_type n);

  template <
    class Iterator,
    enable_if_t<
      !is_same_v<decltype(*std::declval<Iterator &>()) &&, value_type &&>,
      int> = 0>
  void
  insert_assign_n_unchecked(Iterator first, difference_type n, pointer pos);
  template <
    class Iterator,
    enable_if_t<
      is_same_v<decltype(*std::declval<Iterator &>()) &&, value_type &&>, int> =
      0>
  void
  insert_assign_n_unchecked(Iterator first, difference_type n, pointer pos);
  template <class Iterator, class Sential>
  void insert_with_sential(const_iterator pos, Iterator first, Sential last);
  template <class Iterator, class Sential>
  void insert_with_size(
    const_iterator pos, Iterator first, Sential last, size_type n
  );

  void append(size_type n);
  void append(size_type n, const_reference value);

  void copy_assign_alloc(const vector &other);
  void copy_assign_alloc(const vector &other, true_type);
  void copy_assign_alloc(const vector &other, false_type);

  void move_assign_alloc(vector &other);
  void move_assign_alloc(vector &other, true_type);
  void move_assign_alloc(vector &other, false_type);

  void move_assign(vector &other, true_type);
  void move_assign(vector &other, false_type);

  void throw_length_error() const;

private:
  Allocator m_alloc;

  size_type m_capacity;
  size_type m_size;
  pointer m_data;

  pointer m_begin;
  pointer m_end;
  pointer m_cap;
};

template <class T, class Alloc>
bool operator==(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs);

template <class T, class Alloc>
auto operator<=>(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs);

template <class T, class Alloc>
void swap(vector<T, Alloc> &lhs, vector<T, Alloc> &rhs) noexcept;

template <class T, class Alloc, class U>
typename vector<T, Alloc>::size_type erase(vector<T, Alloc> &c, const U &value);

template <class T, class Alloc, class Pred>
typename vector<T, Alloc>::size_type erase_if(vector<T, Alloc> &c, Pred pred);

// construct/copy/destroy
template <class T, class Allocator>
vector<T, Allocator>::vector() noexcept(noexcept(Allocator()))
  : vector(Allocator()) {}

template <class T, class Allocator>
vector<T, Allocator>::vector(const Allocator &alloc) noexcept
  : m_alloc(alloc), m_capacity(0), m_size(0), m_data(nullptr) {}

template <class T, class Allocator>
vector<T, Allocator>::vector(size_type n, const Allocator &alloc)
  : m_alloc(alloc), m_capacity(n), m_size(n), m_data(nullptr) {
  m_data = alloc_traits::allocate(m_alloc, m_capacity);
  for (size_type i = 0; i < m_size; ++i) {
    alloc_traits::construct(m_alloc, m_data + i);
  }
}

template <class T, class Allocator>
vector<T, Allocator>::vector(
  size_type n, const_reference value, const Allocator &alloc
)
  : m_alloc(alloc), m_capacity(n), m_size(n), m_data(nullptr) {
  m_data = alloc_traits::allocate(m_alloc, m_capacity);
  for (size_type i = 0; i < m_size; ++i) {
    alloc_traits::construct(m_alloc, m_data + i, value);
  }
}

template <class T, class Allocator>
template <class InputIter>
vector<T, Allocator>::vector(
  InputIter first, InputIter last, const Allocator &alloc
)
  : m_alloc(alloc) {
  auto n = last - first;
  m_capacity = n;
  m_size = n;
  m_data = alloc_traits::allocate(m_alloc, m_capacity);
  for (size_type i = 0; i < m_size; ++i) {
    alloc_traits::construct(m_alloc, m_data + i, *first++);
  }
}

template <class T, class Allocator>
vector<T, Allocator>::vector(const vector &other)
  : m_alloc(
      tinystl::allocator_traits<
        Allocator>::select_on_container_copy_construction(other.m_alloc)
    ),
    m_capacity(other.m_size), m_size(other.m_size), m_data(nullptr) {
  m_data = alloc_traits::allocate(m_alloc, m_capacity);
  for (size_type i = 0; i < m_size; ++i) {
    alloc_traits::construct(m_alloc, m_data + i, other.m_data[i]);
  }
}

template <class T, class Allocator>
vector<T, Allocator>::vector(vector &&other) noexcept
  : m_alloc(std::move(other.m_alloc)), m_capacity(other.m_size),
    m_size(other.m_size), m_data(other.m_data) {
  other.m_capacity = 0;
  other.m_size = 0;
  other.m_data = nullptr;
}

template <class T, class Allocator>
vector<T, Allocator>::vector(const vector &other, const Allocator &alloc)
  : m_alloc(alloc), m_capacity(other.m_size), m_size(other.m_size),
    m_data(nullptr) {
  m_data = alloc_traits::allocate(m_alloc, m_capacity);
  for (size_type i = 0; i < m_size; ++i) {
    alloc_traits::construct(m_alloc, m_data + i, other.m_data[i]);
  }
}

template <class T, class Allocator>
vector<T, Allocator>::vector(vector &&other, const Allocator &alloc)
  : m_alloc(
      tinystl::allocator_traits<
        Allocator>::select_on_container_copy_construction(alloc)
    ),
    m_capacity(other.m_size), m_size(other.m_size), m_data(other.m_data) {
  other.m_capacity = 0;
  other.m_size = 0;
  other.m_data = nullptr;
}

template <class T, class Allocator>
vector<T, Allocator>::vector(
  std::initializer_list<T> init, const Allocator &alloc
)
  : vector(init.begin(), init.end(), alloc) {}

template <class T, class Allocator>
vector<T, Allocator>::~vector() {
  for (size_t i = 0; i < m_size; ++i) {
    alloc_traits(m_alloc).destroy(m_data + i);
  }
  alloc_traits::deallocate(m_alloc, m_data, m_capacity);
}

template <class T, class Allocator>
vector<T, Allocator> &vector<T, Allocator>::operator=(const vector &other) {
  if (addressof(other) == this) {
    return *this;
  }

  if (tinystl::allocator_traits<
        Allocator>::propagate_on_container_copy_assignment::value &&
      m_alloc != other.m_alloc) {
    clear();
    alloc_traits::deallocate(m_alloc, m_data, m_capacity);
    m_alloc = other.m_alloc;
    m_capacity = other.m_size;
    m_size = other.m_size;
    m_data = alloc_traits::allocate(m_alloc, m_capacity);
    for (size_type i = 0; i < m_size; ++i) {
      alloc_traits::construct(m_alloc, m_data + i, other.m_data[i]);
    }

    return *this;
  }

  if (m_capacity < other.m_size) {
    clear();
    alloc_traits::deallocate(m_alloc, m_data, m_capacity);
    m_capacity = other.m_size;
    m_size = other.m_size;
    m_data = alloc_traits::allocate(m_alloc, m_capacity);
    for (size_type i = 0; i < m_size; ++i) {
      alloc_traits::construct(m_alloc, m_data + i, other.m_data[i]);
    }
  } else {
    std::copy(other.m_data, other.m_data + other.m_size, m_data);
    m_size = other.m_size;
    if (m_size > other.m_size) {
      for (size_type i = other.m_size; i < m_size; ++i) {
        alloc_traits::destroy(m_alloc, m_data + i);
      }
    } else {
      for (size_type i = m_size; i < other.m_size; ++i) {
        alloc_traits::construct(m_alloc, m_data + i, other.m_data[i]);
      }
    }
  }

  return *this;
}

template <class T, class Allocator>
vector<T, Allocator> &vector<T, Allocator>::operator=(vector &&other) {
  if (addressof(other) == this) {
    return *this;
  }

  if (tinystl::allocator_traits<
        Allocator>::propagate_on_container_move_assignment::value) {
    clear();
    alloc_traits::deallocate(m_alloc, m_data, m_capacity);
    m_alloc = std::move(other.m_alloc);
    m_capacity = other.m_size;
    m_size = other.m_size;
    m_data = other.m_data;
    other.m_capacity = 0;
    other.m_size = 0;
    other.m_data = nullptr;
    return *this;
  } else if (m_alloc == other.m_alloc) {
    clear();
    alloc_traits::deallocate(m_alloc, m_data, m_capacity);
    m_capacity = other.m_size;
    m_size = other.m_size;
    m_data = other.m_data;
    other.m_capacity = 0;
    other.m_size = 0;
    other.m_data = nullptr;
    return *this;
  } else {
    clear();
    alloc_traits::deallocate(m_alloc, m_data, m_capacity);
    m_alloc = std::move(other.m_alloc);
    m_capacity = other.m_size;
    m_size = other.m_size;
    m_data = other.m_data;
    other.m_capacity = 0;
    other.m_size = 0;
    other.m_data = nullptr;
    return *this;
  }
}

template <class T, class Allocator>
vector<T, Allocator> &
vector<T, Allocator>::operator=(std::initializer_list<value_type> init) {
  assign(init);
  return *this;
}

template <class T, class Allocator>
void vector<T, Allocator>::assign(size_type n, const_reference value) {
  if (n < m_capacity) {
    size_type sz = this->size();
    std::fill_n(m_begin, std::min(n, sz), value);
    
    if (n > sz) {
      this->construct_at_end(n - sz, value);
    } else {
      this->destroy_at_end(m_begin + n);
    }
  } else {
    this->deallocate();
    this->allocate(this->recommend(n));
    this->construct_at_end(n, value);
  }
}

template <class T, class Allocator>
template <class InputIt>
void vector<T, Allocator>::assign(InputIt first, InputIt last) {}

template <class T, class Allocator>
void vector<T, Allocator>::assign(std::initializer_list<T> ilist) {
  assign(ilist.begin(), ilist.end());
}

// iterator
template <class T, class Allocator>
typename vector<T, Allocator>::iterator vector<T, Allocator>::begin() noexcept {
  return m_data;
}

template <class T, class Allocator>
typename vector<T, Allocator>::const_iterator
vector<T, Allocator>::begin() const noexcept {
  return m_data;
}

template <class T, class Allocator>
typename vector<T, Allocator>::const_iterator
vector<T, Allocator>::cbegin() const noexcept {
  return m_data;
}

template <class T, class Allocator>
typename vector<T, Allocator>::iterator vector<T, Allocator>::end() noexcept {
  return m_data + m_size;
}

template <class T, class Allocator>
typename vector<T, Allocator>::const_iterator
vector<T, Allocator>::end() const noexcept {
  return m_data + m_size;
}

template <class T, class Allocator>
typename vector<T, Allocator>::const_iterator
vector<T, Allocator>::cend() const noexcept {
  return m_data + m_size;
}

template <class T, class Allocator>
typename vector<T, Allocator>::reverse_iterator
vector<T, Allocator>::rbegin() noexcept {
  return reverse_iterator(end());
}

template <class T, class Allocator>
typename vector<T, Allocator>::const_reverse_iterator
vector<T, Allocator>::rbegin() const noexcept {
  return const_reverse_iterator(end());
}

template <class T, class Allocator>
typename vector<T, Allocator>::const_reverse_iterator
vector<T, Allocator>::crbegin() const noexcept {
  return const_reverse_iterator(end());
}

template <class T, class Allocator>
typename vector<T, Allocator>::reverse_iterator
vector<T, Allocator>::rend() noexcept {
  return reverse_iterator(begin());
}

template <class T, class Allocator>
typename vector<T, Allocator>::const_reverse_iterator
vector<T, Allocator>::rend() const noexcept {
  return const_reverse_iterator(begin());
}

template <class T, class Allocator>
typename vector<T, Allocator>::const_reverse_iterator
vector<T, Allocator>::crend() const noexcept {
  return const_reverse_iterator(begin());
}

// capacity
template <class T, class Allocator>
bool vector<T, Allocator>::empty() const noexcept {
  return m_size == 0;
}

template <class T, class Allocator>
typename vector<T, Allocator>::size_type
vector<T, Allocator>::size() const noexcept {
  return m_size;
}

template <class T, class Allocator>
typename vector<T, Allocator>::size_type
vector<T, Allocator>::max_size() const noexcept {
  return alloc_traits::max_size(m_alloc);
}

template <class T, class Allocator>
void vector<T, Allocator>::reserve(size_type new_cap) {
  if (new_cap > max_size()) {
    throw std::length_error("vector::reserve");
  }

  if (new_cap < m_capacity) {
    return;
  }

  pointer new_data = alloc_traits::allocate(m_alloc, new_cap);
  for (size_type i = 0; i < m_size; ++i) {
    alloc_traits::construct(m_alloc, new_data + i, m_data[i]);
    alloc_traits::destroy(m_alloc, m_data + i);
  }
}

template <class T, class Allocator>
typename vector<T, Allocator>::size_type
vector<T, Allocator>::capacity() const noexcept {
  return m_capacity;
}

template <class T, class Allocator>
vector<T, Allocator>::ConstructTransaction::ConstructTransaction(
  vector &v, size_type n
)
  : v(v), pos(v.m_end), new_end(v.m_end + n) {}

template <class T, class Allocator>
vector<T, Allocator>::ConstructTransaction::~ConstructTransaction() {
  v.m_end = pos;
}

template <class T, class Allocator>
vector<T, Allocator>::destroy_vector::destroy_vector(vector &vec)
  : m_vec(vec) {}

template <class T, class Allocator>
void vector<T, Allocator>::destroy_vector::operator()() {
  if (m_vec.m_begin != nullptr) {
    m_vec.clear();
    alloc_traits::deallocate(m_vec.m_alloc, m_vec.m_begin, m_vec.capacity());
  }
}

template <class T, class Allocator>
void vector<T, Allocator>::allocate(size_type n) {
  if (n > this->max_size()) {
    this->throw_length_error();
  }

  auto allocation = allocate_at_least(m_alloc, n);
  m_begin = allocation.ptr;
  m_end = m_begin;
  m_cap = m_begin + allocation.count;
}

template <class T, class Allocator>
void vector<T, Allocator>::deallocate() {
  if (m_begin != nullptr) {
    this->clear();
    alloc_traits::deallocate(m_alloc, m_begin, this->capacity());

    m_begin = nullptr;
    m_end = nullptr;
    m_cap = nullptr;
  }
}

template <class T, class Allocator>
typename vector<T, Allocator>::size_type
vector<T, Allocator>::recommend(size_type new_size) const {
  const auto max_size = this->max_size();
  if (new_size > max_size) {
    this->throw_length_error();
  }
  const auto cap = this->capacity();
  if (cap >= max_size / 2) {
    return max_size;
  }

  return std::max<size_type>(2 * cap, new_size);
}

template <class T, class Allocator>
void vector<T, Allocator>::construct_at_end(size_type n) {
  ConstructTransaction ct(*this, n);
  const_pointer new_end = ct.new_end;
  for (pointer pos = ct.pos; pos != new_end; ct.pos = ++pos) {
    alloc_traits::construct(m_alloc, to_address(pos));
  }
}

template <class T, class Allocator>
void vector<T, Allocator>::construct_at_end(
  size_type n, const_reference value
) {
  ConstructTransaction ct(*this, n);
  const_pointer new_end = ct.new_end;
  for (pointer pos = ct.pos; pos != new_end; ct.pos = ++pos) {
    alloc_traits::construct(m_alloc, to_address(pos), value);
  }
}

template <class T, class Allocator>
template <class InputIterator, class Sential>
void vector<T, Allocator>::construct_at_end(
  InputIterator first, Sential last, size_type n
) {
  // TODO
  ConstructTransaction ct(*this, n);
}

template <class T, class Allocator>
void vector<T, Allocator>::destroy_at_end(pointer new_last) noexcept {
  size_type old_size = this->size();
  base_destroy_at_end(new_last);
}

template <class T, class Allocator>
void vector<T, Allocator>::base_destroy_at_end(pointer new_last) noexcept {
  pointer soon_tod_be_end = m_end;
  while (new_last != soon_tod_be_end) {
    alloc_traits::destroy(m_alloc, to_address(--soon_tod_be_end));
  }
  m_end = new_last;
}

template <class T, class Allocator>
template <class InputIterator, class Sential>
void vector<T, Allocator>::init_with_size(
  InputIterator first, Sential last, size_type n
) {
  auto guard = make_exception_guard(destroy_vector(*this));

  if (n > 0) {
    this->allocate(n);
    this->construct_at_end(std::move(first), std::move(last), n);
  }

  guard.complete();
}

template <class T, class Allocator>
template <class InputIterator, class Sential>
void vector<T, Allocator>::init_with_sential(
  InputIterator first, Sential last
) {
  auto guard = make_exception_guard(destroy_vector(*this));

  for (; first != last; ++first) {
    emplace_back(*first);
  }

  guard.complete();
}
} // namespace tinystl
