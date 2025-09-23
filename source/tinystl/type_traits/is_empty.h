#pragma once

#include "tinystl/type_traits/integral_constant.h"

namespace tinystl {
template <class T>
struct is_empty : bool_constant<__is_empty(T)> {};

template <class T>
inline constexpr bool is_empty_v = __is_empty(T);
} // namespace tinystl
