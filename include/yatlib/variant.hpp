// SPDX-FileCopyrightText: © 2020-2023 Joe T. Sylve, Ph.D. <joe.sylve@gmail.com>
//
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <variant>

namespace yat {
inline namespace v2 {

#pragma region Type Traits

#pragma region is_variant_alternative

/// Determines if a given type is a alternative type of a given variant.
template <typename T, typename Variant>
struct is_variant_alternative;

/// Determines if a given type is an alternative type of a given variant.
template <typename T, typename Variant>
using is_variant_alternative_t =
    typename is_variant_alternative<T, Variant>::type;

template <typename T, typename... Types>
struct is_variant_alternative<T, std::variant<Types...>>
    : std::disjunction<std::is_same<T, Types>...> {};

template <typename T, typename Variant>
inline constexpr bool is_variant_alternative_v =
    is_variant_alternative<T, Variant>::value;

#pragma endregion  // is_variant_alternative

#pragma endregion  // Type Traits

#pragma region Concepts

/// The concept variant_alternative<T, Variant> is satisfied if and only if T is
/// an alternative type of Variant.
template <typename T, typename Variant>
concept variant_alternative = is_variant_alternative_v<T, Variant>;

#pragma endregion  // Concepts

}  // namespace v2
}  // namespace yat
