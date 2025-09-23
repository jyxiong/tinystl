#pragma once

#include <cstddef>

#include "tinystl/type_traits/remove_cv.h"
#include "tinystl/type_traits/is_same.h"

namespace tinystl {
template <class T>
struct is_null_pointer : is_same<std::nullptr_t, remove_cv_t<T>> {
};

template <class T>
inline constexpr bool is_null_pointer_v = is_null_pointer<T>::value;
}; // namespace tinystl
