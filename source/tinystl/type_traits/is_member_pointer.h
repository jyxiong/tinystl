#pragma once

#include "tinystl/type_traits/remove_cv.h"
#include "tinystl/type_traits/integral_constant.h"

namespace tinystl {
template <class T>
struct is_member_pointer_helper : false_type {};

template <class T, class U>
struct is_member_pointer_helper<T U::*> : true_type {};

template <class T>
struct is_member_pointer
  : is_member_pointer_helper<typename remove_cv<T>::type> {};

template <class T>
inline constexpr bool is_member_pointer_v = is_member_pointer<T>::value;
} // namespace tinystl
