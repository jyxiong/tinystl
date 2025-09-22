#pragma once

#include "tinystl/type_traits/integral_constant.h"

namespace tinystl {
template<class T>
struct is_rvalue_reference : false_type {};
template<class T>
struct is_rvalue_reference<T&&> : true_type {};

template <class T>
inline constexpr bool is_rvalue_reference_v = is_rvalue_reference<T>::value;
}
