#pragma once

#include "tinystl/type_traits/helper_classes/integral_constant.h"
#include "tinystl/type_traits/const_volatility_specifiers/remove_cv.h"

namespace tinystl {
template<class T>
struct is_member_pointer_helper : false_type {};
 
template<class T, class U>
struct is_member_pointer_helper<T U::*> : true_type {};
 
template<class T>
struct is_member_pointer : is_member_pointer_helper<typename remove_cv<T>::type> {};

} // namespace tinystl