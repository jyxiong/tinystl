#pragma once

namespace tinystl {
// https://en.cppreference.com/w/cpp/types/remove_cv.html

template <class T>
struct remove_cv {
  typedef T type;
};
template <class T>
struct remove_cv<const T> {
  typedef T type;
};
template <class T>
struct remove_cv<volatile T> {
  typedef T type;
};
template <class T>
struct remove_cv<const volatile T> {
  typedef T type;
};

template <class T>
struct remove_const {
  typedef T type;
};
template <class T>
struct remove_const<const T> {
  typedef T type;
};

template <class T>
struct remove_volatile {
  typedef T type;
};
template <class T>
struct remove_volatile<volatile T> {
  typedef T type;
};

template <class T>
using remove_cv_t = typename remove_cv<T>::type; // since C++14
template <class T>
using remove_const_t = typename remove_const<T>::type; // since C++14
template <class T>
using remove_volatile_t = typename remove_volatile<T>::type; // since C++14

}; // namespace tinystl