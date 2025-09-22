#pragma once

#include "tinystl/type_traits/is_scalar.h"
#include "tinystl/type_traits/integral_constant.h"
#include "tinystl/type_traits/is_array.h"
#include "tinystl/type_traits/is_class.h"
#include "tinystl/type_traits/is_union.h"

namespace tinystl {
// https://en.cppreference.com/w/cpp/types/is_object.html
template <class T>
struct is_object : integral_constant<
                     bool, is_scalar<T>::value || is_array<T>::value ||
                             is_union<T>::value || is_class<T>::value> {};

template <class T>
inline constexpr bool is_object_v = is_object<T>::value;

} // namespace tinystl
