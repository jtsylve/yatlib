// SPDX-FileCopyrightText: © 2020-2023 Joe T. Sylve, Ph.D. <joe.sylve@gmail.com>
//
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <array>

#include "__compat.hpp"
#include "__config.hpp"

namespace yat {
inline namespace v2 {

#pragma region C++20 Library Features
inline namespace YAT_STDCPP20 cpp20 {

#pragma region std::to_array
#if YAT_HAS_STDLIB_FEATURE(to_array, 201907L)

using std::to_array;

#else

/// Creates a std::array from the one dimensional built-in array a. The elements
/// of the std::array are copy-initialized from the corresponding element of a.
/// Copying or moving multidimensional built-in array is not supported.
///
/// See https://en.cppreference.com/w/cpp/container/array/to_array
template <class T, std::size_t N>
constexpr std::array<std::remove_cv_t<T>, N> to_array(T (&a)[N]) {
  constexpr auto to_array = []<std::size_t... I>(T(&a)[N],
                                                 std::index_sequence<I...>)
      -> std::array<std::remove_cv_t<T>, N> { return {{a[I]...}}; };

  return to_array(a, std::make_index_sequence<N>{});
}

/// Creates a std::array from the one dimensional built-in array a. The elements
/// of the std::array are copy-initialized from the corresponding element of a.
/// Copying or moving multidimensional built-in array is not supported.
///
/// See https://en.cppreference.com/w/cpp/container/array/to_array
template <class T, std::size_t N>
constexpr std::array<std::remove_cv_t<T>, N> to_array(T (&&a)[N]) {
  constexpr auto to_array = []<std::size_t... I>(T(&&a)[N],
                                                 std::index_sequence<I...>)
      -> std::array<std::remove_cv_t<T>, N> { return {{std::move(a[I])...}}; };

  return to_array(std::move(a), std::make_index_sequence<N>{});
}

#endif             // YAT_HAS_STDLIB_FEATURE(to_array, 201907L)
#pragma endregion  // std::to_array

}  // namespace YAT_STDCPP20 cpp20
#pragma endregion  // C++20 Library Features

}  // namespace v2
}  // namespace yat