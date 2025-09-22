#pragma once

#include "tinystl/type_traits/integral_constant.h"
#include "tinystl/type_traits/void_t.h"

namespace tinystl {
template <class T, class = void>
struct is_referenceable : false_type {};

template <class T>
struct is_referenceable<T, void_t<T &>> : true_type {};

template <class T>
inline constexpr bool is_referenceable_v = is_referenceable<T>::value;

// template <class T, class = void>
// inline const bool is_referenceable_v = false;

// template <class T>
// inline const bool is_referenceable_v<T, void_t<T &>> = true;

template <class T>
concept referenceable = is_referenceable_v<T>;

} // namespace tinystl
