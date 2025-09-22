#pragma once

namespace tinystl {

// https://en.cppreference.com/w/cpp/types/integral_constant.html
template <class T, T v>
struct integral_constant {
  // Member constants
  static constexpr T value = v;

  // Member types
  using value_type = T;
  using type = integral_constant; // using injected-class-name

  // Memeber functions
  constexpr operator value_type() const noexcept { return value; }
  constexpr value_type operator()() const noexcept {
    return value;
  } // since c++14
};

// Helper alias templates
template <bool B>
using bool_constant = integral_constant<bool, B>;

// Specialization
using true_type = integral_constant<bool, true>;
using false_type = integral_constant<bool, false>;

} // namespace tinystl
