#include "catch2/catch_test_macros.hpp"

#include "tinystl/type_traits.h"

TEST_CASE("is_same basic checks", "[type_traits][is_same]") {
    SECTION("int and int32_t") {
        // usually true if 'int' is 32 bit
        REQUIRE(tinystl::is_same<int, std::int32_t>::value == (sizeof(int) == 4));
        // possibly true if ILP64 data model is used
        REQUIRE(tinystl::is_same<int, std::int64_t>::value == (sizeof(int) == 8));
    }

    SECTION("is_same_v syntax") {
        REQUIRE(tinystl::is_same_v<int, std::int32_t> == (sizeof(int) == 4));
        REQUIRE(tinystl::is_same_v<int, std::int64_t> == (sizeof(int) == 8));
    }

    SECTION("decltype comparison") {
        long double num1 = 1.0;
        long double num2 = 2.0;
        REQUIRE(tinystl::is_same_v<decltype(num1), decltype(num2)>);
    }

    SECTION("float and int32_t are not same") {
        REQUIRE_FALSE(tinystl::is_same<float, std::int32_t>::value);
    }

    SECTION("int signedness") {
        REQUIRE(tinystl::is_same_v<int, int>);
        REQUIRE_FALSE(tinystl::is_same_v<int, unsigned int>);
        REQUIRE(tinystl::is_same_v<int, signed int>);
    }

    SECTION("char signedness") {
        REQUIRE(tinystl::is_same_v<char, char>);
        REQUIRE_FALSE(tinystl::is_same_v<char, unsigned char>);
        REQUIRE_FALSE(tinystl::is_same_v<char, signed char>);
    }

    SECTION("const-qualified type") {
        REQUIRE_FALSE(tinystl::is_same<const int, int>());
    }
}