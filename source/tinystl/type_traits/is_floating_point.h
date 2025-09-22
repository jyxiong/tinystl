#pragma once

#include "tinystl/type_traits/remove_cv.h"
#include "tinystl/type_traits/integral_constant.h"
#include "tinystl/type_traits/is_same.h"

namespace tinystl {
template <class T>
struct is_floating_point
  : integral_constant<
      bool,
      // Note: standard floating-point types
      is_same<float, typename remove_cv<T>::type>::value ||
        is_same<double, typename remove_cv<T>::type>::value ||
        is_same<long double, typename remove_cv<T>::type>::value> {};

template <class T>
inline constexpr bool is_floating_point_v = is_floating_point<T>::value;
} // namespace tinystl
