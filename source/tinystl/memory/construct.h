#pragma once

#include <utility>

namespace tinystl {
template <
  class T, class... Args,
  class = decltype(::new (std::declval<void *>()) T(std::declval<Args>()...))>
constexpr T *construct_at(T *location, Args &&...args) {
  return ::new (static_cast<void *>(location)) T(std::forward<Args>(args)...);
}

} // namespace tinystl