#pragma once

#include "tinystl/type_traits/helper_classes/integral_constant.h"

namespace tinystl {

template <class T>
struct is_union : bool_constant<__is_union(T)> {};

template <class T>
inline constexpr bool is_union_v = is_union<T>::value;

} // namespace tinystl