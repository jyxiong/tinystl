#pragma once

#include "tinystl/type_traits/add_reference.h"
#include "tinystl/type_traits/integral_constant.h"


namespace tinystl {

template <class _Tp, class... _Args>
struct is_constructible : bool_constant<__is_constructible(_Tp, _Args...)> {};

template <class _Tp, class... _Args>
inline constexpr bool is_constructible_v = __is_constructible(_Tp, _Args...);

template <class _Tp>
struct is_copy_constructible
  : bool_constant<__is_constructible(_Tp, add_lvalue_reference_t<const _Tp>)> {
};

template <class _Tp>
inline constexpr bool is_copy_constructible_v =
  is_copy_constructible<_Tp>::value;

template <class _Tp>
struct is_move_constructible
  : bool_constant<__is_constructible(_Tp, add_rvalue_reference_t<_Tp>)> {};

template <class _Tp>
inline constexpr bool is_move_constructible_v =
  is_move_constructible<_Tp>::value;

template <class _Tp>
struct is_default_constructible : bool_constant<__is_constructible(_Tp)> {};

template <class _Tp>
inline constexpr bool is_default_constructible_v = __is_constructible(_Tp);

} // namespace tinystl