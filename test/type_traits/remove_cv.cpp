#include <catch2/catch_test_macros.hpp>

#include <tinystl/type_traits.h>

template <typename U, typename V>
constexpr bool same = tinystl::is_same_v<U, V>;

TEST_CASE("remove_cv_t type trait", "[type_traits][remove_cv]") {
  SECTION("basic cv removal") {
    REQUIRE(same<tinystl::remove_cv_t<int>, int>);
    REQUIRE(same<tinystl::remove_cv_t<const int>, int>);
    REQUIRE(same<tinystl::remove_cv_t<volatile int>, int>);
    REQUIRE(same<tinystl::remove_cv_t<const volatile int>, int>);
  }

  SECTION("pointer cv removal") {
    // remove_cv only works on types, not on pointers
    REQUIRE_FALSE(same<tinystl::remove_cv_t<const volatile int *>, int *>);
    REQUIRE(
      same<tinystl::remove_cv_t<const volatile int *>, const volatile int *>
    );
    REQUIRE(same<tinystl::remove_cv_t<const int *volatile>, const int *>);
    REQUIRE(same<tinystl::remove_cv_t<int *const volatile>, int *>);
  }
}
