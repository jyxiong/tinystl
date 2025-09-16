#pragma once

#include <stdfloat>

#include "tinystl/type_traits/helper_classes/integral_constant.h"
#include "tinystl/type_traits/type_relationships/is_same.h"
#include "tinystl/type_traits/const_volatility_specifiers/remove_cv.h"

namespace tinystl {
template <class T>
    struct is_floating_point : integral_constant < bool,
    // Note: standard floating-point types
    is_same<float, typename remove_cv<T>::type>::value ||
        is_same<double, typename remove_cv<T>::type>::value ||
        is_same<long double, typename remove_cv<T>::type>::value >{};
} // namespace tinystl