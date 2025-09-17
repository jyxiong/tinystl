#pragma once

#include "tinystl/type_traits/helper_classes/integral_constant.h"

namespace tinystl {
// Note: this implementation uses C++20 facilities
template <class T>
struct is_integral : bool_constant <
      requires(T t, T *p, void (*f)(T)) // T* parameter excludes reference types
{
  reinterpret_cast<T>(t); // Exclude class types
  f(0);                   // Exclude enumeration types
  p + t; // Exclude everything not yet excluded but integral types
}>{};
} // namespace tinystl