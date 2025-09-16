#pragma once

#include "tinystl/type_traits/helper_classes/integral_constant.h"
#include "tinystl/type_traits/primary_type_categories/is_union.h"

namespace detail {
template <class T>
tinystl::integral_constant<bool, !tinystl::is_union<T>::value> test(int T::*);

template <class> tinystl::false_type test(...);
} // namespace detail

namespace tinystl {
template <class T> struct is_class : decltype(detail::test<T>(nullptr)) {};
} // namespace tinystl