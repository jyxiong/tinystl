#pragma once

#include <cstddef>

#include "tinystl/type_traits/type_relationships/is_same.h"
#include "tinystl/type_traits/const_volatility_specifiers/remove_cv.h"

namespace tinystl {
template<class T>
struct is_null_pointer : is_same<std::nullptr_t, remove_cv_t<T>> {};
};