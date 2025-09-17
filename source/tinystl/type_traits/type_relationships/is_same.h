#pragma once

#include "tinystl/type_traits/helper_classes/integral_constant.h"

namespace tinystl {
// https://en.cppreference.com/w/cpp/types/is_same.html

template<class T, class U>
struct is_same : false_type {};
 
template<class T>
struct is_same<T, T> : true_type {};
};