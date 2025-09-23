#pragma once

#include "tinystl/type_traits/integral_constant.h"

namespace tinystl {

template <class T>
struct is_enum : tinystl::bool_constant<__is_enum(T)> {};

template <class T>
inline constexpr bool is_enum_v = __is_enum(T);

} // namespace tinystl
