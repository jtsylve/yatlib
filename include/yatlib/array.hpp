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

#include <array>

/////////////////////////////////////////
// P0325R4 - https://wg21.link/P0325R4 //
/////////////////////////////////////////

// Check to see if stdlib support is available
#if defined(__cpp_lib_to_array) && __cpp_lib_to_array >= 201907
  #define YAT_INTERNAL_USE_STD_TO_ARRAY
#endif

namespace yat::detail {
template <typename T, std::size_t N, std::size_t... Index>
constexpr std::array<std::remove_cv_t<T>, N>
to_array(T (&a)[N], std::index_sequence<Index...>) noexcept(
    std::is_nothrow_copy_constructible_v<T>) {
  return {{a[Index]...}};
}

template <typename T, std::size_t N, std::size_t... Index>
constexpr std::array<std::remove_cv_t<T>, N>
to_array(T(&&a)[N], std::index_sequence<Index...>) noexcept(
    std::is_nothrow_move_constructible_v<T>) {
  return {{std::move(a[Index])...}};
}

}  // namespace yat::detail

namespace yat {
#ifdef YAT_INTERNAL_USE_STD_TO_ARRAY
using std::to_array;
#else

/// Creates a std::array from the one dimensional built-in array a. The elements
/// of the std::array are copy-initialized from the corresponding element of a.
/// Copying or moving multidimensional built-in array is not supported.
template <typename T, std::size_t N>
constexpr std::array<std::remove_cv_t<T>, N> to_array(T (&a)[N]) noexcept(
    std::is_nothrow_copy_constructible_v<T>) {
  static_assert(!std::is_array_v<T>,
                "to_array does not accept multidimensional arrays.");
  static_assert(
      std::is_copy_constructible_v<T>,
      "to_array requires to_array requires copy constructible elements.");
  return {detail::to_array(a, std::make_index_sequence<N>())};
}

/// Creates a std::array from the one dimensional built-in array a. The elements
/// of the std::array are copy-initialized from the corresponding element of a.
/// Copying or moving multidimensional built-in array is not supported.
template <typename T, std::size_t N>
constexpr std::array<std::remove_cv_t<T>, N> to_array(T(&&a)[N]) noexcept(
    std::is_nothrow_move_constructible_v<T>) {
  static_assert(!std::is_array_v<T>,
                "to_array does not accept multidimensional arrays.");
  static_assert(
      std::is_move_constructible_v<T>,
      "to_array requires to_array requires move constructible elements.");
  return {detail::to_array(std::move(a), std::make_index_sequence<N>())};
}

#endif  // YAT_INTERNAL_USE_STD_TO_ARRAY
}  // namespace yat

#undef YAT_INTERNAL_USE_STD_TO_ARRAY
