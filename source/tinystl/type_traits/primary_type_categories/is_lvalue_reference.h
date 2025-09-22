#pragma once

#include "tinystl/type_traits/helper_classes/integral_constant.h"

namespace tinystl {
template<class T>
struct is_lvalue_reference : false_type {};
template<class T>
struct is_lvalue_reference<T&> : true_type {};

template<class T>
inline constexpr bool is_lvalue_reference_v = is_lvalue_reference<T>::value;
}