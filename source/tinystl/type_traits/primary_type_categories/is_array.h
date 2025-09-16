#pragma once

#include <cstddef>

#include "tinystl/type_traits/helper_classes/integral_constant.h"

namespace tinystl {

template<class T>
struct is_array : false_type {};
 
template<class T>
struct is_array<T[]> : true_type {};
 
template<class T, std::size_t N>
struct is_array<T[N]> : true_type {};

}