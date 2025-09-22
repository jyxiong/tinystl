#pragma once

#include "tinystl/type_traits/remove_cv.h"
#include "tinystl/type_traits/is_same.h"

namespace tinystl {

// https://en.cppreference.com/w/cpp/types/is_void.html
template <class T>
struct is_void : is_same<void, typename remove_cv<T>::type> {};

template <class T>
inline constexpr bool is_void_v = is_void<T>::value;

} // namespace tinystl
