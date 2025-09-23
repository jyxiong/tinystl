#pragma once

#include "tinystl/type_traits/integral_constant.h"
#include "tinystl/type_traits/is_same.h"
#include "tinystl/type_traits/remove_cv.h"

namespace tinystl {
template <class T>
struct is_floating_point
  : bool_constant<
      // Note: standard floating-point types
      is_same_v<float, remove_cv_t<T>> ||
      is_same_v<double, remove_cv_t<T>> ||
      is_same_v<long double, remove_cv_t<T>>> {};

template <class T>
inline constexpr bool is_floating_point_v = is_floating_point<T>::value;
} // namespace tinystl
