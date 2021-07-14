/*
 * Copyright 2020-2021 Joe T. Sylve, Ph.D.
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

/// Determines if a given type is a alternative type of a given variant.
template <typename T, typename VariantType>
struct is_variant_alternative;

/// Determines if a given type is an alternative type of a given variant.
template <typename T, typename... Types>
struct is_variant_alternative<T, yat::variant<Types...>>
    : std::disjunction<std::is_same<T, Types>...> {};

/// Determines if a given type is an alternative type of a given variant.
template <typename T, typename VariantType>
using is_variant_alternative_t =
    typename is_variant_alternative<T, VariantType>::type;

/// Determines if a given type is an alternative type of a given variant.
template <typename T, typename VariantType>
inline constexpr bool is_variant_alternative_v =
    is_variant_alternative<T, VariantType>::value;

/// Determines if T is among a the set of Types
template <typename T, typename... Types>
struct is_one_of : std::disjunction<std::is_same<T, Types>...> {};

/// Determines if T is among a the set of Types
template <typename T, typename... Types>
using is_one_of_t = typename is_one_of<T, Types...>::type;

/// Determines if T is among a the set of Types
template <typename T, typename... Types>
inline constexpr bool is_one_of_v = is_one_of<T, Types...>::value;

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
inline constexpr bool is_char_type_v = is_char_type<T>::value;

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
inline constexpr bool is_dereferencable_v = is_dereferencable<T>::value;

/// Determines if an array of T is convertible to an array of U
template <typename T, typename U>
struct is_array_convertible : std::is_convertible<T (*)[], U (*)[]> {};

/// Determines if an array of T is convertible to an array of U
template <typename T, typename U>
using is_array_convertible_t = typename is_array_convertible<T, U>::type;

/// Determines if an array of T is convertible to an array of U
template <typename T, typename U>
inline constexpr bool is_array_convertible_v =
    is_array_convertible<T, U>::value;

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

/////////////////////////////////////////
// P1048R1 - https://wg21.link/P1048R1 //
/////////////////////////////////////////

// Check to see if stdlib support is available
#if defined(__cpp_lib_is_scoped_enum) && __cpp_lib_is_scoped_enum >= 202011L
  #define YAT_INTERNAL_USE_STD_IS_SCOPED_ENUM
#endif

#ifdef YAT_INTERNAL_USE_STD_IS_SCOPED_ENUM

namespace yat {

using std::is_scoped_enum;
using std::is_scoped_enum_v;

}  // namespace yat

#else

namespace yat::detail {
//
// Helper types for is_scoped_enum
//

template <typename T, bool = std::is_enum_v<T>>
struct is_scoped_enum_helper : std::false_type {};

template <typename T>
struct is_scoped_enum_helper<T, true>
    : std::negation<std::is_convertible<T, std::underlying_type_t<T>>> {};

}  // namespace yat::detail

namespace yat {

/// Checks whether T is an scoped enumeration type. Provides the member constant
/// value which is equal to true, if T is an scoped enumeration type. Otherwise,
/// value is equal to false.
template <typename T>
struct is_scoped_enum : detail::is_scoped_enum_helper<T> {};

/// true if T is an scoped enumeration type, false otherwise
template <typename T>
inline constexpr bool is_scoped_enum_v = is_scoped_enum<T>::value;

}  // namespace yat

#endif  // YAT_INTERNAL_USE_STD_IS_SCOPED_ENUM

// Cleanup internal macros
#undef YAT_INTERNAL_USE_STD_TYPE_IDENTITY
#undef YAT_INTERNAL_USE_STD_IS_SCOPED_ENUM