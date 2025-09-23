#pragma once

#include "tinystl/type_traits/integral_constant.h"

namespace tinystl {

template <class T>
struct is_const : false_type {};

template <class T>
struct is_const<const T> : true_type {};

template <class T>
inline constexpr bool is_const_v = is_const<T>::value;
} // namespace tinystl