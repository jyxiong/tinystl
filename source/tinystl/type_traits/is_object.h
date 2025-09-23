#pragma once

#include "tinystl/type_traits/integral_constant.h"
#include "tinystl/type_traits/is_array.h"
#include "tinystl/type_traits/is_class.h"
#include "tinystl/type_traits/is_scalar.h"
#include "tinystl/type_traits/is_union.h"

namespace tinystl {
// https://en.cppreference.com/w/cpp/types/is_object.html
template <class T>
struct is_object
  : bool_constant<
      is_scalar_v<T> || is_array_v<T> || is_union_v<T> || is_class_v<T>> {};

template <class T>
inline constexpr bool is_object_v = is_object<T>::value;

} // namespace tinystl
