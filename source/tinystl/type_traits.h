#pragma once

// https://en.cppreference.com/w/cpp/header/type_traits.html

// composite_type_categories

#include <tinystl/type_traits/composite_type_categories/is_arithmetic.h>
#include <tinystl/type_traits/composite_type_categories/is_member_pointer.h>
#include <tinystl/type_traits/composite_type_categories/is_object.h>
#include <tinystl/type_traits/composite_type_categories/is_scalar.h>

// const_volatility_specifiers
#include <tinystl/type_traits/const_volatility_specifiers/remove_cv.h>

// miscellaneous_transformations
#include <tinystl/type_traits/miscellaneous_transformations/enable_if.h>
#include <tinystl/type_traits/miscellaneous_transformations/void_t.h>

// primary_type_categories
#include <tinystl/type_traits/primary_type_categories/is_array.h>
#include <tinystl/type_traits/primary_type_categories/is_class.h>
#include <tinystl/type_traits/primary_type_categories/is_enum.h>
#include <tinystl/type_traits/primary_type_categories/is_floating_point.h>
#include <tinystl/type_traits/primary_type_categories/is_integral.h>
#include <tinystl/type_traits/primary_type_categories/is_null_pointer.h>
#include <tinystl/type_traits/primary_type_categories/is_pointer.h>
#include <tinystl/type_traits/primary_type_categories/is_union.h>
#include <tinystl/type_traits/primary_type_categories/is_void.h>

// type_relationships
#include <tinystl/type_traits/type_relationships/is_same.h>

namespace tinystl {
// composite_type_categories
template <class T>
inline constexpr bool is_arithmetic_v = is_arithmetic<T>::value;
template <class T>
inline constexpr bool is_member_pointer_v = is_member_pointer<T>::value;
template <class T> inline constexpr bool is_object_v = is_object<T>::value;
template <class T> inline constexpr bool is_scalar_v = is_scalar<T>::value;

// const_volatility_specifiers
template <class T>
using remove_cv_t = typename remove_cv<T>::type; // since C++14
template <class T>
using remove_const_t = typename remove_const<T>::type; // since C++14
template <class T>
using remove_volatile_t = typename remove_volatile<T>::type; // since C++14

// miscellaneous_transformations
template <bool B, class T = void>
using enable_if_t = typename enable_if<B, T>::type;

// primary_type_categories
template <class T> inline constexpr bool is_array_v = is_array<T>::value;
template <class T> inline constexpr bool is_class_v = is_class<T>::value;
template <class T> inline constexpr bool is_enum_v = is_enum<T>::value;
template <class T>
inline constexpr bool is_floating_point_v = is_floating_point<T>::value;
template <class T> inline constexpr bool is_integral_v = is_integral<T>::value;
template <class T>
inline constexpr bool is_null_pointer_v = is_null_pointer<T>::value;
template <class T> inline constexpr bool is_pointer_v = is_pointer<T>::value;
template <class T> inline constexpr bool is_union_v = is_union<T>::value;
template <class T> inline constexpr bool is_void_v = is_void<T>::value;

// type_relationships
template <class T, class U> constexpr bool is_same_v = is_same<T, U>::value;
} // namespace tinystl