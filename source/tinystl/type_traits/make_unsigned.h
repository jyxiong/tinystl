#pragma once

#include <type_traits>

namespace tinystl {
template <class T>
struct make_unsigned 
{
  // TODO: use own implementation
  using type = std::make_unsigned_t<T>;
};

template< class T >
using make_unsigned_t = typename make_unsigned<T>::type;
}
