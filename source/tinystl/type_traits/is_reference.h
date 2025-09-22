#pragma once

#include "tinystl/type_traits/is_lvalue_reference.h"
#include "tinystl/type_traits/is_rvalue_reference.h"

namespace tinystl {
template<class T>
struct is_reference : bool_constant<is_lvalue_reference<T>::value || is_rvalue_reference<T>::value> {};

template <class T>
inline constexpr bool is_reference_v = is_reference<T>::value;
}
