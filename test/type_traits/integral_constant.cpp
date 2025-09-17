#include "catch2/catch_test_macros.hpp"

#include "tinystl/type_traits/helper_classes/integral_constant.h"
#include "tinystl/type_traits/type_relationships/is_same.h"

TEST_CASE("integral_constant checks", "[type_traits][integral_constant]") {
  SECTION("type and value checks") {
    using two_t = tinystl::integral_constant<int, 2>;
    using four_t = tinystl::integral_constant<int, 4>;
    REQUIRE_FALSE(tinystl::is_same_v<two_t, four_t>);
    REQUIRE(two_t::value * 2 == four_t::value);
    REQUIRE((two_t() << 1) == (four_t() >> 0));
  }

  SECTION("enum type checks") {
    enum class E { e1, e2 };
    using c1 = tinystl::integral_constant<E, E::e1>;
    using c2 = tinystl::integral_constant<E, E::e2>;
    REQUIRE(c1::value != E::e2);
    REQUIRE(c1() == E::e1);
    REQUIRE(tinystl::is_same_v<c2, c2>);
  }
}
