#pragma once

namespace tinystl {
// https://en.cppreference.com/w/cpp/types/enable_if.html
template <bool B, class T = void>
struct enable_if {};

template <class T> 
struct enable_if<true, T> {
  typedef T type;
};

template <bool B, class T = void>
using enable_if_t = typename enable_if<B, T>::type;

} // namespace tinystl