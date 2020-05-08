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

/// Determines if T is dereferencable
template <typename T, typename = void>
struct is_dereferencable : std::false_type {};

/// Determines if T is dereferencable
template <typename T>
struct is_dereferencable<T, std::void_t<decltype(*std::declval<T>())>>
    : std::true_type {};

/// Determines if T is dereferencable
template <typename T>
using is_dereferencable_t = typename is_dereferencable<T>::type;

/// Determines if T is dereferencable
template <typename T>
constexpr bool is_dereferencable_v = is_dereferencable<T>::value;

}  // namespace yat

/////////////////////////////////////////
// P0887R1 - https://wg21.link/P0887R1 //
/////////////////////////////////////////

// Check to see if stdlib support is available
#if defined(__cpp_lib_type_identity) && __cpp_lib_type_identity >= 201806L
  #define YAT_INTERNAL_USE_STD_TYPE_IDENTITY
#endif

#ifdef YAT_INTERNAL_USE_STD_TYPE_IDENTITY

namespace yat {

using std::type_identity;
using std::type_identity_t;

}  // namespace yat

#else

namespace yat {

/// Provides the member typedef type that names T (i.e., the identity
/// transformation).
///
/// The behavior of a program that adds specializations for type_identity is
/// undefined.
template <typename T>
struct type_identity {
  using type = T;
};

/// Provides the member typedef type that names T (i.e., the identity
/// transformation).
///
/// The behavior of a program that adds specializations for type_identity is
/// undefined.
template <typename T>
using type_identity_t = typename type_identity<T>::type;

}  // namespace yat

#endif  // YAT_INTERNAL_USE_STD_TYPE_IDENTITY

// Cleanup internal macros
#undef YAT_INTERNAL_USE_STD_TYPE_IDENTITY
