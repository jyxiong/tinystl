#include <array>

#include "catch2/catch_test_macros.hpp"

#include "tinystl/array.h"

TEST_CASE("Array constructors", "[array][ctor]") {
  SECTION("aggregate initialization") {
    tinystl::array<int, 3> arr = {1, 2, 3};
    REQUIRE(arr[0] == 1);
    REQUIRE(arr[1] == 2);
    REQUIRE(arr[2] == 3);
  }

  SECTION("copy constructor") {
    tinystl::array<int, 3> arr1 = {4, 5, 6};
    tinystl::array<int, 3> arr2(arr1);
    for (size_t i = 0; i < 3; ++i) {
      REQUIRE(arr2[i] == arr1[i]);
    }
  }

  SECTION("copy assignment") {
    tinystl::array<int, 3> arr1 = {7, 8, 9};
    tinystl::array<int, 3> arr2;
    arr2 = arr1;
    for (size_t i = 0; i < 3; ++i) {
      REQUIRE(arr2[i] == arr1[i]);
    }
  }

  SECTION("move constructor") {
    tinystl::array<int, 3> arr1 = {10, 11, 12};
    tinystl::array<int, 3> arr2(std::move(arr1));
    // arr2 content is valid, arr1 is unspecified
    REQUIRE(arr2[0] == 10);
    REQUIRE(arr2[1] == 11);
    REQUIRE(arr2[2] == 12);
  }

  SECTION("move assignment") {
    tinystl::array<int, 3> arr1 = {13, 14, 15};
    tinystl::array<int, 3> arr2;
    arr2 = std::move(arr1);
    REQUIRE(arr2[0] == 13);
    REQUIRE(arr2[1] == 14);
    REQUIRE(arr2[2] == 15);
  }

  SECTION("compare with std::array") {
    tinystl::array<int, 3> arr = {1, 2, 3};
    std::array<int, 3> stdarr = {1, 2, 3};
    for (size_t i = 0; i < 3; ++i) {
      REQUIRE(arr[i] == stdarr[i]);
    }
  }
}

TEST_CASE("Element access:", "[array]") {
  tinystl::array<int, 5> arr = {1, 2, 3, 4, 5};
  std::array<int, 5> std_arr = {1, 2, 3, 4, 5};

  // Test at()
  SECTION("at returns correct element") {
    // Check that at() returns the correct value for each index
    for (size_t i = 0; i < 5; ++i) {
      REQUIRE(arr.at(i) == std_arr.at(i));
    }
  }

  SECTION("at throws on out-of-range") {
    // Check that at() throws std::out_of_range for invalid index
    REQUIRE_THROWS_AS(arr.at(5), std::out_of_range);
    REQUIRE_THROWS_AS(std_arr.at(5), std::out_of_range);
  }

  // Test operator[]
  SECTION("operator[] returns correct element") {
    for (size_t i = 0; i < 5; ++i) {
      REQUIRE(arr[i] == std_arr[i]);
    }
  }

  // Test front()
  SECTION("front returns first element") {
    REQUIRE(arr.front() == 1);
    REQUIRE(std_arr.front() == 1);
  }

  // Test back()
  SECTION("back returns last element") {
    REQUIRE(arr.back() == 5);
    REQUIRE(std_arr.back() == 5);
  }
}

TEST_CASE("Array iterators", "[array][iterator]") {
  tinystl::array<int, 5> arr = {1, 2, 3, 4, 5};
  std::array<int, 5> std_arr = {1, 2, 3, 4, 5};

  SECTION("begin and end") {
    auto it = arr.begin();
    auto std_it = std_arr.begin();
    for (size_t i = 0; i < 5; ++i, ++it, ++std_it) {
      REQUIRE(*it == *std_it);
    }
    REQUIRE(it == arr.end());
    REQUIRE(std_it == std_arr.end());
  }

  SECTION("const begin and end") {
    const auto& carr = arr;
    auto it = carr.begin();
    auto std_it = std_arr.begin();
    for (size_t i = 0; i < 5; ++i, ++it, ++std_it) {
      REQUIRE(*it == *std_it);
    }
    REQUIRE(it == carr.end());
  }

  SECTION("rbegin and rend") {
    auto rit = arr.rbegin();
    auto std_rit = std_arr.rbegin();
    for (size_t i = 0; i < 5; ++i, ++rit, ++std_rit) {
      REQUIRE(*rit == *std_rit);
    }
    REQUIRE(rit == arr.rend());
    REQUIRE(std_rit == std_arr.rend());
  }

  SECTION("const rbegin and rend") {
    const auto& carr = arr;
    auto rit = carr.rbegin();
    auto std_rit = std_arr.rbegin();
    for (size_t i = 0; i < 5; ++i, ++rit, ++std_rit) {
      REQUIRE(*rit == *std_rit);
    }
    REQUIRE(rit == carr.rend());
  }
}


TEST_CASE("Array capacity", "[array][capacity]") {
  tinystl::array<int, 5> arr = {1, 2, 3, 4, 5};
  std::array<int, 5> std_arr = {1, 2, 3, 4, 5};

  SECTION("size") {
    REQUIRE(arr.size() == std_arr.size());
    REQUIRE(arr.size() == 5);
  }

  SECTION("max_size") {
    REQUIRE(arr.max_size() == std_arr.max_size());
    REQUIRE(arr.max_size() == 5);
  }

  SECTION("empty") {
    tinystl::array<int, 0> empty_arr;
    std::array<int, 0> std_empty_arr;
    REQUIRE(arr.empty() == false);
    REQUIRE(empty_arr.empty() == true);
    REQUIRE(std_arr.empty() == false);
    REQUIRE(std_empty_arr.empty() == true);
  }
}

TEST_CASE("Array operations", "[array][operations]") {
  tinystl::array<int, 5> arr;
  std::array<int, 5> std_arr;

  SECTION("fill") {
    arr.fill(42);
    std_arr.fill(42);
    for (size_t i = 0; i < 5; ++i) {
      REQUIRE(arr[i] == std_arr[i]);
      REQUIRE(arr[i] == 42);
    }
  }

  SECTION("swap") {
    tinystl::array<int, 5> arr2 = {1, 2, 3, 4, 5};
    std::array<int, 5> std_arr2 = {1, 2, 3, 4, 5};
    arr.swap(arr2);
    std_arr.swap(std_arr2);
    for (size_t i = 0; i < 5; ++i) {
      REQUIRE(arr[i] == std_arr[i]);
      REQUIRE(arr2[i] == std_arr2[i]);
    }
  }
}

TEST_CASE("Non-member functions", "[array][non-member]") {
  tinystl::array<int, 3> arr1 = {1, 2, 3};
  tinystl::array<int, 3> arr2 = {1, 2, 3};
  tinystl::array<int, 3> arr3 = {4, 5, 6};

  SECTION("equality and inequality") {
    REQUIRE(arr1 == arr2);
    REQUIRE(arr1 != arr3);
  }

  SECTION("relational operators") {
    REQUIRE(arr1 < arr3);
    REQUIRE(arr3 > arr1);
    REQUIRE(arr1 <= arr2);
    REQUIRE(arr1 >= arr2);
  }
}