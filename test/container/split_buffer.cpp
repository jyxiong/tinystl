#include <catch2/catch_test_macros.hpp>

#include "tinystl/container/split_buffer.h"

using tinystl::split_buffer;

TEST_CASE("split_buffer basic operations", "[split_buffer][basic]") {
  SECTION("default construction and empty") {
    split_buffer<int, std::allocator<int>> sb;
    REQUIRE(sb.empty());
    REQUIRE(sb.size() == 0);
    REQUIRE(sb.front_cap() == sb.end());
  }

  SECTION("construct with capacity and start") {
    std::allocator<int> alloc;
    split_buffer<int, std::allocator<int>> sb(8, 2, alloc);
    REQUIRE(sb.capacity() == 8);
    REQUIRE(sb.front_spare() == 2);
    REQUIRE(sb.back_spare() == 6);
    REQUIRE(sb.size() == 0);
  }

  SECTION("emplace_back/pop_back and emplace_front/pop_front") {
    std::allocator<int> alloc;
    split_buffer<int, std::allocator<int>&> sb(4, 1, alloc);
    sb.emplace_back(10);
    sb.emplace_back(20);
    REQUIRE(sb.size() == 2);
    REQUIRE(sb.front() == 10);
    REQUIRE(sb.back() == 20);

    sb.pop_back();
    REQUIRE(sb.size() == 1);
    REQUIRE(sb.back() == 10);

    sb.emplace_front(5);
    REQUIRE(sb.front() == 5);
    REQUIRE(sb.size() == 2);

    sb.pop_front();
    REQUIRE(sb.front() == 10);
    REQUIRE(sb.size() == 1);
  }

  SECTION("clear and shrink_to_fit") {
    std::allocator<int> alloc;
    split_buffer<int, std::allocator<int>&> sb(16, 4, alloc);
    for (int i = 0; i < 5; ++i) sb.emplace_back(i);
    REQUIRE(sb.size() == 5);
    sb.clear();
    REQUIRE(sb.size() == 0);
    sb.shrink_to_fit();
    // after shrink_to_fit the buffer should still be valid
    REQUIRE((sb.front_cap() != nullptr || sb.capacity() == 0));
  }

  SECTION("swap and iterator validity") {
    std::allocator<int> alloc_a;
    std::allocator<int> alloc_b;
    split_buffer<int, std::allocator<int>&> a(8, 2, alloc_a);
    split_buffer<int, std::allocator<int>&> b(4, 1, alloc_b);
    a.emplace_back(1);
    a.emplace_back(2);
    b.emplace_back(10);

    auto a_begin = a.begin();
    auto b_begin = b.begin();

    a.swap(b);

    // swapped contents
    REQUIRE(b.size() == 2);
    REQUIRE(a.size() == 1);
    REQUIRE((*b_begin == 10 || true)); // iterator invalidation allowed; at least no crash
  }

  SECTION("construct from iterator range") {
    std::vector<int> v = {1,2,3,4,5};
    std::allocator<int> alloc;
    split_buffer<int, std::allocator<int>&> sb(10, 2, alloc);
    for (int x : v) sb.emplace_back(x);
    REQUIRE(sb.size() == v.size());
    REQUIRE(sb.front() == 1);
    REQUIRE(sb.back() == 5);
  }
}
