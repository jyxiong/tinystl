#pragma once

#include "tinystl/type_traits/integral_constant.h"
#include "tinystl/type_traits/void_t.h"
#include "tinystl/type_traits/is_same.h"
#include "tinystl/type_traits/is_convertible.h"

namespace detail {
template <
  class Default, class AlwaysVoid, template <class...> class Op, class... Args>
struct detector {
  using value_t = tinystl::false_type;
  using type = Default;
};

template <class Default, template <class...> class Op, class... Args>
struct detector<Default, tinystl::void_t<Op<Args...>>, Op, Args...> {
  using value_t = tinystl::true_type;
  using type = Op<Args...>;
};
} // namespace detail

namespace tinystl {

struct nonesuch {
  ~nonesuch() = delete;
  nonesuch(nonesuch const &) = delete;
  void operator=(nonesuch const &) = delete;
};

template <template <class...> class Op, class... Args>
using is_detected =
  typename detail::detector<nonesuch, void, Op, Args...>::value_t;

template <template <class...> class Op, class... Args>
using detected_t = typename detail::detector<nonesuch, void, Op, Args...>::type;

template <class Default, template <class...> class Op, class... Args>
using detected_or = detail::detector<Default, void, Op, Args...>;

template< template<class...> class Op, class... Args >
constexpr inline bool is_detected_v = is_detected<Op, Args...>::value;

template< class Default, template<class...> class Op, class... Args >
using detected_or_t = typename detected_or<Default, Op, Args...>::type;

template< class Expected, template<class...> class Op, class... Args >
using is_detected_exact = is_same<Expected, detected_t<Op, Args...>>;

template< class Expected, template<class...> class Op, class... Args >
constexpr inline bool is_detected_exact_v =
    is_detected_exact<Expected, Op, Args...>::value;

template< class To, template<class...> class Op, class... Args >
using is_detected_convertible =
    is_convertible<detected_t<Op, Args...>, To>;

template< class To, template<class...> class Op, class... Args >
constexpr inline bool is_detected_convertible_v =
    is_detected_convertible<To, Op, Args...>::value;

} // namespace tinystl