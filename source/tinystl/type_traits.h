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
#include <tinystl/type_traits/miscellaneous_transformations/conditional.h>
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
