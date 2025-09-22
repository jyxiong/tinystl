#pragma once

#include <concepts>
#include <cstddef>
#include <iterator>

#include "tinystl/iterator/iterator_tag.h"
#include "tinystl/type_traits/is_object.h"
#include "tinystl/type_traits/is_renferenceable.h"

// https://en.cppreference.com/w/cpp/iterator/iterator_tags.html
// https://en.cppreference.com/w/cpp/iterator/iterator_traits.html
namespace detail {
template <class I>
concept cpp17_iterator = requires(I i) {
  { *i } -> tinystl::referenceable;
  { ++i } -> std::same_as<I &>;
  { *i++ } -> tinystl::referenceable;
} && std::copyable<I>;

template <class I>
concept cpp17_input_iterator =
  cpp17_iterator<I> && std::equality_comparable<I> && requires(I i) {
    typename std::incrementable_traits<I>::difference_type;
    typename std::indirectly_readable_traits<I>::value_type;
    typename std::common_reference_t<
      std::iter_reference_t<I> &&,
      typename std::indirectly_readable_traits<I>::value_type &>;
    *i++;
    typename std::common_reference_t<
      decltype(*i++) &&,
      typename std::indirectly_readable_traits<I>::value_type &>;
    requires std::signed_integral<
      typename std::incrementable_traits<I>::difference_type>;
  };

template <class It>
concept cpp17_forward_iterator =
  cpp17_input_iterator<It> && std::constructible_from<It> &&
  std::is_reference_v<std::iter_reference_t<It>> &&
  std::same_as<
    std::remove_cvref_t<std::iter_reference_t<It>>,
    typename std::indirectly_readable_traits<It>::value_type> &&
  requires(It it) {
    { it++ } -> std::convertible_to<const It &>;
    { *it++ } -> std::same_as<std::iter_reference_t<It>>;
  };

template <class I>
concept cpp17_bidirectional_iterator =
  cpp17_forward_iterator<I> && requires(I i) {
    { --i } -> std::same_as<I &>;
    { i-- } -> std::convertible_to<const I &>;
    { *i-- } -> std::same_as<std::iter_reference_t<I>>;
  };

template <class I>
concept cpp17_random_access_iterator =
  cpp17_bidirectional_iterator<I> && std::totally_ordered<I> &&
  requires(I i, typename std::incrementable_traits<I>::difference_type n) {
    { i += n } -> std::same_as<I &>;
    { i -= n } -> std::same_as<I &>;
    { i + n } -> std::same_as<I>;
    { n + i } -> std::same_as<I>;
    { i - n } -> std::same_as<I>;
    { i - i } -> std::same_as<decltype(n)>;
    { i[n] } -> std::convertible_to<std::iter_reference_t<I>>;
  };

// has nested types
template <class Iter>
concept has_iterator_category = requires { typename Iter::iterator_category; };

template <class Iter>
concept has_value_type = requires { typename Iter::value_type; };

template <class Iter>
concept has_difference_type = requires { typename Iter::difference_type; };

template <class Iter>
concept has_pointer = requires { typename Iter::pointer; };

template <class Iter>
concept has_reference = requires { typename Iter::reference; };

// get pointer type
template <class Iter>
concept has_arrow =
  requires(Iter &&iter) { static_cast<Iter &&>(iter).operator->(); };

template <class Iter>
struct get_pointer {
  using type = void;
};

template <has_pointer Iter>
struct get_pointer<Iter> {
  using type = typename Iter::pointer;
};

template <class Iter>
  requires(!has_pointer<Iter>) && (has_arrow<Iter>)
struct get_pointer<Iter> {
  using type = decltype(std::declval<Iter &>().operator->());
};

// get reference type
template <class Iter>
struct get_reference {
  using type = std::remove_reference_t<Iter>;
};

template <has_reference Iter>
struct get_reference<Iter> {
  using type = typename Iter::reference;
};

// get iterator category
template <class Iter>
struct get_iterator_category_impl {
  using type = tinystl::input_iterator_tag;
};

template <cpp17_forward_iterator Iter>
struct get_iterator_category_impl<Iter> {
  using type = tinystl::forward_iterator_tag;
};

template <cpp17_bidirectional_iterator Iter>
struct get_iterator_category_impl<Iter> {
  using type = tinystl::bidirectional_iterator_tag;
};

template <cpp17_random_access_iterator Iter>
struct get_iterator_category_impl<Iter> {
  using type = tinystl::random_access_iterator_tag;
};

template <class Iter>
struct get_iterator_category : get_iterator_category_impl<Iter> {};

template <has_iterator_category Iter>
struct get_iterator_category<Iter> {
  using type = Iter::iterator_category;
};

template <class Iter>
struct get_difference_type {
  using type = void;
};

template <class Iter>
  requires requires {
    typename std::incrementable_traits<Iter>::difference_type;
  }
struct get_difference_type<Iter> {
  using type = typename std::incrementable_traits<Iter>::difference_type;
};

template <class Iter>
concept specifies_members = requires {
  requires has_value_type<Iter>;
  requires has_difference_type<Iter>;
  requires has_reference<Iter>;
  requires has_iterator_category<Iter>;
  requires !has_pointer<Iter>;
};

template <class _Tp>
concept cpp17_iterator_missing_members =
  !specifies_members<_Tp> && cpp17_iterator<_Tp>;

template <class _Tp>
concept cpp17_input_iterator_missing_members =
  cpp17_iterator_missing_members<_Tp> && cpp17_input_iterator<_Tp>;

} // namespace detail

namespace tinystl {

template <class Iter>
struct iterator_traits {};

template <detail::specifies_members Iter>
struct iterator_traits<Iter> {
  using iterator_category = typename Iter::iterator_category;
  using value_type = typename Iter::value_type;
  using difference_type = typename Iter::difference_type;
  using pointer = void;
  using reference = typename Iter::reference;
};

template <detail::cpp17_input_iterator_missing_members Iter>
struct iterator_traits<Iter> {
  using iterator_category = typename detail::get_iterator_category<Iter>::type;
  using value_type = typename std::indirectly_readable_traits<Iter>::value_type;
  using difference_type =
    typename std::incrementable_traits<Iter>::difference_type;
  using pointer = typename detail::get_pointer<Iter>::type;
  using reference = typename detail::get_reference<Iter>::type;
};

template <detail::cpp17_iterator_missing_members Iter>
struct iterator_traits<Iter> {
  using iterator_category = output_iterator_tag;
  using value_type = void;
  using difference_type = detail::get_difference_type<Iter>::type;
  using pointer = void;
  using reference = void;
};

template <class T>
  requires is_object_v<T>
struct iterator_traits<T *> {
  using iterator_category = random_access_iterator_tag;
  using value_type = remove_cv_t<T>;
  using difference_type = std::ptrdiff_t;
  using pointer = T *;
  using reference = T &;
  using iterator_concept = contiguous_iterator_tag;
};

template <class T>
struct [[deprecated("deprecated")]] iterator_traits<const T *> {
  using iterator_category = random_access_iterator_tag;
  using value_type = T;
  using difference_type = ptrdiff_t;
  using pointer = const T *;
  using reference = const T &;
};

} // namespace tinystl
