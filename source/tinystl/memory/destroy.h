#pragma once

#include "tinystl/memory/addressof.h"
#include "tinystl/type_traits/enable_if.h"
#include "tinystl/type_traits/is_array.h"

namespace tinystl {
template <class T, enable_if_t<!is_array_v<T>, int> = 0>
void destroy_at(T *p) {
  p->~T();
}

template <class T, enable_if_t<is_array_v<T>, int> = 0>
void destroy_at(T *p) {
  for (auto &elem : *p) {
    (destroy_at)(addressof(elem));
  }
}

} // namespace tinystl