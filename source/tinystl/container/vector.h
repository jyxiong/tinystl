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
  explicit vector(std::size_t n, const Alloc &alloc = Alloc());
  vector(std::size_t n, const T &val, const Alloc &alloc = Alloc());
  template <std::forward_iterator ForwardIter>
    requires std::constructible_from<T, std::iter_reference_t<ForwardIter>>
  vector(ForwardIter first, ForwardIter last, const Alloc &alloc = Alloc());
  template <std::input_iterator InputIter>
    requires std::constructible_from<T, std::iter_reference_t<InputIter>> &&
             (!std::forward_iterator<InputIter>) &&
             (std::movable<T> || std::copyable<T>)
  vector(InputIter first, InputIter last, const Alloc &alloc = Alloc());
  vector(std::initializer_list<T> init, const Alloc &alloc = Alloc());
  vector(const vector &other);
  vector(const vector &other, const Alloc &alloc);
  vector(vector &&other) noexcept;
  vector(vector &&other, const Alloc &alloc);

  ~vector();

  vector &operator=(const vector &other);
  vector &operator=(vector &&other);
  vector &operator=(std::initializer_list<value_type> init);

  void assign(std::size_t n, const T &val);
  template <std::forward_iterator ForwardIter>
    requires std::constructible_from<T, std::iter_reference_t<ForwardIter>>
  void assign(ForwardIter first, ForwardIter last);
  template <std::input_iterator InputIter>
    requires std::constructible_from<T, std::iter_reference_t<InputIter>> &&
             (!std::forward_iterator<InputIter>)
  void assign(InputIter first, InputIter last);
  void assign(std::initializer_list<T> ilist);

  allocator_type get_allocator() const;

  // element access
  reference at(std::size_t pos);
  const T &at(std::size_t pos) const;
  reference operator[](std::size_t pos);
  const T &operator[](std::size_t pos) const;
  reference front();
  const T &front() const;
  reference back();
  const T &back() const;
  T *data() noexcept;
  const T *data() const noexcept;

  // iterators
  T *begin() noexcept;
  T *begin() const noexcept;
  T *cbegin() const noexcept;
  T *end() noexcept;
  T *end() const noexcept;
  T *cend() const noexcept;
  reverse_iterator rbegin() noexcept;
  const_reverse_iterator rbegin() const noexcept;
  const_reverse_iterator crbegin() const noexcept;
  reverse_iterator rend() noexcept;
  const_reverse_iterator rend() const noexcept;
  const_reverse_iterator crend() const noexcept;

  // capacity
  bool empty() const noexcept;
  std::size_t size() const noexcept;
  std::size_t max_size() const noexcept;
  void reserve(std::size_t new_cap);
  std::size_t capacity() const noexcept;
  void shrink_to_fit();

  // modifiers
  void clear() noexcept;
  T *insert(T *pos, const T &val);
  T *insert(T *pos, T &&val);
  T *insert(T *pos, std::size_t n, const T &val);
  template <class InputIter>
  T *insert(T *position, InputIter first, InputIter last);
  template <class... Args>
  T *emplace(T *pos, Args &&...args);
  T *erase(T *pos);
  T *erase(T *first, T *last);
  void push_back(const T &val);
  void push_back(T &&val);
  template <class... Args>
  reference emplace_back(Args &&...args);
  void pop_back();
  void resize(std::size_t sz);
  void resize(std::size_t sz, const value_type &val);
  void swap(vector &other) noexcept(
    alloc_traits::propagate_on_container_swap::val ||
    alloc_traits::is_always_equal::val
  );

private:
  void throw_length_error();

  size_type recommend(size_type new_size);

  void allocate(std::size_t n);

  void deallocate();

  void construct(std::size_t n);
  void construct(std::size_t n, const T &val);
  template <class InputIter, class Sentinel>
  void construct(InputIter first, Sentinel last, std::size_t n);

  void destruct(pointer new_last);

private:
  Alloc m_alloc;
  T *m_begin;
  T *m_end;
  T *m_cap;
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
vector<T, Alloc>::vector(std::size_t n, const Alloc &alloc) : m_alloc(alloc) {
  if (n > 0) {
    this->allocate(n);
    this->construct(n);
  } else {
    m_begin = m_end = m_cap = nullptr;
  }
}

template <class T, class Alloc>
vector<T, Alloc>::vector(std::size_t n, const T &val, const Alloc &alloc)
  : m_alloc(alloc) {
  if (n > 0) {
    this->allocate(n);
    this->construct(n, val);
  } else {
    m_begin = m_end = m_cap = nullptr;
  }
}

template <class T, class Alloc>
template <std::forward_iterator ForwardIter>
  requires std::constructible_from<T, std::iter_reference_t<ForwardIter>>
vector<T, Alloc>::vector(
  ForwardIter first, ForwardIter last, const Alloc &alloc
)
  : m_alloc(alloc) {
  std::size_t n = std::distance(first, last);
  if (n > 0) {
    this->allocate(n);
    this->construct(first, last, n);
  } else {
    m_begin = m_end = m_cap = nullptr;
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
vector<T, Alloc>::vector(std::initializer_list<T> init, const Alloc &alloc)
  : m_alloc(alloc) {
  std::size_t n = init.size();
  if (n > 0) {
    this->allocate(n);
    this->construct(init.begin(), init.end(), n);
  } else {
    m_begin = m_end = m_cap = nullptr;
  }
}

/* ----------------------------- copy construct ----------------------------- */
template <class T, class Alloc>
vector<T, Alloc>::vector(const vector &other)
  : m_alloc(
      alloc_traits::select_on_container_copy_construction(other.m_alloc)
    ) {
  std::size_t n = other.size();
  if (n > 0) {
    this->allocate(n);
    this->construct(other.begin(), other.end(), n);
  } else {
    m_end = m_cap = m_begin = nullptr;
  }
}

template <class T, class Alloc>
vector<T, Alloc>::vector(const vector &other, const Alloc &alloc)
  : m_alloc(alloc) {
  std::size_t n = other.size();
  if (n > 0) {
    this->allocate(n);
    this->construct(other.begin(), other.end(), n);
  } else {
    m_begin = m_end = m_cap = nullptr;
  }
}

/* ----------------------------- move construct ----------------------------- */
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
    std::size_t n = other.size();
    if (n > 0) {
      this->allocate(n);
      this->construct(
        std::make_move_iterator(other.begin()),
        std::make_move_iterator(other.end()), n
      );
    } else {
      m_begin = m_end = m_cap = nullptr;
    }
  }
}

template <class T, class Alloc>
vector<T, Alloc>::~vector() {}

template <class T, class Alloc>
vector<T, Alloc> &vector<T, Alloc>::operator=(const vector &other) {}

template <class T, class Alloc>
vector<T, Alloc> &vector<T, Alloc>::operator=(vector &&other) {}

template <class T, class Alloc>
vector<T, Alloc> &
vector<T, Alloc>::operator=(std::initializer_list<value_type> init) {}

template <class T, class Alloc>
void vector<T, Alloc>::assign(std::size_t n, const T &val) {
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
  size_type n = std::distance(first, last);
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
void vector<T, Alloc>::assign(std::initializer_list<T> init) {
  assign(init.begin(), init.end());
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
void vector<T, Alloc>::allocate(std::size_t n) {
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
void vector<T, Alloc>::construct(std::size_t n) {
  m_end = m_begin;
  for (std::size_t i = 0; i < n; ++i) {
    alloc_traits::construct(m_alloc, m_end);
    ++m_end;
  }
}

template <class T, class Alloc>
void vector<T, Alloc>::construct(std::size_t n, const T &val) {
  m_end = m_begin;
  for (std::size_t i = 0; i < n; ++i) {
    alloc_traits::construct(m_alloc, m_end, val);
    ++m_end;
  }
}

template <class T, class Alloc>
template <class InputIter, class Sentinel>
void vector<T, Alloc>::construct(InputIter first, Sentinel last, std::size_t n) {
  // FIXME: check last and n
  m_end = m_begin;
  for (std::size_t i = 0; i < n; ++i) {
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
} // namespace tinystl
