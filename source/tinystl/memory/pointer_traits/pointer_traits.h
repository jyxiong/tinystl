#pragma once

#include <cstddef>

#include "tinystl/memory/miscellaneous/addressof.h"
#include "tinystl/type_traits/miscellaneous_transformations/conditional.h"
#include "tinystl/type_traits/miscellaneous_transformations/void_t.h"
#include "tinystl/type_traits/primary_type_categories/is_void.h"

namespace detail {

// pointer traits element type
template <class Ptr>
struct pointer_traits_element_type_impl {};

template <template <class, class...> class Sp, class Tp, class... Args>
struct pointer_traits_element_type_impl<Sp<Tp, Args...>> {
  using type = Tp;
};

template <class Ptr, class = void>
struct pointer_traits_element_type : pointer_traits_element_type_impl<Ptr> {};

template <class Ptr>
struct pointer_traits_element_type<
  Ptr, tinystl::void_t<typename Ptr::element_type>> {
  using type = typename Ptr::element_type;
};

// pointer traits difference type
template <class Ty, class = void>
struct pointer_traits_difference_type {
  using type = ptrdiff_t;
};

template <class Ty>
struct pointer_traits_difference_type<
  Ty, tinystl::void_t<typename Ty::difference_type>> {
  using type = typename Ty::difference_type;
};

// pointer traits rebind
template <class Tp, class Up>
struct pointer_traits_rebind_impl {
  static_assert(
    false, "Cannot rebind pointer; did you forget to add a rebind "
           "member to your pointer?"
  );
};

template <
  template <class, class...> class Sp, class Tp, class... Args, class Up>
struct pointer_traits_rebind_impl<Sp<Tp, Args...>, Up> {
  using type = Sp<Up, Args...>;
};

template <class Tp, class Up, class = void>
struct pointer_traits_rebind : pointer_traits_rebind_impl<Tp, Up> {};

template <class Tp, class Up>
struct pointer_traits_rebind<
  Tp, Up, tinystl::void_t<typename Tp::template rebind<Up>>> {
  using type = typename Tp::template rebind<Up>;
};

// pointer traits impl
template <class Ptr, class = void>
struct pointer_traits_impl {};

template <class Ptr>
struct pointer_traits_impl<
  Ptr, tinystl::void_t<typename pointer_traits_element_type<Ptr>::type>> {
  using pointer = Ptr;
  using element_type = typename pointer_traits_element_type<Ptr>::type;
  using difference_type = typename pointer_traits_difference_type<Ptr>::type;
  template <class Up>
  using rebind = typename pointer_traits_rebind<pointer, Up>::type;

  static pointer pointer_to(
    tinystl::conditional_t<tinystl::is_void_v<element_type>, char, element_type>
      &r
  ) {
    return Ptr::pointer_to(r);
  }
};
} // namespace detail

namespace tinystl {

// https://en.cppreference.com/w/cpp/memory/pointer_traits.html
template <class Ptr>
struct pointer_traits : detail::pointer_traits_impl<Ptr> {};

template <class T>
struct pointer_traits<T *> {
  // Member types
  using pointer = T *;
  using element_type = T;
  using difference_type = std::ptrdiff_t;

  // Member alias templates
  template <class U>
  using rebind = U *;

  // Member functions
  static pointer pointer_to(
    tinystl::conditional_t<tinystl::is_void_v<element_type>, char, element_type>
      &r
  ) noexcept {
    return addressof(r);
  } // constexpr since c++20
};
} // namespace tinystl
