#pragma once

#include "tinystl/type_traits/helper_classes/integral_constant.h"

namespace tinystl {
template<class T>
struct is_pointer : false_type {};
 
template<class T>
struct is_pointer<T*> : true_type {};
 
template<class T>
struct is_pointer<T* const> : true_type {};
 
template<class T>
struct is_pointer<T* volatile> : true_type {};
 
template<class T>
struct is_pointer<T* const volatile> : true_type {};
} // namespace tinystl