/*
 * Copyright 2020 Joe T. Sylve, Ph.D.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#pragma once

#include <memory>
#include <type_traits>

#include "features.hpp"

/////////////////////////////////////////
// P0653R2 - https://wg21.link/P0653R2 //
/////////////////////////////////////////

// Check to see if stdlib support is available
#if defined(__cpp_lib_to_address) && __cpp_lib_to_address >= 201711L
  #define YAT_INTERNAL_USE_STD_TO_ADDRESS
#endif

#ifdef YAT_INTERNAL_USE_STD_TO_ADDRESS

namespace yat {
using std::to_address;
}  // namespace yat

#else

  #ifdef YAT_IS_MSVC
// MSVC's standard library has a std::_Get_first_parameter helper type that
// doesn't have a specialization for types with type template parameters.  This
// breaks std::pointer_traits.  This specialization is a workaround.
template <template <typename, size_t> typename T, typename U, std::size_t N>
struct std::_Get_first_parameter<T<U, N> > {
  using type = U;
};
  #endif

namespace yat::detail {

template <typename, typename = void>
constexpr bool has_pt_to_address_v = false;

template <typename T>
constexpr bool has_pt_to_address_v<
    T, std::void_t<decltype(std::pointer_traits<T>::to_address)> > = true;

}  // namespace yat::detail

namespace yat {

/// Obtain the address represented by p without forming a reference to the
/// pointee.
template <typename T>
constexpr T* to_address(T* p) noexcept {
  static_assert(!std::is_function_v<T>);
  return p;
}

/// Obtain the address represented by p without forming a reference to the
/// pointee.
template <typename T>
constexpr auto to_address(const T& p) noexcept {
  if constexpr (detail::has_pt_to_address_v<T>) {
    return std::pointer_traits<T>::to_address(p);
  } else {
    return to_address(p.operator->());
  }
}

#endif  // YAT_INTERNAL_USE_STD_TO_ADDRESS

}  // namespace yat

// Cleanup internal macros
#undef YAT_INTERNAL_USE_STD_TO_ADDRESS
