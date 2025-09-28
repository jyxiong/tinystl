#pragma once

#include <concepts>
#include <cstddef>
#include <iterator>
#include <memory>
#include <stdexcept>

namespace tinystl {

template <class T, class Alloc = std::allocator<T>>
class vector {
  using alloc_traits = std::allocator_traits<Alloc>;

public:
  using value_type = T;
  using allocator_type = Alloc;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using reference = T &;
  using const_reference = const T &;
  using pointer = T *;
  using const_pointer = const T *;
  using iterator = T *;
  using const_iterator = const T *;
  using reverse_iterator = std::reverse_iterator<T *>;
  using const_reverse_iterator = std::reverse_iterator<T *>;

  // construct/copy/destroy
  vector() noexcept(noexcept(Alloc()));
  explicit vector(const Alloc &alloc) noexcept;
  explicit vector(size_type n, const Alloc &alloc = Alloc());
  vector(size_type n, const_reference val, const Alloc &alloc = Alloc());
  template <std::forward_iterator ForwardIter>
    requires std::constructible_from<T, std::iter_reference_t<ForwardIter>>
  vector(ForwardIter first, ForwardIter last, const Alloc &alloc = Alloc());
  template <std::input_iterator InputIter>
    requires std::constructible_from<T, std::iter_reference_t<InputIter>> &&
             (!std::forward_iterator<InputIter>) &&
             (std::movable<T> || std::copyable<T>)
  vector(InputIter first, InputIter last, const Alloc &alloc = Alloc());
  vector(const vector &other);
  vector(const vector &other, const Alloc &alloc);
  vector(vector &&other) noexcept;
  vector(vector &&other, const Alloc &alloc);
  vector(std::initializer_list<value_type> init, const Alloc &alloc = Alloc());

  ~vector();

  vector &operator=(const vector &other);
  vector &operator=(vector &&other);
  vector &operator=(std::initializer_list<value_type> init);

  void assign(size_type n, const_reference val);
  template <std::forward_iterator ForwardIter>
    requires std::constructible_from<T, std::iter_reference_t<ForwardIter>>
  void assign(ForwardIter first, ForwardIter last);
  template <std::input_iterator InputIter>
    requires std::constructible_from<T, std::iter_reference_t<InputIter>> &&
             (!std::forward_iterator<InputIter>)
  void assign(InputIter first, InputIter last);
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
  pointer data() noexcept;
  const_pointer data() const noexcept;

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
  void reserve(size_type n);
  size_type capacity() const noexcept;
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
  void resize(size_type sz);
  void resize(size_type sz, const_reference val);
  void swap(vector &other) noexcept(
    alloc_traits::propagate_on_container_swap::val ||
    alloc_traits::is_always_equal::val
  );

private:
  void throw_length_error();

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
  Alloc m_alloc;
  pointer m_begin = nullptr;
  pointer m_end = nullptr;
  pointer m_cap = nullptr;
};

template <class T, class Alloc>
bool operator==(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs);

template <class T, class Alloc>
auto operator<=>(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs);

template <class T, class Alloc>
void swap(vector<T, Alloc> &lhs, vector<T, Alloc> &rhs) noexcept;

template <class T, class Alloc, class U>
typename vector<T, Alloc>::size_type erase(vector<T, Alloc> &c, const U &val);

template <class T, class Alloc, class Pred>
typename vector<T, Alloc>::size_type erase_if(vector<T, Alloc> &c, Pred pred);

/* -------------------------------------------------------------------------- */
/*                           public member functions                          */
/* -------------------------------------------------------------------------- */

/* -------------------------------- construct ------------------------------- */
template <class T, class Alloc>
vector<T, Alloc>::vector() noexcept(noexcept(Alloc()))
  : m_begin(nullptr), m_end(nullptr), m_cap(nullptr) {}

template <class T, class Alloc>
vector<T, Alloc>::vector(const Alloc &alloc) noexcept : m_alloc(alloc) {}

template <class T, class Alloc>
vector<T, Alloc>::vector(size_type n, const Alloc &alloc) : m_alloc(alloc) {
  if (n > 0) {
    this->allocate(n);
    this->construct(n);
  }
}

template <class T, class Alloc>
vector<T, Alloc>::vector(size_type n, const_reference val, const Alloc &alloc)
  : m_alloc(alloc) {
  if (n > 0) {
    this->allocate(n);
    this->construct(n, val);
  }
}

template <class T, class Alloc>
template <std::forward_iterator ForwardIter>
  requires std::constructible_from<T, std::iter_reference_t<ForwardIter>>
vector<T, Alloc>::vector(
  ForwardIter first, ForwardIter last, const Alloc &alloc
)
  : m_alloc(alloc) {
  size_type n = static_cast<size_type>(std::distance(first, last));
  if (n > 0) {
    this->allocate(n);
    this->construct(first, last, n);
  }
}

template <class T, class Alloc>
template <std::input_iterator InputIter>
  requires std::constructible_from<T, std::iter_reference_t<InputIter>> &&
           (!std::forward_iterator<InputIter>) &&
           (std::movable<T> || std::copyable<T>)
vector<T, Alloc>::vector(InputIter first, InputIter last, const Alloc &alloc) {
  for (; first != last; ++first) {
    this->emplace_back(*first);
  }
}

template <class T, class Alloc>
vector<T, Alloc>::vector(const vector &other)
  : m_alloc(
      alloc_traits::select_on_container_copy_construction(other.m_alloc)
    ) {
  size_type n = other.size();
  if (n > 0) {
    this->allocate(n);
    this->construct(other.begin(), other.end(), n);
  }
}

template <class T, class Alloc>
vector<T, Alloc>::vector(const vector &other, const Alloc &alloc)
  : m_alloc(alloc) {
  size_type n = other.size();
  if (n > 0) {
    this->allocate(n);
    this->construct(other.begin(), other.end(), n);
  }
}

template <class T, class Alloc>
vector<T, Alloc>::vector(vector &&other) noexcept
  : m_alloc(std::move(other.m_alloc)) {
  m_begin = other.m_begin;
  m_end = other.m_end;
  m_cap = other.m_cap;
  other.m_begin = other.m_end = other.m_cap = nullptr;
}

template <class T, class Alloc>
vector<T, Alloc>::vector(vector &&other, const Alloc &alloc) : m_alloc(alloc) {
  if (m_alloc == other.m_alloc) {
    m_begin = other.m_begin;
    m_end = other.m_end;
    m_cap = other.m_cap;
    other.m_begin = other.m_end = other.m_cap = nullptr;
  } else {
    size_type n = other.size();
    if (n > 0) {
      this->allocate(n);
      this->construct(
        std::make_move_iterator(other.begin()),
        std::make_move_iterator(other.end()), n
      );
    }
  }
}

template <class T, class Alloc>
vector<T, Alloc>::vector(
  std::initializer_list<value_type> init, const Alloc &alloc
)
  : m_alloc(alloc) {
  size_type n = init.size();
  if (n > 0) {
    this->allocate(n);
    this->construct(init.begin(), init.end(), n);
  }
}

/* ------------------------------- destructor ------------------------------- */
template <class T, class Alloc>
vector<T, Alloc>::~vector() {
  if (m_begin != nullptr) {
    this->clear();
    this->deallocate();
  }
}

/* -------------------------------- operator= ------------------------------- */
template <class T, class Alloc>
vector<T, Alloc> &vector<T, Alloc>::operator=(const vector &other) {
  if (this != std::addressof(other)) {
    if constexpr (alloc_traits::propagate_on_container_copy_assignment::value) {
      if (m_alloc != other.m_alloc) {
        this->clear();
        this->deallocate();
      }

      m_alloc = other.m_alloc;
    }

    this->assign(other.begin(), other.end());
  }

  return *this;
}

template <class T, class Alloc>
vector<T, Alloc> &vector<T, Alloc>::operator=(vector &&other) {
  if constexpr (alloc_traits::propagate_on_container_move_assignment::value) {
    this->deallocate();
    m_alloc = std::move(other.m_alloc);
    this->m_begin = other.m_begin;
    this->m_end = other.m_end;
    this->m_cap = other.m_cap;
    other.m_begin = other.m_end = other.m_cap = nullptr;
  } else {
    if (m_alloc != other.m_alloc) {
      assign(
        std::make_move_iterator(other.begin()),
        std::make_move_iterator(other.end())
      );
    } else {
      this->deallocate();
      this->m_begin = other.m_begin;
      this->m_end = other.m_end;
      this->m_cap = other.m_cap;
      other.m_begin = other.m_end = other.m_cap = nullptr;
    }
  }

  return *this;
}

template <class T, class Alloc>
vector<T, Alloc> &
vector<T, Alloc>::operator=(std::initializer_list<value_type> init) {
  assign(init.begin(), init.end());
  return *this;
}

/* --------------------------------- assign --------------------------------- */
template <class T, class Alloc>
void vector<T, Alloc>::assign(size_type n, const_reference val) {
  if (n <= this->capacity()) {
    size_type sz = this->size();
    std::fill_n(m_begin, std::min(n, sz), val);
    if (n > sz) {
      this->construct(n - sz, val);
    } else {
      this->destruct(m_begin + n);
    }
  } else {
    this->destruct(m_begin);
    this->deallocate();
    this->allocate(this->recommend(n));
    this->construct(n, val);
  }
}

template <class T, class Alloc>
template <std::forward_iterator ForwardIter>
  requires std::constructible_from<T, std::iter_reference_t<ForwardIter>>
void vector<T, Alloc>::assign(ForwardIter first, ForwardIter last) {
  size_type n = static_cast<size_type>(std::distance(first, last));
  if (n <= this->capacity()) {
    size_type sz = this->size();
    if (n > sz) {
      ForwardIter mid = std::next(first, sz);
      std::copy(first, mid, m_begin);
      this->construct(mid, last, n - sz);
    } else {
      pointer new_last = std::copy(first, last, m_begin);
      this->destruct(new_last);
    }
  } else {
    this->destruct(m_begin);
    this->deallocate();
    this->allocate(this->recommend(n));
    this->construct(first, last, n);
  }
}

template <class T, class Alloc>
template <std::input_iterator InputIter>
  requires std::constructible_from<T, std::iter_reference_t<InputIter>> &&
           (!std::forward_iterator<InputIter>)
void vector<T, Alloc>::assign(InputIter first, InputIter last) {
  pointer cur = m_begin;
  for (; cur != m_end && first != last; ++cur, ++first) {
    *cur = *first;
  }

  if (cur != m_end) {
    this->destruct(cur);
  } else {
    for (; first != last; ++first) {
      this->emplace_back(*first);
    }
  }
}

template <class T, class Alloc>
void vector<T, Alloc>::assign(std::initializer_list<value_type> init) {
  assign(init.begin(), init.end());
}

/* ----------------------------- element access ----------------------------- */
template <class T, class Alloc>
Alloc vector<T, Alloc>::get_allocator() const {
  return m_alloc;
}

template <class T, class Alloc>
typename vector<T, Alloc>::reference vector<T, Alloc>::at(size_type pos) {
  if (pos >= this->size()) {
    this->throw_length_error();
  }
  return m_begin[pos];
}

template <class T, class Alloc>
typename vector<T, Alloc>::const_reference
vector<T, Alloc>::at(size_type pos) const {
  if (pos >= this->size()) {
    this->throw_length_error();
  }
  return m_begin[pos];
}

template <class T, class Alloc>
typename vector<T, Alloc>::reference
vector<T, Alloc>::operator[](size_type pos) {
  assert(pos < this->size(), "vector[] index out of bounds");
  return m_begin[pos];
}

template <class T, class Alloc>
typename vector<T, Alloc>::const_reference
vector<T, Alloc>::operator[](size_type pos) const {
  assert(pos < this->size(), "vector[] index out of bounds");
  return m_begin[pos];
}

template <class T, class Alloc>
typename vector<T, Alloc>::reference vector<T, Alloc>::front() {
  assert(!this->empty(), "front() called on an empty vector");
  return *m_begin;
}

template <class T, class Alloc>
typename vector<T, Alloc>::const_reference vector<T, Alloc>::front() const {
  assert(!this->empty(), "front() called on an empty vector");
  return *m_begin;
}

template <class T, class Alloc>
typename vector<T, Alloc>::reference vector<T, Alloc>::back() {
  assert(!this->empty(), "back() called on an empty vector");
  return *(m_end - 1);
}

template <class T, class Alloc>
typename vector<T, Alloc>::const_reference vector<T, Alloc>::back() const {
  assert(!this->empty(), "back() called on an empty vector");
  return *(m_end - 1);
}

template <class T, class Alloc>
typename vector<T, Alloc>::pointer vector<T, Alloc>::data() noexcept {
  return m_begin;
}

template <class T, class Alloc>
typename vector<T, Alloc>::const_pointer
vector<T, Alloc>::data() const noexcept {
  return m_begin;
}

/* -------------------------------- iterator -------------------------------- */
template <class T, class Alloc>
typename vector<T, Alloc>::iterator vector<T, Alloc>::begin() noexcept {
  return iterator(m_begin);
}

template <class T, class Alloc>
typename vector<T, Alloc>::const_iterator
vector<T, Alloc>::begin() const noexcept {
  return const_iterator(m_begin);
}

template <class T, class Alloc>
typename vector<T, Alloc>::const_iterator
vector<T, Alloc>::cbegin() const noexcept {
  return this->begin();
}

template <class T, class Alloc>
typename vector<T, Alloc>::iterator vector<T, Alloc>::end() noexcept {
  return iterator(m_end);
}

template <class T, class Alloc>
typename vector<T, Alloc>::const_iterator
vector<T, Alloc>::end() const noexcept {
  return const_iterator(m_end);
}

template <class T, class Alloc>
typename vector<T, Alloc>::const_iterator
vector<T, Alloc>::cend() const noexcept {
  return this->end();
}

template <class T, class Alloc>
typename vector<T, Alloc>::reverse_iterator
vector<T, Alloc>::rbegin() noexcept {
  return reverse_iterator(this->end());
}

template <class T, class Alloc>
typename vector<T, Alloc>::const_reverse_iterator
vector<T, Alloc>::rbegin() const noexcept {
  return const_reverse_iterator(this->end());
}

template <class T, class Alloc>
typename vector<T, Alloc>::const_reverse_iterator
vector<T, Alloc>::crbegin() const noexcept {
  return this->rbegin();
}

template <class T, class Alloc>
typename vector<T, Alloc>::reverse_iterator vector<T, Alloc>::rend() noexcept {
  return reverse_iterator(this->begin());
}

template <class T, class Alloc>
typename vector<T, Alloc>::const_reverse_iterator
vector<T, Alloc>::rend() const noexcept {
  return const_reverse_iterator(this->begin());
}

template <class T, class Alloc>
typename vector<T, Alloc>::const_reverse_iterator
vector<T, Alloc>::crend() const noexcept {
  return this->rend();
}

/* -------------------------------- capacity -------------------------------- */
template <class T, class Alloc>
bool vector<T, Alloc>::empty() const noexcept {
  return m_begin == m_end;
}

template <class T, class Alloc>
typename vector<T, Alloc>::size_type vector<T, Alloc>::size() const noexcept {
  return static_cast<size_type>(m_end - m_begin);
}

template <class T, class Alloc>
typename vector<T, Alloc>::size_type
vector<T, Alloc>::max_size() const noexcept {
  return std::min<size_type>(
    alloc_traits::max_size(m_alloc), std::numeric_limits<difference_type>::max()
  );
}

template <class T, class Alloc>
void vector<T, Alloc>::reserve(size_type n) {
  if (n > this->capacity()) {
    if (n > this->max_size()) {
      this->throw_length_error();
    }

    pointer new_begin = alloc_traits::allocate(m_alloc, n);
    pointer new_end = new_begin;
    pointer new_cap = new_begin + n;

    if (m_begin != nullptr) {
      if constexpr (std::is_nothrow_move_constructible_v<T>) {
        new_end = std::uninitialized_move(m_begin, m_end, new_begin);
      } else {
        new_end = std::uninitialized_copy(m_begin, m_end, new_begin);
      }

      this->destruct(m_begin);
      this->deallocate();
    }

    m_begin = new_begin;
    m_end = new_end;
    m_cap = new_cap;
  }
}

template <class T, class Alloc>
typename vector<T, Alloc>::size_type
vector<T, Alloc>::capacity() const noexcept {
  return static_cast<size_type>(m_cap - m_begin);
}

template <class T, class Alloc>
void vector<T, Alloc>::shrink_to_fit() {
  if (this->capacity() > this->size()) {
    size_type sz = this->size();
    if (sz == 0) {
      this->deallocate();
      return;
    }

    pointer new_begin = alloc_traits::allocate(m_alloc, sz);
    pointer new_end = new_begin;
    pointer new_cap = new_begin + sz;

    if constexpr (std::is_nothrow_move_constructible_v<T>) {
      new_end = std::uninitialized_move(m_begin, m_end, new_begin);
    } else {
      new_end = std::uninitialized_copy(m_begin, m_end, new_begin);
    }

    this->destruct(m_begin);
    this->deallocate();

    m_begin = new_begin;
    m_end = new_end;
    m_cap = new_cap;
  }
}

/* -------------------------------- modifiers ------------------------------- */
template <class T, class Alloc>
void vector<T, Alloc>::clear() noexcept {
  this->destruct(m_begin);
}

template <class T, class Alloc>
typename vector<T, Alloc>::iterator
vector<T, Alloc>::insert(const_iterator pos, const_reference val) {
  size_type offset = static_cast<size_type>(std::distance(this->begin(), pos));
  pointer p = m_begin + offset;
  if (m_end < m_cap) {
    if (p == m_end) {
      this->construct(1, val);
    } else {
      this->move_to_insert(p, m_end, p + 1);

      const_pointer pval = std::pointer_traits<pointer>::pointer_to(val);
      const_pointer pp = std::to_address(p);
      const_pointer pend = std::to_address(m_end);
      if (!(pp <= pval && pval < pend)) {
        pval += 1;
      }

      *p = val;
    }
  } else {
    size_type sz = this->recommend(this->size() + 1);
    pointer new_begin = alloc_traits::allocate(m_alloc, sz);
    pointer new_end = new_begin;
    pointer new_cap = new_begin + sz;

    if constexpr (std::is_nothrow_move_constructible_v<value_type>) {
      new_end = std::uninitialized_move(m_begin, p, new_begin);
    } else {
      new_end = std::uninitialized_copy(m_begin, p, new_begin);
    }

    alloc_traits::construct(m_alloc, new_end, val);
    ++new_end;

    if constexpr (std::is_nothrow_move_constructible_v<value_type>) {
      new_end = std::uninitialized_move(p, m_end, new_end);
    } else {
      new_end = std::uninitialized_copy(p, m_end, new_end);
    }

    this->destruct(m_begin);
    this->deallocate();

    m_begin = new_begin;
    m_end = new_end;
    m_cap = new_cap;
  }

  return iterator(p);
}

template <class T, class Alloc>
typename vector<T, Alloc>::iterator
vector<T, Alloc>::insert(const_iterator pos, value_type &&val) {
  size_type offset = static_cast<size_type>(std::distance(this->begin(), pos));
  pointer p = m_begin + offset;
  if (m_end < m_cap) {
    if (p == m_end) {
      alloc_traits::construct(m_alloc, p, std::move(val));
      ++m_end;
    } else {
      this->move_to_insert(p, m_end, p + 1);
      *p = std::move(val);
    }
  } else {
    size_type sz = this->recommend(this->size() + 1);
    pointer new_begin = alloc_traits::allocate(m_alloc, sz);
    pointer new_end = new_begin;
    pointer new_cap = new_begin + sz;

    if constexpr (std::is_nothrow_move_constructible_v<value_type>) {
      new_end = std::uninitialized_move(m_begin, p, new_begin);
    } else {
      new_end = std::uninitialized_copy(m_begin, p, new_begin);
    }

    alloc_traits::construct(m_alloc, new_end, std::move(val));
    ++new_end;

    if constexpr (std::is_nothrow_move_constructible_v<value_type>) {
      new_end = std::uninitialized_move(p, m_end, new_end);
    } else {
      new_end = std::uninitialized_copy(p, m_end, new_end);
    }

    this->destruct(m_begin);
    this->deallocate();

    m_begin = new_begin;
    m_end = new_end;
    m_cap = new_cap;
  }
  return iterator(p);
}

template <class T, class Alloc>
typename vector<T, Alloc>::iterator
vector<T, Alloc>::insert(const_iterator pos, size_type n, const_reference val) {
  size_type offset = static_cast<size_type>(std::distance(this->begin(), pos));
  pointer p = m_begin + offset;
  if (n > 0) {
    if (n <= static_cast<size_type>(m_cap - m_end)) {
      pointer old_end = m_end;
      size_type old_n = n;

      size_type dx = static_cast<size_type>(m_end - p);
      if (n > dx) {
        this->construct(n - dx, val);
        n = dx;
      }

      if (n > 0) {
        this->move_to_insert(p, old_end, p + old_n);

        const_pointer pval = std::pointer_traits<pointer>::pointer_to(val);
        const_pointer pp = std::to_address(p);
        const_pointer pend = std::to_address(m_end);
        if (!(pp <= pval && pval < pend)) {
          pval += old_n;
        }
        std::fill_n(p, n, *pval);
      }
    } else {
      size_type sz = this->recommend(this->size() + n);
      pointer new_begin = alloc_traits::allocate(m_alloc, sz);
      pointer new_end = new_begin;
      pointer new_cap = new_begin + sz;

      if constexpr (std::is_nothrow_move_constructible_v<value_type>) {
        new_end = std::uninitialized_move(m_begin, p, new_begin);
      } else {
        new_end = std::uninitialized_copy(m_begin, p, new_begin);
      }

      for (size_type i = 0; i < n; ++i) {
        alloc_traits::construct(m_alloc, new_end, val);
        ++new_end;
      }

      if constexpr (std::is_nothrow_move_constructible_v<value_type>) {
        new_end = std::uninitialized_move(p, m_end, new_end);
      } else {
        new_end = std::uninitialized_copy(p, m_end, new_end);
      }

      this->destruct(m_begin);
      this->deallocate();

      m_begin = new_begin;
      m_end = new_end;
      m_cap = new_cap;
    }
  }

  return iterator(p);
}

template <class T, class Alloc>
template <std::forward_iterator ForwardIter>
  requires std::constructible_from<T, std::iter_reference_t<ForwardIter>>
typename vector<T, Alloc>::iterator vector<T, Alloc>::insert(
  const_iterator pos, ForwardIter first, ForwardIter last
) {
  size_type offset = static_cast<size_type>(std::distance(this->begin(), pos));
  pointer p = m_begin + offset;

  difference_type n = std::distance(first, last);

  if (n > 0) {
    if (n <= static_cast<size_type>(m_cap - m_end)) {
      pointer old_end = m_end;

      size_type dx = static_cast<size_type>(m_end - p);
      if (n > dx) {
        ForwardIter m = std::next(first, dx);
        this->construct(m, last, n - dx);
        n = dx;
      }
      if (n > 0) {
        this->move_to_insert(p, old_end, p + n);
        std::copy_n(first, n, p);
      }
    } else {
      size_type sz = this->recommend(this->size() + n);
      pointer new_begin = alloc_traits::allocate(m_alloc, sz);
      pointer new_end = new_begin;
      pointer new_cap = new_begin + sz;

      if constexpr (std::is_move_constructible_v<value_type>) {
        new_end = std::uninitialized_move(m_begin, p, new_begin);
      } else {
        new_end = std::uninitialized_copy(m_begin, p, new_begin);
      }

      for (size_type i = 0; i < n; ++i) {
        alloc_traits::construct(m_alloc, new_end, *first);
        ++new_end;
        ++first;
      }

      if constexpr (std::is_move_constructible_v<value_type>) {
        new_end = std::uninitialized_move(p, m_end, new_end);
      } else {
        new_end = std::uninitialized_copy(p, m_end, new_end);
      }

      this->destruct(m_begin);
      this->deallocate();

      m_begin = new_begin;
      m_end = new_end;
      m_cap = new_cap;
    }
  }

  return iterator(p);
}

template <class T, class Alloc>
template <std::input_iterator InputIter>
  requires std::constructible_from<T, std::iter_reference_t<InputIter>> &&
           (!std::forward_iterator<InputIter>)
typename vector<T, Alloc>::iterator
vector<T, Alloc>::insert(const_iterator pos, InputIter first, InputIter last) {}

template <class T, class Alloc>
typename vector<T, Alloc>::iterator
vector<T, Alloc>::insert(const_iterator pos, std::initializer_list<T> init) {
  return insert(pos, init.begin(), init.end());
}

/* -------------------------------------------------------------------------- */
/*                          private member functions                          */
/* -------------------------------------------------------------------------- */
template <class T, class Alloc>
void vector<T, Alloc>::throw_length_error() {
  throw std::length_error("vector");
}

template <class T, class Alloc>
typename vector<T, Alloc>::size_type
vector<T, Alloc>::recommend(size_type new_size) {
  const size_type max_size = this->max_size();
  if (new_size > max_size) {
    this->throw_length_error();
  }

  const size_type cap = this->capacity();
  if (cap >= max_size / 2) {
    return max_size;
  }

  return std::max<size_type>(2 * cap, new_size);
}

template <class T, class Alloc>
void vector<T, Alloc>::allocate(size_type n) {
  if (n > this->max_size()) {
    this->throw_length_error();
  } else {
    m_begin = alloc_traits::allocate(m_alloc, n);
    m_cap = m_begin + n;
  }
}

template <class T, class Alloc>
void vector<T, Alloc>::deallocate() {
  if (m_begin != nullptr) {
    alloc_traits::deallocate(m_alloc, m_begin, capacity());
    m_begin = m_end = m_cap = nullptr;
  }
}

template <class T, class Alloc>
void vector<T, Alloc>::construct(size_type n) {
  m_end = m_begin;
  for (size_type i = 0; i < n; ++i) {
    alloc_traits::construct(m_alloc, m_end);
    ++m_end;
  }
}

template <class T, class Alloc>
void vector<T, Alloc>::construct(size_type n, const_reference val) {
  m_end = m_begin;
  for (size_type i = 0; i < n; ++i) {
    alloc_traits::construct(m_alloc, m_end, val);
    ++m_end;
  }
}

template <class T, class Alloc>
template <class InputIter, class Sentinel>
void vector<T, Alloc>::construct(InputIter first, Sentinel last, size_type n) {
  // FIXME: check last and n
  m_end = m_begin;
  for (size_type i = 0; i < n; ++i) {
    alloc_traits::construct(m_alloc, m_end, *first);
    ++m_end;
    ++first;
  }
}

template <class T, class Alloc>
void vector<T, Alloc>::destruct(pointer new_last) {
  pointer new_end = m_end;
  while (new_last != new_end) {
    alloc_traits::destroy(m_alloc, --new_end);
  }
  m_end = new_last;
}

template <class T, class Alloc>
void vector<T, Alloc>::move_to_insert(
  pointer from_s, pointer from_e, pointer to
) {
  pointer from_mid = from_s + (m_end - to);
  pointer new_end = m_end;
  for (; from_mid < from_e; ++from_mid, ++new_end) {
    alloc_traits::construct(m_alloc, new_end, std::move(*from_mid));
  }

  std::move_backward(from_s, from_mid, m_end);

  m_end = new_end;
}

} // namespace tinystl
