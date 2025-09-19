#pragma once

#include "tinystl/type_traits/composite_type_categories/is_object.h"
#include "tinystl/type_traits/miscellaneous_transformations/enable_if.h"

namespace tinystl {

template <class T>
typename enable_if<is_object<T>::value, T *>::type addressof(T &arg) noexcept {
  return reinterpret_cast<T *>(
    &const_cast<char &>(reinterpret_cast<const volatile char &>(arg))
  );
}

template <class T>
typename enable_if<!is_object<T>::value, T *>::type addressof(T &arg) noexcept {
  return &arg;
}
} // namespace tinystl