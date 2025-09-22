#pragma once

#include <limits>
#include <utility>


#include "tinystl/memory/construct.h"
#include "tinystl/memory/destroy.h"
#include "tinystl/memory/pointer_traits.h"
#include "tinystl/type_traits/enable_if.h"
#include "tinystl/type_traits/is_empty.h"
#include "tinystl/type_traits/make_unsigned.h"
#include "tinystl/type_traits/void_t.h"


// https://en.cppreference.com/w/cpp/memory/allocator_traits.html

namespace detail {
// alloc_get_pointer
template <class Alloc, class = void>
struct alloc_get_pointer {
  using type = typename Alloc::value_type *;
};
template <class Alloc>
struct alloc_get_pointer<Alloc, tinystl::void_t<typename Alloc::pointer>> {
  using type = typename Alloc::pointer;
};

// alloc_get_const_pointer
template <class Alloc, class = void>
struct alloc_get_const_pointer {
  using pointer_type = typename alloc_get_pointer<Alloc>::type;
  using value_type = typename Alloc::value_type;
  using type = typename tinystl::pointer_traits<pointer_type>::template rebind<
    const value_type>;
};
template <class Alloc>
struct alloc_get_const_pointer<
  Alloc, tinystl::void_t<typename Alloc::const_pointer>> {
  using type = typename Alloc::const_pointer;
};

// alloc_get_void_pointer
template <class Alloc, class = void>
struct alloc_get_void_pointer {
  using pointer_type = typename alloc_get_pointer<Alloc>::type;
  using type =
    typename tinystl::pointer_traits<pointer_type>::template rebind<void>;
};
template <class Alloc>
struct alloc_get_void_pointer<Alloc, tinystl::void_t<typename Alloc::void_pointer>> {
  using type = typename Alloc::void_pointer;
};

// alloc_get_const_void_pointer
template <class Alloc, class = void>
struct alloc_get_const_void_pointer {
  using pointer_type = typename alloc_get_pointer<Alloc>::type;
  using type =
    typename tinystl::pointer_traits<pointer_type>::template rebind<const void>;
};

template <class Alloc>
struct alloc_get_const_void_pointer<
  Alloc, tinystl::void_t<typename Alloc::const_void_pointer>> {
  using type = typename Alloc::const_void_pointer;
};

// alloc_get_difference_type
template <class Alloc, class = void>
struct alloc_get_difference_type {
  using _Ptrty = typename alloc_get_pointer<Alloc>::type;
  using type = typename tinystl::pointer_traits<_Ptrty>::difference_type;
};

template <class Alloc>
struct alloc_get_difference_type<
  Alloc, tinystl::void_t<typename Alloc::difference_type>> {
  using type = typename Alloc::difference_type;
};

// alloc_get_size_type
template <class Alloc, class = void>
struct alloc_get_size_type {
  using type =
    tinystl::make_unsigned_t<typename alloc_get_difference_type<Alloc>::type>;
};

template <class Alloc>
struct alloc_get_size_type<Alloc, tinystl::void_t<typename Alloc::size_type>> {
  using type = typename Alloc::size_type;
};

// propagate_on_container_copy_assignment
template <class Alloc, class = void>
struct alloc_get_propagate_on_container_copy_assignment {
  using type = tinystl::false_type;
};

template <class Alloc>
struct alloc_get_propagate_on_container_copy_assignment<
  Alloc,
  tinystl::void_t<typename Alloc::propagate_on_container_copy_assignment>> {
  using type = typename Alloc::propagate_on_container_copy_assignment;
};

// propagate_on_container_move_assignment
template <class Alloc, class = void>
struct alloc_get_propagate_on_container_move_assignment {
  using type = tinystl::false_type;
};

template <class Alloc>
struct alloc_get_propagate_on_container_move_assignment<
  Alloc,
  tinystl::void_t<typename Alloc::propagate_on_container_move_assignment>> {
  using type = typename Alloc::propagate_on_container_move_assignment;
};

// propagate_on_container_swap
template <class Alloc, class = void>
struct alloc_get_propagate_on_container_swap {
  using type = tinystl::false_type;
};

template <class Alloc>
struct alloc_get_propagate_on_container_swap<
  Alloc, tinystl::void_t<typename Alloc::propagate_on_container_swap>> {
  using type = typename Alloc::propagate_on_container_swap;
};

// is_always_equal
template <class Alloc, class = void>
struct alloc_get_is_always_equal {
  using type = tinystl::is_empty<Alloc>;
};

template <class Alloc>
struct alloc_get_is_always_equal<
  Alloc, tinystl::void_t<typename Alloc::is_always_equal>> {
  using type = typename Alloc::is_always_equal;
};

// rebind_alloc
template <class Alloc, class Newfirst>
struct alloc_get_rebind_alloc_impl;

template <
  template <class, class...> class Alloc, class First, class... Rest,
  class Newfirst>
struct alloc_get_rebind_alloc_impl<Alloc<First, Rest...>, Newfirst> {
  // given Alloc<First, Rest...>, replace First with Newfirst
  using type = Alloc<Newfirst, Rest...>;
};

template <class Alloc, class NewAlloc, class = void>
struct alloc_get_rebind_alloc {
  using type = typename alloc_get_rebind_alloc_impl<Alloc, NewAlloc>::type;
};

template <class Alloc, class NewAlloc>
struct alloc_get_rebind_alloc<
  Alloc, NewAlloc,
  tinystl::void_t<typename Alloc::template rebind<NewAlloc>::other>> {
  using type = typename Alloc::template rebind<NewAlloc>::other;
};

// alloc_has_construct
template <class, class Alloc, class... Args>
const bool alloc_has_construct_impl = false;

template <class Alloc, class... Args>
const bool alloc_has_construct_impl<
  decltype((void)std::declval<Alloc>().construct(std::declval<Args>()...)),
  Alloc, Args...> = true;

template <class Alloc, class... Args>
const bool alloc_has_construct = alloc_has_construct_impl<void, Alloc, Args...>;

// alloc_has_destroy
template <class Alloc, class Pointer, class = void>
const bool alloc_has_destroy = false;

template <class Alloc, class Pointer>
const bool alloc_has_destroy<
  Alloc, Pointer,
  decltype((void)std::declval<Alloc>().destroy(std::declval<Pointer>()))> =
  true;

// alloc_has_max_size
template <class Alloc, class = void>
const bool alloc_has_max_size = false;

template <class Alloc>
const bool
  alloc_has_max_size<Alloc, decltype((void)std::declval<Alloc &>().max_size())> =
    true;

// alloc_has_select_on_container_copy_construction
template <class Alloc, class = void>
const bool alloc_has_select_on_container_copy_construction = false;

template <class Alloc>
const bool alloc_has_select_on_container_copy_construction<
  Alloc, decltype((void)std::declval<Alloc>()
                    .select_on_container_copy_construction())> = true;

} // namespace detail

namespace tinystl {

template <class Alloc>
struct allocator_traits {
  // Member types
  using allocator_type = Alloc;
  using value_type = typename Alloc::value_type;

  using pointer = typename detail::alloc_get_pointer<Alloc>::type;
  using const_pointer = typename detail::alloc_get_const_pointer<Alloc>::type;
  using void_pointer = typename detail::alloc_get_void_pointer<Alloc>::type;
  using const_void_pointer =
    typename detail::alloc_get_const_void_pointer<Alloc>::type;

  using size_type = typename detail::alloc_get_size_type<Alloc>::type;
  using difference_type = typename detail::alloc_get_difference_type<Alloc>::type;

  using propagate_on_container_copy_assignment =
    typename detail::alloc_get_propagate_on_container_copy_assignment<Alloc>::type;
  using propagate_on_container_move_assignment =
    typename detail::alloc_get_propagate_on_container_move_assignment<Alloc>::type;
  using propagate_on_container_swap =
    typename detail::alloc_get_propagate_on_container_swap<Alloc>::type;

  using is_always_equal = typename detail::alloc_get_is_always_equal<Alloc>::type;

  // Member alias templates
  template <class NewAlloc>
  using rebind_alloc = typename detail::alloc_get_rebind_alloc<Alloc, NewAlloc>::type;

  template <class NewAlloc>
  using rebind_traits = allocator_traits<rebind_alloc<NewAlloc>>;

  // Member functions
  static pointer allocate(Alloc &a, size_type n) { return a.allocate(n); }

  static pointer allocate(Alloc &a, size_type n, const_void_pointer hint) {
    return a.allocate(n, hint);
  }

  static void deallocate(Alloc &a, pointer p, size_type n) {
    a.deallocate(p, n);
  }

  template <
    class T, class... Args,
    enable_if_t<detail::alloc_has_construct<Alloc, T *, Args...>, int> = 0>
  static void construct(Alloc &a, T *p, Args &&...args) {
    a.construct(p, std::forward<Args>(args)...);
  }

  template <
    class T, class... Args,
    enable_if_t<!detail::alloc_has_construct<Alloc, T *, Args...>, int> = 0>
  static void construct(Alloc &, T *p, Args &&...args) {
    construct_at(p, std::forward<Args>(args)...);
  }

  template <class T, enable_if_t<detail::alloc_has_destroy<Alloc, T *>, int> = 0>
  static void destroy(Alloc &a, T *p) {
    a.destroy(p);
  }

  template <class T, enable_if_t<!detail::alloc_has_destroy<Alloc, T *>, int> = 0>
  static void destroy(Alloc &, T *p) {
    destroy_at(p);
  }

  template <class U = Alloc, enable_if_t<detail::alloc_has_max_size<U>, int> = 0>
  static size_type max_size(const Alloc &a) noexcept {
    return a.max_size();
  }

  template <class U = Alloc, enable_if_t<!detail::alloc_has_max_size<U>, int> = 0>
  static size_type max_size(const Alloc &) noexcept {
    return std::numeric_limits<size_type>::max() / sizeof(value_type);
  }

  template <
    class U = Alloc,
    enable_if_t<detail::alloc_has_select_on_container_copy_construction<U>, int> = 0>
  static Alloc select_on_container_copy_construction(const Alloc &a) {
    return a.select_on_container_copy_construction();
  }

  template <
    class U = Alloc,
    enable_if_t<!detail::alloc_has_select_on_container_copy_construction<U>, int> = 0>
  static Alloc select_on_container_copy_construction(const Alloc &a) {
    return a;
  }
};

} // namespace tinystl
