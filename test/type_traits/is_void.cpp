#include <catch2/catch_test_macros.hpp>

#include <tinystl/type_traits.h>

void foo();

TEST_CASE("is_void trait", "[type_traits][is_void]") {
  SECTION("basic void types") {
    REQUIRE(tinystl::is_void_v<void>);
    REQUIRE(tinystl::is_void_v<const void>);
    REQUIRE(tinystl::is_void_v<volatile void>);
    REQUIRE(tinystl::is_void_v<const volatile void>);
    REQUIRE(tinystl::is_void_v<tinystl::void_t<>>);
  }
  SECTION("non-void types") {
    REQUIRE_FALSE(tinystl::is_void_v<void *>);
    REQUIRE_FALSE(tinystl::is_void_v<int>);
    REQUIRE_FALSE(tinystl::is_void_v<decltype(foo)>);
    REQUIRE_FALSE(tinystl::is_void_v<tinystl::is_void<void>>);
  }
}
