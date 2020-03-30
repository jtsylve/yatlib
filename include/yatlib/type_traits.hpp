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

#include <cstddef>
#include <type_traits>

#include "features.hpp"
#include "variant.hpp"

/////////////////////////////////////////////////////////////
// Determine if a given type is a member of a std::variant //
/////////////////////////////////////////////////////////////

namespace yat {

/// Determines if a given type is a member type of a variant.
template <typename T, typename VariantType>
struct is_variant_member;

/// Determines if a given type is a member type of a variant.
template <typename T, typename... Types>
struct is_variant_member<T, yat::variant<Types...>>
    : std::disjunction<std::is_same<T, Types>...> {};

/// Determines if a given type is a member type of a variant.
template <typename T, typename VariantType>
using is_variant_member_t = typename is_variant_member<T, VariantType>::type;

/// Determines if a given type is a member type of a variant.
template <typename T, typename VariantType>
constexpr bool is_variant_member_v = is_variant_member<T, VariantType>::value;

/// Determines if T is among a the set of Types
template <typename T, typename... Types>
struct is_one_of : std::disjunction<std::is_same<T, Types>...> {};

/// Determines if T is among a the set of Types
template <typename T, typename... Types>
using is_one_of_t = typename is_one_of<T, Types...>::type;

/// Determines if T is among a the set of Types
template <typename T, typename... Types>
constexpr bool is_one_of_v = is_one_of<T, Types...>::value;

/// Determines if T is a character type
template <typename T>
struct is_char_type : is_one_of<std::decay_t<T>, char, signed char,
                                unsigned char, wchar_t, char16_t, char32_t> {};

#ifdef YAT_SUPPORTS_CPP20
template <>
struct is_char_type<char8_t> : std::true_type {};
#endif  // YAT_SUPPORTS_CPP20

/// Determines if T is a character type
template <typename T>
constexpr bool is_char_type_v = is_char_type<T>::value;

/// Determines if T is a character type
template <typename T>
using is_char_type_t = typename is_char_type<T>::type;

/// Helper type that's only enabled if T is a complete type
template <typename T, std::size_t = sizeof(T)>
using complete_t = T;

/// Determines if a given type is a complete type.  This is useful for detecting
/// specializations
template <typename...>
struct is_complete_type : std::false_type {};

/// Determines if a given type is a complete type.  This is useful for detecting
/// specializations
template <typename T>
struct is_complete_type<complete_t<T>> : std::true_type {};

/// Determines if T is a complete type
template <typename T>
constexpr bool is_complete_type_v = is_complete_type<T>::value;

/// Determines if T is a complete type
template <typename T>
using is_complete_type_t = typename is_complete_type<T>::type;

}  // namespace yat
