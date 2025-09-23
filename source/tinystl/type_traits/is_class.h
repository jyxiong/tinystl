#pragma once

#include "tinystl/type_traits/integral_constant.h"
#include "tinystl/type_traits/is_union.h"

namespace detail {
template <class T>
tinystl::bool_constant<!tinystl::is_union_v<T>> test(int T::*);

template <class>
tinystl::false_type test(...);
} // namespace detail

namespace tinystl {

// template <class T>
// struct is_class : bool_constant<__is_class(T)> {};

template <class T>
struct is_class : decltype(detail::test<T>(nullptr)) {};

template <class T>
inline constexpr bool is_class_v = is_class<T>::value;

} // namespace tinystl
