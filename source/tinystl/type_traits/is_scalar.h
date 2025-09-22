#pragma once

#include "tinystl/type_traits/is_arithmetic.h"
#include "tinystl/type_traits/is_member_pointer.h"
#include "tinystl/type_traits/integral_constant.h"
#include "tinystl/type_traits/is_enum.h"
#include "tinystl/type_traits/is_null_pointer.h"
#include "tinystl/type_traits/is_pointer.h"

namespace tinystl {
// https://en.cppreference.com/w/cpp/types/is_scalar
template <class T>
struct is_scalar
  : integral_constant<
      bool, is_arithmetic<T>::value || is_enum<T>::value ||
              is_pointer<T>::value || is_member_pointer<T>::value ||
              is_null_pointer<T>::value> {};

template <class T>
inline constexpr bool is_scalar_v = is_scalar<T>::value;

} // namespace tinystl
