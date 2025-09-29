#include <catch2/catch_test_macros.hpp>

#include <vector>

#include <tinystl/container/vector.h>

TEST_CASE("Vector constructors", "[vector][ctor]") {
  SECTION("default constructor") {
    tinystl::vector<int> vec;
    std::vector<int> std_vec;
    REQUIRE(vec.size() == std_vec.size());
    REQUIRE(vec.empty() == std_vec.empty());
  }

  SECTION("size constructor") {
    tinystl::vector<int> vec(5);
    std::vector<int> std_vec(5);
    REQUIRE(vec.size() == std_vec.size());
    REQUIRE(vec.size() == 5);
    for (size_t i = 0; i < vec.size(); ++i) {
      REQUIRE(vec[i] == std_vec[i]);  // Both should be default-constructed (0)
    }
  }

  SECTION("size and value constructor") {
    tinystl::vector<int> vec(5, 42);
    std::vector<int> std_vec(5, 42);
    REQUIRE(vec.size() == std_vec.size());
    for (size_t i = 0; i < vec.size(); ++i) {
      REQUIRE(vec[i] == std_vec[i]);
      REQUIRE(vec[i] == 42);
    }
  }

  SECTION("initializer list constructor") {
    tinystl::vector<int> vec = {1, 2, 3, 4, 5};
    std::vector<int> std_vec = {1, 2, 3, 4, 5};
    REQUIRE(vec.size() == std_vec.size());
    for (size_t i = 0; i < vec.size(); ++i) {
      REQUIRE(vec[i] == std_vec[i]);
    }
  }

  SECTION("copy constructor") {
    tinystl::vector<int> vec1 = {1, 2, 3, 4, 5};
    tinystl::vector<int> vec2(vec1);
    std::vector<int> std_vec1 = {1, 2, 3, 4, 5};
    std::vector<int> std_vec2(std_vec1);
    
    REQUIRE(vec2.size() == std_vec2.size());
    for (size_t i = 0; i < vec2.size(); ++i) {
      REQUIRE(vec2[i] == std_vec2[i]);
    }
  }

  SECTION("copy assignment") {
    tinystl::vector<int> vec1 = {1, 2, 3, 4, 5};
    tinystl::vector<int> vec2;
    vec2 = vec1;
    std::vector<int> std_vec1 = {1, 2, 3, 4, 5};
    std::vector<int> std_vec2;
    std_vec2 = std_vec1;
    
    REQUIRE(vec2.size() == std_vec2.size());
    for (size_t i = 0; i < vec2.size(); ++i) {
      REQUIRE(vec2[i] == std_vec2[i]);
    }
  }

  SECTION("move constructor") {
    tinystl::vector<int> vec1 = {1, 2, 3, 4, 5};
    auto vec1_size = vec1.size();
    tinystl::vector<int> vec2(std::move(vec1));
    
    std::vector<int> std_vec1 = {1, 2, 3, 4, 5};
    std::vector<int> std_vec2(std::move(std_vec1));
    
    REQUIRE(vec2.size() == vec1_size);
    REQUIRE(vec2.size() == 5);
    for (size_t i = 0; i < vec2.size(); ++i) {
      REQUIRE(vec2[i] == (int)(i + 1));
    }
  }

  SECTION("move assignment") {
    tinystl::vector<int> vec1 = {1, 2, 3, 4, 5};
    tinystl::vector<int> vec2;
    vec2 = std::move(vec1);
    
    std::vector<int> std_vec1 = {1, 2, 3, 4, 5};
    std::vector<int> std_vec2;
    std_vec2 = std::move(std_vec1);
    
    REQUIRE(vec2.size() == 5);
    for (size_t i = 0; i < vec2.size(); ++i) {
      REQUIRE(vec2[i] == (int)(i + 1));
    }
  }

  SECTION("range constructor") {
    std::vector<int> source = {10, 20, 30, 40, 50};
    tinystl::vector<int> vec(source.begin(), source.end());
    std::vector<int> std_vec(source.begin(), source.end());
    
    REQUIRE(vec.size() == std_vec.size());
    for (size_t i = 0; i < vec.size(); ++i) {
      REQUIRE(vec[i] == std_vec[i]);
    }
  }
}

TEST_CASE("Vector element access", "[vector][access]") {
  tinystl::vector<int> vec = {1, 2, 3, 4, 5};
  std::vector<int> std_vec = {1, 2, 3, 4, 5};

  SECTION("at returns correct element") {
    for (size_t i = 0; i < vec.size(); ++i) {
      REQUIRE(vec.at(i) == std_vec.at(i));
    }
  }

  SECTION("at throws on out-of-range") {
    REQUIRE_THROWS_AS((void)vec.at(5), std::out_of_range);
    REQUIRE_THROWS_AS((void)std_vec.at(5), std::out_of_range);
  }

  SECTION("operator[] returns correct element") {
    for (size_t i = 0; i < vec.size(); ++i) {
      REQUIRE(vec[i] == std_vec[i]);
    }
  }

  SECTION("front returns first element") {
    REQUIRE(vec.front() == std_vec.front());
    REQUIRE(vec.front() == 1);
  }

  SECTION("back returns last element") {
    REQUIRE(vec.back() == std_vec.back());
    REQUIRE(vec.back() == 5);
  }

  SECTION("data returns pointer to data") {
    int *data = vec.data();
    int *std_data = std_vec.data();
    for (size_t i = 0; i < vec.size(); ++i) {
      REQUIRE(data[i] == std_data[i]);
    }
  }
}

TEST_CASE("Vector iterators", "[vector][iterator]") {
  tinystl::vector<int> vec = {1, 2, 3, 4, 5};
  std::vector<int> std_vec = {1, 2, 3, 4, 5};

  SECTION("begin and end") {
    auto it = vec.begin();
    auto std_it = std_vec.begin();
    for (size_t i = 0; i < vec.size(); ++i, ++it, ++std_it) {
      REQUIRE(*it == *std_it);
    }
    REQUIRE(it == vec.end());
    REQUIRE(std_it == std_vec.end());
  }

  SECTION("const begin and end") {
    const auto &cvec = vec;
    const auto &cstd_vec = std_vec;
    auto it = cvec.begin();
    auto std_it = cstd_vec.begin();
    for (size_t i = 0; i < vec.size(); ++i, ++it, ++std_it) {
      REQUIRE(*it == *std_it);
    }
    REQUIRE(it == cvec.end());
  }

  SECTION("cbegin and cend") {
    auto it = vec.cbegin();
    auto std_it = std_vec.cbegin();
    for (size_t i = 0; i < vec.size(); ++i, ++it, ++std_it) {
      REQUIRE(*it == *std_it);
    }
    REQUIRE(it == vec.cend());
  }

  SECTION("rbegin and rend") {
    auto rit = vec.rbegin();
    auto std_rit = std_vec.rbegin();
    for (size_t i = 0; i < vec.size(); ++i, ++rit, ++std_rit) {
      REQUIRE(*rit == *std_rit);
    }
    REQUIRE(rit == vec.rend());
  }

  SECTION("const rbegin and rend") {
    const auto &cvec = vec;
    const auto &cstd_vec = std_vec;
    auto rit = cvec.rbegin();
    auto std_rit = cstd_vec.rbegin();
    for (size_t i = 0; i < vec.size(); ++i, ++rit, ++std_rit) {
      REQUIRE(*rit == *std_rit);
    }
    REQUIRE(rit == cvec.rend());
  }

  SECTION("crbegin and crend") {
    auto rit = vec.crbegin();
    auto std_rit = std_vec.crbegin();
    for (size_t i = 0; i < vec.size(); ++i, ++rit, ++std_rit) {
      REQUIRE(*rit == *std_rit);
    }
    REQUIRE(rit == vec.crend());
  }
}

TEST_CASE("Vector capacity", "[vector][capacity]") {
  tinystl::vector<int> vec;
  std::vector<int> std_vec;

  SECTION("empty vector") {
    REQUIRE(vec.size() == std_vec.size());
    REQUIRE(vec.empty() == std_vec.empty());
    REQUIRE(vec.empty() == true);
  }

  SECTION("non-empty vector") {
    vec = {1, 2, 3, 4, 5};
    std_vec = {1, 2, 3, 4, 5};
    REQUIRE(vec.size() == std_vec.size());
    REQUIRE(vec.size() == 5);
    REQUIRE(vec.empty() == false);
    REQUIRE(std_vec.empty() == false);
  }

  SECTION("max_size") {
    // max_size can differ between implementations, just check it's reasonable
    REQUIRE(vec.max_size() > 0);
    REQUIRE(std_vec.max_size() > 0);
  }

  SECTION("reserve increases capacity") {
    vec.reserve(10);
    std_vec.reserve(10);
    REQUIRE(vec.capacity() >= 10);
    REQUIRE(std_vec.capacity() >= 10);
    REQUIRE(vec.size() == 0);
    REQUIRE(std_vec.size() == 0);
  }

  SECTION("capacity after push_back") {
    size_t initial_cap = vec.capacity();
    for (int i = 0; i < 10; ++i) {
      vec.push_back(i);
    }
    REQUIRE(vec.capacity() >= initial_cap);
    REQUIRE(vec.size() == 10);
  }

  SECTION("shrink_to_fit") {
    vec.reserve(100);
    vec.push_back(1);
    vec.shrink_to_fit();
    // After shrink_to_fit, capacity should be close to size
    REQUIRE(vec.capacity() >= vec.size());
  }
}

TEST_CASE("Vector modifiers", "[vector][modifiers]") {
  SECTION("push_back and pop_back") {
    tinystl::vector<int> vec;
    std::vector<int> std_vec;
    
    for (int i = 1; i <= 5; ++i) {
      vec.push_back(i);
      std_vec.push_back(i);
    }
    
    REQUIRE(vec.size() == std_vec.size());
    for (size_t i = 0; i < vec.size(); ++i) {
      REQUIRE(vec[i] == std_vec[i]);
    }
    
    vec.pop_back();
    std_vec.pop_back();
    REQUIRE(vec.size() == 4);
    REQUIRE(vec.size() == std_vec.size());
  }

  SECTION("emplace_back") {
    tinystl::vector<std::pair<int, int>> vec;
    std::vector<std::pair<int, int>> std_vec;
    
    vec.emplace_back(1, 2);
    std_vec.emplace_back(1, 2);
    
    REQUIRE(vec.size() == std_vec.size());
    REQUIRE(vec.back().first == std_vec.back().first);
    REQUIRE(vec.back().second == std_vec.back().second);
  }

  SECTION("clear") {
    tinystl::vector<int> vec = {1, 2, 3, 4, 5};
    std::vector<int> std_vec = {1, 2, 3, 4, 5};
    
    vec.clear();
    std_vec.clear();
    
    REQUIRE(vec.size() == std_vec.size());
    REQUIRE(vec.empty() == std_vec.empty());
    REQUIRE(vec.empty() == true);
  }

  SECTION("insert single element") {
    tinystl::vector<int> vec = {1, 3, 5};
    std::vector<int> std_vec = {1, 3, 5};
    
    auto it = vec.insert(vec.begin() + 1, 2);
    auto std_it = std_vec.insert(std_vec.begin() + 1, 2);
    
    REQUIRE(vec.size() == std_vec.size());
    REQUIRE(*it == *std_it);
    for (size_t i = 0; i < vec.size(); ++i) {
      REQUIRE(vec[i] == std_vec[i]);
    }
  }

  SECTION("insert multiple elements") {
    tinystl::vector<int> vec = {1, 5};
    std::vector<int> std_vec = {1, 5};
    
    vec.insert(vec.begin() + 1, 3, 42);
    std_vec.insert(std_vec.begin() + 1, 3, 42);
    
    REQUIRE(vec.size() == std_vec.size());
    for (size_t i = 0; i < vec.size(); ++i) {
      REQUIRE(vec[i] == std_vec[i]);
    }
  }

  SECTION("insert range") {
    tinystl::vector<int> vec = {1, 5};
    std::vector<int> std_vec = {1, 5};
    std::vector<int> source = {2, 3, 4};
    
    vec.insert(vec.begin() + 1, source.begin(), source.end());
    std_vec.insert(std_vec.begin() + 1, source.begin(), source.end());
    
    REQUIRE(vec.size() == std_vec.size());
    for (size_t i = 0; i < vec.size(); ++i) {
      REQUIRE(vec[i] == std_vec[i]);
    }
  }

  SECTION("insert initializer list") {
    tinystl::vector<int> vec = {1, 5};
    std::vector<int> std_vec = {1, 5};
    
    vec.insert(vec.begin() + 1, {2, 3, 4});
    std_vec.insert(std_vec.begin() + 1, {2, 3, 4});
    
    REQUIRE(vec.size() == std_vec.size());
    for (size_t i = 0; i < vec.size(); ++i) {
      REQUIRE(vec[i] == std_vec[i]);
    }
  }

  SECTION("emplace") {
    tinystl::vector<std::pair<int, int>> vec;
    std::vector<std::pair<int, int>> std_vec;
    
    vec.emplace(vec.begin(), 1, 2);
    std_vec.emplace(std_vec.begin(), 1, 2);
    
    REQUIRE(vec.size() == std_vec.size());
    REQUIRE(vec[0].first == std_vec[0].first);
    REQUIRE(vec[0].second == std_vec[0].second);
  }

  SECTION("erase single element") {
    tinystl::vector<int> vec = {1, 2, 3, 4, 5};
    std::vector<int> std_vec = {1, 2, 3, 4, 5};
    
    auto it = vec.erase(vec.begin() + 2);
    auto std_it = std_vec.erase(std_vec.begin() + 2);
    
    REQUIRE(vec.size() == std_vec.size());
    REQUIRE(*it == *std_it);
    for (size_t i = 0; i < vec.size(); ++i) {
      REQUIRE(vec[i] == std_vec[i]);
    }
  }

  SECTION("erase range") {
    tinystl::vector<int> vec = {1, 2, 3, 4, 5};
    std::vector<int> std_vec = {1, 2, 3, 4, 5};
    
    auto it = vec.erase(vec.begin() + 1, vec.begin() + 4);
    auto std_it = std_vec.erase(std_vec.begin() + 1, std_vec.begin() + 4);
    
    REQUIRE(vec.size() == std_vec.size());
    REQUIRE(*it == *std_it);
    for (size_t i = 0; i < vec.size(); ++i) {
      REQUIRE(vec[i] == std_vec[i]);
    }
  }

  SECTION("resize") {
    tinystl::vector<int> vec = {1, 2, 3};
    std::vector<int> std_vec = {1, 2, 3};
    
    // Resize larger
    vec.resize(5);
    std_vec.resize(5);
    REQUIRE(vec.size() == std_vec.size());
    for (size_t i = 0; i < vec.size(); ++i) {
      REQUIRE(vec[i] == std_vec[i]);
    }
    
    // Resize smaller
    vec.resize(2);
    std_vec.resize(2);
    REQUIRE(vec.size() == std_vec.size());
    for (size_t i = 0; i < vec.size(); ++i) {
      REQUIRE(vec[i] == std_vec[i]);
    }
  }

  SECTION("resize with value") {
    tinystl::vector<int> vec = {1, 2, 3};
    std::vector<int> std_vec = {1, 2, 3};
    
    vec.resize(5, 42);
    std_vec.resize(5, 42);
    
    REQUIRE(vec.size() == std_vec.size());
    for (size_t i = 0; i < vec.size(); ++i) {
      REQUIRE(vec[i] == std_vec[i]);
    }
  }

  SECTION("swap") {
    tinystl::vector<int> vec1 = {1, 2, 3};
    tinystl::vector<int> vec2 = {4, 5, 6, 7};
    std::vector<int> std_vec1 = {1, 2, 3};
    std::vector<int> std_vec2 = {4, 5, 6, 7};
    
    vec1.swap(vec2);
    std_vec1.swap(std_vec2);
    
    REQUIRE(vec1.size() == std_vec1.size());
    REQUIRE(vec2.size() == std_vec2.size());
    for (size_t i = 0; i < vec1.size(); ++i) {
      REQUIRE(vec1[i] == std_vec1[i]);
    }
    for (size_t i = 0; i < vec2.size(); ++i) {
      REQUIRE(vec2[i] == std_vec2[i]);
    }
  }
}

TEST_CASE("Vector assign", "[vector][assign]") {
  SECTION("assign with count and value") {
    tinystl::vector<int> vec;
    std::vector<int> std_vec;
    
    vec.assign(5, 42);
    std_vec.assign(5, 42);
    
    REQUIRE(vec.size() == std_vec.size());
    for (size_t i = 0; i < vec.size(); ++i) {
      REQUIRE(vec[i] == std_vec[i]);
      REQUIRE(vec[i] == 42);
    }
  }

  SECTION("assign with range") {
    tinystl::vector<int> vec;
    std::vector<int> std_vec;
    std::vector<int> source = {10, 20, 30, 40, 50};
    
    vec.assign(source.begin(), source.end());
    std_vec.assign(source.begin(), source.end());
    
    REQUIRE(vec.size() == std_vec.size());
    for (size_t i = 0; i < vec.size(); ++i) {
      REQUIRE(vec[i] == std_vec[i]);
    }
  }

  SECTION("assign with initializer list") {
    tinystl::vector<int> vec;
    std::vector<int> std_vec;
    
    vec.assign({1, 2, 3, 4, 5});
    std_vec.assign({1, 2, 3, 4, 5});
    
    REQUIRE(vec.size() == std_vec.size());
    for (size_t i = 0; i < vec.size(); ++i) {
      REQUIRE(vec[i] == std_vec[i]);
    }
  }
}

TEST_CASE("Vector comparison operators", "[vector][comparison]") {
  tinystl::vector<int> vec1 = {1, 2, 3};
  tinystl::vector<int> vec2 = {1, 2, 3};
  tinystl::vector<int> vec3 = {1, 2, 4};
  tinystl::vector<int> vec4 = {1, 2};

  std::vector<int> std_vec1 = {1, 2, 3};
  std::vector<int> std_vec2 = {1, 2, 3};
  std::vector<int> std_vec3 = {1, 2, 4};
  std::vector<int> std_vec4 = {1, 2};

  SECTION("equality") {
    REQUIRE((vec1 == vec2) == (std_vec1 == std_vec2));
    REQUIRE((vec1 == vec3) == (std_vec1 == std_vec3));
    REQUIRE((vec1 == vec4) == (std_vec1 == std_vec4));
  }

  SECTION("inequality") {
    REQUIRE((vec1 != vec2) == (std_vec1 != std_vec2));
    REQUIRE((vec1 != vec3) == (std_vec1 != std_vec3));
    REQUIRE((vec1 != vec4) == (std_vec1 != std_vec4));
  }

  SECTION("less than") {
    REQUIRE((vec1 < vec2) == (std_vec1 < std_vec2));
    REQUIRE((vec1 < vec3) == (std_vec1 < std_vec3));
    REQUIRE((vec1 < vec4) == (std_vec1 < std_vec4));
  }

  SECTION("less than or equal") {
    REQUIRE((vec1 <= vec2) == (std_vec1 <= std_vec2));
    REQUIRE((vec1 <= vec3) == (std_vec1 <= std_vec3));
    REQUIRE((vec1 <= vec4) == (std_vec1 <= std_vec4));
  }

  SECTION("greater than") {
    REQUIRE((vec1 > vec2) == (std_vec1 > std_vec2));
    REQUIRE((vec1 > vec3) == (std_vec1 > std_vec3));
    REQUIRE((vec1 > vec4) == (std_vec1 > std_vec4));
  }

  SECTION("greater than or equal") {
    REQUIRE((vec1 >= vec2) == (std_vec1 >= std_vec2));
    REQUIRE((vec1 >= vec3) == (std_vec1 >= std_vec3));
    REQUIRE((vec1 >= vec4) == (std_vec1 >= std_vec4));
  }
}

TEST_CASE("Vector special cases", "[vector][special]") {
  SECTION("self assignment") {
    tinystl::vector<int> vec = {1, 2, 3, 4, 5};
    std::vector<int> std_vec = {1, 2, 3, 4, 5};
    
    vec = vec;
    std_vec = std_vec;
    
    REQUIRE(vec.size() == std_vec.size());
    for (size_t i = 0; i < vec.size(); ++i) {
      REQUIRE(vec[i] == std_vec[i]);
    }
  }

  SECTION("empty vector operations") {
    tinystl::vector<int> vec;
    std::vector<int> std_vec;
    
    REQUIRE(vec.empty() == std_vec.empty());
    REQUIRE(vec.size() == std_vec.size());
    REQUIRE(vec.begin() == vec.end());
    REQUIRE(std_vec.begin() == std_vec.end());
  }

  SECTION("large vector") {
    const size_t large_size = 10000;
    tinystl::vector<int> vec(large_size, 42);
    std::vector<int> std_vec(large_size, 42);
    
    REQUIRE(vec.size() == std_vec.size());
    REQUIRE(vec.size() == large_size);
    
    // Check a few random elements
    for (size_t i = 0; i < large_size; i += 1000) {
      REQUIRE(vec[i] == std_vec[i]);
      REQUIRE(vec[i] == 42);
    }
  }
}
