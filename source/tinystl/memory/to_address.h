#pragma once

#include "tinystl/memory/pointer_traits.h"
#include "tinystl/type_traits/is_function.h"

namespace tinystl {
template <class T>
constexpr T *to_address(T *p) noexcept {
  static_assert(!is_function_v<T>);
  return p;
}

template <class T>
constexpr auto to_address(const T &p) noexcept {
  if constexpr (requires { pointer_traits<T>::to_address(p); })
    return pointer_traits<T>::to_address(p);
  else
    return to_address(p.operator->());
}
} // namespace tinystl