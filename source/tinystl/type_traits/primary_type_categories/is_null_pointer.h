#pragma once

#include <cstddef>

#include "tinystl/type_traits/type_relationships/is_same.h"
#include "tinystl/type_traits/const_volatility_specifiers/remove_cv.h"

namespace tinystl {
template<class T>
struct is_null_pointer : is_same<std::nullptr_t, typename remove_cv<T>::type> {};

template <class T>
inline constexpr bool is_null_pointer_v = is_null_pointer<T>::value;
};