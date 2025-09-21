#include <catch2/catch_test_macros.hpp>

#include <memory>
#include <string>

#include <tinystl/memory.h>
#include <tinystl/type_traits.h>

TEST_CASE("tinystl::allocator basic int usage", "[allocator][int]") {

  // construct
  std::allocator<int> std_alloc;
  int* std_p = std_alloc.allocate(1);
  std::allocator_traits<std::allocator<int>>::construct(std_alloc, std_p, 7);
  REQUIRE(*std_p == 7);
  std::allocator_traits<std::allocator<int>>::destroy(std_alloc, std_p);
  std_alloc.deallocate(std_p, 1);

  tinystl::allocator<int> alloc1;
  static_assert(tinystl::is_same_v<int, decltype(alloc1)::value_type>);
  int *p1 = alloc1.allocate(1);
  alloc1.deallocate(p1, 1);

  using traits_t1 = std::allocator_traits<decltype(alloc1)>;
  p1 = traits_t1::allocate(alloc1, 1);
  traits_t1::construct(alloc1, p1, 7);
  REQUIRE(*p1 == 7);
  traits_t1::deallocate(alloc1, p1, 1);
}

TEST_CASE("tinystl::allocator with std::string", "[allocator][string]") {
  tinystl::allocator<std::string> alloc2;
  using traits_t2 = tinystl::allocator_traits<decltype(alloc2)>;
  traits_t2::rebind_alloc<std::string> alloc_ = alloc2;
  std::string *p2 = traits_t2::allocate(alloc2, 2);
  traits_t2::construct(alloc2, p2, "foo");
  traits_t2::construct(alloc2, p2 + 1, "bar");
  REQUIRE(p2[0] == "foo");
  REQUIRE(p2[1] == "bar");
  traits_t2::destroy(alloc2, p2 + 1);
  traits_t2::destroy(alloc2, p2);
  traits_t2::deallocate(alloc2, p2, 2);
}
