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

namespace yat::detail {

template <typename T, typename = void>
struct has_pt_to_address : std::false_type {};

template <typename T>
struct has_pt_to_address<
    T, std::void_t<decltype(std::pointer_traits<T>::to_address(
           std::declval<const T&>()))> > : std::true_type {};

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
  if constexpr (detail::has_pt_to_address<T>::value) {
    return std::pointer_traits<T>::to_address(p);
  } else {
    return to_address(p.operator->());
  }
};

#endif  // YAT_INTERNAL_USE_STD_TO_ADDRESS

}  // namespace yat

// Cleanup internal macros
#undef YAT_INTERNAL_USE_STD_TO_ADDRESS
