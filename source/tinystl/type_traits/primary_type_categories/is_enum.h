#pragma once

#include "tinystl/type_traits/helper_classes/integral_constant.h"

namespace tinystl {

template <class T>
struct is_enum : tinystl::bool_constant<__is_enum(T)> {};
} // namespace tinystl