#pragma once

// https://en.cppreference.com/w/cpp/header/type_traits.html

// composite_type_categories
#include <tinystl/type_traits/is_arithmetic.h>
#include <tinystl/type_traits/is_member_pointer.h>
#include <tinystl/type_traits/is_object.h>
#include <tinystl/type_traits/is_reference.h>
#include <tinystl/type_traits/is_scalar.h>


// const_volatility_specifiers
#include <tinystl/type_traits/remove_cv.h>

// miscellaneous_transformations
#include <tinystl/type_traits/conditional.h>
#include <tinystl/type_traits/enable_if.h>
#include <tinystl/type_traits/void_t.h>

// primary_type_categories
#include <tinystl/type_traits/is_array.h>
#include <tinystl/type_traits/is_class.h>
#include <tinystl/type_traits/is_enum.h>
#include <tinystl/type_traits/is_floating_point.h>
#include <tinystl/type_traits/is_integral.h>
#include <tinystl/type_traits/is_lvalue_reference.h>
#include <tinystl/type_traits/is_null_pointer.h>
#include <tinystl/type_traits/is_pointer.h>
#include <tinystl/type_traits/is_rvalue_reference.h>
#include <tinystl/type_traits/is_union.h>
#include <tinystl/type_traits/is_void.h>

// type_properties
#include <tinystl/type_traits/is_empty.h>

// type_relationships
#include <tinystl/type_traits/is_same.h>
