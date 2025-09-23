#pragma once

#include "tinystl/type_traits/integral_constant.h"
#include "tinystl/type_traits/is_const.h"
#include "tinystl/type_traits/is_reference.h"

namespace tinystl {

// primary template
// template <class T>
// struct is_function : bool_constant<__is_function(T)> {};

template <class T>
struct is_function
  : bool_constant<!is_const_v<const T> && !is_reference_v<T>> {};

template <class T>
inline constexpr bool is_function_v = is_function<T>::value;
} // namespace tinystl