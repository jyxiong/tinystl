#pragma once

#include <utility>

#include "tinystl/memory/pointer_traits/pointer_traits.h"
#include "tinystl/type_traits/miscellaneous_transformations/void_t.h"
#include "tinystl/type_traits/type_properties/is_empty.h"

// https://en.cppreference.com/w/cpp/memory/allocator_traits.html

namespace detail {
// alloc_pointer
template <class Alloc, class = void> struct alloc_pointer {
  using type = typename Alloc::value_type *;
};
template <class Alloc>
struct alloc_pointer<Alloc, tinystl::void_t<typename Alloc::pointer>> {
  using type = typename Alloc::pointer;
};

// alloc_const_pointer
template <class Alloc, class = void> struct alloc_const_pointer {
  using pointer_type = typename alloc_pointer<Alloc>::type;
  using value_type = typename Alloc::value_type;
  using type = typename tinystl::pointer_traits<pointer_type>::template rebind<
      const value_type>;
};
template <class Alloc>
struct alloc_const_pointer<Alloc,
                           tinystl::void_t<typename Alloc::const_pointer>> {
  using type = typename Alloc::const_pointer;
};

// alloc_void_pointer
template <class Alloc, class = void> struct alloc_void_pointer {
  using pointer_type = typename alloc_pointer<Alloc>::type;
  using type =
      typename tinystl::pointer_traits<pointer_type>::template rebind<void>;
};
template <class Alloc>
struct alloc_void_pointer<Alloc,
                          tinystl::void_t<typename Alloc::void_pointer>> {
  using type = typename Alloc::void_pointer;
};

// alloc_const_void_pointer
template <class Alloc, class = void> struct alloc_const_void_pointer {
  using pointer_type = typename alloc_pointer<Alloc>::type;
  using type = typename tinystl::pointer_traits<pointer_type>::template rebind<
      const void>;
};

template <class Alloc>
struct alloc_const_void_pointer<
    Alloc, tinystl::void_t<typename Alloc::const_void_pointer>> {
  using type = typename Alloc::const_void_pointer;
};

// propagate_on_container_copy_assignment
template <class Alloc, class = void>
struct alloc_propagate_on_container_copy_assignment {
  using type = tinystl::false_type;
};

template <class Alloc>
struct alloc_propagate_on_container_copy_assignment<
    Alloc,
    tinystl::void_t<typename Alloc::propagate_on_container_copy_assignment>> {
  using type = typename Alloc::propagate_on_container_copy_assignment;
};

// propagate_on_container_move_assignment
template <class Alloc, class = void>
struct alloc_propagate_on_container_move_assignment {
  using type = tinystl::false_type;
};

template <class Alloc>
struct alloc_propagate_on_container_move_assignment<
    Alloc,
    tinystl::void_t<typename Alloc::propagate_on_container_move_assignment>> {
  using type = typename Alloc::propagate_on_container_move_assignment;
};

// propagate_on_container_swap
template <class Alloc, class = void> struct alloc_propagate_on_container_swap {
  using type = tinystl::false_type;
};

template <class Alloc>
struct alloc_propagate_on_container_swap<
    Alloc, tinystl::void_t<typename Alloc::propagate_on_container_swap>> {
  using type = typename Alloc::propagate_on_container_swap;
};

// is_always_equal
template <class Alloc, class = void> struct alloc_is_always_equal {
  using type = tinystl::is_empty<Alloc>;
};

template <class Alloc>
struct alloc_is_always_equal<Alloc,
                             tinystl::void_t<typename Alloc::is_always_equal>> {
  using type = typename Alloc::is_always_equal;
};

// rebind_alloc
template <class Alloc, class Newfirst> struct alloc_rebind_alloc_impl;

template <template <class, class...> class Alloc, class First, class... Rest,
          class Newfirst>
struct alloc_rebind_alloc_impl<Alloc<First, Rest...>,
                               Newfirst> { // given Alloc<First, Rest...>,
                                           // replace First
  using type = Alloc<Newfirst, Rest...>;
};

template <class Alloc, class NewAlloc, class = void> struct alloc_rebind_alloc {
  using type = typename alloc_rebind_alloc_impl<Alloc, NewAlloc>::type;
};

template <class Alloc, class NewAlloc>
struct alloc_rebind_alloc<
    Alloc, NewAlloc,
    tinystl::void_t<typename Alloc::template rebind<NewAlloc>::other>> {
  using type = typename Alloc::template rebind<NewAlloc>::other;
};

} // namespace detail

namespace tinystl {

template <class Alloc> struct allocator_traits {
  // Member types
  using allocator_type = Alloc;
  using value_type = typename Alloc::value_type;

  using pointer = typename detail::alloc_pointer<Alloc>::type;
  using const_pointer = typename detail::alloc_const_pointer<Alloc>::type;
  using void_pointer = typename detail::alloc_void_pointer<Alloc>::type;
  using const_void_pointer =
      typename detail::alloc_const_void_pointer<Alloc>::type;

  using size_type = typename Alloc::size_type;
  using difference_type = typename Alloc::difference_type;

  using propagate_on_container_copy_assignment =
      typename detail::alloc_propagate_on_container_copy_assignment<
          Alloc>::type;
  using propagate_on_container_move_assignment =
      typename detail::alloc_propagate_on_container_move_assignment<
          Alloc>::type;
  using propagate_on_container_swap =
      typename detail::alloc_propagate_on_container_swap<Alloc>::type;

  using is_always_equal = typename detail::alloc_is_always_equal<Alloc>::type;

  // Member alias templates
  template <class T>
  using rebind_alloc = typename detail::alloc_rebind_alloc<Alloc, T>::type;

  template <class T> using rebind_traits = allocator_traits<rebind_alloc<T>>;

  // Member functions
  static pointer allocate(Alloc &a, size_type n) {
    return a.allocate(n);
  }

  static pointer allocate(Alloc &a, size_type n, const_void_pointer hint) {
    return a.allocate(n, hint);
  }

  static void deallocate(Alloc &a, pointer p, size_type n) {
    a.deallocate(p, n);
  }

  template <class T, class... Args>
  static void construct(Alloc &a, T *p, Args &&...args)
  {
    a.construct(p, std::forward<Args>(args)...);
  }

  template <class T> static void destroy(Alloc &a, T *p)
  {
    a.destroy(p);
  }

  static size_type max_size(const Alloc &a) noexcept
  {
    return a.max_size();
  }

  static Alloc select_on_container_copy_construction(const Alloc &a)
  {
    return a;
  }
};

} // namespace tinystl