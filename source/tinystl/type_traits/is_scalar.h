#pragma once

#include "tinystl/type_traits/integral_constant.h"
#include "tinystl/type_traits/is_arithmetic.h"
#include "tinystl/type_traits/is_enum.h"
#include "tinystl/type_traits/is_member_pointer.h"
#include "tinystl/type_traits/is_null_pointer.h"
#include "tinystl/type_traits/is_pointer.h"

namespace tinystl {
// https://en.cppreference.com/w/cpp/types/is_scalar
template <class T>
struct is_scalar : bool_constant<
                     is_arithmetic_v<T> || is_enum_v<T> || is_pointer_v<T> ||
                     is_member_pointer_v<T> || is_null_pointer_v<T>> {};

template <class T>
inline constexpr bool is_scalar_v = is_scalar<T>::value;

} // namespace tinystl
