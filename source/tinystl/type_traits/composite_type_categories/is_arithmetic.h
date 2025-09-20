#pragma once

#include "tinystl/type_traits/helper_classes/integral_constant.h"
#include "tinystl/type_traits/primary_type_categories/is_floating_point.h"
#include "tinystl/type_traits/primary_type_categories/is_integral.h"

namespace tinystl {
template <class T>
struct is_arithmetic
  : integral_constant<
      bool, is_integral<T>::value || is_floating_point<T>::value> {};

template <class T>
inline constexpr bool is_arithmetic_v = is_arithmetic<T>::value;
} // namespace tinystl